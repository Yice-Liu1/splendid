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

float xiao_yaw;

float manifold_y;
float	manifold_x;

float kx=12;   //cml:经过测试，对于步兵在10到15。
float kxx=1.0; //cml:小范围内，系数未测试到合适值，目前乘1.
float ky=5;    //cml:目前而言。似乎无法加至很大。




float XC=182.0;
float YC=240.0;


u8 shift_flag=0;



//读功率用串口2  摩擦轮用定时器5的通道1和通道2  拨弹轮定时器9通道1 遥控器接受用串口1
//yaw 中间值 305 pitch 水平 130 上限位160 下线位 115

//void analysis_powerA_rest_power(u8 powerdat[4],u8 rest_powerdat[2])//分析功率是否超出80W
//{
////计算功率
//	s=powerdat;
//	power=*(float*)s;
//  s=rest_powerdat;
//	rest_power=*(int*)s;
//	if(power>80.0) //超功率
//	{
//		flag_power_over=1;
//	}
//	else flag_power_over=0;//未超
//}

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

int main(void)
{   
		float manifold_x_old=0;
	  BSP_Init();	
		SetFrictionWheelSpeed(800);//占空比（800/20000） 启动信号
		delay_ms(3000);
		MPU6050_Init();
		IST8310_Init();
		Init_Quaternion();
	//	TIM6_Start();
	  C_ON();
	  POWER();
	while(1)	
	{ 
//		analysis_powerA_rest_power(usart3_power,usart3_rest);		
		
		//xiao_yaw = (float)stcAngle.Angle[2]/32768*180;
		
		//analysis_power(usart2_rcvbuf);
		
		LASER();
		
		IMU_getYawPitchRoll(angle) ;
		
		GetPitchYawGxGyGz();
		
		remote();			
		
	  //Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);	
		
//		if(s1==1)                           //cml:某一个按键被按下时进入自瞄
//		{
//			
//			shift_flag=1;
//			
//			SetFrictionWheelSpeed(1400);                               //cml:进来就开摩擦轮
//			
//			IMU_getYawPitchRoll(angle);
//			
//			//manifold_y=(((float)y_coordinate-180)/180*ky);           //cml:针对摄像头和炮管距离做处理
//			manifold_y=(((float)y_coordinate-180)/180*ky);
//			manifold_x=(((float)x_coordinate-320)/320*kx); 	
//			
//			if((manifold_x-manifold_x_old)!=0)                          //cml:如果相同坐标就不处理;记住如果效果不好可以减少坐标。 fabs(manifold_x-manifold_x_old)>=5
//			{
//				//if(x_coordinate>360||x_coordinate<310)  XC=XC+manifold_x;
//				if(x_coordinate>330||x_coordinate<310)  XC=XC+manifold_x;
//				if(y_coordinate>190||y_coordinate<170)  YC=YC-manifold_y;
//				//if(y_coordinate>290||y_coordinate<250)  YC=YC-manifold_y;
//			}
//			
        if(x_coordinate!=235)
				{
     			if(x_coordinate<340&&x_coordinate>320)  FRICTIONPID(1700);
				}
//			else FRICTIONPID(0);
//			
//		  manifold_x_old=manifold_x;
//			
//			G=XC;
//			Y=YC;
//		}
//		else 		
//		{
//		 shift_flag=0;
//			XC=G;
//			YC=Y;
//			shift_flag=0;
//		//	Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);	
//		}
			
		
		
	}
}

