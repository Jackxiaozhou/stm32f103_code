#include "sensor.h"
#include "delay.h"
#include "oled.h"
#include "usart.h"
/*******************ʶ�𴫸������ܲ���**************/
//��������ʼ��
void Sensor_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}

//ʶ�𴫸�������
u8 RecognizeSensor(void)
{
	u8 sensor=0;
	if(PCin(0)==0)
	{
		sensor|=(1<<0);
	}
	if(PCin(1)==0)
	{
		sensor|=(1<<1);
	}
	if(PCin(2)==0)
	{
		sensor|=(1<<2);
	}
	if(PCin(3)==0)
	{
		sensor|=(1<<3);
	}
	if(PCin(4)==0)
	{
		sensor|=(1<<4);
	}
	if(PCin(5)==0)
	{
		sensor|=(1<<5);
	}
	return sensor;
}

/***************************************************/
/**********************��������*******************/
void Smoke_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

//��ʼ��ADC1
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��4														   
void  Adc1_Init(void)
{      
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC��λ
	
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//��λ����	    
	
	ADC_DeInit(ADC1);  //��λADC1,������ ADC1��ȫ���Ĵ�������Ϊȱʡֵ
	
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ: ����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���  
	
	
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}

//���ADC1ͨ��4��ֵ
//����ֵ:ת�����
u16 Get_Adc1(void)   
{
  //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
} 

void Smoke_Fun(void)
{
	//��д����
	u16 ad;
	u8 buf[10];
	u8 array[7]={0xFE,0x07,0xA0,0x07,0,0,0};
	Smoke_Init();
	Adc1_Init();
	ad=Get_Adc1();
	sprintf((char*)buf,"%d",ad);
	OLED_ShowString(0,32+14,(u8*)"     ",12,1);
	OLED_ShowString(0,32+14,buf,12,1);
	array[5]=ad%100;
	array[4]=ad/100;
	array[6]=(array[0]+array[1]+array[2]+array[3]+array[4]+array[5])%256;
	UART1_Send_Array(array,7);
}

/***************************************************/
/**********************��ʪ�ȴ�����*******************/
static uint8_t    sensor_data[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

void Temp_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void Sensor_Pin_In(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/**
 * @brief �����������͵ĵ����ֽ�
 *
 * @return uint8_t ���������͵ĵ����ֽ�
 */
static uint8_t read_sensor_data(void)
{
    uint16_t cnt;
    uint8_t  i;
    uint8_t  buffer, tmp;
    buffer = 0;
    for (i = 0; i < 8; i++) {
        cnt = 0;
        while (!GET_SDA()) //����ϴε͵�ƽ�Ƿ����
        {
            if (++cnt >= 3000) {
                break;
            }
        }
        //��ʱMin=26us Max50us ��������"0" �ĸߵ�ƽ
        delay_us(30); //��ʱ30us

        //�жϴ�������������λ
        tmp = 0;
        if (GET_SDA()) {
            tmp = 1;
        }
        cnt = 0;
        while (GET_SDA()) //�ȴ��ߵ�ƽ ����
        {
            if (++cnt >= 2000) {
                break;
            }
        }
        buffer <<= 1;
        buffer |= tmp;
    }
    return buffer;
}

/**
 * @brief ��������
 *
 * @return int32_t 0����ȡ�ɹ��� -1����ȡʧ��
 */
static int32_t read_sensor(void)
{
    uint16_t overtime_cnt;
    uint8_t  i;
	Sensor_Init();
    //��������(Min=800US Max=20Ms)
    SDA_L();
    delay_ms(2); //��ʱ2Ms

    //�ͷ����� ��ʱ(Min=30us Max=50us)
    SDA_H();
    delay_us(30); //��ʱ30us
    //������Ϊ���� �жϴ�������Ӧ�ź�
    SDA_H();
	Sensor_Pin_In();

    //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������
    if (GET_SDA() == 0) {
        overtime_cnt = 0;
        //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����
        while ((!GET_SDA())) {
            if (++overtime_cnt > 3000) //��ֹ������ѭ��
            {
                return -1;
            }
        }
        overtime_cnt = 0;
        //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
        while ((GET_SDA())) {
            if (++overtime_cnt > 3000) //��ֹ������ѭ��
            {
                return -1;
            }
        }
        // ���ݽ��� ������������40λ����
        // ��5���ֽ� ��λ����  5���ֽڷֱ�Ϊʪ�ȸ�λ ʪ�ȵ�λ �¶ȸ�λ �¶ȵ�λ
        // У��� У���Ϊ��ʪ�ȸ�λ+ʪ�ȵ�λ+�¶ȸ�λ+�¶ȵ�λ
        for (i = 0; i < 5; i++) {
            sensor_data[i] = read_sensor_data();
        }
    } else {
        return -1; // δ�յ���������Ӧ
    }
    return 0;
}

void Temp_Fun(void)
{
	//��Ӵ���
		int i,temp,humi;
	u8 buf[20];
	u8 array[7]={0xFE,0x07,0xA0,0,0,0,0};
	Temp_Init();
	i=read_sensor();
	if(i==0)
	{
		 sprintf((char*)buf,"humi:%.1f,temp:%.1f\r\n",(float)((sensor_data[0]<<8)|sensor_data[1])/10,
			(float)((sensor_data[2]<<8)|sensor_data[3])/10);//(����)���ȼ�����/
		 OLED_ShowString(0,32+14,(u8*)"     ",12,1);
		 OLED_ShowString(0,32+14,buf,12,1);
		 temp=((sensor_data[2]<<8)|sensor_data[3])/10;
		 humi=((sensor_data[0]<<8)|sensor_data[1])/10;
		 array[3]=0x05;
		 array[5]=temp;
		 array[4]=temp>>8;
		 array[6]=(array[0]+array[1]+array[2]+array[3]+array[4]+array[5])%256;
		 UART1_Send_Array(array,7);
		 delay_ms(100);
		 array[3]=0x06;
		 array[5]=humi;
		 array[4]=humi>>8;
		 array[6]=(array[0]+array[1]+array[2]+array[3]+array[4]+array[5])%256;
		 UART1_Send_Array(array,7);
	}
	else
	{
		sprintf((char*)buf,"error\r\n");
		OLED_ShowString(0,32+14,(u8*)"     ",12,1);
		 OLED_ShowString(0,32+14,buf,12,1);
	}
}

/***************************************************/
/**********************���崫����*******************/
void Body_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void Body_Fun(void)
{
	//��Ӵ���
		u8 buf[20];
	u8 array[7]={0xFE,0x07,0xA0,0x04,0,0,0};
	Body_Init();
	if(GET_SDA()==1)
	{
		sprintf((char*)buf,"no boby");
		OLED_ShowString(0,32+14,(u8*)"     ",12,1);
		OLED_ShowString(0,32+14,buf,12,1);
		array[5]=1;
		 array[4]=0;
		 array[6]=(array[0]+array[1]+array[2]+array[3]+array[4]+array[5])%256;
		UART1_Send_Array(array,7);
	}
	else
	{
		sprintf((char*)buf,"there are boby");
		OLED_ShowString(0,32+14,(u8*)"     ",12,1);
		OLED_ShowString(0,32+14,buf,12,1);
		array[5]=0;
		 array[4]=1;
		 array[6]=(array[0]+array[1]+array[2]+array[3]+array[4]+array[5])%256;
		UART1_Send_Array(array,7);
	}
}



/***************************************************/
/**********************���ȴ�����*******************/
void Fan_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}



