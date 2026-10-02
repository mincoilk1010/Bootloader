/*
 * stm32f103xx.h
 *
 *  Created on: Aug 10, 2026
 *      Author: GB Center
 */

#ifndef INC_STM32F103XX_H_
#define INC_STM32F103XX_H_


#include "stdio.h"
#include "stddef.h"
#include "stdint.h"
#include "string.h"
#define _vo   volatile
#define __weak __attribute__((weak))


typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int32_t  s32;
typedef int16_t  s16;
typedef int8_t   s8;

typedef float f32;

// dn ban do bo nho (Memory Map)
// base addresses of Flash and SRAM memories
#define FLASH_BASEADDR         0x40022000U 
//#define SRAM_BASEADDR         0x20000000U
//#define ROM_BASEADDR           0x1FFF0000U

//AHBx and APBx Bus Peripheral base assresses
//dn addr base cua cac mien bus khac nhau
#define PERIPH_BASEADDR            0x40000000U
#define APB1PERIPH_BASEADDR        PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR        0x40010000U
#define AHB1PERIPH_BASEADDR        0x40020000U
#define AHB2PERIPH_BASEADDR        0X50000000U


#define GPIOA_BASEADDR        (APB2PERIPH_BASEADDR + 0x0800)  // day la dia chi that trong khong gian nho do hang  dinh nghia
#define GPIOB_BASEADDR        (APB2PERIPH_BASEADDR + 0x0C00)
#define GPIOC_BASEADDR        (APB2PERIPH_BASEADDR + 0x1000)
#define GPIOD_BASEADDR        (APB2PERIPH_BASEADDR + 0x1400)
#define GPIOE_BASEADDR        (APB2PERIPH_BASEADDR + 0X1800)
#define GPIOF_BASEADDR        (APB2PERIPH_BASEADDR + 0x1C00)
#define GPIOG_BASEADDR        (APB2PERIPH_BASEADDR + 0x2000)
#define RCC_BASEADDR          (AHB1PERIPH_BASEADDR + 0x1000)


#define TIM1_BASEADDR      (APB2PERIPH_BASEADDR + 0x2C00)
#define TIM8_BASEADDR      (APB2PERIPH_BASEADDR + 0x3400)
#define TIM2_BASEADDR      (APB1PERIPH_BASEADDR + 0x0000)
#define TIM3_BASEADDR      (APB1PERIPH_BASEADDR + 0x0400)
#define TIM4_BASEADDR      (APB1PERIPH_BASEADDR + 0x0800)
#define TIM5_BASEADDR      (APB1PERIPH_BASEADDR + 0x0C00)
#define TIM6_BASEADDR      (APB1PERIPH_BASEADDR + 0x1000)
#define TIM7_BASEADDR      (APB1PERIPH_BASEADDR + 0x1400)
#define TIM12_BASEADDR      (APB1PERIPH_BASEADDR + 0x1800)
#define TIM13_BASEADDR      (APB1PERIPH_BASEADDR + 0x1C00)
#define TIM14_BASEADDR      (APB1PERIPH_BASEADDR + 0x2000)


#define I2C1_BASE         (APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASE         (APB1PERIPH_BASEADDR + 0x5800)

#define SPI1_BASE         (APB2PERIPH_BASEADDR + 0x3000)
#define SPI2_BASE         (APB1PERIPH_BASEADDR + 0x3800)
#define SPI3_BASE         (APB1PERIPH_BASEADDR + 0x3C00)

#define ADC1_BASEADDR      (APB2PERIPH_BASEADDR + 0x2800)
#define ADC2_BASEADDR      (APB2PERIPH_BASEADDR + 0x2400)

#define USART1_BASE         (APB2PERIPH_BASEADDR + 0x3800)
#define USART2_BASE       (APB1PERIPH_BASEADDR + 0x4400)
#define USART3_BASE       (APB1PERIPH_BASEADDR + 0x4800)
#define UART4_BASE        (APB1PERIPH_BASEADDR + 0x4C00)
#define UART5_BASE        (APB1PERIPH_BASEADDR + 0x5000)

typedef struct
{
    _vo u32 CR;
    _vo u32 CFGR;
    _vo u32 CIR;
    _vo u32 APB2RSTR;
    _vo u32 APB1RSTR;
    _vo u32 AHBENR;
    _vo u32 APB2ENR;
    _vo u32 APB1ENR;
    _vo u32 BDCR;
    _vo u32 CSR;
    _vo u32 AHBSTR;
    _vo u32 CFGR2;

}RCC_REG_t;

typedef struct
{
    _vo u32 ACR;
    _vo u32 KEYR;
    _vo u32 OPTKEYR;
    _vo u32 SR;
    _vo u32 CR;
    _vo u32 AR;
        u32 Reserved;
    _vo u32 OBR;
    _vo u32 WRPR;
}Flash_REG_t;

