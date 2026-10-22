#ifndef __STM32F4xx_HAL_UART_H
#define __STM32F4xx_HAL_UART_H

#include "stm32f4xx_hal_def.h"

typedef struct
{
  uint32_t BaudRate;
  uint32_t WordLength;
  uint32_t StopBits;
  uint32_t Parity;
  uint32_t Mode;
  uint32_t HwFlowCtl;
  uint32_t OverSampling;
} UART_InitTypeDef;

typedef struct
{
  USART_TypeDef         *Instance;
  UART_InitTypeDef      Init;
} UART_HandleTypeDef;

#define UART_WORDLENGTH_8B             0x00000000U
#define UART_STOPBITS_1                0x00000000U
#define UART_PARITY_NONE               0x00000000U
#define UART_MODE_TX_RX                0x00000000U
#define UART_HWCONTROL_NONE            0x00000000U
#define UART_OVERSAMPLING_16           0x00000000U

HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

#endif /* __STM32F4xx_HAL_UART_H */