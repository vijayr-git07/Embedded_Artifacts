#ifndef __STM32F4xx_HAL_CONF_H
#define __STM32F4xx_HAL_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_pwr.h"
#include "stm32f4xx_hal_flash.h"
#include "stm32f4xx_hal_exti.h"

#define HAL_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED

#define HSE_VALUE    ((uint32_t)8000000U)
#define HSI_VALUE    ((uint32_t)16000000U)
#define LSE_VALUE    ((uint32_t)32768U)
#define LSI_VALUE    ((uint32_t)32000U)

#define  VDD_VALUE                    ((uint32_t)3300)
#define  TICK_INT_PRIORITY            ((uint32_t)0)
#define  USE_RTOS                     0
#define  PREFETCH_ENABLE              1
#define  INSTRUCTION_CACHE_ENABLE     1
#define  DATA_CACHE_ENABLE            1

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_CONF_H */