typedef struct
{
    _vo u32 CRL;
    _vo u32 CRH;
    _vo u32 IDR;
    _vo u32 ODR;
    _vo u32 BSRR;
    _vo u32 BRR;
    _vo u32 LCKR;

}GPIO_REG_t;


typedef struct
{
    _vo u32 CR1;
    _vo u32 CR2;
    _vo u32 SMCR;
    _vo u32 DIER;
    _vo u32 SR;
    _vo u32 EGR;
    _vo u32 CCMR1;
    _vo u32 CCMR2;
    _vo u32 CCER;
    _vo u32 CNT;
    _vo u32 PSC;
    _vo u32 ARR;
    _vo u32 CCR1;
    _vo u32 CCR2;
    _vo u32 CCR3;
    _vo u32 CCR4;
    _vo u32 BDTR;
    _vo u32 DCR;
    _vo u32 DMAR;

}TIM_REG_t;

typedef struct
{
    _vo u32 SR;
    _vo u32 CR1;
    _vo u32 CR2;
    _vo u32 SMPR1;
    _vo u32 SMPR2;
    _vo u32 JOFR1;
    _vo u32 JOFR2;
    _vo u32 JOFR3;
    _vo u32 JOFR4;
    _vo u32 HTR;
    _vo u32 LTR;
    _vo u32 SQR1;
    _vo u32 SQR2;
    _vo u32 SQR3;
    _vo u32 JSQR;
    _vo u32 JDR1;
    _vo u32 JDR2;
    _vo u32 JDR3;
    _vo u32 JDR4;
    _vo u32 DR;

}ADC_REG_t;

typedef struct
{
    _vo u32 CR1;
    _vo u32 CR2;
    _vo u32 OAR1;
    _vo u32 OAR2;
    _vo u32 DR;
    _vo u32 SR1;
    _vo u32 SR2;
    _vo u32 CCR;
    _vo u32 TRISE;

}I2C_REG_t;

typedef struct
{
    _vo u32 CR1;
    _vo u32 CR2;
    _vo u32 SR;
    _vo u32 DR;
    _vo u32 CRCPR;
    _vo u32 RXCRCR;
    _vo u32 TXCRCR;
    _vo u32 I2SCFGR;
    _vo u32 I2SPR;
    
}SPI_REG_t;


typedef struct
{
    _vo u32 ISR;
    _vo u32 IFCR;
    _vo u32 CCR1;
    _vo u32 CNDTR1;
    _vo u32 CPAR1;
    _vo u32 CMAR1;
        u32 Reserved;
    _vo u32 CCR2;
    _vo u32 CNDTR2;
    _vo u32 CPAR2;
    _vo u32 CMAR2;
        u32 Reserved1; 
    _vo u32 CCR3;
    _vo u32 CNDTR3;
    _vo u32 CPAR3;
    
}DMA_REG_t;

typedef struct
{
    _vo u32 SR;
    _vo u32 DR;
    _vo u32 BRR;
    _vo u32 CR1;
    _vo u32 CR2;
    _vo u32 CR3;
    _vo u32 GTPR;
}UART_REG_t;


#define GPIOA      ((GPIO_REG_t*)GPIOA_BASEADDR)   // GPIO_RegDef_t *pGPIOA = (GPIO_RegDef_t*)0x40020000;
#define GPIOB      ((GPIO_REG_t*)GPIOB_BASEADDR)
#define GPIOC      ((GPIO_REG_t*)GPIOC_BASEADDR)
#define GPIOD      ((GPIO_REG_t*)GPIOD_BASEADDR)
#define GPIOE      ((GPIO_REG_t*)GPIOE_BASEADDR)
#define GPIOF      ((GPIO_REG_t*)GPIOF_BASEADDR)
#define GPIOG      ((GPIO_REG_t*)GPIOG_BASEADDR)

#define RCC        ((RCC_REG_t*)RCC_BASEADDR)

#define TIM1        ((TIM_REG_t*)TIM1_BASEADDR)
#define TIM2        ((TIM_REG_t*)TIM2_BASEADDR)
#define TIM3        ((TIM_REG_t*)TIM3_BASEADDR)
#define TIM4        ((TIM_REG_t*)TIM4_BASEADDR)


#define Flash       ((Flash_REG_t*)FLASH_BASEADDR)


#define USART1      ((UART_REG_t*)USART1_BASE)
#define USART2      ((UART_REG_t*)USART2_BASE)
#define USART3      ((UART_REG_t*)USART3_BASE)


#include "clock.h"

#endif /* INC_STM32F103XX_H_ */
