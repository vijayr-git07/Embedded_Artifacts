#include "hvac_system_init.h"
#include "hvac_command_parser.h"
#include "hvac_feedback_processor.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"
#include "main.h"

// UART handle from main.c
extern UART_HandleTypeDef huart2;

static void log_uart(const char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint16_t)strlen(msg), 100);
}

void system_init(void)
{
    command_parser_init();
    feedback_processor_init();
    motor_driver_init();
    led_status_init();

    // Read initial feedback position
    feedback_processor_update();
    uint8_t pos = 0xFF;
    if (feedback_processor_get_position(&pos))
        led_status_set_position(pos);
    else
        led_status_set_position(0);

    // Initial log
    log_uart("STM32: HVAC controller ready, all LEDs OFF\r\n");
}