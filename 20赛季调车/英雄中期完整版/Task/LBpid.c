#include "LBpid.h"
//#include "mpu6050.h"
//#include "can2.h"
#include "main.h"

int16_t	Pitch_Fina_Out;
int16_t	Yaw_Fina_Out;


/////////////////////Pitch-PID//////////////////////////////
float Pitch_i=0.0;                       //������
float Pitch_old=0.0;                		 //�Ƕȱ���
float Pitch_d;         							 //΢����
float Pitch_shell_out;							 //�⻷�����
//�⻷PID����
//float Pitch_shell_kp=120;//      4hao:90 0.2 50
//float Pitch_shell_ki=0.2;//
//float Pitch_shell_kd=300;//
//float Speed_Pitch;		//�ٶȱ���
//float pitch_core_kp_out,pitch_core_kd_out;	//�ڻ��������
////�ڻ�PID����
//float Pitch_core_kp=3;       //4hao 15 5
//float Pitch_core_kd=100;
//float Pitch_shell_kp=130;//      4hao:90 0.2 50
//float Pitch_shell_ki=0.2;//
//float Pitch_shell_kd=150;//
//float Speed_Pitch;		//�ٶȱ���
//float pitch_core_kp_out,pitch_core_kd_out;	//�ڻ��������
////�ڻ�PID����
//float Pitch_core_kp=12;       //4hao 15 5
//float Pitch_core_kd=180;
float Pitch_shell_kp=130;//      4hao:90 0.2 50       500
float Pitch_shell_ki=0.1;//   250 0.2 300 12 200   0.2
float Pitch_shell_kd=-280;//   800     500 6  500   500
float Speed_Pitch;		//�ٶȱ���
float pitch_core_kp_out,pitch_core_kd_out;	//�ڻ��������
//�ڻ�PID����
float Pitch_core_kp=13;       //4hao 15 5    4
float Pitch_core_kd=-80;     //   200

float Speed_Pitch_Old=0.0;
float Pitch_core_out;//�ڻ������
//p ����  i ���� ������̬��� d ΢�� ��ֹ���� Ԥ�����ı仯
int Pitch_PID(float RC_Pitch,float GYRO_Pitch,float Encoder_Angle)
{
	int16_t out;
////////////////////////�⻷�ǶȻ�(PID)///////////////////////////////
  Pitch_i+=(Encoder_Angle-RC_Pitch);
//-------------Pitch�����޷�----------------//
  if(Pitch_i>1000) Pitch_i=1000;
  else if(Pitch_i<-1000) Pitch_i=-1000;
//-------------Pitch΢��--------------------//
  Pitch_d=Encoder_Angle-Pitch_old;
//-------------Pitch  PID-------------------//
  Pitch_shell_out = Pitch_shell_kp*(Encoder_Angle-RC_Pitch) + Pitch_shell_ki*Pitch_i - Pitch_shell_kd*Pitch_d;
//�Ƕȱ���
  Pitch_old=Encoder_Angle;
////////////////////////�ڻ����ٶȻ�(PD)///////////////////////////////       
  pitch_core_kp_out = -Pitch_core_kp * (-Pitch_shell_out +  GYRO_Pitch * 4.5f);
  pitch_core_kd_out = Pitch_core_kd * (GYRO_Pitch - Speed_Pitch_Old);
	
	Pitch_core_out = (pitch_core_kp_out + pitch_core_kd_out);
	
  if(Pitch_core_out>=28000)		Pitch_core_out=28000;	
	if(Pitch_core_out<=-28000)	Pitch_core_out=-28000;
	Speed_Pitch_Old = GYRO_Pitch;
	
//--------------------��������--------------------------------//
	out = (int16_t)Pitch_core_out;
	
	
	return out;
}


/////////////////////Yaw-PID//////////////////////////////
float Yaw_i=0.0;                       //������
float Yaw_old=0.0;                		 //�Ƕȱ���
float Yaw_d=0.0;         							 //΢����
float Yaw_shell_out=0.0;							 //�⻷�����
//�⻷PID����
float Yaw_shell_kp=120;//250 0.2 300 0.3 200                      //1000
float Yaw_shell_ki=0.02;//1000 0.02 800  300 0.001 70 4 400        //0.02
float Yaw_shell_kd=20;//                                          //800
float Speed_Yaw;		//�ٶȱ���
float Yaw_core_kp_out,Yaw_core_kd_out;	//�ڻ��������
//�ڻ�PID����
float Yaw_core_kp=30;        //4
float Yaw_core_kd=20;      //400

float Speed_Yaw_Old=0.0;
float Yaw_core_out;//�ڻ������

