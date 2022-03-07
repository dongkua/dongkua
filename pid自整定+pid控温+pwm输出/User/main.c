/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   DS18B20温度传感器读取。
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 指南者 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "core_cm3.h"
#include "./systick/bsp_SysTick.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./ds18b20/bsp_ds18b20.h"
#include "./pid/pid.h" 
#include "./timer/timer.h"
#include "./AdvanceTim/bsp_AdvanceTim.h" 
#include "./GeneralTim/bsp_GeneralTim.h" 

volatile uint32_t time = 0; // ms 计时变量 
volatile uint32_t time2 = 0; // ms 计时变量 
float TM1,TM2;
volatile uint8_t ucDs18b20Id[8];

volatile	uint16_t Step_Auto;	
volatile	uint32_t PID_cool_cnt;
volatile	uint32_t PID_heat_cnt;
volatile	uint16_t pid_self_first_status_flag;	//标志位
volatile	uint16_t PID_Deal,PID_auto_Deal;
volatile	uint16_t zero_across_counter;
volatile	uint16_t k_pid_self_counter;
volatile	uint16_t CCR4_Val;
volatile	uint16_t kkk;
//volatile 	uint16_t save_buff[9000] = {0};
void delay();

void PID_Init()
{
		pid.Sv=30;		//用户设定温度
		pid.OUT0=1;

		zero_across_counter=0;
		PID_cool_cnt=0;
		PID_heat_cnt=0;
		PID_Deal=0;
		PID_auto_Deal=0;
		pid_self_first_status_flag = 0;
		k_pid_self_counter=0;
		if( pid.Pv <= pid.Sv)	//设定温度高于当前温度 
		{
			pid.Sv1=(pid.Sv-pid.Pv)/2+pid.Pv;	
			pid.Sv1=(pid.Sv-pid.Pv)/2+pid.Pv;	
			pid.Sv1=(pid.Sv-pid.Pv)/2+pid.Pv;	
			pid.Sv1=(pid.Sv-pid.Pv)/2+pid.Pv;	
			pid.Sv1=(pid.Sv-pid.Pv)/2+pid.Pv;	
			
		}		
//		CCR4_Val=100;
//		PID_auto_Deal=1;
//		PID_Deal=1;
//		pid.Sv=30;
//		pid.Kp=38.0999985;
//		pid.Ki=0.21666673;
//		pid.Kd=2.8575;
		
	
}



/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
		delay();
	CCR4_Val=50;
	ADVANCE_TIM_Init();
	GENERAL_TIM_Init();
	
	/* 配置SysTick 为1us中断一次 */
	SysTick_Init();
	
	
//	/* 端口初始化 */
//	LED_GPIO_Config();//初始化了PB1口
	
	

	
	USART_Config();	//初始化串口1
	printf("\r\n this is a ds18b20 test demo \r\n");
	while( DS18B20_Init() )	
	printf("\r\n no ds18b20 exit \r\n");//初始化DS18B20，不初始化就等待这里?
	printf("\r\n ds18b20 exit \r\n");
	DS18B20_ReadId(ucDs18b20Id); 
	TM1=DS18B20_GetTemp_MatchRom ( ucDs18b20Id );
	pid.Pv=TM1;
	PID_Init();  //参数初始化	
	pid.Sv1=28;	
	while(1)	
	{	
		printf ( "\r\n%.1f",TM1);		// 打印通过 DS18B20 序列号获取的温度值	
		Delay_ms(10);
		TM1=DS18B20_GetTemp_MatchRom ( ucDs18b20Id );
		pid.Pv=TM1;//当前温度	 
		pid.Pv=TM1;//当前温度	
		PID_auto();
		PID_Calc(); //pid计算								
//		printf ( "\r\n%.1f",TM1);		// 打印通过 DS18B20 序列号获取的温度值	
//		Delay_ms(1000);
//		if ( time >= 10 ) /* 10 * 1 ms = 10ms 时间到 */
//    {
//      time = 0;  
//			TM1=DS18B20_GetTemp_MatchRom ( ucDs18b20Id );
//			pid.Pv=TM1;//当前温度	
//    }
//		if ( time2 >= 1000 ) /* 1000 * 1 ms = 1s 时间到 */
//    {
//      time2 = 0;  
//			pid.Pv=TM1;//当前温度	
//			PID_auto();
//			PID_Calc(); //pid计算								
//		}		
	}	 
}				
void delay(void)
{
	int i;
	for(i=0; i<10000000; i++)
		;
}

/*********************************************END OF FILE**********************/
