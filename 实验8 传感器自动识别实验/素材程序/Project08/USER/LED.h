#ifndef _LED_H
     #define _LED_H

#include "stm32f10x.h"
void LED_Init(void);


#define  LED1_OFF   GPIO_SetBits(GPIOD,GPIO_Pin_2)          //Ϩ��STM-PD2��
#define  LED1_ON    GPIO_ResetBits(GPIOD,GPIO_Pin_2)        //����STM-PD2��

#endif
