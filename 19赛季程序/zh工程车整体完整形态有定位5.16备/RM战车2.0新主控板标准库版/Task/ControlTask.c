#include "main.h"

#define UP     1
#define MIDDLE 3
#define DOWN   2
KEY_MO key_mouse;
int f,l,p,g,s1,s2;//ң��ͨ��
int F=0,L1=0,L2=0,L3=0,P=0,S1;
float G=6500;//pitch��ң�ؽǶȱ���
int friction_quanshu=0;//�����ٶȿ��ƽǶ�ʱ��ȨȦ��
int ii=0,iii=0;
int mix;
int timeXL=5800;
int suduXL=-2000;

int timeSL=4500;
int suduSL=4500;
int time3=4000;
int sudu3=-3000;
int timer=125000;//150000
int ZH=800;
int sh1=0,sh2=0,sh3=0;
int dpz=0;
int L4=0,sh4=0;
int duiwei=0;
int dcf=0,shangdao_flag=1;
int long countL=0,HA;
int handflag=1;
int CML;
float wcb;
int xxl=0;
int xml=0;
int lhy1,lhy2,lhy3,lhp1,lhp2,lhp3;
uint8_t TravelSwitch_1;
uint8_t TravelSwitch_2;

PID_Regulator_t GMPPositionPID = GIMBAL_MOTOR_PITCH_POSITION_PID_DEFAULT;//cxg ע�� 
PID_Regulator_t GMPSpeedPID = GIMBAL_MOTOR_PITCH_SPEED_PID_DEFAULT;
PID_Regulator_t GMYPositionPID = GIMBAL_MOTOR_YAW_POSITION_PID_DEFAULT;			
PID_Regulator_t GMYSpeedPID = GIMBAL_MOTOR_YAW_SPEED_PID_DEFAULT;

PID_Regulator_t CMRotatePID = CHASSIS_MOTOR_ROTATE_PID_DEFAULT; 
PID_Regulator_t CM1SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM2SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM3SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM4SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;

//PID_Regulator_t ShootMotorPositionPID = SHOOT_MOTOR_POSITION_PID_DEFAULT;      //shoot motor
PID_Regulator_t ShootMotorSpeedPID = SHOOT_MOTOR_SPEED_PID_DEFAULT;    //zh:ע��
PID_Regulator_t vice_fwheelspeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;//zh
PID_Regulator_t vice_bwheelspeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t across1wheelspeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t across2wheelspeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t WumingspeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;//
PID_Regulator_t armspeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t handspeedPID = CHASSIS_HAND_SPEED_PID_DEFAULT;


/*--------------------------------------------CTRL Variables----------------------------------------*/
WorkState_e lastWorkState = PREPARE_STATE;
WorkState_e workState = PREPARE_STATE;
RampGen_t GMPitchRamp = RAMP_GEN_DAFAULT;
RampGen_t GMYawRamp = RAMP_GEN_DAFAULT;
static uint32_t time_tick_1ms = 0;
/*

*********************************************************************************************************
*                                            FUNCTIONS 
*********************************************************************************************************
*/

static void SetWorkState(WorkState_e state)
{
    workState = state;
}

