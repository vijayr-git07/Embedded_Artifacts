#ifndef __STM32F4xx_HAL_TIM_H
#define __STM32F4xx_HAL_TIM_H

#include "stm32f4xx_hal_def.h"

typedef struct
{
  uint32_t Prescaler;
  uint32_t CounterMode;
  uint32_t Period;
  uint32_t ClockDivision;
  uint32_t AutoReloadPreload;
} TIM_Base_InitTypeDef;

typedef struct
{
  TIM_TypeDef                 *Instance;
  TIM_Base_InitTypeDef        Init;
} TIM_HandleTypeDef;

typedef struct
{
  uint32_t OCMode;
  uint32_t Pulse;
  uint32_t OCPolarity;
  uint32_t OCFastMode;
} TIM_OC_InitTypeDef;

#define TIM_COUNTERMODE_UP             0x00000000U
#define TIM_CLOCKDIVISION_DIV1         0x00000000U
#define TIM_AUTORELOAD_PRELOAD_DISABLE 0x00000000U

#define TIM_OCMODE_PWM1                0x00000006U
#define TIM_OCPOLARITY_HIGH            0x00000000U
#define TIM_OCFAST_DISABLE             0x00000000U

#define TIM_CHANNEL_1                  0x00000000U

HAL_StatusTypeDef HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim, TIM_OC_InitTypeDef *sConfig, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);
void __HAL_TIM_SET_COMPARE(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t Compare);

#endif /* __STM32F4xx_HAL_TIM_H */