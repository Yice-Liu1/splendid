#include "main.h"

void BSP_Init(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     
	PWM_Config();
	Led_Config();            
////Laser_Config();
	TIM2_Config();	
	SPI5_Init();	
	MPU6050_Init();
	MPU6050_IntConfig();  
	TIM6_Config();		
////Quad_Encoder_Config();		
	CAN1_Config();           
////	CAN2_Config();            
	USART1_Config();        
	USART3_Config();           	
  USART2_Config(); 
	delay_ms(100);
	Encoder_Start();
	TIM4_Config();
	MPU6050_IntConfig();

}


