#include "stm32f10x.h"
#include "sensor.h"
#include "delay.h"
#include "LED.h"
#include "Time.h"
#include "USART.h"
#include "oled.h"

void RCC_Configuration(void);

int main()
{
	u8 sensor=0;
	RCC_Configuration();
	LED_Init();
	Smoke_Init();
	USART1_Init();
	Sensor_Init();
	Delay_Init(72);
	Adc1_Init();
	OLED_Init();
	
	OLED_ShowString(0,0,(u8*)"Current Sensor:",12,1);
	OLED_ShowString(0,32,(u8*)"Sensor Data:",12,1);
	OLED_Refresh();
	while(1)
	{
		sensor=RecognizeSensor();
		switch(sensor)
		{
			case Sensor_Fan:
				//printf("Sensor_Fan\r\n\r\n");
				OLED_ShowString(0,14,(u8*)"Fan",12,1);
				Fan_Init();
				break;
			case Sensor_Body:
				OLED_ShowString(0,14,(u8*)"Body",12,1);//��д���룬��ʾBody,��ִ������������ݲɼ�
			  Body_Fun();
				break;
			case Sensor_Temp:
				OLED_ShowString(0,14,(u8*)"Temp&Humidity",12,1);//ͬ��
			  Temp_Fun();
				break;
			case Sensor_Smoke:
				OLED_ShowString(0,14,(u8*)"Smoke",12,1);//ͬ��
			  Smoke_Fun();
				break;
			default:
				//printf("no sensor\r\n\r\n");
				OLED_ShowString(0,14,(u8*)"                   ",12,1);
				OLED_ShowString(0,32+14,(u8*)"                     ",12,1);
				break;
		}
		OLED_Refresh();

		delay_ms(500);
  }

}


/****************************************
��������: RCC_Configuration(void)
������������
����˵����STM32�˿�ʱ�ӳ�ʼ��
�������أ���
��д���ߣ�FRO-PJF01
�޸�ʱ�䣺2017.02.22
****************************************/
void RCC_Configuration(void)
{
  SystemInit();	  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);
}

 
