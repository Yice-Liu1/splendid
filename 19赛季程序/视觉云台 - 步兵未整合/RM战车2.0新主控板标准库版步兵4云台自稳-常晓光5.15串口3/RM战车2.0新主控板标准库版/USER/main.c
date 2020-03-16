#include "main.h"
float TEMP[6];
int cxgkey;
float err[3],chazhi;
float manifold_y;
float	manifold_x;

float kx=12;   //cml:经过测试，对于步兵在10到15。
float kxx=1.0; //cml:小范围内，系数未测试到合适值，目前乘1.
float ky=5;    //cml:目前而言。似乎无法加至很大。


float XC=182.0;
float YC=240.0;
float SSH=0.1;
float sh=125;


//cml:以下变量仅为测试使用
float ZBsave=2;
int ssh=5;
float xsh=1;
float dpz=0;
float aq=5;



int main(void)
 {  
	float manifold_x_old=0;
	float jubu_use;
	float xn;
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
	SetFrictionWheelSpeed(1000);
 
	while(1)	//xiaolaji&&benji&&shaji&&
	{
	
		IMU_getYawPitchRoll(angle) ;

		//remote();
		
		if(1)                           //cml:某一个按键被按下时进入自瞄
		{
			SetFrictionWheelSpeed(1400);                               //cml:进来就开摩擦轮
			
			IMU_getYawPitchRoll(angle);
			
			//manifold_y=(((float)y_coordinate-180)/180*ky);           //cml:针对摄像头和炮管距离做处理
			
			jubu_use = (float)y_coordinate-270;
			
//			if(jubu_use>0) manifold_y=(((float)y_coordinate-)/90*ky);
//			else manifold_y=(((float)y_coordinate-270)/270*ky);
//			
//			xn = (float)y_coordinate-280;
//			
//			if(xn>0) manifold_x=(((float)x_coordinate-330)/310*kx*xsh);
//			else  manifold_x=(((float)x_coordinate-330)/330*kx*xsh);
			
//			if(manifold_x<350||manifold_x>290) manifold_x=(((float)x_coordinate-320)/320*kx*xsh); 
//			else  manifold_x=(((float)x_coordinate-320)/320*kx); 		  //cml:无实际作用
			manifold_y=(((float)y_coordinate-180)/180*ky);
			manifold_x=(((float)x_coordinate-320)/320*kx); 	
			
			if((manifold_x-manifold_x_old)!=0)                          //cml:如果相同坐标就不处理;记住如果效果不好可以减少坐标。 fabs(manifold_x-manifold_x_old)>=5
			{
				//if(x_coordinate>360||x_coordinate<310)  XC=XC+manifold_x;
				if(x_coordinate>330||x_coordinate<310)  XC=XC+manifold_x;
				if(y_coordinate>190||y_coordinate<170)  YC=YC-manifold_y;
				//if(y_coordinate>290||y_coordinate<250)  YC=YC-manifold_y;
			}
			
			if(x_coordinate<340&&x_coordinate>320)  FRICTIONPID(1700);
		  manifold_x_old=manifold_x;
		}
		
		GetPitchYawGxGyGz();
		
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
////////		//cml:若以此来当成坐标的转换的话
////////		//cml:找到中心坐标：470/2=235
////////	XC=XC+SSH*0.1;
////////		
////////	//	XC=XC+manifold_x;
////////	
////////		if(XC>124) XC=124;
////////		if(XC<6) XC=6;
////////		//if(XC!=124) iii++;
//////////if(iii==0)
//////////		Gimbal_Set(manifold_y,60,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//相对位置值pid	
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
		//cml；传入中心坐标的加减。
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
	
	

	
	
	
//	Gimbal_Set(manifold_y,manifold_x,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//相对位置值pid	
	/*	if(y_coordinate>=6540)
		y_coordinate=6540;
	  if(y_coordinate<=5399)
		y_coordinate=5399;*/
		
/*if(s1==1)//左上角的按键，搬到最下面
	{	
		if(BAIDONG_YAW_flag==0)
		{
		stastic_yaw_angle=yaw_angle;
		BAIDONG_YAW_flag=1;
		}
		BAIDONG_YAW =	imu_PID(stastic_yaw_angle,yaw_angle);	
		Gimbal_Set(YC,XC+BAIDONG_YAW,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//相对位置值pid	
		//Gimbal_Set(G,128+BAIDONG_YAW,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//相对位置值pid
	}
;if(s1!=1)	
	{
		Gimbal_Set(y_coordinate,x_coordinate,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_X,REAL_YP,REAL_YY);//相对位置值pid	
	}*/

		
	
	//IMU_PID();	
//	cxgkey= RC_CtrlData.key.v;
		
//USART_SendData(USART2, IMU_FDB);
		
}