int Yaw_PID(float RC_Yaw,float GYRO_Yaw,float Encoder_Angle)
{
	int16_t out;
////////////////////////�⻷�ǶȻ�(PID)///////////////////////////////
  Yaw_i+=(Encoder_Angle-RC_Yaw);
//-------------Yaw�����޷�----------------//
  if(Yaw_i>1000) Yaw_i=1000;
  else if(Yaw_i<-1000) Yaw_i=-1000;
//-------------Yaw΢��--------------------//
  Yaw_d=Encoder_Angle-Yaw_old;
//-------------Yaw  PID-------------------//
  Yaw_shell_out = -Yaw_shell_kp*(Encoder_Angle-RC_Yaw) + Yaw_shell_ki*Yaw_i + Yaw_shell_kd*Yaw_d;
//�Ƕȱ���
  Yaw_old=Encoder_Angle;

////////////////////////�ڻ����ٶȻ�(PD)///////////////////////////////       
  Yaw_core_kp_out = Yaw_core_kp * (Yaw_shell_out -  GYRO_Yaw * 3.5f);
  Yaw_core_kd_out = Yaw_core_kd * (GYRO_Yaw - Speed_Yaw_Old);
	
	Yaw_core_out = (Yaw_core_kp_out + Yaw_core_kd_out);

	Speed_Yaw_Old = GYRO_Yaw;
	
//--------------------��������--------------------------------//
	
	if(Yaw_core_out>=28000)		Yaw_core_out=28000;	
	if(Yaw_core_out<=-28000)	Yaw_core_out=-28000;
	out = (int16_t)Yaw_core_out;
	
	
	return out;
}


