#ifndef _SENSOR_H
#define _SENSOR_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "stdio.h"

#define Sensor_Fan		0x01
#define Sensor_Fire		0x06//0b00 0110
#define Sensor_Body		0x10//0b01 0000
#define Sensor_Temp		0x04//0b00 0100
#define Sensor_Smoke	0x02//0b00 0010

#define GET_SDA() GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define SDA_H()   GPIO_SetBits(GPIOA,GPIO_Pin_0)
#define SDA_L()   GPIO_ResetBits(GPIOA,GPIO_Pin_0)


void Sensor_Init(void);
u8 RecognizeSensor(void);
void Smoke_Init(void);
void Adc1_Init(void);
u16 Get_Adc1(void);
void Smoke_Fun(void);
void Temp_Init(void);
void Sensor_Pin_In(void);
static uint8_t read_sensor_data(void);
static int32_t read_sensor(void);
void Temp_Fun(void);
void Body_Fun(void);
void Beep_Fun(void);
void Fire_Fun(void);
void Fan_Init(void);

#endif
