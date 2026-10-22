#ifndef __STM32F4xx_HAL_ADC_H
#define __STM32F4xx_HAL_ADC_H

#include "stm32f4xx_hal_def.h"

typedef struct
{
    void *Instance;
    // ... other ADC config fields
} ADC_HandleTypeDef;

HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout);
uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef* hadc);

#endif /* __STM32F4xx_HAL_ADC_H */