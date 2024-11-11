#include "adc.h"
void PHtores_ADC_Init()
{
	//1.ʹ��ʱ��
	//adc1λ��apb2   gpioaλ��AHB1
	//ʹ��gpioaʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	//adc1��·ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//2.gpio����
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_AN;
	gpio_struct.GPIO_Pin=GPIO_Pin_0;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_struct);
	//3.adc �������ü���ʼ��
	ADC_CommonInitTypeDef adc_comminstruct;
	adc_comminstruct.ADC_Mode=ADC_Mode_Independent;//����ģʽ
	adc_comminstruct.ADC_Prescaler=ADC_Prescaler_Div2;//��Ƶ
	ADC_CommonInit(&adc_comminstruct);
	
	//4.adc�ṹ�����ü���ʼ��
	ADC_InitTypeDef adc_initstruct;
	adc_initstruct.ADC_Resolution=ADC_Resolution_12b;
	adc_initstruct.ADC_ContinuousConvMode=DISABLE;//�ر�ɨ��Դ
	adc_initstruct.ADC_DataAlign=ADC_DataAlign_Right;//�����Ҷ���
	adc_initstruct.ADC_ExternalTrigConv=ADC_ExternalTrigConvEdge_None;//ʹ���ڲ��������
	adc_initstruct.ADC_NbrOfConversion=1;//����ת��
	ADC_Init(ADC1,&adc_initstruct);
	//5.ʹ��adcͨ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_3Cycles);
	//adcʹ��
	ADC_Cmd(ADC1,ENABLE);
	
}
//��ȡadcֵ
u16 get_adc_val()
{
	//���ù���ͨ��
	//1.adc1,ͨ��0��һ�����У�480���������ڣ���߲���ʱ�������߾�ȷ�ȣ����ǲ���ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_480Cycles);
	//2.��ʼת��
	ADC_SoftwareStartConv(ADC1);
	//3.�ȴ�ת������
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	//4.��ȡת�����
	return ADC_GetConversionValue(ADC1);
}
/*
	��ȡͨ��ch��ת��ֵ��ȡtimes�Σ�Ȼ��ƽ��
	����ch��times��ת�����ƽ��ֵ
*/
unsigned short int getAdcAverage1(unsigned char ch, unsigned char times)
{
	//һ��ͨ���ɼ�5�Σ�ȡ�ͣ���ƽ��ֵ
	unsigned short int temp_val=0;
	unsigned char t;
	for(t =0;t<times;t++)
	{
		temp_val+= get_adc_val();
		delay_ms(5);
	}
	return temp_val/times;
}	
	