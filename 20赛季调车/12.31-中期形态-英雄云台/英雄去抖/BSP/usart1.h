#ifndef __USART1_H
#define __USART1_H

#include <stdint.h>

#define PITCH_MAX 19.0f
#define YAW_MAX 720.0f//720.0				//cyq:ÔÆÌ¨½Ç¶ÈµÄ·¶Î§
/*
*********************************************************************************************************
*                                               MACROS
*********************************************************************************************************
*/

#define  BSP_USART1_DMA_RX_BUF_LEN               30u                   
 
#define BSP_USART1_RX_BUF_SIZE_IN_FRAMES         (BSP_USART1_RX_BUF_SIZE / RC_FRAME_LENGTH)
#define  RC_FRAME_LENGTH                            18u


void USART1_Config(void);
	
#endif
