#include "main.h"
#include "gun.h"
void BSP_Init(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     
	PWM1_Config();
	Led_Config();	
	SPI5_Init();	
	MPU6050_Init();
	MPU6050_IntConfig();  			
	CAN1_Config();
	CAN2_Config();
	USART1_Config();        
	USART3_Config();           	
  USART2_Config(); 
	//TIM6_Config();
	TIM2_Config();
	delay_ms(100);
	Encoder_Start();
	MPU6050_IntConfig();
}


