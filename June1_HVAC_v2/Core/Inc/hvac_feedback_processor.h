#ifndef HVAC_FEEDBACK_PROCESSOR_H
#define HVAC_FEEDBACK_PROCESSOR_H

#include "stm32f4xx_hal.h"

typedef enum {
    FLAP_POS_INVALID = 0xFF,
    FLAP_POS_0 = 0,
    FLAP_POS_1 = 1,
    FLAP_POS_2 = 2,
    FLAP_POS_3 = 3,
    FLAP_POS_4 = 4,
    FLAP_POS_5 = 5
} flap_position_t;

void feedback_processor_init(void);
void feedback_processor_update(void);
flap_position_t feedback_processor_get_position(void);
uint8_t feedback_processor_is_valid(void);
uint16_t feedback_processor_get_adc(void);

#endif // HVAC_FEEDBACK_PROCESSOR_H