#ifndef _USART_H
     #define _USART_H
		 
#include "stm32f10x.h"		 
#include <stdio.h>

int fputc(int ch, FILE *f);
void USART1_Init(void);
void USART1_IRQHandler(void);
void UART1_Send_Array(unsigned char send_array[],unsigned char num);


#endif
