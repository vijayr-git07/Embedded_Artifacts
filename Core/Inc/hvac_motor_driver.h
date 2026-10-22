#ifndef HVAC_MOTOR_DRIVER_H
#define HVAC_MOTOR_DRIVER_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef enum {
    HVAC_MOTOR_DIR_CW = 0,
    HVAC_MOTOR_DIR_CCW = 1
} hvac_motor_dir_t;

void hvac_motor_driver_init(void);
void hvac_motor_driver_set_direction(hvac_motor_dir_t dir);
void hvac_motor_driver_enable(void);
void hvac_motor_driver_stop(void);

#endif // HVAC_MOTOR_DRIVER_H