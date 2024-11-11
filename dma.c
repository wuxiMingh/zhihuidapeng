#include "dma.h"
#include "stdio.h"
#include "stm32f4xx_it.h"

//�Ĵ������Ĵ�������
void drv_dma_init(uint32_t *src,uint32_t *drc,uint32_t size)
{
	DMA_InitTypeDef DMA_InitStruct;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_InitStruct.DMA_BufferSize =size;//��Ҫ���˵�����
	DMA_InitStruct.DMA_Channel = DMA_Channel_0;//ͨ��0
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToMemory;//���ݴ��䷽���ڴ浽�ڴ�
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;//ֱ��ģʽ
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)drc;//Ŀ���ַ
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)src;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA2_Stream0, &DMA_InitStruct);
	DMA_Cmd(DMA2_Stream0, ENABLE);
}
///�Ĵ���������adc��dma
void dma_Adc_init(uint32_t *src,uint32_t *drc,uint32_t size)
{
	DMA_InitTypeDef DMA_InitStruct;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_InitStruct.DMA_BufferSize =size;//��Ҫ���˵�����
	DMA_InitStruct.DMA_Channel = DMA_Channel_0;//ͨ��0
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;//���ݴ��䷽���ڴ浽�ڴ�
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;//ֱ��ģʽ
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)drc;//Ŀ���ַ
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = 0x40026400+0x4c;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA2_Stream0, &DMA_InitStruct);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
}
//extern uint32_t data_src[10];//Դ����
//extern uint32_t data_drc[10];//Ŀ��
//void drv_dma_compare(void)
//{
//	int i=0;
//	for(i=0;i<10;i++)
//	{
//		if(data_drc[i]!= data_src[i])
//		{
//			printf("data_err[%d] data=0x%x\r\n",i,data_drc[i]);
//			return;
//		}
//		printf("data_drc[i]=0x%x\r\n",data_drc[i]);
//	}

//}
