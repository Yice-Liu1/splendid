//����1��pitch�Ƕ�287.7-249,yaw��176-70
 
#include "main.h"

float YAW;
float BAIDONG_YAW;
int BAIDONG_YAW_flag;
float stastic_yaw_angle;
long int i;  //��ʱ
float TEMP[6];
int cxgkey;
float err[3],chazhi;
float manifold_y;
float	manifold_x;

float XC=175.0;
float YC=80.0;
float SSH=0.1;
float sh=125;

float ZBsave=2;
int ssh=30;
float xsh=1.5;
float dpz=0.1;
float aq=20;

int iii=3;
//int c[600]={0};
u8 exam=1;


/**********2020 �²���**********/

//pitch 305-330
//yaw   85-155

float pitch_wish=320;   //pitch�����������£�330�ӽ�ˮƽ ���鿪ʼ������305-330
float yaw_wish=120;     //yaw�����������ң�  ����120     ���鿪ʼ������85 -155
unsigned short x_coordinate_last;
unsigned short x_coordinate_use;
//����Ϊ����ϵ��
float kx=5000;
float kx2=300000;
float ky=3000;
/**********2020 �²���***********/
int main(void)

{
	float manifold_x_old=0;   //   172  233  232-172    120
	int common_i=0;
	BSP_Init();	
	TIM4_Config();
	LASER();  


	PID_Reset(&GMPPositionPID);
	PID_Reset(&GMYPositionPID);
	SetFrictionWheelSpeed(800);
	delay_ms(300);

	MPU6050_Init();
	IST8310_Init();
	Init_Quaternion();
	

		
 TIM6_Start();   
 
	while(1)
	{
		IMU_getYawPitchRoll(angle) ;//��Ԫ��ת����ŷ���ǣ��������Ǻ�������õ�
		GetPitchYawGxGyGz();//yaw�������
    //ͨ�����Ӿ���Ļ���ģ�X��320��0-640��Y��180��0-360������ֵ����
		manifold_y=(float)y_coordinate-180;//Y���ֵ���㣬y_coordinateΪ�Ӿ���ȡ���䣬����2   Y��-pitch   ��MANIFOLD_DATA_PROCESS��������
		manifold_x=(float)x_coordinate-320;//X���ֵ����                                      X��-yaw
		if(y_coordinate<170||y_coordinate>=190)//������������Ļ��������
		{
			pitch_wish=pitch_wish-manifold_y/ky;//һ�κ����㷨   ��������λ������
		}
		
		/*X���㷨����*****/
		if(x_coordinate<270||x_coordinate>370)    //300-340
			{
				//if((x_coordinate_use-x_coordinate_last)!=0)//����֮ǰʹ��ǰ��ֵ�Ƚ�    ���Ӿ������ظ�ֵ�϶ֻ࣬�ò�ֵͬ����Ҫ�Ĳ���������
				
				 //yaw_wish=yaw_wish-manifold_x/kx;   //yaw��һ�κ����㷨
       
				
				
				if(manifold_x>=0)    //yaw����κ����㷨 ��������ֵ
				{
					if(manifold_x*manifold_x>40000)
						manifold_x=190;
					else
					yaw_wish=yaw_wish-manifold_x*manifold_x/kx2;
				}
				if(manifold_x<0)
				{
					if(manifold_x*manifold_x>40000)
						manifold_x=-190;
					else
				yaw_wish=yaw_wish+manifold_x*manifold_x/kx2;
				}
	      
			}
	/***********/
		  //x_coordinate_last=x_coordinate_use;						
	  
		
		
	/*	if(x_coordinate<280||x_coordinate>360)    //300-340   //yaw��һ�κ����㷨
		{
			//yaw_wish=yaw_wish-manifold_x/ky;
			if(manifold_x>=0)
			yaw_wish=yaw_wish-manifold_x*manifold_x/ky;
			if(manifold_x<0)
			yaw_wish=yaw_wish+manifold_x*manifold_x/ky;

		}
		if(x_coordinate<300&&x_coordinate>=280||x_coordinate>340&&x_coordinate<=360)
		{
			yaw_wish=yaw_wish-manifold_x/kx2;
		}
		*/
			
			if(pitch_wish>330) pitch_wish=330;//yaw pitch��������
			if(pitch_wish<280) pitch_wish=280;
			if(yaw_wish>177) yaw_wish=177;
			if(yaw_wish<85) yaw_wish=85;
			Gimbal_Set(pitch_wish,yaw_wish,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);//������ת��
		
		
	}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	//	if(ZBsave) c[iii]=470-x_coordinate;
	//	SSH=manifold_x-65;
		
//		if(manifold_x>124) manifold_x=124;
//		if(manifold_x<6) manifold_x=6;
//		if(manifold_x<6&&manifold_x>-6) manifold_x=0;
//		if(manifold_x>59) manifold_x=59;
//		if(manifold_x<=-59) manifold_x=-59;
		
	//	if((manifold_x-manifold_x_old)==0) manifold_x=0;
	
////////	if((manifold_x-manifold_x_old)!=0) 
////////	{
////////		//manifold_x=0;
////////		SSH=manifold_x-65;
////////		if(ZBsave)
////////		{
////////			c[iii]=XC;
////////			iii++;
////////		}
////////	}
////////	else 	SSH=0;
////////	manifold_x_old=manifold_x;
////////		
////////		//cml:���Դ������������ת���Ļ�
////////		//cml:�ҵ��������꣺470/2=235
////////	XC=XC+SSH*0.1;
////////		
////////	//	XC=XC+manifold_x;
////////	
////////		if(XC>124) XC=124;
////////		if(XC<6) XC=6;
////////		//if(XC!=124) iii++;
//////////if(iii==0)
//////////		Gimbal_Set(manifold_y,60,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//���λ��ֵpid	
//////////else if(iii==1)
//////////		Gimbal_Set(manifold_y,240,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);	
//////////else if(iii==2)
	//	Gimbal_Set(manifold_y,XC,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);
//else if(iii=3)
//		Gimbal_Set(manifold_y,240,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);
//else 
//		Gimbal_Set(manifold_y,60,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);

//		if(iii>599) ZBsave=0;
//delay_ms(500);
//delay_ms(3);
		//cml��������������ļӼ���
//	
//		GREEN_LED_OFF();
//////////		
////////		if(x_coordinate>240)   
////////		{
//////////			if(x_coordinate>100) XC+=100;
//////////			else if(x_coordinate>50) XC+=50;
//////////			else if(x_coordinate>20) XC+=20;
//////////			else XC+=5;
////////			XC++;
////////		}
////////		if(x_coordinate<230)
////////		{
//////////			if(x_coordinate<100) XC-=100;
//////////			else if(x_coordinate<50) XC-=50;
//////////			else if(x_coordinate<20) XC-=20;
//////////			else XC-=5;
////////			XC--;
////////		}
////////		if(y_coordinate>130)
////////		{
//////////			if(y_coordinate>100) YC+=100;
//////////			else if(y_coordinate>50) YC+=50;
//////////			else if(y_coordinate>20) YC+=20;
//////////			else YC+=5;
////////			YC++;
////////		}
////////		if(y_coordinate<120)
////////		{
//////////			if(y_coordinate<100) YC-=100;
//////////			else if(y_coordinate<50) YC-=50;
//////////			else if(y_coordinate<20) YC-=20;
//////////			else YC-=5;
////////			YC--;
////////		}
////////		if(YC>350||YC<5) YC=8000;
////////		if(XC>350||XC<5) XC=8000;

	//	manifold_x=(((float)x_coordinate/470)*118+6);
	
	

	
	
	
//	Gimbal_Set(manifold_y,manifold_x,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//���λ��ֵpid	
	/*	if(y_coordinate>=6540)
		y_coordinate=6540;
	  if(y_coordinate<=5399)
		y_coordinate=5399;*/
		
/*if(s1==1)//���Ͻǵİ������ᵽ������
	{	
		if(BAIDONG_YAW_flag==0)
		{
		stastic_yaw_angle=yaw_angle;
		BAIDONG_YAW_flag=1;
		}
		BAIDONG_YAW =	imu_PID(stastic_yaw_angle,yaw_angle);	
		Gimbal_Set(YC,XC+BAIDONG_YAW,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//���λ��ֵpid	
		//Gimbal_Set(G,128+BAIDONG_YAW,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//���λ��ֵpid
	}
;if(s1!=1)	
	{
		Gimbal_Set(y_coordinate,x_coordinate,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//���λ��ֵpid	
	}*/

		
	
	//IMU_PID();	
//	cxgkey= RC_CtrlData.key.v;
		
//USART_SendData(USART2, IMU_FDB);
		
}