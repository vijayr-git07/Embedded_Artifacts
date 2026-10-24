#ifndef HVAC_FEEDBACK_H
#define HVAC_FEEDBACK_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

#define HVAC_FLAP_POS_INVALID 0xFF

void hvac_feedback_init(ADC_HandleTypeDef *hadc);
void hvac_feedback_update(void);
uint8_t hvac_feedback_get_position(uint8_t *valid);

#endif // HVAC_FEEDBACK_H