#include "hvac_led_status.h"
#include "main.h"

// Power LED: PC0
#define POWER_LED_GPIO_PORT GPIOC
#define POWER_LED_PIN       GPIO_PIN_0

// Position LEDs: PC1–PC5 (positions 1–5)
#define POS_LED_GPIO_PORT   GPIOC
static const uint16_t pos_led_pins[5] = {
    GPIO_PIN_1, // pos 1
    GPIO_PIN_2, // pos 2
    GPIO_PIN_3, // pos 3
    GPIO_PIN_4, // pos 4
    GPIO_PIN_5  // pos 5
};

static uint8_t current_led_state = 0xFF;
static uint8_t last_led_position = 0xFF;

void led_status_init(void)
{
    // Power LED ON
    HAL_GPIO_WritePin(POWER_LED_GPIO_PORT, POWER_LED_PIN, GPIO_PIN_SET);
    // All position LEDs OFF
    for (uint8_t i = 0; i < 5; ++i)
        HAL_GPIO_WritePin(POS_LED_GPIO_PORT, pos_led_pins[i], GPIO_PIN_RESET);
    current_led_state = 0xFF;
    last_led_position = 0xFF;
}

void led_status_set_position(uint8_t pos)
{
    // Only update if changed
    if (last_led_position == pos)
        return;

    // Turn off all position LEDs
    for (uint8_t i = 0; i < 5; ++i)
        HAL_GPIO_WritePin(POS_LED_GPIO_PORT, pos_led_pins[i], GPIO_PIN_RESET);

    // Only positions 1–5 have LEDs
    if (pos >= 1 && pos <= 5)
        HAL_GPIO_WritePin(POS_LED_GPIO_PORT, pos_led_pins[pos-1], GPIO_PIN_SET);

    last_led_position = pos;
}

void led_status_update(void)
{
    // No periodic update needed; LED state is set by led_status_set_position()
}