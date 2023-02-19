#include "Delay.h"


static u8  fac_us=0;//us
static u16 fac_ms=0;//ms


/****************************************
��������: Delay_Rough(int time)
������������ʱ����
����˵��������ʱ����������
�������أ���
��д���ߣ�FRO-PJF01
�޸�ʱ�䣺2017.02.22
****************************************/
void Delay_Rough(int time)
{
	int i,j;
	for(i=0;i<time;i++)
	  for(j=0;j<time;j++)
	       ;
}

/****************************************
��������: delay_init(u8 SYSCLK)
������������ʱ����
����˵��������ʱ��ʼ������������
�������أ���
��д���ߣ�FRO-PJF01
�޸�ʱ�䣺2017.02.22
****************************************/
void Delay_Init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ�ӣ�HCLK/8
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
}								    
/****************************************
��������: delay_ms(u16 nms)
������������ʱ����
����˵��������ʱ����������
�������أ���
��д���ߣ�FRO-PJF01
�޸�ʱ�䣺2017.02.22
****************************************/
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;     //ʱ�����(SysTick->LOAD?24bit)
	SysTick->VAL =0x00;                //��ռ�����
	SysTick->CTRL=0x01 ;               //��ʼ���� 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��
	SysTick->CTRL=0x00;               //�رռ�����
	SysTick->VAL =0X00;               //��ռ�����    
}   
/****************************************
��������: delay_us(u32 nus)
������������ʱ����
����˵��������ʱ����������
�������أ���
��д���ߣ�FRO-PJF01
�޸�ʱ�䣺2017.02.22
****************************************/	    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us;         //ʱ����� 		 
	SysTick->VAL=0x00;                //��ռ�����
	SysTick->CTRL=0x01 ;              //��ʼ��������
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//ʱ�����
	SysTick->CTRL=0x00;               //�رռ�����
	SysTick->VAL =0X00;               //��ռ�����
}


