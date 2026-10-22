#ifndef __STM32F407XX_H
#define __STM32F407XX_H

#include "stm32f4xx.h"

#define GPIOA               ((GPIO_TypeDef *) 0x40020000UL)
#define GPIOB               ((GPIO_TypeDef *) 0x40020400UL)
#define GPIOC               ((GPIO_TypeDef *) 0x40020800UL)
#define GPIOD               ((GPIO_TypeDef *) 0x40020C00UL)
#define GPIOE               ((GPIO_TypeDef *) 0x40021000UL)
#define GPIOF               ((GPIO_TypeDef *) 0x40021400UL)
#define GPIOG               ((GPIO_TypeDef *) 0x40021800UL)
#define GPIOH               ((GPIO_TypeDef *) 0x40021C00UL)
#define GPIOI               ((GPIO_TypeDef *) 0x40022000UL)

#define RCC                 ((RCC_TypeDef *) 0x40023800UL)
#define ADC1                ((ADC_TypeDef *) 0x40012000UL)
#define TIM3                ((TIM_TypeDef *) 0x40000400UL)
#define USART2              ((USART_TypeDef *) 0x40004400UL)

#endif /* __STM32F407XX_H */