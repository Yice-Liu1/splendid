//步兵1号pitch角度287.7-249,yaw轴176-70
#include "main.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))	
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))	 
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
int t;
float YAW;
float BAIDONG_YAW;
int   BAIDONG_YAW_flag;
float stastic_yaw_angle;
long int i;  //延时
float TEMP[6];
int cxgkey=0;
float err[3],chazhi;
char  data='a';
short int sh;
short int mix;
float angle_use;

float self_down=0;
float self_up=0.00018;

float p_self=-2500.0;
float i_self=40.0;
float d_self=10.0;
int16_t self_out;


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

int self_static()
{
	int stable;
	float feedback_old=0.0;
	float accumulation_i=0.0;
	float error[2];
	float pid_out;
	int16_t output;
	
//	error[0] = 145 - angle_use; //cml:先以0°作为期望
	error[0] = angle_use - 145; 
	error[1] = angle_use - feedback_old;
	
	accumulation_i += error[0]; 
	
  if(accumulation_i>1000) accumulation_i=1000;
  else if(accumulation_i<-1000) accumulation_i=-1000; //cml:参考例程得来的需要限制，还不知道为什么
	
	pid_out = p_self * error[0] + i_self * accumulation_i + d_self * error[1];
	
	feedback_old = angle_use;
	
	if(pid_out>4800) pid_out=4800;
	if(pid_out<-4800) pid_out=-4800;
	
	output = (int16_t)pid_out;
	
	
}


//cml:黑步兵pitch 轴 电机范围：104 - 164
//cml:黑步兵云台陀螺仪pitch    63 - 


int main(void)
{  
	 t=0;
	GREEN_LED_ON();
  int16_t mix1=0x00;	
	int16_t mix2=0x00;
	int16_t mix3=0x00;	
	u8 nub=0;
	u16 time=0;
	sh=275;
	mix=125;
	BSP_Init();	
	LASER(); 
	PID_Reset(&GMPPositionPID);
	PID_Reset(&GMYPositionPID);
	SetFrictionWheelSpeed(800);
	delay_ms(300);
	MPU6050_Init();
	IST8310_Init();
	Init_Quaternion();
	TIM6_Start();
	IMU_getYawPitchRoll(angle);
	delay_ms(2000);
////////////	
	while(1)	
	{
		//0.000186000005
		
//USART_SendData(USART2,9);
//	mix=USART_ReceiveData(USART2);
//		FRICTIONPID(0);
////		//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
////		remote();	
		IMU_getYawPitchRoll(angle); //cml:更新欧拉角  //yaw & pitch
//	//angle_use = angle[0];
//	self_down = self_down + self_up;
//	angle_use = angle[0] + self_down;
		Gimbal_Set(0,250,0,MPU6050_Real_Data.Gyro_Z,0,REAL_YY);//cml:控制电机
		//            期望 |            角速度      |       反馈
//	self_out=self_static();
//	Set_Gimbal_Current(CAN1,self_out,0);	
		
//		
//		//	BAIDONG_YAW =	imu_PID(100,pitch_angle);	

//		// Gimbal_Set(sh+pitch_angle,140+BAIDONG_YAW,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);

//		mix1=MPU6050_Real_Data.Gyro_Z;
//		mix2=(int16_t)Pitch_shell_out;
////		mix3=(int16_t)Pitch_Fina_Out;
//		USART_SendData(USART2,6);
//		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
//		nub=data_bag(mix1,mix2,0);
//		for(u8 i=0;i<nub;i++)
//		{
//			USART_SendData(USART2,data_buffer[i]);
//			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
//		}
//delay_us(cxgkey);
//GetPitchYawGxGyGz();


//		remote();
		
		
		
////////////////////////	remote();	
////////////////////////if(s1==1)//左上角的按键，搬到最下面
////////////////////////	{	
////////////////////////		if(BAIDONG_YAW_flag==0)
////////////////////////		{
////////////////////////		stastic_yaw_angle=yaw_angle;
////////////////////////		BAIDONG_YAW_flag=1;
////////////////////////		}
////////////////////////		BAIDONG_YAW =	imu_PID(stastic_yaw_angle,yaw_angle);	
////////////////////////		Gimbal_Set(G,140+BAIDONG_YAW,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//相对位置值pid	
////////////////////////		//Gimbal_Set(G,128+BAIDONG_YAW,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//相对位置值pid
////////////////////////	}
////////////////////////;if(s1!=1)	
////////////////////////	{
////////////////////////		Gimbal_Set(G,140,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//相对位置值pid	
////////////////////////	}

		}
}