WorkState_e GetWorkState()
{
	return workState;
}
//���̿�������
void CMControlLoop(void)
{  
	//������ת������
	if(GetWorkState()==PREPARE_STATE) //�����׶Σ����̲���ת
	{
		ChassisSpeedRef.rotate_ref = 0;	 
	}
	else
	{
		 //���̸����������תPID����
		 CMRotatePID.ref = 0;
		 CMRotatePID.fdb = GMYawEncoder.ecd_angle;
		#if CALIBRATION
		 CMRotatePID.kp = RotateSpeedSavedPID.kp_offset;
		 CMRotatePID.ki = RotateSpeedSavedPID.ki_offset;
		 CMRotatePID.kd = RotateSpeedSavedPID.kd_offset;
		#endif
		 CMRotatePID.Calc(&CMRotatePID);   
		 ChassisSpeedRef.rotate_ref = CMRotatePID.output;
	}
//	if(Is_Lost_Error_Set(LOST_ERROR_RC))      //���ң������ʧ��ǿ�ƽ��ٶ��趨ֵreset
//	{
//		ChassisSpeedRef.forward_back_ref = 0;
//		ChassisSpeedRef.left_right_ref = 0;
//	}
	#if CHASSISMOTORTYPE//���̵��
	CM1SpeedPID.ref =  -ChassisSpeedRef.forward_back_ref*3*0.45 + ChassisSpeedRef.left_right_ref*3*0.45 + ChassisSpeedRef.rotate_ref*9;
	CM2SpeedPID.ref = ChassisSpeedRef.forward_back_ref*3*0.45 + ChassisSpeedRef.left_right_ref*3*0.45 + ChassisSpeedRef.rotate_ref*9;
	CM3SpeedPID.ref = ChassisSpeedRef.forward_back_ref*3*0.45 - ChassisSpeedRef.left_right_ref*3*0.45 + ChassisSpeedRef.rotate_ref*9;
	CM4SpeedPID.ref = -ChassisSpeedRef.forward_back_ref*3*0.45 - ChassisSpeedRef.left_right_ref*3*0.45 + ChassisSpeedRef.rotate_ref*9;

	CM1SpeedPID.fdb = CM1Encoder.filter_rate;//�����ٶ�
	CM2SpeedPID.fdb = CM2Encoder.filter_rate;
	CM3SpeedPID.fdb = CM3Encoder.filter_rate;
	CM4SpeedPID.fdb = CM4Encoder.filter_rate;

	#else
	CM1SpeedPID.ref =  -ChassisSpeedRef.forward_back_ref*7 + ChassisSpeedRef.left_right_ref*7 ;// + ChassisSpeedRef.maobu_ref*3;//ChassisSpeedRef.rotate_ref*20;
	CM2SpeedPID.ref = ChassisSpeedRef.forward_back_ref*7 + ChassisSpeedRef.left_right_ref*7 ;//+ ChassisSpeedRef.maobu_ref*3;//ChassisSpeedRef.rotate_ref*20;
	CM3SpeedPID.ref = ChassisSpeedRef.forward_back_ref*7 - ChassisSpeedRef.left_right_ref*7 ;//+ ChassisSpeedRef.maobu_ref*3;//ChassisSpeedRef.rotate_ref*20;
	CM4SpeedPID.ref = -ChassisSpeedRef.forward_back_ref*7 - ChassisSpeedRef.left_right_ref*7 ;//	+ ChassisSpeedRef.maobu_ref*3;//ChassisSpeedRef.rotate_ref*20;
  if(RC_CtrlData.rc.s1==2)
	{
		CM1SpeedPID.ref +=ChassisSpeedRef.maobu_ref*3;
		CM2SpeedPID.ref +=ChassisSpeedRef.maobu_ref*3;
		CM3SpeedPID.ref +=ChassisSpeedRef.maobu_ref*3;
		CM4SpeedPID.ref +=ChassisSpeedRef.maobu_ref*3;
	}
	else
	{
		CM1SpeedPID.ref +=ChassisSpeedRef.rotate_ref*20;
		CM2SpeedPID.ref +=ChassisSpeedRef.rotate_ref*20;
		CM3SpeedPID.ref +=ChassisSpeedRef.rotate_ref*20;
		CM4SpeedPID.ref +=ChassisSpeedRef.rotate_ref*20;
	}
	
	
		
	CM1SpeedPID.fdb = Motor1_Measure.speed_rpm;
	CM2SpeedPID.fdb = Motor2_Measure.speed_rpm;
	CM3SpeedPID.fdb = Motor3_Measure.speed_rpm;
	CM4SpeedPID.fdb = Motor4_Measure.speed_rpm;
	#endif
	
	#if CALIBRATION//У׼
	 CM1SpeedPID.kp = CM2SpeedPID.kp = CM3SpeedPID.kp = CM4SpeedPID.kp = ChassisSpeedSavedPID.kp_offset;
	 CM1SpeedPID.ki = CM2SpeedPID.ki = CM3SpeedPID.ki = CM4SpeedPID.ki = ChassisSpeedSavedPID.ki_offset;
	 CM1SpeedPID.kd = CM2SpeedPID.kd = CM3SpeedPID.kd = CM4SpeedPID.kd = ChassisSpeedSavedPID.kd_offset;
	#endif
	CM1SpeedPID.Calc(&CM1SpeedPID);
	CM2SpeedPID.Calc(&CM2SpeedPID);
	CM3SpeedPID.Calc(&CM3SpeedPID);
	CM4SpeedPID.Calc(&CM4SpeedPID);
	if(CM1SpeedPID.output>-700 && CM1SpeedPID.output<700 ) CM1SpeedPID.output=0;
	if(CM2SpeedPID.output>-700 && CM2SpeedPID.output<700 ) CM2SpeedPID.output=0;
	if(CM3SpeedPID.output>-700 && CM3SpeedPID.output<700 ) CM3SpeedPID.output=0;
	if(CM4SpeedPID.output>-700 && CM4SpeedPID.output<700 ) CM4SpeedPID.output=0;
	 if((GetWorkState() == STOP_STATE) ||Is_Serious_Error() || GetWorkState() == CALI_STATE || GetWorkState() == PREPARE_STATE)    //|| dead_lock_flag == 1����ͣ����������У׼���޿�������ʱ����ʹ���̿���ֹͣ
	 {
		 Set_CM_Speed(CAN1, 0,0,0,0);
	 }
	 else
	 {
		 Set_CM_Speed(CAN1, CHASSIS_SPEED_ATTENUATION * CM1SpeedPID.output, CHASSIS_SPEED_ATTENUATION * CM2SpeedPID.output, CHASSIS_SPEED_ATTENUATION * CM3SpeedPID.output, CHASSIS_SPEED_ATTENUATION * CM4SpeedPID.output);
   // GREEN_LED_ON();		 
	 } 
}
//�����������������//zhע��
int16_t pwm_ccr = 0;
void ShooterMControlLoop(void)	
{				      
	if(GetShootState() == SHOOTING)
	{
		ShootMotorSpeedPID.ref = PID_SHOOT_MOTOR_SPEED;				
	}
	else
	{
		ShootMotorSpeedPID.ref = 0;		
	}
	
	ShootMotorSpeedPID.fdb = GetQuadEncoderDiff();   
	ShootMotorSpeedPID.Calc(&ShootMotorSpeedPID);
	PWM3 = ShootMotorSpeedPID.output;	
}



