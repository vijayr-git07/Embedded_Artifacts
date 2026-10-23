#ifndef HVAC_MOTOR_DRIVER_H
#define HVAC_MOTOR_DRIVER_H

#include "stm32f4xx_hal.h"

typedef enum {
    MOTOR_DIR_STOP = 0,
    MOTOR_DIR_FORWARD,
    MOTOR_DIR_REVERSE
} motor_direction_t;

void motor_driver_init(void);
void motor_driver_set_direction(motor_direction_t dir);
void motor_driver_enable(void);
void motor_driver_stop(void);

#endif // HVAC_MOTOR_DRIVER_H