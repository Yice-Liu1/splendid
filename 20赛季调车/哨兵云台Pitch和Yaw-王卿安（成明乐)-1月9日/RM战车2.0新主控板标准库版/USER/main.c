//步兵1号pitch角度287.7-249,yaw轴176-70
 
#include "main.h"

float YAW;
float BAIDONG_YAW;
int BAIDONG_YAW_flag;
float stastic_yaw_angle;
long int i;  //延时
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


//2020 新参数

//pitch 305-330
//yaw   

float pitch_wish=320;
float yaw_wish=120;


int main(void)

{   ;		

	float manifold_x_old=0;   //   172  233  232-172    120
	//float dpz=0;
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
 
	while(1)	//xiaolaji&&benji&&shaji&&
	{
	//坐标要不要筛选
	//
		IMU_getYawPitchRoll(angle) ;
		GetPitchYawGxGyGz();

		manifold_y=(((float)y_coordinate-180)/180*aq);   //360  //改
	//manifold_x=(((float)x_coordinate/640)*118+6);    //6-124  118  640 320  
		if(manifold_x<350||manifold_x>290)    //改不同
		{
			manifold_x=(((float)x_coordinate-320)/320*ssh*xsh); //试出
		}
		else
		{
		 manifold_x=(((float)x_coordinate-320)/320*ssh); 
		}		
		
	//	if((manifold_x-manifold_x_old)!=0) 
			if((manifold_x-manifold_x_old)!=0)   //问题 数组
			{
				if(x_coordinate>340||x_coordinate<300)  //改 
				{
					XC=XC+manifold_x;
				}
					if(x_coordinate<340&&x_coordinate>320)  
					{
						XC=XC+dpz;
					}
					else if(x_coordinate<320&&x_coordinate>300)  
					{
						XC=XC-dpz;
					}
/************************************************************/					
//				if(y_coordinate>200||y_coordinate<160)  
//				{
//					YC=YC+manifold_y;
//				}
				
//				if()
//				{
//					YC=YC+manifold_y;
//				}
				
			}
		manifold_x_old=manifold_x;
		
		if(XC>240) XC=240;
		if(XC<100) XC=100;
		if(YC>110) YC=110;
		if(YC<60) YC=60;
			
			if(pitch_wish>330) pitch_wish=330;
			if(pitch_wish<310) pitch_wish=310;
			
			
			
		Gimbal_Set(pitch_wish,yaw_wish,MPU6050_Real_Data.Gyro_Y,MPU6050_Real_Data.Gyro_Z,REAL_YP,REAL_YY);
		
		
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