/**********************************************************
*����״̬�л�״̬��,��1ms��ʱ�ж�ͬƵ��
**********************************************************/

void WorkStateFSM(void)
{
	lastWorkState = workState;
	switch(workState)
	{
		case PREPARE_STATE:
		{
			if(GetInputMode() == STOP || Is_Serious_Error())
			{
				workState = STOP_STATE;
			}
			else if(GetCaliCmdFlagGrp())   
			{
				workState = CALI_STATE;
			}
			else if(time_tick_1ms > PREPARE_TIME_TICK_MS)
			{
				workState = NORMAL_STATE;
			}			
		}break;
		case NORMAL_STATE:     
		{
			if(GetInputMode() == STOP || Is_Serious_Error())
			{
				workState = STOP_STATE;
			}
			else if(GetCaliCmdFlagGrp())   
			{
				workState = CALI_STATE;
			}
//			else if((!IsRemoteBeingAction() ||(Get_Lost_Error(LOST_ERROR_RC) == LOST_ERROR_RC)) && GetShootState() != SHOOTING)
//			{
//				workState = STANDBY_STATE; 				
//			}			
		}break;
		case STANDBY_STATE:     
		{
			if(GetInputMode() == STOP || Is_Serious_Error())
			{
				workState = STOP_STATE;
			}
			else if(GetCaliCmdFlagGrp())   
			{
				workState = CALI_STATE;
			}
			else if(IsRemoteBeingAction() || (GetShootState()==SHOOTING) || GetFrictionState() == FRICTION_WHEEL_START_TURNNING)
			{
				workState = NORMAL_STATE;
			}				
		}break;
		case STOP_STATE:   
		{
			if(GetInputMode() != STOP && !Is_Serious_Error())
			{
				workState = PREPARE_STATE;   
			}
		}break;
		case CALI_STATE:      
		{
			if(GetInputMode() == STOP || Is_Serious_Error())
			{
				workState = STOP_STATE;
			}
		}break;	    
		default:
		{
			
		}
	}	
	WorkStateSwitchProcess();
}

static void WorkStateSwitchProcess(void)
{
	//���������ģʽ�л���prapareģʽ��Ҫ��һϵ�в�����ʼ��
	if((lastWorkState != workState) && (workState == PREPARE_STATE))  
	{
		ControtLoopTaskInit();
		RemoteTaskInit();
	}
}




void GimbalYawControlModeSwitch(void)
{
	static uint8_t normalFlag = 0;   //��������ģʽ��־
	static uint8_t standbyFlag = 1;  //IMU����ģʽ��־
	static uint32_t modeChangeDelayCnt = 0;
	static float angleSave = 0.0f;    //�����л�ģʽʱ�����л�ǰ�ĽǶ�ֵ�����ڽǶȸ���ֵ�л�
	switch(GetWorkState())
	{
		case PREPARE_STATE:   //�������̣�����б��
		{
			GMYPositionPID.ref = 0.0f;
			GMYPositionPID.fdb = -GMYawEncoder.ecd_angle*GMYawRamp.Calc(&GMYawRamp);
			angleSave = yaw_angle;	
			standbyFlag = 1;
		}break;
		case NORMAL_STATE:
		{
			if(standbyFlag == 1)
			{
				standbyFlag = 0;
				normalFlag = 1;
				GimbalRef.yaw_angle_dynamic_ref = angleSave;   //�޸��趨ֵΪSTANDBY״̬�¼�¼�����һ��ZGYROMODULEAngleֵ
				modeChangeDelayCnt = 0;   //delay����
			}
			GMYPositionPID.ref = GimbalRef.yaw_angle_dynamic_ref;   //�趨����ֵ
			GMYPositionPID.fdb = yaw_angle; 					//�趨����ֵ
			angleSave = yaw_angle;   //ʱ�̱���IMU��ֵ���ڴ�NORMAL��STANDBYģʽ�л�
		}break;
		case STANDBY_STATE:   //IMUģʽ
		{
			modeChangeDelayCnt++;
			if(modeChangeDelayCnt < STATE_SWITCH_DELAY_TICK)    //delay�����ʱ����NORMAL_STATEһ��
			{
				GMYPositionPID.ref = GimbalRef.yaw_angle_dynamic_ref;   //�趨����ֵ
				GMYPositionPID.fdb = yaw_angle; 					//�趨����ֵ
				angleSave = yaw_angle;
			}
			else     //delayʱ�䵽���л�ģʽ��IMU
			{
				if(normalFlag == 1)   //�޸�ģʽ��־
				{
					normalFlag = 0;
					standbyFlag = 1;
					GimbalRef.yaw_angle_dynamic_ref = angleSave;    //�������delayʱ����ڱ����
				}
				GMYPositionPID.ref = GimbalRef.yaw_angle_dynamic_ref;   //�趨����ֵ
				GMYPositionPID.fdb = yaw_angle; 					//�趨����ֵ	
				angleSave = yaw_angle;           //IMUģʽʱ������ZGyro��ֵ��ģʽ�л�ʱ�޸ĸ���ֵʹ��						
			}
		}break;
		case STOP_STATE:    //ֹͣ����ģʽ
		{
			
		}break;
		case CALI_STATE:    //У׼ģʽ
		{
			
		}break;
	}	
}

