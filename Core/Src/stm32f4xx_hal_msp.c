#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance==ADC1)
    {
        __HAL_RCC_ADC1_CLK_ENABLE();
        // GPIO configuration for ADC1 assumed to be handled by CubeMX
    }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
    if(htim_pwm->Instance==TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
        // GPIO configuration for TIM3 PWM assumed to be handled by CubeMX
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    if(huart->Instance==USART2)
    {
        __HAL_RCC_USART2_CLK_ENABLE();
        // GPIO configuration for USART2 assumed to be handled by CubeMX
    }
}