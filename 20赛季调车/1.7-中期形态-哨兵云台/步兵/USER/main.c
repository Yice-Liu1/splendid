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
//�������ô���2  Ħ�����ö�ʱ��5��ͨ��1��ͨ��2  �����ֶ�ʱ��9ͨ��1 ң���������ô���1
//yaw �м�ֵ 305 pitch ˮƽ 130 ����λ160 ����λ 115

void analysis_powerA_rest_power(u8 powerdat[4],u8 rest_powerdat[2])//���������Ƿ񳬳�80W
{
//���㹦��
	s=powerdat;
	power=*(float*)s;
  s=rest_powerdat;
	rest_power=*(int*)s;
	if(power>80.0) //������
	{
		flag_power_over=1;
	}
	else flag_power_over=0;//δ��
}
int ZJX1=2000;
int Pitch_test = 335,Yaw_test = 0;
int main(void)
{   
	  BSP_Init();	
		SetFrictionWheelSpeed(800);//ռ�ձȣ�800/20000�� �����ź�
		delay_ms(3000);
		MPU6050_Init();
		IST8310_Init();
		Init_Quaternion();
		TIM6_Start();
	  C_ON();
	  POWER();
	while(1)	
	{ 
		analysis_powerA_rest_power(usart3_power,usart3_rest);		

		LASER();

		IMU_getYawPitchRoll(angle) ;
		GetPitchYawGxGyGz();
//			FRICTIONPID(ZJX1);//1000�ڱ�
//		remote();		

		Gimbal_Set(Pitch_test,Yaw_test,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);
	}
}

