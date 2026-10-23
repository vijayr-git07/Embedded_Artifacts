#include "hvac_motor_driver.h"

// Motor direction pins (L298N IN1/IN2)
#define MOTOR_IN1_GPIO_PORT GPIOB
#define MOTOR_IN1_PIN       GPIO_PIN_0
#define MOTOR_IN2_GPIO_PORT GPIOB
#define MOTOR_IN2_PIN       GPIO_PIN_1

// PWM output (TIM3 CH1, PA6)
extern TIM_HandleTypeDef htim3;

static motor_direction_t current_dir = MOTOR_DIR_STOP;
static uint8_t motor_enabled = 0;

void motor_driver_init(void)
{
    current_dir = MOTOR_DIR_STOP;
    motor_enabled = 0;
    // Ensure safe state
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}

void motor_driver_set_direction(motor_direction_t dir)
{
    if (dir == MOTOR_DIR_FORWARD) {
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_PIN, GPIO_PIN_SET);
        current_dir = MOTOR_DIR_FORWARD;
    } else if (dir == MOTOR_DIR_REVERSE) {
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
        current_dir = MOTOR_DIR_REVERSE;
    } else {
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
        current_dir = MOTOR_DIR_STOP;
    }
}

void motor_driver_enable(void)
{
    if (!motor_enabled) {
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
        motor_enabled = 1;
    }
}

void motor_driver_stop(void)
{
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    motor_enabled = 0;
    current_dir = MOTOR_DIR_STOP;
}