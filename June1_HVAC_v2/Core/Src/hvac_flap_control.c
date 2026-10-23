#include "hvac_flap_control.h"
#include "hvac_command_parser.h"
#include "hvac_feedback_processor.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"
#include "main.h"

// UART handle from main.c
extern UART_HandleTypeDef huart2;

static uint8_t target_position = 0;
static uint8_t movement_active_flag = 0;
static uint8_t last_reported_target = 0xFF;
static uint8_t completion_sequence_printed = 1;
static uint8_t last_motor_dir = 0xFF;
static uint8_t last_led_pos = 0xFF;
static uint8_t last_adc_reported = 0;
static uint8_t last_feedback_valid = 1;

static const char *mode_names[6] = {
    "Initial Mode",        // 0
    "Face Mode",           // 1
    "Face and Foot Mode",  // 2
    "Foot Mode",           // 3
    "Foot and Defrost",    // 4
    "Defrost Mode"         // 5
};

static void log_motor_start(uint8_t dir)
{
    const char *msg = (dir == 0) ? "Motor: Moving Forward\r\n" : "Motor: Moving Reverse\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
}

static void log_motor_stop(void)
{
    const char *msg = "Motor: Target reached\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
}

static void log_adc(uint16_t adc)
{
    char msg[48];
    int len = snprintf(msg, sizeof(msg), "Sensor: Potentiometer = %u\r\n", adc);
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, 100);
}

static void log_led(uint8_t pos)
{
    if (pos <= 5) {
        char msg[48];
        int len = snprintf(msg, sizeof(msg), "LED: %s indicator ON\r\n", mode_names[pos]);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, 100);
    }
}

void flap_control_init(void)
{
    target_position = 0;
    movement_active_flag = 0;
    last_reported_target = 0xFF;
    completion_sequence_printed = 1;
    last_motor_dir = 0xFF;
    last_led_pos = 0xFF;
    last_adc_reported = 0;
    last_feedback_valid = 1;
}

void flap_control_set_target(uint8_t target)
{
    target_position = target;
}

void flap_control_process(void)
{
    uint8_t cmd, is_new_cmd;
    if (command_parser_get_latest(&cmd, &is_new_cmd)) {
        if (is_new_cmd) {
            target_position = cmd;
            movement_active_flag = 0;
            completion_sequence_printed = 0;
            last_reported_target = 0xFF;
            last_motor_dir = 0xFF;
            last_led_pos = 0xFF;
            last_adc_reported = 0;
            last_feedback_valid = 1;
        }
    }

    uint8_t feedback_valid;
    uint8_t current_pos;
    uint16_t adc_val;
    feedback_processor_get_position(&current_pos, &adc_val, &feedback_valid);

    // Fault: invalid feedback disables motor
    if (!feedback_valid) {
        motor_driver_stop();
        movement_active_flag = 0;
        last_motor_dir = 0xFF;
        return;
    }

    // If already at target, ensure motor is stopped and print completion sequence if not already done
    if (current_pos == target_position && feedback_processor_is_in_stop_window(target_position)) {
        if (movement_active_flag) {
            motor_driver_stop();
            movement_active_flag = 0;
            log_motor_stop();
            log_adc(adc_val);
            log_led(target_position);
            completion_sequence_printed = 1;
            last_reported_target = target_position;
            last_adc_reported = adc_val;
            last_led_pos = target_position;
        } else if (!completion_sequence_printed) {
            // If not moving but completion not logged (e.g. startup)
            log_motor_stop();
            log_adc(adc_val);
            log_led(target_position);
            completion_sequence_printed = 1;
            last_reported_target = target_position;
            last_adc_reported = adc_val;
            last_led_pos = target_position;
        }
        return;
    }

    // If not at target, move in correct direction
    if (current_pos != target_position) {
        uint8_t dir = (target_position > current_pos) ? 0 : 1; // 0: Forward, 1: Reverse
        if (!movement_active_flag || last_motor_dir != dir) {
            motor_driver_set_direction(dir);
            motor_driver_enable();
            log_motor_start(dir);
            movement_active_flag = 1;
            last_motor_dir = dir;
            completion_sequence_printed = 0;
        }
        // Do not log ADC or LED until target reached
    }
}