#ifndef  __CONTROLTASK_H
#define  __CONTROLTASK_H

#include "main.h"
#include "PID.h"

//float hkp,hki,hkd;
//float qwe;
extern float p_kp,p_ki,p_kd;
extern float s_kp,s_ki,s_kd;

#define PREPARE_TIME_TICK_MS 4000      //prapare time in ms
#define PITCH_PREPARE_TIME_TICK_MS 2000
#define YAW_PREPARE_TIME_TICK_MS 2000
#define STATE_SWITCH_DELAY_TICK 100000   //mode change delay count in ms

#define YAW_POSITION_KP_DEFAULTS  6
#define YAW_POSITION_KI_DEFAULTS  0
#define YAW_POSITION_KD_DEFAULTS  2 

#define YAW_SPEED_KP_DEFAULTS  33//  25
#define YAW_SPEED_KI_DEFAULTS  0
#define YAW_SPEED_KD_DEFAULTS  0

// avoid bang --->  position:20.0  speed:19.0
//big bang   22.5 20.0
#define PITCH_POSITION_KP_DEFAULTS  6
#define PITCH_POSITION_KI_DEFAULTS  0
#define PITCH_POSITION_KD_DEFAULTS  3

#define PITCH_SPEED_KP_DEFAULTS  10.0	//7
#define PITCH_SPEED_KI_DEFAULTS  0
#define PITCH_SPEED_KD_DEFAULTS  0

#define CHASSIS_SPEED_KP_DEFAULTS  5//4 //4
#define CHASSIS_SPEED_KI_DEFAULTS  0
#define CHASSIS_SPEED_KD_DEFAULTS  0.2      //0.2

#define CHASSIS_ROTATE_KP_DEFAULTS  4
#define CHASSIS_ROTATE_KI_DEFAULTS  0
#define CHASSIS_ROTATE_KD_DEFAULTS  0

#define CHASSIS_HAND_KP_DEFAULTS  5//4 //4
#define CHASSIS_HAND_KI_DEFAULTS  0
#define CHASSIS_HAND_KD_DEFAULTS  0.2      //0.2

#define SHOOT_SPEED_KP_DEFAULTS  8
#define SHOOT_SPEED_KI_DEFAULTS  0
#define SHOOT_SPEED_KD_DEFAULTS  3

#define VICE_SPEED_KP_DEFAULTS   4
#define VICE_SPEED_KI_DEFAULTS  0
#define VICE_SPEED_KD_DEFAULTS  0.2

#define PID_SHOOT_MOTOR_SPEED_INIT (2)
#define PID_SHOOT_MOTOR_SPEED      (0)
#define PID_SHOOT_MOTOR_CHECK_SPEED (0.1)

#define CHASSIS_SPEED_ATTENUATION   (1.0f)

#define CHASSIS_MOTOR_ROTATE_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	CHASSIS_ROTATE_KP_DEFAULTS,\
	CHASSIS_ROTATE_KI_DEFAULTS,\
	CHASSIS_ROTATE_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	5000,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}\

//gimbal position pid control
//20  19
#define GIMBAL_MOTOR_PITCH_POSITION_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	PITCH_POSITION_KP_DEFAULTS,\
	PITCH_POSITION_KI_DEFAULTS,\
	PITCH_POSITION_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	4900,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}\

//gimbal speed pid control
#define GIMBAL_MOTOR_PITCH_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	PITCH_SPEED_KP_DEFAULTS,\
	PITCH_SPEED_KI_DEFAULTS,\
	PITCH_SPEED_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	4900,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}\

//gimbal yaw position pid control
#define GIMBAL_MOTOR_YAW_POSITION_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	YAW_POSITION_KP_DEFAULTS,\
	YAW_POSITION_KI_DEFAULTS,\
	YAW_POSITION_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	4900,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}\

//gimbal yaw speed pid control
#define GIMBAL_MOTOR_YAW_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	YAW_SPEED_KP_DEFAULTS,\
	YAW_SPEED_KI_DEFAULTS,\
	YAW_SPEED_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	4900,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}\

//D����ԭ��Ϊ0.4
#define CHASSIS_MOTOR_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	CHASSIS_SPEED_KP_DEFAULTS,\
	CHASSIS_SPEED_KI_DEFAULTS,\
	CHASSIS_SPEED_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	3500,\
	1500,\
	0,\
	30000,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}\

#define CHASSIS_HAND_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	CHASSIS_HAND_KP_DEFAULTS,\
	CHASSIS_HAND_KI_DEFAULTS,\
	CHASSIS_HAND_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	3500,\
	1500,\
	0,\
	30000,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}\

