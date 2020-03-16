
#include "main.h"








u8 _data_to_send[50];
void usart1_send(u8 data)
{
USART_SendData(USART2, data);//向串口1发送数据
while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
}

void ANO_DT_Send_Data(u8 *dataToSend , u8 length)
{
u8 i;
	for(i=0;i<length;i++)
	{
	usart1_send(dataToSend[i]);
	}
}
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z,s32 bar)
{
	u8 _cnt=0;
	vs16 _temp;
	u8 i;
	u8 sum=0;
	
	_data_to_send[_cnt++]=0xAA;
	_data_to_send[_cnt++]=0xAA;
	_data_to_send[_cnt++]=0x02;
	_data_to_send[_cnt++]=0;
	
	_temp = a_x;
	_data_to_send[_cnt++]=_BYTE1(_temp);
	_data_to_send[_cnt++]=_BYTE0(_temp);
	_temp = a_y;
	_data_to_send[_cnt++]=_BYTE1(_temp);
	_data_to_send[_cnt++]=_BYTE0(_temp);
	_temp = a_z;	
	_data_to_send[_cnt++]=_BYTE1(_temp);
	_data_to_send[_cnt++]=_BYTE0(_temp);
	
	_temp = g_x;	
	_data_to_send[_cnt++]=_BYTE1(_temp);
	_data_to_send[_cnt++]=_BYTE0(_temp);
	_temp = g_y;	
	_data_to_send[_cnt++]=_BYTE1(_temp);
	_data_to_send[_cnt++]=_BYTE0(_temp);
	_temp = g_z;	
	_data_to_send[_cnt++]=_BYTE1(_temp);
	_data_to_send[_cnt++]=_BYTE0(_temp);
	
	_temp = m_x;	
	_data_to_send[_cnt++]=_BYTE1(_temp);
	_data_to_send[_cnt++]=_BYTE0(_temp);
	_temp = m_y;	
	_data_to_send[_cnt++]=_BYTE1(_temp);
	_data_to_send[_cnt++]=_BYTE0(_temp);
	_temp = m_z;	
	_data_to_send[_cnt++]=_BYTE1(_temp);
	_data_to_send[_cnt++]=_BYTE0(_temp);
	
	_data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += _data_to_send[i];
	_data_to_send[_cnt++] = sum;
	
	ANO_DT_Send_Data(_data_to_send, _cnt);
}
