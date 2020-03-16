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
}Encoder;             //�йر������Ľṹ����������

typedef struct{
	int16_t	 	speed_rpm;
  int16_t  	real_current;
  int16_t  	given_current;
  uint8_t  	hall;
	uint16_t 	angle;
}Measure;             //����ֵ�ṹ��

void CAN2_Config(void);

#endif
