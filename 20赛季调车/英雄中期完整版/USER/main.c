#include "main.h"

#include "gun.h"
#include "RemoteTask.h"

float BAIDONG_YAW;
int BAIDONG_YAW_flag;
float stastic_yaw_angle;
long int i;  //��ʱ
u8 *s;
u8 flag_power_over=0;
float power=0;
int dianji=1000,TESTN=120,dr=0,ER=0;
int rest_power=0;
//�������ô���2  Ħ�����ö�ʱ��5��ͨ��1��ͨ��2  �����ֶ�ʱ��9ͨ��1 ң���������ô���1
//yaw �м�ֵ 305 pitch ˮƽ 130 ����λ160 ����λ 115

void analysis_powerArest(u8 powerdat[30])//���������Ƿ񳬳�80W
{
//���㹦��
	s=powerdat+4;
	power=*(float*)s;
	s=powerdat+8;
	rest_power=*(int*)s;
}
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
		analysis_powerArest(usart2_rcvbuf);
		
		LASER();
		IMU_getYawPitchRoll(angle) ;
		GetPitchYawGxGyGz();
		
    PWM4=120;
		remote();
		Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);
		Set_Current(CAN1,Yaw_Fina_Out,-Pitch_Fina_Out,ShootbolunPID.output,0 );
		
	}
}

