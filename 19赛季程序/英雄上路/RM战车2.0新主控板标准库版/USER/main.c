#include "main.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))	
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))	 
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

float YAW;
float BAIDONG_YAW;
int   BAIDONG_YAW_flag;
float stastic_yaw_angle;
int cxgkey=0;
float err[3],chazhi;
char  data='a';
short int sh;
short int mix;

int ceshi=190;

short look;

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





//pitch:中心  171  190  220
//yaw : 中心  300  280  260


int main(void)
{  
//  int16_t mix1=0x00;	
//	int16_t mix2=0x00;
//	int16_t mix3=0x00;	
//	u8 nub=0;
	sh=190;
	mix=150;
	BSP_Init();	
	pwm_prepartion();
	delay_ms(300);
	Init_Quaternion();
	

	
	while(1)	
	{
//				sprintf(str,"Angle:%.3f %.3f %.3f\r\n",
//		(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
	//	look=(float)stcAngle.Angle[2]/32768*180;
		
//		USART_SendData(USART6,9);
//		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);
//	  Big_Small_Bang(500,500);
//		IMU_getYawPitchRoll(angle);
		 //remote();
		//
		//	GimbalPID();
		//	Pitch_Fina_Out=0;
		//	MPU6050_Real_Data.Gyro_X = mygetqval[3];	
		//	sh=REAL_YP;
		//		//USART_SendData(USART2,data_buffer[i]);
		GetPitchYawGxGyGz();
		Gimbal_Set(ceshi,280,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);
		 delay_ms(1);

		//Gimbal_Set(sh,mix,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,sh,REAL_YY);
		//		
		//	BAIDONG_YAW =	imu_PID(100,pitch_angle);	

		// Gimbal_Set(sh+pitch_angle,140+BAIDONG_YAW,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);
				
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