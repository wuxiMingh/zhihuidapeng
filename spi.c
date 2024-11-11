#include "main.h"
#include "spi.h"
#if 0
//spi初始化
void SPI1_Init(void)
{
	//1.使能时钟，spi接口位于apb2总线上 gpiob位于ahb1总线上
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//2.1gpio结构体配置
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_AF;//复用模式
	gpio_struct.GPIO_Pin=SPI_SCK1_PIN|SPI_MISO1_PIN|SPI_MOST1_PIN;
	gpio_struct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	//2.2配置css引脚 w25Q_cs 引脚
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_OType=GPIO_OType_PP;
	gpio_struct.GPIO_Pin=W25Q_CS_PIN;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	//设置高电平--->空闲状态  拉低就意味要开始与从机通信
	W25Q_CS_Write(1);
	//3.管脚复用
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);
	//4.spi结构体配置 及初始化
	SPI_InitTypeDef spi1_struct;
	spi1_struct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//全双工
	spi1_struct.SPI_Mode=SPI_Mode_Master;//主机模式
	spi1_struct.SPI_DataSize=SPI_DataSize_8b;//设置数据位
	//根据从机模式 可以选择位 模式0和模式3 ，我们选择模式3 ：第二跳变沿 采集数据 时钟极性位
	spi1_struct.SPI_CPOL=SPI_CPOL_High;//极性为高，
	spi1_struct.SPI_CPHA=SPI_CPHA_2Edge;//第二跳变沿采集数据
	spi1_struct.SPI_NSS=SPI_NSS_Soft;//设置软件管理
	spi1_struct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;//4分屏
	spi1_struct.SPI_FirstBit=SPI_FirstBit_MSB;//高位在前
	SPI_Init(SPI1,&spi1_struct);
	//5.使能spi
	SPI_Cmd(SPI1,ENABLE);
}
//由于我们要与从设备进行通信，那么要查看从设备手册文档
//由于在发送数据的同时，也要接收数据
u16 W25Q128_SPI_READ_WRITE(u16 data)
{
	while(SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)==RESET);//等待数据传输完成
	SPI_SendData(SPI1,data);
	while(SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE)==RESET);
	return SPI_ReceiveData(SPI1);//接收数据
}



/*
读取厂家id两个函数
1.u8 W25Q128_SendByte(u8 byte)
2.读取falsh

*/
//读取flash数据
uint8_t W25Q128_SendByte(u8 byte){
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
	
	SPI_I2S_SendData(SPI1,byte);
	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
	
	return SPI_I2S_ReceiveData(SPI1);
}
uint16_t W25Q128_ReadDevicedId(void)
{
	uint16_t id=0;
	//1.把cs片选引脚拉低 表示选中
	W25Q_CS_Write(0);
	//2.发送指令
	W25Q128_SPI_READ_WRITE(0X90);
	//3.发送地址  24bits
	W25Q128_SPI_READ_WRITE(0X00);
	W25Q128_SPI_READ_WRITE(0X00);
	W25Q128_SPI_READ_WRITE(0X00);
	//4.接收厂家id和设备id 主机发送任意数据即可
	id = W25Q128_SPI_READ_WRITE(0x00)<<8;
	id |= W25Q128_SPI_READ_WRITE(0x00);
	//5.拉高cs片选引脚  表示不通信
	W25Q_CS_Write(1);
	return id;
}
#endif
void SPI1_Init()
{
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_AF;
	gpio_struct.GPIO_Pin=SPI_SCK1_PIN|SPI_MISO1_PIN|SPI_MOST1_PIN;
	gpio_struct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_Pin=W25Q_CS_PIN;
	gpio_struct.GPIO_OType=GPIO_OType_PP;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&gpio_struct);
  
	W25_CS_Write(1);
	
	
GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);

SPI_InitTypeDef spi1_struct;
spi1_struct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
spi1_struct.SPI_Mode=SPI_Mode_Master;
spi1_struct.SPI_DataSize=SPI_DataSize_8b;

spi1_struct.SPI_CPOL=SPI_CPOL_High;
spi1_struct.SPI_CPHA=SPI_CPHA_2Edge;
spi1_struct.SPI_NSS=SPI_NSS_Soft;
spi1_struct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;
spi1_struct.SPI_FirstBit=SPI_FirstBit_MSB;
SPI_Init(SPI1,&spi1_struct);
SPI_Cmd(SPI1,ENABLE);
}

u16 W25Q128_SPI_READ_WRITE(u16 data)
{
 while(SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)==RESET);
 SPI_SendData(SPI1,data);
 while(SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE)==RESET);
	return SPI_ReceiveData(SPI1);
}

uint16_t W25Q128_ReadDeviceld(void)
{
 uint16_t ID=0;
 W25_CS_Write(0);
 W25Q128_SPI_READ_WRITE(0x90);
 W25Q128_SPI_READ_WRITE(0x00);
 W25Q128_SPI_READ_WRITE(0x00);
 W25Q128_SPI_READ_WRITE(0x00);
 ID  = W25Q128_SPI_READ_WRITE(0x00)<<8;
 ID |= W25Q128_SPI_READ_WRITE(0x00);
	W25_CS_Write(1);
	return ID;
	
 
}