#include "LBpid.h"
//#include "mpu6050.h"
//#include "can2.h"
#include "main.h"

 int16_t	Pitch_Fina_Out;
 int16_t	Yaw_Fina_Out;
 
 void usart1_send_char(u8 c)
           {   	
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); 
	USART_SendData(USART3,c);  
           } 



          void usart1_niming_report(u8 fun,u8*data,u8 len)
          {
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	
	send_buf[len+4]=0;	
	send_buf[0]=0XAA;	
                send_buf[1]=0XAA;	
	send_buf[2]=fun;	
	send_buf[3]=len;	
	for(i=0;i<len;i++)send_buf[4+i]=data[i];	
	for(i=0;i<len+4;i++)send_buf[len+4]+=send_buf[i];
	for(i=0;i<len+5;i++)usart1_send_char(send_buf[i]);
            }


     
          void t_send_data(int t)
           {
	u8 tbuf[2];
	
	tbuf[0]=(t>>8)&0XFF;
	tbuf[1]=(t)&0XFF;
	usart1_niming_report(0XF1,tbuf,2);
	
           }


/////////////////////Pitch-PID//////////////////////////////
float Pitch_i=0.0;                       //积分项
float Pitch_old=0.0;                		 //角度保存
float Pitch_d;         							 //微分项
float Pitch_shell_out;							 //外环总输出
//外环PID参数
//float Pitch_shell_kp=120;//      4hao:90 0.2 50
//float Pitch_shell_ki=0.2;//
//float Pitch_shell_kd=300;//
//float Speed_Pitch;		//速度保留
//float pitch_core_kp_out,pitch_core_kd_out;	//内环单项输出
////内环PID参数
//float Pitch_core_kp=3;       //4hao 15 5     120
//float Pitch_core_kd=100;                       1500
//float Pitch_shell_kp=130;//      4hao:90 0.2 50      150
//float Pitch_shell_ki=0.2;//                           0.005
//float Pitch_shell_kd=150;//                                180
//float Speed_Pitch;		//速度保留
//float pitch_core_kp_out,pitch_core_kd_out;	//内环单项输出
////内环PID参数
//float Pitch_core_kp=12;       //4hao 15 5
//float Pitch_core_kd=180;
float Pitch_shell_kp=0;//      4hao:90 0.2 50  60
float Pitch_shell_ki=0.008;//200 0.001 50 20 50 0.008
float Pitch_shell_kd=300;//20 0,01 0 250 0      180 
float Speed_Pitch;		//速度保留
float pitch_core_kp_out,pitch_core_kd_out;	//内环单项输出
//内环PID参数
float Pitch_core_kp=-7;       //4hao 15 5     45
float Pitch_core_kd=50;                      // 60

float Speed_Pitch_Old=0.0;
float Pitch_core_out;//内环总输出

int Pitch_PID(float RC_Pitch,float GYRO_Pitch,float Encoder_Angle)
{
	int16_t out;
////////////////////////外环角度环(PID)///////////////////////////////
  Pitch_i+=(Encoder_Angle-RC_Pitch);
//-------------Pitch积分限幅----------------//
  if(Pitch_i>1000) Pitch_i=1000;
  else if(Pitch_i<-1000) Pitch_i=-1000;
//-------------Pitch微分--------------------//
  Pitch_d=Encoder_Angle-Pitch_old;
//-------------Pitch  PID-------------------//
  Pitch_shell_out = -Pitch_shell_kp*(Encoder_Angle-RC_Pitch) + Pitch_shell_ki*Pitch_i + Pitch_shell_kd*Pitch_d;
//角度保存
  Pitch_old=Encoder_Angle;

////////////////////////内环角速度环(PD)///////////////////////////////       
  pitch_core_kp_out = Pitch_core_kp * (-Pitch_shell_out +  GYRO_Pitch * 4.5f);
  pitch_core_kd_out = Pitch_core_kd * (GYRO_Pitch - Speed_Pitch_Old);
	
	Pitch_core_out = (pitch_core_kp_out + pitch_core_kd_out);
	Speed_Pitch_Old = GYRO_Pitch;
	
//--------------------输出到电机--------------------------------//
	if(Pitch_core_out>=30000)		Pitch_core_out=30000;	
	if(Pitch_core_out<=-30000)	Pitch_core_out=-30000;
	out = (int16_t)Pitch_core_out;

	
	return out;
}


