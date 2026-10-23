#include "hvac_command_parser.h"
#include <string.h>

// UART handle provided by CubeMX project
extern UART_HandleTypeDef huart2;

#define UART_CMD_BUF_SIZE 1

static hvac_command_t latest_command = HVAC_CMD_NONE;
static uint8_t command_valid_flag = 0;
static uint8_t command_new_flag = 0;
static uint8_t last_invalid_logged = 0;

static hvac_command_t parse_uart_char(uint8_t c)
{
    switch (c) {
        case '0': return HVAC_CMD_POS_0;
        case '1': return HVAC_CMD_POS_1;
        case '2': return HVAC_CMD_POS_2;
        case '3': return HVAC_CMD_POS_3;
        case '4': return HVAC_CMD_POS_4;
        case '5': return HVAC_CMD_POS_5;
        default:  return HVAC_CMD_INVALID;
    }
}

void command_parser_init(void)
{
    latest_command = HVAC_CMD_NONE;
    command_valid_flag = 0;
    command_new_flag = 0;
    last_invalid_logged = 0;
}

void command_parser_process(void)
{
    uint8_t rx;
    if (HAL_UART_Receive(&huart2, &rx, 1, 0) == HAL_OK) {
        hvac_command_t cmd = parse_uart_char(rx);
        if (cmd != HVAC_CMD_INVALID) {
            if (cmd != latest_command) {
                latest_command = cmd;
                command_valid_flag = 1;
                command_new_flag = 1;
                last_invalid_logged = 0;
            }
        } else {
            // Only log once per invalid command
            if (!last_invalid_logged) {
                const char *msg = "STM32: Invalid command ignored\r\n";
                HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint16_t)strlen(msg), 100);
                last_invalid_logged = 1;
            }
            command_valid_flag = 0;
            command_new_flag = 0;
        }
    }
}

hvac_command_t command_parser_get_latest(void)
{
    if (command_valid_flag) {
        return latest_command;
    }
    return HVAC_CMD_NONE;
}

uint8_t command_parser_is_new(void)
{
    return command_new_flag;
}

void command_parser_clear_new_flag(void)
{
    command_new_flag = 0;
}