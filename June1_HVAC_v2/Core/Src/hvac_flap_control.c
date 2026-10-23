#include "hvac_flap_control.h"
#include "hvac_feedback_processor.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"
#include "hvac_command_parser.h"
#include "main.h"

// UART handle from main.c
extern UART_HandleTypeDef huart2;

typedef enum {
    FLAP_IDLE = 0,
    FLAP_MOVING,
    FLAP_FAULT
} flap_state_t;

static uint8_t target_position = 0;
static uint8_t last_target_position = 0xFF;
static flap_state_t flap_state = FLAP_IDLE;
static uint8_t completion_sequence_printed = 1;
static uint8_t last_reported_target = 0xFF;
static uint8_t last_reported_adc = 0;
static uint8_t last_reported_led = 0xFF;

static void log_motor_start(uint8_t dir);
static void log_motor_stop(void);
static void log_sensor_adc(uint16_t adc);
static void log_led(uint8_t pos);

void flap_control_init(void)
{
    target_position = 0;
    last_target_position = 0xFF;
    flap_state = FLAP_IDLE;
    completion_sequence_printed = 1;
    last_reported_target = 0xFF;
    last_reported_adc = 0;
    last_reported_led = 0xFF;
}

void flap_control_set_target(uint8_t target)
{
    if (target < 6)
    {
        target_position = target;
        completion_sequence_printed = 0;
        last_reported_target = 0xFF;
        last_reported_adc = 0;
        last_reported_led = 0xFF;
    }
}

void flap_control_process(void)
{
    uint8_t current_pos, valid;
    uint16_t adc;
    feedback_processor_get_position(&current_pos, &adc, &valid);

    // Only process if feedback is valid
    if (!valid)
    {
        motor_driver_stop();
        flap_state = FLAP_FAULT;
        return;
    }

    // If new command, print log sequence
    if (target_position != last_target_position)
    {
        hvac_command_parser_log_ack(target_position);
        last_target_position = target_position;
        completion_sequence_printed = 0;
        last_reported_target = 0xFF;
        last_reported_adc = 0;
        last_reported_led = 0xFF;
    }

    // If already at target, ensure stopped and print completion logs once
    if (current_pos == target_position && feedback_processor_in_stop_window(target_position))
    {
        if (flap_state != FLAP_IDLE)
        {
            motor_driver_stop();
            flap_state = FLAP_IDLE;
        }
        if (!completion_sequence_printed)
        {
            log_motor_stop();
            log_sensor_adc(adc);
            log_led(target_position);
            completion_sequence_printed = 1;
            last_reported_target = target_position;
            last_reported_adc = adc;
            last_reported_led = target_position;
        }
        return;
    }

    // If not at target, move in correct direction
    if (current_pos < target_position)
    {
        if (flap_state != FLAP_MOVING || motor_driver_get_direction() != MOTOR_DIR_FORWARD)
        {
            motor_driver_set_direction(MOTOR_DIR_FORWARD);
            motor_driver_enable();
            log_motor_start(MOTOR_DIR_FORWARD);
            flap_state = FLAP_MOVING;
        }
        completion_sequence_printed = 0;
    }
    else if (current_pos > target_position)
    {
        if (flap_state != FLAP_MOVING || motor_driver_get_direction() != MOTOR_DIR_REVERSE)
        {
            motor_driver_set_direction(MOTOR_DIR_REVERSE);
            motor_driver_enable();
            log_motor_start(MOTOR_DIR_REVERSE);
            flap_state = FLAP_MOVING;
        }
        completion_sequence_printed = 0;
    }
    else
    {
        // Should not reach here, but stop as safety
        motor_driver_stop();
        flap_state = FLAP_IDLE;
    }

    // If in stop window during movement, stop and print logs
    if (feedback_processor_in_stop_window(target_position))
    {
        motor_driver_stop();
        flap_state = FLAP_IDLE;
        if (!completion_sequence_printed)
        {
            log_motor_stop();
            log_sensor_adc(adc);
            log_led(target_position);
            completion_sequence_printed = 1;
            last_reported_target = target_position;
            last_reported_adc = adc;
            last_reported_led = target_position;
        }
    }
}

static void log_motor_start(uint8_t dir)
{
    static uint8_t last_dir = 0xFF;
    if (last_dir == dir)
        return;
    last_dir = dir;
    const char *msg = (dir == MOTOR_DIR_FORWARD) ? "Motor: Moving Forward\r\n" : "Motor: Moving Reverse\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint16_t)strlen(msg), 100);
}

static void log_motor_stop(void)
{
    static uint8_t printed = 0;
    if (printed)
        return;
    printed = 1;
    const char *msg = "Motor: Target reached\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint16_t)strlen(msg), 100);
}

static void log_sensor_adc(uint16_t adc)
{
    static uint16_t last_adc = 0xFFFF;
    if (last_adc == adc)
        return;
    last_adc = adc;
    char msg[48];
    snprintf(msg, sizeof(msg), "Sensor: Potentiometer = %u\r\n", adc);
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint16_t)strlen(msg), 100);
}

static void log_led(uint8_t pos)
{
    static uint8_t last_led = 0xFF;
    if (last_led == pos)
        return;
    last_led = pos;
    const char *mode = hvac_command_parser_mode_name(pos);
    char msg[48];
    snprintf(msg, sizeof(msg), "LED: %s indicator ON\r\n", mode);
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint16_t)strlen(msg), 100);
}