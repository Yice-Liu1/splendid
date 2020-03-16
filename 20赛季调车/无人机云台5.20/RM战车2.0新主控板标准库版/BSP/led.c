#include "main.h"

/*    PE7  �̵�     
      PF14  ���       */
			
void Led_Config()
{
	GPIO_InitTypeDef gpio;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	gpio.GPIO_Pin=GPIO_Pin_7;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOE,&gpio);
	
	gpio.GPIO_Pin=GPIO_Pin_14;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOF,&gpio);
	
	gpio.GPIO_Pin=GPIO_Pin_13;
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOG,&gpio);
	
	GREEN_LED_OFF();
	RED_LED_OFF();
	
}
