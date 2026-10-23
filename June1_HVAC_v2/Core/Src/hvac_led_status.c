#include "hvac_led_status.h"
#include "hvac_feedback_processor.h"

// Power LED: PC0
#define POWER_LED_GPIO_PORT GPIOC
#define POWER_LED_GPIO_PIN  GPIO_PIN_0

// Position LEDs: PC1–PC5 (positions 1–5)
static const uint16_t position_led_pins[5] = {
    GPIO_PIN_1, // pos 1
    GPIO_PIN_2, // pos 2
    GPIO_PIN_3, // pos 3
    GPIO_PIN_4, // pos 4
    GPIO_PIN_5, // pos 5
};
#define POSITION_LED_GPIO_PORT GPIOC

static uint8_t last_position = 0xFF;
static uint8_t last_feedback_valid = 0;

void led_status_init(void)
{
    // Power LED ON
    HAL_GPIO_WritePin(POWER_LED_GPIO_PORT, POWER_LED_GPIO_PIN, GPIO_PIN_SET);
    // All position LEDs OFF
    for (uint8_t i = 0; i < 5; ++i) {
        HAL_GPIO_WritePin(POSITION_LED_GPIO_PORT, position_led_pins[i], GPIO_PIN_RESET);
    }
    last_position = 0xFF;
    last_feedback_valid = 0;
}

void led_status_update(void)
{
    uint8_t pos, valid;
    uint16_t adc;
    feedback_processor_get_position(&pos, &adc, &valid);

    if (!valid) {
        // Feedback invalid: keep previous LED state
        return;
    }

    if (pos != last_position || !last_feedback_valid) {
        // Turn off all position LEDs
        for (uint8_t i = 0; i < 5; ++i) {
            HAL_GPIO_WritePin(POSITION_LED_GPIO_PORT, position_led_pins[i], GPIO_PIN_RESET);
        }
        // Only one position LED active at a time (positions 1–5)
        if (pos >= 1 && pos <= 5) {
            HAL_GPIO_WritePin(POSITION_LED_GPIO_PORT, position_led_pins[pos - 1], GPIO_PIN_SET);
        }
        last_position = pos;
        last_feedback_valid = valid;
    }
}