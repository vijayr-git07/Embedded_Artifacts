#include "hvac_flap_control.h"
#include "hvac_command_parser.h"
#include "hvac_feedback_processor.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"
#include "main.h"
#include <string.h>

// UART handle from main.c
extern UART_HandleTypeDef huart2;

#define FLAP_POSITIONS 6

typedef enum {
    FLAP_IDLE = 0,
    FLAP_MOVING,
    FLAP_FAULT
} flap_state_t;

static uint8_t target_position = 0;
static uint8_t last_target_position = 0xFF;
static flap_state_t flap_state = FLAP_IDLE;
static uint8_t last_flap_position = 0xFF;
static uint8_t completion_sequence_printed = 1;
static uint8_t last_reported_target = 0xFF;

static void log_uart(const char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint16_t)strlen(msg), 100);
}

static const char *mode_names[6] = {
    "Initial Mode",      // 0
    "Face Mode",         // 1
    "Face and Foot Mode",// 2
    "Foot Mode",         // 3
    "Foot and Defrost",  // 4
    "Defrost Mode"       // 5
};

void flap_control_init(void)
{
    target_position = 0;
    last_target_position = 0xFF;
    flap_state = FLAP_IDLE;
    last_flap_position = 0xFF;
    completion_sequence_printed = 1;
    last_reported_target = 0xFF;
}

void flap_control_set_target(uint8_t target)
{
    if (target < FLAP_POSITIONS)
        target_position = target;
}

static void print_ack_sequence(uint8_t mode, uint8_t direction, uint16_t adc)
{
    char buf[64];
    // 1. Command received
    snprintf(buf, sizeof(buf), "STM32: Command received - %s\r\n", mode_names[mode]);
    log_uart(buf);

    // 2. Motor moving
    if (direction == 1)
        log_uart("Motor: Moving Forward\r\n");
    else if (direction == 2)
        log_uart("Motor: Moving Reverse\r\n");

    // 3. Motor target reached
    log_uart("Motor: Target reached\r\n");

    // 4. Sensor: Potentiometer value
    snprintf(buf, sizeof(buf), "Sensor: Potentiometer = %u\r\n", adc);
    log_uart(buf);

    // 5. LED: Mode indicator ON
    snprintf(buf, sizeof(buf), "LED: %s indicator ON\r\n", mode_names[mode]);
    log_uart(buf);
}

void flap_control_process(void)
{
    uint8_t cmd, is_new_cmd;
    if (command_parser_get_latest(&cmd, &is_new_cmd))
    {
        if (is_new_cmd)
        {
            target_position = cmd;
            completion_sequence_printed = 0;
            last_reported_target = 0xFF;
        }
    }

    uint8_t feedback_valid = feedback_processor_is_valid();
    uint8_t current_pos = 0xFF;
    if (feedback_valid)
        feedback_processor_get_position(&current_pos);

    // Fault: invalid feedback
    if (!feedback_valid)
    {
        motor_driver_stop();
        flap_state = FLAP_FAULT;
        return;
    }

    // If already at target, ensure motor is stopped and print completion sequence if not already done
    if (current_pos == target_position)
    {
        motor_driver_stop();
        flap_state = FLAP_IDLE;
        if (!completion_sequence_printed && last_reported_target != target_position)
        {
            print_ack_sequence(target_position, 0, feedback_processor_get_adc());
            completion_sequence_printed = 1;
            last_reported_target = target_position;
        }
        led_status_set_position(current_pos);
        return;
    }

    // If not at target, check if in precise stop window
    if (feedback_processor_in_stop_window(target_position))
    {
        motor_driver_stop();
        flap_state = FLAP_IDLE;
        if (!completion_sequence_printed && last_reported_target != target_position)
        {
            print_ack_sequence(target_position, 0, feedback_processor_get_adc());
            completion_sequence_printed = 1;
            last_reported_target = target_position;
        }
        led_status_set_position(target_position);
        return;
    }

    // If not at target, move in correct direction
    if (current_pos < target_position)
    {
        motor_driver_set_direction(1); // Forward
        motor_driver_enable();
        flap_state = FLAP_MOVING;
        if (!completion_sequence_printed && last_reported_target != target_position)
        {
            // Only print "Moving Forward" once per new command
            char buf[64];
            snprintf(buf, sizeof(buf), "STM32: Command received - %s\r\n", mode_names[target_position]);
            log_uart(buf);
            log_uart("Motor: Moving Forward\r\n");
            completion_sequence_printed = 0;
        }
    }
    else if (current_pos > target_position)
    {
        motor_driver_set_direction(2); // Reverse
        motor_driver_enable();
        flap_state = FLAP_MOVING;
        if (!completion_sequence_printed && last_reported_target != target_position)
        {
            char buf[64];
            snprintf(buf, sizeof(buf), "STM32: Command received - %s\r\n", mode_names[target_position]);
            log_uart(buf);
            log_uart("Motor: Moving Reverse\r\n");
            completion_sequence_printed = 0;
        }
    }
    // LED update is handled in led_status_update()
}