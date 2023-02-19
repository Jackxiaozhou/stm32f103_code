#include "USART.h"
#include "led.h"

int fputc(int ch, FILE *f) //重定向c库里面的fputc到串口，那么使用printf时就能将打印的信息从串口发送出去，在PC上同串口助手接收信息
{
	//将Printf内容发往串口
	USART_SendData(USART1, (unsigned char) ch);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}

/****************************************
函数名称: USART1_Init(void)
函数参数：无
功能说明：串口1初始化
函数返回：无
编写作者：FRO-PJF01
修改时间：2017.02.22
****************************************/
void USART1_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure_PA9;
	GPIO_InitTypeDef   GPIO_InitStructure_PA10;
	USART_InitTypeDef  USART1_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure_USART1;  
	
	//第一步：启动USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); //开启串口1的时钟
	
	//第二步：设置复用IO口的参数，参见技术手册P110
	//PA9是串口1的TX脚，PA10是串口1的RX脚
	GPIO_InitStructure_PA9.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure_PA9.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure_PA9.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure_PA9);
	
  GPIO_InitStructure_PA10.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure_PA10.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure_PA10.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure_PA10);
	
	//第三步：复位串口1
	USART_DeInit(USART1);
	
	//第四步：设置串口1的通讯参数，基本是115200-8-N-1
	USART1_InitStructure.USART_BaudRate = 115200;
	USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART1_InitStructure.USART_Parity = USART_Parity_No ;
	USART1_InitStructure.USART_StopBits = USART_StopBits_1;
	USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART1_InitStructure);
	 
  //第五步：设置串口1的中断优先级	 
	NVIC_InitStructure_USART1.NVIC_IRQChannel = USART1_IRQn;         //设置串口1的中断
	NVIC_InitStructure_USART1.NVIC_IRQChannelPreemptionPriority = 3; //设置抢占优先级为3
	NVIC_InitStructure_USART1.NVIC_IRQChannelSubPriority = 2;        //设置子优先级为2
	NVIC_InitStructure_USART1.NVIC_IRQChannelCmd = ENABLE;           //使能中断通道
	NVIC_Init(&NVIC_InitStructure_USART1);                           //初始化中断优先级
	
	//第六步：使能串口1
	USART_Cmd(USART1,ENABLE);
	
	//第七步：使能串口1的中断接收
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
}

char mystrncmp(u8* buf1,const u8* buf2,u32 len)
{
	u32 i=0;
	while(len--)
	{
		if(buf1[i]==buf2[i])
		{
			i++;
			continue;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}
u8 rx_flag = 0,count=0;
u8 buffer[64] = {0};
u8 start = 0;
void USART1_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		res = USART_ReceiveData(USART1);

		if(res == 0xFE)								//协议帧头，接收到帧头开始存储
		{
			start = 1;									//start表示开始接收
			count = 0;									//count表示接收到第几个字节
		}
		if(start == 1 && count < 7)		//当开始接收，且接收字节数小于7时执行下面。指令固定为7个字节。
		{
			buffer[count] = res;				//将接收到的指令存储到buffer数组
			count++;
			if(count >= 7)								//接收完全部7个字节指令
			{
				//校验正确
				if(buffer[6] == (buffer[0]+buffer[1]+buffer[2]+buffer[3]+buffer[4]+buffer[5])%256)
				{
					if(buffer[2]==0xA0)	//类型zigbee
					{
						if(buffer[3]==0x02)				//灯光模块
						{
							if(buffer[5]==1)	//打开
							{
								LED1_ON;
							}
							else	//关闭
							{
								LED1_OFF;
							}
						}
						else if(buffer[3]==0x03)	//风扇模块
						{
							if(buffer[5]==1)	//打开
							{
								GPIO_SetBits(GPIOA,GPIO_Pin_0);
							}
							else	//关闭
							{
								GPIO_ResetBits(GPIOA,GPIO_Pin_0);
							}
						}
					}
				}
			}
		}	
	}
	
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);  
}

void UART1_Send_Array(unsigned char send_array[],unsigned char num)
{
	unsigned char i=0; 

	while(i<num)
	{
		USART_SendData(USART1,send_array[i]); 
		while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
		i++;    
	}
        
}
