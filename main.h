#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f4xx.h" 
#include "stm32f4xx_conf.h" 

extern char wifi_buf[];
extern int wifi_len;
extern int wifi_work_flag;
extern int wifi_rec_flag;
void wen_shi_du();
void guang_zao();

#endif