/////////////////////Yaw-PID//////////////////////////////
float Yaw_i=0.0;                       //积分项
float Yaw_old=0.0;                		 //角度保存
float Yaw_d=0.0;         							 //微分项
float Yaw_shell_out=0.0;							 //外环总输出
//外环PID参数
float Yaw_shell_kp=20;//1800 0.8 12000 50 350 55
float Yaw_shell_ki=0;//100 0.2 500  0.008
float Yaw_shell_kd=0;//180
float Speed_Yaw;		//速度保留
float Yaw_core_kp_out,Yaw_core_kd_out;	//内环单项输出
//内环PID参数
float Yaw_core_kp=-30;  //30
float Yaw_core_kd=0;  //100

float Speed_Yaw_Old=0.0;
float Yaw_core_out;//内环总输出

int Yaw_PID(float RC_Yaw,float GYRO_Yaw,float Encoder_Angle)
{
	int16_t out;
////////////////////////外环角度环(PID)///////////////////////////////
  Yaw_i+=(Encoder_Angle-RC_Yaw);
//-------------Yaw积分限幅----------------//
  if(Yaw_i>1000) Yaw_i=1000;
  else if(Yaw_i<-1000) Yaw_i=-1000;
//-------------Yaw微分--------------------//
  Yaw_d=Encoder_Angle-Yaw_old;
//-------------Yaw  PID-------------------//
  Yaw_shell_out = Yaw_shell_kp*(Encoder_Angle-RC_Yaw) + Yaw_shell_ki*Yaw_i + Yaw_shell_kd*Yaw_d;
//角度保存
  Yaw_old=Encoder_Angle;

////////////////////////内环角速度环(PD)///////////////////////////////       
  Yaw_core_kp_out = Yaw_core_kp * (Yaw_shell_out +  GYRO_Yaw * 3.5f);
  Yaw_core_kd_out = Yaw_core_kd * (GYRO_Yaw - Speed_Yaw_Old);
	
	Yaw_core_out = (Yaw_core_kp_out + Yaw_core_kd_out);
	Speed_Yaw_Old = GYRO_Yaw;
	
//--------------------输出到电机--------------------------------//
	//out = (int16_t)Yaw_core_out;
		if(Yaw_core_out>=28000)		Yaw_core_out=28000;	
	if(Yaw_core_out<=-28000)	Yaw_core_out=-28000;
	
	out = (int16_t)Yaw_core_out;

	
	return out;
}


void Gimbal_Set(float RC_Pitch,float RC_Yaw,float GYRO_Pitch,float GYRO_Yaw,float Pitch_Encoder_Angle,float Yaw_Encoder_Angle)
{
	Pitch_Fina_Out = Pitch_PID(RC_Pitch,GYRO_Pitch,Pitch_Encoder_Angle);
	Yaw_Fina_Out   = Yaw_PID(RC_Yaw , GYRO_Yaw , Yaw_Encoder_Angle);
	Set_Gimbal_Current1(CAN1,0,Yaw_Fina_Out);
	//t_send_data(Yaw_Fina_Out);
	delay_ms(1);
	//t_send_data(Pitch_Fina_Out);
	Set_Gimbal_Current2(CAN1,0,Pitch_Fina_Out);
	delay_ms(1);
	
//	CAN_SetGimbalMsg();
}
/////////////////////imu-PID//////////////////////////////
float Yaw_imu_i=0.0;                       //积分项
float Yaw_imu_d=0.0;         							 //微分项
float Yaw_imu_old=0.0;                		 //角度保存
float Yaw_imu_out=0.0;							 //外环总输出
//外环PID参数
float Yaw_imu_kp=3;//
float Yaw_imu_ki=0.0001;//
float Yaw_imu_kd=0.1;//
float imu_PID(float RC_Yaw_angle,float Yaw_angle)
{
	int16_t out;
////////////////////////外环角度环(PID)///////////////////////////////
  Yaw_imu_i+=(Yaw_angle-RC_Yaw_angle);
//-------------Pitch积分限幅----------------//
  if(Yaw_imu_i>1000) Yaw_imu_i=1000;
  else if(Yaw_imu_i<-1000) Yaw_imu_i=-1000;
//-------------Pitch微分--------------------//
  Yaw_d=Yaw_angle-Yaw_imu_old;
//-------------Pitch  PID-------------------//
  Yaw_imu_out = Yaw_imu_kp*(Yaw_angle-RC_Yaw_angle) + Yaw_imu_ki*Yaw_imu_i + Yaw_imu_kd*Yaw_imu_d;
//角度保存
  Yaw_imu_old=Yaw_angle;
		out = Yaw_imu_out;
	
//	if(out>=30)		out=30;	
//	if(out<=-70)	out=-70;
	
	return out;

}














