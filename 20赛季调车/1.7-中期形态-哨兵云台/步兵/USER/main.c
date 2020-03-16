#include "main.h"

float BAIDONG_YAW;
int BAIDONG_YAW_flag;
float stastic_yaw_angle;
long int i;  //延时
u8 *s;
u8 flag_power_over=0;
float power=0;
int dianji=1000;
int rest_power=0;
//读功率用串口2  摩擦轮用定时器5的通道1和通道2  拨弹轮定时器9通道1 遥控器接受用串口1
//yaw 中间值 305 pitch 水平 130 上限位160 下线位 115

void analysis_powerA_rest_power(u8 powerdat[4],u8 rest_powerdat[2])//分析功率是否超出80W
{
//计算功率
	s=powerdat;
	power=*(float*)s;
  s=rest_powerdat;
	rest_power=*(int*)s;
	if(power>80.0) //超功率
	{
		flag_power_over=1;
	}
	else flag_power_over=0;//未超
}
int ZJX1=2000;
int Pitch_test = 335,Yaw_test = 0;
int main(void)
{   
	  BSP_Init();	
		SetFrictionWheelSpeed(800);//占空比（800/20000） 启动信号
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
//			FRICTIONPID(ZJX1);//1000哨兵
//		remote();		

		Gimbal_Set(Pitch_test,Yaw_test,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);
	}
}

