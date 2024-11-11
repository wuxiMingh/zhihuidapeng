#include "beef.h"
#include "delay.h"
int music_num[30]={50,100,150,100,50,40,30,20,10,20,10,10};
int music_num_end[30]={100,50,80,60,50,40,30,20,10};
void beef_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//����ṹ��
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_Pin=GPIO_Pin_10;
	gpio_struct.GPIO_OType=GPIO_OType_PP;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}
void beef_open()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
}
void beef_close()
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}
void beef_shan()
{
	int flag=3;
	while(flag)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		delay_ms(100);
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
		delay_ms(100);
		flag--;                                                 
	}
}

//TIM2 PWM���ֳ�ʼ��  
//PWM�����ʼ�� 
//arr���Զ���װֵ 
//psc��ʱ��Ԥ��Ƶ�� 
// Header:PWM2 CH3 ����beep 
// File Name: ���������� 
// Author:dandy  
// Date:2021.11.5 
//��ʱ��2Ҳ��ͨ�ö�ʱ����

void beef_pwm_init()
{		 					 
	//�˲������ֶ��޸�IO������	
	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʱ���ṹ���ʼ����
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	//1��ʱ��2ʱ�ӳ�ʼ��ʹ�ܣ�
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	
	
	////2GPIOʱ�����ã�B��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	
	
	//3GPIOB ��10����Ա ���óɶ�ʱ��2��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);
	
	//GPIOB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;   
	//���ù���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
	//���츴�����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
	//����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	//4��ʼ��PB10
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	  					
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;  //��ʱ����Ƶ  ȷ������Ƶ�ʼ��� һ����ֵ����ռʱ�䡣��λ�������ڡ�
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=1000;  	 //�Զ���װ��ֵȷ��������ʱʱ�䡣��ʱʱ�䣬�����ڡ�1000 * 2us = 2000us = 2ms//
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //ʱ�ӷָ
	//5��ʼ����ʱ��2��
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); 
		
	//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1  ģʽ 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  
	//�Ƚ����ʹ�� 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
	//�������:TIM����Ƚϼ��Ը�    ���ԣ���0���趨ֵ���ֵ֮�䣬������͵�ƽ���Ǹߵ�ƽ������ߵ�ƽ�� 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  
	//6ͨ��3��ʼ���� 
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);   
	
	//7��ʱ��2��ͨ��3�ıȽϲ���Ĵ���ʹ�ܡ� 
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);   
	//8�Զ�װ��ֵ�Ĵ���ʹ��  
	TIM_ARRPreloadConfig(TIM2,ENABLE); 
	//9ʹ��TIM2 
	TIM_Cmd(TIM2, ENABLE);
	TIM_SetCompare3(TIM2,0); 
}  

void  beef_huxi(u16 *led0pwmval,u8 *dir)
{
	//�����ƣ�
	//dir==1 led0pwmval����

	if(*dir)
		(*led0pwmval)+=8;
	else     //dir==0 led0pwmval�ݼ� 
		(*led0pwmval)-=8;
	//led0pwmval����300�󣬷���Ϊ�ݼ�
	if((*led0pwmval)>100)
		*dir=0;
	//led0pwmval�ݼ���0�󣬷����Ϊ����
	if((*led0pwmval)==0)
		*dir=1;	
	//�޸ıȽ�ֵ���޸�ռ�ձ�	
	TIM_SetCompare3(TIM2,*led0pwmval); 
}
void beef_start_music()
{
	for(int i=0;i<20;i++)
	{
		TIM_SetCompare3(TIM2,music_num[i]); 
		delay_ms(50);
	}
}
void beef_end_music()
{
		for(int i=0;i<20;i++)
	{
		TIM_SetCompare3(TIM2,music_num_end[i]); 
		delay_ms(50);
	}
}
