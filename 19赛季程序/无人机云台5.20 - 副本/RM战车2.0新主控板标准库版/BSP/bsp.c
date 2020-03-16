#include "main.h"

void BSP_Init(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     
	PWM_Config();
//	PWM_Picture();
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
	//TIM6_Stop(); 
	MPU6050_IntConfig();
	//TIM4_Config();
	//USART6_INIT();  //cml:´®¿Ú6³õÊ¼»¯
//	TIM4_Config();
	Ding_Init();

}


