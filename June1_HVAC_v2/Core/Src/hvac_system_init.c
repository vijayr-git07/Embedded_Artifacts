#include "hvac_system_init.h"
#include "hvac_command_parser.h"
#include "hvac_feedback_processor.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"
#include "hvac_flap_control.h"
#include "main.h"

// UART handle from main.c
extern UART_HandleTypeDef huart2;

void system_init(void)
{
    command_parser_init();
    feedback_processor_init();
    motor_driver_init();
    led_status_init();
    flap_control_init();

    // Initial status log
    const char *msg = "STM32: HVAC controller ready, all LEDs OFF\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
}