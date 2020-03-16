#include "main.h"
//#include"led.h"

extern int t;
//int flag_z=0,flag_y=0;
int flag_z,flag_y;
void Exti_Configuration(void)
{
//    GPIO_InitTypeDef  gpio;
    EXTI_InitTypeDef  exti;
    NVIC_InitTypeDef  nvic;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
       
//    gpio.GPIO_Mode = GPIO_Mode_IN;
//    gpio.GPIO_OType = GPIO_OType_PP;
//    gpio.GPIO_PuPd = GPIO_PuPd_UP;
//    gpio.GPIO_Speed = GPIO_Speed_50MHz;
//    
//    gpio.GPIO_Pin = GPIO_Pin_3;
//    GPIO_Init(GPIOA, &gpio);
/*****PI0中断*****/	/**小轮**/
	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOI,EXTI_PinSource0);
    
    exti.EXTI_Line = EXTI_Line0;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger =  EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
    
    nvic.NVIC_IRQChannel = EXTI0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
/******PA3中断*****/  /**后**/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource3);
    
    exti.EXTI_Line = EXTI_Line3;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger =  EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
    
    nvic.NVIC_IRQChannel = EXTI3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

/*****PI5中断后*****/
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOI,EXTI_PinSource5);
    
    exti.EXTI_Line = EXTI_Line5;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger =  EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
    
    nvic.NVIC_IRQChannel = EXTI9_5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
/*****PH12中断小轮*****/
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOH,EXTI_PinSource12);
    
    exti.EXTI_Line = EXTI_Line12;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger =  EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
    
    nvic.NVIC_IRQChannel = EXTI15_10_IRQn;//EXTI15_10_IRQHandler
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
}

void  EXTI9_5_IRQHandler(void)//小轮右
{
    if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
    {
				 flag_y=1;    
			GREEN_LED_ON();
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}

void  EXTI0_IRQHandler(void)//后右
{
    if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
    {
				 flag_y=0;    
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI3_IRQHandler(void)//小轮左
{
    if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
    {
				 flag_z=1;    
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

void EXTI15_10_IRQHandler(void)//后左
{
    if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
    {
				 flag_z=0;    
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
}
//  接近开关，点亮低电平，熄灭高电平  检测灭到亮 即高电平到低电平，下降沿
//  光电                                                           下降沿
