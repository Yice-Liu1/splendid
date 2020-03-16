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


#define PI					3.14159265358979f
extern uint8_t jiaoduh,jiaodul,zhuansuh,zhhuansul,shiji_currenth,shiji_currentl,temp,null,std;
extern u16 Fspeed_fdb,Uspeed_fdb,speed_fdb;
extern float Yp,Yy,REAL_YP,REAL_YY;//
extern int fenzhuang,y;
extern float M1,M2,M3,M4,M5,quanshu;
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


extern u16 x_coordinate;  
extern u16 y_coordinate; 
extern u8 coordinate_flag;


extern u8 uart_data[9];   //串口接受缓存
extern u8  data;        //串口单次接收数据内容     
extern u8 Rx_Adr;		//正在接收第几字节
extern float manifold_y;
extern float manifold_x;  //限制幅度后妙算给单片机的坐标

extern float XC;
extern float YC;
extern int time_rotate;
extern float yaw_offset;
extern float yaw_old;
extern float delta_yaw_angle;
extern float yaw_rotate;
extern float yaw;
extern u8 shift_flag;
extern u8 flag_follow;

typedef struct 
{     
	uint8_t quat_euler:1;     
	uint8_t gyro_rangle:3;     
	uint8_t accel_rangle:2;     
	uint8_t imu_sensor_rotation:5;     
	uint8_t ahrs_rotation_sequence:3;     
	int16_t quat[4];     
	float quat_fp32[4];     
	int16_t euler_angle[3];     
	float euler_angle_fp32[3];     
	int16_t gyro_int16[3];     
	int16_t accel_int16[3];     
	int16_t mag_int16[3];     
	float gyro_fp32[3];     
	float accel_fp32[3];     
	uint16_t sensor_time;     
	uint16_t sensor_temperature;     
	int16_t sensor_control_temperature;     
	float gyro_sen;     
	float accel_sen; 
}rm_imu_data_t; 


#endif
