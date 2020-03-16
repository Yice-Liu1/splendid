#include "main.h"
#include "stdio.h"

static uint32_t can_count = 0;

volatile Encoder CM1Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder CM2Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder CM3Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder CM4Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder GMYawEncoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder GMPitchEncoder = {0,0,0,0,0,0,0,0,0};

Measure Motor1_Measure = {0,0,0,0,0}; 
Measure Motor2_Measure = {0,0,0,0,0}; 
Measure Motor3_Measure = {0,0,0,0,0}; 
Measure Motor4_Measure = {0,0,0,0,0}; 

float CM1Speed_x,CM2Speed_x,CM3Speed_x,CM4Speed_x;
float CM1Speed_y,CM2Speed_y,CM3Speed_y,CM4Speed_y;
float CM1Speed_spin,CM2Speed_spin,CM3Speed_spin,CM4Speed_spin;

float ZGyroModuleAngle = 0.0f;

void GetEncoderBias(volatile Encoder *v, CanRxMsg * msg)
{

            v->ecd_bias = (msg->Data[0]<<8)|msg->Data[1];  //�����ʼ������ֵ��Ϊƫ��  
            v->ecd_value = v->ecd_bias;
            v->last_raw_value = v->ecd_bias;
            v->temp_count++;
}


void EncoderProcess(volatile Encoder *v, CanRxMsg * msg)
{
	int i=0;
	int32_t temp_sum = 0;    
	v->last_raw_value = v->raw_value;
	v->raw_value = (msg->Data[0]<<8)|msg->Data[1];
	v->diff = v->raw_value - v->last_raw_value;
	if(v->diff < -7500)    //���α������ķ���ֵ���̫�󣬱�ʾȦ�������˸ı�
	{
		v->round_cnt++;
		v->ecd_raw_rate = v->diff + 8192;
	}
	else if(v->diff>7500)
	{
		v->round_cnt--;
		v->ecd_raw_rate = v->diff- 8192;
	}		
	
	else
	{
		v->ecd_raw_rate = v->diff;
	}
	//����õ������ı��������ֵ
	v->ecd_value = v->raw_value + v->round_cnt * 8192;
	//����õ��Ƕ�ֵ����Χ���������
	v->ecd_angle = (float)(v->raw_value - v->ecd_bias)*360/8192 + v->round_cnt * 360;
	v->rate_buf[v->buf_count++] = v->ecd_raw_rate;
	if(v->buf_count == RATE_BUF_SIZE)
	{
		v->buf_count = 0;
	}
	//�����ٶ�ƽ��ֵ
	for(i = 0;i < RATE_BUF_SIZE; i++)
	{
		temp_sum += v->rate_buf[i];
	}
	v->filter_rate = (int32_t)(temp_sum/RATE_BUF_SIZE);					
}

void get_measure(Measure *mea,CanRxMsg * msg)
{
	mea->angle = (uint16_t)(msg->Data[0]<<8 | msg->Data[1]);
	mea->real_current = (int16_t)(msg->Data[2]<<8 | msg->Data[3]);
	mea->speed_rpm = mea->real_current;
	mea->given_current = (int16_t)(msg->Data[4]<<8 | msg->Data[5]);
	mea->hall = msg->Data[6];
}