//��̨pitch����Ƴ���
void GMPitchControlLoop(void)
{
	#if CALIBRATION
	 GMPPositionPID.kp = PitchPositionSavedPID.kp_offset ;
	 GMPPositionPID.ki = PitchPositionSavedPID.ki_offset ;
	 GMPPositionPID.kd = PitchPositionSavedPID.kd_offset ;
		
	 GMPSpeedPID.kp = PitchSpeedSavedPID.kp_offset ;
	 GMPSpeedPID.ki = PitchSpeedSavedPID.ki_offset ;
	 GMPSpeedPID.kd = PitchSpeedSavedPID.kd_offset ;
	#endif
	
	GMPPositionPID.ref = -GimbalRef.pitch_angle_dynamic_ref;
	GMPPositionPID.fdb = GMPitchEncoder.ecd_angle* GMPitchRamp.Calc(&GMPitchRamp);    //����б�º���
	GMPPositionPID.Calc(&GMPPositionPID);   //�õ�pitch��λ�û����������
	//pitch speed control
	GMPSpeedPID.ref = GMPPositionPID.output;
	GMPSpeedPID.fdb = MPU6050_Real_Data.Gyro_Y;
	GMPSpeedPID.Calc(&GMPSpeedPID);
}

void GMYawControlLoop(void)
{
	#if CALIBRATION
	 GMYPositionPID.kp = YawPositionSavedPID.kp_offset ;
	 GMYPositionPID.ki = YawPositionSavedPID.ki_offset ;
	 GMYPositionPID.kd = YawPositionSavedPID.kd_offset ;
		
	 GMYSpeedPID.kp = YawSpeedSavedPID.kp_offset ;
	 GMYSpeedPID.ki = YawSpeedSavedPID.ki_offset ;
	 GMYSpeedPID.kd = YawSpeedSavedPID.kd_offset ;
	#endif
	GMYPositionPID.Calc(&GMYPositionPID);
	//yaw speed control
	GMYSpeedPID.ref = GMYPositionPID.output;
	GMYSpeedPID.fdb = MPU6050_Real_Data.Gyro_Z;
	GMYSpeedPID.Calc(&GMYSpeedPID);			
}

void SetGimbalMotorOutput(void)
{
	//��̨�������								
	if((GetWorkState() == STOP_STATE) ||Is_Serious_Error() || GetWorkState() == CALI_STATE)   
	{
		Set_Gimbal_Current(CAN1, 0, 0);     //yaw + pitch			
	}
	else
	{		
		Set_Gimbal_Current(CAN1, (int16_t)GMYSpeedPID.output, -(int16_t)GMPSpeedPID.output);     //yaw + pitch	
	}		
}


//���������ʼ������
void ControtLoopTaskInit(void)
{
	//������ʼ��
	time_tick_1ms = 0;   //�ж��еļ�������
	//���������ʼ��
	AppParamInit();
	//У׼�����ƫ��ֵ��ʼ��
	Sensor_Offset_Param_Init(&gAppParamStruct);
	//���ù���ģʽ
	SetWorkState(PREPARE_STATE);
	//б�³�ʼ��
	GMPitchRamp.SetScale(&GMPitchRamp, PITCH_PREPARE_TIME_TICK_MS);
	GMYawRamp.SetScale(&GMYawRamp, YAW_PREPARE_TIME_TICK_MS);
	GMPitchRamp.ResetCounter(&GMPitchRamp);
	GMYawRamp.ResetCounter(&GMYawRamp);
	//��̨�����Ƕȳ�ʼ��
	GimbalRef.pitch_angle_dynamic_ref = 0.0f;
	GimbalRef.yaw_angle_dynamic_ref = 0.0f;
    //��������ʼ��
    LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_RC));
    LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_IMU));
    LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_ZGYRO));
    LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR1));
    LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR2));
    LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR3));
    LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR4));
    LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR5));
    LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_MOTOR6));
    LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_DEADLOCK));
    LostCounterFeed(GetLostCounter(LOST_COUNTER_INDEX_NOCALI));
    
	//PID��ʼ��
	ShootMotorSpeedPID.Reset(&ShootMotorSpeedPID);
	GMPPositionPID.Reset(&GMPPositionPID);
	GMPSpeedPID.Reset(&GMPSpeedPID);
	GMYPositionPID.Reset(&GMYPositionPID);
	GMYSpeedPID.Reset(&GMYSpeedPID);
	CMRotatePID.Reset(&CMRotatePID);
	CM1SpeedPID.Reset(&CM1SpeedPID);
	CM2SpeedPID.Reset(&CM2SpeedPID);
	CM3SpeedPID.Reset(&CM3SpeedPID);
	CM4SpeedPID.Reset(&CM4SpeedPID);
}


