#include "main.h"
float TEMP[6];
int cxgkey;
//pitch��Ƕȷ�Χ��6795-5050 yaw �м���4965,3832�����ұߣ�6025�����
//PID_Regulator_t GMPPositionPID;
 float err[3],chazhi;

int main(void)
{   			
    	BSP_Init();	
PWM_Config();
	YAW(1450);//1450//1500
	PITCH(1500);
//	SetFrictionWheelSpeed(1000);
	//delay_ms(2000);
//SetFrictionWheelSpeed(0);
	PID_Reset(&GMPPositionPID);
	PID_Reset(&GMYPositionPID);
		//SetFrictionWheelSpeed(800);
	delay_ms(3000);
	MPU6050_Init();
		IST8310_Init();
	Init_Quaternion();
	while(1)	
	{

		remote();

}
	}