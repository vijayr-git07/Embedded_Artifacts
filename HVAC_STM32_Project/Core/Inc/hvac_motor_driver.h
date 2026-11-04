#ifndef HVAC_MOTOR_DRIVER_H
#define HVAC_MOTOR_DRIVER_H

#include "stm32f4xx_hal.h"

void motor_driver_init(void);
void motor_driver_set_direction(uint8_t direction); // 0: forward, 1: reverse
void motor_driver_enable(void);
void motor_driver_stop(void);

#endif // HVAC_MOTOR_DRIVER_H