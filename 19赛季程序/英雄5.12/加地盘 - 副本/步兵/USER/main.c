#include "main.h"

float BAIDONG_YAW;
int BAIDONG_YAW_flag;
float stastic_yaw_angle;
long int i;  //��ʱ
u8 *s;
u8 flag_power_over=0;
float power=0;
int t=0;
int xue=0,flag_dan=0;

//�������ô���2  Ħ�����ö�ʱ��5��ͨ��1��ͨ��2  ������CAN2��ַ7 ң���������ô���1
//yaw �м�ֵ 305 pitch ˮƽ 130 ����λ160 ����λ 115

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
int YY=285;
int GG=164;
int fadan=700,shoot=1000;
int main(void)
{   

	  BSP_Init();	
		SetFrictionWheelSpeed(800);//ռ�ձȣ�800/20000�� �����ź�
		delay_ms(3000);
		MPU6050_Init();
		IST8310_Init();
		Init_Quaternion();
		TIM6_Start();
		LASER();
	while(1)	
	{ 
		flag_dan=LIGHT;
		if(usart2_rcvfns==1)
		{
			analysis_power(usart2_rcvbuf);
			usart2_rcvfns=0;
		}
		
						if(flag_genshui)
		{
			  if(REAL_YY>Old_YY+5)
		{
			nishizhen(-2000);
		}
		else if(REAL_YY<Old_YY-5)
		{
			nishizhen(2000);
		}
		}
		
		LASER();
		
		IMU_getYawPitchRoll(angle);
		
		remote();	

		Sheetfadan(-fadan,shoot);
		
		FRICTIONPID(xue);
		
    Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);
		
		Set_current(Yaw_Fina_Out,Pitch_Fina_Out,fadanMotorSpeedPID.output,BigShootMotorSpeedPID.output);
		 
	}
}

