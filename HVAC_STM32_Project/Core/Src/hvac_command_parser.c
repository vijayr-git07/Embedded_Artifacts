#include "hvac_command_parser.h"

// Use the existing UART handle from main.c
extern UART_HandleTypeDef huart2;

#define UART_CMD_MIN '0'
#define UART_CMD_MAX '5'

static uint8_t latest_command = 0;
static uint8_t command_valid_flag = 0;
static uint8_t last_reported_command = 0xFF;
static uint8_t new_command_flag = 0;
static uint8_t rx_buf = 0;

static const char *mode_names[6] = {
    "Initial Mode",      // 0
    "Face Mode",         // 1
    "Face and Foot Mode",// 2
    "Foot Mode",         // 3
    "Defrost Mode",      // 4
    "Full Open",         // 5
};

static void log_invalid_command(void)
{
    static uint8_t last_invalid = 0;
    static uint8_t last_invalid_reported = 0;
    if (last_invalid != last_invalid_reported) {
        const char *msg = "STM32: Invalid command ignored\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint16_t)strlen(msg), 100);
        last_invalid_reported = last_invalid;
    }
}

void command_parser_init(void)
{
    latest_command = 0;
    command_valid_flag = 0;
    last_reported_command = 0xFF;
    new_command_flag = 0;
    rx_buf = 0;
}

void command_parser_process(void)
{
    uint8_t ch;
    HAL_StatusTypeDef status = HAL_UART_Receive(&huart2, &ch, 1, 0);
    if (status == HAL_OK) {
        if (ch >= UART_CMD_MIN && ch <= UART_CMD_MAX) {
            uint8_t pos = ch - UART_CMD_MIN;
            if (pos != latest_command) {
                latest_command = pos;
                command_valid_flag = 1;
                new_command_flag = 1;
            }
        } else {
            // Only log once per invalid command
            static uint8_t last_invalid = 0;
            last_invalid++;
            log_invalid_command();
        }
    }
}

uint8_t command_parser_get_latest(uint8_t *out_position, uint8_t *out_is_new)
{
    if (command_valid_flag) {
        if (out_position) *out_position = latest_command;
        if (out_is_new) *out_is_new = new_command_flag;
        new_command_flag = 0;
        return 1;
    }
    return 0;
}