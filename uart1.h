#ifndef _UART_H
#define _UART_H
#include <stdio.h>
void usart1_Config();
void myprintf(const char *str);
void USART1_IRQHandler();
int fputc(int ch,FILE *fp);
#endif