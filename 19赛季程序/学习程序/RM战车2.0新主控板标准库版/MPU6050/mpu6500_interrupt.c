#include "main.h"

u8 isMPU6050_is_DRY = 0;
void MPU6050_IntConfig(void)			//中断设置
{
	GPIO_InitTypeDef    gpio;
    NVIC_InitTypeDef    nvic;
    EXTI_InitTypeDef    exti;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	//开启
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,  ENABLE);  //开启GPIOA时钟

	gpio.GPIO_Pin = GPIO_Pin_1;							
    gpio.GPIO_Mode = GPIO_Mode_IN;							//输入模式
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;							//上拉输入
    gpio.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_Init(GPIOE, &gpio);								//初始化
  
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,GPIO_PinSource1); //GPIOE_1引脚作中断输入
    exti.EXTI_Line = EXTI_Line1;								
    exti.EXTI_Mode = EXTI_Mode_Interrupt;						//中断模式
    exti.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿中断
    exti.EXTI_LineCmd = ENABLE;									//该线中断使能
    EXTI_Init(&exti);											//中断初始化
    
    nvic.NVIC_IRQChannel = EXTI1_IRQn;							//该中断频道
    nvic.NVIC_IRQChannelPreemptionPriority = 2;					//的抢占优先级为2
    nvic.NVIC_IRQChannelSubPriority = 0;						//从占优先级为0
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);											//初始化
}
//中断任务
void EXTI1_IRQHandler(void)         //中断频率1KHz
{   
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {    
        EXTI_ClearFlag(EXTI_Line1);				//清除中断标志位          
        EXTI_ClearITPendingBit(EXTI_Line1);		//清除中断标志位
        //读取原始数据
        isMPU6050_is_DRY = 1;   //mpu6050中断标志
//IMU_getYawPitchRoll(angle) ;       
			GetPitchYawGxGyGz();//读取姿态数据,数据已经处理成连续方式
    }
}

