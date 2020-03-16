#include "main.h"
//add priority to error
LostCounter_t lost_counter[LOST_COUNTER_NUM] = { 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,2000};
const LostCounter_t lost_counter_len[LOST_COUNTER_NUM] = {20,200, 500, 500, 500, 500, 500, 500, 500, 500, 100};


void Can_Error_Process(Can_Info *can_info);

uint32_t lost_err = 0xffff;     //每一位代表一个错误

void SuperviseTask()
{
	int i = 0;
	if(Is_AppParam_Calied())   //校准过，则喂狗
	{
		LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_NOCALI));
	}
	//step 1;check the error 
	for(i = 0; i < LOST_COUNTER_NUM; i++)
	{
		if(LostCounterOverflowCheck(lost_counter[i], lost_counter_len[i]) == 0)   
		{
			LostCounterCount(&lost_counter[i], 1);   
			lost_err &= ~(uint32_t)(1 << i);    
		}
		else
		{
			lost_err |= (uint32_t)(1 << i);   
		}
	}
	
	
	
	if(Get_Lost_Error(LOST_ERROR_NOCALI) == LOST_ERROR_NOCALI)
	{
		LED_TOGGLE(0,6,0,0,4);  
	}	
	if(Get_Lost_Error(LOST_ERROR_IMU) == LOST_ERROR_IMU)
	{
		LED_TOGGLE(0,6,0,1,4);
	}
	else if(Get_Lost_Error(LOST_ERROR_ZGYRO) == LOST_ERROR_ZGYRO)
	{
		LED_TOGGLE(0,7,5,10,4);
	}
	else if(Get_Lost_Error(LOST_ERROR_MOTOR6) == LOST_ERROR_MOTOR6)
	{
		LED_TOGGLE(0,5,3,6,4);
	}	
	else if(Get_Lost_Error(LOST_ERROR_MOTOR5) == LOST_ERROR_MOTOR5)
	{
		LED_TOGGLE(0,5,3,5,4);
	}	
	else if(Get_Lost_Error(LOST_ERROR_MOTOR1) == LOST_ERROR_MOTOR1)
	{
		LED_TOGGLE(0,3,2,1,4);
	}
	else if(Get_Lost_Error(LOST_ERROR_MOTOR2) == LOST_ERROR_MOTOR2)
	{
		LED_TOGGLE(0,4,2,2,4);
	}
	else if(Get_Lost_Error(LOST_ERROR_MOTOR3) == LOST_ERROR_MOTOR3)
	{
		LED_TOGGLE(0,5,3,3,4);
	}
	else if(Get_Lost_Error(LOST_ERROR_MOTOR4) == LOST_ERROR_MOTOR4)
	{
		LED_TOGGLE(0,5,3,4,4);
	}
	else if(Get_Lost_Error(LOST_ERROR_RC) == LOST_ERROR_RC)
	{
		LED_TOGGLE(1,5,5,5,4);
	}
	else if(Get_Lost_Error(LOST_ERROR_DEADLOCK) == LOST_ERROR_DEADLOCK)
	{
		LED_TOGGLE(0,6,6,6,4);
	}
	else
	{
		LED_TOGGLE(1,6,6,0,4);   
	}
}

void Set_Error_Flag()
{
	
}

uint32_t *GetLostCounter(uint8_t index)
{
    if(index < LOST_COUNTER_NUM)
    {
        return &lost_counter[index];
    }
    else
    {
        return ((void *)0);
    } 
}

uint32_t Get_Lost_Error(uint32_t err_code)
{
	return lost_err&err_code;
}


uint8_t Is_Lost_Error_Set(uint32_t err_code)
{
	return Get_Lost_Error( err_code) == err_code;
}

uint8_t Is_Serious_Error()
{
	//return (Is_Lost_Error_Set(LOST_ERROR_IMU) || Is_Lost_Error_Set(LOST_ERROR_DEADLOCK)||Is_Lost_Error_Set(LOST_ERROR_ZGYRO) || Is_Lost_Error_Set(LOST_ERROR_NOCALI));
	return 0;
}



void LED_TOGGLE(uint8_t led, uint16_t CYCLE_S, uint16_t TOGGLE_S, uint16_t shine_times,uint16_t TIME_SPAN)
{
	    static uint32_t timeSpanCount = 0;
		static uint32_t tempCount = 0;
		static uint32_t sec = 0;
		uint32_t sec_cycle = 0;   
		
		timeSpanCount++;  //以时间间隔计数
		sec = timeSpanCount/(1000/TIME_SPAN);   
		sec_cycle = sec % CYCLE_S;  
		
		
		if(sec_cycle/TOGGLE_S == 0)   
		{   
				tempCount++;
				if(shine_times <=0 )    
				{
					if(led == 0)
					{
						RED_LED_ON();
						GREEN_LED_OFF();	
					}
					else
					{
						GREEN_LED_ON();
						RED_LED_OFF();	
					}
				}
			    else if(tempCount%(((1000/TIME_SPAN)*TOGGLE_S)/(2*shine_times)) == 0)     //tempCount可以被分母整除，就闪灯
				{
					if(led == 0)
					{
						RED_LED_TOGGLE();
						GREEN_LED_OFF();	
					}
					else
					{
						GREEN_LED_TOGGLE();
						RED_LED_OFF();	
					}
				}			
		}
		else                          
		{
				tempCount = 0;
				RED_LED_OFF();
				GREEN_LED_OFF();
		}	
}

