#ifndef __gimbalpid_H
#define __gimbalpid_H

#include "stm32f4xx.h"


int pitch_pid(float wish_pitch,float angle_pitch,float speed_pitch);
int yaw_pid(float wish_yaw,float angle_yaw,float speed_pitch);
int piture_pid(float picture_angle,float picture_wish);
void  Gimbal_Go(void);


#endif

