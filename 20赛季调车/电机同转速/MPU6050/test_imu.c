/**
  *@file test_imu.c
  *@date 2016-12-12
  *@author Albert.D
  *@brief 
  */
  
#include "main.h"
#include "mpu6500_reg.h"
#include "IST8310_reg.h"

uint8_t MPU_id = 0;

IMUDataTypedef imu_data = {0,0,0,0,0,0,0,0,0,0};

IMUDataTypedef imu_data_offest = {0,0,0,0,0,0,0,0,0,0};


int16_t MPU6050_FIFO[6][11] = {0};//[0]-[9]为最近10次数据 [10]为10次数据的平均值
volatile MPU6050_RAW_DATA    MPU6050_Raw_Data;    //原始数据
volatile MPU6050_REAL_DATA   MPU6050_Real_Data;
MagMaxMinData_t MagMaxMinData;
//Write a register to MPU6050
uint8_t MPU6050_Write_Reg(uint8_t const reg, uint8_t const data)
{
  static uint8_t MPU_Tx;
  
  MPU6050_NSS_Low();
  
  MPU_Tx = reg&0x7f;
  SPI5_ReadWriteByte(MPU_Tx);
  MPU_Tx = data;
  SPI5_ReadWriteByte(MPU_Tx);
  
  MPU6050_NSS_High();
  return 0;
}

//Read a register from MPU6050
uint8_t MPU6050_Read_Reg(uint8_t const reg)
{
  static uint8_t MPU_Rx, MPU_Tx;
  
  MPU6050_NSS_Low();
  
  MPU_Tx = reg|0x80;
  MPU_Rx = SPI5_ReadWriteByte(MPU_Tx);
  MPU_Rx = SPI5_ReadWriteByte(MPU_Tx);
  
  MPU6050_NSS_High();
  return MPU_Rx;
}

//Read registers from MPU6050,address begin with regAddr
uint8_t MPU6050_Read_Regs(uint8_t const regAddr, uint8_t *pData, uint8_t len)
{
  static uint8_t MPU_Tx, MPU_Tx_buff[14] = {0xff};
  int i = 0;
  MPU6050_NSS_Low();
  
  MPU_Tx = regAddr|0x80;
  MPU_Tx_buff[0] = MPU_Tx;
  SPI5_ReadWriteByte(MPU_Tx);
  for(i=0; i<len;i++)
  {
	pData[i] = SPI5_ReadWriteByte(MPU_Tx_buff[i]);
  }
  MPU6050_NSS_High();
  return 0;
}

//Write IST8310 register through MPU6050
static void IST_Reg_Write_By_MPU(uint8_t addr, uint8_t data)
{
  MPU6050_Write_Reg(MPU6050_I2C_SLV1_CTRL, 0x00);
  delay_ms(2);
  MPU6050_Write_Reg(MPU6050_I2C_SLV1_REG, addr);
  delay_ms(2);
  MPU6050_Write_Reg(MPU6050_I2C_SLV1_DO, data);
  delay_ms(2);
  
  MPU6050_Write_Reg(MPU6050_I2C_SLV1_CTRL, 0x080 | 0x01);
  delay_ms(10);
}

//Write IST8310 register through MPU6050
static uint8_t IST_Reg_Read_By_MPU(uint8_t addr)
{
  uint8_t data;
  
  MPU6050_Write_Reg(MPU6050_I2C_SLV4_REG, addr);
  delay_ms(10);
  MPU6050_Write_Reg(MPU6050_I2C_SLV4_CTRL, 0x80);
  delay_ms(10);
  data = MPU6050_Read_Reg(MPU6050_I2C_SLV4_DI);
  MPU6050_Write_Reg(MPU6050_I2C_SLV4_CTRL, 0x00);
  delay_ms(10);
  return data;
}

