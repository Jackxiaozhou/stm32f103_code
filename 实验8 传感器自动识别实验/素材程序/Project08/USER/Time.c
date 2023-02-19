#include "Time.h"
#include "stm32f10x_tim.h"
#include "LED.h"

unsigned char LED_Flag = 0;


/****************************************
��������: TIME3_Init(void)
������������
����˵������ʱ��TIM3��ʼ����ʵ��1S�����ж�
�������أ���
��д���ߣ�FRO-PJF01
�޸�ʱ�䣺2017.02.22
****************************************/
void TIME3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM3_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure_TIM3;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);             //ʹ��TIM3��ʱ��
	
	TIM3_TimeBaseInitStructure.TIM_Prescaler = 7199;                // ���÷�Ƶϵ��
	TIM3_TimeBaseInitStructure.TIM_CounterMode= TIM_CounterMode_Up; // ����Ϊ���ϼ���
	TIM3_TimeBaseInitStructure.TIM_Period = 9999;                   // �����Զ����ؼ�������ֵ
	TIM3_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    // ����ʱ�ӷ�Ƶ����
	
	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseInitStructure);            //��ʼ����ʱ��3
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                        //ʹ�ܶ�ʱ��3�ĸ����ж�
	
	NVIC_InitStructure_TIM3.NVIC_IRQChannel = TIM3_IRQn;            // ���ö�ʱ��TIM3���ж�
	NVIC_InitStructure_TIM3.NVIC_IRQChannelPreemptionPriority = 0x01; //�����ж����ȼ�
	NVIC_InitStructure_TIM3.NVIC_IRQChannelSubPriority = 0x01;      //�����ж������ȼ�
	NVIC_InitStructure_TIM3.NVIC_IRQChannelCmd = ENABLE;            //ʹ��TIM3�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure_TIM3);                            //��ʼ��NVIC�Ĵ���
	 
	TIM_Cmd(TIM3,ENABLE);                                           //ʹ�ܶ�ʱ��3
}
/****************************************
��������: TIM3_IRQHandler(void)
������������
����˵������ʱ��TIM3�жϷ������������ж�ʱSTM-PD2�Ʒ�ת�������TIM3���жϱ�־
�������أ���
��д���ߣ�FRO-PJF01
�޸�ʱ�䣺2017.02.22
****************************************/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != 0)  //�ж��Ƿ����TIM3�����ϸ����ж�
	{
		if(LED_Flag == 0)                           //��תLED��
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
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);    //��λTIM3�����ϸ����жϱ�־λ
}
