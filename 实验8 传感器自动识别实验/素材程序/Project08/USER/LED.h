#ifndef _LED_H
     #define _LED_H

#include "stm32f10x.h"
void LED_Init(void);


#define  LED1_OFF   GPIO_SetBits(GPIOD,GPIO_Pin_2)          //œ®√STM-PD2µ∆
#define  LED1_ON    GPIO_ResetBits(GPIOD,GPIO_Pin_2)        //µ„¡¡STM-PD2µ∆

#endif
