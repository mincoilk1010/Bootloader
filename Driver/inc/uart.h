#ifndef uart_h
#define uart_h

#include "stm32f103xx.h"

extern char recv_data[20];
extern int rx_index;
void UART1_init();
void UART1_Send_1byte(char data);
void UART1_Send_String(char *data, u32 len);
char UART1_Read(char *data, u32 timeout);
void printlog(char *format, ...);
#endif




