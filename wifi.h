#ifndef _WIFI_H
#define _WIFI_H

#include "stm32f4xx_it.h"
void Usart2_Config();
void Usart2_Send_str(char *s);
void Set_Server_Mode();
void Set_Client_Mode();
void Send_Information(u8 humi, u8 temp, u16 val);
#endif