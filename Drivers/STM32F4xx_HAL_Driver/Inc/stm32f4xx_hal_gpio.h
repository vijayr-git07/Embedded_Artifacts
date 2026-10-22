#ifndef __STM32F4xx_HAL_GPIO_H
#define __STM32F4xx_HAL_GPIO_H

#include "stm32f4xx_hal_def.h"

typedef struct
{
  uint32_t Pin;
  uint32_t Mode;
  uint32_t Pull;
  uint32_t Speed;
  uint32_t Alternate;
} GPIO_InitTypeDef;

typedef struct
{
  GPIO_TypeDef *Instance;
} GPIO_HandleTypeDef;

#define GPIO_PIN_RESET                 0
#define GPIO_PIN_SET                   1

#define GPIO_MODE_INPUT                0x00000000U
#define GPIO_MODE_OUTPUT_PP            0x00000001U
#define GPIO_MODE_AF_PP                0x00000002U
#define GPIO_MODE_ANALOG               0x00000003U

#define GPIO_NOPULL                    0x00000000U
#define GPIO_PULLUP                    0x00000001U
#define GPIO_PULLDOWN                  0x00000002U

#define GPIO_SPEED_FREQ_LOW            0x00000000U
#define GPIO_SPEED_FREQ_MEDIUM         0x00000001U
#define GPIO_SPEED_FREQ_HIGH           0x00000002U
#define GPIO_SPEED_FREQ_VERY_HIGH      0x00000003U

void HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init);
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t PinState);

#endif /* __STM32F4xx_HAL_GPIO_H */