#ifndef HVAC_FLAP_CONTROL_H
#define HVAC_FLAP_CONTROL_H

#include "stm32f4xx_hal.h"

void flap_control_init(void);
void flap_control_process(void);
void flap_control_set_target(uint8_t position);

#endif // HVAC_FLAP_CONTROL_H