#include "main.h"
int f,l,p,g,s1,s2,bolun,old_f=0;//遥控通道
int F=0,L=0,P=0,S1;
float G=265;//pitch轴遥控角度变量
float Y=350;//YAW轴遥控角度变量
float SPEED_LEVAL;//摩擦轮速度档位，2》1
int speed_number=0;  //摩擦轮速度
double IMU_G=-2000.00;//pitch轴IMU遥控角度变量
float IMU_FDB;//pitch轴IMU反馈角度

int8_t fire=0;

u8 flag_follow=1;



 int friction_quanshu=0;//用于速度控制角度时的权圈数
PID_Regulator_t GMPPositionPID = GIMBAL_MOTOR_PITCH_POSITION_PID_DEFAULT;//cxg 注释     
PID_Regulator_t GMPSpeedPID = GIMBAL_MOTOR_PITCH_SPEED_PID_DEFAULT;
PID_Regulator_t GMYPositionPID = GIMBAL_MOTOR_YAW_POSITION_PID_DEFAULT;			
PID_Regulator_t GMYSpeedPID = GIMBAL_MOTOR_YAW_SPEED_PID_DEFAULT;

PID_Regulator_t CMRotatePID = CHASSIS_MOTOR_ROTATE_PID_DEFAULT; //乔：
PID_Regulator_t CM1SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM2SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM3SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM4SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;

//PID_Regulator_t ShootMotorPositionPID = SHOOT_MOTOR_POSITION_PID_DEFAULT;      //shoot motor
PID_Regulator_t ShootMotorSpeedPID = SHOOT_MOTOR_SPEED_PID_DEFAULT;

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
//底盘控制任务
void CMControlLoop(void)
{  
	//底盘旋转量计算
	if(GetWorkState()==PREPARE_STATE) //启动阶段，底盘不旋转
	{
		ChassisSpeedRef.rotate_ref = 0;	 
	}
	else
	{
		 //底盘跟随编码器旋转PID计算
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
//	if(Is_Lost_Error_Set(LOST_ERROR_RC))      //如果遥控器丢失，强制将速度设定值reset
//	{
//		ChassisSpeedRef.forward_back_ref = 0;
//		ChassisSpeedRef.left_right_ref = 0;
//	}
	#if CHASSISMOTORTYPE//底盘电机
	CM1SpeedPID.ref =  -ChassisSpeedRef.forward_back_ref*3*0.45 + ChassisSpeedRef.left_right_ref*3*0.45 + ChassisSpeedRef.rotate_ref*9;
	CM2SpeedPID.ref = ChassisSpeedRef.forward_back_ref*3*0.45 + ChassisSpeedRef.left_right_ref*3*0.45 + ChassisSpeedRef.rotate_ref*9;
	CM3SpeedPID.ref = ChassisSpeedRef.forward_back_ref*3*0.45 - ChassisSpeedRef.left_right_ref*3*0.45 + ChassisSpeedRef.rotate_ref*9;
	CM4SpeedPID.ref = -ChassisSpeedRef.forward_back_ref*3*0.45 - ChassisSpeedRef.left_right_ref*3*0.45 + ChassisSpeedRef.rotate_ref*9;

	CM1SpeedPID.fdb = CM1Encoder.filter_rate;//反馈速度
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
	
	#if CALIBRATION//校准
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
	 if((GetWorkState() == STOP_STATE) ||Is_Serious_Error() || GetWorkState() == CALI_STATE || GetWorkState() == PREPARE_STATE)    //|| dead_lock_flag == 1紧急停车，编码器校准，无控制输入时都会使底盘控制停止
	 {
		 Set_CM_Speed(CAN1, 0,0,0,0);
	 }
	 else
	 {
		 Set_CM_Speed(CAN1, CHASSIS_SPEED_ATTENUATION * CM1SpeedPID.output, CHASSIS_SPEED_ATTENUATION * CM2SpeedPID.output, CHASSIS_SPEED_ATTENUATION * CM3SpeedPID.output, CHASSIS_SPEED_ATTENUATION * CM4SpeedPID.output);
   // GREEN_LED_ON();		 
	 } 
}
//发射机构射击电机任务
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
*工作状态切换状态机,与1ms定时中断同频率
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
	//如果从其他模式切换到prapare模式，要将一系列参数初始化
	if((lastWorkState != workState) && (workState == PREPARE_STATE))  
	{
		ControtLoopTaskInit();
		RemoteTaskInit();
	}
}




