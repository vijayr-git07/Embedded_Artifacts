#ifndef HVAC_FEEDBACK_PROCESSOR_H
#define HVAC_FEEDBACK_PROCESSOR_H

#include "stm32f4xx_hal.h"

void feedback_processor_init(void);
void feedback_processor_update(void);
uint8_t feedback_processor_get_position(void);
uint8_t feedback_processor_is_valid(void);
uint16_t feedback_processor_get_adc(void);
uint8_t feedback_processor_in_precise_stop_window(uint8_t position);

#endif // HVAC_FEEDBACK_PROCESSOR_H