#ifndef __BSP_H
#define __BSP_H
#include "timer.h"
#include "delay.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "can1.h"
#include "can2.h"
#include "flash.h"
#include "led.h"
#include "gun.h"
#include "encoder.h"
#include "spi.h"


#define     BSP_USART1_RX_BUF_SIZE            128u
#define     BSP_USART3_RX_BUF_SIZE            1024u
#define     BSP_USART3_TX_BUF_SIZE            512u

void BSP_Init(void);
#endif

