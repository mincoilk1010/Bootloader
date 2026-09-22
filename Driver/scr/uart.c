#include "uart.h"
#include "clock.h"
#include "gpio.h"
char recv_data[20];
int rx_index;
void UART1_init(u32 baud)
{
    GPIOA_PCLK_EN();
    //PA9->TX
    gpio_init(GPIOA, PIN_9, GPIO_MODE_OUTPUT_50MHZ);
    //PA10->RX
    gpio_init(GPIOA, PIN_10, GPIO_MODE_INPUT_FLOATING);

    USART1_PCLK_EN();
    USART1->BRR = 72000000/baud;

    //USART1->CR1 &= ~(1 << 9);
    //USART1->CR1 &= ~(1 << 12);
    USART1->CR1 |= (1 << 3) | (1 << 2) | (1 << 13);
}
void UART1_Send_1byte(char data)
{
    while(!(USART1->SR & (1 << 7)));
    USART1->DR = data;

}
void UART1_Send_String(char *data, u32 len)
{
    for(u32 i = 0; i < len; i++)
    {
        UART1_Send_1byte(data[i]);
    }
}

char UART1_Read(char *data, u32 timeout)
{
    while(!(USART1->SR & (1 << 5)))
    {
    }
    *data = USART1->DR;

}
void printlog(char *format, ...);