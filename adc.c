#include "adc.h"
void PHtores_ADC_Init()
{
	//1.使能时钟
	//adc1位于apb2   gpioa位于AHB1
	//使能gpioa时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	//adc1电路使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//2.gpio配置
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_AN;
	gpio_struct.GPIO_Pin=GPIO_Pin_0;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_struct);
	//3.adc 外设配置及初始化
	ADC_CommonInitTypeDef adc_comminstruct;
	adc_comminstruct.ADC_Mode=ADC_Mode_Independent;//独立模式
	adc_comminstruct.ADC_Prescaler=ADC_Prescaler_Div2;//分频
	ADC_CommonInit(&adc_comminstruct);
	
	//4.adc结构体配置及初始化
	ADC_InitTypeDef adc_initstruct;
	adc_initstruct.ADC_Resolution=ADC_Resolution_12b;
	adc_initstruct.ADC_ContinuousConvMode=DISABLE;//关闭扫描源
	adc_initstruct.ADC_DataAlign=ADC_DataAlign_Right;//数据右对齐
	adc_initstruct.ADC_ExternalTrigConv=ADC_ExternalTrigConvEdge_None;//使用内部软件触发
	adc_initstruct.ADC_NbrOfConversion=1;//单次转换
	ADC_Init(ADC1,&adc_initstruct);
	//5.使能adc通道
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_3Cycles);
	//adc使能
	ADC_Cmd(ADC1,ENABLE);
	
}
//获取adc值
u16 get_adc_val()
{
	//设置规则通道
	//1.adc1,通道0，一个序列，480个机器周期，提高采样时间可以提高精确度，这是采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_480Cycles);
	//2.开始转换
	ADC_SoftwareStartConv(ADC1);
	//3.等待转换结束
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	//4.获取转换结果
	return ADC_GetConversionValue(ADC1);
}
/*
	获取通道ch的转换值，取times次，然后平均
	返回ch的times次转换结果平均值
*/
unsigned short int getAdcAverage1(unsigned char ch, unsigned char times)
{
	//一个通道采集5次，取和，求平均值
	unsigned short int temp_val=0;
	unsigned char t;
	for(t =0;t<times;t++)
	{
		temp_val+= get_adc_val();
		delay_ms(5);
	}
	return temp_val/times;
}	
	