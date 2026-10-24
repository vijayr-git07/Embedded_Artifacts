#include "hvac_app.h"
#include "hvac_bsp.h"
#include "hvac_cmd_parser.h"
#include "hvac_feedback.h"
#include "hvac_flap_control.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"

static uint8_t initialized = 0;

void hvac_app_init(void)
{
    if (initialized) return;
    hvac_bsp_init();

    hvac_cmd_parser_init(&huart2);
    hvac_feedback_init(&hadc1);

    GPIO_TypeDef *led_ports[5] = {
        HVAC_LED_POS0_PORT,
        HVAC_LED_POS1_PORT,
        HVAC_LED_POS2_PORT,
        HVAC_LED_POS3_PORT,
        HVAC_LED_POS4_PORT
    };
    uint16_t led_pins[5] = {
        HVAC_LED_POS0_PIN,
        HVAC_LED_POS1_PIN,
        HVAC_LED_POS2_PIN,
        HVAC_LED_POS3_PIN,
        HVAC_LED_POS4_PIN
    };

    hvac_motor_driver_init(HVAC_MOTOR_DIR_PORT, HVAC_MOTOR_DIR_PIN_CW, HVAC_MOTOR_DIR_PIN_CCW,
                          &htim3, HVAC_MOTOR_PWM_CHANNEL);
    hvac_led_status_init(HVAC_LED_POWER_PORT, HVAC_LED_POWER_PIN, led_ports, led_pins);

    hvac_flap_control_init();

    // Read initial feedback and update LED
    hvac_feedback_update();
    uint8_t valid;
    uint8_t pos = hvac_feedback_get_position(&valid);
    hvac_led_status_update(pos, valid);

    initialized = 1;
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