#ifndef  __CANBUSTASK_H
#define  __CANBUSTASK_H

#include "main.h"

/* CAN Bus 1 */  
#define CAN_BUS1_ZGYRO_FEEDBACK_MSG_ID            0x401

/* CAN Bus 2 */  
#define CAN_BUS2_MOTOR1_FEEDBACK_MSG_ID           0x201
#define CAN_BUS2_MOTOR2_FEEDBACK_MSG_ID           0x202 
#define CAN_BUS2_MOTOR3_FEEDBACK_MSG_ID           0x203
#define CAN_BUS2_MOTOR4_FEEDBACK_MSG_ID           0x204
#define CAN_BUS2_MOTOR5_FEEDBACK_MSG_ID           0x205
#define CAN_BUS2_MOTOR6_FEEDBACK_MSG_ID           0x206

#define CAN_BUS1_ZGYRO_FEEDBACK_MSG_ID   		  0x401

#define CHASSISMOTORTYPE 0   //EC60 1   3510  0
#define CALIBRATION  0    // 手动校准 0     上位机校准 1
#define GIMBAL_BIAS_YAW  	1510			//633
#define GIMBAL_BIAS_PITCH 	 6880			//4391
extern volatile Encoder CM1Encoder;
extern volatile Encoder CM2Encoder;
extern volatile Encoder CM3Encoder;
extern volatile Encoder CM4Encoder;
extern volatile Encoder GMYawEncoder;
extern volatile Encoder GMPitchEncoder;
 extern volatile Encoder HANDMOTOR;
extern float ZGyroModuleAngle;

extern Measure Motor1_Measure;
extern Measure Motor2_Measure;
extern Measure Motor3_Measure;
extern Measure Motor4_Measure;

void GetEncoderBias(volatile Encoder *v, CanRxMsg * msg);
void EncoderProcess(volatile Encoder *v, CanRxMsg * msg);
void get_measure(Measure *mea,CanRxMsg * msg);
void CanReceiveMsgProcess(CanRxMsg * msg);
void Set_Gimbal_Current(CAN_TypeDef *CANx, int16_t gimbal_pitch_iq, int16_t gimbal_yaw_iq);
void Set_CM_Speed(CAN_TypeDef *CANx, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq);
void Set_Gimbal_Calibrate(CAN_TypeDef *CANx, int16_t gimbal_yaw_iq, int16_t gimbal_pitch_iq);
void Set_FRICTION_Current(CAN_TypeDef *CANx, int16_t friction_iq);
void Set_Current(CAN_TypeDef *CANx,int16_t gimbal_yaw_iq, int16_t gimbal_pitch_iq, int16_t bolun_iq,int16_t friction_iq);
void Set_Current2(CAN_TypeDef *CANx,int16_t gimbal_yaw_iq, int16_t gimbal_pitch_iq, int16_t friction_iq5,int16_t friction_iq6);
#endif
