#ifndef HVAC_LED_STATUS_H
#define HVAC_LED_STATUS_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

void hvac_led_status_init(GPIO_TypeDef *power_port, uint16_t power_pin,
                         GPIO_TypeDef *pos_ports[5], uint16_t pos_pins[5]);
void hvac_led_status_update(uint8_t flap_position, uint8_t valid);

#endif // HVAC_LED_STATUS_H