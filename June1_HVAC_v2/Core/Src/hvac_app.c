#include "hvac_app.h"
#include "hvac_command_parser.h"
#include "hvac_feedback_processor.h"
#include "hvac_flap_control.h"
#include "hvac_motor_driver.h"
#include "hvac_led_status.h"
#include "hvac_system_init.h"

// UART handle provided by CubeMX project
extern UART_HandleTypeDef huart2;

static uint8_t startup_log_printed = 0;

void hvac_app_init(void)
{
    system_init();

    // Print initial status log ONCE
    if (!startup_log_printed) {
        const char *msg = "STM32: HVAC controller ready, all LEDs OFF\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint16_t)strlen(msg), 100);
        startup_log_printed = 1;
    }
}

void hvac_app_task(void)
{
    // 1. Process UART commands
    command_parser_process();

    // 2. Read ADC feedback
    feedback_processor_update();

    // 3. Update flap position state (handled in feedback_processor_update)

    // 4. Execute flap control logic (handles motor and LED requests)
    flap_control_process();

    // 5. Update motor outputs (handled by flap_control_process)

    // 6. Update LED status (handled by flap_control_process)
}