#ifndef HVAC_MOTOR_DRIVER_H
#define HVAC_MOTOR_DRIVER_H

#include "stm32f4xx_hal.h"

void motor_driver_init(void);
void motor_driver_set_direction(uint8_t dir); // 0: Forward, 1: Reverse
void motor_driver_enable(void);
void motor_driver_stop(void);

#endif // HVAC_MOTOR_DRIVER_H