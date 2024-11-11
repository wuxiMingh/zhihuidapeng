#include "led.h"
#include "delay.h"
#include "stdio.h"
extern int key_flag;
void Led_init()
{
	//ʹ��led��������
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	//����ṹ��
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	gpio_struct.GPIO_OType=GPIO_OType_PP;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&gpio_struct);
	//������λ��
	GPIO_SetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);
}
void led_open()
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_10);
}
void led_close()
{
	GPIO_SetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_10);
}
//led����
void led_shan(int led_falg)
{
	delay_init(168);
	while(led_falg)
	{
		delay_ms(50);
		led_open();
		delay_ms(50);
		led_close();
	}
}
//��������Ϊ��Ŀ��һ��
// File Name: pwm�����ƣ�TIM1_CH1��ͨ��1����ʱ��1����led2
// Author:dandy��
// Date:2021.11.2;

//������ �� led + ��ʱ��
//�߼���ʱ��1 ��PWM����

void pwm_led_init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;


	//1TIM1ʱ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); 
	//2E��ʱ������
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	
	//3��������ʱ��1
	TIM_DeInit(TIM1);
	
	//4E���9�����Ÿ���Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); 
	
	//5����E����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;   
	//���ù���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
	//���츴�����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
	//����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	//5��ʼ������
	GPIO_Init(GPIOE,&GPIO_InitStructure);  
	
	//6�߼���ʱ����ʼ��
	TIM_TimeBaseStructure.TIM_Prescaler=400;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=100;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
		
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	        
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	//7����Tָ���Ĳ�����ʼ������TIM1 4OC1
//	TIM_OCInitStructure.TIM_Pulse = 100;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	//8.ʹ�ܶ�ʱ��
	TIM_Cmd(TIM1, ENABLE);  	
	//9�������ģʽʹ��
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

int led_huxi(u16 *led0pwmval,u8 *dir)
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
	TIM_SetCompare1(TIM1,*led0pwmval);
    return 0;
}

//����
void fan_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_Pin=GPIO_Pin_15;
	gpio_struct.GPIO_OType=GPIO_OType_PP;
	gpio_struct.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	//�����ò�������
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}
void fan_open()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}
void fan_close()
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}