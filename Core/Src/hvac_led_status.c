#include "hvac_led_status.h"

static GPIO_TypeDef *power_led_port = NULL;
static uint16_t power_led_pin = 0;
static GPIO_TypeDef *pos_led_ports[5] = {0};
static uint16_t pos_led_pins[5] = {0};

static uint8_t last_flap_position = 0xFF;
static uint8_t last_valid = 0;

void hvac_led_status_init(GPIO_TypeDef *power_port, uint16_t power_pin,
                         GPIO_TypeDef *pos_ports[5], uint16_t pos_pins[5])
{
    power_led_port = power_port;
    power_led_pin = power_pin;
    for (uint8_t i = 0; i < 5; ++i) {
        pos_led_ports[i] = pos_ports[i];
        pos_led_pins[i] = pos_pins[i];
    }
    // Power LED ON
    HAL_GPIO_WritePin(power_led_port, power_led_pin, GPIO_PIN_SET);
    // All position LEDs OFF
    for (uint8_t i = 0; i < 5; ++i) {
        HAL_GPIO_WritePin(pos_led_ports[i], pos_led_pins[i], GPIO_PIN_RESET);
    }
    last_flap_position = 0xFF;
    last_valid = 0;
}

void hvac_led_status_update(uint8_t flap_position, uint8_t valid)
{
    if (!valid) {
        // Do not update LEDs on invalid feedback
        return;
    }
    if (flap_position > 4) return;

    if (last_flap_position == flap_position && last_valid == valid) {
        // No change
        return;
    }

    // Only one position LED ON at a time
    for (uint8_t i = 0; i < 5; ++i) {
        if (i == flap_position) {
            HAL_GPIO_WritePin(pos_led_ports[i], pos_led_pins[i], GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(pos_led_ports[i], pos_led_pins[i], GPIO_PIN_RESET);
        }
    }
    last_flap_position = flap_position;
    last_valid = valid;
}