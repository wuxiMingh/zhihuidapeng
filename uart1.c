#include "uart1.h"
#include "main.h"
#include "stdio.h"
void usart1_Config()
{
	//ʹ��usart1ʱ�ӣ���APB2������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	//ʹ��gpioaʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	//���ùܽŸ���
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	//gpio���ã���ʼ��
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_AF;
	gpio_struct.GPIO_OType=GPIO_OType_PP;//����ģʽ
	gpio_struct.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
	gpio_struct.GPIO_PuPd=GPIO_PuPd_UP;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_struct);
	//4.�������ü���ʼ��
	USART_InitTypeDef u1;
	u1.USART_BaudRate=115200;//������
	u1.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	u1.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	u1.USART_Parity=USART_Parity_No;//��У��
	u1.USART_StopBits=USART_StopBits_1;
	u1.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&u1);
	//ʹ�ܴ���
	USART_Cmd(USART1,ENABLE);
}
//usart1����
void myprintf(const char *str)
{
	int i;
	for(i=0;*str;i++)
	{
		USART_SendData(USART1,*str);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		str++;
	}
}
void USART1_IRQHandler()
{
	u8 resdata=0;
	resdata =USART_ReceiveData(USART1);
	if(resdata=='1')
	{
		//Led_Open();
		USART_SendData(USART1,resdata);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
	if(resdata=='2')
	{
		//Led_Close();
		USART_SendData(USART1,resdata);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

int fputc(int ch,FILE *fp)
{
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}