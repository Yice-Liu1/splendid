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
#include "door.h"   //cml:电磁阀

extern uint8_t jiaoduh,jiaodul,zhuansuh,zhhuansul,shiji_currenth,shiji_currentl,temp,null,std;
extern u16 Fspeed_fdb,Uspeed_fdb,speed_fdb,Yp,Yy;//
extern int fenzhuang,y,H_fdb,Hcount,diff,last_H_fdb;
extern float M1,M2,M3,M4,M5,M6,M7,M8,M9,M10,quanshu;
extern float err[3],chazhi;
extern int friction_quanshu;//摩擦轮转的次数
extern int mix,ii,iii;
extern volatile Encoder HANDEncoder;
extern int flag_z,flag_y;
//extern float hkp,hki,hkd;
#endif
