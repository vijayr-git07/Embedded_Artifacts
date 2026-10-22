#ifndef __STM32F4xx_HAL_H
#define __STM32F4xx_HAL_H

#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_pwr.h"
#include "stm32f4xx_hal_flash.h"

HAL_StatusTypeDef HAL_Init(void);
void HAL_IncTick(void);

#endif /* __STM32F4xx_HAL_H */