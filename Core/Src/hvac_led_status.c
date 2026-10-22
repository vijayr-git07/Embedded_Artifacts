#include "hvac_led_status.h"

#define MAX_LED_POSITIONS 5

static GPIO_TypeDef *led_ports[MAX_LED_POSITIONS];
static uint16_t led_pins[MAX_LED_POSITIONS];
static uint8_t num_leds = 0;
static GPIO_TypeDef *pwr_led_port = NULL;
static uint16_t pwr_led_pin = 0;
static uint8_t last_led_pos = 0xFF;

void hvac_led_status_init(GPIO_TypeDef **ports, uint16_t *pins, uint8_t n_leds, GPIO_TypeDef *pwr_port, uint16_t pwr_pin)
{
    uint8_t i;
    num_leds = (n_leds > MAX_LED_POSITIONS) ? MAX_LED_POSITIONS : n_leds;
    for (i = 0; i < num_leds; ++i)
    {
        led_ports[i] = ports[i];
        led_pins[i] = pins[i];
        HAL_GPIO_WritePin(led_ports[i], led_pins[i], GPIO_PIN_RESET);
    }
    pwr_led_port = pwr_port;
    pwr_led_pin = pwr_pin;
    HAL_GPIO_WritePin(pwr_led_port, pwr_led_pin, GPIO_PIN_RESET);
    last_led_pos = 0xFF;
}

void hvac_led_status_update(uint8_t flap_position, uint8_t valid)
{
    if (!valid || flap_position >= num_leds)
        return;

    if (last_led_pos == flap_position)
        return;

    // Turn off all LEDs
    for (uint8_t i = 0; i < num_leds; ++i)
        HAL_GPIO_WritePin(led_ports[i], led_pins[i], GPIO_PIN_RESET);

    // Turn on only the current position LED
    HAL_GPIO_WritePin(led_ports[flap_position], led_pins[flap_position], GPIO_PIN_SET);

    last_led_pos = flap_position;
}

void hvac_led_status_power(uint8_t on)
{
    if (pwr_led_port == NULL) return;
    HAL_GPIO_WritePin(pwr_led_port, pwr_led_pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}