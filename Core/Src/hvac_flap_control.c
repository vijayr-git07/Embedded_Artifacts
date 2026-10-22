#include "hvac_flap_control.h"
#include "hvac_cmd_parser.h"
#include "hvac_feedback.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"

#define FLAP_POSITION_TOLERANCE 0 // Only exact match

static uint8_t target_position = HVAC_FLAP_POS_INVALID;
static uint8_t movement_active_flag = 0;
static uint8_t last_led_position = HVAC_FLAP_POS_INVALID;

void hvac_flap_control_init(void)
{
    target_position = HVAC_FLAP_POS_INVALID;
    movement_active_flag = 0;
    last_led_position = HVAC_FLAP_POS_INVALID;
}

void hvac_flap_control_set_target(uint8_t pos)
{
    if (pos <= HVAC_FLAP_POS_MAX)
        target_position = pos;
}

void hvac_flap_control_process(void)
{
    uint8_t cmd_valid = 0;
    uint8_t cmd = hvac_cmd_parser_get_latest(&cmd_valid);

    if (cmd_valid && cmd <= HVAC_FLAP_POS_MAX)
    {
        target_position = cmd;
    }

    uint8_t feedback_valid = 0;
    uint8_t current_pos = hvac_feedback_get_position(&feedback_valid);

    if (!feedback_valid)
    {
        hvac_motor_driver_stop();
        movement_active_flag = 0;
        // Do not update LEDs on invalid feedback
        return;
    }

    if (target_position == HVAC_FLAP_POS_INVALID)
    {
        hvac_motor_driver_stop();
        movement_active_flag = 0;
        return;
    }

    if (current_pos == target_position)
    {
        hvac_motor_driver_stop();
        if (last_led_position != current_pos)
        {
            hvac_led_status_update(current_pos, 1);
            last_led_position = current_pos;
        }
        movement_active_flag = 0;
    }
    else if (current_pos < target_position)
    {
        hvac_motor_driver_set_direction(MOTOR_DIR_CW);
        hvac_motor_driver_enable(1);
        movement_active_flag = 1;
    }
    else if (current_pos > target_position)
    {
        hvac_motor_driver_set_direction(MOTOR_DIR_CCW);
        hvac_motor_driver_enable(1);
        movement_active_flag = 1;
    }
}