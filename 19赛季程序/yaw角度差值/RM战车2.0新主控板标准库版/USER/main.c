//步兵1号pitch角度287.7-249,yaw轴176-70
 
#include "main.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))	
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))	 
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

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

float yaw_save[100]={0};
float old_angle=0.0;

float little_count=0.0;


float self_down=0;
float self_up=0.00011;  //0.000088-0.00018

float p_self=-2500.0;
float i_self=40.0;
float d_self=10.0;
int16_t self_out;

int countByZJX=0;

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

int self_static_pid()
{
	int stable;
	float feedback_old=0.0;
	float accumulation_i=0.0;
	float error[2];
	float pid_out;
	int16_t output;
	
//	error[0] = 145 - angle_use; //cml:先以0°作为期望
	error[0] = angle_use - 130; 
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
  int16_t mix1=0x00;	
	int16_t mix2=0x00;
	int16_t mix3=0x00;	
	u8 nub=0,flagsh=1;
	u16 time=0;
	
	u8 a=0;
	u8 i=0;
	
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
	delay_ms(3000);
////////////	
	while(1)	
	{
	//0.000186000005
	//USART_SendData(USART2,9);
	//	mix=USART_ReceiveData(USART2);
	//		FRICTIONPID(0);
	////		//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	//remote();	
		
/***************************************************/		
		IMU_getYawPitchRoll(angle);    //cml:angle[0]会有漂移
		
		if(i!=100)
		{
			yaw_save[i]=angle[0]-old_angle;
			i++;
		}
		if(i==100 && flagsh)
		{
			for(a=10;a<100;a++)
			{
				little_count  += yaw_save[a];
			}
			little_count = little_count/90.0;
		//	i=11;
			flagsh=0;
			
		}
		old_angle=angle[0];
	
	/****************************************************/	
		
		
		self_down = self_down + self_up;
		angle_use = angle[0] - self_down;
		self_out=self_static_pid();
		
		
	

		}
}