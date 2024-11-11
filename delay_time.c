#include "main.h"
#include "delay_time.h"
#include "stm32f4xx_tim.h"
unsigned int Time_n=0;
//定时器初始化
void Sitck_Init()
{
	//设置时钟源
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	//秒中断一次
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
//外设基本TIM6定时器初始化
void Time6_Config()
{
	//初始化结构体
	TIM_TimeBaseInitTypeDef tim_struct;
	//时钟使能time6外部基本定时器
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	//初始化定时器 结构体中的变量
	tim_struct.TIM_CounterMode=TIM_CounterMode_Up;//计数模式：向上计数
	//设置计时
	tim_struct.TIM_Period=5000;
	tim_struct.TIM_Prescaler=8400 - 1;
	//
	TIM_TimeBaseInit(TIM6,&tim_struct);
	
	//设置时钟源，
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	//使能定时器
	TIM_Cmd(TIM6,ENABLE);
}
//定时器中断控制---向量配置
void NVIC_Config_time6()
{
	NVIC_InitTypeDef nvic_struct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//设置分组
	//定时器TIM6,中断初始化
	nvic_struct.NVIC_IRQChannel=TIM6_DAC_IRQn;//定时器6中断通道
	nvic_struct.NVIC_IRQChannelCmd=ENABLE;//使能
	nvic_struct.NVIC_IRQChannelPreemptionPriority=0;
	nvic_struct.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&nvic_struct);
}
//void TIM6_DAC_IRQHandler(void)//起始向量表中规定函数名
//{
//	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET)
//	{
//		GPIO_ToggleBits(GPIOE,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);
//		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
//		
//	}
//}



//通用定时器---time2通道3,pwm脉冲带框,beef
void Pwm_tim2_Ch3_Beep(unsigned int n)
{
	//1.定时器2时钟初始化使能：通用定时器为外设需设置时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//2.GPIO时钟配置：B组：
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//3.GPIOB:第十个成员 复用成定时器2；
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);
	//4.复用模式：
	GPIO_InitTypeDef pwm_struct;
	pwm_struct.GPIO_Mode=GPIO_Mode_AF;//设置为复用模式
	pwm_struct.GPIO_OType=GPIO_OType_PP;
	pwm_struct.GPIO_Pin=GPIO_Pin_10;
	pwm_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&pwm_struct);
	//5.初始化定时器：就是通用定时器的初始化
	TIM_TimeBaseInitTypeDef tim2_struct;
	tim2_struct.TIM_CounterMode=TIM_CounterMode_Up;//基本定时器上拉计数
	//预分
	tim2_struct.TIM_Prescaler=8400-1;
	tim2_struct.TIM_Period=100-1;
	TIM_TimeBaseInit(TIM2,&tim2_struct);
	//6.初始化通道3：
	TIM_OCInitTypeDef Tim_Ocinit_struct;
	Tim_Ocinit_struct.TIM_OCMode=TIM_OCMode_PWM1;//设置比较模式
	Tim_Ocinit_struct.TIM_OutputState=TIM_OutputState_Enable;//使能输出比较寄存器
	Tim_Ocinit_struct.TIM_OCPolarity=TIM_OCPolarity_High;//极性为高电平
	Tim_Ocinit_struct.TIM_Pulse= n;
	TIM_OC3Init(TIM2,&Tim_Ocinit_struct);
	//7.比较捕获寄存器使能
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
	//8.自动装载寄存器使能
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}
void Pwm_tim1_Ch1_Led()
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
	TIM_TimeBaseStructure.TIM_Prescaler=8400;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=100;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
		
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	        
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	//7根据T指定的参数初始化外设TIM1 4OC1
//	TIM_OCInitStructure.TIM_Pulse = 100;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	//8.使能定时器
	TIM_Cmd(TIM1, ENABLE);  	
	//9控制输出模式使能
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	//8.自动装载寄存器使能
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
