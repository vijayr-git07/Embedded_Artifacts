#include "hvac_cmd_parser.h"

#define UART_RX_BUF_SIZE 1

static UART_HandleTypeDef *cmd_uart = NULL;
static uint8_t latest_command = HVAC_CMD_INVALID;
static uint8_t command_valid_flag = 0;
static uint8_t rx_buf[UART_RX_BUF_SIZE];

void hvac_cmd_parser_init(UART_HandleTypeDef *huart)
{
    cmd_uart = huart;
    latest_command = HVAC_CMD_INVALID;
    command_valid_flag = 0;
}

void hvac_cmd_parser_process(void)
{
    if (cmd_uart == NULL) return;

    // Polling, non-blocking receive
    if (HAL_UART_Receive(cmd_uart, rx_buf, 1, 0) == HAL_OK)
    {
        uint8_t cmd = rx_buf[0];
        // Accept ASCII '0'..'4' or binary 0..4
        if ((cmd >= '0' && cmd <= '4')) {
            latest_command = cmd - '0';
            command_valid_flag = 1;
        } else if (cmd <= 4) {
            latest_command = cmd;
            command_valid_flag = 1;
        } else {
            // Invalid command, ignore
            command_valid_flag = 0;
        }
    }
}

uint8_t hvac_cmd_parser_get_latest(uint8_t *cmd_valid)
{
    if (cmd_valid) *cmd_valid = command_valid_flag;
    return latest_command;
}