void CanReceiveMsgProcess(CanRxMsg * msg)
{      
        //GMYawEncoder.ecd_bias = yaw_ecd_bias;
        can_count++;
		switch(msg->StdId)
		{
				case CAN_BUS2_MOTOR1_FEEDBACK_MSG_ID:
				{
					LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR1));
					#if CHASSISMOTORTYPE
					 (can_count<=50) ? GetEncoderBias(&CM1Encoder ,msg):EncoderProcess(&CM1Encoder ,msg);       //��ȡ���������ĳ�ʼƫ��ֵ  
          #else
					 get_measure(&Motor1_Measure,msg);
          #endif          
                    
				}break;
				case CAN_BUS2_MOTOR2_FEEDBACK_MSG_ID:
				{
					LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR2));
					#if CHASSISMOTORTYPE
					 (can_count<=50) ? GetEncoderBias(&CM2Encoder ,msg):EncoderProcess(&CM2Encoder ,msg);       //��ȡ���������ĳ�ʼƫ��ֵ  
          #else
					 get_measure(&Motor2_Measure,msg);
          #endif  
					
				}break;
				case CAN_BUS2_MOTOR3_FEEDBACK_MSG_ID:
				{
					LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR3));
					#if CHASSISMOTORTYPE
					 (can_count<=50) ? GetEncoderBias(&CM3Encoder ,msg):EncoderProcess(&CM3Encoder ,msg);       //��ȡ���������ĳ�ʼƫ��ֵ  
          #else
					 get_measure(&Motor3_Measure,msg);
          #endif  
					
				}break;
				case CAN_BUS2_MOTOR4_FEEDBACK_MSG_ID:
				{
					LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR4));
					#if CHASSISMOTORTYPE
					 (can_count<=50) ? GetEncoderBias(&CM4Encoder ,msg):EncoderProcess(&CM4Encoder ,msg);       //��ȡ���������ĳ�ʼƫ��ֵ  
          #else
					 get_measure(&Motor4_Measure,msg);
          #endif  
					
				}break;
				case CAN_BUS2_MOTOR5_FEEDBACK_MSG_ID:
				{
					LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR5));
					 EncoderProcess(&GMYawEncoder ,msg);    
						// �Ƚϱ����������ֵ��ƫ��ֵ�������������ֵ�ͳ�ʼƫ��֮���೬����ֵ����ƫ��ֵ��������ֹ������̨�������˶�
					// if(can_count>=90 && can_count<=100)
					if(GetWorkState() == PREPARE_STATE)   //׼���׶�Ҫ�����֮��Ĳ�ֵһ�����ܴ�����ֵ������϶��ǳ������ٽ��л�
					 {
						 #if CALIBRATION 
							 if((GMYawEncoder.ecd_bias - GMYawEncoder.ecd_value) <-4000)
							 {
								GMYawEncoder.ecd_bias = gAppParamStruct.GimbalCaliData.GimbalYawOffset + 8192;
							 }
							 else if((GMYawEncoder.ecd_bias - GMYawEncoder.ecd_value) > 4000)
							 {
								GMYawEncoder.ecd_bias = gAppParamStruct.GimbalCaliData.GimbalYawOffset - 8192;
							 }
						 #else 
							 if((GMYawEncoder.ecd_bias - GMYawEncoder.ecd_value) <-4000)
							 {
								GMYawEncoder.ecd_bias = gAppParamStruct.GimbalCaliData.GimbalYawOffset + 8192;
								GMYawEncoder.ecd_bias = GIMBAL_BIAS_YAW  + 8192;
							 }
							 else if((GMYawEncoder.ecd_bias - GMYawEncoder.ecd_value) > 4000)
							 {
								 GMYawEncoder.ecd_bias = GIMBAL_BIAS_YAW - 8192;
							 }
						 #endif
					 }
				}break;
				case CAN_BUS2_MOTOR6_FEEDBACK_MSG_ID:
				{
					LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR6));
					EncoderProcess(&GMPitchEncoder ,msg);
						//�����м�ֵ�趨Ҳ��Ҫ�޸�
						 if(can_count<=100)
						 {
						 #if CALIBRATION
							 if((GMPitchEncoder.ecd_bias - GMPitchEncoder.ecd_value) <-4000)
							 {
								 GMPitchEncoder.ecd_bias = gAppParamStruct.GimbalCaliData.GimbalPitchOffset + 8192;
							 }
							 else if((GMPitchEncoder.ecd_bias - GMPitchEncoder.ecd_value) > 4000)
							 {
								 GMPitchEncoder.ecd_bias = gAppParamStruct.GimbalCaliData.GimbalPitchOffset - 8192;
							 }
						 #else
							 if((GMPitchEncoder.ecd_bias - GMPitchEncoder.ecd_value) <-4000)
							 {
								 GMPitchEncoder.ecd_bias = GIMBAL_BIAS_PITCH + 8192;
							 }
							 else if((GMPitchEncoder.ecd_bias - GMPitchEncoder.ecd_value) > 4000)
							 {
								 GMPitchEncoder.ecd_bias = GIMBAL_BIAS_PITCH - 8192;
							 }
					   #endif
						 }
				}break;				
				case CAN_BUS1_ZGYRO_FEEDBACK_MSG_ID:
				{
					LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_ZGYRO));
					ZGyroModuleAngle = -0.01f*((int32_t)(msg->Data[0]<<24)|(int32_t)(msg->Data[1]<<16) | (int32_t)(msg->Data[2]<<8) | (int32_t)(msg->Data[3])); 
				}break;
				default:
				{
				}
		}
		
			if(!LostCounterOverflowCheck(fabs(GMYawEncoder.ecd_angle), 70.0f) || GetWorkState() == STOP_STATE)  //�����ֹͣģʽ��һֱι����ֹ��������ʧ��
			{
				LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_DEADLOCK));
			}		 
}

