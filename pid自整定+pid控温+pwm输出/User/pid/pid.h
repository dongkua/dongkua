#ifndef _pid_
#define _pid_
#include "stm32f10x.h"
typedef struct
{
	float Sv;//用户设定温度
	float Pv;//当前温度

	float Kp;
	float Ki;
	float Kd;

	float Ek;  //本次偏差
	float Ek_1;//上次偏差
	float SEk; //历史偏差之和

	float Iout;
	float Pout;
	float Dout;	
	float OUT0;
	float OUT;
	
	
	float Sv1;//用户设定温度
		

	u16 pwmcycle;//pwm周期

}PID;

extern PID pid; //存放PID算法所需要的数据
void PID_Calc(void); //pid计算
void PID_out(void);
void PID_auto(void);
#endif








