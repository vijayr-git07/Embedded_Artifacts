#ifndef __STM32F4xx_HAL_UART_H
#define __STM32F4xx_HAL_UART_H

#include "stm32f4xx_hal_def.h"

typedef struct
{
    USART_TypeDef               *Instance;
    uint32_t                    Init;
    uint8_t                     Lock;
    uint32_t                    State;
    void                        *DMA_Handle;
} UART_HandleTypeDef;

typedef struct
{
    __IO uint32_t SR;
    __IO uint32_t DR;
    __IO uint32_t BRR;
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t CR3;
    __IO uint32_t GTPR;
} USART_TypeDef;

HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

#endif /* __STM32F4xx_HAL_UART_H */