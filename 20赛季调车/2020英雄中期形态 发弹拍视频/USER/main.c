#include "main.h"

#include "gun.h"
#include "RemoteTask.h"



int shan=0;  //标志位
int chen=0;  //标志位
int tim;
int bospeed=0;   //拨弹轮期望
int faspeed=0;
float BAIDONG_YAW;
int BAIDONG_YAW_flag;
float stastic_yaw_angle;
long int i;  //延时
u8 *s;
u8 flag_power_over=0;
float power=0;
int dianji=1000,TESTN=120,dr=0,ER=0;
int rest_power=0;
//读功率用串口2  摩擦轮用定时器5的通道1和通道2  拨弹轮定时器9通道1 遥控器接受用串口1
//yaw 中间值 305 pitch 水平 130 上限位160 下线位 115

void analysis_powerArest(u8 powerdat[30])//分析功率是否超出80W
{
//计算功率
	s=powerdat+4;
	power=*(float*)s;
	s=powerdat+8;
	rest_power=*(int*)s;
}
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

		analysis_powerArest(usart2_rcvbuf);			

		LASER();
    
		IMU_getYawPitchRoll(angle) ;

		GetPitchYawGxGyGz();

		remote();
    PWM4=120;
		//Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);
   //while(1)
	 //{


		if(RC_CtrlData.rc.s2==1)//ER>=55/右侧拨杆
		{ tim=0;
			shan=0;
			chen=0;
			PWM4=147;
			bospeed=0;
			faspeed=0;
			FRICTIONPID(bospeed,0);   //s2=1时不拨弹
			FRICTIONPID2(faspeed,faspeed);
		}
		if(RC_CtrlData.rc.s2==3)
		{		
			tim++;
        PWM4=255;			//挡板后退
			if(chen==0)
			{
			bospeed=-450;      //s2=3时拨弹
			}
			if(M5<-100){shan=1;}    //已拨弹
			if(M5==0&&shan==1&&tim>=1000)     //卡弹且已拨弹停止拨弹
			{
				chen=1;
				bospeed=0;
			}
			FRICTIONPID(bospeed,0); 
			faspeed=8000;
			FRICTIONPID2(faspeed,-faspeed);
		}			
			if(RC_CtrlData.rc.s2==2)
			{	
				tim=0;
				{ bospeed=30;
		 		HNADPIDSPEED(bospeed);
			  }
				//tim++;
        shan=0;
				chen=0;	
				faspeed=8000;
				FRICTIONPID2(faspeed,-faspeed);
				delay_ms(20);
				PWM4=55;
				if(tim>50)
				{bospeed=0;
				FRICTIONPID(bospeed,0);
					}
			}
					
	//	Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);
		
	}
}

