#include "main.h"


//���䰲������


#define dt                  0.0015//?????????
#define R_angle          0.69 //????????(??????)
#define Q_angle          0.0001//????????
#define Q_gyro          0.0003 //????????  ????????????????
//float gyro_m;
//float incAngle;
static float kalmananglepitch=0;
static float kalmanangleyaw=0;
static float kalmanangleroll=0;

//���䰲���������Ϸ�
volatile float exInt, eyInt, ezInt;  // ������
volatile float q0 = 1.0f;
volatile float q1 = 0.0f;
volatile float q2 = 0.0f;
volatile float q3 = 0.0f;

volatile float mygetqval[9];	//���ڴ�Ŵ�����ת�����������
static volatile float gx, gy, gz, ax, ay, az, mx, my, mz;   //��������ڴ��ļ���

static volatile float q[4]; //����Ԫ��
volatile uint32_t lastUpdate, now; // �������ڼ��� ��λ us
volatile float angle[3] = {0};
volatile float yaw_temp,pitch_temp,roll_temp;
volatile float last_yaw_temp,last_pitch_temp,last_roll_temp;
volatile float yaw_angle,pitch_angle,roll_angle; //ʹ�õ��ĽǶ�ֵ






// Fast inverse square-root
/**************************ʵ�ֺ���********************************************
*����ԭ��:	   float invSqrt(float x)
*��������:	   ���ټ��� 1/Sqrt(x) 	
��������� Ҫ�����ֵ
��������� ���
*******************************************************************************/
float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void Init_Quaternion
*��������:	 ��ʼ����Ԫ��
��������� ��ǰ�Ĳ���ֵ��
���������û��
*******************************************************************************/
//��ʼ��IMU����
#define BOARD_DOWN 0   //�������泯�°ڷ�

void Init_Quaternion()//���ݲ������ݣ���ʼ��q0,q1,q2.q3���Ӷ��ӿ������ٶ�
{
	int16_t hx,hy,hz;
//	HMC58X3_getlastValues(&hx,&hy,&hz);
	#ifdef BOARD_DOWN
	if(hx<0 && hy <0)   //OK
	{
		if(fabs(hx/hy)>=1)
		{
			q0 = -0.005;
			q1 = -0.199;
			q2 = 0.979;
			q3 = -0.0089;
		}
		else
		{
			q0 = -0.008;
			q1 = -0.555;
			q2 = 0.83;
			q3 = -0.002;
		}
		
	}
	else if (hx<0 && hy > 0) //OK
	{
		if(fabs(hx/hy)>=1)   
		{
			q0 = 0.005;
			q1 = -0.199;
			q2 = -0.978;
			q3 = 0.012;
		}
		else
		{
			q0 = 0.005;
			q1 = -0.553;
			q2 = -0.83;
			q3 = -0.0023;
		}
		
	}
	else if (hx > 0 && hy > 0)   //OK
	{
		if(fabs(hx/hy)>=1)
		{
			q0 = 0.0012;
			q1 = -0.978;
			q2 = -0.199;
			q3 = -0.005;
		}
		else
		{
			q0 = 0.0023;
			q1 = -0.83;
			q2 = -0.553;
			q3 = 0.0023;
		}
		
	}
	else if (hx > 0 && hy < 0)     //OK
	{
		if(fabs(hx/hy)>=1)
		{
			q0 = 0.0025;
			q1 = 0.978;
			q2 = -0.199;
			q3 = 0.008;			
		}
		else
		{
			q0 = 0.0025;
			q1 = 0.83;
			q2 = -0.56;
			q3 = 0.0045;
		}		
	}
	#else
		if(hx<0 && hy <0)
	{
		if(fabs(hx/hy)>=1)
		{
			q0 = 0.195;
			q1 = -0.015;
			q2 = 0.0043;
			q3 = 0.979;
		}
		else
		{
			q0 = 0.555;
			q1 = -0.015;
			q2 = 0.006;
			q3 = 0.829;
		}
		
	}
	else if (hx<0 && hy > 0)
	{
		if(fabs(hx/hy)>=1)
		{
			q0 = -0.193;
			q1 = -0.009;
			q2 = -0.006;
			q3 = 0.979;
		}
		else
		{
			q0 = -0.552;
			q1 = -0.0048;
			q2 = -0.0115;
			q3 = 0.8313;
		}
		
	}
	else if (hx>0 && hy > 0)
	{
		if(fabs(hx/hy)>=1)
		{
			q0 = -0.9785;
			q1 = 0.008;
			q2 = -0.02;
			q3 = 0.195;
		}
		else
		{
			q0 = -0.9828;
			q1 = 0.002;
			q2 = -0.0167;
			q3 = 0.5557;
		}
		
	}
	else if (hx > 0 && hy < 0)
	{
		if(fabs(hx/hy)>=1)
		{
			q0 = -0.979;
			q1 = 0.0116;
			q2 = -0.0167;
			q3 = -0.195;			
		}
		else
		{
			q0 = -0.83;
			q1 = 0.014;
			q2 = -0.012;
			q3 = -0.556;
		}		
	}
	#endif

	//����hx hy hz���ж�q��ֵ��ȡ�ĸ������ֵ���ƽ�����,��ʼֵ������ŷ����ת������Ԫ������õ�
	 
}


	


