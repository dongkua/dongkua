#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"

//ALIENTEK ̽����STM32F407������ ʵ��5
//�ⲿ�ж�ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾    
//���ߣ�����ԭ�� @ALIENTEK 

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200); 	//���ڳ�ʼ�� 
	LED_Init();				  //��ʼ��LED�˿�  
	BEEP_Init();        //��ʼ���������˿�
	EXTIX_Init();       //��ʼ���ⲿ�ж����� 
	LED1=0;					    //�ȵ������
	KEY_Init();         //��ʼ������
	

	u16 times=0;  
	while(1)
	{ 
			times++;
		{
		if(times%30==0)LED1=!LED1;//��˸LED,��ʾϵͳ��������.
			delay_ms(10);   
		} 
		printf("OK\r\n");	//��ӡOK��ʾ��������
		delay_ms(10000);	  //ÿ��1s��ӡһ��
	}

}
