#include "sensor.h"
#include "delay.h"
#include "oled.h"
#include "usart.h"
/*******************识别传感器功能部分**************/
//传感器初始化
void Sensor_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}

//识别传感器函数
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
/**********************烟雾传感器*******************/
void Smoke_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

//初始化ADC1
//这里我们仅以规则通道为例
//我们默认仅开启通道4														   
void  Adc1_Init(void)
{      
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE );	  //使能ADC1通道时钟
	
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC复位
	
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//复位结束	    
	
	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1的全部寄存器重设为缺省值
	
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式: 独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器  
	
	
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}

//获得ADC1通道4的值
//返回值:转换结果
u16 Get_Adc1(void)   
{
  //设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
} 

void Smoke_Fun(void)
{
	//编写代码
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
/**********************温湿度传感器*******************/
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
 * @brief 读传感器发送的单个字节
 *
 * @return uint8_t 传感器发送的单个字节
 */
static uint8_t read_sensor_data(void)
{
    uint16_t cnt;
    uint8_t  i;
    uint8_t  buffer, tmp;
    buffer = 0;
    for (i = 0; i < 8; i++) {
        cnt = 0;
        while (!GET_SDA()) //检测上次低电平是否结束
        {
            if (++cnt >= 3000) {
                break;
            }
        }
        //延时Min=26us Max50us 跳过数据"0" 的高电平
        delay_us(30); //延时30us

        //判断传感器发送数据位
        tmp = 0;
        if (GET_SDA()) {
            tmp = 1;
        }
        cnt = 0;
        while (GET_SDA()) //等待高电平 结束
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
 * @brief 读传感器
 *
 * @return int32_t 0：读取成功； -1：读取失败
 */
static int32_t read_sensor(void)
{
    uint16_t overtime_cnt;
    uint8_t  i;
	Sensor_Init();
    //主机拉低(Min=800US Max=20Ms)
    SDA_L();
    delay_ms(2); //延时2Ms

    //释放总线 延时(Min=30us Max=50us)
    SDA_H();
    delay_us(30); //延时30us
    //主机设为输入 判断传感器响应信号
    SDA_H();
	Sensor_Pin_In();

    //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行
    if (GET_SDA() == 0) {
        overtime_cnt = 0;
        //判断从机是否发出 80us 的低电平响应信号是否结束
        while ((!GET_SDA())) {
            if (++overtime_cnt > 3000) //防止进入死循环
            {
                return -1;
            }
        }
        overtime_cnt = 0;
        //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
        while ((GET_SDA())) {
            if (++overtime_cnt > 3000) //防止进入死循环
            {
                return -1;
            }
        }
        // 数据接收 传感器共发送40位数据
        // 即5个字节 高位先送  5个字节分别为湿度高位 湿度低位 温度高位 温度低位
        // 校验和 校验和为：湿度高位+湿度低位+温度高位+温度低位
        for (i = 0; i < 5; i++) {
            sensor_data[i] = read_sensor_data();
        }
    } else {
        return -1; // 未收到传感器响应
    }
    return 0;
}

void Temp_Fun(void)
{
	//添加代码
		int i,temp,humi;
	u8 buf[20];
	u8 array[7]={0xFE,0x07,0xA0,0,0,0,0};
	Temp_Init();
	i=read_sensor();
	if(i==0)
	{
		 sprintf((char*)buf,"humi:%.1f,temp:%.1f\r\n",(float)((sensor_data[0]<<8)|sensor_data[1])/10,
			(float)((sensor_data[2]<<8)|sensor_data[3])/10);//(类型)优先级大于/
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
/**********************人体传感器*******************/
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
	//添加代码
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
/**********************风扇传感器*******************/
void Fan_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}



