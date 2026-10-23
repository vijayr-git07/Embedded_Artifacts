#include "hvac_motor_driver.h"
#include "main.h"

// Motor direction pins: IN1 = PB0, IN2 = PB1
#define MOTOR_IN1_GPIO_PORT GPIOB
#define MOTOR_IN1_GPIO_PIN  GPIO_PIN_0
#define MOTOR_IN2_GPIO_PORT GPIOB
#define MOTOR_IN2_GPIO_PIN  GPIO_PIN_1

// PWM: TIM3 CH1 (PA6)
extern TIM_HandleTypeDef htim3;

static uint8_t motor_enabled_flag = 0;
static uint8_t motor_direction = MOTOR_DIR_STOP;

void motor_driver_init(void)
{
    motor_enabled_flag = 0;
    motor_direction = MOTOR_DIR_STOP;
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_GPIO_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_GPIO_PIN, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}

void motor_driver_set_direction(uint8_t dir)
{
    if (dir == MOTOR_DIR_FORWARD)
    {
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_GPIO_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_GPIO_PIN, GPIO_PIN_SET);
        motor_direction = MOTOR_DIR_FORWARD;
    }
    else if (dir == MOTOR_DIR_REVERSE)
    {
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_GPIO_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_GPIO_PIN, GPIO_PIN_RESET);
        motor_direction = MOTOR_DIR_REVERSE;
    }
    else
    {
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_GPIO_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_GPIO_PIN, GPIO_PIN_RESET);
        motor_direction = MOTOR_DIR_STOP;
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
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_GPIO_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_GPIO_PIN, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    motor_enabled_flag = 0;
    motor_direction = MOTOR_DIR_STOP;
}

uint8_t motor_driver_get_direction(void)
{
    return motor_direction;
}