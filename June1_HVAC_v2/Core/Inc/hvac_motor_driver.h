#ifndef HVAC_MOTOR_DRIVER_H
#define HVAC_MOTOR_DRIVER_H

#include "stm32f4xx_hal.h"

#define MOTOR_DIR_FORWARD  0
#define MOTOR_DIR_REVERSE  1

void motor_driver_init(void);
void motor_driver_set_direction(uint8_t dir);
void motor_driver_enable(void);
void motor_driver_stop(void);

#endif // HVAC_MOTOR_DRIVER_H