//�������񣬷���timer6 1ms��ʱ�ж���ִ��
void Control_Task(void)
{
	time_tick_1ms++;
	WorkStateFSM();			//״̬�����
	
	//��������ݴ����Ƶ����ݳ�ʼ����Ԫ��
	if(time_tick_1ms <100)	
	{
		Init_Quaternion();
	}
	//ƽ̨�ȶ������󣬸�λ������ģ��
//	if(time_tick_1ms == PREPARE_TIME_TICK_MS/2)
//	{
//		GYRO_RST();
//	}
		
	
	//step 1: ��̨����
	GimbalYawControlModeSwitch();   //ģʽ�л������õ�λ�û����趨ֵ�͸���ֵ
	GMPitchControlLoop();
	GMYawControlLoop();
	SetGimbalMotorOutput();
	
	CalibrateLoop();   //У׼���񣬵����յ�У׼��������Чִ�У�����ֱ������
	//chassis motor control
	if(time_tick_1ms%4 == 0)         //motor control frequency 4ms
	{
		//�������
	//	SuperviseTask();    
		//���̿�������
		CMControlLoop();			
		ShooterMControlLoop();       //���������������
	}

	
}
void remote(void)
{
	int speed;
	 Key_Mouse(RC_CtrlData.rc.ch0,RC_CtrlData.rc.ch1,RC_CtrlData.rc.ch2,RC_CtrlData.rc.ch3,RC_CtrlData.bolun,RC_CtrlData.rc.s1,RC_CtrlData.rc.s2,RC_CtrlData.key.v,RC_CtrlData.mouse.x,RC_CtrlData.mouse.y,RC_CtrlData.mouse.z,RC_CtrlData.mouse.press_l,RC_CtrlData.mouse.press_r);
	l=(RC_CtrlData.rc.ch0-1024);//�ұ�����   ��ת
	f=(RC_CtrlData.rc.ch3-1024);//��ߴ�ֱ   ǰ��
	p=(RC_CtrlData.rc.ch2-1024);//�������	 ƽ��
	g=(RC_CtrlData.rc.ch1-1024);//�ұ�����
	s1=RC_CtrlData.rc.s1;   //cml:���Ͻ�
	s2=RC_CtrlData.rc.s2;   //cml:���Ͻ�

	//if(l==-1024) stop();
 if(f!=0||l!=0||p!=0||g!=0)
	{
		speed=(-f*6);
		forward(speed);
		 //HANDPID(speed);
	//	HANDPID(f);
		speed=l*6;	
		nishizhen(speed);
		speed=p*10;
	   right_pinyi(speed);
//		if(s1!=UP)
//		{
//		  WuMingLunPID((g*8)+600,(-g*8)-600,0,0);
//		
		if(key_mouse.com_s!=2)
		{
		WuMingLunPID((g*8)+520,(-g*8)-520,0,0);}


		MOTORPID(CM1SpeedPID.ref,CM2SpeedPID.ref,CM3SpeedPID.ref,CM4SpeedPID.ref); //�����˶�
	//	MOTORPID(0,0,0,0);
	}
 if(l==0&&f==0&&p==0&&g==0)
	{
	 		stop();
			WuMingLunPID(520,-520,0,0);
  }
	

	
	/*�����̨*/
	if(s2==3)//��
	{YAW(1450);//1450//1500
	PITCH(1500);}
	else if(s2==2)//��
	{YAW(1500);//1500//1950
	PITCH(1950);}
	else if(s2==1)//��
	{YAW(2200);//2200//1500
	PITCH(1500);}
		

/*��ת*/
	if(key_mouse.com_q==1)//
	{
	  HANDPID(0);
	}
	else if(key_mouse.com_q==2)//��λ��
	{
	  HANDPID(-150);
	}
	
/*��ȡ*/
	if(key_mouse.com_a==1)
	{
		EDOOR2_CLOSE();
	}
	
	else if(key_mouse.com_a==2)
	{
		EDOOR2_OPEN();
	}
	
	
	/*��е��*/
	if(key_mouse.com_arm==1)//��е����
	{
	  EDOOR6_CLOSE();
	}
	else if(key_mouse.com_arm==2)//��е����
	{
	  EDOOR6_OPEN();
	}
	
  /*����*/
	if(key_mouse.com_e==1)//������
	{
	  EDOOR5_CLOSE();
	}
	else if(key_mouse.com_e==2)//������
	{
	  EDOOR5_OPEN();
	}
		
	/*�ϳ�*/
	if(key_mouse.com_d==1)//�ϳ���
	{
	  EDOOR7_CLOSE();
		EDOOR8_CLOSE();
	}
	else if(key_mouse.com_d==2)//�ϳ���
	{
	   EDOOR7_OPEN();
		 EDOOR8_OPEN();
	}
	
	/*ƽ��*/
//	if(s1==3)//
//	{
//	  EDOOR3_CLOSE();
//		EDOOR4_CLOSE();
//	}
//	else if(s1==2)//
//	{
//	   EDOOR3_OPEN();
//		 EDOOR4_CLOSE();
//	}
//	
//	else if(s1==2)//
//	{
//	   EDOOR3_OPEN();
//		 EDOOR4_CLOSE();
//	}

	
	/*�ǵ�*/
	
	if(key_mouse.com_s==1)
	{
		EDOOR1_CLOSE();
	}
	else if(key_mouse.com_s==2)
	{
		EDOOR1_OPEN();
		WuMingLunPID((g*8)+520,(-g*8)-520,p*15,-p*15);
	}

}




