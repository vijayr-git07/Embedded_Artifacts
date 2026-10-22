#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

// UART handle
extern UART_HandleTypeDef huart2;

// ADC handle
extern ADC_HandleTypeDef hadc1;

// Timer handle for PWM
extern TIM_HandleTypeDef htim3;

// GPIO pin defines for motor and LEDs
#define MOTOR_DIR_GPIO_Port GPIOB
#define MOTOR_DIR_Pin       GPIO_PIN_0

#define LED0_GPIO_Port      GPIOC
#define LED0_Pin            GPIO_PIN_0
#define LED1_GPIO_Port      GPIOC
#define LED1_Pin            GPIO_PIN_1
#define LED2_GPIO_Port      GPIOC
#define LED2_Pin            GPIO_PIN_2
#define LED3_GPIO_Port      GPIOC
#define LED3_Pin            GPIO_PIN_3
#define LED4_GPIO_Port      GPIOC
#define LED4_Pin            GPIO_PIN_4

#endif /* __MAIN_H */