//Initialize the MPU6050 I2C Slave0 for I2C reading
static void MPU_Auto_Read_IST_config(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num)
{
  MPU6050_Write_Reg(MPU6050_I2C_SLV1_ADDR, device_address);
  delay_ms(2);
  MPU6050_Write_Reg(MPU6050_I2C_SLV1_REG, IST8310_R_CONFA);
  delay_ms(2);
  MPU6050_Write_Reg(MPU6050_I2C_SLV1_DO, IST8310_ODR_MODE);
  delay_ms(2);
  
  MPU6050_Write_Reg(MPU6050_I2C_SLV0_ADDR, 0x80 | device_address);
  delay_ms(2);
  MPU6050_Write_Reg(MPU6050_I2C_SLV0_REG, reg_base_addr);
  delay_ms(2);
  
  MPU6050_Write_Reg(MPU6050_I2C_SLV4_CTRL, 0x03);
  delay_ms(2);
  
  MPU6050_Write_Reg(MPU6050_I2C_MST_DELAY_CTRL, 0x01 | 0x02);
  delay_ms(2);
  
  MPU6050_Write_Reg(MPU6050_I2C_SLV1_CTRL, 0x80 | 0x01);
  delay_ms(6);
  
  MPU6050_Write_Reg(MPU6050_I2C_SLV0_CTRL, 0x80 | data_num);
  delay_ms(7);
}

//Initialize the IST8310 三轴磁力计
uint8_t IST8310_Init(void)
{
  MPU6050_Write_Reg(MPU6050_USER_CTRL, 0x30);
  delay_ms(10);
  MPU6050_Write_Reg(MPU6050_I2C_MST_CTRL, 0x0d);
  delay_ms(10);
  
  MPU6050_Write_Reg(MPU6050_I2C_SLV1_ADDR, IST8310_ADDRESS);
  delay_ms(10);
  MPU6050_Write_Reg(MPU6050_I2C_SLV4_ADDR, 0x80 | IST8310_ADDRESS);
  delay_ms(10);
  
  IST_Reg_Write_By_MPU(IST8310_R_CONFB, 0x01);
  if(IST8310_DEVICE_ID_A != IST_Reg_Read_By_MPU(IST8310_WHO_AM_I))
    return 1; //error
  delay_ms(10);
  
  IST_Reg_Write_By_MPU(IST8310_R_CONFA, 0x00);
  if(IST_Reg_Read_By_MPU(IST8310_R_CONFA) != 0x00)
    return 2;
  delay_ms(10);
  
  IST_Reg_Write_By_MPU(IST8310_R_CONFB, 0x00);
  if(IST_Reg_Read_By_MPU(IST8310_R_CONFB) != 0x00)
    return 3;
  delay_ms(10);
  
  IST_Reg_Write_By_MPU(IST8310_AVGCNTL, 0x24);
  if(IST_Reg_Read_By_MPU(IST8310_AVGCNTL) != 0x24)
    return 4;
  delay_ms(10);
  
  IST_Reg_Write_By_MPU(IST8310_PDCNTL, 0xc0);
  if(IST_Reg_Read_By_MPU(IST8310_PDCNTL) != 0xc0)
    return 5;
  delay_ms(10);
  
  MPU6050_Write_Reg(MPU6050_I2C_SLV1_CTRL, 0x00);
  delay_ms(10);
  MPU6050_Write_Reg(MPU6050_I2C_SLV4_CTRL, 0x00);
  delay_ms(10);
  
  MPU_Auto_Read_IST_config(IST8310_ADDRESS, IST8310_R_XL, 0x06);
  delay_ms(100);
  return 0;
}

//Set the accelerated velocity resolution
uint8_t MPU6050_Set_Accel_Fsr(uint8_t fsr)
{
  return MPU6050_Write_Reg(MPU6050_ACCEL_CONFIG, fsr<<3);
}

//Set the angular velocity resolution
uint8_t MPU6050_Set_Gyro_Fsr(uint8_t fsr)
{
  return MPU6050_Write_Reg(MPU6050_GYRO_CONFIG, fsr<<3);
}

//Get 6 axis data from MPU6050
void IMU_Get_Data(void)
{
	
	
	
  uint8_t mpu_buff[14];
  MPU6050_Read_Regs(MPU6050_ACCEL_XOUT_H, mpu_buff, 14);
  
  imu_data.ax = mpu_buff[0]<<8 |mpu_buff[1];
  imu_data.ay = mpu_buff[2]<<8 |mpu_buff[3];
  imu_data.az = mpu_buff[4]<<8 |mpu_buff[5];
  
  imu_data.temp = mpu_buff[6]<<8 |mpu_buff[7];
  
  imu_data.gx = mpu_buff[8]<<8 |mpu_buff[9] - imu_data_offest.gx;
  imu_data.gy = mpu_buff[10]<<8 |mpu_buff[11] - imu_data_offest.gy;
  imu_data.gz = mpu_buff[12]<<8 |mpu_buff[13] - imu_data_offest.gz;
  
  
}



