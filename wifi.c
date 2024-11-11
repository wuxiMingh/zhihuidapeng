
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
int wifi_rec_flag =0; //wifi数据接收标志位
int wifi_work_flag =0; //wifi模块服务端工作标志位
char wifi_buf[1024] ={0};
int wifi_len =0;
//接收开关标志位声明
int wifi_led_flag=0;
int wifi_huxiBeef_flag=0;
int wifi_fan_flag=0;
//
extern u8 humi;extern u8 temp;
extern u16 temp_guan;
/*
  接收标志位处理函数
	用来处理开关的一些函数
	
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
//废案
void EXTI3_IRQHandler()
{
	printf("1111111\r\n");
	if(EXTI_GetITStatus(EXTI_Line3))//有中断响应 
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
		//清除中断标志位 
		EXTI_ClearITPendingBit(EXTI_Line3); 
    } 
}


//接收中断函数
void USART2_IRQHandler ()
{
	
    u8 res;
    u8 usart;
    if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET) //接收中断
    {
         //清楚接收中断标志位
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        res = USART_ReceiveData(USART2);
        wifi_buf[wifi_len++] = res;
        USART_SendData(USART1,res);//直接打印到串口助手
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
    //串口时钟使能 USART2位于APB1总线
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    //GPIO时钟使能 PD6,PD5 位于AHB1总线
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    //管脚复用
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);
    //GPIO配置 以及初始化
    GPIO_InitTypeDef gpio_struct;
    gpio_struct.GPIO_Mode = GPIO_Mode_AF;
    gpio_struct.GPIO_OType = GPIO_OType_PP;
    gpio_struct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    gpio_struct.GPIO_PuPd = GPIO_PuPd_UP;
    gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD,&gpio_struct);
    //串口的配置以及初始化
    USART_InitTypeDef usart2;
    usart2.USART_BaudRate = 115200; //波特率
    usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件控制流无
    usart2.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
    usart2.USART_Parity = USART_Parity_No;//无校验
    usart2.USART_StopBits = USART_StopBits_1;
    usart2.USART_WordLength = USART_WordLength_8b;//数据字长为 8位
    USART_Init(USART2,&usart2);
    //串口使能
    USART_Cmd(USART2,ENABLE);
    //串口接收中断使能
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);//空闲中断
}


//通过串口2发送命令
void Usart2_Send_str(char *s)
{
    int len = strlen(s);
    int i;
    for(i=0;i<len;i++)
    {
        USART_SendData(USART2,*s);
        while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET); //等待发完
        s++;
    }
}

//将ESPB266设置为服务端模式
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
    wifi_work_flag =1; //开始工作
}

void Set_Client_Mode()
{
    char *ESP8266_STA_Client[]={
    "AT\r\n",//检测ESP8266模块是否正常工作
    "ATE1\n\n",//关闭回显功能
    "AT+CWMODE=1\r\n",//设置为STA模式
    "AT+RST\r\n",//重启ESP8266模块
    "ATE1\r\n",//关闭回显功能
    "AT+CWLAP\rn",//查询可以连接的WIFI
    "AT+CWJAP:=\"wxm\",\"12345678\"\r\n",//连接指定的WIFI
    "AT+CIPMUX=O\r\n",//设置为单连接模式
    "AT+CIPMODE=1\r\n",//设置为透传模式
    "AT+CIPSTART:=\"TCP\",\"192.168.4.1\",8888\r\n",//建立TCP连接
    //"AT+CIPSEND\r\n",//发送数据
    };
    int i=0;
    for(i=0;i<10;i++)
    {
        Usart2_Send_str(ESP8266_STA_Client[i]);
        delay_ms(1000);
    }
}

//定时发送数据函数
//1.发送数据
void Send_Information(u8 humi, u8 temp, u16 val)
{
    char t[40];
    sprintf(t, "%d:%d:%d\r\n", humi, temp, val); // 格式化字符串

    // 准备要发送的字符串
    char command[] = "AT+CIPSEND=0,10\r\n"; // 使用普通字符数组

    // 发送所有字符串
    Usart2_Send_str(command); // 发送命令
	delay_ms(200); // 可选的发送间隔
    Usart2_Send_str(t); // 发送格式化的信息
	delay_ms(200); // 可选的发送间隔
}
//定时器中断
void  TIM7_IRQHandler()
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		Send_Information(humi,temp,temp_guan);
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//清除更新中断标志位
	}

}