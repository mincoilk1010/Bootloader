#include "clock.h"

void delay_init(void)
{
    TIM1_PCLK_EN();
    TIM1->CR1 &= ~(1 << 0);
    TIM1->ARR  = 0Xffff;
    TIM1->CNT = 0;

    TIM1->PSC = (SystemCoreClockHz / 1000000U) - 1U;
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
