#include "hvac_led_status.h"

// Power LED: PC0
#define POWER_LED_GPIO_PORT GPIOC
#define POWER_LED_PIN       GPIO_PIN_0

// Position LEDs: PC1-PC5
#define POS1_LED_GPIO_PORT GPIOC
#define POS1_LED_PIN       GPIO_PIN_1
#define POS2_LED_GPIO_PORT GPIOC
#define POS2_LED_PIN       GPIO_PIN_2
#define POS3_LED_GPIO_PORT GPIOC
#define POS3_LED_PIN       GPIO_PIN_3
#define POS4_LED_GPIO_PORT GPIOC
#define POS4_LED_PIN       GPIO_PIN_4
#define POS5_LED_GPIO_PORT GPIOC
#define POS5_LED_PIN       GPIO_PIN_5

static void set_all_position_leds_off(void)
{
    HAL_GPIO_WritePin(POS1_LED_GPIO_PORT, POS1_LED_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(POS2_LED_GPIO_PORT, POS2_LED_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(POS3_LED_GPIO_PORT, POS3_LED_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(POS4_LED_GPIO_PORT, POS4_LED_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(POS5_LED_GPIO_PORT, POS5_LED_PIN, GPIO_PIN_RESET);
}

void led_status_init(void)
{
    // Power LED ON
    HAL_GPIO_WritePin(POWER_LED_GPIO_PORT, POWER_LED_PIN, GPIO_PIN_SET);
    set_all_position_leds_off();
}

void led_status_update(flap_position_t pos)
{
    set_all_position_leds_off();

    switch (pos) {
        case FLAP_POS_1:
            HAL_GPIO_WritePin(POS1_LED_GPIO_PORT, POS1_LED_PIN, GPIO_PIN_SET);
            break;
        case FLAP_POS_2:
            HAL_GPIO_WritePin(POS2_LED_GPIO_PORT, POS2_LED_PIN, GPIO_PIN_SET);
            break;
        case FLAP_POS_3:
            HAL_GPIO_WritePin(POS3_LED_GPIO_PORT, POS3_LED_PIN, GPIO_PIN_SET);
            break;
        case FLAP_POS_4:
            HAL_GPIO_WritePin(POS4_LED_GPIO_PORT, POS4_LED_PIN, GPIO_PIN_SET);
            break;
        case FLAP_POS_5:
            HAL_GPIO_WritePin(POS5_LED_GPIO_PORT, POS5_LED_PIN, GPIO_PIN_SET);
            break;
        default:
            // All LEDs OFF for FLAP_POS_0 or invalid
            break;
    }
    // Power LED always ON
    HAL_GPIO_WritePin(POWER_LED_GPIO_PORT, POWER_LED_PIN, GPIO_PIN_SET);
}