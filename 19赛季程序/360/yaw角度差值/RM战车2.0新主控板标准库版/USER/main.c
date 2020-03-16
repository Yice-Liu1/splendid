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
float self_up=0;  //0.000088-0.00018

float p_self=-2500.0;
float i_self=40.0;
float d_self=10.0;
int16_t self_out;

u8 cheer_flag=1;
int countByZJX=0;

float copy_angle=0.0;

float yaw_float=0.0;

float add=0.0;

//cml:

float yaw_old=0.0;
float angle_err;

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
	int j=0;
	
  int16_t mix1=0x00;	
	int16_t mix2=0x00;
	int16_t mix3=0x00;	
	u8 nub=0,flagsh=1;
	u16 time=0;
	
	
	
	u8 a=0;
	int i=0;
	
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

	 for(j=0;j<50;j++)
	{
			IMU_getYawPitchRoll(angle);
	}
	
	yaw_old = angle[0];

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
		
		if(i<30)
		{
			yaw_save[i]=angle[0];           //cml:只要后10位 10 11 12 13 14 15 16 17 18 19 
			i++;
		}
		
		angle_err = yaw_save[20] - yaw_save[19];
		
		if(angle_err<-0.001||angle_err>0.001)
		{
			angle_err=0;
		}
			
		
		self_down = self_down + angle_err;
		
		if(i==30) i=0;
		
		angle_use = angle[0] + self_down;
		
		
//		angle_err = angle[0] - yaw_old; 
//		
////		self_down = self_down + angle_err;
////		angle_use = angle[0] - self_down;
//		yaw_old=angle[0];
		
//		if(i<50)
//		{
//			yaw_save[i]=angle[0];        //cml:每一个角度值  我现在需要知道漂移的差值，准备存入10个
//			i++;
//		}
	//	yaw_float = 
		
//		if(i<30)
//		{
//			yaw_save[i]=angle[0];           //cml:只要后10位 10 11 12 13 14 15 16 17 18 19 
//			i++;
//		}
//		
//		if(i==30)
//		{
//			for(j=10;j<20;j++)
//			{
//				add = add + ( yaw_save[j+1] - yaw_save[j] ); //cml:  10 11 12  13 14 15 16 17  18 19
//			}
//						
//			 add = add/9.0;		
//			i=0;
//		}
//		
//		self_up = add;
		
//		self_down = self_down + angle_err;
//		angle_use = angle[0] - self_down;
//		yaw_old=angle[0];
		
		//if(angle_err)
	
		
//	  if(angle[0]>-175&&angle[0]<175) 
//		{
//			copy_angle = angle[0];

//			if(i!=100)
//			{
//				yaw_save[i]=angle[0];
//				i++;
//			}
//			if(i==100 && cheer_flag)
//			{
//				for(a=10;a<90;a++)
//				{
//					little_count  += yaw_save[a+1]-yaw_save[a]; // 46-45  47-44     11  10
//				}
//				little_count = little_count/81.0;
//				if( little_count<0.1 || little_count>-0.1 )
//				{
//					self_up=little_count;
//				}
//				i=0;
//			}
//		}
//		
//		if(j!=800)
//		{
//			shuzu[j]=self_down;
//			j++;
//		}
//		
//		//self_up=little_count;
//	//	self_up=little_count;
//		self_down = self_down + self_up;
//		angle_use = copy_angle - self_down;
	
		
		//old_angle=angle[0];
	
	/****************************************************/	
//		if(angle[1]>-5&&angle[1]<=5)
//			self_up=0.000142370642;
//		if(angle[1]>=-10&&angle[1]<=-5)
//			self_up=0.000138303792;
//		if(angle[1]>=-15&&angle[1]<-10)
//			self_up=0.000106499311;
//		if(angle[1]>=-20&&angle[1]<-15)
//			self_up=0.000104499311;
//		if(angle[1]>=-25&&angle[1]<-20)
//			self_up=0.000102499311;
//		if(angle[1]>=-30&&angle[1]<-25)
//			self_up=0.000102399311;
//		if(angle[1]>=-35&&angle[1]<-30)
//			self_up=0.000102099311;
//		if(angle[1]>=-40&&angle[1]<-35)
//			self_up=0.000073905311;
//			if(angle[1]>=-45&&angle[1]<-40)
//			self_up=0.000070905311;
//			if(angle[1]>=-50&&angle[1]<-45)
//			self_up=4.50000043e-005;
//			if(angle[1]>=-55&&angle[1]<-50)
//			self_up=-4.00000026e-005;
//			if(angle[1]>=-60&&angle[1]<-55)
//			self_up=3.00000011e-005;
//			if(angle[1]>=-65&&angle[1]<-60)
//			self_up=1.6e-005;
//			if(angle[1]>=-70&&angle[1]<-65)
//			self_up=4.59999865e-006;
//			if(angle[1]>=-75&&angle[1]<-70)
//			self_up=4.55999862e-006;
//			if(angle[1]>=-80&&angle[1]<-75)
//			self_up=4.55999862e-006;
//			if(angle[1]>=-85&&angle[1]<-80)
//			self_up=9.99999997e-007;
//			if(angle[1]>=5&&angle[1]<10)
//			self_up=0.000180000003;
//			if(angle[1]>=10&&angle[1]<15)
//			self_up=0.000159999996;
//		  if(angle[1]>=15&&angle[1]<20)
//			self_up=0.000180000003;
//			if(angle[1]>=20&&angle[1]<25)
//			self_up=0.000180000003;
//			if(angle[1]>=25&&angle[1]<30)
//			self_up=0.000169999999;
//			if(angle[1]>=30&&angle[1]<35)
//			self_up=0.000180000003;
//			if(angle[1]>=35&&angle[1]<40)
//			self_up=0.000184999997;
//			if(angle[1]>=40&&angle[1]<45)
//			self_up=0.000169999985;
//			if(angle[1]>=45&&angle[1]<50)
//			self_up=0.000180000003;
//			if(angle[1]>=50&&angle[1]<55)
//			self_up=0.000169999985;
//			if(angle[1]>=55&&angle[1]<60)
//			self_up=0.000170599989;
//			if(angle[1]>=60&&angle[1]<65)
//			self_up=0.000161000004;
//			if(angle[1]>=65&&angle[1]<70)
//			self_up=0.000155000002;
//			if(angle[1]>=70&&angle[1]<75)
//			self_up=0.000150000007;
//			if(angle[1]>=75&&angle[1]<80)
//			self_up=0.000118000004;
//			if(angle[1]>=80&&angle[1]<85)
////			self_up=9.6999969e-005;
//    self_up=little_count;
//		self_down = self_down + self_up;
//		angle_use = angle[0] - self_down;
//		self_out=self_static_pid();
		
		
	

		}
}