void Gimbal_Set(float RC_Pitch,float RC_Yaw,float GYRO_Pitch,float GYRO_Yaw,float Pitch_Encoder_Angle,float Yaw_Encoder_Angle)
{
	Pitch_Fina_Out = Pitch_PID(RC_Pitch,GYRO_Pitch,Pitch_Encoder_Angle);
	Yaw_Fina_Out   = Yaw_PID(RC_Yaw , GYRO_Yaw , Yaw_Encoder_Angle);
//		delay_us(10);
	
//	CAN_SetGimbalMsg();
}
/////////////////////imu-PID//////////////////////////////
float Yaw_imu_i=0.0;                       //������
float Yaw_imu_d=0.0;         							 //΢����
float Yaw_imu_old=0.0;                		 //�Ƕȱ���
float Yaw_imu_out=0.0;							 //�⻷�����
//�⻷PID����
float Yaw_imu_kp=3;//
float Yaw_imu_ki=0.0001;//
float Yaw_imu_kd=0.1;//
float imu_PID(float RC_Yaw_angle,float Yaw_angle)
{
	int16_t out;
////////////////////////�⻷�ǶȻ�(PID)///////////////////////////////
  Yaw_imu_i+=(Yaw_angle-RC_Yaw_angle);
//-------------Pitch�����޷�----------------//
  if(Yaw_imu_i>1000) Yaw_imu_i=1000;
  else if(Yaw_imu_i<-1000) Yaw_imu_i=-1000;
//-------------Pitch΢��--------------------//
  Yaw_d=Yaw_angle-Yaw_imu_old;
//-------------Pitch  PID-------------------//
  Yaw_imu_out = Yaw_imu_kp*(Yaw_angle-RC_Yaw_angle) + Yaw_imu_ki*Yaw_imu_i + Yaw_imu_kd*Yaw_imu_d;
//�Ƕȱ���
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
//float Pitch_i;                       //������
//float Pitch_old;                		 //�Ƕȱ���
//float Pitch_d;         							 //΢����
//float Pitch_shell_out;							 //�⻷�����
////�⻷PID����
//float Pitch_shell_kp=0;//   20
//float Pitch_shell_ki=0;//0.2
//float Pitch_shell_kd=0;//50
//float Speed_Pitch;		//�ٶȱ���
//float pitch_core_kp_out,pitch_core_kd_out;	//�ڻ��������
////�ڻ�PID����
//float Pitch_core_kp=10;
//float Pitch_core_kd=5;

//float Speed_Pitch_Old;
//float Pitch_core_out;//�ڻ������

//int Pitch_PID(float RC_Pitch,float GYRO_Pitch,float Encoder_Angle)
//{
//	int16_t out;
//////////////////////////�⻷�ǶȻ�(PID)///////////////////////////////
//  Pitch_i+=(Encoder_Angle-RC_Pitch);
////-------------Pitch�����޷�----------------//
//  if(Pitch_i>1000) Pitch_i=1000;
//  else if(Pitch_i<-1000) Pitch_i=-1000;
////-------------Pitch΢��--------------------//
//  Pitch_d=Encoder_Angle-Pitch_old;
////-------------Pitch  PID-------------------//
//  Pitch_shell_out = Pitch_shell_kp*(Encoder_Angle-RC_Pitch) + Pitch_shell_ki*Pitch_i + Pitch_shell_kd*Pitch_d;
////�Ƕȱ���
//  Pitch_old=Encoder_Angle;

//////////////////////////�ڻ����ٶȻ�(PD)///////////////////////////////       
//  pitch_core_kp_out = Pitch_core_kp * (Pitch_shell_out +  GYRO_Pitch * 3.5f);
//  pitch_core_kd_out = Pitch_core_kd * (GYRO_Pitch - Speed_Pitch_Old);
//	
//	Pitch_core_out = (pitch_core_kp_out + pitch_core_kd_out);
//	Speed_Pitch_Old = GYRO_Pitch;
//	
////--------------------��������--------------------------------//
//	out = (int16_t)Pitch_core_out;
//	
//	if(out>=4500)		out=4500;	
//	if(out<=-4500)	out=-4500;
//	
//	return out;
//}


///////////////////////Yaw-PID//////////////////////////////
//float Yaw_i;                       //������
//float Yaw_old;                		 //�Ƕȱ���
//float Yaw_d;         							 //΢����
//float Yaw_shell_out;							 //�⻷�����
////�⻷PID����
//float Yaw_shell_kp=0;//
//float Yaw_shell_ki=0;//
//float Yaw_shell_kd=0;//
//float Speed_Yaw;		//�ٶȱ���
//float Yaw_core_kp_out,Yaw_core_kd_out;	//�ڻ��������
////�ڻ�PID����
//float Yaw_core_kp=0;
//float Yaw_core_kd=0;

//float Speed_Yaw_Old;
//float Yaw_core_out;//�ڻ������

//int Yaw_PID(float RC_Yaw,float GYRO_Yaw,float Encoder_Angle)
//{
//	int16_t out;
//////////////////////////�⻷�ǶȻ�(PID)///////////////////////////////
//  Yaw_i+=(Encoder_Angle-RC_Yaw);
////-------------Yaw�����޷�----------------//
//  if(Yaw_i>1000) Yaw_i=1000;
//  else if(Yaw_i<-1000) Yaw_i=-1000;
////-------------Yaw΢��--------------------//
//  Yaw_d=Encoder_Angle-Yaw_old;
////-------------Yaw  PID-------------------//
//  Yaw_shell_out = Yaw_shell_kp*(Encoder_Angle-RC_Yaw) + Yaw_shell_ki*Yaw_i + Yaw_shell_kd*Yaw_d;
////�Ƕȱ���
//  Yaw_old=Encoder_Angle;

//////////////////////////�ڻ����ٶȻ�(PD)///////////////////////////////       
//  Yaw_core_kp_out = Yaw_core_kp * (Yaw_shell_out +  GYRO_Yaw * 3.5f);
//  Yaw_core_kd_out = Yaw_core_kd * (GYRO_Yaw - Speed_Yaw_Old);
//	
//	Yaw_core_out = (Yaw_core_kp_out + Yaw_core_kd_out);
//	Speed_Yaw_Old = GYRO_Yaw;
//	
////--------------------��������--------------------------------//
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
//float Yaw_imu_i;                       //������
//float Yaw_imu_d;         							 //΢����
//float Yaw_imu_old;                		 //�Ƕȱ���
//float Yaw_imu_out;							 //�⻷�����
////�⻷PID����
//float Yaw_imu_kp=3;//
//float Yaw_imu_ki=0.0001;//
//float Yaw_imu_kd=0.1;//
//float imu_PID(float RC_Yaw_angle,float Yaw_angle)
//{
//	int16_t out;
//////////////////////////�⻷�ǶȻ�(PID)///////////////////////////////
//  Yaw_imu_i+=(Yaw_angle-RC_Yaw_angle);
////-------------Pitch�����޷�----------------//
//  if(Yaw_imu_i>1000) Yaw_imu_i=1000;
//  else if(Yaw_imu_i<-1000) Yaw_imu_i=-1000;
////-------------Pitch΢��--------------------//
//  Pitch_d=Yaw_angle-Yaw_imu_old;
////-------------Pitch  PID-------------------//
//  Yaw_imu_out = Yaw_imu_kp*(Yaw_angle-RC_Yaw_angle) + Yaw_imu_ki*Yaw_imu_i + Yaw_imu_kd*Yaw_imu_d;
////�Ƕȱ���
//  Yaw_imu_old=Yaw_angle;
//		out = Yaw_imu_out;
//	
//	if(out>=30)		out=30;	
//	if(out<=-70)	out=-70;
//	
//	return out;

//}


