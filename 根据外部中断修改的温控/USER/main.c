#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"

//ALIENTEK 探索者STM32F407开发板 实验5
//外部中断实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司    
//作者：正点原子 @ALIENTEK 

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200); 	//串口初始化 
	LED_Init();				  //初始化LED端口  
	BEEP_Init();        //初始化蜂鸣器端口
	EXTIX_Init();       //初始化外部中断输入 
	LED1=0;					    //先点亮红灯
	KEY_Init();         //初始化按键
	

	u16 times=0;  
	while(1)
	{ 
			times++;
		{
		if(times%30==0)LED1=!LED1;//闪烁LED,提示系统正在运行.
			delay_ms(10);   
		} 
		printf("OK\r\n");	//打印OK提示程序运行
		delay_ms(10000);	  //每隔1s打印一次
	}

}
