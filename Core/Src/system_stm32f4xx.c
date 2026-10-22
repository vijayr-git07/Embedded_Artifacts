#include "stm32f4xx.h"

void SystemInit(void)
{
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2)); // Enable FPU
    RCC->CR |= RCC_CR_HSION;
    RCC->CFGR = 0x00000000U;
    RCC->CR &= (uint32_t)0xFEF6FFFFU;
    RCC->PLLCFGR = 0x24003010U;
    RCC->CR &= (uint32_t)0xFFFBFFFFU;
    RCC->CIR = 0x00000000U;
}