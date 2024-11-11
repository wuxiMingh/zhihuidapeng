#ifndef _LED_C
#define _LED_C
#include "stm32f4xx_it.h"

void Led_init();
void led_open();
void led_close();
void led_shan(int led_falg);
void pwm_led_init();
int led_huxi(u16 *led0pwmval,u8 *dir);
void fan_init();
void fan_open();
void fan_close();
#endif