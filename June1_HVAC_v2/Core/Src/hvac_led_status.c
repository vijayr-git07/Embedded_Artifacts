#include "hvac_led_status.h"
#include "hvac_feedback_processor.h"

// Power LED: PC0
#define POWER_LED_GPIO_PORT GPIOC
#define POWER_LED_GPIO_PIN  GPIO_PIN_0

// Position LEDs: PC1–PC5
static const GPIO_TypeDef * const pos_led_ports[6] = {
    NULL, // 0: no LED
    GPIOC, // 1: PC1
    GPIOC, // 2: PC2
    GPIOC, // 3: PC3
    GPIOC, // 4: PC4
    GPIOC  // 5: PC5
};
static const uint16_t pos_led_pins[6] = {
    0,           // 0: no LED
    GPIO_PIN_1,  // 1
    GPIO_PIN_2,  // 2
    GPIO_PIN_3,  // 3
    GPIO_PIN_4,  // 4
    GPIO_PIN_5   // 5
};

static uint8_t last_led_pos = 0xFF;
static uint8_t last_feedback_valid = 1;

void led_status_init(void)
{
    // Power LED ON
    HAL_GPIO_WritePin(POWER_LED_GPIO_PORT, POWER_LED_GPIO_PIN, GPIO_PIN_SET);
    // All position LEDs OFF
    for (uint8_t i = 1; i <= 5; ++i) {
        HAL_GPIO_WritePin((GPIO_TypeDef*)pos_led_ports[i], pos_led_pins[i], GPIO_PIN_RESET);
    }
    last_led_pos = 0xFF;
    last_feedback_valid = 1;
}

void led_status_update(void)
{
    uint8_t pos, valid;
    feedback_processor_get_position(&pos, NULL, &valid);

    if (!valid) {
        // Invalid feedback: do not change LEDs
        return;
    }

    if (pos == 0) {
        // All position LEDs OFF
        for (uint8_t i = 1; i <= 5; ++i) {
            HAL_GPIO_WritePin((GPIO_TypeDef*)pos_led_ports[i], pos_led_pins[i], GPIO_PIN_RESET);
        }
        last_led_pos = 0;
        return;
    }

    if (pos != last_led_pos) {
        // Turn off all, turn on only current
        for (uint8_t i = 1; i <= 5; ++i) {
            HAL_GPIO_WritePin((GPIO_TypeDef*)pos_led_ports[i], pos_led_pins[i], (i == pos) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
        last_led_pos = pos;
    }
}