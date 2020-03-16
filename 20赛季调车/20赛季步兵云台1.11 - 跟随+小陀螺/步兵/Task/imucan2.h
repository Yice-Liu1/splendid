#ifndef __CAN2_H
#define __CAN2_H

#include "stm32f4xx.h"

#define RATE_BUF_SIZE 6
typedef struct{
	int32_t raw_value;   									//���������������ԭʼֵ
	int32_t last_raw_value;								//��һ�εı�����ԭʼֵ
	int32_t ecd_value;                       //��������������ı�����ֵ
	int32_t diff;													//���α�����֮��Ĳ�ֵ
	int32_t temp_count;                   //������
	uint8_t buf_count;								//�˲�����buf��
	int32_t ecd_bias;											//��ʼ������ֵ	
	int32_t ecd_raw_rate;									//ͨ������������õ����ٶ�ԭʼֵ
	int32_t rate_buf[RATE_BUF_SIZE];	
	int32_t round_cnt;										//Ȧ��
	int32_t filter_rate;											//�ٶ�
	float ecd_angle;											//�Ƕ�
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
