#include "hvac_system_init.h"
#include "hvac_command_parser.h"
#include "hvac_feedback_processor.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"
#include "hvac_flap_control.h"

void system_init(void)
{
    command_parser_init();
    feedback_processor_init();
    motor_driver_init();
    led_status_init();
    flap_control_init();

    // Read initial feedback and update LED state
    feedback_processor_update();
    led_status_update(feedback_processor_get_position());
}