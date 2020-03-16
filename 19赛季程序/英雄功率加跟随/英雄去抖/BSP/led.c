#include "main.h"

/*    PE7  绿灯     
      PF14  红灯       */
			
void Led_Config()
{
	GPIO_InitTypeDef gpio;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);
	gpio.GPIO_Pin=GPIO_Pin_7;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOE,&gpio);
	
	gpio.GPIO_Pin=GPIO_Pin_4;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOH,&gpio);
	
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
	
	LASER();
	GREEN_LED_ON();
	RED_LED_OFF();
	
}

void light()
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,  ENABLE);   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//上拉   
	GPIO_Init(GPIOI, &GPIO_InitStructure);//初始化     
}
