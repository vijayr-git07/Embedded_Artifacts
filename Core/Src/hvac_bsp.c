#include "hvac_bsp.h"

// Example mapping: 5 LEDs on GPIOC pins 0..4
GPIO_TypeDef* HVAC_BSP_LED_GPIO_Port[HVAC_BSP_LED_COUNT] = {
    GPIOC, GPIOC, GPIOC, GPIOC, GPIOC
};

uint16_t HVAC_BSP_LED_Pin[HVAC_BSP_LED_COUNT] = {
    GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4
};