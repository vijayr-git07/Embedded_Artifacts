#include "hvac_motor_driver.h"
#include "main.h"

// TIM3 CH1 PWM: PA6
// Direction: PB0 (IN1), PB1 (IN2)
#define MOTOR_IN1_GPIO_PORT GPIOB
#define MOTOR_IN1_GPIO_PIN  GPIO_PIN_0
#define MOTOR_IN2_GPIO_PORT GPIOB
#define MOTOR_IN2_GPIO_PIN  GPIO_PIN_1

// PWM: TIM3, Channel 1
extern TIM_HandleTypeDef htim3;

static uint8_t motor_enabled_flag = 0;
static uint8_t motor_direction = 0; // 0: Forward, 1: Reverse

void motor_driver_init(void)
{
    // Ensure safe state
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_GPIO_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_GPIO_PIN, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    motor_enabled_flag = 0;
    motor_direction = 0;
}

void motor_driver_set_direction(uint8_t dir)
{
    if (dir == 0) {
        // Forward: IN1=RESET, IN2=SET
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_GPIO_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_GPIO_PIN, GPIO_PIN_SET);
    } else {
        // Reverse: IN1=SET, IN2=RESET
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_GPIO_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_GPIO_PIN, GPIO_PIN_RESET);
    }
    motor_direction = dir;
}

void motor_driver_enable(void)
{
    if (!motor_enabled_flag) {
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
        motor_enabled_flag = 1;
    }
}

void motor_driver_stop(void)
{
    // Stop PWM and set IN1/IN2 to safe state
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_PORT, MOTOR_IN1_GPIO_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_PORT, MOTOR_IN2_GPIO_PIN, GPIO_PIN_RESET);
    motor_enabled_flag = 0;
}