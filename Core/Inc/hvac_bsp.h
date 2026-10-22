#ifndef HVAC_BSP_H
#define HVAC_BSP_H

#include "stm32f4xx_hal.h"

// UART for command interface
#define HVAC_UART_INSTANCE         USART2
#define HVAC_UART_BAUDRATE         115200

// ADC for potentiometer feedback
#define HVAC_ADC_INSTANCE          ADC1
#define HVAC_ADC_CHANNEL           ADC_CHANNEL_1

// Motor direction GPIOs
#define HVAC_MOTOR_DIR_GPIO_PORT   GPIOB
#define HVAC_MOTOR_DIR_PIN_A       GPIO_PIN_0
#define HVAC_MOTOR_DIR_PIN_B       GPIO_PIN_1

// Motor PWM
#define HVAC_MOTOR_PWM_TIM         TIM3
#define HVAC_MOTOR_PWM_CHANNEL     TIM_CHANNEL_1

// Flap position LEDs (positions 0-4)
#define HVAC_LED_NUM_POSITIONS     5
#define HVAC_LED0_GPIO_PORT        GPIOC
#define HVAC_LED0_GPIO_PIN         GPIO_PIN_0
#define HVAC_LED1_GPIO_PORT        GPIOC
#define HVAC_LED1_GPIO_PIN         GPIO_PIN_1
#define HVAC_LED2_GPIO_PORT        GPIOC
#define HVAC_LED2_GPIO_PIN         GPIO_PIN_2
#define HVAC_LED3_GPIO_PORT        GPIOC
#define HVAC_LED3_GPIO_PIN         GPIO_PIN_3
#define HVAC_LED4_GPIO_PORT        GPIOC
#define HVAC_LED4_GPIO_PIN         GPIO_PIN_4

// Power LED
#define HVAC_PWR_LED_GPIO_PORT     GPIOC
#define HVAC_PWR_LED_GPIO_PIN      GPIO_PIN_5

// BSP initialization
void hvac_bsp_init(void);

// BSP handles
extern UART_HandleTypeDef huart_hvac;
extern ADC_HandleTypeDef hadc_hvac;
extern TIM_HandleTypeDef htim_motor_pwm;

#endif // HVAC_BSP_H