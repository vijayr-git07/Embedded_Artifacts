#include "hvac_led_status.h"
#include "hvac_bsp.h"

#define HVAC_FLAP_POSITIONS 5

static uint8_t current_led_state = 0xFF;

void hvac_led_status_init(void)
{
    // Turn off all LEDs
    for (uint8_t i = 0; i < HVAC_FLAP_POSITIONS; ++i)
    {
        HAL_GPIO_WritePin(HVAC_BSP_LED_GPIO_Port[i], HVAC_BSP_LED_Pin[i], GPIO_PIN_RESET);
    }
    current_led_state = 0xFF;
}

void hvac_led_status_update(uint8_t position, uint8_t valid)
{
    if (!valid)
    {
        // Do not update LEDs on invalid feedback
        return;
    }

    if (position >= HVAC_FLAP_POSITIONS)
        return;

    if (current_led_state == position)
        return;

    // Turn off all LEDs
    for (uint8_t i = 0; i < HVAC_FLAP_POSITIONS; ++i)
    {
        HAL_GPIO_WritePin(HVAC_BSP_LED_GPIO_Port[i], HVAC_BSP_LED_Pin[i], GPIO_PIN_RESET);
    }
    // Turn on only the selected LED
    HAL_GPIO_WritePin(HVAC_BSP_LED_GPIO_Port[position], HVAC_BSP_LED_Pin[position], GPIO_PIN_SET);
    current_led_state = position;
}