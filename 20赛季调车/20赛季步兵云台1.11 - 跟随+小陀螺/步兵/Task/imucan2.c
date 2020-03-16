#include "main.h"

#include "imucan2.h"
#define    RM_IMU_QUAT_ID  0x401 //四元数or欧拉角输出ID
#define    RM_IMU_GYRO_ID  0x402 //角速度输出
#define    RM_IMU_ACCEL_ID  0x403 //加速度输出
#define    RM_IMU_MAG_ID  0x404 //磁场输出
#define   RM_IMU_PARAM_ID  0x405 //配置信息

//转换成 m/s^2 
//加速度量程
#define  ACCEL_3G_SEN 0.0008974358974f 
#define  ACCEL_6G_SEN 0.00179443359375f 
#define  ACCEL_12G_SEN 0.0035888671875f 
#define  ACCEL_24G_SEN 0.007177734375f

//转换成 rad/s 
//陀螺仪量程
#define  GYRO_2000_SEN 0.00106526443603169529841533860381f 
#define  GYRO_1000_SEN 0.00053263221801584764920766930190693f 
#define  GYRO_500_SEN 0.00026631610900792382460383465095346f 
#define  GYRO_250_SEN 0.00013315805450396191230191732547673f 
#define  GYRO_125_SEN 0.000066579027251980956150958662738366f 

#define M_PI  (float)3.1415926535


uint32_t Std2;
float yaw,pitch,roll;

rm_imu_data_t rm_imu_data;


void CAN2_Config()
{

  	GPIO_InitTypeDef       GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef       NVIC_InitStructure;

    //使能相关时钟
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);	
	
	
    //初始化GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  //引脚复用映射配置
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2); 
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2); 
	  
  	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM=DISABLE;		//非时间触发通信模式  
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//软件自动离线管理	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART=ENABLE;	//禁止报文自动传送 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
  	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal ;	 //模式设置 
  	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq ;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=CAN_BS1_6tq; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=3;  //分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN2, &CAN_InitStructure);   // 初始化CAN1 
    
		//配置过滤器
 	  CAN_FilterInitStructure.CAN_FilterNumber=14;	  
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; 
  	CAN_FilterInit(&CAN_FilterInitStructure);
		

	
	  CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

}  

void Quat_To_Angle()//四元数解算
{
	yaw=180+(180/M_PI)*atan2(rm_imu_data.quat_fp32[0] * rm_imu_data.quat_fp32[3] + rm_imu_data.quat_fp32[1] * rm_imu_data.quat_fp32[2],  rm_imu_data.quat_fp32[0] * rm_imu_data.quat_fp32[0] + rm_imu_data.quat_fp32[1] * rm_imu_data.quat_fp32[1] - 0.5);
  pitch=(180/M_PI)*asin(2*(rm_imu_data.quat_fp32[0]*rm_imu_data.quat_fp32[2]-rm_imu_data.quat_fp32[1]*rm_imu_data.quat_fp32[3]));
	roll=(180/M_PI)*atan2(rm_imu_data.quat_fp32[0] * rm_imu_data.quat_fp32[1] + rm_imu_data.quat_fp32[2] * rm_imu_data.quat_fp32[3], rm_imu_data.quat_fp32[0] * rm_imu_data.quat_fp32[0] + rm_imu_data.quat_fp32[3] * rm_imu_data.quat_fp32[3] - 0.5); 
}