/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void IMU_getValues(volatile float * values)
*��������:	 ��ȡ���ٶ� ������ ������ �ĵ�ǰֵ  
��������� �������ŵ������׵�ַ
���ٶ�ֵ��ԭʼ���ݣ�-8192-+8192
���ٶ�ֵ��deg/s
������ֵ��ԭʼ����
���������û��
*******************************************************************************/
void IMU_getValues(volatile float * values) 
{  
	int16_t accgyroval[6];
	int i;
	//��ȡ���ٶȺ������ǵĵ�ǰADC
	MPU6050_getMotion6(&accgyroval[0], &accgyroval[1], &accgyroval[2], &accgyroval[3], &accgyroval[4], &accgyroval[5]);
	MPU6050_Raw_Data.Accel_X = accgyroval[0];
	MPU6050_Raw_Data.Accel_Y = accgyroval[1];
	MPU6050_Raw_Data.Accel_Z = accgyroval[2];
	MPU6050_Raw_Data.Gyro_X = accgyroval[3];
	MPU6050_Raw_Data.Gyro_Y = accgyroval[4];
	MPU6050_Raw_Data.Gyro_Z = accgyroval[5];
	/******************************/
		MPU6050_Raw_Data.Mag_X = values[6];
		MPU6050_Raw_Data.Mag_Y = values[7];
		MPU6050_Raw_Data.Mag_Z = values[8];
	
    for(i = 0; i<6; i++) {
      if(i < 3) {
        values[i] = (float) accgyroval[i];
      }
      else {
        values[i] = ((float) accgyroval[i]) / 32.8f; //ת�ɶ�ÿ��
		//�����Ѿ������̸ĳ��� 1000��ÿ��  32.8 ��Ӧ 1��ÿ��
      }
    }
//    HMC58X3_mgetValues(&values[6]);	//��ȡ�����Ƶ�ADCֵ
	
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void IMU_AHRSupdate
*��������:	 ����AHRS ������Ԫ��         ���䰲�������˲�
��������� ��ǰ�Ĳ���ֵ��
���������û��
*******************************************************************************/
#define Kp 2.0f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.01f   // integral gain governs rate of convergence of gyroscope biases
void IMU_AHRSupdate(void) {
  /*  float norm;
    float hx, hy, hz, bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez,halfT;
    float tempq0,tempq1,tempq2,tempq3;

    float q0q0 = q0*q0;
    float q0q1 = q0*q1;
    float q0q2 = q0*q2;
    float q0q3 = q0*q3;
    float q1q1 = q1*q1;
    float q1q2 = q1*q2;
    float q1q3 = q1*q3;
    float q2q2 = q2*q2;   
    float q2q3 = q2*q3;
    float q3q3 = q3*q3;   */

	  float halfT;
	/*�������˲����붨��*/
	  float K_0,T_0,U_0;//���п��������������һ����ֵ�����ڼ������Ź���ֵ
    float K_1,T_1,U_1;//���п���������ĺ��������ڼ������Ź���ֵ��ƫ��
    float Y_0,P_0,L_0;
    float Y_1,P_1,L_1;
    float Rate1,Rate2,Rate3;//ȥ��ƫ���Ľ��ٶ�
    float Pdot[4],YPdot[4],Rpdot[4];//����Э���̲��΢�־���
    float angle_err,angle_err2,angle_err3;//�Ƕ�ƫ��
    float Pitchcount,Yawcount,Rollcount;//���������
    static float q_bias = 0,q_bias_Y=0,q_bias_R=0;        //�����ǵ�ƫ��               
    static float P[2][2] = {{1,0},{0,1}},Y[2][2] = {{1,0},{0,1}},R[2][2] = {{1,0},{0,1}};//����Э�������
/*�������˲����붨������*/
    gx = mygetqval[3] * M_PI/180;
    gy = mygetqval[4] * M_PI/180;
    gz = mygetqval[5] * M_PI/180;
    ax = mygetqval[0];
    ay = mygetqval[1];
    az = mygetqval[2];
    mx = mygetqval[6];
    my = mygetqval[7];
    mz = mygetqval[8];		

    now = Get_Time_Micros();  //��ȡʱ�� ��λ��us   
    if(now<lastUpdate)
    {
    //halfT =  ((float)(now + (0xffffffff- lastUpdate)) / 2000000.0f);   //  uint 0.5s
    }
    else	
    {
        halfT =  ((float)(now - lastUpdate) / 2000000.0f);
    }
    lastUpdate = now;	//����ʱ��
		
		
		/*�������㷨����*/
		Rate1 = mygetqval[3] - q_bias;
//??????????????  
/*****Pitch,Yaw,Roll�Ῠ�����˲�*******/		
Pdot[0] = Q_angle - P[0][1] - P[1][0];//??????       
Pdot[1] = - P[1][1];                       
Pdot[2] = - P[1][1];                                 
Pdot[3] = Q_gyro;//??????                       
kalmananglepitch+= Rate1 * dt; //���ٶȻ��ֵó��Ƕ�
P[0][0] += Pdot[0] * dt; //����Э�������
P[0][1] += Pdot[1] * dt;
P[1][0] += Pdot[2] * dt;
P[1][1] += Pdot[3] * dt;
angle_err = mygetqval[0] - kalmananglepitch; //����ǶȲ�
Pitchcount = R_angle + P[0][0];
K_0 = P[0][0] / Pitchcount; //���㿨��������
K_1 = P[1][0] / Pitchcount;
Y_0 = P[0][0]; 
Y_1 = P[0][1];
P[0][0] -= K_0 * Y_0; //����Э�������
P[0][1] -= K_0 * Y_1;
P[1][0] -= K_1 * Y_0;
P[1][1] -= K_1 * Y_1;
kalmananglepitch+= K_0 * angle_err; //???????
q_bias += K_1 * angle_err;//?????????


Rate2 = mygetqval[4] - q_bias_Y;
YPdot[0] = Q_angle - Y[0][1] - Y[1][0];//??????       
YPdot[1] = - Y[1][1];                       
YPdot[2] = - Y[1][1];                                 
YPdot[3] = Q_gyro;//??????                       
kalmanangleyaw+= Rate2 * dt; //���ٶȻ��ֵó��Ƕ�
Y[0][0] += YPdot[0] * dt; //����Э�������
Y[0][1] += YPdot[1] * dt;
Y[1][0] += YPdot[2] * dt;
Y[1][1] += YPdot[3] * dt;
angle_err2 = mygetqval[1] - kalmanangleyaw; //����ǶȲ�
Yawcount = R_angle + Y[0][0];
T_0 = Y[0][0] / Pitchcount; //���㿨��������
T_1 = Y[1][0] / Pitchcount;
P_0 = Y[0][0]; 
P_1 = Y[0][1];
Y[0][0] -= T_0 * P_0; //����Э�������
Y[0][1] -= T_0 * P_1;
Y[1][0] -= T_1 * P_0;
Y[1][1] -= T_1 * P_1;
kalmananglepitch+= T_0 * angle_err2; //???????
q_bias_Y += T_1 * angle_err2;//?????????

Rate3 = mygetqval[5] - q_bias_R;
Rpdot[0] = Q_angle - R[0][1] - R[1][0];//??????       
Rpdot[1] = - R[1][1];                       
Rpdot[2] = - R[1][1];                                 
Rpdot[3] = Q_gyro;//??????                       
kalmanangleroll+= Rate3 * dt; //���ٶȻ��ֵó��Ƕ�
R[0][0] += Rpdot[0] * dt; //����Э�������
R[0][1] += Rpdot[1] * dt;
R[1][0] += Rpdot[2] * dt;
R[1][1] += Rpdot[3] * dt;
angle_err3 = mygetqval[2] - kalmanangleroll; //����ǶȲ�
Rollcount = R_angle + R[0][0];
U_0 = R[0][0] / Pitchcount; //���㿨��������
U_1 = R[1][0] / Pitchcount;
L_0 = R[0][0]; 
L_1 = R[0][1];
R[0][0] -= U_0 * L_0; //����Э�������
R[0][1] -= U_0 * L_1;
R[1][0] -= U_1 * L_0;
R[1][1] -= U_1 * L_1;
kalmanangleroll+= U_0 * angle_err3; //???????
q_bias_R += U_1 * angle_err;//?????????

		
		/*//�������˲�ע�����²���
    //������ƽ�����㷨
    norm = invSqrt(ax*ax + ay*ay + az*az);       
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;
    //�ѼӼƵ���ά����ת�ɵ�λ������
    norm = invSqrt(mx*mx + my*my + mz*mz);          
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm; 
    // compute reference direction of flux
    hx = 2.0f*mx*(0.5f - q2q2 - q3q3) + 2.0f*my*(q1q2 - q0q3) + 2.0f*mz*(q1q3 + q0q2);
    hy = 2.0f*mx*(q1q2 + q0q3) + 2.0f*my*(0.5f - q1q1 - q3q3) + 2.0f*mz*(q2q3 - q0q1);
    hz = 2.0f*mx*(q1q3 - q0q2) + 2.0f*my*(q2q3 + q0q1) + 2.0f*mz*(0.5f - q1q1 - q2q2);         
    bx = sqrt((hx*hx) + (hy*hy));
    bz = hz; 
    // estimated direction of gravity and flux (v and w)
    vx = 2.0f*(q1q3 - q0q2);
    vy = 2.0f*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;
    wx = 2.0f*bx*(0.5f - q2q2 - q3q3) + 2.0f*bz*(q1q3 - q0q2);
    wy = 2.0f*bx*(q1q2 - q0q3) + 2.0f*bz*(q0q1 + q2q3);
    wz = 2.0f*bx*(q0q2 + q1q3) + 2.0f*bz*(0.5f - q1q1 - q2q2);  
    // error is sum of cross product between reference direction of fields and direction measured by sensors
    ex = (ay*vz - az*vy) + (my*wz - mz*wy);
    ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
    ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

    if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
    {
        exInt = exInt + ex * Ki * halfT;
        eyInt = eyInt + ey * Ki * halfT;	
        ezInt = ezInt + ez * Ki * halfT;
        // �ò���������PI����������ƫ
        gx = gx + Kp*ex + exInt;
        gy = gy + Kp*ey + eyInt;
        gz = gz + Kp*ez + ezInt;
    }
    // ��Ԫ��΢�ַ���
    tempq0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    tempq1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    tempq2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    tempq3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

    // ��Ԫ���淶��
    norm = invSqrt(tempq0*tempq0 + tempq1*tempq1 + tempq2*tempq2 + tempq3*tempq3);
    q0 = tempq0 * norm;
    q1 = tempq1 * norm;
    q2 = tempq2 * norm;
    q3 = tempq3 * norm;
		
		//�������˲�ע�����ϲ���*/

}


/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void IMU_getQ(float * q)
*��������:	 ������Ԫ�� ���ص�ǰ����Ԫ����ֵ
��������� ��Ҫ�����Ԫ���������׵�ַ
���������û��
*******************************************************************************/

void IMU_getQ(volatile float * q) 
{
    IMU_getValues(mygetqval);	 //��ȡԭʼ����,���ٶȼƺʹ�������ԭʼֵ��������ת������deg/s
    IMU_AHRSupdate();
    q[0] = q0; //���ص�ǰֵ
    q[1] = q1;
    q[2] = q2;
    q[3] = q3;
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void IMU_getYawPitchRoll(float * angles)
*��������:	 ������Ԫ�� ���ص�ǰ��������̬����
��������� ��Ҫ�����̬�ǵ������׵�ַ
���������û��
*******************************************************************************/
void IMU_getYawPitchRoll(volatile float * angles) 
{  
     volatile float gx=0.0, gy=0.0, gz=0.0; //������������
   // IMU_getQ(q); //����ȫ����Ԫ��
	IMU_getValues(mygetqval);
	IMU_AHRSupdate();
   /* //��Ԫ��ת����ŷ���ǣ��������Ǻ������㼴��
    angles[0] = -atan2(2 * q[1] * q[2] + 2 * q[0] * q[3], -2 * q[2]*q[2] - 2 * q[3] * q[3] + 1)* 180/M_PI; // yaw        -pi----pi
    angles[1] = -asin(-2 * q[1] * q[3] + 2 * q[0] * q[2])* 180/M_PI; // pitch    -pi/2    --- pi/2 
    angles[2] = atan2(2 * q[2] * q[3] + 2 * q[0] * q[1], -2 * q[1] * q[1] - 2 * q[2] * q[2] + 1)* 180/M_PI; // roll       -pi-----pi  
	*/
	angles[0]=kalmananglepitch;
	angles[1]=kalmanangleyaw;
	angles[2]=kalmanangleroll;
}

static int yaw_count = 0;
void GetPitchYawGxGyGz()
{
	MPU6050_Real_Data.Gyro_X = mygetqval[3];	//������ٶ�
	MPU6050_Real_Data.Gyro_Y = -mygetqval[4];
	MPU6050_Real_Data.Gyro_Z = mygetqval[5];

	last_yaw_temp = yaw_temp;
	yaw_temp = angle[0]; 
	if(yaw_temp-last_yaw_temp>=330)  //yaw��ǶȾ����������������
	{
		yaw_count--;
	}
	else if (yaw_temp-last_yaw_temp<=-330)
	{
		yaw_count++;
	}
	yaw_angle = 1*yaw_temp + yaw_count*360;  //yaw��Ƕȣ���������ĳ�2������
	pitch_angle = angle[1];
    roll_angle = angle[2];	
}


