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

float xiao_yaw,Old_Y;      //cml:JY901������yaw�Ƕ�
float xiao_pitch;


float angle_err;
float yaw_save[100]={0};
float self_down=0;
float self_up=0;  
float yaw_old=0.0;
float angle_use;

u8 flag1=1,flag2=1;

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
int YY=285,Y_flag=1;
int GG=164;
int fadan=1000,shoot=700;//shoot����CAN2_7 fadanС����CAN1_7
int main(void)
{   

	int j=0;
	
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
	
	for(j=0;j<50;j++)
	{
			IMU_getYawPitchRoll(angle);
	}
	
	yaw_old = angle[0];
	
	while(1)	
	{ 
		
			IMU_getYawPitchRoll(angle);    //cml:angle[0]����Ư��
		
		if(i<30)
		{
			yaw_save[i]=angle[0];           //cml:ֻҪ��10λ 10 11 12 13 14 15 16 17 18 19 
			i++;
		}
		
		angle_err = yaw_save[20] - yaw_save[19];
		
		if(angle_err<-0.0001||angle_err>0.0001)
		{
			angle_err=0;
		}
			
		
		self_down = self_down + angle_err;
		
		if(i==30) i=0;
		
		angle_use = angle[0] - self_down;
		
		if(angle_use>0) angle_use = angle_use - 180;
		else angle_use = angle_use - 180;
		
		angle_use = -angle_use*2;
		
		
		flag_dan=LIGHT;
		
		xiao_yaw=(float)stcAngle.Angle[2]/32768*180+180.0;
//		angle_use = angle_use ;
		
		//if(Y_flag) {Y=angle_use;Y_flag=0;}
		
		analysis_powerArest(usart2_rcvbuf);
		
		LASER();
		
		GetPitchYawGxGyGz();
		
		remote();	
		
		IMU_getYawPitchRoll(angle);

	//	Sheetfadan(-shoot,fadan);
		
	//	FRICTIONPID(xue);
		
		 if(s1==3) 
		 {
			 if(l!=0)
			 {
				 if(flag2==1)
				 {
					 Y=angle_use;
					 flag2=0;Old_Y=REAL_YY;
				 }
				Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,angle_use);
				 if(REAL_YY>Old_Y+5) nishizhen(-1600);
         else if(REAL_YY<Old_Y-5) nishizhen(1600);
					else nishizhen(0);
				 flag1=1;
				// Y=REAL_YY;
			 }
			 else
			 {
				 flag2=1;
				 if(flag1==1)
				 {
					 Y=REAL_YY;
					 flag1=0;
				 }
				 Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);
			 }				 
		 }
		else Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);
		
		
//    if(s1==3) Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,angle_use);
//		else Gimbal_Set(G,Y,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);
		
		Set_current(Yaw_Fina_Out,Pitch_Fina_Out,fadanMotorSpeedPID.output,BigShootMotorSpeedPID.output);
		 
	}
}

