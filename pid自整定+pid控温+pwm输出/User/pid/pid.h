#ifndef _pid_
#define _pid_
#include "stm32f10x.h"
typedef struct
{
	float Sv;//�û��趨�¶�
	float Pv;//��ǰ�¶�

	float Kp;
	float Ki;
	float Kd;

	float Ek;  //����ƫ��
	float Ek_1;//�ϴ�ƫ��
	float SEk; //��ʷƫ��֮��

	float Iout;
	float Pout;
	float Dout;	
	float OUT0;
	float OUT;
	
	
	float Sv1;//�û��趨�¶�
		

	u16 pwmcycle;//pwm����

}PID;

extern PID pid; //���PID�㷨����Ҫ������
void PID_Calc(void); //pid����
void PID_out(void);
void PID_auto(void);
#endif








