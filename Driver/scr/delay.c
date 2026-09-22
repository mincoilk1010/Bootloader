#include "clock.h"
#define TIM1_CLK_HZ 72
void delay_init()
{
    TIM1_PCLK_EN();
    TIM1->CR1 &= ~(1 << 0);
    TIM1->ARR  = 0Xffff;
    TIM1->CNT = 0;
    TIM1->PSC = TIM1_CLK_HZ - 1;
    TIM1->CR1 |= 1 << 0;

}
void delay_us(u32 us)
{
    u32 start = TIM1->CNT;
    while((u32)(TIM1->CNT - start) < us);

}
void delay_ms(u32 ms)
{
    while (ms--)
    {
        delay_us(1000);
    }

}
