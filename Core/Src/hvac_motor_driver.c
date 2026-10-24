#include "hvac_motor_driver.h"

static GPIO_TypeDef *motor_dir_port = NULL;
static uint16_t motor_dir_pin_cw = 0;
static uint16_t motor_dir_pin_ccw = 0;
static TIM_HandleTypeDef *motor_pwm_tim = NULL;
static uint32_t motor_pwm_channel = 0;

static motor_direction_t motor_direction = MOTOR_DIR_STOP;
static uint8_t motor_enabled_flag = 0;

#define MOTOR_PWM_DUTY_ACTIVE 80 // percent
#define MOTOR_PWM_DUTY_IDLE   0

void hvac_motor_driver_init(GPIO_TypeDef *dir_port, uint16_t dir_pin_cw, uint16_t dir_pin_ccw,
                           TIM_HandleTypeDef *htim_pwm, uint32_t pwm_channel)
{
    motor_dir_port = dir_port;
    motor_dir_pin_cw = dir_pin_cw;
    motor_dir_pin_ccw = dir_pin_ccw;
    motor_pwm_tim = htim_pwm;
    motor_pwm_channel = pwm_channel;

    // Ensure safe state
    HAL_GPIO_WritePin(motor_dir_port, motor_dir_pin_cw, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(motor_dir_port, motor_dir_pin_ccw, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(motor_pwm_tim, motor_pwm_channel);
    motor_direction = MOTOR_DIR_STOP;
    motor_enabled_flag = 0;
}

void hvac_motor_driver_set_direction(motor_direction_t dir)
{
    if (motor_dir_port == NULL) return;

    switch (dir) {
        case MOTOR_DIR_CW:
            HAL_GPIO_WritePin(motor_dir_port, motor_dir_pin_cw, GPIO_PIN_SET);
            HAL_GPIO_WritePin(motor_dir_port, motor_dir_pin_ccw, GPIO_PIN_RESET);
            motor_direction = MOTOR_DIR_CW;
            break;
        case MOTOR_DIR_CCW:
            HAL_GPIO_WritePin(motor_dir_port, motor_dir_pin_cw, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(motor_dir_port, motor_dir_pin_ccw, GPIO_PIN_SET);
            motor_direction = MOTOR_DIR_CCW;
            break;
        default:
            HAL_GPIO_WritePin(motor_dir_port, motor_dir_pin_cw, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(motor_dir_port, motor_dir_pin_ccw, GPIO_PIN_RESET);
            motor_direction = MOTOR_DIR_STOP;
            break;
    }
}

void hvac_motor_driver_enable(uint8_t enable)
{
    if (motor_pwm_tim == NULL) return;

    if (enable) {
        // Set PWM duty
        __HAL_TIM_SET_COMPARE(motor_pwm_tim, motor_pwm_channel,
            (motor_pwm_tim->Init.Period * MOTOR_PWM_DUTY_ACTIVE) / 100);
        HAL_TIM_PWM_Start(motor_pwm_tim, motor_pwm_channel);
        motor_enabled_flag = 1;
    } else {
        HAL_TIM_PWM_Stop(motor_pwm_tim, motor_pwm_channel);
        motor_enabled_flag = 0;
    }
}

void hvac_motor_driver_stop(void)
{
    if (motor_pwm_tim == NULL || motor_dir_port == NULL) return;

    HAL_TIM_PWM_Stop(motor_pwm_tim, motor_pwm_channel);
    HAL_GPIO_WritePin(motor_dir_port, motor_dir_pin_cw, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(motor_dir_port, motor_dir_pin_ccw, GPIO_PIN_RESET);
    motor_direction = MOTOR_DIR_STOP;
    motor_enabled_flag = 0;
}