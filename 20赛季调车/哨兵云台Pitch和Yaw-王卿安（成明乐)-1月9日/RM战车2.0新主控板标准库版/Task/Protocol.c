#include "main.h"

u8 data_to_send[50];
flag PID_Flag;
uint32_t Upload_Speed = 10;   //数据上传速度  单位 Hz
#define upload_time (1000000/Upload_Speed)  //计算上传的时间。单位为us
uint32_t system_micrsecond;   //系统时间 单位ms

u8 TxBuffer[256];
u8 TxCounter=0;
u8 count=0;
u8 Rx_Act=0;		//正在使用的buf号
u8 Rx_Adr=0;		//正在接收第几字节
u8 Rx_Ok0 = 0;
u8 Rx_Ok1 = 0;

void Send_Status(void)
{
	u8 _cnt=0;
	vs16 _temp;
	vs32 _temp2 = 0;
	u8 _temp3;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	_temp = (int)(pitch_angle*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(roll_angle*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(yaw_angle*100); // ALT
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	_temp3 = 0 ;  //FLY_MODE
	data_to_send[_cnt++]=_temp3;
	_temp3 = 0 ; //ARMED
	data_to_send[_cnt++]=_temp3;
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;	
	
	Uart3_Put_Buf(data_to_send,_cnt);
}



void Send_Sensor(void)
{
	u8 _cnt=0;
	vs16 _temp;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
//	_temp = MPU6050_Raw_Data.Accel_X;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = MPU6050_Raw_Data.Accel_Y;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = MPU6050_Raw_Data.Accel_Z;	
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = MPU6050_Raw_Data.Gyro_X;	
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = MPU6050_Raw_Data.Gyro_Y;	
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = MPU6050_Raw_Data.Gyro_Z;	
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = MPU6050_Raw_Data.Mag_X;	
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = MPU6050_Raw_Data.Mag_Y;	
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = MPU6050_Raw_Data.Mag_Z;	
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;	
	
	Uart3_Put_Buf(data_to_send,_cnt);
}



void SendRCData(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(RC_CtrlData.rc.ch3);
	data_to_send[_cnt++]=BYTE0(RC_CtrlData.rc.ch3);
	data_to_send[_cnt++]=BYTE1(RC_CtrlData.rc.ch2);
	data_to_send[_cnt++]=BYTE0(RC_CtrlData.rc.ch2);
	data_to_send[_cnt++]=BYTE1(RC_CtrlData.rc.ch0);
	data_to_send[_cnt++]=BYTE0(RC_CtrlData.rc.ch0);
	data_to_send[_cnt++]=BYTE1(RC_CtrlData.rc.ch1);
	data_to_send[_cnt++]=BYTE0(RC_CtrlData.rc.ch1);
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=RC_CtrlData.rc.s1;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=RC_CtrlData.rc.s2;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;	
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
  Uart3_Put_Buf(data_to_send,_cnt);	
}



void Send_Moto_Rmp(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x06;
	data_to_send[_cnt++]=0;	
	data_to_send[_cnt++]=BYTE1(Motor1_Measure.speed_rpm);
	data_to_send[_cnt++]=BYTE0(Motor1_Measure.speed_rpm);
	data_to_send[_cnt++]=BYTE1(Motor2_Measure.speed_rpm);
	data_to_send[_cnt++]=BYTE0(Motor2_Measure.speed_rpm);
	data_to_send[_cnt++]=BYTE1(Motor3_Measure.speed_rpm);
	data_to_send[_cnt++]=BYTE0(Motor3_Measure.speed_rpm);
	data_to_send[_cnt++]=BYTE1(Motor4_Measure.speed_rpm);
	data_to_send[_cnt++]=BYTE0(Motor4_Measure.speed_rpm);

	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
  Uart3_Put_Buf(data_to_send,_cnt);		
}



void Send_PID1(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10;
	data_to_send[_cnt++]=0;
	
	vs16 _temp;
	_temp = GMPPositionPID.kp;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = GMPPositionPID.ki;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = GMPPositionPID.kd;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = GMPSpeedPID.kp;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = GMPSpeedPID.ki;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = GMPSpeedPID.kd;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = GMYPositionPID.kp;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = GMYPositionPID.ki;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = GMYPositionPID.kd;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart3_Put_Buf(data_to_send,_cnt);		
}

void Send_PID2(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x11;
	data_to_send[_cnt++]=0;
	
	vs16 _temp;
	_temp = GMYSpeedPID.kp;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = GMYSpeedPID.ki;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = GMYSpeedPID.kd;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = CMRotatePID.kp;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = CMRotatePID.ki;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = CMRotatePID.kd;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = CM1SpeedPID.kp;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = CM1SpeedPID.ki;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = CM1SpeedPID.kd;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart3_Put_Buf(data_to_send,_cnt);		
}

void Send_PID5(void)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x14;
	data_to_send[_cnt++]=0;
	
	vs16 _temp;
	_temp = GMPitchEncoder.raw_value&0xff;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = GMPitchEncoder.ecd_bias&0xff;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = GMYawEncoder.raw_value&0xff;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = GMYawEncoder.ecd_bias&0xff;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart3_Put_Buf(data_to_send,_cnt);		
}

static AppParam_t calibrate ;

void Data_Receive_Anl(u8 *data_buf,u8 num)
{
		u8 sum = 0;
	for(u8 i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		//判断sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//判断帧头
	
	if(*(data_buf+2)==0X01)
	{
		if(*(data_buf+4)==0X02)
		{
			SetCaliCmdFlag(CALI_END_FLAG_GYRO);
			return;
		}
		if(*(data_buf+4)==0X04)
		{
		//	SetCaliCmdFlag(CALI_START_FLAG_MAG);
			SetCaliCmdFlag(CALI_END_FLAG_MAG);	
      return;			
		}
		if(*(data_buf+4)==0X05)
		{
		//	SetCaliCmdFlag(CALI_START_FLAG_MAG);
			SetCaliCmdFlag(CALI_END_FLAG_GIMBAL);	
      return;			
		}
		return;
	}
	
	if(*(data_buf+2)==0X02)
	{
		if(*(data_buf+4)==0X01)//上位机获取PID命令
		{
			PID_Flag.SendPID1 = 1;
			PID_Flag.SendPID2 = 1;
			PID_Flag.SendPID5 = 1;
			return;
		}
		return;
	}
	
	if(*(data_buf+2)==0X10)	
	{
		calibrate.PitchPositionPID.kp_offset = (vs16)(*(data_buf+4)<<8)|*(data_buf+5);
		calibrate.PitchPositionPID.ki_offset = (vs16)(*(data_buf+6)<<8)|*(data_buf+7);
		calibrate.PitchPositionPID.kd_offset = (vs16)(*(data_buf+8)<<8)|*(data_buf+9);
		calibrate.PitchSpeedPID.kp_offset = (vs16)(*(data_buf+10)<<8)|*(data_buf+11);
		calibrate.PitchSpeedPID.ki_offset = (vs16)(*(data_buf+12)<<8)|*(data_buf+13);
		calibrate.PitchSpeedPID.kd_offset = (vs16)(*(data_buf+14)<<8)|*(data_buf+15);
		calibrate.YawPositionPID.kp_offset = (vs16)(*(data_buf+16)<<8)|*(data_buf+17);
		calibrate.YawPositionPID.ki_offset = (vs16)(*(data_buf+18)<<8)|*(data_buf+19);
		calibrate.YawPositionPID.kd_offset = (vs16)(*(data_buf+20)<<8)|*(data_buf+21);
		Send_Check(0x10,sum);
    return;		
	}
	
	if(*(data_buf+2)==0X11)	
	{
		calibrate.YawSpeedPID.kp_offset = (vs16)(*(data_buf+4)<<8)|*(data_buf+5);
		calibrate.YawSpeedPID.ki_offset = (vs16)(*(data_buf+6)<<8)|*(data_buf+7);
		calibrate.YawSpeedPID.kd_offset = (vs16)(*(data_buf+8)<<8)|*(data_buf+9);
		calibrate.RotateSpeedPID.kp_offset = (vs16)(*(data_buf+10)<<8)|*(data_buf+11);
		calibrate.RotateSpeedPID.ki_offset = (vs16)(*(data_buf+12)<<8)|*(data_buf+13);
		calibrate.RotateSpeedPID.kd_offset = (vs16)(*(data_buf+14)<<8)|*(data_buf+15);
		calibrate.ChassisSpeedPID.kp_offset = (vs16)(*(data_buf+16)<<8)|*(data_buf+17);
		calibrate.ChassisSpeedPID.ki_offset = (vs16)(*(data_buf+18)<<8)|*(data_buf+19);
		calibrate.ChassisSpeedPID.kd_offset = (vs16)(*(data_buf+20)<<8)|*(data_buf+21);
		Send_Check(0x11,sum);
		PIDCaliProcess(&calibrate);   //保存calidata到static变量中
    SetCaliCmdFlag(CALI_FLAG_PID);	//PID参数			
    return;				
	}
		if(*(data_buf+2)==0X14)	
	{
		Send_Check(0x14,sum);
    return;		
	}
}

void Send_Check(u8 check ,u8 sum1)
{
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEF;
	data_to_send[3]=2;
	data_to_send[4]=check;
	data_to_send[5]=sum1;

	
	u8 sum = 0;
	for(u8 i=0;i<6;i++)
		sum += data_to_send[i];
	
	data_to_send[6]=sum;

	Uart3_Put_Buf(data_to_send,7);		
}
void Data_Event(void)
{
	if(Rx_Ok1) 
	 {
	 	 Rx_Ok1 = 0;
	   Data_Receive_Anl(Rx_Buf[1],Rx_Buf[1][3]+5);
	 }
	else if(Rx_Ok0)
	 {
	 	 Rx_Ok0 = 0;
		 Data_Receive_Anl(Rx_Buf[0],Rx_Buf[0][3]+5);
	 }	
	if(PID_Flag.SendPID1 == 1 )	 
	 {
		 PID_Flag.SendPID1 = 0;
		 Send_PID1();
	 }
	if(PID_Flag.SendPID2 == 1 )	 
	 {
		 PID_Flag.SendPID2 = 0;
		 Send_PID2();
	 }
	if(PID_Flag.SendPID5 == 1 )	 
	 {
		 PID_Flag.SendPID5 = 0;
		 Send_PID5();
	 }
}



void Dataframe_Process(u8 com_data)       //被cxg改写，，可以从别的程序复制回来
{
	if(Rx_Adr==0)		//寻找帧头0XAAAF
			{
				
				if(com_data==0xFF)	
				{
					uart_data[0] = com_data;
					GREEN_LED_ON();
					RED_LED_OFF();
					Rx_Adr=1;
				}
				else
					Rx_Adr = 0;
			}
	
			 else if(Rx_Adr==1)		//数据
			{
				uart_data[Rx_Adr] = com_data;
				Rx_Adr++;
			}
					 else if(Rx_Adr==2)		//数据
			{
				
					uart_data[Rx_Adr] = com_data;
				Rx_Adr++;
			}
					 else if(Rx_Adr==3)		//数据
			{
				
					uart_data[Rx_Adr] = com_data;
				Rx_Adr++;
			}
					 else if(Rx_Adr==4&&com_data!=0XFE)		//数据
			{
				
					uart_data[Rx_Adr] = com_data;
				Rx_Adr++;
			}
					 else if(Rx_Adr==5)		//数据
			{
				
					uart_data[Rx_Adr] = com_data;
				Rx_Adr++;
			}
					 else if(Rx_Adr==6)		//数据
			{
				
					uart_data[Rx_Adr] = com_data;
				Rx_Adr++;
			}
							 else if(Rx_Adr==7)		//数据
			{
				
					uart_data[Rx_Adr] = com_data;
				Rx_Adr++;
			}
			
				else if(Rx_Adr==8)		//寻找帧头0XAAAF
			{	
				uart_data[Rx_Adr] = com_data;
				RED_LED_ON();
				GREEN_LED_OFF();
				x_coordinate=((uart_data[1] << 8)|uart_data[0]);
				y_coordinate=((uart_data[3] << 8)|uart_data[2]);
		
//USART_SendData(USART2,uart_data[4]);       //uart_data[1]实际是第二个数据
				//	USART_SendData(USART2,0X);
					USART_SendData(USART2,uart_data[4]);
			//		USART_SendData(USART2,uart_data[1]);
			//		USART_SendData(USART2,uart_data[2]);
			//		USART_SendData(USART2,uart_data[3]);
					//(USART2,"Q");
				Rx_Adr=0;	
			}
}
			


void Protocol(void)
{
	if((Get_Time_Micros() - system_micrsecond) > upload_time)
		{
			system_micrsecond = Get_Time_Micros();
			Send_Sensor();
			Send_Status();
			Send_PID5();
			SendRCData();
			Send_Moto_Rmp();
			delay_ms(1);
		}
		Data_Event();
}
