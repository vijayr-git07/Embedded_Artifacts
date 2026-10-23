#ifndef HVAC_FLAP_CONTROL_H
#define HVAC_FLAP_CONTROL_H

#include "stm32f4xx_hal.h"
#include "hvac_command_parser.h"
#include "hvac_feedback_processor.h"

typedef enum {
    FLAP_MOVE_NONE = 0,
    FLAP_MOVE_FORWARD,
    FLAP_MOVE_REVERSE
} flap_move_dir_t;

void flap_control_init(void);
void flap_control_process(void);

#endif // HVAC_FLAP_CONTROL_H