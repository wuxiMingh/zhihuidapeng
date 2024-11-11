#include "key.h"
#include "stm32f4xx_it.h"
#include "delay.h"
#include "oled.h"
#include "led.h"
#include "wifi.h"
#include "beef.h"
extern int key_flag;//�����ⲿ�����������ƿ���
//�ⲿ�ж�4������ 
void EXTI4_Config(void) 
{ 
	EXTI_InitTypeDef exti_struct; 
//1 �ⲿ�ж�ʱ������-ϵͳʱ�ӡ�  42MHZ 
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE); 
//2.�ж���ӳ�亯�� PE4���ó� �ⲿ�ж�4��
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource3); 	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4); 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource5); 
//3.EXTI4�жϳ�ʼ�� 
	exti_struct.EXTI_Line=EXTI_Line3| EXTI_Line4|EXTI_Line5;//4����   
	exti_struct.EXTI_LineCmd=ENABLE;//�����ж� 
	exti_struct.EXTI_Mode=EXTI_Mode_Interrupt;//�ж� 
	exti_struct.EXTI_Trigger=EXTI_Trigger_Falling;//�½��� 
    EXTI_Init(&exti_struct); 
} 
//����1 2 3����  PE4 PE5 PE6 
void Key_Config(void) 
{ 
    //ʹ��ʱ�� 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); 
//����GPIO���� 
    GPIO_InitTypeDef gpio_struct; 
    gpio_struct.GPIO_Mode=GPIO_Mode_IN; 
    gpio_struct.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6; 
    gpio_struct.GPIO_PuPd=GPIO_PuPd_UP;//���� 
    gpio_struct.GPIO_Speed=GPIO_Speed_2MHz; 
    GPIO_Init(GPIOE,&gpio_struct); 
} 
//------------------�жϷ������ı�д 
//�ⲿ�ж�4 key1�ж�
void EXTI4_IRQHandler(void) 
{ 
	    
    	if(EXTI_GetITStatus(EXTI_Line4))//���ж���Ӧ 
    	{   
			
			delay_us(25);
			
			beef_pwm_init();
			if(key_flag==0)
			key_flag=1;
            //����жϱ�־λ 
			beef_start_music();
			beef_init();
            EXTI_ClearITPendingBit(EXTI_Line4); 
    	} 
		delay_us(25);
}
void EXTI9_5_IRQHandler(void)                         
{
	   
	    if(EXTI_GetITStatus(EXTI_Line5))//���ж���Ӧ 
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
            //����жϱ�־λ 
            EXTI_ClearITPendingBit(EXTI_Line5); 
    	} 
}