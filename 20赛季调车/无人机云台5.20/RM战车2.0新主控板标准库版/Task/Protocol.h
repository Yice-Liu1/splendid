#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include "stm32f4xx.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

typedef struct 
{
	u8 SendPID1;
	u8 SendPID2;
	u8 SendPID5;
} flag;


extern flag PID_Flag;
extern u8 TxBuffer[256];
extern u8 TxCounter;
extern u8 count;
extern u8 Rx_Act;		//正在使用的buf号
extern u8 Rx_Adr;		//正在接收第几字节
extern u8 Rx_Ok0;
extern u8 Rx_Ok1;


void Send_Status(void);
void Send_Sensor(void);
void SendRCData(void);
void Send_Moto_Rmp(void);
void Send_PID1(void);
void Send_PID2(void);
void Send_PID5(void);
void Data_Receive_Anl(u8 *data_buf,u8 num);
void Send_Check(u8 check ,u8 sum1);
void Dataframe_Process(u8 com_data);
void Data_Event(void);
void Protocol(void);

#endif
