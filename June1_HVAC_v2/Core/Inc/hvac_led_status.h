#ifndef HVAC_LED_STATUS_H
#define HVAC_LED_STATUS_H

#include "stm32f4xx_hal.h"
#include "hvac_feedback_processor.h"

void led_status_init(void);
void led_status_update(flap_position_t pos);

#endif // HVAC_LED_STATUS_H