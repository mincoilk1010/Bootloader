
#include "gpio.h"
void gpio_init(GPIO_REG_t *port, u8 pin, GPIO_Mode_t mode)
{
    if(pin < 8)
    {
        port->CRL &= ~(0xF << (pin * 4));
        port->CRL |= (mode << (pin * 4));
    }
    else
    {
        port->CRH &= ~(0xF << ((pin - 8) * 4));
        port->CRH |= (mode << ((pin - 8) * 4));
    }
}

void gpio_write(GPIO_REG_t *port, u8 pin, u8 value)
{
   port->ODR &= ~(1 << pin);
   port->ODR |= (value << pin);
}

u8 gpio_read(GPIO_REG_t *port, u8 pin)
{
    return (port->IDR >> pin) & 0x1;
}
void gpio_toggle(GPIO_REG_t *port, u8 pin)
{
    port->ODR ^= (1 << pin);
}
