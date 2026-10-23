#ifndef HVAC_LED_STATUS_H
#define HVAC_LED_STATUS_H

#include "stm32f4xx_hal.h"

void led_status_init(void);
void led_status_update(void);
void led_status_set_position(uint8_t pos);

#endif // HVAC_LED_STATUS_H