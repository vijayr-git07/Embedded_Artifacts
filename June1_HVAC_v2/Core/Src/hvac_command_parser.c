#include "hvac_command_parser.h"
#include "main.h"
#include <string.h>

// UART handle from main.c
extern UART_HandleTypeDef huart2;

#define UART_CMD_MIN '0'
#define UART_CMD_MAX '5'

static uint8_t latest_command = 0;
static uint8_t command_valid_flag = 0;
static uint8_t last_reported_command = 0xFF;
static uint8_t is_new_command = 0;
static uint8_t rx_buf[1];

static const char *mode_names[6] = {
    "Initial Mode",      // 0
    "Face Mode",         // 1
    "Face and Foot Mode",// 2
    "Foot Mode",         // 3
    "Defrost Mode",      // 4
    "Full Open",         // 5
};

static void log_uart(const char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
}

void command_parser_init(void)
{
    latest_command = 0;
    command_valid_flag = 0;
    last_reported_command = 0xFF;
    is_new_command = 0;
}

void command_parser_process(void)
{
    if (HAL_UART_Receive(&huart2, rx_buf, 1, 0) == HAL_OK)
    {
        uint8_t c = rx_buf[0];
        if (c >= UART_CMD_MIN && c <= UART_CMD_MAX)
        {
            uint8_t cmd = c - UART_CMD_MIN;
            if (cmd != latest_command)
            {
                latest_command = cmd;
                command_valid_flag = 1;
                is_new_command = 1;
            }
            else
            {
                // Same as previous, do not retrigger
                is_new_command = 0;
            }
        }
        else
        {
            // Invalid command, only log once per invalid
            if (last_reported_command != 0xFE)
            {
                log_uart("STM32: Invalid command ignored\r\n");
                last_reported_command = 0xFE;
            }
            command_valid_flag = 0;
            is_new_command = 0;
        }
    }
}

uint8_t command_parser_get_latest(uint8_t *out_target_pos, uint8_t *out_is_new)
{
    if (command_valid_flag)
    {
        if (out_target_pos) *out_target_pos = latest_command;
        if (out_is_new) *out_is_new = is_new_command;
        if (is_new_command)
        {
            last_reported_command = latest_command;
            is_new_command = 0;
        }
        return 1;
    }
    return 0;
}

const char *command_parser_mode_name(uint8_t pos)
{
    if (pos < 6)
        return mode_names[pos];
    return "Unknown";
}