/**************************
����
**************************/
void MOTORPID(int w,int s,int q,int d)
{
							
	CM1SpeedPID.ref=w;
	CM1SpeedPID.fdb=M1; 
	CM2SpeedPID.ref=s;
	CM2SpeedPID.fdb=M2; 
	CM3SpeedPID.ref=q;
	CM3SpeedPID.fdb=M3; 
	CM4SpeedPID.ref=d;
	CM4SpeedPID.fdb=M4; 
	PID_Calc(&CM1SpeedPID);		
	PID_Calc(&CM2SpeedPID);
	PID_Calc(&CM3SpeedPID);		
	PID_Calc(&CM4SpeedPID);
		
	if(CM1SpeedPID.output>=8000)
		{
			CM1SpeedPID.output=8000;	
		
		}
		if(CM1SpeedPID.output<=-8000)
		{
			CM1SpeedPID.output=-8000;	
			
		}
		
		if(CM2SpeedPID.output>=8000)
		{
			CM2SpeedPID.output=8000;	
		
		}
		if(CM2SpeedPID.output<=-8000)
		{
			CM2SpeedPID.output=-8000;	
			
		}
		
		if(CM3SpeedPID.output>=8000)
		{
			CM3SpeedPID.output=8000;	
		
		}
		if(CM3SpeedPID.output<=-8000)
		{
			CM3SpeedPID.output=-8000;	
			
		}
		
		if(CM4SpeedPID.output>=8000)
		{
			CM4SpeedPID.output=8000;	
		
		}
		if(CM4SpeedPID.output<=-8000)
		{
			CM4SpeedPID.output=-8000;	
			
		}
	Set_CM_Speed(CAN1,CM1SpeedPID.output,CM2SpeedPID.output,CM3SpeedPID.output,CM4SpeedPID.output);
	delay_ms(5);	
		
}


/************************************
zh:��֪����ʲô���õ�����  �Ƹ�+С��
************************************/
void WuMingLunPID(int l,int t,int w,int s)
{ 
	across2wheelspeedPID.ref=l;
	across2wheelspeedPID.fdb=M5;
	across1wheelspeedPID.ref=t;
	across1wheelspeedPID.fdb=M6;
	vice_fwheelspeedPID.ref=w;
	vice_fwheelspeedPID.fdb=M7;
	vice_bwheelspeedPID.ref=s;
	vice_bwheelspeedPID.fdb=M8;
	
	PID_Calc(&vice_fwheelspeedPID);
	PID_Calc(&vice_bwheelspeedPID);
	PID_Calc(&across2wheelspeedPID);
	PID_Calc(&across1wheelspeedPID);
	if(across2wheelspeedPID.output>=5000)
		{
			across2wheelspeedPID.output=5000;	
		
		}
		if(across2wheelspeedPID.output<=-5000)
		{
			across2wheelspeedPID.output=-5000;	
			
		}
	if(across1wheelspeedPID.output>=5000)
		{
			across1wheelspeedPID.output=5000;	
		
		}
		if(across1wheelspeedPID.output<=-5000)
		{
			across1wheelspeedPID.output=-5000;	
			
		}
	if(vice_fwheelspeedPID.output>=5000)
		{
			vice_fwheelspeedPID.output=5000;	
		
		}
		if(vice_fwheelspeedPID.output<=-5000)
		{
			vice_fwheelspeedPID.output=-5000;	
			
		}
	if(vice_bwheelspeedPID.output>=5000)
		{
			vice_bwheelspeedPID.output=5000;	
		
		}
		if(vice_bwheelspeedPID.output<=-5000)
		{
			vice_bwheelspeedPID.output=-5000;	
			
		}
	Arcoss_Vice(CAN1,across2wheelspeedPID.output, across1wheelspeedPID.output, vice_fwheelspeedPID.output,vice_bwheelspeedPID.output);
		delay_ms(1);
	
}

	void ARMPID(int l)
	{ 
	armspeedPID.ref=l;
	armspeedPID.fdb=M9;
	PID_Calc(&armspeedPID);

	if(armspeedPID.output>=5000)
		{
			armspeedPID.output=5000;	
		
		}
		if(armspeedPID.output<=-5000)
		{
			armspeedPID.output=-5000;	
			
		}

	Set_GETarm(CAN2,armspeedPID.output);
		delay_ms(1);

	}


