#include "main.h"

#include "gun.h"
#include "RemoteTask.h"



int shan=0;  //��־λ
int chen=0;  //��־λ
int tim;
int bospeed=0;   //����������
int faspeed=0;
float BAIDONG_YAW;
int BAIDONG_YAW_flag;
float stastic_yaw_angle;
long int i;  //��ʱ
u8 *s;
u8 flag_power_over=0;
float power=0;
int dianji=1000,TESTN=120,dr=0,ER=0;
int rest_power=0;
//�������ô���2  Ħ�����ö�ʱ��5��ͨ��1��ͨ��2  �����ֶ�ʱ��9ͨ��1 ң���������ô���1
//yaw �м�ֵ 305 pitch ˮƽ 130 ����λ160 ����λ 115

void analysis_powerArest(u8 powerdat[30])//���������Ƿ񳬳�80W
{
//���㹦��
	s=powerdat+4;
	power=*(float*)s;
	s=powerdat+8;
	rest_power=*(int*)s;
}
int main(void)
{   
	 
	  BSP_Init();	
		SetFrictionWheelSpeed(800);//ռ�ձȣ�800/20000�� �����ź�
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


		if(RC_CtrlData.rc.s2==1)//ER>=55/�Ҳದ��
		{ tim=0;
			shan=0;
			chen=0;
			PWM4=147;
			bospeed=0;
			faspeed=0;
			FRICTIONPID(bospeed,0);   //s2=1ʱ������
			FRICTIONPID2(faspeed,faspeed);
		}
		if(RC_CtrlData.rc.s2==3)
		{		
			tim++;
        PWM4=255;			//�������
			if(chen==0)
			{
			bospeed=-450;      //s2=3ʱ����
			}
			if(M5<-100){shan=1;}    //�Ѳ���
			if(M5==0&&shan==1&&tim>=1000)     //�������Ѳ���ֹͣ����
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

