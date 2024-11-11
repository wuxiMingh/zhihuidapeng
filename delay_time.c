#include "main.h"
#include "delay_time.h"
#include "stm32f4xx_tim.h"
unsigned int Time_n=0;
//��ʱ����ʼ��
void Sitck_Init()
{
	//����ʱ��Դ
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	//���ж�һ��
	if(SysTick_Config(SystemCoreClock/1000))
	{
		//beef();
	}
}
void Sitck_Init_j()
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}
//
void delay_us_j(uint16_t nus)
{
	SysTick->CTRL &=~(1<<0);
	SysTick->LOAD=21*nus-1;
	SysTick->VAL=0;
	SysTick->CTRL|=(1<<0);
	while((SysTick->CTRL&0x00010000)==0);
	SysTick->CTRL=0;
	SysTick->VAL=0;
}
void delay_ms_j(uint16_t nus)
{
	SysTick->CTRL &=~(1<<0);
	SysTick->LOAD=21*1000*nus-1;
	SysTick->VAL=0;
	SysTick->CTRL|=(1<<0);
	while((SysTick->CTRL&0x00010000)==0);
	SysTick->CTRL=0;
	SysTick->VAL=0;
}


void delay_ms_systick(unsigned int nums)
{
	Time_n=nums;
	while(Time_n);
}
//void SysTick_Handler(void)
//{
//  if(Time_n)
//	  Time_n--;
//}
//�������TIM6��ʱ����ʼ��
void Time6_Config()
{
	//��ʼ���ṹ��
	TIM_TimeBaseInitTypeDef tim_struct;
	//ʱ��ʹ��time6�ⲿ������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	//��ʼ����ʱ�� �ṹ���еı���
	tim_struct.TIM_CounterMode=TIM_CounterMode_Up;//����ģʽ�����ϼ���
	//���ü�ʱ
	tim_struct.TIM_Period=5000;
	tim_struct.TIM_Prescaler=8400 - 1;
	//
	TIM_TimeBaseInit(TIM6,&tim_struct);
	
	//����ʱ��Դ��
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIM6,ENABLE);
}
//��ʱ���жϿ���---��������
void NVIC_Config_time6()
{
	NVIC_InitTypeDef nvic_struct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//���÷���
	//��ʱ��TIM6,�жϳ�ʼ��
	nvic_struct.NVIC_IRQChannel=TIM6_DAC_IRQn;//��ʱ��6�ж�ͨ��
	nvic_struct.NVIC_IRQChannelCmd=ENABLE;//ʹ��
	nvic_struct.NVIC_IRQChannelPreemptionPriority=0;
	nvic_struct.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&nvic_struct);
}
//void TIM6_DAC_IRQHandler(void)//��ʼ�������й涨������
//{
//	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET)
//	{
//		GPIO_ToggleBits(GPIOE,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);
//		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
//		
//	}
//}



//ͨ�ö�ʱ��---time2ͨ��3,pwm�������,beef
void Pwm_tim2_Ch3_Beep(unsigned int n)
{
	//1.��ʱ��2ʱ�ӳ�ʼ��ʹ�ܣ�ͨ�ö�ʱ��Ϊ����������ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//2.GPIOʱ�����ã�B�飺
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//3.GPIOB:��ʮ����Ա ���óɶ�ʱ��2��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);
	//4.����ģʽ��
	GPIO_InitTypeDef pwm_struct;
	pwm_struct.GPIO_Mode=GPIO_Mode_AF;//����Ϊ����ģʽ
	pwm_struct.GPIO_OType=GPIO_OType_PP;
	pwm_struct.GPIO_Pin=GPIO_Pin_10;
	pwm_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&pwm_struct);
	//5.��ʼ����ʱ��������ͨ�ö�ʱ���ĳ�ʼ��
	TIM_TimeBaseInitTypeDef tim2_struct;
	tim2_struct.TIM_CounterMode=TIM_CounterMode_Up;//������ʱ����������
	//Ԥ��
	tim2_struct.TIM_Prescaler=8400-1;
	tim2_struct.TIM_Period=100-1;
	TIM_TimeBaseInit(TIM2,&tim2_struct);
	//6.��ʼ��ͨ��3��
	TIM_OCInitTypeDef Tim_Ocinit_struct;
	Tim_Ocinit_struct.TIM_OCMode=TIM_OCMode_PWM1;//���ñȽ�ģʽ
	Tim_Ocinit_struct.TIM_OutputState=TIM_OutputState_Enable;//ʹ������ȽϼĴ���
	Tim_Ocinit_struct.TIM_OCPolarity=TIM_OCPolarity_High;//����Ϊ�ߵ�ƽ
	Tim_Ocinit_struct.TIM_Pulse= n;
	TIM_OC3Init(TIM2,&Tim_Ocinit_struct);
	//7.�Ƚϲ���Ĵ���ʹ��
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
	//8.�Զ�װ�ؼĴ���ʹ��
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}
void Pwm_tim1_Ch1_Led()
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
	TIM_TimeBaseStructure.TIM_Prescaler=8400;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=100;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
		
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	        
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	//7����Tָ���Ĳ�����ʼ������TIM1 4OC1
//	TIM_OCInitStructure.TIM_Pulse = 100;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	//8.ʹ�ܶ�ʱ��
	TIM_Cmd(TIM1, ENABLE);  	
	//9�������ģʽʹ��
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	//8.�Զ�װ�ؼĴ���ʹ��
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
