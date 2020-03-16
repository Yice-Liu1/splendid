#ifndef __USART3_H
#define __USART3_H

#include <stdio.h>
#include "stm32f4xx.h"

extern u8 Rx_Buf[2][32];
void USART3_Config(void);
void Uart3_Put_Buf(unsigned char *DataToSend , u8 data_num);

#endif