#define SHOOT_MOTOR_POSITION_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	220.f,\
	0.0f,\
	0.0f,\
	0,\
	0,\
	0,\
	4900,\
	3500,\
	1500,\
	0,\
	4950,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}\

#define SHOOT_MOTOR_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	SHOOT_SPEED_KP_DEFAULTS,\
	SHOOT_SPEED_KI_DEFAULTS,\
	SHOOT_SPEED_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	1000,\
	200,\
	100,\
	0,\
	4950,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}\

#define VICE_MOTOR_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	SHOOT_SPEED_KP_DEFAULTS,\
	SHOOT_SPEED_KI_DEFAULTS,\
	SHOOT_SPEED_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	1000,\
	200,\
	100,\
	0,\
	4950,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}\

//#define VICE_MOTOR_SPEED_PID_DEFAULT \
//{\
//	0,\
//	0,\
//	{0,0,0},\
//	CHASSIS_SPEED_KP_DEFAULTS,\
//	CHASSIS_SPEED_KI_DEFAULTS,\
//	CHASSIS_SPEED_KD_DEFAULTS,\
//	0,\
//	0,\
//	0,\
//	4900,\
//	3500,\
//	1500,\
//	0,\
//	30000,\
//	0,\
//	0,\
//	0,\
//	&PID_Calc,\
//	&PID_Reset,\
//}\

#define PID_CALI_DEFAULT  \
{\
	0,\
	0,\
	0,\
	0,\
}\

//initiate status: 
typedef enum
{
    PREPARE_STATE,     		//�ϵ���ʼ��״̬ 4s������
    STANDBY_STATE,			//��ֹ̨ͣ��ת״̬
    NORMAL_STATE,			//������״̬
    STOP_STATE,        	//ֹͣ�˶�״̬
    CALI_STATE,    			//У׼״̬
}WorkState_e;


//extern float hp,hi,hd;
extern PID_Regulator_t CMRotatePID;      		//���̸�����̨PID
extern PID_Regulator_t GMPPositionPID;      	//pitch��λ�û�PID
extern PID_Regulator_t GMPSpeedPID;      		//pitch���ٶȻ�PID
extern PID_Regulator_t GMYPositionPID;			//yaw��λ�û�PID
extern PID_Regulator_t GMYSpeedPID;      		//yaw���ٶȻ�PID

extern PID_Regulator_t ShootMotorPositionPID;    //������λ�û�PID
extern PID_Regulator_t ShootMotorSpeedPID;       //�������ٶȻ�PID

extern PID_Regulator_t CM1SpeedPID;				 //���̵���ٶȻ�PID
extern PID_Regulator_t CM2SpeedPID;
extern PID_Regulator_t CM3SpeedPID;
extern PID_Regulator_t CM4SpeedPID;

extern PID_Regulator_t vice_fwheelspeedPID;
extern PID_Regulator_t vice_bwheelspeedPID;
/**********************
������ң����ͨ��ȫ�ֱ���
***********************/
extern int f,l,p,g,s1;
/**********************
������ң����ͨ��ȫ�ֱ�����ת��ֵ
***********************/
extern int F,L,P,S1;
extern float G;

WorkState_e GetWorkState(void);
void ControtLoopTaskInit(void);
void WorkStateFSM(void);
static void WorkStateSwitchProcess(void);
void Control_Task(void);
void GMPitchControlLoop(void);
void GMYawControlLoop(void);
void GMPitchControlLoop(void);
void GimbalYawControlModeSwitch(void);   //ģʽ�л������õ�λ�û����趨ֵ�͸���ֵ
void SetGimbalMotorOutput(void);
void remote(void);                     //cxg
void GimbalPID(void);                  //cxg
void MOTORPID(int w,int s,int q,int d);      //CXG
void FRICTIONPID(int f);                    //������Ħ����PID
void WuMingLunPID(int l,int t,int w,int s);//zh
void HANDPID(int h);
void ARMPID(int h);
//void GetPID(int l,int t);
//void WuMingLunPID(int w);//zh
void Key_Mouse(int16_t RC_CH0,int16_t RC_CH1,int16_t RC_CH2,int16_t RC_CH3,int16_t bolun,int8_t switch1,int8_t switch2,uint16_t Key,int16_t X_Value,int16_t Y_Value,int16_t Z_Value,uint8_t Mouse_Press_l,uint8_t Mouse_Press_r);


#endif
