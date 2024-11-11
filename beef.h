#ifndef _BEEF_H
#define _BEEF_H

#include "stm32f4xx_it.h"
void beef_init();
void beef_open();
void beef_close();
void beef_shan();
void beef_pwm_init();
void  beef_huxi(u16 *led0pwmval,u8 *dir);
void beef_start_music();
void beef_end_music();
#endif