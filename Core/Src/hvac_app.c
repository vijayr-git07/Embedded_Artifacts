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
    initialized = 1;

    hvac_bsp_init();

    hvac_cmd_parser_init(&huart_hvac);
    hvac_feedback_init(&hadc_hvac);

    GPIO_TypeDef *led_ports[HVAC_LED_NUM_POSITIONS] = {
        HVAC_LED0_GPIO_PORT,
        HVAC_LED1_GPIO_PORT,
        HVAC_LED2_GPIO_PORT,
        HVAC_LED3_GPIO_PORT,
        HVAC_LED4_GPIO_PORT
    };
    uint16_t led_pins[HVAC_LED_NUM_POSITIONS] = {
        HVAC_LED0_GPIO_PIN,
        HVAC_LED1_GPIO_PIN,
        HVAC_LED2_GPIO_PIN,
        HVAC_LED3_GPIO_PIN,
        HVAC_LED4_GPIO_PIN
    };

    hvac_led_status_init(led_ports, led_pins, HVAC_LED_NUM_POSITIONS, HVAC_PWR_LED_GPIO_PORT, HVAC_PWR_LED_GPIO_PIN);

    hvac_motor_driver_init(HVAC_MOTOR_DIR_GPIO_PORT, HVAC_MOTOR_DIR_PIN_A, HVAC_MOTOR_DIR_PIN_B,
                          &htim_motor_pwm, HVAC_MOTOR_PWM_CHANNEL);

    hvac_flap_control_init();

    // Read initial feedback and update LEDs
    hvac_feedback_update();
    uint8_t valid;
    uint8_t pos = hvac_feedback_get_position(&valid);
    hvac_led_status_update(pos, valid);
    hvac_led_status_power(1);
}

void hvac_app_task(void)
{
    // 1. Process UART commands
    hvac_cmd_parser_process();

    // 2. Read ADC feedback
    hvac_feedback_update();

    // 3. Flap control logic
    hvac_flap_control_process();

    // 4. LED status update is handled by flap control logic as needed
}