#ifndef HVAC_CMD_PARSER_H
#define HVAC_CMD_PARSER_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

#define HVAC_CMD_INVALID 0xFF

void hvac_cmd_parser_init(UART_HandleTypeDef *huart);
void hvac_cmd_parser_process(void);
uint8_t hvac_cmd_parser_get_latest(uint8_t *cmd_valid);

#endif // HVAC_CMD_PARSER_H