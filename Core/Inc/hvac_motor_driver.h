#ifndef HVAC_MOTOR_DRIVER_H
#define HVAC_MOTOR_DRIVER_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef enum {
    MOTOR_DIR_STOP = 0,
    MOTOR_DIR_CW,
    MOTOR_DIR_CCW
} motor_direction_t;

void hvac_motor_driver_init(GPIO_TypeDef *dir_port, uint16_t dir_pin_a, uint16_t dir_pin_b,
                           TIM_HandleTypeDef *htim_pwm, uint32_t pwm_channel);
void hvac_motor_driver_set_direction(motor_direction_t dir);
void hvac_motor_driver_enable(uint8_t enable);
void hvac_motor_driver_stop(void);

#endif // HVAC_MOTOR_DRIVER_H