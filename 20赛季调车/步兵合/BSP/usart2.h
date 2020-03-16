////#ifndef __USART2_H
////#define __USART2_H

////#include <stdio.h>
////#include "stm32f4xx.h"

////extern u8 usart2_rcvfns;
////extern u8 usart2_rcvbuf[30];

//////extern u8 Rx_Buf2[2][32];
////void USART2_Config(void);
////void Uart2_Put_Buf(unsigned char *DataToSend , u8 data_num);
//////#define USART_REC_LEN  			1  	//定义最大接收字节数 200
//////#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
////	  	
//////extern u16  USART_TX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//////extern u16 USART_RX_STA;         		//接收状态标记	
////#endif


#ifndef __USART2_H
#define __USART2_H

#include <stdio.h>
#include "stm32f4xx.h"

extern u8 Rx_Buf2[2][32];
void USART2_Config(void);
void Uart2_Put_Buf(unsigned char *DataToSend , u8 data_num);
void MANIFOLD_DATA_PROCESS(uint8_t *pData);
#define  BSP_USART2_DMA_RX_BUF_LEN               18u 
#define  manifold_FRAME_LENGTH         9u
//#define USART_REC_LEN  			1  	//定义最大接收字节数 200
//#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
//extern u16  USART_TX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//extern u16 USART_RX_STA;         		//接收状态标记	
#endif
