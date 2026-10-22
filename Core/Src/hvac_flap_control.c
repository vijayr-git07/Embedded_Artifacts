#include "hvac_flap_control.h"
#include "hvac_cmd_parser.h"
#include "hvac_feedback.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"

#define HVAC_FLAP_POSITIONS 5
#define HVAC_FLAP_TOLERANCE 0 // Position must match exactly

static uint8_t target_position = 0xFF; // Invalid at startup
static uint8_t movement_active_flag = 0;

void hvac_flap_control_init(void)
{
    target_position = 0xFF;
    movement_active_flag = 0;
}

void hvac_flap_control_process(void)
{
    uint8_t cmd, cmd_valid;
    uint8_t current_pos, feedback_valid;

    // Get latest command
    hvac_cmd_parser_get_latest(&cmd, &cmd_valid);

    // Get feedback
    current_pos = hvac_feedback_get_position();
    feedback_valid = hvac_feedback_is_valid();

    // If new valid command, update target
    if (cmd_valid && cmd < HVAC_FLAP_POSITIONS)
    {
        if (target_position != cmd)
        {
            target_position = cmd;
        }
    }

    // If feedback invalid, stop motor and preserve LED state
    if (!feedback_valid)
    {
        hvac_motor_driver_stop();
        movement_active_flag = 0;
        return;
    }

    // Compare current and target positions
    if (target_position == 0xFF)
    {
        // No valid target yet, ensure motor is stopped
        hvac_motor_driver_stop();
        movement_active_flag = 0;
        return;
    }

    if (current_pos == target_position)
    {
        // Target reached
        if (movement_active_flag)
        {
            hvac_motor_driver_stop();
            movement_active_flag = 0;
            hvac_led_status_update(current_pos, 1);
        }
        else
        {
            // Already at target, ensure LED is correct
            hvac_led_status_update(current_pos, 1);
        }
    }
    else
    {
        // Need to move
        if (current_pos < target_position)
        {
            hvac_motor_driver_set_direction(HVAC_MOTOR_DIR_CW);
            hvac_motor_driver_enable();
            movement_active_flag = 1;
        }
        else if (current_pos > target_position)
        {
            hvac_motor_driver_set_direction(HVAC_MOTOR_DIR_CCW);
            hvac_motor_driver_enable();
            movement_active_flag = 1;
        }
    }
}