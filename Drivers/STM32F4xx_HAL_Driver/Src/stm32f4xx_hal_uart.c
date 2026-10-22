#include "stm32f4xx_hal_uart.h"

HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    // Dummy implementation: always return TIMEOUT
    return HAL_TIMEOUT;
}