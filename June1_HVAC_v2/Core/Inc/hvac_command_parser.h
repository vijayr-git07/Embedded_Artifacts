#ifndef HVAC_COMMAND_PARSER_H
#define HVAC_COMMAND_PARSER_H

#include "stm32f4xx_hal.h"

typedef enum {
    HVAC_CMD_NONE = 0,
    HVAC_CMD_POS_0 = 0,
    HVAC_CMD_POS_1 = 1,
    HVAC_CMD_POS_2 = 2,
    HVAC_CMD_POS_3 = 3,
    HVAC_CMD_POS_4 = 4,
    HVAC_CMD_POS_5 = 5,
    HVAC_CMD_INVALID = 0xFF
} hvac_command_t;

void command_parser_init(void);
void command_parser_process(void);
hvac_command_t command_parser_get_latest(void);
uint8_t command_parser_is_new(void);
void command_parser_clear_new_flag(void);

#endif // HVAC_COMMAND_PARSER_H