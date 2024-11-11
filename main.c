#include "main.h"
#include "uart1.h"
#include "delay.h"
#include "wifi.h"
#include "nvic.h"
#include "led.h"
#include "oled.h"
#include "dht11.h"
#include "beef.h"
#include "adc.h"
#include "key.h"
#include "FreeRTOS.H"
#include "task.h"
int beef_flag_sun=0;
int key_flag=0;//按键标志位，控制按键开关
int beef_falg_ws=0;


//呼吸灯参数
u16 led0pwmval=0;    
u8 dir=1;

//温度湿度光照值数据初始化

u8 humi=0,temp=0;
u16 temp_guan=0;

//呼吸操作标志位，外部声明
extern int wifi_huxiBeef_flag;
extern int wifi_fan_flag;
int wifi_huxiLed_flag=1;
int wifi_oled_flag=1;
void wen_shi_du()
{
	
	if(DHT11_WORK(&humi,&temp))
	{
		char w[20],s[20];
		sprintf(w,"%d",temp);
		Oled_print(48,0,w);
		sprintf(s,"%d",humi);
		Oled_print(48,3,s);                                 
	}
	
	if(temp>=29)
	{
		if(beef_falg_ws==0)
		beef_shan();                            
		beef_falg_ws=1;
	}
	else if(temp<15)
	{
		if(beef_falg_ws==0)
		beef_shan();
		beef_falg_ws=1;
	}
	else
	{
		beef_falg_ws=0;
		fan_close();
	}
	
}
void guang_zao()
{
	char sun[20];
	temp_guan=get_adc_val();
	if(temp_guan<1000)
	{
		Oled_print(72,6," ");
	}
	sprintf(sun,"%d",temp_guan);
	Oled_print(48,6,sun);
	if(temp_guan>=3000)
	{
		if(beef_flag_sun==0)
		beef_shan();
		GPIO_ResetBits(GPIOE,GPIO_Pin_10);
		beef_flag_sun=1;
	}
	else
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_10);
		beef_flag_sun=0;
	}
}
//项目stm32
#if 0
int main()
{

	nvic_Config();
	
	Led_init();
	fan_init();
	delay_init(168);
	beef_init();
	PHtores_ADC_Init();
	
	//按键初始化
	Key_Config();
	EXTI4_Config();
	
 	pwm_led_init();
	//beef_pwm_init();
	//wifi初始化
	
	usart1_Config();
    Usart2_Config();
    Set_Server_Mode();
    //Set_Client_Mode();
    printf("数据发送完成\r\n");
	//oled 初始化
	oled_init(); 
	Tim7_Config();
	while(1)
	{	
		delay_us(100);
		if(key_flag)
		{
			TIM_Cmd(TIM7,ENABLE);
			//呼吸灯表示工作状态
			if( wifi_huxiLed_flag==1)
			{
				led_huxi(&led0pwmval,&dir);
				delay_us(5);
			}
			else
			{
				TIM_SetCompare1(TIM1,200);
			}
			
			if(wifi_fan_flag==1||temp>=29)
			{
				fan_open();
			}
			else 
			{
				delay_ms(10);
				fan_close();
			}
			//beef_huxi(&led0pwmval,&dir);
			//oled显示屏控制
			if(wifi_oled_flag)
			{
				oled_test();
				wen_shi_du();
				guang_zao();
			}
			else
			{
				OLed_Fill(0x00);
			}
			//控制wifi_beefhuxi
			if(wifi_huxiBeef_flag==1)
			{
				beef_pwm_init();
				 beef_huxi(&led0pwmval,&dir);
			}
			else
			{
				 TIM_SetCompare3(TIM2,0);
				 beef_init();
			}
			   
	    }
		else
		{
			OLed_Fill(0x00);
			fan_close();
			TIM_SetCompare1(TIM1,200);
			TIM_Cmd(TIM7,DISABLE);
		}
	}
}
#endif
//dma
#if 0
uint32_t data_src[10]={0x1111,0x2222,0x3333,0x4444};//源数据
uint32_t data_drc[10]={0};//目标
int main()
{
	
}
#endif

#if 0
//freertos 动态创建

//task_start 创建配置
#define TASK_START_SIZE 128
#define TASK_START_PRIO 1
TaskHandle_t Task_Start_JuBin;//用来删除任务的，类似linux文件描述符

//task1  创建配置
#define TASK1_SIZE 128
#define TASK1_PRIO 2
TaskHandle_t Task1_JuBin;

//task2 创建配置
#define TASK2_SIZE 128
#define TASK2_PRIO 3
TaskHandle_t Task2_JuBin;
//taks3 创建配置
#define TASK3_SIZE 128
#define TASK3_PRIO 4
TaskHandle_t Task3_JuBin;

void task1(void *pvParameters)
{
	while(1)
	{
		printf("111111111111\r\n");
		GPIO_SetBits(GPIOE,GPIO_Pin_8);
		vTaskDelay(100);
		GPIO_ResetBits(GPIOE,GPIO_Pin_8);
		vTaskDelay(100);
	}
}
void task2(void *pvParameters)
{
	while(1)
	{
		printf("222222222\r\n");
		GPIO_SetBits(GPIOE,GPIO_Pin_9);
		vTaskDelay(100);
		GPIO_ResetBits(GPIOE,GPIO_Pin_9);
		vTaskDelay(100);
	}
}
void task3(void *pvParameters)
{
	while(1)
	{
		printf("3333333333\r\n");
		GPIO_SetBits(GPIOE,GPIO_Pin_10);
		vTaskDelay(1000);
		GPIO_ResetBits(GPIOE,GPIO_Pin_10);
		vTaskDelay(100);
	}
}
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();
	xTaskCreate( (TaskFunction_t)           task1,
                 (char *)                   "task1",
				  (configSTACK_DEPTH_TYPE)  TASK_START_SIZE,
                 (void *)                   NULL,
                 (UBaseType_t)              TASK1_PRIO ,
                 (TaskHandle_t *)           &Task1_JuBin);
	xTaskCreate( (TaskFunction_t)           task2,
                 (char *)                   "task2",
				  (configSTACK_DEPTH_TYPE)  TASK2_SIZE,
                 (void *)                   NULL,
                 (UBaseType_t)              TASK2_PRIO ,
                 (TaskHandle_t *)           &Task2_JuBin);
	xTaskCreate( (TaskFunction_t)           task3,
                 (char *)                   "task3",
				  (configSTACK_DEPTH_TYPE)  TASK3_SIZE,
                 (void *)                   NULL,
                 (UBaseType_t)              TASK3_PRIO ,
                 (TaskHandle_t *)           &Task3_JuBin);
	vTaskDelete(NULL);
	taskEXIT_CRITICAL();
}
int main()
{
	Led_init();
	//nvic_Config();
	delay_init(168);
	usart1_Config();
	
	xTaskCreate( (TaskFunction_t)           start_task,
                 (char *)                   "start_task",
				  (configSTACK_DEPTH_TYPE)  TASK_START_SIZE,
                 (void *)                   NULL,
                 (UBaseType_t)              TASK_START_PRIO ,
                 (TaskHandle_t *)           &Task_Start_JuBin);
	vTaskStartScheduler();
	return 0;
}
#endif

#if 1
//静态创建任务

int main()
{
	
}
#endif