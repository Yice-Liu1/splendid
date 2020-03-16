//步兵1号pitch角度287.7-249,yaw轴176-70
 
#include "main.h"

float YAW;
float BAIDONG_YAW;
int BAIDONG_YAW_flag;
float stastic_yaw_angle;
long int i;  //延时
float TEMP[6];
int cxgkey;
//pitch轴角度范围是6560-5399 yaw 中间是2911,3832是最右边，6025最左边，自带陀螺仪的参数：yaw范围112.5到163.9,pitch轴-13到53
//PID_Regulator_t GMPPositionPID;
 float err[3],chazhi;
char data='a';
int main(void)
{   	

	BSP_Init();	
	LASER();  


	PID_Reset(&GMPPositionPID);
	PID_Reset(&GMYPositionPID);
	SetFrictionWheelSpeed(800);
	delay_ms(3000);

	MPU6050_Init();
	IST8310_Init();
	Init_Quaternion();
	

//while(pitch_angle<=-35||pitch_angle>=-27)

//{
//	IMU_getYawPitchRoll(angle);
//	GetPitchYawGxGyGz();
//}
//if((pitch_angle>=-35||pitch_angle<=-27)&&i<=50000)
//{
//	i++;
//	IMU_getYawPitchRoll(angle);
//	GetPitchYawGxGyGz();
//	}
		
 TIM6_Start();
	while(1)	
	{
		
  IMU_getYawPitchRoll(angle) ;
	remote();	
//	SPEED_GimbalPID();
// GimbalPID();
//YAW=	imu_PID(0,yaw_angle);
		
//Gimbal_Set(G,140,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//相对位置值pid			
//cml:    265， 	    Y轴的角速度                   X轴的角速度
		
	
if(s1==1)//左上角的按键，搬到最下面
	{	
		if(BAIDONG_YAW_flag==0)
		{
		stastic_yaw_angle=yaw_angle;
		BAIDONG_YAW_flag=1;
		}
		BAIDONG_YAW =	imu_PID(stastic_yaw_angle,yaw_angle);	
		Gimbal_Set(G,140+BAIDONG_YAW,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//相对位置值pid	
		//Gimbal_Set(G,128+BAIDONG_YAW,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//相对位置值pid
	}
;if(s1!=1)	
	{
		Gimbal_Set(G,140,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//相对位置值pid	
	}

		
	
	//IMU_PID();	
//	cxgkey= RC_CtrlData.key.v;
		
//USART_SendData(USART2, IMU_FDB);
		}
}