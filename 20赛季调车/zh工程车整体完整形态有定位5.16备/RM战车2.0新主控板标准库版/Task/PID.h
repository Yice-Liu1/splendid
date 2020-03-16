#ifndef __PID_H
#define __PID_H
#include "stm32f4xx.h"

typedef struct PID_Regulator_t
{
	float ref;//期望值
	float fdb;//反馈值
	 float err[3];//0：当前，1；前一次，2前前
	float kp;
	float ki;
	float kd;
	float componentKp;
	float componentKi;
	float componentKd;
	float componentKpMax;
	float componentKiMax;
	float componentKdMax;
	float output;
	float outputMax;
	float kp_offset;
	float ki_offset;
	float kd_offset;
	void (*Calc)(struct PID_Regulator_t *pid);//函数指针
	void (*Reset)(struct PID_Regulator_t *pid);
}PID_Regulator_t;
void PID_Reset(PID_Regulator_t *pid);
void PID_Calc(PID_Regulator_t *pid);
void POSITION_PID_Calc(PID_Regulator_t *pid);
void xxx(int i);
#endif
