#include "hvac_bsp.h"

// UART2 for command interface
UART_HandleTypeDef huart2;

// ADC1 for potentiometer feedback
ADC_HandleTypeDef hadc1;

// TIM3 for motor PWM
TIM_HandleTypeDef htim3;

// GPIO initialization is assumed to be handled by CubeMX-generated code.
// This function can be used to perform any additional BSP setup if needed.
void hvac_bsp_init(void)
{
    // No additional BSP init required; CubeMX handles peripheral init.
    // Ensure all outputs are in safe state if needed.
}