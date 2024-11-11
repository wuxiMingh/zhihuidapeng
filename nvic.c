#include "nvic.h"

//中断初始化
void nvic_Config()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//nvic 结构体
	NVIC_InitTypeDef nvic_struct;
	nvic_struct.NVIC_IRQChannel=USART2_IRQn;//定时器6中断通道
	nvic_struct.NVIC_IRQChannelCmd=ENABLE;//使能定时器通道
	nvic_struct.NVIC_IRQChannelPreemptionPriority=0;//抢占式优先级
	nvic_struct.NVIC_IRQChannelSubPriority=5;//响应式优先级
	NVIC_Init(&nvic_struct);
	
	 //外部中断4  EXIT4  按键4  注意 如果没有对按键4的GPIO配置的话，一定要记得配置 
    nvic_struct.NVIC_IRQChannel=EXTI4_IRQn;//设置中断通道 
    nvic_struct.NVIC_IRQChannelCmd=ENABLE;//使能 
    nvic_struct.NVIC_IRQChannelPreemptionPriority=0;//抢占式优先级 
    nvic_struct.NVIC_IRQChannelSubPriority=0;//响应式优先级 
	NVIC_Init(&nvic_struct); 
	
	
	nvic_struct.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_Init(&nvic_struct); 
	
	nvic_struct.NVIC_IRQChannel=EXTI3_IRQn;
	NVIC_Init(&nvic_struct); 
	
	nvic_struct.NVIC_IRQChannel=TIM7_IRQn;
	nvic_struct.NVIC_IRQChannelPreemptionPriority=1;//抢占式优先级
	nvic_struct.NVIC_IRQChannelSubPriority=5;//响应式优先级
	NVIC_Init(&nvic_struct);
}

void Tim7_Config(void)
{
	
	//初始化结构体
	TIM_TimeBaseInitTypeDef tim_struct;
	//时钟使能time6外部基本定时器
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	//初始化定时器 结构体中的变量
	tim_struct.TIM_CounterMode=TIM_CounterMode_Up;//计数模式：向上计数
	//设置计时
	tim_struct.TIM_Period=500000-1;
	tim_struct.TIM_Prescaler=8400 - 1;
	//
	TIM_TimeBaseInit(TIM7,&tim_struct);
	
	//设置时钟源，
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	//使能定时器
	
	
}