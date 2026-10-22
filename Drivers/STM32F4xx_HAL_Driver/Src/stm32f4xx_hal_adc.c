#include "stm32f4xx_hal_adc.h"

HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc)
{
    // Dummy implementation
    return HAL_OK;
}

HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout)
{
    // Dummy implementation
    return HAL_OK;
}

uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc)
{
    // Dummy implementation
    return 2048;
}

HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef* hadc)
{
    // Dummy implementation
    return HAL_OK;
}