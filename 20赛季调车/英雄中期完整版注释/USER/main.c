#include "main.h"
#include "gun.h"
#include "RemoteTask.h"

float BAIDONG_YAW;
int BAIDONG_YAW_flag;         
float stastic_yaw_angle;         //2020δʹ��
long int i;                      //��ʱ  
u8 *s;
float power=0;                   //ʵʱ���ʲ���ϵͳ����
int rest_power=0;                //��������ʣ��
int dianji=1000,TESTN=120,dr=0,ER=0;
//�������ô���6  д�ڴ���2�ļ���  ң���������ô���1
//yaw �м�ֵ 305 pitch ˮƽ 130 ����λ160 ����λ 115

void analysis_powerArest(u8 powerdat[30])//�õ���������
{
	s=powerdat+4;
	power=*(float*)s;
	s=powerdat+8;
	rest_power=*(int*)s;
}

int main(void)    
{   
	 
	  BSP_Init();	
		SetFrictionWheelSpeed(800);//ռ�ձȣ�800/20000�� �����ź�     //2020δʹ��
		delay_ms(3000);
		MPU6050_Init();
		IST8310_Init();
		Init_Quaternion();
	  C_ON();
	while(1)	
	{ 
		analysis_powerArest(usart2_rcvbuf);    
		LASER();
		IMU_getYawPitchRoll(angle) ;
		GetPitchYawGxGyGz();
    PWM4=120;         //�Ƶ������λ
		remote();
		Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);
		Set_Current(CAN1,Yaw_Fina_Out,-Pitch_Fina_Out,ShootbolunPID.output,0 );
		
	}
}

