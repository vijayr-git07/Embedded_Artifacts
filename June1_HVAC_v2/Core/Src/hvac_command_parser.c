#include "hvac_command_parser.h"
#include <string.h>

// Use the existing UART handle from main.c
extern UART_HandleTypeDef huart2;

#define UART_CMD_BUF_SIZE 1

static uint8_t latest_command = 0xFF; // 0xFF = invalid/uninitialized
static uint8_t command_valid_flag = 0;
static uint8_t command_is_new = 0;
static uint8_t last_reported_command = 0xFF;

static const char *mode_names[6] = {
    "Initial Mode",      // 0
    "Face Mode",         // 1
    "Face and Foot Mode",// 2
    "Foot Mode",         // 3
    "Defrost Mode",      // 4
    "Full Open",         // 5
};

static uint8_t uart_rx_buf[UART_CMD_BUF_SIZE];

void command_parser_init(void)
{
    latest_command = 0xFF;
    command_valid_flag = 0;
    command_is_new = 0;
    last_reported_command = 0xFF;
}

void command_parser_process(void)
{
    HAL_StatusTypeDef status;
    uint8_t rx;
    status = HAL_UART_Receive(&huart2, &rx, 1, 0);
    if (status == HAL_OK)
    {
        if (rx >= '0' && rx <= '5')
        {
            uint8_t pos = rx - '0';
            if (latest_command != pos)
            {
                latest_command = pos;
                command_valid_flag = 1;
                command_is_new = 1;
            }
            else
            {
                // Same as previous, do not mark as new
                command_valid_flag = 1;
                command_is_new = 0;
            }
        }
        else
        {
            // Invalid command
            if (latest_command != 0xFE)
            {
                latest_command = 0xFE; // 0xFE = last invalid
                command_valid_flag = 0;
                command_is_new = 1;
            }
            else
            {
                command_valid_flag = 0;
                command_is_new = 0;
            }
        }
    }
}

uint8_t command_parser_get_latest(uint8_t *out_position, uint8_t *out_is_new)
{
    if (latest_command <= 5)
    {
        if (out_position) *out_position = latest_command;
        if (out_is_new) *out_is_new = command_is_new;
        return 1;
    }
    else
    {
        if (out_position) *out_position = 0xFF;
        if (out_is_new) *out_is_new = command_is_new;
        return 0;
    }
}