/*
 * tim.c
 *
 *  Created on: Sep 23, 2026
 *      Author: Admin
 */

#include "tim.h"

void tim_int() // tim2
{
	TIM2_PCLK_EN();
	TIM2->CR1 &= ~(1 << 0);
	TIM2->PSC = 35;
	TIM2->ARR = 999;
	TIM2->CNT = 0;
	TIM2->EGR |= 1 << 0;
	TIM2->DIER |= 1 << 0;
	TIM2->CR1 |= 1 << 0;
}
