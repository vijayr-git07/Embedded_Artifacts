#include "hvac_flap_control.h"
#include "hvac_command_parser.h"
#include "hvac_feedback_processor.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"
#include "main.h"
#include <string.h>

// UART handle from main.c
extern UART_HandleTypeDef huart2;

typedef enum {
    FLAP_IDLE = 0,
    FLAP_MOVING_FORWARD,
    FLAP_MOVING_REVERSE,
    FLAP_FAULT
} flap_state_t;

static uint8_t target_position = 0;
static uint8_t last_target_position = 0xFF;
static flap_state_t flap_state = FLAP_IDLE;
static uint8_t completion_sequence_printed = 1;
static uint8_t last_reported_target = 0xFF;
static uint8_t last_reported_adc = 0;
static uint8_t last_led_reported = 0xFF;

static void log_uart(const char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
}

void flap_control_init(void)
{
    target_position = 0;
    last_target_position = 0xFF;
    flap_state = FLAP_IDLE;
    completion_sequence_printed = 1;
    last_reported_target = 0xFF;
    last_reported_adc = 0;
    last_led_reported = 0xFF;
}

void flap_control_set_target(uint8_t target)
{
    target_position = target;
}

static void print_ack_sequence(uint8_t pos, uint8_t adc)
{
    char buf[64];
    // 1. Command received
    snprintf(buf, sizeof(buf), "STM32: Command received - %s\r\n", command_parser_mode_name(pos));
    log_uart(buf);

    // 2. Motor moving
    if (pos > feedback_processor_get_position())
        log_uart("Motor: Moving Forward\r\n");
    else if (pos < feedback_processor_get_position())
        log_uart("Motor: Moving Reverse\r\n");
    else
        log_uart("Motor: Already at target\r\n");
}

static void print_completion_sequence(uint8_t pos, uint16_t adc)
{
    char buf[64];
    // 3. Motor: Target reached
    log_uart("Motor: Target reached\r\n");
    // 4. Sensor: Potentiometer = <adc value>
    snprintf(buf, sizeof(buf), "Sensor: Potentiometer = %u\r\n", adc);
    log_uart(buf);
    // 5. LED: <mode name> indicator ON
    snprintf(buf, sizeof(buf), "LED: %s indicator ON\r\n", command_parser_mode_name(pos));
    log_uart(buf);
}

void flap_control_process(void)
{
    uint8_t new_cmd = 0;
    uint8_t cmd_pos = 0;
    if (command_parser_get_latest(&cmd_pos, &new_cmd))
    {
        if (new_cmd)
        {
            target_position = cmd_pos;
            last_target_position = 0xFF; // force update
            completion_sequence_printed = 0;
            last_reported_target = 0xFF;
            last_led_reported = 0xFF;
            print_ack_sequence(target_position, feedback_processor_get_adc());
        }
    }

    uint8_t valid = feedback_processor_is_valid();
    uint8_t current_pos = feedback_processor_get_position();
    uint16_t adc = feedback_processor_get_adc();

    if (!valid)
    {
        // Fault: stop motor, preserve safe state
        if (flap_state != FLAP_FAULT)
        {
            motor_driver_stop();
            flap_state = FLAP_FAULT;
        }
        return;
    }

    if (target_position == current_pos)
    {
        // Already at target, ensure motor is stopped
        if (flap_state != FLAP_IDLE)
        {
            motor_driver_stop();
            flap_state = FLAP_IDLE;
        }
        if (!completion_sequence_printed)
        {
            print_completion_sequence(target_position, adc);
            completion_sequence_printed = 1;
            last_reported_target = target_position;
            last_reported_adc = adc;
            last_led_reported = target_position;
        }
        return;
    }

    // If not at target, check if in precise window
    if (feedback_processor_in_precise_window(target_position))
    {
        if (flap_state != FLAP_IDLE)
        {
            motor_driver_stop();
            flap_state = FLAP_IDLE;
        }
        if (!completion_sequence_printed)
        {
            print_completion_sequence(target_position, adc);
            completion_sequence_printed = 1;
            last_reported_target = target_position;
            last_reported_adc = adc;
            last_led_reported = target_position;
        }
        return;
    }

    // Not at target, not in window: move
    if (target_position > current_pos)
    {
        if (flap_state != FLAP_MOVING_FORWARD)
        {
            motor_driver_set_direction(1);
            motor_driver_enable();
            flap_state = FLAP_MOVING_FORWARD;
            completion_sequence_printed = 0;
        }
    }
    else if (target_position < current_pos)
    {
        if (flap_state != FLAP_MOVING_REVERSE)
        {
            motor_driver_set_direction(0);
            motor_driver_enable();
            flap_state = FLAP_MOVING_REVERSE;
            completion_sequence_printed = 0;
        }
    }
}