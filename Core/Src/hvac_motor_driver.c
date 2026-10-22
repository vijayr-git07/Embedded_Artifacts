#include "hvac_motor_driver.h"
#include "hvac_bsp.h"

static hvac_motor_dir_t motor_direction = HVAC_MOTOR_DIR_CW;
static uint8_t motor_enabled_flag = 0;

void hvac_motor_driver_init(void)
{
    // Ensure safe state
    motor_enabled_flag = 0;
    motor_direction = HVAC_MOTOR_DIR_CW;
    HAL_GPIO_WritePin(HVAC_BSP_MOTOR_DIR_GPIO_Port, HVAC_BSP_MOTOR_DIR_Pin, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(HVAC_BSP_MOTOR_PWM_TIM, HVAC_BSP_MOTOR_PWM_CHANNEL);
}

void hvac_motor_driver_set_direction(hvac_motor_dir_t dir)
{
    motor_direction = dir;
    if (dir == HVAC_MOTOR_DIR_CW)
    {
        HAL_GPIO_WritePin(HVAC_BSP_MOTOR_DIR_GPIO_Port, HVAC_BSP_MOTOR_DIR_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(HVAC_BSP_MOTOR_DIR_GPIO_Port, HVAC_BSP_MOTOR_DIR_Pin, GPIO_PIN_SET);
    }
}

void hvac_motor_driver_enable(void)
{
    if (!motor_enabled_flag)
    {
        HAL_TIM_PWM_Start(HVAC_BSP_MOTOR_PWM_TIM, HVAC_BSP_MOTOR_PWM_CHANNEL);
        motor_enabled_flag = 1;
    }
}

void hvac_motor_driver_stop(void)
{
    if (motor_enabled_flag)
    {
        HAL_TIM_PWM_Stop(HVAC_BSP_MOTOR_PWM_TIM, HVAC_BSP_MOTOR_PWM_CHANNEL);
        motor_enabled_flag = 0;
    }
    // Optionally, set direction pin to safe state
    HAL_GPIO_WritePin(HVAC_BSP_MOTOR_DIR_GPIO_Port, HVAC_BSP_MOTOR_DIR_Pin, GPIO_PIN_RESET);
}