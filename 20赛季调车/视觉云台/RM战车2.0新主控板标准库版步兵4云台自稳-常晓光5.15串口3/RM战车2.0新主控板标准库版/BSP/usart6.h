#ifndef __USART6_H
#define __USART6_H

#include <stdio.h>
#include "stm32f4xx.h"

extern u8 Rx_Buf[2][32];
void USART6_Config(void);
void Uart6_Put_Buf(unsigned char *DataToSend , u8 data_num);

#endif
