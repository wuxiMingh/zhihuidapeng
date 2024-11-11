#include "nvic.h"

//�жϳ�ʼ��
void nvic_Config()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//nvic �ṹ��
	NVIC_InitTypeDef nvic_struct;
	nvic_struct.NVIC_IRQChannel=USART2_IRQn;//��ʱ��6�ж�ͨ��
	nvic_struct.NVIC_IRQChannelCmd=ENABLE;//ʹ�ܶ�ʱ��ͨ��
	nvic_struct.NVIC_IRQChannelPreemptionPriority=0;//��ռʽ���ȼ�
	nvic_struct.NVIC_IRQChannelSubPriority=5;//��Ӧʽ���ȼ�
	NVIC_Init(&nvic_struct);
	
	 //�ⲿ�ж�4  EXIT4  ����4  ע�� ���û�ж԰���4��GPIO���õĻ���һ��Ҫ�ǵ����� 
    nvic_struct.NVIC_IRQChannel=EXTI4_IRQn;//�����ж�ͨ�� 
    nvic_struct.NVIC_IRQChannelCmd=ENABLE;//ʹ�� 
    nvic_struct.NVIC_IRQChannelPreemptionPriority=0;//��ռʽ���ȼ� 
    nvic_struct.NVIC_IRQChannelSubPriority=0;//��Ӧʽ���ȼ� 
	NVIC_Init(&nvic_struct); 
	
	
	nvic_struct.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_Init(&nvic_struct); 
	
	nvic_struct.NVIC_IRQChannel=EXTI3_IRQn;
	NVIC_Init(&nvic_struct); 
	
	nvic_struct.NVIC_IRQChannel=TIM7_IRQn;
	nvic_struct.NVIC_IRQChannelPreemptionPriority=1;//��ռʽ���ȼ�
	nvic_struct.NVIC_IRQChannelSubPriority=5;//��Ӧʽ���ȼ�
	NVIC_Init(&nvic_struct);
}

void Tim7_Config(void)
{
	
	//��ʼ���ṹ��
	TIM_TimeBaseInitTypeDef tim_struct;
	//ʱ��ʹ��time6�ⲿ������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	//��ʼ����ʱ�� �ṹ���еı���
	tim_struct.TIM_CounterMode=TIM_CounterMode_Up;//����ģʽ�����ϼ���
	//���ü�ʱ
	tim_struct.TIM_Period=500000-1;
	tim_struct.TIM_Prescaler=8400 - 1;
	//
	TIM_TimeBaseInit(TIM7,&tim_struct);
	
	//����ʱ��Դ��
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	//ʹ�ܶ�ʱ��
	
	
}