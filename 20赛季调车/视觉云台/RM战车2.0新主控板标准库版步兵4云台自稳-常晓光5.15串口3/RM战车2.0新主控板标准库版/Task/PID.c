#include "main.h"

void PID_Reset(PID_Regulator_t *pid)
{
	pid->err[0]=0;
	pid->err[1]=0;
	pid->err[2]=0;
	pid->output=0;
}


void PID_Calc(PID_Regulator_t *pid)
{

	pid->err[0]=pid->err[1];//err[1]��err[0]
	pid->err[1]=pid->ref-pid->fdb;
	pid->err[2]+=pid->err[1];
	VAL_LIMIT(pid->ki,-pid->componentKiMax,pid->componentKiMax);//����ki��Χ
	pid->output=pid->kp*pid->err[1]+pid->ki*pid->err[2]+pid->kd*(pid->err[1]-pid->err[0]);//pid�ļ��㹫ʽ	
	VAL_LIMIT(pid->output,-pid->outputMax,pid->outputMax);
}
		void xxx(int i)
		{
			i=0;
		}


