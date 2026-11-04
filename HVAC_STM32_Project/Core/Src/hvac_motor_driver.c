#include "hvac_motor_driver.h"

// Use the existing TIM3 handle from main.c
extern TIM_HandleTypeDef htim3;

// Motor direction pins: PB0 (IN1), PB1 (IN2)
#define MOTOR_IN1_GPIO_PORT GPIOB
#define MOTOR_IN1_GPIO_PIN  GPIO_PIN_0
#define MOTOR_IN2_GPIO_PORT GPIOB
#define MOTOR_IN2_GPIO_PIN  GPIO_PIN_1

// PWM: TIM3 CH1 (PA6)
#define MOTOR_PWM_TIMER     (&htim3)
#define MOTOR_PWM_CHANNEL   TIM_CHANNEL_1

static uint8_t motor_enabled_flag = 0;
static uint8_t motor_direction = 0; // 0: forward, 1: reverse

void motor_driver_init(void)
{
    motor_enabled_flag = 0;
    motor_direction = 0;
    // Ensure safe state
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_GPIO_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_GPIO_PIN, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(MOTOR_PWM_TIMER, MOTOR_PWM_CHANNEL);
}

void motor_driver_set_direction(uint8_t direction)
{
    motor_direction = direction;
    if (direction == 0) {
        // Forward: IN1=RESET, IN2=SET
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_GPIO_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_GPIO_PIN, GPIO_PIN_SET);
    } else {
        // Reverse: IN1=SET, IN2=RESET
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_GPIO_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_GPIO_PIN, GPIO_PIN_RESET);
    }
}

void motor_driver_enable(void)
{
    if (!motor_enabled_flag) {
        HAL_TIM_PWM_Start(MOTOR_PWM_TIMER, MOTOR_PWM_CHANNEL);
        motor_enabled_flag = 1;
    }
}

void motor_driver_stop(void)
{
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_GPIO_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_GPIO_PIN, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(MOTOR_PWM_TIMER, MOTOR_PWM_CHANNEL);
    motor_enabled_flag = 0;
}