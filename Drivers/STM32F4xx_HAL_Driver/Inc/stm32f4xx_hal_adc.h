#ifndef __STM32F4xx_HAL_ADC_H
#define __STM32F4xx_HAL_ADC_H

#include "stm32f4xx_hal_def.h"

typedef struct
{
  uint32_t ClockPrescaler;
  uint32_t Resolution;
  uint32_t ScanConvMode;
  uint32_t ContinuousConvMode;
  uint32_t DiscontinuousConvMode;
  uint32_t ExternalTrigConvEdge;
  uint32_t ExternalTrigConv;
  uint32_t DataAlign;
  uint32_t NbrOfConversion;
  uint32_t DMAContinuousRequests;
  uint32_t EOCSelection;
} ADC_InitTypeDef;

typedef struct
{
  ADC_TypeDef                 *Instance;
  ADC_InitTypeDef             Init;
} ADC_HandleTypeDef;

typedef struct
{
  uint32_t Channel;
  uint32_t Rank;
  uint32_t SamplingTime;
} ADC_ChannelConfTypeDef;

#define ADC_RESOLUTION_12B             0x00000000U
#define ADC_CLOCK_SYNC_PCLK_DIV4       0x00000003U
#define ADC_SCAN_DISABLE               0x00000000U
#define ADC_SCAN_ENABLE                0x00000001U
#define ADC_SOFTWARE_START             0x00000000U
#define ADC_DATAALIGN_RIGHT            0x00000000U
#define ADC_EOC_SINGLE_CONV            0x00000001U
#define ADC_SAMPLETIME_3CYCLES         0x00000000U

#define ADC_CHANNEL_1                  0x00000001U

HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig);
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout);
uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef* hadc);

#endif /* __STM32F4xx_HAL_ADC_H */