//#include "LBpid.h"
////#include "mpu6050.h"
////#include "can2.h"
//#include "main.h"

//int16_t	Pitch_Fina_Out;
//int16_t	Yaw_Fina_Out;


///////////////////////Pitch-PID//////////////////////////////
//float Pitch_i;                       //积分项
//float Pitch_old;                		 //角度保存
//float Pitch_d;         							 //微分项
//float Pitch_shell_out;							 //外环总输出
////外环PID参数
//float Pitch_shell_kp=0;//   20
//float Pitch_shell_ki=0;//0.2
//float Pitch_shell_kd=0;//50
//float Speed_Pitch;		//速度保留
//float pitch_core_kp_out,pitch_core_kd_out;	//内环单项输出
////内环PID参数
//float Pitch_core_kp=10;
//float Pitch_core_kd=5;

//float Speed_Pitch_Old;
//float Pitch_core_out;//内环总输出

//int Pitch_PID(float RC_Pitch,float GYRO_Pitch,float Encoder_Angle)
//{
//	int16_t out;
//////////////////////////外环角度环(PID)///////////////////////////////
//  Pitch_i+=(Encoder_Angle-RC_Pitch);
////-------------Pitch积分限幅----------------//
//  if(Pitch_i>1000) Pitch_i=1000;
//  else if(Pitch_i<-1000) Pitch_i=-1000;
////-------------Pitch微分--------------------//
//  Pitch_d=Encoder_Angle-Pitch_old;
////-------------Pitch  PID-------------------//
//  Pitch_shell_out = Pitch_shell_kp*(Encoder_Angle-RC_Pitch) + Pitch_shell_ki*Pitch_i + Pitch_shell_kd*Pitch_d;
////角度保存
//  Pitch_old=Encoder_Angle;

//////////////////////////内环角速度环(PD)///////////////////////////////       
//  pitch_core_kp_out = Pitch_core_kp * (Pitch_shell_out +  GYRO_Pitch * 3.5f);
//  pitch_core_kd_out = Pitch_core_kd * (GYRO_Pitch - Speed_Pitch_Old);
//	
//	Pitch_core_out = (pitch_core_kp_out + pitch_core_kd_out);
//	Speed_Pitch_Old = GYRO_Pitch;
//	
////--------------------输出到电机--------------------------------//
//	out = (int16_t)Pitch_core_out;
//	
//	if(out>=4500)		out=4500;	
//	if(out<=-4500)	out=-4500;
//	
//	return out;
//}


///////////////////////Yaw-PID//////////////////////////////
//float Yaw_i;                       //积分项
//float Yaw_old;                		 //角度保存
//float Yaw_d;         							 //微分项
//float Yaw_shell_out;							 //外环总输出
////外环PID参数
//float Yaw_shell_kp=0;//
//float Yaw_shell_ki=0;//
//float Yaw_shell_kd=0;//
//float Speed_Yaw;		//速度保留
//float Yaw_core_kp_out,Yaw_core_kd_out;	//内环单项输出
////内环PID参数
//float Yaw_core_kp=0;
//float Yaw_core_kd=0;

