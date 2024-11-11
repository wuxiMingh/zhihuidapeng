#ifndef _IIC_H
#define _IIC_H
#include <stdio.h>
#include "main.h"
#include "delay.h"
//iic�˿�
#define IIC_PORT GPIOB

//SDA����
#define IIC_SDA_PIN GPIO_Pin_7
//SCL����
#define IIC_SCL_PIN GPIO_Pin_6

//sclʱ����д����
#define SCL_Write(N) (N)?GPIO_SetBits(IIC_PORT,IIC_SCL_PIN):GPIO_ResetBits(IIC_PORT,IIC_SCL_PIN)

//SDA ������д����
#define SDA_Write(N) (N)?GPIO_SetBits(IIC_PORT,IIC_SDA_PIN):GPIO_ResetBits(IIC_PORT,IIC_SDA_PIN)
//
void IIC_Init();
void SET_SDA_OUT();
void SET_SDA_IN();
void Start_signal();
void Stop_siganl();
u8 IIC_get_Ask();
void IIC_send_Ask(u8 ask);
void IIC_send_Byte(u8 data);
u8 IIC_read_Byte(void);
void IIC_AT24C02_write_data(u8 addr,u8 data);
u8 AT24C02_read_data(u8 addr);

#endif