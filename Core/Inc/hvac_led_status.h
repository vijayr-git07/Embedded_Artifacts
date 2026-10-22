#ifndef HVAC_LED_STATUS_H
#define HVAC_LED_STATUS_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

void hvac_led_status_init(GPIO_TypeDef **led_ports, uint16_t *led_pins, uint8_t num_leds, GPIO_TypeDef *pwr_led_port, uint16_t pwr_led_pin);
void hvac_led_status_update(uint8_t flap_position, uint8_t valid);
void hvac_led_status_power(uint8_t on);

#endif // HVAC_LED_STATUS_H