#include "main.h"

float BAIDONG_YAW;
int BAIDONG_YAW_flag;
float stastic_yaw_angle;
long int i;  //��ʱ
u8 *s;
u8 flag_power_over=0;
float power=0;
int rest_power=0;
int t=0;
int xue=0,flag_dan=0;

float xiao_yaw;      //cml:JY901������yaw�Ƕ�
float xiao_pitch;

u8 flag_m=0;

float old_yaw;

u8 flagjy=0;


//�������ô���2  Ħ�����ö�ʱ��5��ͨ��1��ͨ��2  ������CAN2��ַ7 ң���������ô���1
//yaw �м�ֵ 305 pitch ˮƽ 130 ����λ160 ����λ 115

void analysis_powerArest(u8 powerdat[30])//���������Ƿ񳬳�80W
{
//���㹦��
	s=powerdat+4;
	power=*(float*)s;
	s=powerdat+8;
	rest_power=*(int*)s;
}

int ZJX = 0;
int YY=285,Y_flag=1;
int GG=164;
int fadan=1000,shoot=700;//shoot����CAN2_7 fadanС����CAN1_7
int main(void)
{   

	  BSP_Init();	
		PID_Reset(&GMPPositionPID);
	  PID_Reset(&GMYPositionPID);
		SetFrictionWheelSpeed(800);//ռ�ձȣ�800/20000�� �����ź�
		delay_ms(3000);
		MPU6050_Init();
		IST8310_Init();
		Init_Quaternion();
		TIM6_Start();
		LASER();
	  POWER();
	while(1)	
	{ 
		flag_dan=LIGHT;
		
		xiao_yaw=(float)stcAngle.Angle[2]/32768*180+180.0;
		
//		if(Y_flag) {Y=xiao_yaw;Y_flag=0;}
//		if(Y<0)  Y+=360;//notice the order of 488-489 and 490-493
//		if(Y>=360) Y-=360;
//		if(REAL_YY-Y>180){
//				Y=Y+360;
//		}
//		else if(REAL_YY-Y<-180)	{Y=Y-360;}
//		analysis_powerArest(usart2_rcvbuf);
		
		LASER();
		
		GetPitchYawGxGyGz();
		
		IMU_getYawPitchRoll(angle);
		
		remote();	

//		Sheetfadan(-shoot,fadan);
		
//		FRICTIONPID(xue);

		Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);
		
//-    if(s1==3)
//		{			
//			Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,xiao_yaw);
//			if(flagjy==1)
//			{
//				old_yaw=REAL_YY;
//				flagjy=0;
//			}
//			if(REAL_YY-old_yaw>5) 
//			{
//				nishizhen(-1600);
//			}
//			else if(REAL_YY-old_yaw<-5) nishizhen(1600);
//			else 
//			{
//				nishizhen(0);
//			}
//			 
//			
//		}
//		else 
//		{
//			Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);
//			flagjy=1;
//		}
		
//-		Set_current(Yaw_Fina_Out,Pitch_Fina_Out,fadanMotorSpeedPID.output,BigShootMotorSpeedPID.output);
	Set_Gimbal_Current_ByZJX(CAN1,Yaw_Fina_Out,-Pitch_Fina_Out ); 
 //   	Set_Gimbal_Current_ByZJX(CAN1,Yaw_Fina_Out,0 ); 
//		if(flag_m==0)
//		{
//			if( (REAL_YY-285)>5 ) nishizhen(-1600);
//			else if( (REAL_YY-285)<-5 ) nishizhen(1600);
//			else nishizhen(0);
//		}
			
		
		
		 
	}
}

