#include "main.h"
#include <math.h>

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))	
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))	 
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	
#define pi 3.1415926535897932

//#define  DING  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_1)  


short int sh;
short int mix;

/***********************************/
float xiao_yaw;
float xyaw_speed;

float xiao_pitch;
float xpitch_speed;

short int Pitch_End_Out=0;
short int Yaw_End_Out=0;

u8 DING=0;


//cml:无人机弹道常数
//H v1 v0 g
int H;
int v1;
int v0;
//int g;

float a=0;
float b;

float piture_pitch;

int pic_pwm=1800;





//cml:以下变量均为测试使用。
//float yaw_wish=65;
//float pitch_wish=0;



u8 data_buffer[50]={0};   //50个数据,每一位是1个字节.
unsigned char data_bag(int16_t data1,int16_t data2,int16_t data3)
{
	u8 cnt=0;
	data_buffer[cnt++]=0xAA;
	data_buffer[cnt++]=0x05;
  data_buffer[cnt++]=0xAF;
	data_buffer[cnt++]=0xF1;
	data_buffer[cnt++]=0;   

	
	data_buffer[cnt++]=BYTE1(data1);   
	data_buffer[cnt++]=BYTE0(data1);    //cnt=7
	data_buffer[cnt++]=BYTE1(data2);   
	data_buffer[cnt++]=BYTE0(data2);    //cnt=7
	data_buffer[cnt++]=BYTE1(data3);   
	data_buffer[cnt++]=BYTE0(data3);    //cnt=7
//data_buffer[cnt++]=0x00;   
//data_buffer[cnt++]=0xAA;   
//data_buffer[cnt++]=0xAA;   
//data_buffer[cnt++]=0xAA;  

  data_buffer[4]=cnt-5;	
	
	u8 sum=0;
	for(u8 i=0;i<cnt;i++)
	{
		sum+=data_buffer[i];
	}
	data_buffer[cnt++]=sum;

	return cnt;
}


// xiao_yaw 范围     
// xiao_pitch 




int main(void)
{  
	
	
/********函数初始化********/	
	BSP_Init();	
	SetFrictionWheelSpeed(800);
	delay_ms(300);
	Init_Quaternion();
/****************************/	
	
	while(1)	
	{
		
		xiao_yaw=(float)stcAngle.Angle[2]/32768*180;
		//TIM4->CCR1=mmix;//cml:1600-2050
		
////		DING=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5); 
////		IMU_getYawPitchRoll(angle); 
////		//remote();	
////		
////		Pitch_End_Out = pitch_pid(pitch_wish,angle[1],MPU6050_Real_Data.Gyro_Y);
////    Yaw_End_Out   = yaw_pid(yaw_wish,REAL_YY,MPU6050_Real_Data.Gyro_Z);
////	
////		//Set_Gimbal_Current(CAN1,-0,-Pitch_End_Out);
//////		delay_ms(1);
////		 a=(90.0-angle[1])/pi;
////		 
////	   b=-atan((H*g*sin(a))/(v0*cos(a - pi/2)*(v1*sin(a) - cos(a - pi/2)* sqrt(2*H*g + v0*v0*cos(a)*cos(a) + v1*v1 - 2*v0*v1*cos(a)) + v0*sin(a)*sin(a - pi/2))));
////		
////		 piture_pitch = (float)stcAngle.Angle[1]/32768*180;
////		
////		if(b-piture_pitch>10)   //duoji
////		{
////			pic_pwm++;
////			if(pic_pwm>=2050) pic_pwm = 2050;
////		}
////		if(b-piture_pitch<-10)
////		{
////			pic_pwm--;
////			if(pic_pwm<=1600) pic_pwm = 1600;
////		}		
////	//	TIM4->CCR1=pic_pwm;
		
	//TIM4->CCR1=1800+piture_pid(piture_pitch,mmix);
		
//		if(DING==1)
//		{
//			
//		}

//		for(a=0;a<5;a++)
//		{
//			//		USART_SendData(USART3,FF);
////		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
//		}
		
		//xiao_yaw=self_static_pid(100,100);
		//xiao_yaw = self_static_pid(125,stcAngle.Angle[0]/32768*180);

	 // remote();	
		//GimbalPID();
//		if(sh>80) sh=80;
//		if(sh<30) sh=30;
//	Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);
		
	//	BAIDONG_YAW =	imu_PID(100,pitch_angle);	
		
 //  Gimbal_Set(sh+pitch_angle,140+BAIDONG_YAW,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);
		
//		mix1=sh;
//		mix2=(int16_t)REAL_YP;
//		mix3=(int16_t)Pitch_Fina_Out;
//		nub=data_bag(mix1,mix2,mix3);
//		for(u8 i=0;i<nub;i++)
//		{
//			USART_SendData(USART2,data_buffer[i]);
//			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
//		}
//		delay_us(cxgkey);
		//GetPitchYawGxGyGz();
		

		}
}



















