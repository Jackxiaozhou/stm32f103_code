#include "Exti.h"
#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "Key.h"

unsigned char LED_Flag;
/****************************************
函数名称: EXTIX_Init(void)
函数参数：无
功能说明：设置中断线8到PA8上面，并初始化中断，设置优先级
函数返回：无
编写作者：FRO-PJF01
修改时间：2017.02.22
****************************************/
void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure_PA8;
	NVIC_InitTypeDef NVIC_InitStructure_PA8;
	
	Key_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);      //中断线映射
	EXTI_InitStructure_PA8.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure_PA8.EXTI_Mode = EXTI_Mode_Interrupt;         //定义为外部中断模式
	EXTI_InitStructure_PA8.EXTI_Trigger = EXTI_Trigger_Falling;     //下降延触发模式
	EXTI_InitStructure_PA8.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure_PA8);
	
	NVIC_InitStructure_PA8.NVIC_IRQChannel = EXTI9_5_IRQn;           //中断8通道
	NVIC_InitStructure_PA8.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级
	NVIC_InitStructure_PA8.NVIC_IRQChannelSubPriority = 0x01;        //抢占子优先级
	NVIC_InitStructure_PA8.NVIC_IRQChannelCmd = ENABLE;              //使能通道8中断
	NVIC_Init(&NVIC_InitStructure_PA8);	
}

/****************************************
函数名称: EXTI9_5_IRQHandler(void)
函数参数：无
功能说明：中断5到9的函数，也即包括了中断8的函数
函数返回：无
编写作者：FRO-PJF01
修改时间：2017.02.22
****************************************/
void EXTI9_5_IRQHandler(void)
{
	Delay_Rough(200);    //延时消抖
	if(KEY_STM_PA8 == 0) 
	{
			if(LED_Flag == 0)
	    {
		    LED1_ON;
		    LED_Flag = 1;
      }
	    else
	    {
		    LED1_OFF;
		    LED_Flag = 0;
      }
  }
	
	
	EXTI_ClearITPendingBit(EXTI_Line8); //清除中断8
}