void GimbalYawControlModeSwitch(void)
{
	static uint8_t normalFlag = 0;   //正常工作模式标志
	static uint8_t standbyFlag = 1;  //IMU工作模式标志
	static uint32_t modeChangeDelayCnt = 0;
	static float angleSave = 0.0f;    //用于切换模式时保存切换前的角度值，用于角度给定值切换
	switch(GetWorkState())
	{
		case PREPARE_STATE:   //启动过程，加入斜坡
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
				GimbalRef.yaw_angle_dynamic_ref = angleSave;   //修改设定值为STANDBY状态下记录的最后一个ZGYROMODULEAngle值
				modeChangeDelayCnt = 0;   //delay清零
			}
			GMYPositionPID.ref = GimbalRef.yaw_angle_dynamic_ref;   //设定给定值
			GMYPositionPID.fdb = yaw_angle; 					//设定反馈值
			angleSave = yaw_angle;   //时刻保存IMU的值用于从NORMAL向STANDBY模式切换
		}break;
		case STANDBY_STATE:   //IMU模式
		{
			modeChangeDelayCnt++;
			if(modeChangeDelayCnt < STATE_SWITCH_DELAY_TICK)    //delay的这段时间与NORMAL_STATE一样
			{
				GMYPositionPID.ref = GimbalRef.yaw_angle_dynamic_ref;   //设定给定值
				GMYPositionPID.fdb = yaw_angle; 					//设定反馈值
				angleSave = yaw_angle;
			}
			else     //delay时间到，切换模式到IMU
			{
				if(normalFlag == 1)   //修改模式标志
				{
					normalFlag = 0;
					standbyFlag = 1;
					GimbalRef.yaw_angle_dynamic_ref = angleSave;    //保存的是delay时间段内保存的
				}
				GMYPositionPID.ref = GimbalRef.yaw_angle_dynamic_ref;   //设定给定值
				GMYPositionPID.fdb = yaw_angle; 					//设定反馈值	
				angleSave = yaw_angle;           //IMU模式时，保存ZGyro的值供模式切换时修改给定值使用						
			}
		}break;
		case STOP_STATE:    //停止工作模式
		{
			
		}break;
		case CALI_STATE:    //校准模式
		{
			
		}break;
	}	
}

//云台pitch轴控制程序
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
	GMPPositionPID.fdb = GMPitchEncoder.ecd_angle* GMPitchRamp.Calc(&GMPitchRamp);    //加入斜坡函数
	GMPPositionPID.Calc(&GMPPositionPID);   //得到pitch轴位置环输出控制量
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

//void SetGimbalMotorOutput(void)
//{
//	//云台控制输出								
//	if((GetWorkState() == STOP_STATE) ||Is_Serious_Error() || GetWorkState() == CALI_STATE)   
//	{
//		Set_Gimbal_Current(CAN1, 0, 0);     //yaw + pitch			
//	}
//	else
//	{		
//		Set_Gimbal_Current(CAN1, (int16_t)GMYSpeedPID.output, -(int16_t)GMPSpeedPID.output);     //yaw + pitch	
//	}		
//}


