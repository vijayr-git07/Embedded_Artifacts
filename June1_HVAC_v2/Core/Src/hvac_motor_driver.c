#include "hvac_motor_driver.h"
#include "main.h"

// Motor direction pins: IN1 = PB0, IN2 = PB1
#define MOTOR_IN1_GPIO_PORT GPIOB
#define MOTOR_IN1_PIN       GPIO_PIN_0
#define MOTOR_IN2_GPIO_PORT GPIOB
#define MOTOR_IN2_PIN       GPIO_PIN_1

// PWM: TIM3 CH1 (PA6)
extern TIM_HandleTypeDef htim3;

static uint8_t motor_enabled_flag = 0;
static uint8_t motor_direction = 0; // 1=forward, 0=reverse

void motor_driver_init(void)
{
    // Ensure safe state
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    motor_enabled_flag = 0;
    motor_direction = 0;
}

void motor_driver_set_direction(uint8_t forward)
{
    if (forward)
    {
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_PIN, GPIO_PIN_SET);
        motor_direction = 1;
    }
    else
    {
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
        motor_direction = 0;
    }
}

void motor_driver_enable(void)
{
    if (!motor_enabled_flag)
    {
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
        motor_enabled_flag = 1;
    }
}

void motor_driver_stop(void)
{
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    motor_enabled_flag = 0;
}