#include "main.h"

void TIM2_Config(void)
{
	TIM_TimeBaseInitTypeDef tim;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	tim.TIM_Period=0xffffffff;
	tim.TIM_Prescaler=168-1;
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	tim.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_TimeBaseInit(TIM2,&tim);
	 
	TIM_Cmd(TIM2,ENABLE);	
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
	tim.TIM_Prescaler=8400-1;
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

void TIM2_IRQHandler(void)
{
	 if (TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET) 
	 {
			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
      TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		  
	 }
}
//主任务中断 频率1K
int flag_ka=0,c=0,miao;
void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)!= RESET) 
	{
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	//IMU_PID();
		miao++;
   if(c>1)
	 {
		 		if((BigShootMotorSpeedPID.output<-3900)&&M8<20&&M8>-20) 
			{
				xue=500;
			}
	 }
	 else xue=0;
		if(miao>9)
		{
			miao=0;

			if(flag_dan)
			{
				if(xue!=500) xue=-2500;
			}

		if(c<4) c++;
		}
		 if(RC_CtrlData.mouse.press_l||s2==2) 
		{
					    if(c==4)
		{
      if(fadanMotorSpeedPID.output>3900&&M7>-20&&M7<20) fadan=-1500;
      else 		fadan=1000;
		}
		}
		else fadan=0;
	
	}
}

uint32_t Get_Time_Micros(void)
{
	return TIM2->CNT;
}
