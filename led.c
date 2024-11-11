#include "led.h"
#include "delay.h"
#include "stdio.h"
extern int key_flag;
void Led_init()
{
	//使能led外设总线
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	//定义结构体
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	gpio_struct.GPIO_OType=GPIO_OType_PP;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&gpio_struct);
	//先设置位灭
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
//led闪闪
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
//呼吸灯作为项目中一项
// File Name: pwm呼吸灯：TIM1_CH1；通道1；定时器1驱动led2
// Author:dandy；
// Date:2021.11.2;

//呼吸灯 ： led + 定时器
//高级定时器1 的PWM配置

void pwm_led_init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;


	//1TIM1时钟配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); 
	//2E组时钟配置
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	
	//3重启动定时器1
	TIM_DeInit(TIM1);
	
	//4E组第9个引脚复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); 
	
	//5配置E组引脚配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;   
	//复用功能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	//速度100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
	//推挽复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
	//上拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	//5初始化引脚
	GPIO_Init(GPIOE,&GPIO_InitStructure);  
	
	//6高级定时器初始化
	TIM_TimeBaseStructure.TIM_Prescaler=400;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=100;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
		
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	        
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	//7根据T指定的参数初始化外设TIM1 4OC1
//	TIM_OCInitStructure.TIM_Pulse = 100;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	//8.使能定时器
	TIM_Cmd(TIM1, ENABLE);  	
	//9控制输出模式使能
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

int led_huxi(u16 *led0pwmval,u8 *dir)
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
	TIM_SetCompare1(TIM1,*led0pwmval);
    return 0;
}

//风扇
void fan_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_Pin=GPIO_Pin_15;
	gpio_struct.GPIO_OType=GPIO_OType_PP;
	gpio_struct.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	//先设置不开风扇
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