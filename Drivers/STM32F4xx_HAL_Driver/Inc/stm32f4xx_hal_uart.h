#ifndef __STM32F4xx_HAL_UART_H
#define __STM32F4xx_HAL_UART_H

#include "stm32f4xx_hal_def.h"

typedef struct
{
    void *Instance;
    // ... other UART config fields
} UART_HandleTypeDef;

HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

#endif /* __STM32F4xx_HAL_UART_H */