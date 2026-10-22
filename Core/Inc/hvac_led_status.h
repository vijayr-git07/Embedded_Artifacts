#ifndef HVAC_LED_STATUS_H
#define HVAC_LED_STATUS_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

void hvac_led_status_init(void);
void hvac_led_status_update(uint8_t position, uint8_t valid);

#endif // HVAC_LED_STATUS_H