void MPU6050_AverageFilter(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz) //[0]-[9]为最近10次数据 [10]为10次数据的平均值
{
	uint8_t i = 0;
	int32_t sum=0;
	
	for(i=1;i<10;i++)
	{
		MPU6050_FIFO[0][i-1]=MPU6050_FIFO[0][i];
		MPU6050_FIFO[1][i-1]=MPU6050_FIFO[1][i];
		MPU6050_FIFO[2][i-1]=MPU6050_FIFO[2][i];
		MPU6050_FIFO[3][i-1]=MPU6050_FIFO[3][i];
		MPU6050_FIFO[4][i-1]=MPU6050_FIFO[4][i];
		MPU6050_FIFO[5][i-1]=MPU6050_FIFO[5][i];
	}
	
	MPU6050_FIFO[0][9]=ax;//将新的数据放置到 数据的最后面
	MPU6050_FIFO[1][9]=ay;
	MPU6050_FIFO[2][9]=az;
	MPU6050_FIFO[3][9]=gx;
	MPU6050_FIFO[4][9]=gy;
	MPU6050_FIFO[5][9]=gz;
	
	for(i=0;i<10;i++)//求当前数组的合，再取平均值
	{	
		 sum+=MPU6050_FIFO[0][i];
	}
	MPU6050_FIFO[0][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[1][i];
	}
	MPU6050_FIFO[1][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[2][i];
	}
	MPU6050_FIFO[2][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[3][i];
	}
	MPU6050_FIFO[3][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[4][i];
	}
	MPU6050_FIFO[4][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[5][i];
	}
	MPU6050_FIFO[5][10]=sum/10;
	
}

void MPU6050_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) 
{
//	if(isMPU6050_is_DRY)
	{
		isMPU6050_is_DRY = 0;
		IMU_Get_Data();
//		HMC58X3_ReadData(&(mpu_buf[14]));  //14-19为陀螺仪数据
		MPU6050_AverageFilter(imu_data.ax, imu_data.ay, imu_data.az, imu_data.gx, imu_data.gy, imu_data.gz);  		
		*ax  =MPU6050_FIFO[0][10];
		*ay  =MPU6050_FIFO[1][10];
		*az = MPU6050_FIFO[2][10];
//		*gx  =MPU6050_FIFO[3][10] - GyroSavedCaliData.GyroXOffset;
//		*gy = MPU6050_FIFO[4][10] - GyroSavedCaliData.GyroYOffset;
//		*gz = MPU6050_FIFO[5][10] - GyroSavedCaliData.GyroZOffset;
		*gx  =MPU6050_FIFO[3][10];
		*gy = MPU6050_FIFO[4][10];
		*gz = MPU6050_FIFO[5][10];
	} 
}

//Initialize the MPU6050
uint8_t MPU6050_Init(void)
{
	//初始化NSS引脚
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = GPIO_Pin_6;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOF, &gpio);
	IST8310_Init();
	
  uint8_t index = 0;
  uint8_t MPU6050_Init_Data[12][2] = 
  {
    {MPU6050_PWR_MGMT_1,    0x80},      // Reset Device
    {MPU6050_PWR_MGMT_1,    0x03},      // Clock Source - Gyro-Z
    {MPU6050_PWR_MGMT_2,    0x00},      // Enable Acc & Gyro
    {MPU6050_CONFIG,        0x03},      // LPF 98Hz
    {MPU6050_GYRO_CONFIG,   0x10},      // +-1000dps
    {MPU6050_ACCEL_CONFIG,  0x01},      // +-2G
    {MPU6050_ACCEL_CONFIG_2,0x02},      // enable LowPassFilter  Set Acc LPF
    {MPU6050_USER_CTRL,     0x20},      // Enable AUX
	{MPU6050_INT_PIN_CFG,	0x02},
	{MPU6050_INT_ENABLE,	0x01}
  };
  
  delay_ms(100);
  MPU_id = MPU6050_Read_Reg(MPU6050_WHO_AM_I);  //read id of device,check if MPU6050 or not
  for(index = 0; index < 12; index++)
  {
    MPU6050_Write_Reg(MPU6050_Init_Data[index][0], MPU6050_Init_Data[index][1]);
    delay_ms(1);
  }
  return 0;
}


