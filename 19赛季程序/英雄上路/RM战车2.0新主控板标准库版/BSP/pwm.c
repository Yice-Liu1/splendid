#include "stm32f4xx.h"
#include "pwm.h"

void PWM_INIT(void)
{
    GPIO_InitTypeDef          gpio;
    TIM_TimeBaseInitTypeDef   tim;
    TIM_OCInitTypeDef         oc;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   //PCLK1=42MHz,TIM5 clk =84MHz

    gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&gpio);

	
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM2); 
	
    /* TIM5 */
    tim.TIM_Prescaler = 84-1;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_Period = 20000;   //2.5ms
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2,&tim);
		

    oc.TIM_OCMode = TIM_OCMode_PWM2;
    oc.TIM_OutputState = TIM_OutputState_Enable;
    oc.TIM_OutputNState = TIM_OutputState_Disable;
    oc.TIM_Pulse = 1000;
    oc.TIM_OCPolarity = TIM_OCPolarity_Low;
    oc.TIM_OCNPolarity = TIM_OCPolarity_High;
    oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
    oc.TIM_OCNIdleState = TIM_OCIdleState_Set;
    TIM_OC3Init(TIM2,&oc);
    TIM_OC4Init(TIM2,&oc);
		oc.TIM_Pulse = 0;
    TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);
           
    TIM_ARRPreloadConfig(TIM2,ENABLE);
   // TIM_ARRPreloadConfig(TIM9,ENABLE);
		
    TIM_Cmd(TIM2,ENABLE);
	//	TIM_Cmd(TIM9,ENABLE);
}

void pwm_prepartion(void)
{
  TIM2->CCR3 = 1000;
  TIM2->CCR4 = 1000;
}

void pwm_shoot(int signal)
{
	TIM2->CCR3 = signal;
  TIM2->CCR4 = signal;
}





