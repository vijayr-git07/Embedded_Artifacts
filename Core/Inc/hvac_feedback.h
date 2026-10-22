#ifndef HVAC_FEEDBACK_H
#define HVAC_FEEDBACK_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

void hvac_feedback_init(void);
void hvac_feedback_update(void);
uint8_t hvac_feedback_get_position(void);
uint8_t hvac_feedback_is_valid(void);
uint16_t hvac_feedback_get_adc(void);

#endif // HVAC_FEEDBACK_H