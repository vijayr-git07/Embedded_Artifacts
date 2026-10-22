#ifndef __STM32F407XX_H
#define __STM32F407XX_H

#include <stdint.h>

#define PERIPH_BASE           ((uint32_t)0x40000000)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400)

#define GPIOC                 ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOB                 ((GPIO_TypeDef *) GPIOB_BASE)

typedef struct
{
  volatile uint32_t MODER;
  volatile uint32_t OTYPER;
  volatile uint32_t OSPEEDR;
  volatile uint32_t PUPDR;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t LCKR;
  volatile uint32_t AFR[2];
} GPIO_TypeDef;

#endif /* __STM32F407XX_H */