#include "USART.h"
#include "led.h"

int fputc(int ch, FILE *f) //�ض���c�������fputc�����ڣ���ôʹ��printfʱ���ܽ���ӡ����Ϣ�Ӵ��ڷ��ͳ�ȥ����PC��ͬ�������ֽ�����Ϣ
{
	//��Printf���ݷ�������
	USART_SendData(USART1, (unsigned char) ch);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}

/****************************************
��������: USART1_Init(void)
������������
����˵��������1��ʼ��
�������أ���
��д���ߣ�FRO-PJF01
�޸�ʱ�䣺2017.02.22
****************************************/
void USART1_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure_PA9;
	GPIO_InitTypeDef   GPIO_InitStructure_PA10;
	USART_InitTypeDef  USART1_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure_USART1;  
	
	//��һ��������USART1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); //��������1��ʱ��
	
	//�ڶ��������ø���IO�ڵĲ������μ������ֲ�P110
	//PA9�Ǵ���1��TX�ţ�PA10�Ǵ���1��RX��
	GPIO_InitStructure_PA9.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure_PA9.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure_PA9.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure_PA9);
	
  GPIO_InitStructure_PA10.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure_PA10.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure_PA10.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure_PA10);
	
	//����������λ����1
	USART_DeInit(USART1);
	
	//���Ĳ������ô���1��ͨѶ������������115200-8-N-1
	USART1_InitStructure.USART_BaudRate = 115200;
	USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART1_InitStructure.USART_Parity = USART_Parity_No ;
	USART1_InitStructure.USART_StopBits = USART_StopBits_1;
	USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART1_InitStructure);
	 
  //���岽�����ô���1���ж����ȼ�	 
	NVIC_InitStructure_USART1.NVIC_IRQChannel = USART1_IRQn;         //���ô���1���ж�
	NVIC_InitStructure_USART1.NVIC_IRQChannelPreemptionPriority = 3; //������ռ���ȼ�Ϊ3
	NVIC_InitStructure_USART1.NVIC_IRQChannelSubPriority = 2;        //���������ȼ�Ϊ2
	NVIC_InitStructure_USART1.NVIC_IRQChannelCmd = ENABLE;           //ʹ���ж�ͨ��
	NVIC_Init(&NVIC_InitStructure_USART1);                           //��ʼ���ж����ȼ�
	
	//��������ʹ�ܴ���1
	USART_Cmd(USART1,ENABLE);
	
	//���߲���ʹ�ܴ���1���жϽ���
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
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		res = USART_ReceiveData(USART1);

		if(res == 0xFE)								//Э��֡ͷ�����յ�֡ͷ��ʼ�洢
		{
			start = 1;									//start��ʾ��ʼ����
			count = 0;									//count��ʾ���յ��ڼ����ֽ�
		}
		if(start == 1 && count < 7)		//����ʼ���գ��ҽ����ֽ���С��7ʱִ�����档ָ��̶�Ϊ7���ֽڡ�
		{
			buffer[count] = res;				//�����յ���ָ��洢��buffer����
			count++;
			if(count >= 7)								//������ȫ��7���ֽ�ָ��
			{
				//У����ȷ
				if(buffer[6] == (buffer[0]+buffer[1]+buffer[2]+buffer[3]+buffer[4]+buffer[5])%256)
				{
					if(buffer[2]==0xA0)	//����zigbee
					{
						if(buffer[3]==0x02)				//�ƹ�ģ��
						{
							if(buffer[5]==1)	//��
							{
								LED1_ON;
							}
							else	//�ر�
							{
								LED1_OFF;
							}
						}
						else if(buffer[3]==0x03)	//����ģ��
						{
							if(buffer[5]==1)	//��
							{
								GPIO_SetBits(GPIOA,GPIO_Pin_0);
							}
							else	//�ر�
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
