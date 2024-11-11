#ifndef _DELAY_TIME_H
#define _DELAY_TIME_H
#include "main.h"

void Sitck_Init_j();
void delay_us_j(uint16_t nus);
void delay_ms_j(uint16_t nus);
void Sitck_Init();
void delay_ms_systick(unsigned int nums);
void SysTick_Handler(void);
void TIM6_DAC_IRQHandler(void);
void NVIC_Config_time6();
void Time6_Config();
void Pwm_tim2_Ch3_Beep(unsigned int n);
void Pwm_tim1_Ch1_Led();
#endif