//float Speed_Yaw_Old;
//float Yaw_core_out;//内环总输出

//int Yaw_PID(float RC_Yaw,float GYRO_Yaw,float Encoder_Angle)
//{
//	int16_t out;
//////////////////////////外环角度环(PID)///////////////////////////////
//  Yaw_i+=(Encoder_Angle-RC_Yaw);
////-------------Yaw积分限幅----------------//
//  if(Yaw_i>1000) Yaw_i=1000;
//  else if(Yaw_i<-1000) Yaw_i=-1000;
////-------------Yaw微分--------------------//
//  Yaw_d=Encoder_Angle-Yaw_old;
////-------------Yaw  PID-------------------//
//  Yaw_shell_out = Yaw_shell_kp*(Encoder_Angle-RC_Yaw) + Yaw_shell_ki*Yaw_i + Yaw_shell_kd*Yaw_d;
////角度保存
//  Yaw_old=Encoder_Angle;

//////////////////////////内环角速度环(PD)///////////////////////////////       
//  Yaw_core_kp_out = Yaw_core_kp * (Yaw_shell_out +  GYRO_Yaw * 3.5f);
//  Yaw_core_kd_out = Yaw_core_kd * (GYRO_Yaw - Speed_Yaw_Old);
//	
//	Yaw_core_out = (Yaw_core_kp_out + Yaw_core_kd_out);
//	Speed_Yaw_Old = GYRO_Yaw;
//	
////--------------------输出到电机--------------------------------//
//	out = (int16_t)Yaw_core_out;
//	
//	if(out>=4000)		out=4000;	
//	if(out<=-4000)	out=-4000;
//	
//	return out;
//}


//void Gimbal_Set(float RC_Pitch,float RC_Yaw,float GYRO_Pitch,float GYRO_Yaw,float Pitch_Encoder_Angle,float Yaw_Encoder_Angle)
//{
//	Pitch_Fina_Out = Pitch_PID(RC_Pitch,GYRO_Pitch,Pitch_Encoder_Angle);
//	Yaw_Fina_Out   = Yaw_PID(RC_Yaw , GYRO_Yaw , Yaw_Encoder_Angle);
//		Set_Gimbal_Current(CAN1,Yaw_Fina_Out,Pitch_Fina_Out);
//		//delay_us(0);
//	
////	CAN_SetGimbalMsg();
//}
///////////////////////imu-PID//////////////////////////////
//float Yaw_imu_i;                       //积分项
//float Yaw_imu_d;         							 //微分项
//float Yaw_imu_old;                		 //角度保存
//float Yaw_imu_out;							 //外环总输出
////外环PID参数
//float Yaw_imu_kp=3;//
//float Yaw_imu_ki=0.0001;//
//float Yaw_imu_kd=0.1;//
//float imu_PID(float RC_Yaw_angle,float Yaw_angle)
//{
//	int16_t out;
//////////////////////////外环角度环(PID)///////////////////////////////
//  Yaw_imu_i+=(Yaw_angle-RC_Yaw_angle);
////-------------Pitch积分限幅----------------//
//  if(Yaw_imu_i>1000) Yaw_imu_i=1000;
//  else if(Yaw_imu_i<-1000) Yaw_imu_i=-1000;
////-------------Pitch微分--------------------//
//  Pitch_d=Yaw_angle-Yaw_imu_old;
////-------------Pitch  PID-------------------//
//  Yaw_imu_out = Yaw_imu_kp*(Yaw_angle-RC_Yaw_angle) + Yaw_imu_ki*Yaw_imu_i + Yaw_imu_kd*Yaw_imu_d;
////角度保存
//  Yaw_imu_old=Yaw_angle;
//		out = Yaw_imu_out;
//	
//	if(out>=30)		out=30;	
//	if(out<=-70)	out=-70;
//	
//	return out;

//}


