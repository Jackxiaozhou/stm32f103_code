#include "LED.h"

/****************************************
��������: LED_Init(void)
������������
����˵����STM-PD2����IO�������ʼ��
�������أ���
��д���ߣ�FRO-PJF01
�޸�ʱ�䣺2017.02.22
****************************************/

void LED_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	LED1_OFF;
}
