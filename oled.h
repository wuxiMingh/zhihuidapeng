#ifndef _OLED_H
#define _OLED_H

#include "main.h"
void WriteOLedCmd(uint8_t cmd);
void WriteOLedData(uint8_t data);
void OLed_Fill(unsigned char bmp_data);
void OLed_SetPos(unsigned char x, unsigned char y);
void InitOLed(void);
void Oled_print(uint8_t x, uint8_t y,char *str);
void OLed_ShowChina(uint8_t x,uint8_t y,uint8_t *buf);
int oled_test();
void oled_init();
#endif