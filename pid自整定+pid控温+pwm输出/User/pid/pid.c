#include "./pid/pid.h"   
#include "./led/bsp_led.h"   
#include "./GeneralTim/bsp_GeneralTim.h" 

PID pid; //存放PID算法所需要的数据

uint16_t pw;
uint16_t save_buff[9000] = {0}; //保存数据缓存
extern volatile	uint16_t Step_Auto;	
extern volatile	uint32_t PID_cool_cnt;
extern volatile	uint32_t PID_heat_cnt;
extern volatile	uint16_t pid_self_first_status_flag;	//标志位
extern volatile	uint16_t PID_Deal,PID_auto_Deal;
extern volatile	uint16_t zero_across_counter;
extern volatile	uint16_t k_pid_self_counter;
extern volatile	uint16_t CCR4_Val;
extern volatile	uint16_t kkk;
void PID_Calc()  //pid计算
{
	float DelEk;	
	float out;
	if (PID_Deal == 1)
	{
		pid.Ek=pid.Sv-pid.Pv;   //得到当前的偏差值
		pid.SEk+=pid.Ek;        //历史偏差总和
		DelEk=pid.Ek-pid.Ek_1;  //最近两次偏差之差

		pid.Pout=pid.Kp*pid.Ek;     //比例输出
		pid.Iout=pid.Ki*pid.SEk;  			//积分输出
		pid.Dout=pid.Kd*DelEk;    			//微分输出
		out= pid.Pout+ pid.Iout+ pid.Dout;
		kkk++;
		if(out>255)
		{
			kkk=0;
			pid.SEk=0;
		}
		if(out>255)
		{
			pid.OUT=255;
		}
		else if(out<0)
		{
			pid.OUT=0; 
		}
		else 
		{
			pid.OUT=out;
		}
		pid.Ek_1=pid.Ek;  //更新偏差
//		printf ( "\r\n%.1f\r\n",out);	
		PID_out();
		
	}
}
void PID_out()  //输出PID运算结果到负载---每1ms被调用1次
{
	uint16_t kk;
	if (pid.Pv>pid.Sv)//当前温度大于用户设定温度
	{
		CCR4_Val=100;
		GENERAL_TIM_Init();
	}
		else
		{
		kk=100-pid.OUT*100/255;
		CCR4_Val=kk;
		GENERAL_TIM_Init();
//	printf ( "\r\n%3d\r\n",kk);
		}
}
void PID_auto()//继电器反馈法自整定pid参数
{

	uint8_t i = 0;
	float KC = 0;
	float TC = 0;
	float V_temp = 0,min_temp = 0,max_temp = 0;
	float TIME_Hight=0,TIME_LOW=0;
	
	
	//第一步进入比较初始温度 确定此时温度处于哪种情况

if (PID_auto_Deal== 0)
{
/******************************************************************************************************/
		
		PID_Deal = 0;//退出pid
		
		//程序第一次进入 查看对比当前温度和设定温度

		if(pid.Pv < pid.Sv1)//当前温度低于设定温度
		{
			PID_heat_cnt++;		//热
			PID_cool_cnt = 0;
			if(PID_heat_cnt >= 3)//连续3次结果
			{
				CCR4_Val=0;//加热
				GENERAL_TIM_Init();

				if(Step_Auto == 0)		
				{
					Step_Auto = 1;
					zero_across_counter++;
//					printf ( "\r\n1");	
				}
			}
		}
		else//当前温度 大于 设定温度 停止加热
		{
			PID_cool_cnt++;
			PID_heat_cnt = 0;
			if(PID_cool_cnt > 3)
			{
				CCR4_Val=100;		//不加热
				GENERAL_TIM_Init();
				if(Step_Auto == 1)		//设定温度高于当前温度 
				{
					Step_Auto = 0;
					zero_across_counter++;
				}
			}
		}
		if(PID_heat_cnt >= 65535)//连续3次结果
		{
			PID_heat_cnt=65535;
		}
		if(PID_cool_cnt >= 65534)//连续3次结果
		{
			PID_cool_cnt=65534;
		}
		
		
		
		/*****************开始计算强行振荡的周期和幅值****************************/			

		if((zero_across_counter == 3 ) || (zero_across_counter == 4 ))
		{
			save_buff[k_pid_self_counter] = pid.Pv;
			
			k_pid_self_counter++;
			
			if(k_pid_self_counter >=9000)
			{
				k_pid_self_counter = 0;					
			}
		}
		else if(zero_across_counter == 5 )//5次过0 则说明出现了振荡 整定成功
		{
				PID_Deal = 1;
				PID_auto_Deal = 1;
				zero_across_counter = 0;

					max_temp=save_buff[0];
					min_temp=save_buff[0];
					for(i = 0;i < k_pid_self_counter;i++)
					{
						if(save_buff[i] >= max_temp)
						{
							max_temp = save_buff[i];
							TIME_LOW=i;
						}
						if(save_buff[i] <= min_temp)
						{
							min_temp = save_buff[i];
							TIME_Hight=i;
						}
					}
					V_temp =  max_temp - min_temp;					//最大减最小就是幅值
					
					
				KC = 127/V_temp;
//如果记录了 最低温度 与 最高温度对应的时间 那么沿用这个公式：TC = 2 * (TIME_Hight - TIME_LOW);	
				TC = k_pid_self_counter;	//TC =2 * (TIME_Hight - TIME_LOW);
			
				pid.Kp = 0.6*KC;		
				pid.Ki = (0.6*KC)/(0.5*TC)/10;
				pid.Kd = (0.6*KC)*(0.125*TC)/60;
				printf ( "\r\n整定成功");		
			}
	}	
}