//控制任务初始化程序
void ControtLoopTaskInit(void)
{
	//计数初始化
	time_tick_1ms = 0;   //中断中的计数清零
	//程序参数初始化
	AppParamInit();
	//校准后参数偏差值初始化
	Sensor_Offset_Param_Init(&gAppParamStruct);
	//设置工作模式
	SetWorkState(PREPARE_STATE);
	//斜坡初始化
	GMPitchRamp.SetScale(&GMPitchRamp, PITCH_PREPARE_TIME_TICK_MS);
	GMYawRamp.SetScale(&GMYawRamp, YAW_PREPARE_TIME_TICK_MS);
	GMPitchRamp.ResetCounter(&GMPitchRamp);
	GMYawRamp.ResetCounter(&GMYawRamp);
	//云台给定角度初始化
	GimbalRef.pitch_angle_dynamic_ref = 0.0f;
	GimbalRef.yaw_angle_dynamic_ref = 0.0f;
    //监控任务初始化
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
    
	//PID初始化
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


//控制任务，放在timer6 1ms定时中断中执行
//void Control_Task(void)
//{
//	time_tick_1ms++;
//	WorkStateFSM();			//状态机检测
//	
//	//启动后根据磁力计的数据初始化四元数
//	if(time_tick_1ms <100)	
//	{
//		Init_Quaternion();
//	}
//	//平台稳定启动后，复位陀螺仪模块
////	if(time_tick_1ms == PREPARE_TIME_TICK_MS/2)
////	{
////		GYRO_RST();
////	}
//		
//	
//	//step 1: 云台控制
//	GimbalYawControlModeSwitch();   //模式切换处理，得到位置环的设定值和给定值
//	GMPitchControlLoop();
//	GMYawControlLoop();
//	SetGimbalMotorOutput();
//	
//	CalibrateLoop();   //校准任务，当接收到校准命令后才有效执行，否则直接跳过
//	//chassis motor control
//	if(time_tick_1ms%4 == 0)         //motor control frequency 4ms
//	{
//		//监控任务
//	//	SuperviseTask();    
//		//底盘控制任务
//		CMControlLoop();			
//		ShooterMControlLoop();       //发射机构控制任务
//	}

//	
//}


void remote(void)
{
	int speed;
	
		fire=Key_Mouse(RC_CtrlData.rc.ch0,RC_CtrlData.rc.ch1,RC_CtrlData.rc.ch2,RC_CtrlData.rc.ch3,RC_CtrlData.key.v,300,300, RC_CtrlData.mouse.y, RC_CtrlData.mouse.x,RC_CtrlData.mouse.press_l,RC_CtrlData.mouse.press_r);
		l=(Key_mouse.ch0-1024);//右边左右
		f=(Key_mouse.ch3-1024);	//左边垂直
		p=(Key_mouse.ch2-1024);//左边左右	
		g=(Key_mouse.ch1-1024);//左边上下
		s1=RC_CtrlData.rc.s1;//决定是yaw还是pitch	
		s2=RC_CtrlData.rc.s2;//摩擦轮控制
	  bolun=(RC_CtrlData.bolun-1024);
//	  if(l==-1024) stop();
//		else if(f!=0||l!=0||p!=0)
//		{
//				speed=(f*7);
//				forward(speed);
//				speed=l*4;	
//				nishizhen(speed);
//				speed=p*4;
//				right_pinyi(-speed);
//			CM1SpeedPID.ref=CM1Speed_x+CM1Speed_y+CM1Speed_spin;
//			CM2SpeedPID.ref=CM2Speed_x+CM2Speed_y+CM2Speed_spin;
//			CM3SpeedPID.ref=CM3Speed_x+CM3Speed_y+CM3Speed_spin;
//			CM4SpeedPID.ref=CM4Speed_x+CM4Speed_y+CM4Speed_spin;
//			MOTORPID(CM1SpeedPID.ref,CM2SpeedPID.ref,CM3SpeedPID.ref,CM4SpeedPID.ref);	
//		}
//		
//		
//		 if(l==0&&f==0&&p==0)
//		 {
//			CM1Speed_x=0;CM2Speed_x=0;CM3Speed_x=0;
//		  CM1Speed_y=0;CM2Speed_y=0;CM3Speed_y=0;
//			CM1Speed_spin=0;CM2Speed_spin=0;CM2Speed_spin=0;
//	    CM1SpeedPID.ref=CM1Speed_x+CM1Speed_y+CM1Speed_spin;
//			CM2SpeedPID.ref=CM2Speed_x+CM2Speed_y+CM2Speed_spin;
//			CM3SpeedPID.ref=CM3Speed_x+CM3Speed_y+CM3Speed_spin;
//			CM4SpeedPID.ref=CM4Speed_x+CM4Speed_y+CM4Speed_spin;
//			MOTORPID(CM1SpeedPID.ref,CM2SpeedPID.ref,CM3SpeedPID.ref,CM4SpeedPID.ref);
//		 }

//				IMU_G+=((float)(l+roll_angle)/20);
//			G+=((float)(g)/3000);	
//		 
//		 if(g!=0)
//		 {
//			 flag_follow=1;
//		 }
//		if(G>=135)
//		G=135;
//	if(G<=105)
//		G=105;
	//	 if(G>360-G) G=G-360;
		 
//	Y+=((float)(bolun)/3000);	
//		if(Y>=300)
//		G=300;
//	if(G<=230)
//		G=230;
//	if(s1==1)//左上角的按键，搬到最下面
//	{		
//		SPEED_LEVAL=0.5;
//		//BAIDONG_YAW_flag=0;	 
//	}
//if(s1==2)//左上角的按键，搬到最下面
//	{		
//		SPEED_LEVAL=1.5;
//		//BAIDONG_YAW_flag=0;	 
//	}
//	if(s1==3)
//	{
//		SPEED_LEVAL=1;
//		//BAIDONG_YAW_flag=0; 
//	}
//	
//	//cml:步兵跟随
//	
//  if(g==0)
//	{
//		if( REAL_YY-265>5 )
//		{
//			//MOTORPID(300,300,300,300);
//		}
//		else if(REAL_YY-265<-5)
//		{
//			//MOTORPID(-300,-300,-300,-300);
//		}
//		else if( (REAL_YY-265)<5 && (REAL_YY-265)>-5 && flag_follow )
//		{
//			stop();
//			flag_follow=0;
//		}
//	}
	
	
//	if(s2==1)
//	{
//		SetFrictionWheelSpeed(1090);
//		FRICTIONPID(4000/SPEED_LEVAL);
//	}
//	if(s2==3)
//	{
//		SetFrictionWheelSpeed(1090);
//		FRICTIONPID(4000/SPEED_LEVAL);
//	}	
//	if(s2==2)
//	{
//		if(fire==0)
//	{
//		FRICTIONPID(0);
//		SetFrictionWheelSpeed(1000);
//	}
//	if(fire==1)
//	{
//		SetFrictionWheelSpeed(1090);
//		FRICTIONPID(2000/SPEED_LEVAL);
//	}
//	if(fire==-1)
//	{
//		SetFrictionWheelSpeed(1090);
//		FRICTIONPID(-2000/SPEED_LEVAL);
//	}

//	}	
//if(s2==2)
//{
//		// friction_quanshu=0;
//		FRICTIONPID(0);
//	SetFrictionWheelSpeed(1000);	
//}
//if(s2!=2)
//{	
////if(SPEED_LEVAL==1)
////	{speed_number=1400;}
////	else
////		speed_number=1400;
//	if(s2==1)
//{
//	SetFrictionWheelSpeed(1400);
//		FRICTIONPID(2500/SPEED_LEVAL);
//}
//if(s2==3)
//{
//		SetFrictionWheelSpeed(1600);
//	FRICTIONPID(2500/SPEED_LEVAL);
//;}	
//
	if(Y<0)  Y+=360;
	if(Y>=360) Y-=360;
	Y+=((float)(bolun)/3000);	


	if(REAL_YY-Y>180){
			Y=Y+360;
	}
	else if(REAL_YY-Y<-180)	{Y=Y-360;}
}


/******************************************************
常晓光自编云台pid控制 yaw+pitch

******************************************************/
void GimbalPID(void)
{
	if(G>=6540)
		G=6540;
	if(G<=5399)
		G=5399;
	//云台控制输出								
	GMPPositionPID.ref=G;
    GMPPositionPID.fdb=Yp; 
	GMYPositionPID.ref=0;
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
	//	Set_Gimbal_Current(CAN1,-(int16_t)GMYPositionPID.output,-(int16_t)GMPPositionPID.output);
				delay_ms(1);
			
		
		
}
/**************************
常晓光的底盘速度环
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
		if(CM4SpeedPID.output<=-5000)
		{
			CM4SpeedPID.output=-5000;	
			
		}


		Set_CM_Speed(CAN1,CM1SpeedPID.output,CM2SpeedPID.output,CM3SpeedPID.output,CM4SpeedPID.output);
	
		
}
/**************
常晓光摩擦轮PID控制
******************/
void FRICTIONPID(int f)
{
    ShootMotorSpeedPID.ref=f;
    ShootMotorSpeedPID.fdb=M5;
	
    PID_Calc(&ShootMotorSpeedPID);
	if(ShootMotorSpeedPID.output>=4000)
		{
			ShootMotorSpeedPID.output=4000;	
		
		}
		if(ShootMotorSpeedPID.output<=-4000)
		{
			ShootMotorSpeedPID.output=-4000;	
			
		}


//Set_Current(CAN1, -(int16_t)GMYPositionPID.output, -(int16_t)GMPPositionPID.output,-ShootMotorSpeedPID.output);
		Set_Current(CAN1, -(int16_t)GMYPositionPID.output, -(int16_t)GMPPositionPID.output,0);
	//Set_FRICTION_Current(CAN1, ShootMotorSpeedPID.output);
delay_ms(1);
}	

/**************************
cxgIMU的云台PID
***************************/
void IMU_PID(void)
{
	IMU_FDB=yaw_angle;

	//IMU_FDB=IMU_FDB*100;
	//云台控制输出								
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
常晓光自编云台速度pid控制 yaw+pitch

******************************************************/
void SPEED_GimbalPID(void)
{
	if(G>=6640)
		G=6640;
	if(G<=5399)
		G=5399;
	//云台控制输出	
	
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
/* 返回值：鼠标按键按下信号               */

/* 输入参数：原始遥控器通道值CH0;CH1;CH2;CH3;键盘返回值;前后最大速度（0~440）;左右平移最大速度（0~440）;鼠标前后值;鼠标左右值;鼠标左键按下值;鼠标右键按下值 */

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
	
	
	return fire;/* 返回值：鼠标按键按下信号               */

/* 输入参数：原始遥控器通道值CH0;CH1;CH2;CH3;键盘返回值;前后最大速度（0~440）;左右平移最大速度（0~440）;鼠标前后值;鼠标左右值;鼠标左键按下值;鼠标右键按下值 */

}

	
