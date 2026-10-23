#include "hvac_led_status.h"
#include "hvac_feedback_processor.h"
#include "main.h"

// Power LED: PC0
#define POWER_LED_GPIO_PORT GPIOC
#define POWER_LED_PIN       GPIO_PIN_0

// Position LEDs: PC1–PC5 (positions 1–5)
static const GPIO_TypeDef * const pos_led_ports[5] = {
    GPIOC, GPIOC, GPIOC, GPIOC, GPIOC
};
static const uint16_t pos_led_pins[5] = {
    GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5
};

static uint8_t last_led_pos = 0xFF;

void led_status_init(void)
{
    // Power LED ON
    HAL_GPIO_WritePin(POWER_LED_GPIO_PORT, POWER_LED_PIN, GPIO_PIN_SET);
    // All position LEDs OFF
    for (uint8_t i = 0; i < 5; ++i)
        HAL_GPIO_WritePin((GPIO_TypeDef*)pos_led_ports[i], pos_led_pins[i], GPIO_PIN_RESET);
    last_led_pos = 0xFF;
}

void led_status_update(void)
{
    uint8_t pos, valid;
    feedback_processor_get_position(&pos, NULL, &valid);

    // Power LED always ON
    HAL_GPIO_WritePin(POWER_LED_GPIO_PORT, POWER_LED_PIN, GPIO_PIN_SET);

    // Only update if feedback valid
    if (!valid)
        return;

    // Only one position LED ON at a time (positions 1–5)
    // Position 0: all LEDs OFF except power
    if (pos == 0)
    {
        for (uint8_t i = 0; i < 5; ++i)
            HAL_GPIO_WritePin((GPIO_TypeDef*)pos_led_ports[i], pos_led_pins[i], GPIO_PIN_RESET);
        last_led_pos = 0xFF;
        return;
    }

    // Only update if changed
    if (last_led_pos != pos)
    {
        for (uint8_t i = 0; i < 5; ++i)
            HAL_GPIO_WritePin((GPIO_TypeDef*)pos_led_ports[i], pos_led_pins[i], GPIO_PIN_RESET);
        if (pos >= 1 && pos <= 5)
            HAL_GPIO_WritePin((GPIO_TypeDef*)pos_led_ports[pos-1], pos_led_pins[pos-1], GPIO_PIN_SET);
        last_led_pos = pos;
    }
}