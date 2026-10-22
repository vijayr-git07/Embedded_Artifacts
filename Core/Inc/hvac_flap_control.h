#ifndef HVAC_FLAP_CONTROL_H
#define HVAC_FLAP_CONTROL_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

void hvac_flap_control_init(void);
void hvac_flap_control_process(void);

#endif // HVAC_FLAP_CONTROL_H