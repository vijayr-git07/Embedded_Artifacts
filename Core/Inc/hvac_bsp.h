#ifndef HVAC_BSP_H
#define HVAC_BSP_H

#include "stm32f4xx_hal.h"

// UART for command interface
#define HVAC_BSP_UART_HANDLE      (&huart2)

// ADC for potentiometer feedback
#define HVAC_BSP_ADC_HANDLE      (&hadc1)

// Motor direction GPIO
#define HVAC_BSP_MOTOR_DIR_GPIO_Port   GPIOB
#define HVAC_BSP_MOTOR_DIR_Pin         GPIO_PIN_0

// Motor PWM (TIM3 CH1)
#define HVAC_BSP_MOTOR_PWM_TIM         (&htim3)
#define HVAC_BSP_MOTOR_PWM_CHANNEL     TIM_CHANNEL_1

// LED GPIOs (5 positions)
#define HVAC_BSP_LED_COUNT 5
extern GPIO_TypeDef* HVAC_BSP_LED_GPIO_Port[HVAC_BSP_LED_COUNT];
extern uint16_t      HVAC_BSP_LED_Pin[HVAC_BSP_LED_COUNT];

// Extern handles (defined in main.c or elsewhere)
extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;

#endif // HVAC_BSP_H