#include "main.h"
int f,l,p,g,s1,s2,bolun;//ң��ͨ��
int F=0,L=0,P=0,S1;
float G=164;//pitch��ң�ؽǶȱ���
float Y=285;//YAW��ң�ؽǶȱ���
float SPEED_LEVAL;//Ħ�����ٶȵ�λ��2��1
int speed_number=0;  //Ħ�����ٶ�
double IMU_G=-2000.00;//pitch��IMUң�ؽǶȱ���
float IMU_FDB;//pitch��IMU�����Ƕ�
int flag_genshui=0,Old_YY=0;
int8_t fire=0;



 int friction_quanshu=0;//�����ٶȿ��ƽǶ�ʱ��ȨȦ��
PID_Regulator_t GMPPositionPID = GIMBAL_MOTOR_PITCH_POSITION_PID_DEFAULT;//cxg ע��     
PID_Regulator_t GMPSpeedPID = GIMBAL_MOTOR_PITCH_SPEED_PID_DEFAULT;
PID_Regulator_t GMYPositionPID = GIMBAL_MOTOR_YAW_POSITION_PID_DEFAULT;			
PID_Regulator_t GMYSpeedPID = GIMBAL_MOTOR_YAW_SPEED_PID_DEFAULT;

PID_Regulator_t CMRotatePID = CHASSIS_MOTOR_ROTATE_PID_DEFAULT; //�ǣ�
PID_Regulator_t CM1SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM2SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM3SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM4SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;

//PID_Regulator_t ShootMotorPositionPID = SHOOT_MOTOR_POSITION_PID_DEFAULT;      //shoot motor
PID_Regulator_t ShootMotorSpeedPID = SHOOT_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t fadanMotorSpeedPID = SHOOT_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t BigShootMotorSpeedPID=SHOOT_MOTOR_SPEED_PID_DEFAULT;
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
//�����������������
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
{int speed;
	
		fire=Key_Mouse(RC_CtrlData.rc.ch0,RC_CtrlData.rc.ch1,RC_CtrlData.rc.ch2,RC_CtrlData.rc.ch3,RC_CtrlData.key.v,300,300, RC_CtrlData.mouse.y, RC_CtrlData.mouse.x,RC_CtrlData.mouse.press_l,RC_CtrlData.mouse.press_r);
		l=(Key_mouse.ch0-1024);//�ұ�����
		f=(Key_mouse.ch3-1024);	//��ߴ�ֱ
		p=(Key_mouse.ch2-1024);//�������	
		g=(Key_mouse.ch1-1024);//�������
		s1=RC_CtrlData.rc.s1;//������yaw����pitch	
		s2=RC_CtrlData.rc.s2;//Ħ���ֿ���
	  bolun=(RC_CtrlData.bolun-1024);
	  if(l==-1024) stop();
		else if(f!=0||l!=0||p!=0)
		{
				speed=(f*7);
				forward(speed);
					if(flag_genshui!=1)
				{
					speed=l*4;	
					nishizhen(speed);
					speed=p*4;
					right_pinyi(speed);
				}
				CM1SpeedPID.ref=CM1Speed_x+CM1Speed_y+CM1Speed_spin;
				CM2SpeedPID.ref=CM2Speed_x+CM2Speed_y+CM2Speed_spin;
				CM3SpeedPID.ref=CM3Speed_x+CM3Speed_y+CM3Speed_spin;
				CM4SpeedPID.ref=CM4Speed_x+CM4Speed_y+CM4Speed_spin;
			MOTORPID(CM1SpeedPID.ref,CM2SpeedPID.ref,CM3SpeedPID.ref,CM4SpeedPID.ref);	
		}
		
		
		 if(l==0&&f==0&&p==0)
		 {
	stop();
		 }

				IMU_G+=((float)(l+roll_angle)/20);
			G+=((float)(g)/3000);	
//		if(G>=60)
//		G=60;
//	if(G<=10)
//		G=10;
	Y+=((float)(bolun)/3000);	
//		if(Y>=300)
//		G=300;
//	if(G<=230)
//		G=230;
	if(s2==2)
	{
		SetFrictionWheelSpeed(0);
		shoot=0;
	}
	else 
	{
		SetFrictionWheelSpeed(2000);
	}
	
		if(s1==1&&flag_genshui==0) 
	{
		Old_YY=REAL_YY;
		flag_genshui=1;
	}
	else flag_genshui=0;

}


