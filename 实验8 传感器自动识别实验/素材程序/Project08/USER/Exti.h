#ifndef _EXTI_H
   #define _EXTI_H
#include "stm32f10x.h"
void EXTIX_Init(void);

#define  KEY_STM_PA8   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)          //Ï¨ÃðSTM-PD2µÆ

#endif