void HANDPID(int h)
{ 
	
//	handspeedPID.ref=h;
	//handspeedPID.fdb=M10;
	handspeedPID.ref=h;
	handspeedPID.fdb=HANDEncoder.ecd_angle;

	//PID_Calc(&handspeedPID);
	POSITION_PID_Calc(&handspeedPID);
	Set_GEThand(CAN2,handspeedPID.output);
	delay_ms(1);
	
}

void Key_Mouse(int16_t RC_CH0,int16_t RC_CH1,int16_t RC_CH2,int16_t RC_CH3,int16_t bolun,int8_t switch1,int8_t switch2,uint16_t Key,int16_t X_Value,int16_t Y_Value,int16_t Z_Value,uint8_t Mouse_Press_l,uint8_t Mouse_Press_r)
{
	uint8_t key_qian=0,key_hou=0,key_zuo=0,key_you=0;
  static int16_t add=0,minu=0;
	static uint8_t shift_scan=1,ctrl_scan=1,q_scan=1,w_scan=1,e_scan=1,a_scan=1,s_scan=1,d_scan=1;
	static uint8_t q_flag=1,w_flag=1,e_flag=1,a_flag=1,s_flag=1,d_flag=1,shift_flag=1,ctrl_flag=1;
	static int8_t com=0,com_arm=1,com_duoji=0,com_d=1,com_q=1,com_a=1,com_e=1,com_s=1,com_shift=1,com_ctrl=1;
	/*��е��*/
//	if((Key&0x01)==key_w)//Wǰ
//	{com_arm=1;}
//	else if((Key&0x02)==key_s)//s��
//	{com_arm=2;}
//	else com_arm=0;
////////	if((Key&0x04)==key_a)//a��
////////	{key_zuo=1;}
////////	if((Key&0x08)==key_d)//d��
////////	{key_you=1;}
//////////////////////////    if(w_scan&&((Key&0x01)==key_w))//w����е��
//////////////////////////		{
//////////////////////////			if(w_flag)
//////////////////////////			{
//////////////////////////				com_arm=1;
//////////////////////////			}
//////////////////////////			else
//////////////////////////			{
//////////////////////////				com_arm=2;
//////////////////////////			}
//////////////////////////			w_flag=1-w_flag;
//////////////////////////			w_scan=0;
//////////////////////////		}
//////////////////////////		else if((Key&0x01)!=key_w) w_scan=1;
////////	if((Key&0x10)==key_shift)
////////	{
////////	add+=10;
////////		if(add>=330) assdd=330;
////////	}
////////	else {
////////		add-=20;
////////	if(add<=0)add=0;
////////	}
////////	if((Key&0x20)==key_ctrl)
////////	{
////////	minu+=10;
////////		if(minu>=220) minu=220;
////////	}
////////	else {
////////		minu-=20;
////////	if(minu<=0)minu=0;
////////	}
		
/*��λ*/
		if(q_scan&&((Key&0x40)==key_q))//q����λ
		{
			if(q_flag)
			{
				com_q=2;
			}
			else
			{
				com_q=1;
			}
			q_flag=1-q_flag;
			q_scan=0;
		}
		else if((Key&0x40)!=key_q) q_scan=1;
		
/*��ȡ*/		
		if(a_scan&&((Key&0x04)==key_a))//a����ȡ
		{
			if(a_flag)
			{
				com_a=2;
			}
			else
			{
				com_a=1;
			}
			a_flag=1-a_flag;
			a_scan=0;
		}
		else if((Key&0x04)!=key_a) a_scan=1;
		
/*��ȡ*/
		if(s_scan&&((Key&0x02)==key_s))//s������
		{
			if(s_flag)
			{
				com_s=2;
			}
			else
			{
				com_s=1;
			}
			s_flag=1-s_flag;
			s_scan=0;
		}
		else if((Key&0x02)!=key_s) s_scan=1;
		
/*��е��*/
		if(w_scan&&((Key&0x01)==key_w))//w����е��
		{
			if(w_flag)
			{
				com_arm=2;
			}
			else
			{
				com_arm=1;
			}
			w_flag=1-w_flag;
			w_scan=0;
		}
		else if((Key&0x01)!=key_w) w_scan=1;
		
/*����*/		
		if(e_scan&&((Key&0x80)==key_e))//e������
		{
			if(e_flag)
			{
				com_e=2;
			}
			else
			{
				com_e=1;
			}
			e_flag=1-e_flag;
			e_scan=0;
		}
		else if((Key&0x80)!=key_e) e_scan=1;
		//else com=0;
		
/*�ϳ�*/
		if(d_scan&&((Key&0x08)==key_d))//d��
		{
			if(d_flag)
			{
				com_d=1;
			}
			else
			{
				com_d=2;
			}
			d_flag=1-d_flag;
			d_scan=0;
		}
		else if((Key&0x08)!=key_d) d_scan=1;
		
/*�ֶ��ǵ�*/
		if(shift_scan&&((Key&0x10)==key_shift))//shift���ǵ�
		{
			if(shift_flag)
			{
				com_shift=1;
			}
			else
			{
				com_shift=2;
			}
			shift_flag=1-shift_flag;
			shift_scan=0;
		}
		else if((Key&0x10)!=key_shift) shift_scan=1;
		
		/*�ֶ��ǵ�*/
		if(ctrl_scan&&((Key&0x20)==key_ctrl))//shift���ǵ�
		{
			if(ctrl_flag)
			{
				com_ctrl=1;
			}
			else
			{
				com_ctrl=2;
			}
			ctrl_flag=1-ctrl_flag;
			ctrl_scan=0;
		}
		else if((Key&0x10)!=key_ctrl) ctrl_scan=1;
		
		
		

///////////////////////*���*/		
//////////////////////		if(shift_scan&&((Key&0x10)==key_shift))
//////////////////////		{
//////////////////////		  /*״̬1*/
//////////////////////			if(shift_flag)
//////////////////////			{
//////////////////////				com_duoji=1;
//////////////////////			}
//////////////////////			else
//////////////////////			{
//////////////////////				com_duoji=0;
//////////////////////			}
//////////////////////		  shift_flag=1-shift_flag;
//////////////////////			shift_scan=0;
//////////////////////		}
//////////////////////		else if((Key&0x10)!=key_shift) shift_scan=1;

//////////////////////	  if(ctrl_scan&&((Key&0x20)==key_ctrl))
//////////////////////		{
//////////////////////			/*״̬2*/
//////////////////////			if(ctrl_flag)
//////////////////////			{
//////////////////////				com_duoji=2;
//////////////////////			}
//////////////////////			else
//////////////////////			{
//////////////////////				 com_duoji=0;
//////////////////////			}
//////////////////////		  ctrl_flag=1-ctrl_flag;
//////////////////////			ctrl_scan=0;
//////////////////////		}
//////////////////////		else if((Key&0x20)!=key_ctrl) ctrl_scan=1;
		
		//else com_duoji=0;
//////////////		if((Key&0x40)==key_q)
//////////////		{
//////////////		com=1;
//////////////			if(Mouse_Press_l)com=5;//
//////////////			if(Mouse_Press_r)com=9;
//////////////		}
//////////////		else if((Key&0x80)==key_e)
//////////////		{
//////////////		com=2;
//////////////			if(Mouse_Press_l)com=6;
//////////////			if(Mouse_Press_r)com=10;
//////////////		}
	//	else com=0;
	key_mouse.ch3=RC_CH3 + key_qian*(330+add-minu)-key_hou*(330+add-minu);			//ch3
	key_mouse.ch2=RC_CH2 + key_zuo*(330+add-minu)-key_you*(330+add-minu);			//ch2
	key_mouse.ch1=RC_CH1 + Y_Value*9;			//ch1
	key_mouse.ch0=RC_CH0 + X_Value*9;			//ch0
//////////	if(switch2==DOWN)
//////////	{key_mouse.com=com;key_mouse.flag=0;}
//////////	else {key_mouse.com=0;key_mouse.flag=1;}
	printf("ch0:%d,ch1:%d,ch2:%d,ch3:%d\r\n",key_mouse.ch0,key_mouse.ch1,key_mouse.ch2,key_mouse.ch3);
	printf("com:%d\r\n",com);
	printf("l:%d,r:%d\r\n",Mouse_Press_l,Mouse_Press_r);
	printf("key:%d\r\n",Key);
//	return fire;/* ����ֵ����갴�������ź�   
  key_mouse.com=com;
	key_mouse.com_arm=com_arm;
	key_mouse.com_duoji=com_duoji;
	key_mouse.com_d=com_d;
	key_mouse.com_q=com_q;
	key_mouse.com_a=com_a;
	key_mouse.com_e=com_e;
	key_mouse.com_s=com_s;
	key_mouse.com_shift=com_shift;
	key_mouse.com_ctrl=com_ctrl;
	
}



