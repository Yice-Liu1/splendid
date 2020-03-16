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



#include "gimbalpid.h"
#include "ding.h"

/*************************************************************************************/

#include "JY901.h"

extern struct STime		stcTime;
extern struct SAcc 		stcAcc;
extern struct SGyro 		stcGyro;
extern struct SAngle 	stcAngle;
extern struct SMag 		stcMag;
extern struct SDStatus stcDStatus;
extern struct SPress 	stcPress;
extern struct SLonLat 	stcLonLat;
extern struct SGPSV 		stcGPSV;
extern struct SQ       stcQ;


/***********************************************************/

extern uint8_t jiaoduh,jiaodul,zhuansuh,zhhuansul,shiji_currenth,shiji_currentl,temp,null,std;
extern u16 Fspeed_fdb,Uspeed_fdb,speed_fdb;
extern float Yp,Yy,REAL_YP,REAL_YY;//
extern int fenzhuang,y;
extern float M1,M2,M3,M4,M5,quanshu;

extern float yaw_wish;
extern float pitch_wish;

extern u8 tenshoot;
extern short int upcount;

extern u8 DING;

extern short int Pitch_End_Out;
extern short int Yaw_End_Out;

#endif
