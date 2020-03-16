#include "LBpid.h"
//#include "mpu6050.h"
//#include "can2.h"
#include "main.h"

int16_t	Pitch_Fina_Out=0;
int16_t	Yaw_Fina_Out=0;


/////////////////////Pitch-PID//////////////////////////////
float Pitch_i;                       //������
float Pitch_old;                		 //�Ƕȱ���
float Pitch_d;         							 //΢����
float Pitch_shell_out;							 //�⻷�����
//�⻷PID����
float Pitch_shell_kp=60;//     ���ٰ�//60     �����ȶ�//22
float Pitch_shell_ki=0.05;//         //0.05           //0.05
float Pitch_shell_kd=40;//           //40             //10
float Speed_Pitch;		//�ٶȱ���
float pitch_core_kp_out,pitch_core_kd_out;	//�ڻ��������
//�ڻ�PID����
float Pitch_core_kp=3;               //3              //4
float Pitch_core_kd=13;              //13             //13

float Speed_Pitch_Old;
float Pitch_core_out;//�ڻ������

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
  Pitch_shell_out = Pitch_shell_kp*(Encoder_Angle-RC_Pitch) + Pitch_shell_ki*Pitch_i + Pitch_shell_kd*Pitch_d;
//�Ƕȱ���
  Pitch_old=Encoder_Angle;

////////////////////////�ڻ����ٶȻ�(PD)///////////////////////////////       
  pitch_core_kp_out = Pitch_core_kp * (Pitch_shell_out +  GYRO_Pitch * 3.5f);
  pitch_core_kd_out = Pitch_core_kd * (GYRO_Pitch - Speed_Pitch_Old);
	
	Pitch_core_out = (pitch_core_kp_out + pitch_core_kd_out);
	Speed_Pitch_Old = GYRO_Pitch;
	
  out = (int16_t)Pitch_core_out;

	if(out>=4800)		out=4800;	
	if(out<=-4800)	out=-4800;
	
	return out;
}


/////////////////////////////Yaw��-PID///////////////////////////////
float Yaw_i=0.0;                       //������     
float Yaw_old=0.0;                		 //�Ƕȱ���   
float Yaw_d=0.0;         							 //΢���� 		
float Yaw_shell_out=0.0;							 //�⻷����� 
//�⻷PID����
float Yaw_shell_kp=70;//24         20
float Yaw_shell_ki=0.001;//0.001    0.05
float Yaw_shell_kd=15;//           10


float Speed_Yaw;		                                       //�ٶȱ���
float Yaw_core_i=0.0;                                      //cml:�ڻ�������
float Yaw_core_kp_out,Yaw_core_ki_out,Yaw_core_kd_out;     //�ڻ��������

float Yaw_core_kp=7;      //5                                       //�ڻ�PID����
float Yaw_core_ki=0;      //0
float Yaw_core_kd=5;     //10

float Speed_Yaw_Old;
float Yaw_core_out;//�ڻ������

//                             mpu ����ֵ     �������������ֵ
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
  Yaw_core_kp_out = Yaw_core_kp * (Yaw_shell_out +  GYRO_Yaw * 3.5f);
	//Yaw_core_ki_out
  Yaw_core_kd_out = Yaw_core_kd * (GYRO_Yaw - Speed_Yaw_Old);
	
	Yaw_core_out = (Yaw_core_kp_out + Yaw_core_kd_out);
	Speed_Yaw_Old = GYRO_Yaw;
//////	
////////--------------------��������--------------------------------//

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
	Set_Gimbal_Current(CAN1,Pitch_Fina_Out,Yaw_Fina_Out);//����ʹ���ת����can1���䣬ע�⣡1.��ͬ�����ʶ����ͬ��2.��ͬ���yaw,pitch���ݴ���λ�ò�ͬ6020 pitch��0��1��6010 pitch��2��3   
	delay_us(500);
}


/////////////////////imu-PID//////////////////////////////
float Yaw_imu_i=0.0;                       //������ //cml:����ֵ
float Yaw_imu_d=0.0;         							 //΢���� //cml:����ֵ
float Yaw_imu_old=0.0;                		     //�Ƕȱ���
float Yaw_imu_out=0.0;									    	 //�⻷�����
//�⻷PID����
float Yaw_imu_kp=3;//
float Yaw_imu_ki=0.0001;//
float Yaw_imu_kd=0.1;//
float imu_PID(float RC_Yaw_angle,float Yaw_angle)
{
	int16_t out;
////////////////////////�⻷�ǶȻ�(PID)///////////////////////////////
  Yaw_imu_i+=(Yaw_angle-RC_Yaw_angle);   //cml:Yaw_imu_i�ĳ�ʼֵ�������õģ�
//-------------Pitch�����޷�----------------//
  if(Yaw_imu_i>1000) Yaw_imu_i=1000;
  else if(Yaw_imu_i<-1000) Yaw_imu_i=-1000;
//-------------Pitch΢��--------------------//
  //Pitch_d=Yaw_angle-Yaw_imu_old;
	Yaw_imu_d=Yaw_angle-Yaw_imu_old;
//-------------Pitch  PID-------------------//
  Yaw_imu_out = Yaw_imu_kp*(Yaw_angle-RC_Yaw_angle) + Yaw_imu_ki*Yaw_imu_i + Yaw_imu_kd*Yaw_imu_d;
//�Ƕȱ���
  Yaw_imu_old=Yaw_angle;
	out = Yaw_imu_out;
	
	if(out>=30)		out=30;	
	if(out<=-70)	out=-70;
	
	return out;

}

