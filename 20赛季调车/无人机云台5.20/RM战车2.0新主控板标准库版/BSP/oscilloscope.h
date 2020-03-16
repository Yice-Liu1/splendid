#ifndef __OSCILLOSCOPE_H
#define __OSCILLOSCOPE_H

#define _BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )
#define _BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define _BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define _BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
#include"main.h"
void usart1_send(u8 data);
void ANO_DT_Send_Data(u8 *dataToSend , u8 length);
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z,s32 bar);
#endif