/******************************************************
�������Ա���̨pid���� yaw+pitch

******************************************************/
void GimbalPID(void)
{
	if(G>=6540)
		G=6540;
	if(G<=5399)
		G=5399;
	//��̨�������								
	GMPPositionPID.ref=G;
    GMPPositionPID.fdb=Yp; 
	GMYPositionPID.ref=2911;
    GMYPositionPID.fdb=Yy; 
	
PID_Calc(&GMYPositionPID);		
		PID_Calc(&GMPPositionPID);
	
		if(GMPPositionPID.output>=4000)
			GMPPositionPID.output=4000;
		if(GMPPositionPID.output<=-4000)
			GMPPositionPID.output=-4000;
		if(GMPPositionPID.output>=4000)
			GMPPositionPID.output=4000;
		if(GMPPositionPID.output<=-4000)
			GMPPositionPID.output=-4000;

		
		//Set_Gimbal_Current(CAN1, -(int16_t)GMPPositionPID.output, -(int16_t)GMYPositionPID.output);
//		Set_Gimbal_Current(CAN1,-(int16_t)GMYPositionPID.output,-(int16_t)GMPPositionPID.output);
				delay_ms(1);
			
		
		
}
/**************************
������ĵ����ٶȻ�
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
	

		if(CM1SpeedPID.output>=5000)
		{
			CM1SpeedPID.output=5000;	
		
		}
		if(CM1SpeedPID.output<=-5000)
		{
			CM1SpeedPID.output=-5000;	
			
		}
		
		if(CM2SpeedPID.output>=5000)
		{
			CM2SpeedPID.output=5000;	
		
		}
		if(CM2SpeedPID.output<=-5000)
		{
			CM2SpeedPID.output=-5000;	
			
		}
		
		if(CM3SpeedPID.output>=5000)
		{
			CM3SpeedPID.output=5000;	
		
		}
		if(CM3SpeedPID.output<=-5000)
		{
			CM3SpeedPID.output=-5000;	
			
		}
		
		if(CM4SpeedPID.output>=5000)
		{
			CM4SpeedPID.output=5000;	
		
		}
		if(CM4SpeedPID.output<=-50000)
		{
			CM4SpeedPID.output=-50000;	
			
		}


		Set_CM_Speed(CAN1,CM1SpeedPID.output,CM2SpeedPID.output,CM3SpeedPID.output,CM4SpeedPID.output);
	
		
}

void Sheetfadan(int f,int l)
{
    ShootMotorSpeedPID.ref=f;
    ShootMotorSpeedPID.fdb=M2_7;
	  fadanMotorSpeedPID.ref=l;
	  fadanMotorSpeedPID.fdb=M7;
	
    PID_Calc(&ShootMotorSpeedPID);
	  PID_Calc(&fadanMotorSpeedPID);
	if(ShootMotorSpeedPID.output>=4000)
		{
			ShootMotorSpeedPID.output=4000;	
		
		}
		if(ShootMotorSpeedPID.output<=-4000)
		{
			ShootMotorSpeedPID.output=-4000;	
			
		}
		
			if(fadanMotorSpeedPID.output>=4000)
		{
			fadanMotorSpeedPID.output=4000;	
		
		}
		if(fadanMotorSpeedPID.output<=-4000)
		{
			fadanMotorSpeedPID.output=-4000;	
			
		}

//		SetfadanwheelSpeed(ShootMotorSpeedPID.output);
//		fadanwheelSpeed(fadanMotorSpeedPID.output);
    delay_ms(1);
}


/**************
������Ħ����PID����
******************/
void FRICTIONPID(int f)
{
    BigShootMotorSpeedPID.ref=f;
    BigShootMotorSpeedPID.fdb=M8;
	
    PID_Calc(&BigShootMotorSpeedPID);
	if(BigShootMotorSpeedPID.output>=4000)
		{
			BigShootMotorSpeedPID.output=4000;	
		
		}
		if(BigShootMotorSpeedPID.output<=-4000)
		{
			BigShootMotorSpeedPID.output=-4000;	
			
		}

//		SetBigfadanwheelSpeed(BigShootMotorSpeedPID.output);
//		 delay_ms(1);
//Set_Current(CAN1, -(int16_t)GMYPositionPID.output, -(int16_t)GMPPositionPID.output,ShootMotorSpeedPID.output);
	//Set_FRICTION_Current(CAN1, ShootMotorSpeedPID.output);
}	

/**************************
cxgIMU����̨PID
***************************/
void IMU_PID(void)
{
	IMU_FDB=yaw_angle;

	//IMU_FDB=IMU_FDB*100;
	//��̨�������								
	GMYPositionPID.ref=IMU_G;
    GMYPositionPID.fdb=IMU_FDB; 
		if(IMU_G>=6600)
		IMU_G=6600;
		if(IMU_G<=-3400)
		IMU_G=-3000;
	
	//GMYPositionPID.ref=2950;
   // GMYPositionPID.fdb=Yy; 
	
PID_Calc(&GMYPositionPID);		
	//	PID_Calc(&GMPPositionPID);
	//Set_Gimbal_Current(CAN1, (int16_t)GMPPositionPID.output, -(int16_t)GMYPositionPID.output);
		

		
		if(GMPPositionPID.output>=2500)
			GMPPositionPID.output=2500;
		if(GMPPositionPID.output<=-2500)
			GMPPositionPID.output=-2500;
}
/******************************************************
�������Ա���̨�ٶ�pid���� yaw+pitch

******************************************************/
void SPEED_GimbalPID(void)
{
	if(G>=6640)
		G=6640;
	if(G<=5399)
		G=5399;
	//��̨�������	
	
	GMPSpeedPID.ref=((float)(g)/20);
    GMPSpeedPID.fdb=-MPU6050_Real_Data.Gyro_Y; 
	GMYSpeedPID.ref=1;
    GMYSpeedPID.fdb=MPU6050_Real_Data.Gyro_X; 
	
PID_Calc(&GMPSpeedPID);		
		PID_Calc(&GMYSpeedPID);
	
		if(GMPSpeedPID.output>=4000)
			GMPSpeedPID.output=4000;
		if(GMPSpeedPID.output<=-4000)
			GMPSpeedPID.output=-4000;
		if(GMYSpeedPID.output>=4000)
			GMYSpeedPID.output=4000;
		if(GMYSpeedPID.output<=-4000)
			GMYSpeedPID.output=-4000;

		
	
	//	Set_Gimbal_Current(CAN1,-(int16_t)GMYSpeedPID.output,(int16_t)GMPSpeedPID.output);
	delay_ms(1);
		
		
}
PC_Data	Key_mouse;
	
	
	/********************************************/
