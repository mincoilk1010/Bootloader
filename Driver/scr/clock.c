#include "clock.h"
void clock_init()
{
    RCC->CR |= 1 << 16;  // enable HSE
    while((RCC->CR & (1 << 17)) == 0);  // wait HSE ready
    Flash->ACR &= ~(7 << 0);
    Flash->ACR |= (2 << 0);
    Flash->ACR |= 1 << 4;

    RCC->CFGR &= ~(0b111 << 8);// APB1 = HCLK / 2
    RCC->CFGR |= 4 << 8;
    RCC->CFGR &= ~(0b111 << 11);// APB2 = HCLK /1
    RCC->CFGR |=  1 << 16; // PLL soure = HSE
    RCC->CFGR &= ~(0b1111 << 18);
    RCC->CFGR |= 7 << 18;  // PLL = HSE x9;
    RCC->CR |= 1 << 24; // enable PLL
    while((RCC->CR & (1 << 25)) == 0); // wait PLL ready
    RCC->CFGR &= ~(0x3 << 0);
    RCC->CFGR |= (0x2 << 0);

    while((RCC->CFGR & (0x3 << 2)) != (0x2 << 2));

}
