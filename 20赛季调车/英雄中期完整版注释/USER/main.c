#include "main.h"
#include "gun.h"
#include "RemoteTask.h"

float BAIDONG_YAW;
int BAIDONG_YAW_flag;         
float stastic_yaw_angle;         //2020未使用
long int i;                      //延时  
u8 *s;
float power=0;                   //实时功率裁判系统数据
int rest_power=0;                //缓存能量剩余
int dianji=1000,TESTN=120,dr=0,ER=0;
//读功率用串口6  写在串口2文件中  遥控器接受用串口1
//yaw 中间值 305 pitch 水平 130 上限位160 下线位 115

void analysis_powerArest(u8 powerdat[30])//得到功率数据
{
	s=powerdat+4;
	power=*(float*)s;
	s=powerdat+8;
	rest_power=*(int*)s;
}

int main(void)    
{   
	 
	  BSP_Init();	
		SetFrictionWheelSpeed(800);//占空比（800/20000） 启动信号     //2020未使用
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
    PWM4=120;         //推弹舵机复位
		remote();
		Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);
		Set_Current(CAN1,Yaw_Fina_Out,-Pitch_Fina_Out,ShootbolunPID.output,0 );
		
	}
}

