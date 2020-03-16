#include "main.h"

void BSP_Init(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     
	PWM_Config();
	Led_Config();	
	SPI5_Init();	
	MPU6050_Init();
	MPU6050_IntConfig();  			
	CAN1_Config();                      
	USART1_Config();        
	USART3_Config();           	
  USART2_Config(); 
	//USART6_INIT();
	TIM6_Config();
	TIM2_Config();
	delay_ms(100);
	Encoder_Start();
	MPU6050_IntConfig();
	
//TIM4_Config();
}


