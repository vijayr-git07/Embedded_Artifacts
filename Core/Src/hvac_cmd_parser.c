#include "hvac_cmd_parser.h"
#include "hvac_bsp.h"

#define HVAC_CMD_UART_BUF_SIZE  4

static uint8_t latest_command = 0;
static uint8_t command_valid_flag = 0;
static uint8_t rx_buf[HVAC_CMD_UART_BUF_SIZE];

void hvac_cmd_parser_init(void)
{
    latest_command = 0;
    command_valid_flag = 0;
}

void hvac_cmd_parser_process(void)
{
    uint8_t rx;
    HAL_StatusTypeDef status;

    status = HAL_UART_Receive(HVAC_BSP_UART_HANDLE, &rx, 1, 0);
    if (status == HAL_OK)
    {
        // Accept ASCII '0'..'4' as valid commands
        if (rx >= '0' && rx <= '4')
        {
            latest_command = rx - '0';
            command_valid_flag = 1;
        }
        else
        {
            // Ignore invalid commands
            command_valid_flag = 0;
        }
    }
}

uint8_t hvac_cmd_parser_get_latest(uint8_t *position, uint8_t *valid)
{
    if (position) *position = latest_command;
    if (valid) *valid = command_valid_flag;
    return latest_command;
}