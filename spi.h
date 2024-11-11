#ifndef _SPI_H
#define _SPI_H
#include "main.h"

#if 0
//通过查阅原理图发现 css 选择pb2  spi_sck1:pb3 spi_miso1:pb4
#define W25Q_CS_PIN   GPIO_Pin_2
#define SPI_SCK1_PIN   GPIO_Pin_3
#define SPI_MISO1_PIN   GPIO_Pin_4
#define SPI_MOST1_PIN  GPIO_Pin_5

#define W25Q_CS_Write(N) (N)?GPIO_SetBits(GPIOB,W25Q_CS_PIN):GPIO_ResetBits(GPIOB,W25Q_CS_PIN)


//spi.c函数声明
void SPI1_Init(void);
u16 W25Q128_SPI_READ_WRITE(u16 data);
uint16_t W25Q128_ReadDevicedId(void);
uint8_t W25Q128_SendByte(u8 byte);
#endif
#define W25Q_CS_PIN   GPIO_Pin_2
#define SPI_SCK1_PIN  GPIO_Pin_3
#define SPI_MISO1_PIN GPIO_Pin_4
#define SPI_MOST1_PIN GPIO_Pin_5

#define W25_CS_Write(N)  N?GPIO_SetBits(GPIOB,W25Q_CS_PIN):GPIO_ResetBits(GPIOB,W25Q_CS_PIN)

void SPI1_Init(void);
u16 W25Q128_SPT_READ_WRITE(u16 data);
uint16_t W25Q128_ReadDeviceld(void);
#endif