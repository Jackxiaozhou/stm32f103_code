#include "Delay.h"


static u8  fac_us=0;//us
static u16 fac_ms=0;//ms


/****************************************
函数名称: Delay_Rough(int time)
函数参数：延时次数
功能说明：粗延时，供测试用
函数返回：无
编写作者：FRO-PJF01
修改时间：2017.02.22
****************************************/
void Delay_Rough(int time)
{
	int i,j;
	for(i=0;i<time;i++)
	  for(j=0;j<time;j++)
	       ;
}

/****************************************
函数名称: delay_init(u8 SYSCLK)
函数参数：延时次数
功能说明：粗延时初始化，供测试用
函数返回：无
编写作者：FRO-PJF01
修改时间：2017.02.22
****************************************/
void Delay_Init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟，HCLK/8
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
}								    
/****************************************
函数名称: delay_ms(u16 nms)
函数参数：延时次数
功能说明：粗延时，供测试用
函数返回：无
编写作者：FRO-PJF01
修改时间：2017.02.22
****************************************/
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;     //时间加载(SysTick->LOAD?24bit)
	SysTick->VAL =0x00;                //清空计数器
	SysTick->CTRL=0x01 ;               //开始倒数 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达
	SysTick->CTRL=0x00;               //关闭计数器
	SysTick->VAL =0X00;               //清空计数器    
}   
/****************************************
函数名称: delay_us(u32 nus)
函数参数：延时次数
功能说明：粗延时，供测试用
函数返回：无
编写作者：FRO-PJF01
修改时间：2017.02.22
****************************************/	    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us;         //时间加载 		 
	SysTick->VAL=0x00;                //清空计数器
	SysTick->CTRL=0x01 ;              //开始倒数计数
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//时间加载
	SysTick->CTRL=0x00;               //关闭计数器
	SysTick->VAL =0X00;               //清空计数器
}


