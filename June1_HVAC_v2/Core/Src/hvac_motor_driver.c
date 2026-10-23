#include "hvac_motor_driver.h"
#include "main.h"

// TIM3 handle from main.c
extern TIM_HandleTypeDef htim3;

// Motor direction pins: PB0, PB1
#define MOTOR_IN1_GPIO_PORT GPIOB
#define MOTOR_IN1_PIN       GPIO_PIN_0
#define MOTOR_IN2_GPIO_PORT GPIOB
#define MOTOR_IN2_PIN       GPIO_PIN_1

// PWM: TIM3 CH1 (PA6)
#define MOTOR_PWM_TIMER     &htim3
#define MOTOR_PWM_CHANNEL   TIM_CHANNEL_1

static uint8_t motor_enabled_flag = 0;
static uint8_t motor_direction = MOTOR_DIR_FORWARD;

void motor_driver_init(void)
{
    motor_enabled_flag = 0;
    motor_direction = MOTOR_DIR_FORWARD;
    // Ensure safe state
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(MOTOR_PWM_TIMER, MOTOR_PWM_CHANNEL);
}

void motor_driver_set_direction(uint8_t dir)
{
    motor_direction = dir;
    if (dir == MOTOR_DIR_FORWARD)
    {
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_PIN, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    }
}

void motor_driver_enable(void)
{
    if (!motor_enabled_flag)
    {
        HAL_TIM_PWM_Start(MOTOR_PWM_TIMER, MOTOR_PWM_CHANNEL);
        motor_enabled_flag = 1;
    }
}

void motor_driver_stop(void)
{
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(MOTOR_PWM_TIMER, MOTOR_PWM_CHANNEL);
    motor_enabled_flag = 0;
}