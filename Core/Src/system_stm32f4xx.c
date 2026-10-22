#include "stm32f4xx.h"

void SystemInit(void)
{
    // Reset RCC clock configuration to default reset state
    RCC->CR |= (uint32_t)0x00000001;
    RCC->CFGR = 0x00000000;
    RCC->CR &= (uint32_t)0xFEF6FFFF;
    RCC->PLLCFGR = 0x24003010;
    RCC->CR &= (uint32_t)0xFFFBFFFF;
    RCC->CIR = 0x00000000;
    // Vector Table Relocation in internal SRAM if needed
}