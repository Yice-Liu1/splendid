#include "main.h"
extern int fiction_time;//2006定时器计数变量
char ssh=0;
void TIM2_Config(u16 Prescaler)
{
	TIM_TimeBaseInitTypeDef tim;
	NVIC_InitTypeDef nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	tim.TIM_Period=Prescaler;
	tim.TIM_Prescaler=8400;//0.5s
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	tim.TIM_ClockDivision=TIM_CKD_DIV1;;
	TIM_TimeBaseInit(TIM2,&tim);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	
	nvic.NVIC_IRQChannel=TIM2_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 2;
  nvic.NVIC_IRQChannelSubPriority = 0;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
}


void TIM2_Start(void)
{
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
	 TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM2, ENABLE);	 
//  TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
 // TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
}

void TIM2_Stop(void)
{
  TIM_Cmd(TIM2, DISABLE);	 
	//TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
  TIM_ITConfig(TIM2, TIM_IT_Update,DISABLE);
//	TIM_Cmd(TIM2, DISABLE);	 
	TIM_SetCounter(TIM2,0);
 TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}
void TIM6_Config(void)
{
	TIM_TimeBaseInitTypeDef tim;
	NVIC_InitTypeDef nvic;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	nvic.NVIC_IRQChannel=TIM6_DAC_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 1;
  nvic.NVIC_IRQChannelSubPriority = 0;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
	
	tim.TIM_Period=1000-1;
	tim.TIM_Prescaler=84-1;
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	tim.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM6,&tim);
}

void TIM6_Start(void)
{
  TIM_Cmd(TIM6, ENABLE);	 
  TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);
  TIM_ClearFlag(TIM6, TIM_FLAG_Update);	
}
void TIM6_Stop(void)
{
  TIM_Cmd(TIM6, ENABLE);	 
  TIM_ITConfig(TIM6, TIM_IT_Update,DISABLE);
  TIM_ClearFlag(TIM6, TIM_FLAG_Update);	
}

////////////void TIM2_IRQHandler(void)
////////////{
////////////		if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //????
////////////	{  	cxgkey=1; 
//////////////		if(ssh==0)
//////////////		{
//////////////			RED_LED_OFF();
//////////////			ssh=1;
//////////////			cxgkey=1;
//////////////		}
//////////////		else
//////////////		{
//////////////			RED_LED_ON();
//////////////			ssh=0;
//////////////			cxgkey=1;
//////////////		}
////////////	}  
////////////	TIM_ClearITPendingBit(TIM2,TIM_IT_Update); 
//////////////	 if (TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET) 
//////////////	 {
//////////////			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//////////////      TIM_ClearFlag(TIM2, TIM_FLAG_Update);
//////////////		  
//////////////	 }
////////////}

void TIM2_IRQHandler(void)
{
	
	 if (TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET) 
	 {
		  
		 	TIM2_Stop();
		  mix++;
		  iii++;
	
	 }
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}



//主任务中断 频率1K
void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)!= RESET) 
	{
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	//IMU_PID();
		
	
	}
}

uint32_t Get_Time_Micros(void)
{
	return TIM2->CNT;
}
