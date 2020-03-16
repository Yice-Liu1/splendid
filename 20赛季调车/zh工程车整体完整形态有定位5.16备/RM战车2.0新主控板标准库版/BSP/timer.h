#ifndef __TIMER_H
#define __TIMER_H

#include "main.h"

void TIM2_Config(u16 Prescaler);
void TIM6_Config(void);
void TIM6_Start(void);
void TIM2_Start(void);
void TIM6_STOP(void);
void TIM2_STOP(void);
uint32_t Get_Time_Micros(void);

#endif
