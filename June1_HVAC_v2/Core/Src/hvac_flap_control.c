#include "hvac_flap_control.h"
#include "hvac_command_parser.h"
#include "hvac_feedback_processor.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

// UART handle from main.c
extern UART_HandleTypeDef huart2;

static uint8_t target_position = 0;
static uint8_t movement_active_flag = 0;
static uint8_t last_reported_target = 0xFF;
static uint8_t completion_sequence_printed = 1;
static uint8_t last_reported_adc = 0;
static uint8_t last_led_reported = 0xFF;

static void uart_log(const char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint16_t)strlen(msg), 100);
}

static void log_motor_direction(uint8_t dir)
{
    if (dir == MOTOR_DIR_FORWARD)
        uart_log("Motor: Moving Forward\r\n");
    else if (dir == MOTOR_DIR_REVERSE)
        uart_log("Motor: Moving Reverse\r\n");
}

static void log_motor_stop(void)
{
    uart_log("Motor: Target reached\r\n");
}

static void log_adc(uint16_t adc)
{
    char buf[48];
    snprintf(buf, sizeof(buf), "Sensor: Potentiometer = %u\r\n", adc);
    uart_log(buf);
}

static void log_led(uint8_t pos)
{
    char buf[48];
    snprintf(buf, sizeof(buf), "LED: %s indicator ON\r\n", command_parser_mode_name(pos));
    uart_log(buf);
}

void flap_control_init(void)
{
    target_position = 0;
    movement_active_flag = 0;
    last_reported_target = 0xFF;
    completion_sequence_printed = 1;
    last_reported_adc = 0;
    last_led_reported = 0xFF;
}

void flap_control_set_target(uint8_t target)
{
    target_position = target;
}

void flap_control_process(void)
{
    uint8_t cmd_pos, is_new_cmd;
    if (command_parser_get_latest(&cmd_pos, &is_new_cmd))
    {
        if (is_new_cmd && cmd_pos <= 5)
        {
            target_position = cmd_pos;
            completion_sequence_printed = 0;
            last_reported_target = 0xFF;
            last_led_reported = 0xFF;
            command_parser_log_command(cmd_pos);
        }
    }

    uint8_t feedback_valid = feedback_processor_is_valid();
    uint8_t current_pos = 0;
    uint16_t adc_val = 0;
    feedback_processor_get_position(&current_pos, &adc_val, NULL);

    // Fault: invalid feedback
    if (!feedback_valid)
    {
        motor_driver_stop();
        movement_active_flag = 0;
        return;
    }

    // If already at target (precise window), stop motor
    if (feedback_processor_in_precise_window(target_position))
    {
        if (movement_active_flag)
        {
            motor_driver_stop();
            movement_active_flag = 0;
            if (!completion_sequence_printed)
            {
                log_motor_stop();
                log_adc(adc_val);
                log_led(target_position);
                completion_sequence_printed = 1;
                last_reported_target = target_position;
                last_led_reported = target_position;
            }
        }
        return;
    }

    // If not at target, move
    if (current_pos != target_position)
    {
        // Determine direction
        uint8_t dir = (current_pos < target_position) ? MOTOR_DIR_FORWARD : MOTOR_DIR_REVERSE;
        if (!movement_active_flag)
        {
            motor_driver_set_direction(dir);
            motor_driver_enable();
            log_motor_direction(dir);
            movement_active_flag = 1;
            completion_sequence_printed = 0;
        }
        else
        {
            // Already moving, check direction
            motor_driver_set_direction(dir);
        }
    }
    else
    {
        // At coarse position, but not precise window: stop for safety
        motor_driver_stop();
        movement_active_flag = 0;
    }
}