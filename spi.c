#include "main.h"
#include "spi.h"
#if 0
//spi��ʼ��
void SPI1_Init(void)
{
	//1.ʹ��ʱ�ӣ�spi�ӿ�λ��apb2������ gpiobλ��ahb1������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//2.1gpio�ṹ������
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_AF;//����ģʽ
	gpio_struct.GPIO_Pin=SPI_SCK1_PIN|SPI_MISO1_PIN|SPI_MOST1_PIN;
	gpio_struct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	//2.2����css���� w25Q_cs ����
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_OType=GPIO_OType_PP;
	gpio_struct.GPIO_Pin=W25Q_CS_PIN;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	//���øߵ�ƽ--->����״̬  ���;���ζҪ��ʼ��ӻ�ͨ��
	W25Q_CS_Write(1);
	//3.�ܽŸ���
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);
	//4.spi�ṹ������ ����ʼ��
	SPI_InitTypeDef spi1_struct;
	spi1_struct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//ȫ˫��
	spi1_struct.SPI_Mode=SPI_Mode_Master;//����ģʽ
	spi1_struct.SPI_DataSize=SPI_DataSize_8b;//��������λ
	//���ݴӻ�ģʽ ����ѡ��λ ģʽ0��ģʽ3 ������ѡ��ģʽ3 ���ڶ������� �ɼ����� ʱ�Ӽ���λ
	spi1_struct.SPI_CPOL=SPI_CPOL_High;//����Ϊ�ߣ�
	spi1_struct.SPI_CPHA=SPI_CPHA_2Edge;//�ڶ������زɼ�����
	spi1_struct.SPI_NSS=SPI_NSS_Soft;//�����������
	spi1_struct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;//4����
	spi1_struct.SPI_FirstBit=SPI_FirstBit_MSB;//��λ��ǰ
	SPI_Init(SPI1,&spi1_struct);
	//5.ʹ��spi
	SPI_Cmd(SPI1,ENABLE);
}
//��������Ҫ����豸����ͨ�ţ���ôҪ�鿴���豸�ֲ��ĵ�
//�����ڷ������ݵ�ͬʱ��ҲҪ��������
u16 W25Q128_SPI_READ_WRITE(u16 data)
{
	while(SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)==RESET);//�ȴ����ݴ������
	SPI_SendData(SPI1,data);
	while(SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE)==RESET);
	return SPI_ReceiveData(SPI1);//��������
}



/*
��ȡ����id��������
1.u8 W25Q128_SendByte(u8 byte)
2.��ȡfalsh

*/
//��ȡflash����
uint8_t W25Q128_SendByte(u8 byte){
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
	
	SPI_I2S_SendData(SPI1,byte);
	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
	
	return SPI_I2S_ReceiveData(SPI1);
}
uint16_t W25Q128_ReadDevicedId(void)
{
	uint16_t id=0;
	//1.��csƬѡ�������� ��ʾѡ��
	W25Q_CS_Write(0);
	//2.����ָ��
	W25Q128_SPI_READ_WRITE(0X90);
	//3.���͵�ַ  24bits
	W25Q128_SPI_READ_WRITE(0X00);
	W25Q128_SPI_READ_WRITE(0X00);
	W25Q128_SPI_READ_WRITE(0X00);
	//4.���ճ���id���豸id ���������������ݼ���
	id = W25Q128_SPI_READ_WRITE(0x00)<<8;
	id |= W25Q128_SPI_READ_WRITE(0x00);
	//5.����csƬѡ����  ��ʾ��ͨ��
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