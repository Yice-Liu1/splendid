#include "main.h"

float BAIDONG_YAW;
int BAIDONG_YAW_flag;
float stastic_yaw_angle;
long int i;  //��ʱ
u8 *s;
u8 flag_power_over=0;
float power=0;
int dianji=1000;
int rest_power=0;

float xiao_yaw;

float manifold_y;
float	manifold_x;

float kx=12;   //cml:�������ԣ����ڲ�����10��15��
float kxx=1.0; //cml:С��Χ�ڣ�ϵ��δ���Ե�����ֵ��Ŀǰ��1.
float ky=5;    //cml:Ŀǰ���ԡ��ƺ��޷������ܴ�




float XC=182.0;
float YC=240.0;


u8 shift_flag=0;

int speed=1000;

//�������ô���2  Ħ�����ö�ʱ��5��ͨ��1��ͨ��2  �����ֶ�ʱ��9ͨ��1 ң���������ô���1
//yaw �м�ֵ 305 pitch ˮƽ 130 ����λ160 ����λ 115

//void analysis_powerA_rest_power(u8 powerdat[4],u8 rest_powerdat[2])//���������Ƿ񳬳�80W
//{
////���㹦��
//	s=powerdat;
//	power=*(float*)s;
//  s=rest_powerdat;
//	rest_power=*(int*)s;
//	if(power>80.0) //������
//	{
//		flag_power_over=1;
//	}
//	else flag_power_over=0;//δ��
//}

void analysis_power(u8 powerdat[30])//���������Ƿ񳬳�80W
{
//���㹦��
	s=powerdat+4;
	power=*(float*)s;
	if(power>30.0) //������
	{
		flag_power_over=1;
	}
	else flag_power_over=0;//δ��
}

int main(void)
{   
		float manifold_x_old=0;
	  BSP_Init();	
		SetFrictionWheelSpeed(800);//ռ�ձȣ�800/20000�� �����ź�
		delay_ms(3000);
		MPU6050_Init();
		IST8310_Init();
		Init_Quaternion();
	//	TIM6_Start();
	  C_ON();
	  POWER();
	while(1)	
	{ 
remote();
			
        if(x_coordinate!=235)
				{
     			if(x_coordinate<340&&x_coordinate>320)  FRICTIONPID(1700);
				}
	
		
		
	}
}

