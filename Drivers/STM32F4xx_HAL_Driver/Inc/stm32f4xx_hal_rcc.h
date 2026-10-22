#ifndef __STM32F4xx_HAL_RCC_H
#define __STM32F4xx_HAL_RCC_H

#include "stm32f4xx_hal_def.h"

#define __HAL_RCC_GPIOA_CLK_ENABLE()   (RCC->AHB1ENR |= (1<<0))
#define __HAL_RCC_GPIOB_CLK_ENABLE()   (RCC->AHB1ENR |= (1<<1))
#define __HAL_RCC_GPIOC_CLK_ENABLE()   (RCC->AHB1ENR |= (1<<2))
#define __HAL_RCC_USART2_CLK_ENABLE()  (RCC->APB1ENR |= (1<<17))
#define __HAL_RCC_ADC1_CLK_ENABLE()    (RCC->APB2ENR |= (1<<8))
#define __HAL_RCC_TIM3_CLK_ENABLE()    (RCC->APB1ENR |= (1<<1))
#define __HAL_RCC_SYSCFG_CLK_ENABLE()  (RCC->APB2ENR |= (1<<14))
#define __HAL_RCC_PWR_CLK_ENABLE()     (RCC->APB1ENR |= (1<<28))

#endif /* __STM32F4xx_HAL_RCC_H */