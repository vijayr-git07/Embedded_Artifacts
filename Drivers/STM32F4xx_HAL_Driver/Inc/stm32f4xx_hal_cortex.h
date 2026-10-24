#ifndef __STM32F4xx_HAL_CORTEX_H
#define __STM32F4xx_HAL_CORTEX_H

#include "stm32f4xx_hal_def.h"

void HAL_NVIC_SetPriority(int IRQn, uint32_t priority, uint32_t subpriority);
void HAL_NVIC_EnableIRQ(int IRQn);

#endif /* __STM32F4xx_HAL_CORTEX_H */