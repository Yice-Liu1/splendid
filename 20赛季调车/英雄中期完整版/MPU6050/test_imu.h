/**
  *@file test_imu.h
  *@date 2016-12-12
  *@author Albert.D
  *@brief 
  */
  
#ifndef _TEST__IMU_H
#define _TEST__IMU_H


#include "main.h"

#define MPU6050_NSS_Low()  GPIO_ResetBits(GPIOF, GPIO_Pin_6)
#define MPU6050_NSS_High() GPIO_SetBits(GPIOF, GPIO_Pin_6)

typedef struct
{
  int16_t ax;
  int16_t ay;
  int16_t az;
  
  int16_t temp;
  
  int16_t gx;
  int16_t gy;
  int16_t gz;
  
  int16_t mx;
  int16_t my;
  int16_t mz;
}IMUDataTypedef;

typedef struct __MPU6050_RAW_Data__
{
    short Accel_X;  //寄存器原生X轴加速度表示值
    short Accel_Y;  //寄存器原生Y轴加速度表示值
    short Accel_Z;  //寄存器原生Z轴加速度表示值
    short Temp;     //寄存器原生温度表示值
    short Gyro_X;   //寄存器原生X轴陀螺仪表示值
    short Gyro_Y;   //寄存器原生Y轴陀螺仪表示值
    short Gyro_Z;   //寄存器原生Z轴陀螺仪表示值
	short Mag_X;   //寄存器原生X轴陀螺仪表示值
    short Mag_Y;   //寄存器原生Y轴陀螺仪表示值
    short Mag_Z;   //寄存器原生Z轴陀螺仪表示值
	
}MPU6050_RAW_DATA;

typedef struct __MPU6050_REAL_Data__
{
    float Accel_X;  //转换成实际的X轴加速度，
    float Accel_Y;  //转换成实际的Y轴加速度，
    float Accel_Z;  //转换成实际的Z轴加速度，
    float Temp;     //转换成实际的温度，单位为摄氏度
    float Gyro_X;   //转换成实际的X轴角加速度，
    float Gyro_Y;   //转换成实际的Y轴角加速度，
    float Gyro_Z;   //转换成实际的Z轴角加速度
	  float Mag_X;   //转换成实际的X轴角加速度，
    float Mag_Y;   //转换成实际的Y轴角加速度，
    float Mag_Z;   //转换成实际的Z轴角加速度
	
}MPU6050_REAL_DATA;

typedef __packed struct
{
	int16_t MaxMagX;
	int16_t MaxMagY;
	int16_t MaxMagZ;
	int16_t MinMagX;
	int16_t MinMagY;
	int16_t MinMagZ;
}MagMaxMinData_t;

extern uint8_t MPU_id;
extern IMUDataTypedef imu_data;
extern int16_t MPU6050_FIFO[6][11];//[0]-[9]为最近10次数据 [10]为10次数据的平均值
extern volatile MPU6050_RAW_DATA MPU6050_Raw_Data;
extern volatile MPU6050_REAL_DATA MPU6050_Real_Data;
extern MagMaxMinData_t MagMaxMinData;

uint8_t MPU6050_Init(void);
uint8_t MPU6050_Write_Reg(uint8_t const reg, uint8_t const data);
uint8_t MPU6050_Read_Reg(uint8_t const reg);
uint8_t MPU6050_Read_Regs(uint8_t const regAddr, uint8_t *pData, uint8_t len);
void IMU_Get_Data(void);
void MPU6050_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);
void MPU6050_AverageFilter(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz);
uint8_t IST8310_Init(void);
//static uint8_t IST_Reg_Read_By_MPU(uint8_t addr);
//static void MPU_Auto_Read_IST_config(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num);
#endif

