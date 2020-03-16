#include "LBpid.h"
//#include "mpu6050.h"
//#include "can2.h"
#include "main.h"

int16_t	Pitch_Fina_Out=0;
int16_t	Yaw_Fina_Out=0;


/////////////////////Pitch-PID//////////////////////////////
float Pitch_i;                       //积分项
float Pitch_old;                		 //角度保存
float Pitch_d;         							 //微分项
float Pitch_shell_out;							 //外环总输出
//外环PID参数
float Pitch_shell_kp=60;//     高速版//60     低速稳定//22
float Pitch_shell_ki=0.05;//         //0.05           //0.05
float Pitch_shell_kd=40;//           //40             //10
float Speed_Pitch;		//速度保留
float pitch_core_kp_out,pitch_core_kd_out;	//内环单项输出
//内环PID参数
float Pitch_core_kp=3;               //3              //4
float Pitch_core_kd=13;              //13             //13

float Speed_Pitch_Old;
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
  Pitch_shell_out = Pitch_shell_kp*(Encoder_Angle-RC_Pitch) + Pitch_shell_ki*Pitch_i + Pitch_shell_kd*Pitch_d;
//角度保存
  Pitch_old=Encoder_Angle;

////////////////////////内环角速度环(PD)///////////////////////////////       
  pitch_core_kp_out = Pitch_core_kp * (Pitch_shell_out +  GYRO_Pitch * 3.5f);
  pitch_core_kd_out = Pitch_core_kd * (GYRO_Pitch - Speed_Pitch_Old);
	
	Pitch_core_out = (pitch_core_kp_out + pitch_core_kd_out);
	Speed_Pitch_Old = GYRO_Pitch;
	
  out = (int16_t)Pitch_core_out;

	if(out>=4800)		out=4800;	
	if(out<=-4800)	out=-4800;
	
	return out;
}


/////////////////////////////Yaw轴-PID///////////////////////////////
float Yaw_i=0.0;                       //积分项     
float Yaw_old=0.0;                		 //角度保存   
float Yaw_d=0.0;         							 //微分项 		
float Yaw_shell_out=0.0;							 //外环总输出 
//外环PID参数
float Yaw_shell_kp=70;//24         20
float Yaw_shell_ki=0.001;//0.001    0.05
float Yaw_shell_kd=15;//           10


float Speed_Yaw;		                                       //速度保留
float Yaw_core_i=0.0;                                      //cml:内环积分项
float Yaw_core_kp_out,Yaw_core_ki_out,Yaw_core_kd_out;     //内环单项输出

float Yaw_core_kp=7;      //5                                       //内环PID参数
float Yaw_core_ki=0;      //0
float Yaw_core_kd=5;     //10

float Speed_Yaw_Old;
float Yaw_core_out;//内环总输出

//                             mpu 反馈值     电机编码器反馈值
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
  Yaw_shell_out = -Yaw_shell_kp*(Encoder_Angle-RC_Yaw) + Yaw_shell_ki*Yaw_i + Yaw_shell_kd*Yaw_d;
//角度保存
  Yaw_old=Encoder_Angle;

////////////////////////内环角速度环(PD)///////////////////////////////       
  Yaw_core_kp_out = Yaw_core_kp * (Yaw_shell_out +  GYRO_Yaw * 3.5f);
	//Yaw_core_ki_out
  Yaw_core_kd_out = Yaw_core_kd * (GYRO_Yaw - Speed_Yaw_Old);
	
	Yaw_core_out = (Yaw_core_kp_out + Yaw_core_kd_out);
	Speed_Yaw_Old = GYRO_Yaw;
//////	
////////--------------------输出到电机--------------------------------//

// out = (int16_t)Yaw_shell_out;

  if(Yaw_core_out>28000) Yaw_core_out=28000;
	if(Yaw_core_out<-28000) Yaw_core_out=-28000;
	
	 out = (int16_t)Yaw_core_out;
	
	return out;
}


void Gimbal_Set(float RC_Pitch,float RC_Yaw,float GYRO_Pitch,float GYRO_Yaw,float Pitch_Encoder_Angle,float Yaw_Encoder_Angle)
{
	Pitch_Fina_Out = Pitch_PID(RC_Pitch,GYRO_Pitch,Pitch_Encoder_Angle);
  Yaw_Fina_Out   = Yaw_PID(RC_Yaw , GYRO_Yaw , Yaw_Encoder_Angle);
	Set_Gimbal_Current(CAN1,Pitch_Fina_Out,Yaw_Fina_Out);//发送使电机转动，can1传输，注意！1.不同电机标识符不同，2.不同电机yaw,pitch数据传输位置不同6020 pitch在0，1，6010 pitch在2，3   
	delay_us(500);
}


/////////////////////imu-PID//////////////////////////////
float Yaw_imu_i=0.0;                       //积分项 //cml:赋初值
float Yaw_imu_d=0.0;         							 //微分项 //cml:赋初值
float Yaw_imu_old=0.0;                		     //角度保存
float Yaw_imu_out=0.0;									    	 //外环总输出
//外环PID参数
float Yaw_imu_kp=3;//
float Yaw_imu_ki=0.0001;//
float Yaw_imu_kd=0.1;//
float imu_PID(float RC_Yaw_angle,float Yaw_angle)
{
	int16_t out;
////////////////////////外环角度环(PID)///////////////////////////////
  Yaw_imu_i+=(Yaw_angle-RC_Yaw_angle);   //cml:Yaw_imu_i的初始值在哪设置的？
//-------------Pitch积分限幅----------------//
  if(Yaw_imu_i>1000) Yaw_imu_i=1000;
  else if(Yaw_imu_i<-1000) Yaw_imu_i=-1000;
//-------------Pitch微分--------------------//
  //Pitch_d=Yaw_angle-Yaw_imu_old;
	Yaw_imu_d=Yaw_angle-Yaw_imu_old;
//-------------Pitch  PID-------------------//
  Yaw_imu_out = Yaw_imu_kp*(Yaw_angle-RC_Yaw_angle) + Yaw_imu_ki*Yaw_imu_i + Yaw_imu_kd*Yaw_imu_d;
//角度保存
  Yaw_imu_old=Yaw_angle;
	out = Yaw_imu_out;
	
	if(out>=30)		out=30;	
	if(out<=-70)	out=-70;
	
	return out;

}

