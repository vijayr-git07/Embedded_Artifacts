#include "hvac_flap_control.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"
#include <string.h>

// UART handle provided by CubeMX project
extern UART_HandleTypeDef huart2;

// Mode names for logging
static const char *mode_names[6] = {
    "Initial Mode",         // 0
    "Face Mode",            // 1
    "Face and Foot Mode",   // 2
    "Foot Mode",            // 3
    "Defrost Mode",         // 4
    "Full Open Mode"        // 5
};

static hvac_command_t last_reported_target = HVAC_CMD_NONE;
static uint8_t command_log_printed = 0;
static uint8_t motor_start_logged = 0;
static uint8_t target_reached_logged = 0;
static uint8_t completion_sequence_printed = 0;
static flap_move_dir_t last_move_dir = FLAP_MOVE_NONE;

static void log_once(const char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint16_t)strlen(msg), 100);
}

static void log_mode_name(hvac_command_t cmd)
{
    if (cmd <= HVAC_CMD_POS_5) {
        char buf[64];
        snprintf(buf, sizeof(buf), "STM32: Command received - %s\r\n", mode_names[cmd]);
        log_once(buf);
    }
}

static void log_motor_start(flap_move_dir_t dir)
{
    if (dir == FLAP_MOVE_FORWARD) {
        log_once("Motor: Moving Forward\r\n");
    } else if (dir == FLAP_MOVE_REVERSE) {
        log_once("Motor: Moving Reverse\r\n");
    }
}

static void log_motor_stop(void)
{
    log_once("Motor: Target reached\r\n");
}

static void log_adc_value(uint16_t adc)
{
    char buf[48];
    snprintf(buf, sizeof(buf), "Sensor: Potentiometer = %u\r\n", adc);
    log_once(buf);
}

static void log_led_on(hvac_command_t cmd)
{
    if (cmd == HVAC_CMD_POS_0) {
        log_once("LED: All position indicators OFF\r\n");
    } else if (cmd <= HVAC_CMD_POS_5) {
        char buf[48];
        snprintf(buf, sizeof(buf), "LED: %s indicator ON\r\n", mode_names[cmd]);
        log_once(buf);
    }
}

void flap_control_init(void)
{
    last_reported_target = HVAC_CMD_NONE;
    command_log_printed = 0;
    motor_start_logged = 0;
    target_reached_logged = 0;
    completion_sequence_printed = 0;
    last_move_dir = FLAP_MOVE_NONE;
}

void flap_control_process(void)
{
    hvac_command_t target = command_parser_get_latest();
    flap_position_t current = feedback_processor_get_position();
    uint8_t feedback_valid = feedback_processor_is_valid();

    // Only process if command is valid
    if (target == HVAC_CMD_NONE || target > HVAC_CMD_POS_5) {
        // No valid command, ensure safe state
        motor_driver_stop();
        led_status_update(FLAP_POS_INVALID);
        // Reset log gating if no command
        last_reported_target = HVAC_CMD_NONE;
        command_log_printed = 0;
        motor_start_logged = 0;
        target_reached_logged = 0;
        completion_sequence_printed = 0;
        last_move_dir = FLAP_MOVE_NONE;
        return;
    }

    // Detect new command
    if (command_parser_is_new() || target != last_reported_target) {
        // New command received
        log_mode_name(target);
        command_log_printed = 1;
        motor_start_logged = 0;
        target_reached_logged = 0;
        completion_sequence_printed = 0;
        last_reported_target = target;
        last_move_dir = FLAP_MOVE_NONE;
        command_parser_clear_new_flag();
    }

    // If feedback invalid, stop motor and preserve LED state
    if (!feedback_valid) {
        motor_driver_stop();
        // Do not update LEDs
        return;
    }

    // If already at target, ensure motor is stopped and LED is correct
    if (current == (flap_position_t)target) {
        // Check precise stop window
        if (feedback_processor_in_precise_window((flap_position_t)target)) {
            motor_driver_stop();
            if (!target_reached_logged) {
                log_motor_stop();
                target_reached_logged = 1;
            }
            if (!completion_sequence_printed) {
                log_adc_value(feedback_processor_get_adc());
                log_led_on(target);
                completion_sequence_printed = 1;
            }
            led_status_update(current);
        } else {
            // Not yet in precise window, keep moving
            // Determine direction to nudge into window
            // For simplicity, do nothing (motor should already be stopped)
            motor_driver_stop();
        }
        return;
    }

    // Not at target, need to move
    // Determine direction
    flap_move_dir_t move_dir = FLAP_MOVE_NONE;
    if (current < (flap_position_t)target) {
        move_dir = FLAP_MOVE_FORWARD;
    } else if (current > (flap_position_t)target) {
        move_dir = FLAP_MOVE_REVERSE;
    }

    // Only log motor start once per command
    if (!motor_start_logged && move_dir != FLAP_MOVE_NONE) {
        log_motor_start(move_dir);
        motor_start_logged = 1;
        target_reached_logged = 0;
        completion_sequence_printed = 0;
        last_move_dir = move_dir;
    }

    // Move motor in correct direction
    if (move_dir == FLAP_MOVE_FORWARD) {
        motor_driver_set_direction(MOTOR_DIR_FORWARD);
        motor_driver_enable();
    } else if (move_dir == FLAP_MOVE_REVERSE) {
        motor_driver_set_direction(MOTOR_DIR_REVERSE);
        motor_driver_enable();
    } else {
        motor_driver_stop();
    }

    // Check if reached precise stop window for target
    if (feedback_processor_in_precise_window((flap_position_t)target)) {
        motor_driver_stop();
        if (!target_reached_logged) {
            log_motor_stop();
            target_reached_logged = 1;
        }
        if (!completion_sequence_printed) {
            log_adc_value(feedback_processor_get_adc());
            log_led_on(target);
            completion_sequence_printed = 1;
        }
        led_status_update((flap_position_t)target);
    } else {
        // Not yet reached, update LED to current position
        led_status_update(current);
    }
}