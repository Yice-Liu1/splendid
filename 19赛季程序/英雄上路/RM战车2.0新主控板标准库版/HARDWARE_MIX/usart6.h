#ifndef __USART6_H
#define __USART6_H

#include <stdio.h>
#include "stm32f4xx.h"

//extern u8 Rx_Buf2[2][32];
//void USART2_Config(void);
//void Uart2_Put_Buf(unsigned char *DataToSend , u8 data_num);
void USART6_INIT(void);
void CopeSerial2Data(unsigned char ucData);



#endif

