#ifndef HVAC_FEEDBACK_PROCESSOR_H
#define HVAC_FEEDBACK_PROCESSOR_H

#include "stm32f4xx_hal.h"

void feedback_processor_init(void);
void feedback_processor_update(void);
uint8_t feedback_processor_get_position(uint8_t *out_pos, uint16_t *out_adc, uint8_t *out_valid);
uint8_t feedback_processor_is_valid(void);
uint16_t feedback_processor_get_adc(void);
uint8_t feedback_processor_in_precise_window(uint8_t target_pos);

#endif // HVAC_FEEDBACK_PROCESSOR_H