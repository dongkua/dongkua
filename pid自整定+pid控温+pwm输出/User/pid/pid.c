#include "./pid/pid.h"   
#include "./led/bsp_led.h"   
#include "./GeneralTim/bsp_GeneralTim.h" 

PID pid; //���PID�㷨����Ҫ������

uint16_t pw;
uint16_t save_buff[9000] = {0}; //�������ݻ���
extern volatile	uint16_t Step_Auto;	
extern volatile	uint32_t PID_cool_cnt;
extern volatile	uint32_t PID_heat_cnt;
extern volatile	uint16_t pid_self_first_status_flag;	//��־λ
extern volatile	uint16_t PID_Deal,PID_auto_Deal;
extern volatile	uint16_t zero_across_counter;
extern volatile	uint16_t k_pid_self_counter;
extern volatile	uint16_t CCR4_Val;
extern volatile	uint16_t kkk;
void PID_Calc()  //pid����
{
	float DelEk;	
	float out;
	if (PID_Deal == 1)
	{
		pid.Ek=pid.Sv-pid.Pv;   //�õ���ǰ��ƫ��ֵ
		pid.SEk+=pid.Ek;        //��ʷƫ���ܺ�
		DelEk=pid.Ek-pid.Ek_1;  //�������ƫ��֮��

		pid.Pout=pid.Kp*pid.Ek;     //�������
		pid.Iout=pid.Ki*pid.SEk;  			//�������
		pid.Dout=pid.Kd*DelEk;    			//΢�����
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
		pid.Ek_1=pid.Ek;  //����ƫ��
//		printf ( "\r\n%.1f\r\n",out);	
		PID_out();
		
	}
}
void PID_out()  //���PID������������---ÿ1ms������1��
{
	uint16_t kk;
	if (pid.Pv>pid.Sv)//��ǰ�¶ȴ����û��趨�¶�
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
void PID_auto()//�̵���������������pid����
{

	uint8_t i = 0;
	float KC = 0;
	float TC = 0;
	float V_temp = 0,min_temp = 0,max_temp = 0;
	float TIME_Hight=0,TIME_LOW=0;
	
	
	//��һ������Ƚϳ�ʼ�¶� ȷ����ʱ�¶ȴ����������

if (PID_auto_Deal== 0)
{
/******************************************************************************************************/
		
		PID_Deal = 0;//�˳�pid
		
		//�����һ�ν��� �鿴�Աȵ�ǰ�¶Ⱥ��趨�¶�

		if(pid.Pv < pid.Sv1)//��ǰ�¶ȵ����趨�¶�
		{
			PID_heat_cnt++;		//��
			PID_cool_cnt = 0;
			if(PID_heat_cnt >= 3)//����3�ν��
			{
				CCR4_Val=0;//����
				GENERAL_TIM_Init();

				if(Step_Auto == 0)		
				{
					Step_Auto = 1;
					zero_across_counter++;
//					printf ( "\r\n1");	
				}
			}
		}
		else//��ǰ�¶� ���� �趨�¶� ֹͣ����
		{
			PID_cool_cnt++;
			PID_heat_cnt = 0;
			if(PID_cool_cnt > 3)
			{
				CCR4_Val=100;		//������
				GENERAL_TIM_Init();
				if(Step_Auto == 1)		//�趨�¶ȸ��ڵ�ǰ�¶� 
				{
					Step_Auto = 0;
					zero_across_counter++;
				}
			}
		}
		if(PID_heat_cnt >= 65535)//����3�ν��
		{
			PID_heat_cnt=65535;
		}
		if(PID_cool_cnt >= 65534)//����3�ν��
		{
			PID_cool_cnt=65534;
		}
		
		
		
		/*****************��ʼ����ǿ���񵴵����ںͷ�ֵ****************************/			

		if((zero_across_counter == 3 ) || (zero_across_counter == 4 ))
		{
			save_buff[k_pid_self_counter] = pid.Pv;
			
			k_pid_self_counter++;
			
			if(k_pid_self_counter >=9000)
			{
				k_pid_self_counter = 0;					
			}
		}
		else if(zero_across_counter == 5 )//5�ι�0 ��˵���������� �����ɹ�
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
					V_temp =  max_temp - min_temp;					//������С���Ƿ�ֵ
					
					
				KC = 127/V_temp;
//�����¼�� ����¶� �� ����¶ȶ�Ӧ��ʱ�� ��ô���������ʽ��TC = 2 * (TIME_Hight - TIME_LOW);	
				TC = k_pid_self_counter;	//TC =2 * (TIME_Hight - TIME_LOW);
			
				pid.Kp = 0.6*KC;		
				pid.Ki = (0.6*KC)/(0.5*TC)/10;
				pid.Kd = (0.6*KC)*(0.125*TC)/60;
				printf ( "\r\n�����ɹ�");		
			}
	}	
}
