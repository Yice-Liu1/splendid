#include "main.h"

float p_kp=60,p_ki=0,p_kd=800;
float s_kp=2.5,s_ki=0,s_kd=0;
float position_output,speed_p_old,speed_p,speed_i;
void PID_Reset(PID_Regulator_t *pid)
{
	pid->err[0]=0;
	pid->err[1]=0;
	pid->err[2]=0;
	pid->output=0;
}


void PID_Calc(PID_Regulator_t *pid)
{

	pid->err[0]=pid->err[1];//err[1]给err[0]
	pid->err[1]=pid->ref-pid->fdb;
	pid->err[2]+=pid->err[1];
	VAL_LIMIT(pid->ki,-pid->componentKiMax,pid->componentKiMax);//限制ki范围
	pid->output=pid->kp*pid->err[1]+pid->ki*pid->err[2]+pid->kd*(pid->err[1]-pid->err[0]);//增量pid的计算公式	
	//pid->output=hp*pid->err[1]+hi*pid->err[2]+hd*(pid->err[1]-pid->err[0]);
	VAL_LIMIT(pid->output,-pid->outputMax,pid->outputMax);
}


void POSITION_PID_Calc(PID_Regulator_t *pid)
{
	float speed_fdb;
	if(Std==0x201)
	{
	speed_fdb=M6;
	}
		if(Std==0x202)
	{
	speed_fdb=M7;
	}
  pid->err[0]=pid->err[1];//err[1]给err[0]
	pid->err[1]=pid->ref-pid->fdb;
	pid->err[2]+=pid->err[1];
	//VAL_LIMIT(pid->ki,-pid->componentKiMax,pid->componentKiMax);//限制ki范围
	//pid->output=pid->kp*pid->err[1]+pid->ki*pid->err[2]+pid->kd*(pid->err[1]-pid->err[0]);//增量pid的计算公式	
	//pid->output=p_kp*pid->err[1]+p_ki*pid->err[2]+p_kd*(pid->err[1]-pid->err[0]);
	
	position_output=p_kp*pid->err[1]+p_ki*pid->err[2]+p_kd*(pid->err[1]-pid->err[0]);

	//	if(position_output>9000) position_output=9000;
//	if(position_output<-9000) position_output=-9000;
	
	//VAL_LIMIT(pid->output,-pid->outputMax,pid->outputMax);
//	
speed_p_old=speed_p;
	speed_p=position_output-speed_fdb;
	speed_i+=speed_p;
	pid->output=(s_kp*speed_p+s_ki*speed_i+s_kd*(speed_p-speed_p_old));
	
	VAL_LIMIT(pid->output,-pid->outputMax,pid->outputMax);
	if(pid->output>5000) pid->output=5000;

	if(pid->output<-5000) pid->output=-5000;
   VAL_LIMIT(pid->output,-pid->outputMax,pid->outputMax);
}

		void xxx(int i)
		{
			i=0;
		}


