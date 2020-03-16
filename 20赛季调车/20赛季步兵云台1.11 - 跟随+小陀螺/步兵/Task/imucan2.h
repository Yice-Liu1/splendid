#ifndef __CAN2_H
#define __CAN2_H

#include "stm32f4xx.h"

#define RATE_BUF_SIZE 6
typedef struct{
	int32_t raw_value;   									//编码器不经处理的原始值
	int32_t last_raw_value;								//上一次的编码器原始值
	int32_t ecd_value;                       //经过处理后连续的编码器值
	int32_t diff;													//两次编码器之间的差值
	int32_t temp_count;                   //计数用
	uint8_t buf_count;								//滤波更新buf用
	int32_t ecd_bias;											//初始编码器值	
	int32_t ecd_raw_rate;									//通过编码器计算得到的速度原始值
	int32_t rate_buf[RATE_BUF_SIZE];	
	int32_t round_cnt;										//圈数
	int32_t filter_rate;											//速度
	float ecd_angle;											//角度
}Encoder;

typedef struct{
	int16_t	 	speed_rpm;
  int16_t  	real_current;
  int16_t  	given_current;
  uint8_t  	hall;
	uint16_t 	angle;
}Measure;

typedef struct 
{     
	uint8_t quat_euler:1;     
	uint8_t gyro_rangle:3;     
	uint8_t accel_rangle:2;     
	uint8_t imu_sensor_rotation:5;     
	uint8_t ahrs_rotation_sequence:3;     
	int16_t quat[4];     
	float quat_fp32[4];     
	int16_t euler_angle[3];     
	float euler_angle_fp32[3];     
	int16_t gyro_int16[3];     
	int16_t accel_int16[3];     
	int16_t mag_int16[3];     
	float gyro_fp32[3];     
	float accel_fp32[3];     
	uint16_t sensor_time;     
	uint16_t sensor_temperature;     
	int16_t sensor_control_temperature;     
	float gyro_sen;     
	float accel_sen; 
}rm_imu_data_t; 



void CAN2_Config(void);
void Quat_To_Angle();

#endif