void CAN2_RX0_IRQHandler(void)
{	
    CanRxMsg rx_message;
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET) 
    {			      
		    CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
        CAN_Receive(CAN2, CAN_FIFO0, &rx_message);
		    Std2=rx_message.StdId;
				switch(Std2) 
				{     
					case RM_IMU_PARAM_ID:     //配置信息
					{         
						rm_imu_data.accel_rangle = rx_message.Data[0] &0x0F;         
						rm_imu_data.gyro_rangle = (rx_message.Data[0] &0xF0) >> 4;       
						rm_imu_data.sensor_control_temperature = rx_message.Data[2];        
						rm_imu_data.imu_sensor_rotation = rx_message.Data[3] & 0x1F;         
						rm_imu_data.ahrs_rotation_sequence = (rx_message.Data[3] & 0xE0) >> 5;       
						rm_imu_data.quat_euler = rx_message.Data[4] & 0x01;         
						switch(rm_imu_data.gyro_rangle)		
						{ 
							case 0: rm_imu_data.gyro_sen = GYRO_2000_SEN; break;             
							case 1: rm_imu_data.gyro_sen = GYRO_1000_SEN; break;             
							case 2: rm_imu_data.gyro_sen = GYRO_500_SEN; break;             
							case 3: rm_imu_data.gyro_sen = GYRO_250_SEN; break;             
							case 4: rm_imu_data.gyro_sen = GYRO_125_SEN; break;         
						}
				
						switch(rm_imu_data.accel_rangle)         
						{             
							case 0: rm_imu_data.accel_sen = ACCEL_3G_SEN; break;             
							case 1: rm_imu_data.accel_sen = ACCEL_6G_SEN; break;             
							case 2: rm_imu_data.accel_sen = ACCEL_12G_SEN; break;             
							case 3: rm_imu_data.accel_sen = ACCEL_24G_SEN; break;         
						}         
				
						break;     
					} 		
				
					case RM_IMU_QUAT_ID:    
					{         
						if(rm_imu_data.quat_euler && rx_message.DLC == 6)   //若设置欧拉角输出，得到欧拉角
						{             
							memcpy(rm_imu_data.euler_angle, rx_message.Data, rx_message.DLC);             
							rm_imu_data.euler_angle_fp32[0] = rm_imu_data.euler_angle[0] * 0.0001f;             
							rm_imu_data.euler_angle_fp32[1] = rm_imu_data.euler_angle[1] * 0.0001f;             
							rm_imu_data.euler_angle_fp32[2] = rm_imu_data.euler_angle[2] * 0.0001f;         
						}         
						
						else if(rm_imu_data.quat_euler == 0 && rx_message.DLC == 8) //若设置四元数输出，得到四元数
						{             
							memcpy(rm_imu_data.quat, rx_message.Data, rx_message.DLC);             
							rm_imu_data.quat_fp32[0] = rm_imu_data.quat[0] * 0.0001f;             
							rm_imu_data.quat_fp32[1] = rm_imu_data.quat[1] * 0.0001f;             
							rm_imu_data.quat_fp32[2] = rm_imu_data.quat[2] * 0.0001f;             
							rm_imu_data.quat_fp32[3] = rm_imu_data.quat[3] * 0.0001f;         
						}         
							break;     
					} 
			
					case RM_IMU_GYRO_ID:     //角速度输出
					{         
						memcpy(rm_imu_data.gyro_int16, rx_message.Data,6);         
						rm_imu_data.gyro_fp32[0] = rm_imu_data.gyro_int16[0] * rm_imu_data.gyro_sen;         
						rm_imu_data.gyro_fp32[1] = rm_imu_data.gyro_int16[1] * rm_imu_data.gyro_sen;         
						rm_imu_data.gyro_fp32[2] = rm_imu_data.gyro_int16[2] * rm_imu_data.gyro_sen;         
						rm_imu_data.sensor_temperature = (int16_t)((rx_message.Data[6] << 3) | (rx_message.Data[7] >> 5));         
						if (rm_imu_data.sensor_temperature > 1023)         
						{             
							rm_imu_data.sensor_temperature -= 2048;         
						}         
						 break;     
					} 
			
					case RM_IMU_ACCEL_ID:     //加速度输出
					{         
						memcpy(rm_imu_data.accel_int16, rx_message.Data,6);         
						rm_imu_data.accel_fp32[0] = rm_imu_data.accel_int16[0] * rm_imu_data.accel_sen;         
						rm_imu_data.accel_fp32[1] = rm_imu_data.accel_int16[1] * rm_imu_data.accel_sen;         
						rm_imu_data.accel_fp32[2] = rm_imu_data.accel_int16[2] * rm_imu_data.accel_sen;         
						memcpy(&rm_imu_data.sensor_time, (rx_message.Data + 6), 2);         
						break;     
					}   
					
					case RM_IMU_MAG_ID:     //磁场信息
					{         
						memcpy(rm_imu_data.mag_int16, rx_message.Data,6);         
						break;     
					} 
			}
					Quat_To_Angle();
    }
}








