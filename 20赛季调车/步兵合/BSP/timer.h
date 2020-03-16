#ifndef __TIMER_H
#define __TIMER_H

#include "main.h"

void TIM2_Config(void);
void TIM6_Config(void);
void TIM6_Start(void);
void TIM4_Config(void);
void TIM6_STOP(void);
uint32_t Get_Time_Micros(void);

#endif