/********************************************************************************
   �����̵���巢��ָ�ID��Ϊ0x200������̷���IDΪ0x201-0x204
*********************************************************************************/
void Set_CM_Speed(CAN_TypeDef *CANx, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq)//Ӧ���ǵ��̵�����������ݷ��ͺ���
{
    CanTxMsg tx_message;
    tx_message.StdId = 0x200;             //Ӧ�����й�can���͵�ͨ��Э��
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = (uint8_t)(cm1_iq >> 8);  //��16λ���ݷ�Ϊ��8λ�͵�8λ
    tx_message.Data[1] = (uint8_t)cm1_iq;
    tx_message.Data[2] = (uint8_t)(cm2_iq >> 8);
    tx_message.Data[3] = (uint8_t)cm2_iq;
    tx_message.Data[4] = (uint8_t)(cm3_iq >> 8);
    tx_message.Data[5] = (uint8_t)cm3_iq;
    tx_message.Data[6] = (uint8_t)(cm4_iq >> 8);
    tx_message.Data[7] = (uint8_t)cm4_iq;
    CAN_Transmit(CANx,&tx_message);
}
/********************************************************************************
   ���������ź�
********************************************************************************/
void SetfadanwheelSpeed(int16_t fadan)
{
		CanTxMsg tx_message;    
		tx_message.StdId = 0x1FF;
		tx_message.IDE = CAN_Id_Standard;
		tx_message.RTR = CAN_RTR_Data;
		tx_message.DLC = 0x08;
	
		tx_message.Data[0] = 0x00;
    tx_message.Data[1] = 0x00;
	 	tx_message.Data[2] = 0x00;
    tx_message.Data[3] = 0x00;
		tx_message.Data[4] = (uint8_t)(fadan >> 8);
		tx_message.Data[5] =  (uint8_t)fadan;
    tx_message.Data[6] = 0x00;
    tx_message.Data[7] = 0x00;
		
	  CAN_Transmit(CAN2,&tx_message);
}

void Set_current(int16_t gimbal_yaw_iq,int16_t gimbal_pitch_iq,int16_t fadan,int16_t Bigfadan)
{
		CanTxMsg tx_message;
    tx_message.StdId = 0x1FF;             //Ӧ�����й�can���͵�ͨ��Э��
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = (uint8_t)(gimbal_yaw_iq >> 8);  //��16λ���ݷ�Ϊ��8λ�͵�8λ
    tx_message.Data[1] = (uint8_t)gimbal_yaw_iq;
    tx_message.Data[2] = (uint8_t)(gimbal_pitch_iq >> 8);
    tx_message.Data[3] = (uint8_t)gimbal_pitch_iq;
    tx_message.Data[4] = (uint8_t)(fadan >> 8);
    tx_message.Data[5] = (uint8_t)fadan;
    tx_message.Data[6] = (uint8_t)(Bigfadan >> 8);
    tx_message.Data[7] = (uint8_t)Bigfadan;
    CAN_Transmit(CAN1,&tx_message);	
}


/********************************************************************************
   ������巢��ָ�ID��Ϊ0x1FF��ֻ����������壬���ݻش�IDΪ0x205��0x206
	 cyq:����Ϊ�������������ָ�
*********************************************************************************/
void Set_Gimbal_Current(CAN_TypeDef *CANx, int16_t gimbal_yaw_iq, int16_t gimbal_pitch_iq)
{
    CanTxMsg tx_message;    
    tx_message.StdId = 0x1FF;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = (unsigned char)(gimbal_yaw_iq >> 8);
    tx_message.Data[1] = (unsigned char)gimbal_yaw_iq;
    tx_message.Data[2] = (unsigned char)(gimbal_pitch_iq >> 8);
    tx_message.Data[3] = (unsigned char)gimbal_pitch_iq;
	
    CAN_Transmit(CANx,&tx_message);
}

/************************************
cxgĦ�����������
**************************************/
//void Set_Current(CAN_TypeDef *CANx,int16_t gimbal_yaw_iq, int16_t gimbal_pitch_iq, int16_t friction_iq)
//{
//    CanTxMsg tx_message;    
//    tx_message.StdId = 0x1FF;
//    tx_message.IDE = CAN_Id_Standard;
//    tx_message.RTR = CAN_RTR_Data;
//    tx_message.DLC = 0x08;
//   
//	
//tx_message.Data[0] = (unsigned char)(gimbal_yaw_iq >> 8);
//    tx_message.Data[1] = (unsigned char)gimbal_yaw_iq;
//    tx_message.Data[2] = (unsigned char)(gimbal_pitch_iq >> 8);
//    tx_message.Data[3] = (unsigned char)gimbal_pitch_iq;
//    tx_message.Data[4] = (unsigned char)(friction_iq >> 8);
//    tx_message.Data[5] = (unsigned char)friction_iq;
//    tx_message.Data[6] = 0x00;
//    tx_message.Data[7] = 0x00;
//    CAN_Transmit(CANx,&tx_message);
//}
/************
cxg��̨������һ�����
******************/
void Set_FRICTION_Current(CAN_TypeDef *CANx, int16_t friction_iq)
{
    CanTxMsg tx_message;    
    tx_message.StdId = 0x1FF;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
   
	
//    tx_message.Data[0] = 0x00;
//    tx_message.Data[1] = 0x00;
//    tx_message.Data[2] = 0x00;
//    tx_message.Data[3] = 0x00;
    tx_message.Data[4] = (unsigned char)(friction_iq >> 8);
    tx_message.Data[5] = (unsigned char)friction_iq;
    tx_message.Data[6] = 0x00;
    tx_message.Data[7] = 0x00;
    CAN_Transmit(CANx,&tx_message);
}

