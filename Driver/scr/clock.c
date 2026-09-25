#include "clock.h"

#define HSI_CLOCK_HZ       8000000U
#define HSE_STARTUP_TIMEOUT 1000000U

u32 SystemCoreClockHz = HSI_CLOCK_HZ;

void clock_init(void)
{
    u32 timeout = HSE_STARTUP_TIMEOUT;

    RCC->CR |= 1 << 16;  // enable HSE
    while (((RCC->CR & (1 << 17)) == 0U) && (timeout-- != 0U))
    {
    }


    if ((RCC->CR & (1 << 17)) == 0U)
    {
        return; // HSI is enabled after reset and runs at 8 MHz.
    }

    Flash->ACR &= ~(7 << 0);
    Flash->ACR |= (2 << 0);
    Flash->ACR |= 1 << 4;

    RCC->CFGR &= ~(0b111 << 8);// APB1 = HCLK / 2
    RCC->CFGR |= 4 << 8;
    RCC->CFGR &= ~(0b111 << 11);// APB2 = HCLK /1
    RCC->CFGR &= ~(1 << 17);    // PLL input is HSE, not HSE / 2
    RCC->CFGR |=  1 << 16; // PLL soure = HSE
    RCC->CFGR &= ~(0b1111 << 18);
    RCC->CFGR |= 7 << 18;  // PLL = HSE x9;
    RCC->CR |= 1 << 24; // enable PLL
    timeout = HSE_STARTUP_TIMEOUT;
    while (((RCC->CR & (1 << 25)) == 0U) && (timeout-- != 0U))
    {
    }
    if ((RCC->CR & (1 << 25)) == 0U)
    {
        return;
    }
    RCC->CFGR &= ~(0x3 << 0);
    RCC->CFGR |= (0x2 << 0);

    while((RCC->CFGR & (0x3 << 2)) != (0x2 << 2));

    SystemCoreClockHz = 72000000U;
}
