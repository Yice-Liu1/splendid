#include "gimbalpid.h"


float Kp_pi_po=0;//300;    //cml:pitch轴 角度环PID 参数//180 -5 60 50 10 10
float Ki_pi_po=0;//1.6;
float Kd_pi_po=0;
float out_pi_po=0;


float Kp_pi_sp=0;//71.4;//250//25;
float Ki_pi_sp=0;//14.2;//50//10;
float Kd_pi_sp=0;//16.6;//500//20;
float out_pi_sp=0;


int pitch_pid(float wish_pitch,float angle_pitch,float speed_pitch)
{
	float error[2];                       //cml:误差存放
	float accumu_i_posi=0.0;              //cml:积分存放
  float angle_old=0.0;                  //cml:前角度存放
	
	float error_speed[2];
	float accumu_i_speed=0.0;
	float angle_speed_old=0.0;
	
	short int output;
	
	
//error[0] = 145 - angle_use;                            //cml:先以0°作为期望
	error[0] = angle_pitch - wish_pitch; 
	error[1] = angle_pitch - angle_old;
	
	accumu_i_posi += error[0]; 
	
  if(accumu_i_posi>1000) accumu_i_posi=1000;
  else if(accumu_i_posi<-1000) accumu_i_posi=-1000;      //cml:参考例程得来的需要限制，还不知道为什么
	
	out_pi_po =   Kp_pi_po * error[0] + Ki_pi_po * accumu_i_posi + Kd_pi_po * error[1];
							
/*****************************************************************************************************/

	
	
  error_speed[0] = 	out_pi_po - speed_pitch*2.0f;
	error_speed[1] =  speed_pitch - angle_speed_old;
//error_speed[1] = 加速度
	
	accumu_i_speed += error_speed[0];
	
	if(accumu_i_speed>1000) accumu_i_speed=1000;
  else if(accumu_i_speed<-1000) accumu_i_speed=-1000;      //cml:依旧积分限制
	
	out_pi_sp =   Kp_pi_sp * error_speed[0] + Ki_pi_sp * accumu_i_speed + Kd_pi_sp * error_speed[1];


/*************************************************/
 
 angle_old = angle_pitch;
 angle_speed_old = speed_pitch;


	if(out_pi_sp>4500) out_pi_sp=4500;
	if(out_pi_sp<-4500) out_pi_sp=-4500;
	
	output = (short int)out_pi_sp;
	
	return output;
}


/***********************************yaw_pid*****************************************/
float Kp_ya_po=60;      //cml:pitch轴 角度环PID 参数
float Ki_ya_po=0.01;
float Kd_ya_po=40;
float out_ya_po=0.0;


float Kp_ya_sp=30;   //30
float Ki_ya_sp=0.1;
float Kd_ya_sp=5;
float out_ya_sp=0;



int yaw_pid(float wish_yaw,float angle_yaw,float speed_yaw)
{
	float error[2];                       //cml:误差存放
	float accumu_i_posi=0.0;              //cml:积分存放
  float angle_old=0.0;                  //cml:前角度存放
	
	float error_speed[2];
	float accumu_i_speed=0.0;
	float angle_speed_old=0.0;
	
	short int output;
	
	
//error[0] = 145 - angle_use;                            //cml:先以0°作为期望
	error[0] = angle_yaw - wish_yaw; 
	error[1] = angle_yaw - angle_old;
	
	accumu_i_posi += error[0]; 
	
  if(accumu_i_posi>1000) accumu_i_posi=1000;
  else if(accumu_i_posi<-1000) accumu_i_posi=-1000;      //cml:参考例程得来的需要限制，还不知道为什么
	
	out_ya_po =   Kp_ya_po * error[0] + Ki_ya_po * accumu_i_posi + Ki_ya_po * error[1];
							
/*****************************************************************************************************/

	
	
  error_speed[0] = 	out_ya_po + speed_yaw * 3.5f;
	error_speed[1] =  speed_yaw - angle_speed_old;
//error_speed[1] = 加速度
	
	accumu_i_speed += error_speed[0];
	
	if(accumu_i_speed>1000) accumu_i_speed=1000;
  else if(accumu_i_speed<-1000) accumu_i_speed=-1000;      //cml:依旧积分限制
	
	out_ya_sp =   Kp_ya_sp * error_speed[0] + Ki_ya_sp * accumu_i_speed + Kd_ya_sp * error[1];


/*************************************************/
 
 angle_old = angle_yaw;
 angle_speed_old = speed_yaw;


	if(out_ya_sp>25000) out_ya_sp=25000;
	if(out_ya_sp<-25000) out_ya_sp=-25000;
	
	output = (short int)out_ya_sp;
	
	return output;
}


float Kp_pic=0;
float Ki_pic=0;
float Kd_pic=0;
float out_pic=0;



int piture_pid(float picture_angle,float picture_wish)
{
	float error[2];                       //cml:误差存放
	float accumu_i_pic=0.0;              //cml:积分存放
  float angle_old=0.0;                  //cml:前角度存放
	short int output;
	
	
	error[0] = picture_wish - picture_angle; 
	error[1] = picture_angle - angle_old;
	
	accumu_i_pic += error[0]; 
	
  if(accumu_i_pic>1000) accumu_i_pic=1000;
  else if(accumu_i_pic<-1000) accumu_i_pic=-1000;      //cml:参考例程得来的需要限制，还不知道为什么
	
	out_pic =   Kp_pic * error[0] + Ki_pic * accumu_i_pic + Kd_pic * error[1];

 
 angle_old = picture_angle;

	if(out_pic>250) out_pic=250;
	if(out_pic<-200) out_pic=-200;
	
	output = (short int)out_pic;
	
	
	
	return output;
	
}