void Set_Gimbal_Calibrate(CAN_TypeDef *CANx, int16_t gimbal_yaw_iq, int16_t gimbal_pitch_iq)
{
    CanTxMsg tx_message;    
    tx_message.StdId = 0x1FF;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = 0x00;
    tx_message.Data[1] = 0x00;
    tx_message.Data[2] = 0x00;
    tx_message.Data[3] = 0x00;
    tx_message.Data[4] = 0x00;
    tx_message.Data[5] = 0x00;
    tx_message.Data[6] = 0x04;
    tx_message.Data[7] = 0x00;
    CAN_Transmit(CANx,&tx_message);
}
/***************************************
���̿��ƣ�ǰ����
****************************************/
void forward(int speed)
{
//				if(flag_power_over==0)
//	{
//		if((speed-CM1Speed_x)>2500)
//		{
//				CM1Speed_x+=7;
//				CM2Speed_x-=7;
//				CM3Speed_x+=7;
//				CM4Speed_x-=7;
//		}
//		else if((speed-CM1Speed_x)<-2500)
//		{
//				CM1Speed_x-=7;
//				CM2Speed_x+=7;
//				CM3Speed_x-=7;
//				CM4Speed_x+=7;
//		}
//		else
//		{
//			  CM1Speed_x=speed;
//				CM2Speed_x=(-speed);
//				CM3Speed_x=speed;
//				CM4Speed_x=(-speed);
//		}

//	}
//	else if(flag_power_over==1)
//	{
//		if(CM1SpeedPID.ref>2500)
//		{
//			CM1Speed_x-=15;
//			CM2Speed_x+=15;
//			CM3Speed_x-=15;
//			CM4Speed_x+=15;
//		}
//		else if(CM1SpeedPID.ref<-2500)
//		{
//			CM1Speed_x+=15;
//			CM2Speed_x-=15;
//			CM3Speed_x+=15;
//			CM4Speed_x-=15;
//		}
//				else
//		{
			  CM1Speed_x=speed;
				CM2Speed_x=(-speed);
				CM3Speed_x=speed;
				CM4Speed_x=(-speed);
//		}
//	}
//	if(speed==0) {CM1Speed_x=0;CM2Speed_x=0;CM3Speed_x=0;CM4Speed_x=0;}
}

/***************************************
���̿��ƣ����ˣ�
****************************************/
void backward(int speed)
{//MOTORPID(-speed,speed,-speed,speed);
	//Set_CM_Speed(CAN1,-1200,1200,-1200,1200);
	//delay_ms(20);

}
/***************************************
���̿��ƣ���ʱ�룩
****************************************/
void nishizhen(int speed)
{
	    CM1Speed_spin=speed;
	    CM2Speed_spin=speed;
	    CM3Speed_spin=speed;
	    CM4Speed_spin=speed;
}
/***************************************
���̿��ƣ�˳ʱ�룩
****************************************/
void shunshizhen(int speed)
{MOTORPID(-speed,-speed,-speed,-speed);
	//Set_CM_Speed(CAN1,-600,-600,-600,-600);
	delay_ms(20);
}
/***************************************
���̿�����ƽ��
****************************************/
void left_pinyi(int speed)
{MOTORPID(speed,-speed,-speed,speed);
//Set_CM_Speed(CAN1,1500,-1500,-1500,1500);
	delay_ms(20);
}
/***************************************
���̿��ƣ�˳ʱ�룩
****************************************/
void right_pinyi(int speed)
{
		CM1Speed_y=(speed);
		CM2Speed_y=-speed;
		CM3Speed_y=-speed;
		CM4Speed_y=(speed);
}
/***************************************
���̿���(ֹͣ)
****************************************/
void stop(void)
{
	MOTORPID(0,0,0,0);
	//Set_CM_Speed(CAN1,0,0,0,0);

}

	
