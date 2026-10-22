#include "hvac_app.h"
#include "hvac_cmd_parser.h"
#include "hvac_feedback.h"
#include "hvac_flap_control.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"

void hvac_app_init(void)
{
    hvac_cmd_parser_init();
    hvac_feedback_init();
    hvac_motor_driver_init();
    hvac_led_status_init();
    hvac_flap_control_init();

    // Read initial feedback and update LED
    hvac_feedback_update();
    hvac_led_status_update(hvac_feedback_get_position(), hvac_feedback_is_valid());
}

void hvac_app_task(void)
{
    // 1. Process UART commands
    hvac_cmd_parser_process();

    // 2. Read ADC feedback
    hvac_feedback_update();

    // 3. Execute flap control logic (includes motor and LED update)
    hvac_flap_control_process();
}