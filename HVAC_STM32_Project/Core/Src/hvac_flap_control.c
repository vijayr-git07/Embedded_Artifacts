#include "hvac_flap_control.h"
#include "hvac_command_parser.h"
#include "hvac_feedback_processor.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"

// UART handle for logging
extern UART_HandleTypeDef huart2;

// Mode names for logging
static const char *mode_names[6] = {
    "Initial Mode",      // 0
    "Face Mode",         // 1
    "Face and Foot Mode",// 2
    "Foot Mode",         // 3
    "Defrost Mode",      // 4
    "Full Open",         // 5
};

static uint8_t target_position = 0;
static uint8_t last_target_position = 0xFF;
static uint8_t movement_active_flag = 0;
static uint8_t completion_sequence_printed = 1;
static uint8_t last_reported_target = 0xFF;
static uint8_t last_reported_adc = 0;
static uint8_t last_feedback_valid = 0;

static void log_once(const char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint16_t)strlen(msg), 100);
}

static void log_mode_ack(uint8_t pos)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "STM32: Command received - %s\r\n", mode_names[pos]);
    log_once(buf);
}

static void log_motor_moving(uint8_t dir)
{
    const char *msg = (dir == 0) ? "Motor: Moving Forward\r\n" : "Motor: Moving Reverse\r\n";
    log_once(msg);
}

static void log_motor_target_reached(void)
{
    log_once("Motor: Target reached\r\n");
}

static void log_sensor_adc(uint16_t adc)
{
    char buf[48];
    snprintf(buf, sizeof(buf), "Sensor: Potentiometer = %u\r\n", adc);
    log_once(buf);
}

static void log_led_on(uint8_t pos)
{
    char buf[48];
    snprintf(buf, sizeof(buf), "LED: %s indicator ON\r\n", mode_names[pos]);
    log_once(buf);
}

void flap_control_init(void)
{
    target_position = 0;
    last_target_position = 0xFF;
    movement_active_flag = 0;
    completion_sequence_printed = 1;
    last_reported_target = 0xFF;
    last_reported_adc = 0;
    last_feedback_valid = 0;
}

void flap_control_process(void)
{
    uint8_t cmd_pos, is_new_cmd;
    if (command_parser_get_latest(&cmd_pos, &is_new_cmd)) {
        if (is_new_cmd && cmd_pos <= 5) {
            target_position = cmd_pos;
            completion_sequence_printed = 0;
            last_reported_target = 0xFF;
            last_reported_adc = 0;
            last_feedback_valid = 0;
        }
    }

    uint8_t feedback_pos, feedback_valid;
    uint16_t adc_val;
    feedback_processor_get_position(&feedback_pos, &adc_val, &feedback_valid);

    // If feedback is invalid, stop motor and preserve safe state
    if (!feedback_valid) {
        motor_driver_stop();
        movement_active_flag = 0;
        return;
    }

    // If already at target, ensure motor is stopped and print completion logs if not already done
    if (feedback_pos == target_position) {
        motor_driver_stop();
        if (!completion_sequence_printed) {
            if (last_reported_target != target_position) {
                log_mode_ack(target_position);
                last_reported_target = target_position;
            }
            log_motor_target_reached();
            log_sensor_adc(adc_val);
            log_led_on(target_position);
            completion_sequence_printed = 1;
        }
        movement_active_flag = 0;
        return;
    }

    // If not at target, move in correct direction
    if (feedback_pos < target_position) {
        // Move Forward (open)
        if (!movement_active_flag || last_target_position != target_position) {
            log_mode_ack(target_position);
            log_motor_moving(0);
            movement_active_flag = 1;
            last_target_position = target_position;
        }
        motor_driver_set_direction(0); // 0 = forward
        motor_driver_enable();
    } else if (feedback_pos > target_position) {
        // Move Reverse (close)
        if (!movement_active_flag || last_target_position != target_position) {
            log_mode_ack(target_position);
            log_motor_moving(1);
            movement_active_flag = 1;
            last_target_position = target_position;
        }
        motor_driver_set_direction(1); // 1 = reverse
        motor_driver_enable();
    }

    // Check if in precise stop window for target
    extern uint8_t feedback_processor_is_in_precise_window(uint8_t position);
    if (feedback_processor_is_in_precise_window(target_position)) {
        motor_driver_stop();
        if (!completion_sequence_printed) {
            log_motor_target_reached();
            log_sensor_adc(adc_val);
            log_led_on(target_position);
            completion_sequence_printed = 1;
        }
        movement_active_flag = 0;
    }
}

void flap_control_set_target(uint8_t position)
{
    // Set the target position for the flap control
    if (position <= 5) {
        target_position = position;
        completion_sequence_printed = 0;
        last_reported_target = 0xFF;
        last_reported_adc = 0;
        last_feedback_valid = 0;
    }
}