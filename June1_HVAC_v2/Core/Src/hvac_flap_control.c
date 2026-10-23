#include "hvac_flap_control.h"
#include "hvac_command_parser.h"
#include "hvac_feedback_processor.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"
#include <string.h>

// Use the existing UART handle from main.c
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
static uint8_t last_flap_position = 0xFF;
static uint8_t log_command_printed = 0;
static uint8_t log_motor_start_printed = 0;
static uint8_t log_target_reached_printed = 0;
static uint8_t log_adc_printed = 0;
static uint8_t log_led_printed = 0;
static uint8_t last_reported_target = 0xFF;
static uint8_t last_invalid_reported = 0;

static void log_uart(const char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
}

void flap_control_init(void)
{
    target_position = 0;
    last_target_position = 0xFF;
    movement_active_flag = 0;
    last_flap_position = 0xFF;
    log_command_printed = 0;
    log_motor_start_printed = 0;
    log_target_reached_printed = 0;
    log_adc_printed = 0;
    log_led_printed = 0;
    last_reported_target = 0xFF;
    last_invalid_reported = 0;
}

void flap_control_set_target(uint8_t position)
{
    target_position = position;
}

void flap_control_process(void)
{
    uint8_t cmd_pos, cmd_is_new;
    uint8_t valid_cmd = command_parser_get_latest(&cmd_pos, &cmd_is_new);

    // Handle invalid command logging
    if (!valid_cmd)
    {
        if (!last_invalid_reported && cmd_is_new)
        {
            log_uart("STM32: Invalid command ignored\r\n");
            last_invalid_reported = 1;
        }
        return;
    }
    else
    {
        last_invalid_reported = 0;
    }

    // On new command, update target and reset log flags
    if (cmd_is_new && cmd_pos <= 5)
    {
        target_position = cmd_pos;
        last_target_position = target_position;
        movement_active_flag = 0;
        log_command_printed = 0;
        log_motor_start_printed = 0;
        log_target_reached_printed = 0;
        log_adc_printed = 0;
        log_led_printed = 0;
        last_reported_target = 0xFF;
    }

    // Only process if feedback is valid
    if (!feedback_processor_is_valid())
    {
        motor_driver_stop();
        return;
    }

    uint8_t current_position = feedback_processor_get_position();

    // If already at target, ensure motor is stopped and log completion if not already done
    if (current_position == target_position)
    {
        motor_driver_stop();
        movement_active_flag = 0;

        if (!log_command_printed)
        {
            char buf[64];
            snprintf(buf, sizeof(buf), "STM32: Command received - %s\r\n", mode_names[target_position]);
            log_uart(buf);
            log_command_printed = 1;
        }
        if (!log_motor_start_printed)
        {
            // If already at target, no movement, so skip motor start log
            log_motor_start_printed = 1;
        }
        if (!log_target_reached_printed)
        {
            log_uart("Motor: Target reached\r\n");
            log_target_reached_printed = 1;
        }
        if (!log_adc_printed)
        {
            char buf[48];
            snprintf(buf, sizeof(buf), "Sensor: Potentiometer = %u\r\n", feedback_processor_get_adc());
            log_uart(buf);
            log_adc_printed = 1;
        }
        if (!log_led_printed)
        {
            char buf[48];
            snprintf(buf, sizeof(buf), "LED: %s indicator ON\r\n", mode_names[target_position]);
            log_uart(buf);
            log_led_printed = 1;
        }
        last_reported_target = target_position;
        return;
    }

    // If not at target, check if we need to move
    if (current_position != target_position)
    {
        // Only log command and motor start once per command
        if (!log_command_printed)
        {
            char buf[64];
            snprintf(buf, sizeof(buf), "STM32: Command received - %s\r\n", mode_names[target_position]);
            log_uart(buf);
            log_command_printed = 1;
        }

        // Determine direction
        if (target_position > current_position)
        {
            // Move Forward (open)
            motor_driver_set_direction(1);
            motor_driver_enable();
            movement_active_flag = 1;
            if (!log_motor_start_printed)
            {
                log_uart("Motor: Moving Forward\r\n");
                log_motor_start_printed = 1;
            }
        }
        else if (target_position < current_position)
        {
            // Move Reverse (close)
            motor_driver_set_direction(0);
            motor_driver_enable();
            movement_active_flag = 1;
            if (!log_motor_start_printed)
            {
                log_uart("Motor: Moving Reverse\r\n");
                log_motor_start_printed = 1;
            }
        }
        else
        {
            // Should not happen, but stop motor just in case
            motor_driver_stop();
            movement_active_flag = 0;
        }

        // Check if we are in the precise stop window for the target
        if (feedback_processor_in_precise_stop_window(target_position))
        {
            motor_driver_stop();
            movement_active_flag = 0;

            if (!log_target_reached_printed)
            {
                log_uart("Motor: Target reached\r\n");
                log_target_reached_printed = 1;
            }
            if (!log_adc_printed)
            {
                char buf[48];
                snprintf(buf, sizeof(buf), "Sensor: Potentiometer = %u\r\n", feedback_processor_get_adc());
                log_uart(buf);
                log_adc_printed = 1;
            }
            if (!log_led_printed)
            {
                char buf[48];
                snprintf(buf, sizeof(buf), "LED: %s indicator ON\r\n", mode_names[target_position]);
                log_uart(buf);
                log_led_printed = 1;
            }
            last_reported_target = target_position;
        }
    }
    last_flap_position = current_position;
}