#include "hvac_command_parser.h"
#include "main.h"
#include "hvac_flap_control.h"

// UART handle from main.c
extern UART_HandleTypeDef huart2;

#define UART_CMD_MIN '0'
#define UART_CMD_MAX '5'

static uint8_t latest_command = 0;
static uint8_t command_valid_flag = 0;
static uint8_t last_reported_command = 0xFF;
static uint8_t command_is_new = 0;

static const char *mode_names[6] = {
    "Initial Mode",        // 0
    "Face Mode",           // 1
    "Face and Foot Mode",  // 2
    "Foot Mode",           // 3
    "Foot and Defrost",    // 4
    "Defrost Mode"         // 5
};

static void log_command(uint8_t cmd)
{
    char msg[64];
    if (cmd <= 5) {
        int len = snprintf(msg, sizeof(msg), "STM32: Command received - %s\r\n", mode_names[cmd]);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, 100);
    }
}

static void log_invalid(void)
{
    const char *msg = "STM32: Invalid command ignored\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
}

void command_parser_init(void)
{
    latest_command = 0;
    command_valid_flag = 0;
    last_reported_command = 0xFF;
    command_is_new = 0;
}

void command_parser_process(void)
{
    uint8_t rx;
    HAL_StatusTypeDef status = HAL_UART_Receive(&huart2, &rx, 1, 0);
    if (status == HAL_OK) {
        if (rx >= UART_CMD_MIN && rx <= UART_CMD_MAX) {
            uint8_t cmd = rx - UART_CMD_MIN;
            if (cmd != latest_command) {
                latest_command = cmd;
                command_valid_flag = 1;
                command_is_new = 1;
                log_command(cmd);
            } else {
                // Same as previous, do not re-log
                command_valid_flag = 1;
                command_is_new = 0;
            }
        } else {
            // Only log once per invalid command
            if (last_reported_command != 0xFE) {
                log_invalid();
                last_reported_command = 0xFE;
            }
            command_valid_flag = 0;
            command_is_new = 0;
        }
    }
}

uint8_t command_parser_get_latest(uint8_t *out_target_pos, uint8_t *out_is_new)
{
    if (command_valid_flag) {
        if (out_target_pos) *out_target_pos = latest_command;
        if (out_is_new) *out_is_new = command_is_new;
        if (command_is_new) {
            last_reported_command = latest_command;
            command_is_new = 0;
        }
        return 1;
    }
    return 0;
}