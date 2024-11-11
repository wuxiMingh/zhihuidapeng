#include "iic.h"
#include "main.h"
#include "delay.h"
#include "oled.h"
//初始化iic总线的配置，并将两个引脚设置为空闲状态
void IIC_Init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_OType=GPIO_OType_OD;//开漏输出
	gpio_struct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	//将两根线置为空闲状态
	
}
//设置sda为输出模式
void SET_SDA_OUT()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_OType=GPIO_OType_OD;//开漏输出
	gpio_struct.GPIO_Pin=IIC_SDA_PIN;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
}
//设置sda模式为输入
void SET_SDA_IN()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_IN;
	gpio_struct.GPIO_PuPd=GPIO_PuPd_UP;
	
	gpio_struct.GPIO_Pin=IIC_SDA_PIN;
	
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
}
//起始信号
void Start_signal()
{

	SET_SDA_OUT();
	//全部先拉高
	SCL_Write(1);
	SDA_Write(1);
	delay_us(5);
	//拉低saa
	SDA_Write(0);
	delay_us(5);
	//拉低scl
	SCL_Write(0);
}
//停止信号
void Stop_siganl()
{

	SET_SDA_OUT();
	SCL_Write(0);
	SDA_Write(0);
	delay_us(5);
	//scl拉高
	SCL_Write(1);
	delay_us(5);
	//sda拉高
	SDA_Write(1);
}

//等待从机应答 0：应答  1：不应答
u8 IIC_get_Ask()
{

	//scl拉低
	SCL_Write(0);
	//输入模式
	SET_SDA_IN();
	//拉高
	SCL_Write(1);
	delay_us(5);
	
	if(GPIO_ReadInputDataBit(IIC_PORT,IIC_SDA_PIN)==RESET)
	{
		SCL_Write(0);
		delay_us(5);
		return 0;
	}
	return 1;
}
//主机应答
void IIC_send_Ask(u8 ask)
{

	//scl 拉低----》数据无效
	SCL_Write(0);
	//输出模式
	SET_SDA_OUT();
	//拉低scl
	SCL_Write(0);
	delay_us(5);
	//sda准备数据
	if(ask)
	{
		SDA_Write(1);//不应答
	}
	else{
		SDA_Write(0);//应答
	}
	SCL_Write(0);
	delay_us(5);
}
//发送数据  1个字节 8bit 高位在前
void IIC_send_Byte(u8 data)
{
	int i;
	//scl拉低--》数据无效
	SCL_Write(0);
	//输出模式
	SET_SDA_OUT();
	//scl拉低
	SCL_Write(0);
	delay_us(2);
	for(i=7;i>=0;i--)
	{
		if(data &(0x01<<i))
		{
			SDA_Write(1);
		}
		else 
		{
			SDA_Write(0);
		}
		delay_us(2);
		//拉高scl -->发送数据
		SCL_Write(1);
		delay_us(2);
		//拉低scl
		SCL_Write(0);
		delay_us(2);
	}
}

//iic 读字节数据
u8 IIC_read_Byte(void)
{

	u8 data=0;
	int i; 
	//拉低scl --数据无效
	SCL_Write(0);
	//输入模式
	SET_SDA_IN();
	//scl 拉低
	SCL_Write(0);
	delay_us(2);
	//读取数据
	for(i=7;i>=0;i--)
	{
		SCL_Write(1);
		delay_us(2);
		if(GPIO_ReadInputDataBit(IIC_PORT,IIC_SDA_PIN)==SET)
		{
			data|=1<<i;
		}
		SCL_Write(0);
		delay_us(2);
	}
	SCL_Write(1);
	delay_us(2);
	return data;
}

//向AT24C02 指定地址写入1个字节数据
void IIC_AT24C02_write_data(u8 addr,u8 data)
{
	//1.起始信号
	Start_signal();
	//2.发送从器件地址：写
	IIC_send_Byte(0xA0);
	//3.等待应答
	if(IIC_get_Ask())
	{
		printf("dddddddd no ask\r\n");
		Stop_siganl();
		return ;
	}
	//4.写地址
	IIC_send_Byte(addr);
	//5.等待应答
	if(IIC_get_Ask())
	{
		printf("word addr no ask\r\n");
		Stop_siganl();
		return;
	}
	//6.写数据
	IIC_send_Byte(data);
	//7.等待应答
	if(IIC_get_Ask())
	{
		printf("data no ask\r\n");
		Stop_siganl();
		return;
	}
	Stop_siganl();
}
//随机读
u8 AT24C02_read_data(u8 addr)
{
	Start_signal();
	//2.发送从器件地址：写
	IIC_send_Byte(0xA0);
	//等待应答
	if(IIC_get_Ask())
	{
		printf("read device addr no ask\r\n");
		Stop_siganl();
	}
	//2.发送从器件地址：写
	IIC_send_Byte(addr);
	//等待应答
	if(IIC_get_Ask())
	{
		printf("read word addr no ask\r\n");
		Stop_siganl();
	}
	//6.起始信号
	Start_signal();
	
	//2.发送从器件地址：写
	IIC_send_Byte(0xA1);
	//等待应答
	if(IIC_get_Ask())
	{
		printf("read device2 addr no ask\r\n");
		Stop_siganl();
	}
	//9.读取数据
	u8 data=IIC_read_Byte();
	//10.主机发送 不应答
	IIC_send_Ask(1);
	//11.停止信号
	Stop_siganl();
	return data;
}