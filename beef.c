#include "beef.h"
#include "delay.h"
int music_num[30]={50,100,150,100,50,40,30,20,10,20,10,10};
int music_num_end[30]={100,50,80,60,50,40,30,20,10};
void beef_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//定义结构体
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

//TIM2 PWM部分初始化  
//PWM输出初始化 
//arr：自动重装值 
//psc：时钟预分频数 
// Header:PWM2 CH3 驱动beep 
// File Name: 蚊子声音； 
// Author:dandy  
// Date:2021.11.5 
//定时器2也是通用定时器；

void beef_pwm_init()
{		 					 
	//此部分需手动修改IO口设置	
	GPIO_InitTypeDef GPIO_InitStructure;
	//定时器结构体初始化。
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	//1定时器2时钟初始化使能；
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	
	
	////2GPIO时钟配置；B组
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	
	
	//3GPIOB 第10个成员 复用成定时器2；
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);
	
	//GPIOB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;   
	//复用功能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	//速度100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
	//推挽复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
	//上拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	//4初始化PB10
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	  					
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;  //定时器分频  确定工作频率即是 一个数值的所占时间。单位机器周期。
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=1000;  	 //自动重装载值确定整个定时时间。定时时间，大周期。1000 * 2us = 2000us = 2ms//
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //时钟分割。
	//5初始化定时器2；
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); 
		
	//选择定时器模式:TIM脉冲宽度调制模式1  模式 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  
	//比较输出使能 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
	//输出极性:TIM输出比较极性高    极性：从0到设定值最高值之间，是输出低电平还是高电平。输出高电平。 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  
	//6通道3初始化； 
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);   
	
	//7定时器2的通道3的比较捕获寄存器使能。 
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);   
	//8自动装载值寄存器使能  
	TIM_ARRPreloadConfig(TIM2,ENABLE); 
	//9使能TIM2 
	TIM_Cmd(TIM2, ENABLE);
	TIM_SetCompare3(TIM2,0); 
}  

void  beef_huxi(u16 *led0pwmval,u8 *dir)
{
	//呼吸灯，
	//dir==1 led0pwmval递增

	if(*dir)
		(*led0pwmval)+=8;
	else     //dir==0 led0pwmval递减 
		(*led0pwmval)-=8;
	//led0pwmval到达300后，方向为递减
	if((*led0pwmval)>100)
		*dir=0;
	//led0pwmval递减到0后，方向改为递增
	if((*led0pwmval)==0)
		*dir=1;	
	//修改比较值，修改占空比	
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
