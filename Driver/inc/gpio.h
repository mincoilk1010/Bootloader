#ifndef GPIO_H_H
#define GPIO_H_H

#include "stm32f103xx.h"
/*
 * GPIO_PIN_NUMBERS
 *
 */

#define PIN_0        0
#define PIN_1        1
#define PIN_2        2
#define PIN_3        3
#define PIN_4        4
#define PIN_5        5
#define PIN_6        6
#define PIN_7        7
#define PIN_8        8
#define PIN_9        9
#define PIN_10       10
#define PIN_11       11
#define PIN_12       12
#define PIN_13       13
#define PIN_14       14
#define PIN_15       15

typdef enum
{
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT_10MHZ = 1,
    GPIO_MODE_OUTPUT_2MHZ = 2,
    GPIO_MODE_OUTPUT_50MHZ = 3
} GPIO_Mode_t;

void gpio_init(GPIO_REG_t *port, u8 pin, GPIO_Mode_t mode);
void gpio_write(GPIO_REG_t *port, u8 pin, u8 value);
u8 gpio_read(GPIO_REG_t *port, u8 pin);
#endif