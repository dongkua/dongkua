/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   DS18B20�¶ȴ�������ȡ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 ָ���� ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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

volatile uint32_t time = 0; // ms ��ʱ���� 
volatile uint32_t time2 = 0; // ms ��ʱ���� 
float TM1,TM2;
volatile uint8_t ucDs18b20Id[8];

volatile	uint16_t Step_Auto;	
volatile	uint32_t PID_cool_cnt;
volatile	uint32_t PID_heat_cnt;
volatile	uint16_t pid_self_first_status_flag;	//��־λ
volatile	uint16_t PID_Deal,PID_auto_Deal;
volatile	uint16_t zero_across_counter;
volatile	uint16_t k_pid_self_counter;
volatile	uint16_t CCR4_Val;
volatile	uint16_t kkk;
//volatile 	uint16_t save_buff[9000] = {0};
void delay();

void PID_Init()
{
		pid.Sv=30;		//�û��趨�¶�
		pid.OUT0=1;

		zero_across_counter=0;
		PID_cool_cnt=0;
		PID_heat_cnt=0;
		PID_Deal=0;
		PID_auto_Deal=0;
		pid_self_first_status_flag = 0;
		k_pid_self_counter=0;
		if( pid.Pv <= pid.Sv)	//�趨�¶ȸ��ڵ�ǰ�¶� 
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
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
		delay();
	CCR4_Val=50;
	ADVANCE_TIM_Init();
	GENERAL_TIM_Init();
	
	/* ����SysTick Ϊ1us�ж�һ�� */
	SysTick_Init();
	
	
//	/* �˿ڳ�ʼ�� */
//	LED_GPIO_Config();//��ʼ����PB1��
	
	

	
	USART_Config();	//��ʼ������1
	printf("\r\n this is a ds18b20 test demo \r\n");
	while( DS18B20_Init() )	
	printf("\r\n no ds18b20 exit \r\n");//��ʼ��DS18B20������ʼ���͵ȴ�����?
	printf("\r\n ds18b20 exit \r\n");
	DS18B20_ReadId(ucDs18b20Id); 
	TM1=DS18B20_GetTemp_MatchRom ( ucDs18b20Id );
	pid.Pv=TM1;
	PID_Init();  //������ʼ��	
	pid.Sv1=28;	
	while(1)	
	{	
		printf ( "\r\n%.1f",TM1);		// ��ӡͨ�� DS18B20 ���кŻ�ȡ���¶�ֵ	
		Delay_ms(10);
		TM1=DS18B20_GetTemp_MatchRom ( ucDs18b20Id );
		pid.Pv=TM1;//��ǰ�¶�	 
		pid.Pv=TM1;//��ǰ�¶�	
		PID_auto();
		PID_Calc(); //pid����								
//		printf ( "\r\n%.1f",TM1);		// ��ӡͨ�� DS18B20 ���кŻ�ȡ���¶�ֵ	
//		Delay_ms(1000);
//		if ( time >= 10 ) /* 10 * 1 ms = 10ms ʱ�䵽 */
//    {
//      time = 0;  
//			TM1=DS18B20_GetTemp_MatchRom ( ucDs18b20Id );
//			pid.Pv=TM1;//��ǰ�¶�	
//    }
//		if ( time2 >= 1000 ) /* 1000 * 1 ms = 1s ʱ�䵽 */
//    {
//      time2 = 0;  
//			pid.Pv=TM1;//��ǰ�¶�	
//			PID_auto();
//			PID_Calc(); //pid����								
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
