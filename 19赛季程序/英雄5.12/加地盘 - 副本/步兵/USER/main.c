#include "main.h"

float BAIDONG_YAW;
int BAIDONG_YAW_flag;
float stastic_yaw_angle;
long int i;  //延时
u8 *s;
u8 flag_power_over=0;
float power=0;
int t=0;
int xue=0,flag_dan=0;

//读功率用串口2  摩擦轮用定时器5的通道1和通道2  拨弹轮CAN2地址7 遥控器接受用串口1
//yaw 中间值 305 pitch 水平 130 上限位160 下线位 115

void analysis_power(u8 powerdat[30])//分析功率是否超出80W
{
//计算功率
	s=powerdat+4;
	power=*(float*)s;
	if(power>30.0) //超功率
	{
		flag_power_over=1;
	}
	else flag_power_over=0;//未超
}
int YY=285;
int GG=164;
int fadan=700,shoot=1000;
int main(void)
{   

	  BSP_Init();	
		SetFrictionWheelSpeed(800);//占空比（800/20000） 启动信号
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

