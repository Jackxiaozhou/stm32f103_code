#include "Time.h"
#include "stm32f10x_tim.h"
#include "LED.h"

unsigned char LED_Flag = 0;


/****************************************
函数名称: TIME3_Init(void)
函数参数：无
功能说明：定时器TIM3初始化，实现1S产生中断
函数返回：无
编写作者：FRO-PJF01
修改时间：2017.02.22
****************************************/
void TIME3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM3_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure_TIM3;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);             //使能TIM3的时钟
	
	TIM3_TimeBaseInitStructure.TIM_Prescaler = 7199;                // 设置分频系数
	TIM3_TimeBaseInitStructure.TIM_CounterMode= TIM_CounterMode_Up; // 设置为向上计数
	TIM3_TimeBaseInitStructure.TIM_Period = 9999;                   // 设置自动重载计数周期值
	TIM3_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    // 设置时钟分频因子
	
	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseInitStructure);            //初始化定时器3
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                        //使能定时器3的更新中断
	
	NVIC_InitStructure_TIM3.NVIC_IRQChannel = TIM3_IRQn;            // 设置定时器TIM3的中断
	NVIC_InitStructure_TIM3.NVIC_IRQChannelPreemptionPriority = 0x01; //设置中断优先级
	NVIC_InitStructure_TIM3.NVIC_IRQChannelSubPriority = 0x01;      //设置中断子优先级
	NVIC_InitStructure_TIM3.NVIC_IRQChannelCmd = ENABLE;            //使能TIM3中断通道
	NVIC_Init(&NVIC_InitStructure_TIM3);                            //初始化NVIC寄存器
	 
	TIM_Cmd(TIM3,ENABLE);                                           //使能定时器3
}
/****************************************
函数名称: TIM3_IRQHandler(void)
函数参数：无
功能说明：定时器TIM3中断服务函数，产生中断时STM-PD2灯翻转，并清除TIM3的中断标志
函数返回：无
编写作者：FRO-PJF01
修改时间：2017.02.22
****************************************/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != 0)  //判断是否产生TIM3的向上更新中断
	{
		if(LED_Flag == 0)                           //翻转LED灯
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
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);    //复位TIM3的向上更新中断标志位
}
