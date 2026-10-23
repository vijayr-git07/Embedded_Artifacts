#ifndef HVAC_MOTOR_DRIVER_H
#define HVAC_MOTOR_DRIVER_H

#include "stm32f4xx_hal.h"

#define MOTOR_DIR_STOP    0
#define MOTOR_DIR_FORWARD 1
#define MOTOR_DIR_REVERSE 2

void motor_driver_init(void);
void motor_driver_set_direction(uint8_t dir);
void motor_driver_enable(void);
void motor_driver_stop(void);
uint8_t motor_driver_get_direction(void);

#endif // HVAC_MOTOR_DRIVER_H