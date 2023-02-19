#include "LED.h"

/****************************************
函数名称: LED_Init(void)
函数参数：无
功能说明：STM-PD2进行IO口输出初始化
函数返回：无
编写作者：FRO-PJF01
修改时间：2017.02.22
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
