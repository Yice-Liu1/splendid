#include "main.h"

float err[3],chazhi;

float ml; 
float cbw;
int CM1,CM2,CM3,CM4;


int main(void)
{   			


//SetFrictionWheelSpeed(1500);
  BSP_Init();	
		USART2_Config(); 
//	PWM_Config();
//  YAW(1450);//1450//1500
//	PITCH(1500);
////	SetFrictionWheelSpeed(0)
//	PID_Reset(&GMPPositionPID);
//	PID_Reset(&GMYPositionPID);
//	//SetFrictionWheelSpeed(800);
//	DOOR_Init();
//	delay_ms(300);
//	EDOOR1_CLOSE();
//	EDOOR2_CLOSE();
//	EDOOR3_CLOSE();
//	EDOOR4_CLOSE();
//	EDOOR5_CLOSE();
	
	while(1)	//1450ÖÐ£¬2000ÏÂ//
	{	//1450//1400//
		//1450//2000
		//2150//1400
	//	PITCH(cbw);//2000
//YAW(ml);//1450
//	 remote();
//		//Set_CM_Speed(CAN1,CM1,CM2,CM3,CM4);
//		//MOTORPID(CM1SpeedPID.ref,CM2SpeedPID.ref,CM3SpeedPID.ref,CM4SpeedPID.ref);
//	//	HANDPID(ml);
//		delay_ms(1);
		//HANDPID(-100);
		//Set_GEThand(CAN2,cbw);
//		delay_ms(1);
		 //HANDPID(-500);
//		MOTORPID(2000,CM2SpeedPID.ref,CM3SpeedPID.ref,CM4SpeedPID.ref);	
	
	}

}

