#include "iic.h"
#include "main.h"
#include "delay.h"
#include "oled.h"
//��ʼ��iic���ߵ����ã�����������������Ϊ����״̬
void IIC_Init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_OType=GPIO_OType_OD;//��©���
	gpio_struct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	//����������Ϊ����״̬
	
}
//����sdaΪ���ģʽ
void SET_SDA_OUT()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_OType=GPIO_OType_OD;//��©���
	gpio_struct.GPIO_Pin=IIC_SDA_PIN;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
}
//����sdaģʽΪ����
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
//��ʼ�ź�
void Start_signal()
{

	SET_SDA_OUT();
	//ȫ��������
	SCL_Write(1);
	SDA_Write(1);
	delay_us(5);
	//����saa
	SDA_Write(0);
	delay_us(5);
	//����scl
	SCL_Write(0);
}
//ֹͣ�ź�
void Stop_siganl()
{

	SET_SDA_OUT();
	SCL_Write(0);
	SDA_Write(0);
	delay_us(5);
	//scl����
	SCL_Write(1);
	delay_us(5);
	//sda����
	SDA_Write(1);
}

//�ȴ��ӻ�Ӧ�� 0��Ӧ��  1����Ӧ��
u8 IIC_get_Ask()
{

	//scl����
	SCL_Write(0);
	//����ģʽ
	SET_SDA_IN();
	//����
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
//����Ӧ��
void IIC_send_Ask(u8 ask)
{

	//scl ����----��������Ч
	SCL_Write(0);
	//���ģʽ
	SET_SDA_OUT();
	//����scl
	SCL_Write(0);
	delay_us(5);
	//sda׼������
	if(ask)
	{
		SDA_Write(1);//��Ӧ��
	}
	else{
		SDA_Write(0);//Ӧ��
	}
	SCL_Write(0);
	delay_us(5);
}
//��������  1���ֽ� 8bit ��λ��ǰ
void IIC_send_Byte(u8 data)
{
	int i;
	//scl����--��������Ч
	SCL_Write(0);
	//���ģʽ
	SET_SDA_OUT();
	//scl����
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
		//����scl -->��������
		SCL_Write(1);
		delay_us(2);
		//����scl
		SCL_Write(0);
		delay_us(2);
	}
}

//iic ���ֽ�����
u8 IIC_read_Byte(void)
{

	u8 data=0;
	int i; 
	//����scl --������Ч
	SCL_Write(0);
	//����ģʽ
	SET_SDA_IN();
	//scl ����
	SCL_Write(0);
	delay_us(2);
	//��ȡ����
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

//��AT24C02 ָ����ַд��1���ֽ�����
void IIC_AT24C02_write_data(u8 addr,u8 data)
{
	//1.��ʼ�ź�
	Start_signal();
	//2.���ʹ�������ַ��д
	IIC_send_Byte(0xA0);
	//3.�ȴ�Ӧ��
	if(IIC_get_Ask())
	{
		printf("dddddddd no ask\r\n");
		Stop_siganl();
		return ;
	}
	//4.д��ַ
	IIC_send_Byte(addr);
	//5.�ȴ�Ӧ��
	if(IIC_get_Ask())
	{
		printf("word addr no ask\r\n");
		Stop_siganl();
		return;
	}
	//6.д����
	IIC_send_Byte(data);
	//7.�ȴ�Ӧ��
	if(IIC_get_Ask())
	{
		printf("data no ask\r\n");
		Stop_siganl();
		return;
	}
	Stop_siganl();
}
//�����
u8 AT24C02_read_data(u8 addr)
{
	Start_signal();
	//2.���ʹ�������ַ��д
	IIC_send_Byte(0xA0);
	//�ȴ�Ӧ��
	if(IIC_get_Ask())
	{
		printf("read device addr no ask\r\n");
		Stop_siganl();
	}
	//2.���ʹ�������ַ��д
	IIC_send_Byte(addr);
	//�ȴ�Ӧ��
	if(IIC_get_Ask())
	{
		printf("read word addr no ask\r\n");
		Stop_siganl();
	}
	//6.��ʼ�ź�
	Start_signal();
	
	//2.���ʹ�������ַ��д
	IIC_send_Byte(0xA1);
	//�ȴ�Ӧ��
	if(IIC_get_Ask())
	{
		printf("read device2 addr no ask\r\n");
		Stop_siganl();
	}
	//9.��ȡ����
	u8 data=IIC_read_Byte();
	//10.�������� ��Ӧ��
	IIC_send_Ask(1);
	//11.ֹͣ�ź�
	Stop_siganl();
	return data;
}