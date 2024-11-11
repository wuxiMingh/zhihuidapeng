#include "key.h"
#include "stm32f4xx_it.h"
#include "delay.h"
#include "oled.h"
#include "led.h"
#include "wifi.h"
#include "beef.h"
extern int key_flag;//引用外部变量，来控制开关
//外部中断4的配置 
void EXTI4_Config(void) 
{ 
	EXTI_InitTypeDef exti_struct; 
//1 外部中断时钟总线-系统时钟。  42MHZ 
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE); 
//2.中断线映射函数 PE4配置成 外部中断4；
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource3); 	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4); 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource5); 
//3.EXTI4中断初始化 
	exti_struct.EXTI_Line=EXTI_Line3| EXTI_Line4|EXTI_Line5;//4引脚   
	exti_struct.EXTI_LineCmd=ENABLE;//开启中断 
	exti_struct.EXTI_Mode=EXTI_Mode_Interrupt;//中断 
	exti_struct.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿 
    EXTI_Init(&exti_struct); 
} 
//按键1 2 3配置  PE4 PE5 PE6 
void Key_Config(void) 
{ 
    //使能时钟 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); 
//配置GPIO引脚 
    GPIO_InitTypeDef gpio_struct; 
    gpio_struct.GPIO_Mode=GPIO_Mode_IN; 
    gpio_struct.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6; 
    gpio_struct.GPIO_PuPd=GPIO_PuPd_UP;//上拉 
    gpio_struct.GPIO_Speed=GPIO_Speed_2MHz; 
    GPIO_Init(GPIOE,&gpio_struct); 
} 
//------------------中断服务函数的编写 
//外部中断4 key1中断
void EXTI4_IRQHandler(void) 
{ 
	    
    	if(EXTI_GetITStatus(EXTI_Line4))//有中断响应 
    	{   
			
			delay_us(25);
			
			beef_pwm_init();
			if(key_flag==0)
			key_flag=1;
            //清除中断标志位 
			beef_start_music();
			beef_init();
            EXTI_ClearITPendingBit(EXTI_Line4); 
    	} 
		delay_us(25);
}
void EXTI9_5_IRQHandler(void)                         
{
	   
	    if(EXTI_GetITStatus(EXTI_Line5))//有中断响应 
    	{   
			delay_ms(15);
			beef_pwm_init();
			Send_Information(1,1,1);
			
			if(key_flag==1)
			{
				key_flag=0;
			}
			beef_end_music();
			beef_init();
            //清除中断标志位 
            EXTI_ClearITPendingBit(EXTI_Line5); 
    	} 
}