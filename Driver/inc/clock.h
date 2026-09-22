#ifndef CLOCK_H
#define CLOCK_H
#include "stddef.h"
#include "stdint.h"
#include "stm32f103xx.h"


#define GPIOA_PCLK_EN()     	(RCC->APB2ENR |= (1 << 2))
#define GPIOB_PCLK_EN()     	(RCC->APB2ENR |= (1 << 3))
#define GPIOC_PCLK_EN()     	(RCC->APB2ENR |= (1 << 4))
#define GPIOD_PCLK_EN()     	(RCC->APB2ENR |= (1 << 5))
#define GPIOE_PCLK_EN()     	(RCC->APB2ENR |= (1 << 6))
#define GPIOF_PCLK_EN()     	(RCC->APB2ENR |= (1 << 7))



#define TIM1_PCLK_EN()     	(RCC->APB2ENR|= (1 << 11))
#define TIM2_PCLK_EN()     	(RCC->APB1ENR|= (1 << 0))
#define TIM3_PCLK_EN()     	(RCC->APB1ENR|= (1 << 1))
#define TIM4_PCLK_EN()     	(RCC->APB1ENR|= (1 << 2))

#define USART2_PCLK_EN()     	(RCC->APB1ENR|= (1 << 17))
#define USART3_PCLK_EN()     	(RCC->APB1ENR|= (1 << 18))
#define	USART1_PCLK_EN()     	(RCC->APB2ENR|= (1 << 14))
/*
typedef enum
{
	GPIOA_peripheral = 0,
	GPIOB_peripheral,
	GPIOC_peripheral,
	GPIOD_peripheral,
	GPIOE_peripheral,
	GPIOF_peripheral,
	GPIOG_peripheral,
	GPIOH_peripheral = 7,
	CRC_peripheral = 12,
	DAM1_peripheral = 21,
	DMA2_peripheral

}AHB1_peripheral_t;

typedef enum
{
	DCMI_peripheral = 0,
	CRYP_peripheral = 4,
	HASH_peripheral,
	RNG_peripheral,
	OTGFS_peripheral
}AHB2_peripheral_t;

typedef enum
{
	TIM2_peripheral = 0,
	TIM3_peripheral,
	TIM4_peripheral,
	TIM5_peripheral,
	TIM6_peripheral,
	TIM7_peripheral,
	TIM12_peripheral,
	TIM13_peripheral,
	WWDG_peripheral = 11,
	SPI2_peripheral = 14,
	SPI3_peripheral,
	USART2_peripheral = 17,
	USART3_peripheral,
	USART4_peripheral,
	USART5_peripheral,
	I2C1_peripheral,
	I2C2_peripheral,
	I2C3_peripheral,
	CAN1_peripheral = 25,
	CAN2_peripheral,
	PWR_peripheral = 28,
	DAC_peripheral

}APB1_peripheral_t;

typedef enum
{
	TIM1_peripheral = 0,
	TIM8_peripheral,
	USART1_peripheral = 4,
	USART6_peripheral,
	ADC1_peripheral = 8,
	ADC2_peripheral,
	ADC3_peripheral,
	SDIO_peripheral,
	SPI1_peripheral,
	SYSCFG_peripheral = 14,
	TIM9_peripheral = 16,
	TIM10_peripheral,
	TIM11_peripheral


}APB2_peripheral_t;

void clock_enable_AHB1(AHB1_peripheral_t peripheral);
void clock_enable_AHB2(AHB2_peripheral_t peripheral);
void clock_enable_APB1(APB1_peripheral_t peripheral);
void clock_enable_APB2(APB2_peripheral_t peripheral);
*/
void clock_init();
#endif