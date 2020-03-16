#ifndef __DING_H
#define __DING_H	 
#include "stm32f4xx.h" 

/*下面的方式是通过直接操作库函数方式读取IO*/
//#define  DING  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_1)  //PF1

void Ding_Init(void);	//IO初始化


#endif
