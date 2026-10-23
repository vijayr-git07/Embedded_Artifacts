#include "hvac_app.h"
#include "hvac_command_parser.h"
#include "hvac_feedback_processor.h"
#include "hvac_flap_control.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"
#include "hvac_system_init.h"

void hvac_app_init(void)
{
    system_init();
}

void hvac_app_task(void)
{
    command_parser_process();
    feedback_processor_update();
    flap_control_process();
    led_status_update();
}