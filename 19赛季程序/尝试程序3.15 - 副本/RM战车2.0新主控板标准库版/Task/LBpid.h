#ifndef __LBPID_H
#define __LBPID_H

#include "stm32f4xx.h"

typedef struct{
	float Pitch;
	float Yaw;
	float GYRO_Pitch;
	float GYRO_Yaw;
	float Pitch_Encoder_Angle;
	float Yaw_Encoder_Angle;
}PID;

extern 	PID		PID_Input; 

void Gimbal_Set(float RC_Pitch,float RC_Yaw,float GYRO_Pitch,float GYRO_Yaw,float Pitch_Encoder_Angle,float Yaw_Encoder_Angle);
float imu_PID(float RC_Pitch_angle,float Pitch_angle);

#endif