/* ����ֵ����갴�������ź�               */

/* ���������ԭʼң����ͨ��ֵCH0;CH1;CH2;CH3;���̷���ֵ;ǰ������ٶȣ�0~440��;����ƽ������ٶȣ�0~440��;���ǰ��ֵ;�������ֵ;����������ֵ;����Ҽ�����ֵ */

int8_t Key_Mouse(uint16_t RC_CH0,uint16_t RC_CH1,uint16_t RC_CH2,uint16_t RC_CH3,uint16_t Key,int16_t Forward_Backword_Value,int16_t Left_Right_Value,int16_t Y_Value,int16_t X_Value,uint8_t Mouse_Press_l,uint8_t Mouse_Press_r)
{
	float Key_Ctl_CH3=0,Key_Ctl_CH2=0,Key_Ctl_CH1=0;
	fire=Mouse_Press_l-Mouse_Press_r;
	
	switch (Key)
	{
		case 1:
		{
			Key_Ctl_CH3=1;
		}break;
		case 2:
		{
			Key_Ctl_CH3=-1;
		}break;
		case 4:
		{
			Key_Ctl_CH2=-1;
		}break;
		case 8:
		{
			Key_Ctl_CH2=1;
		}break;
		case 5:
		{
			Key_Ctl_CH2=-0.5f;
			Key_Ctl_CH3=0.5f;
		}break;
		case 9:
		{
			Key_Ctl_CH2=0.5f;
			Key_Ctl_CH3=0.5f;
		}break;
		case 6:
		{
			Key_Ctl_CH2=-0.5f;
			Key_Ctl_CH3=-0.5f;
		}break;		
		case 10:
		{
			Key_Ctl_CH2=0.5f;
			Key_Ctl_CH3=-0.5f;
		}break;	
		case 64:
		{
			Key_Ctl_CH1=-Forward_Backword_Value*0.5f;
		}break;
		case 128:
		{
			Key_Ctl_CH1=Forward_Backword_Value*0.5f;
		}break;
		case 65:
		{
			Key_Ctl_CH3=0.5f;
			Key_Ctl_CH1=-0.5*Forward_Backword_Value;
		}break;	
		case 129:
		{
			Key_Ctl_CH3=0.5;
			Key_Ctl_CH1=0.5*Forward_Backword_Value;
		}break;	
		case 66:
		{
			Key_Ctl_CH3=-0.5;
			Key_Ctl_CH1=-0.5*Forward_Backword_Value;
		}break;	
		case 130:
		{
			Key_Ctl_CH3=-0.5;
			Key_Ctl_CH1=0.5*Forward_Backword_Value;
		}break;			
		case 256:
		{
			Key_mouse.H_speed=1;
		}break;		
		case 1024:
		{
			Key_mouse.H_speed=2;
			Key_mouse.L_speed=2;
		}break;
		
		case 17:
		{
			Key_Ctl_CH3=1.5f;
		}break;				

		case 0:
		{
			Key_Ctl_CH3=0;
			Key_Ctl_CH2=0;
			Key_Ctl_CH1=0;
			Key_mouse.H_speed=1;
			Key_mouse.L_speed=1;
			
		}break;
		default :{}
	} 
	
	Key_mouse.ch3=RC_CH3 + Key_Ctl_CH3 * Forward_Backword_Value/0.8;			//ch3
	Key_mouse.ch2=RC_CH2 + Key_Ctl_CH2 * Left_Right_Value;			//ch2
	Key_mouse.ch1=RC_CH1 + Y_Value * -9 ;			//ch1
	Key_mouse.ch0=RC_CH0 +Key_Ctl_CH1*3+ X_Value * 9;			//ch0
	if(	Key_mouse.ch1>=1684)
	{
		Key_mouse.ch1=1684;
	}
	if(	Key_mouse.ch1<=364)
	{
		Key_mouse.ch1=364;
	}
	
	
	return fire;/* ����ֵ����갴�������ź�               */

/* ���������ԭʼң����ͨ��ֵCH0;CH1;CH2;CH3;���̷���ֵ;ǰ������ٶȣ�0~440��;����ƽ������ٶȣ�0~440��;���ǰ��ֵ;�������ֵ;����������ֵ;����Ҽ�����ֵ */

}

	
