#ifndef __MAIN_H
#define __MAIN_H

#define VAL_LIMIT(val, min, max)\
if(val<=min)\
{\
	val = min;\
}\
else if(val>=max)\
{\
	val = max;\
}\



#define ABS_VALUE(value)\
if(value>=0)\
{\
	value = value;\
}\
else if(value<0)\
{\
	value=-value;\
}\

#include "stm32f4xx.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#include "bsp.h"
//Task
#include "imu.h"
#include "test_imu.h"
#include "mpu6500_interrupt.h"
#include "IOTask.h"
#include "ControlTask.h"
#include "CanBusTask.h"
#include "RemoteTask.h"
#include "SuperviseTask.h"
#include "PID.h"
#include "lost_counter.h"
#include "Ramp.h"
#include "Protocol.h"
#include "oscilloscope.h"
#include "LBpid.h"
#include "timer.h"



//#include "JY901.h"

//extern struct STime		stcTime;
//extern struct SAcc 		stcAcc;
//extern struct SGyro 		stcGyro;
//extern struct SAngle 	stcAngle;
//extern struct SMag 		stcMag;
//extern struct SDStatus stcDStatus;
//extern struct SPress 	stcPress;
//extern struct SLonLat 	stcLonLat;
//extern struct SGPSV 		stcGPSV;
//extern struct SQ       stcQ;



extern uint8_t jiaoduh,jiaodul,zhuansuh,zhhuansul,shiji_currenth,shiji_currentl,temp,null,std;
extern u16 Fspeed_fdb,Uspeed_fdb,speed_fdb;
extern float Yp,Yy,REAL_YP,REAL_YY;//
extern int fenzhuang,y;
extern float M1,M2,M3,M4,M5,quanshu;
extern int friction_quanshu;//Ħ����ת�Ĵ���
extern float YAW;    //yaw���ʱ���
extern float stastic_yaw_angle;
extern int BAIDONG_YAW_flag;
extern u8 flag_power_over;
extern u8 usart3_power[4],usart3_rest[2];//һ֡���ݽ�����ϱ�־λ
extern float shiji_current[4],power_fdb;
extern int f,old_f;
extern float CM1Speed_x,CM2Speed_x,CM3Speed_x,CM4Speed_x;
extern float CM1Speed_y,CM2Speed_y,CM3Speed_y,CM4Speed_y;
extern float CM1Speed_spin,CM2Speed_spin,CM3Speed_spin,CM4Speed_spin;


extern u16 x_coordinate;  
extern u16 y_coordinate; 
extern u8 coordinate_flag;


extern u8 uart_data[9];   //���ڽ��ܻ���
extern u8  data;        //���ڵ��ν�����������     
extern u8 Rx_Adr;		//���ڽ��յڼ��ֽ�
extern float manifold_y;
extern float manifold_x;  //���Ʒ��Ⱥ��������Ƭ��������

extern float XC;
extern float YC;



extern u8 shift_flag;





#endif
