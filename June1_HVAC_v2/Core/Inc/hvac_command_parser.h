#ifndef HVAC_COMMAND_PARSER_H
#define HVAC_COMMAND_PARSER_H

#include "stm32f4xx_hal.h"

void command_parser_init(void);
void command_parser_process(void);
uint8_t command_parser_get_latest(uint8_t *out_target_pos, uint8_t *out_is_new);

#endif // HVAC_COMMAND_PARSER_H