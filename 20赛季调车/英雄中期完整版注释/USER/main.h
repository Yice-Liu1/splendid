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
#include "led.h"
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
#include "mpu6500_reg.h"
#include "IST8310_reg.h"
extern uint8_t jiaoduh,jiaodul,zhuansuh,zhhuansul,shiji_currenth,shiji_currentl,temp,null,std;
extern u16 Fspeed_fdb,Uspeed_fdb,speed_fdb;
extern float Yp,Yy,REAL_YP,REAL_YY;//
extern int fenzhuang,y;
extern float M1,M2,M3,M4,M5,M6,M2_5,M2_6;
extern int friction_quanshu;//摩擦轮转的次数
extern float YAW;    //yaw自问变量
extern float stastic_yaw_angle;
extern int BAIDONG_YAW_flag;
extern u8 flag_power_over;
extern u8 usart3_power[4],usart3_rest[2];//一帧数据接受完毕标志位
extern float shiji_current[4],power_fdb;
extern int f,old_f;
extern float CM1Speed_x,CM2Speed_x,CM3Speed_x,CM4Speed_x;
extern float CM1Speed_y,CM2Speed_y,CM3Speed_y,CM4Speed_y;
extern float CM1Speed_spin,CM2Speed_spin,CM3Speed_spin,CM4Speed_spin;
extern float rotationangle;
extern float power;
extern int rest_power;
extern int16_t	Pitch_Fina_Out;
extern int16_t	Yaw_Fina_Out;
extern PID_Regulator_t ShootbolunPID;
#endif
