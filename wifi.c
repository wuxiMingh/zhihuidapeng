
#include "stm32f4xx.h"
#include "string.h"
#include "main.h"
#include "delay.h"
#include "led.h"
#include "stdio.h"
#include "oled.h"
extern int key_flag;
extern u16 led0pwmval;    
extern u8 dir;
extern int wifi_huxiLed_flag;
extern int wifi_oled_flag;
int wifi_rec_flag =0; //wifi���ݽ��ձ�־λ
int wifi_work_flag =0; //wifiģ�����˹�����־λ
char wifi_buf[1024] ={0};
int wifi_len =0;
//���տ��ر�־λ����
int wifi_led_flag=0;
int wifi_huxiBeef_flag=0;
int wifi_fan_flag=0;
//
extern u8 humi;extern u8 temp;
extern u16 temp_guan;
/*
  ���ձ�־λ������
	���������ص�һЩ����
	
*/
void Key_flag()
{
	if(key_flag==0)
	{
		key_flag=1;
	}
	else
	{
		key_flag=0;
	}
}

void Wifi_led_flag()
{
	 if(wifi_led_flag==0)
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_8);
		wifi_led_flag=1;
	}
	else
	{
		led_close();
		wifi_led_flag=0;
	}
}

void Wifi_huxiBeef_flag()
{
	if(wifi_huxiBeef_flag==0)
	{
		wifi_huxiBeef_flag=1;
	}
	else
	{
		wifi_huxiBeef_flag=0;
	}
	
}

void Wifi_fan_flag()
{
	if(wifi_fan_flag==0)
	{
		wifi_fan_flag=1;
	}
	else
	{
		wifi_fan_flag=0;
	}
}


void Wifi_huxiLed_flag()
{
	if( wifi_huxiLed_flag==0)
	{
		wifi_huxiLed_flag=1;
	}
	else if(wifi_huxiLed_flag==1)
	{
		wifi_huxiLed_flag=0;
	}
}
void Wifi_oled_flag()
{
	if(wifi_oled_flag==0)
	{
		wifi_oled_flag=1;
	}
	else
	{
		wifi_oled_flag=0;
	}
}
//�ϰ�
void EXTI3_IRQHandler()
{
	printf("1111111\r\n");
	if(EXTI_GetITStatus(EXTI_Line3))//���ж���Ӧ 
	{   
		delay_us(55);
		
		if(wifi_huxiLed_flag==1)
		{
			TIM_SetCompare1(TIM1,200);
			Led_init();
			wifi_huxiLed_flag=0;
		}
		else
		{
			pwm_led_init();
			led_huxi(&led0pwmval,&dir);
		}
		//����жϱ�־λ 
		EXTI_ClearITPendingBit(EXTI_Line3); 
    } 
}


//�����жϺ���
void USART2_IRQHandler ()
{
	
    u8 res;
    u8 usart;
    if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET) //�����ж�
    {
         //��������жϱ�־λ
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        res = USART_ReceiveData(USART2);
        wifi_buf[wifi_len++] = res;
        USART_SendData(USART1,res);//ֱ�Ӵ�ӡ����������
        if(wifi_work_flag)
        {
            switch(res)
            {
                case '*':Key_flag();break;

                case '&':Wifi_led_flag();break;
                 
				case '#':Wifi_huxiBeef_flag();break;
				
				case '$':Wifi_fan_flag();break;
				
				case '^':Wifi_huxiLed_flag();break;
				
				case '~':Wifi_oled_flag();break;
                break;
            }
        }
    }
    else if(USART_GetITStatus(USART2,USART_IT_IDLE) == SET)
    {
        usart = USART2->SR;
        usart = USART2->DR;
        if(wifi_rec_flag && strstr(wifi_buf,"CONNECT"))
        {
            wifi_work_flag =1;
        }
		wifi_len=0;
    }
}
void Usart2_Config()
{
    //����ʱ��ʹ�� USART2λ��APB1����
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    //GPIOʱ��ʹ�� PD6,PD5 λ��AHB1����
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    //�ܽŸ���
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);
    //GPIO���� �Լ���ʼ��
    GPIO_InitTypeDef gpio_struct;
    gpio_struct.GPIO_Mode = GPIO_Mode_AF;
    gpio_struct.GPIO_OType = GPIO_OType_PP;
    gpio_struct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    gpio_struct.GPIO_PuPd = GPIO_PuPd_UP;
    gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD,&gpio_struct);
    //���ڵ������Լ���ʼ��
    USART_InitTypeDef usart2;
    usart2.USART_BaudRate = 115200; //������
    usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ����������
    usart2.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
    usart2.USART_Parity = USART_Parity_No;//��У��
    usart2.USART_StopBits = USART_StopBits_1;
    usart2.USART_WordLength = USART_WordLength_8b;//�����ֳ�Ϊ 8λ
    USART_Init(USART2,&usart2);
    //����ʹ��
    USART_Cmd(USART2,ENABLE);
    //���ڽ����ж�ʹ��
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);//�����ж�
}


