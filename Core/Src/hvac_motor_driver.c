#include "hvac_motor_driver.h"

static GPIO_TypeDef *dir_port = NULL;
static uint16_t dir_pin_a = 0;
static uint16_t dir_pin_b = 0;
static TIM_HandleTypeDef *pwm_tim = NULL;
static uint32_t pwm_channel = 0;

static uint8_t motor_enabled_flag = 0;
static motor_direction_t motor_direction = MOTOR_DIR_STOP;

#define MOTOR_PWM_DUTY 80 // percent

void hvac_motor_driver_init(GPIO_TypeDef *d_port, uint16_t d_pin_a, uint16_t d_pin_b,
                           TIM_HandleTypeDef *htim_pwm, uint32_t pwm_ch)
{
    dir_port = d_port;
    dir_pin_a = d_pin_a;
    dir_pin_b = d_pin_b;
    pwm_tim = htim_pwm;
    pwm_channel = pwm_ch;

    // Ensure safe state
    HAL_GPIO_WritePin(dir_port, dir_pin_a, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(dir_port, dir_pin_b, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(pwm_tim, pwm_channel);
    motor_enabled_flag = 0;
    motor_direction = MOTOR_DIR_STOP;
}

void hvac_motor_driver_set_direction(motor_direction_t dir)
{
    if (dir_port == NULL) return;

    switch (dir)
    {
        case MOTOR_DIR_CW:
            HAL_GPIO_WritePin(dir_port, dir_pin_a, GPIO_PIN_SET);
            HAL_GPIO_WritePin(dir_port, dir_pin_b, GPIO_PIN_RESET);
            motor_direction = MOTOR_DIR_CW;
            break;
        case MOTOR_DIR_CCW:
            HAL_GPIO_WritePin(dir_port, dir_pin_a, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(dir_port, dir_pin_b, GPIO_PIN_SET);
            motor_direction = MOTOR_DIR_CCW;
            break;
        default:
            HAL_GPIO_WritePin(dir_port, dir_pin_a, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(dir_port, dir_pin_b, GPIO_PIN_RESET);
            motor_direction = MOTOR_DIR_STOP;
            break;
    }
}

void hvac_motor_driver_enable(uint8_t enable)
{
    if (pwm_tim == NULL) return;

    if (enable)
    {
        // Set PWM duty cycle
        uint32_t period = pwm_tim->Init.Period;
        __HAL_TIM_SET_COMPARE(pwm_tim, pwm_channel, (period * MOTOR_PWM_DUTY) / 100);
        HAL_TIM_PWM_Start(pwm_tim, pwm_channel);
        motor_enabled_flag = 1;
    }
    else
    {
        HAL_TIM_PWM_Stop(pwm_tim, pwm_channel);
        motor_enabled_flag = 0;
    }
}

void hvac_motor_driver_stop(void)
{
    hvac_motor_driver_enable(0);
    hvac_motor_driver_set_direction(MOTOR_DIR_STOP);
}