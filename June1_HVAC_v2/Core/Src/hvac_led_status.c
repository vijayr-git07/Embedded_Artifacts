#include "hvac_led_status.h"
#include "hvac_feedback_processor.h"
#include "main.h"

// Power LED: PC0
#define POWER_LED_GPIO_PORT GPIOC
#define POWER_LED_PIN       GPIO_PIN_0

// Position LEDs: PC1–PC5 (positions 1–5)
static const uint16_t position_led_pins[5] = {
    GPIO_PIN_1, // pos 1
    GPIO_PIN_2, // pos 2
    GPIO_PIN_3, // pos 3
    GPIO_PIN_4, // pos 4
    GPIO_PIN_5  // pos 5
};
#define POSITION_LED_GPIO_PORT GPIOC

static uint8_t last_led_state = 0xFF;

void led_status_init(void)
{
    // Power LED ON
    HAL_GPIO_WritePin(POWER_LED_GPIO_PORT, POWER_LED_PIN, GPIO_PIN_SET);
    // All position LEDs OFF
    for (uint8_t i = 0; i < 5; ++i)
        HAL_GPIO_WritePin(POSITION_LED_GPIO_PORT, position_led_pins[i], GPIO_PIN_RESET);
    last_led_state = 0xFF;
}

void led_status_update(void)
{
    uint8_t valid = feedback_processor_is_valid();
    uint8_t pos = feedback_processor_get_position();

    // Power LED always ON
    HAL_GPIO_WritePin(POWER_LED_GPIO_PORT, POWER_LED_PIN, GPIO_PIN_SET);

    // Only one position LED ON at a time (positions 1–5)
    if (!valid || pos == 0)
    {
        // All position LEDs OFF
        for (uint8_t i = 0; i < 5; ++i)
            HAL_GPIO_WritePin(POSITION_LED_GPIO_PORT, position_led_pins[i], GPIO_PIN_RESET);
        last_led_state = 0xFF;
    }
    else
    {
        if (last_led_state != pos)
        {
            // Turn all OFF, then ON the correct one
            for (uint8_t i = 0; i < 5; ++i)
                HAL_GPIO_WritePin(POSITION_LED_GPIO_PORT, position_led_pins[i], GPIO_PIN_RESET);
            if (pos >= 1 && pos <= 5)
                HAL_GPIO_WritePin(POSITION_LED_GPIO_PORT, position_led_pins[pos-1], GPIO_PIN_SET);
            last_led_state = pos;
        }
    }
}