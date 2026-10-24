#ifndef HVAC_BSP_H
#define HVAC_BSP_H

#include "stm32f4xx_hal.h"

// UART for command interface
extern UART_HandleTypeDef huart2;

// ADC for potentiometer feedback
extern ADC_HandleTypeDef hadc1;

// Motor direction GPIOs
#define HVAC_MOTOR_DIR_PORT      GPIOB
#define HVAC_MOTOR_DIR_PIN_CW    GPIO_PIN_0
#define HVAC_MOTOR_DIR_PIN_CCW   GPIO_PIN_1

// Motor PWM
#define HVAC_MOTOR_PWM_TIM       htim3
#define HVAC_MOTOR_PWM_CHANNEL   TIM_CHANNEL_1

extern TIM_HandleTypeDef htim3;

// LED GPIOs
#define HVAC_LED_POWER_PORT      GPIOC
#define HVAC_LED_POWER_PIN       GPIO_PIN_13

#define HVAC_LED_POS0_PORT       GPIOC
#define HVAC_LED_POS0_PIN        GPIO_PIN_0
#define HVAC_LED_POS1_PORT       GPIOC
#define HVAC_LED_POS1_PIN        GPIO_PIN_1
#define HVAC_LED_POS2_PORT       GPIOC
#define HVAC_LED_POS2_PIN        GPIO_PIN_2
#define HVAC_LED_POS3_PORT       GPIOC
#define HVAC_LED_POS3_PIN        GPIO_PIN_3
#define HVAC_LED_POS4_PORT       GPIOC
#define HVAC_LED_POS4_PIN        GPIO_PIN_4

void hvac_bsp_init(void);

#endif // HVAC_BSP_H