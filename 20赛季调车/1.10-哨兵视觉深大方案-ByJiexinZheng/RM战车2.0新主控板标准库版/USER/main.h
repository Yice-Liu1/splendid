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
#include "usart6.h"
#define MAKEWORD(a,b)  ((unsigned)(((unsigned char)(a)) | ((unsigned)((unsigned char)(b))) << 8))
#define MAKELONG(a,b)  ((long)(((unsigned)(a)) | ((unsigned int)((unsigned)(b)))<<16))
extern uint8_t jiaoduh,jiaodul,zhuansuh,zhhuansul,shiji_currenth,shiji_currentl,temp,null,std;
extern u16 Fspeed_fdb,Uspeed_fdb,speed_fdb;
extern float Yp,Yy,REAL_YP,REAL_YY;//
extern int fenzhuang,y;
extern float M1,M2,M3,M4,M5,quanshu;
extern float err[3],chazhi;
extern int friction_quanshu;//Ħ����ת�Ĵ���
extern float TEMP[6];
extern int cxgkey;
extern float YAW;    //yaw���ʱ���
extern float stastic_yaw_angle;
extern int BAIDONG_YAW_flag;

extern u16 x_coordinate;  
extern u16 y_coordinate; 
extern u8 coordinate_flag;


extern u8 uart_data[9];   //���ڽ��ܻ���
extern u8  data;        //���ڵ��ν�����������     
extern u8 Rx_Adr;		//���ڽ��յڼ��ֽ�
extern float manifold_y;
extern float manifold_x;  //���Ʒ��Ⱥ��������Ƭ��������

void USART6_Config(void);

#endif
