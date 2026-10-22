#ifndef HVAC_CMD_PARSER_H
#define HVAC_CMD_PARSER_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

void hvac_cmd_parser_init(void);
void hvac_cmd_parser_process(void);
uint8_t hvac_cmd_parser_get_latest(uint8_t *position, uint8_t *valid);

#endif // HVAC_CMD_PARSER_H