//ͨ������2��������
void Usart2_Send_str(char *s)
{
    int len = strlen(s);
    int i;
    for(i=0;i<len;i++)
    {
        USART_SendData(USART2,*s);
        while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET); //�ȴ�����
        s++;
    }
}

//��ESPB266����Ϊ�����ģʽ
void Set_Server_Mode()
{
    char *ESP8266_AP_Server[] ={
    "AT\r\n",
    "ATE1\r\n",
    "AT+CWMODE=2\r\n",
    "AT+RST\r\n",
    "ATE1\r\n",
    "AT+CWSAP=\"wxm\",\"12345678\",1,4\r\n",
    "AT+CIPMUX=1\r\n",
    "AT+CIPSERVER=1,8888\r\n",
    "AT+CIFSR\r\n"
    };
    int i=0;
    for(i=0;i<9;i++)
    {
        Usart2_Send_str(ESP8266_AP_Server[i]);
        delay_ms(200);
    }
    wifi_work_flag =1; //��ʼ����
}

void Set_Client_Mode()
{
    char *ESP8266_STA_Client[]={
    "AT\r\n",//���ESP8266ģ���Ƿ���������
    "ATE1\n\n",//�رջ��Թ���
    "AT+CWMODE=1\r\n",//����ΪSTAģʽ
    "AT+RST\r\n",//����ESP8266ģ��
    "ATE1\r\n",//�رջ��Թ���
    "AT+CWLAP\rn",//��ѯ�������ӵ�WIFI
    "AT+CWJAP:=\"wxm\",\"12345678\"\r\n",//����ָ����WIFI
    "AT+CIPMUX=O\r\n",//����Ϊ������ģʽ
    "AT+CIPMODE=1\r\n",//����Ϊ͸��ģʽ
    "AT+CIPSTART:=\"TCP\",\"192.168.4.1\",8888\r\n",//����TCP����
    //"AT+CIPSEND\r\n",//��������
    };
    int i=0;
    for(i=0;i<10;i++)
    {
        Usart2_Send_str(ESP8266_STA_Client[i]);
        delay_ms(1000);
    }
}

//��ʱ�������ݺ���
//1.��������
void Send_Information(u8 humi, u8 temp, u16 val)
{
    char t[40];
    sprintf(t, "%d:%d:%d\r\n", humi, temp, val); // ��ʽ���ַ���

    // ׼��Ҫ���͵��ַ���
    char command[] = "AT+CIPSEND=0,10\r\n"; // ʹ����ͨ�ַ�����

    // ���������ַ���
    Usart2_Send_str(command); // ��������
	delay_ms(200); // ��ѡ�ķ��ͼ��
    Usart2_Send_str(t); // ���͸�ʽ������Ϣ
	delay_ms(200); // ��ѡ�ķ��ͼ��
}
//��ʱ���ж�
void  TIM7_IRQHandler()
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		Send_Information(humi,temp,temp_guan);
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//��������жϱ�־λ
	}

}