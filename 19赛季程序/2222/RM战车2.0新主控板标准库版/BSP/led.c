#include "main.h"

/*    PE7  绿灯     
      PF14  红灯       */
			
void Led_Config()
{
	GPIO_InitTypeDef gpio;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
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
	
	GREEN_LED_OFF();
	RED_LED_OFF();
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);
	gpio.GPIO_Pin=GPIO_Pin_2;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOH,&gpio);
	
	gpio.GPIO_Pin=GPIO_Pin_3;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOH,&gpio);
	
	gpio.GPIO_Pin=GPIO_Pin_4;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOH,&gpio);
	
	gpio.GPIO_Pin=GPIO_Pin_5;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOH,&gpio);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);		//使能GPIOF时钟//电磁阀
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);		//使能GPIOE时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		//使能GPIOC时钟//触碰开关
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
  //GPIO.F1&GPIO.F0初始化设置
  gpio.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_10;    //cml：PF1 PF0
  gpio.GPIO_Mode = GPIO_Mode_OUT;     			//普通输出模式
  gpio.GPIO_OType = GPIO_OType_OD ;    			//推挽输出
  gpio.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  gpio.GPIO_PuPd = GPIO_PuPd_UP;      			//上拉
  GPIO_Init(GPIOF, &gpio);            			//初始化GPIOF
	//GPIO.E4&GPIO.E5初始化设置
	gpio.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_12;    //cml：PE4 PE5
  gpio.GPIO_Mode = GPIO_Mode_OUT;     			//普通输出模式
  gpio.GPIO_OType = GPIO_OType_OD ;    			//推挽输出
  gpio.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  gpio.GPIO_PuPd = GPIO_PuPd_UP;      			//上拉
  GPIO_Init(GPIOE, &gpio);            			//初始化GPIOE
	
	 gpio.GPIO_Pin = GPIO_Pin_9;    //cml：PF1 PF0
  gpio.GPIO_Mode = GPIO_Mode_OUT;     			//普通输出模式
  gpio.GPIO_OType = GPIO_OType_OD ;    			//推挽输出
  gpio.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  gpio.GPIO_PuPd = GPIO_PuPd_UP;      			//上拉
  GPIO_Init(GPIOI, &gpio);    
	
	
	gpio.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;    //cml：PF1 PF0
  gpio.GPIO_Mode = GPIO_Mode_OUT;     			//普通输出模式
  gpio.GPIO_OType = GPIO_OType_OD ;    			//推挽输出
  gpio.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  gpio.GPIO_PuPd = GPIO_PuPd_UP;      			//上拉
  GPIO_Init(GPIOC, &gpio);    
	
	//GPIO.C0,C1,C4,C5初始化设置
	gpio.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_0|GPIO_Pin_1;    //cml：PE4 PE5
  gpio.GPIO_Mode = GPIO_Mode_IN;     			//普通输出模式
  gpio.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  gpio.GPIO_PuPd = GPIO_PuPd_DOWN;      			//下拉
  GPIO_Init(GPIOC, &gpio);            			//初始化GPIOE
	
	//cml:初始化时电磁阀未开
	GPIO_SetBits(GPIOF,GPIO_Pin_1);		//GPIOF1 设置高
	GPIO_SetBits(GPIOF,GPIO_Pin_0);		//GPIOF0 设置高
	GPIO_SetBits(GPIOE,GPIO_Pin_4);		//GPIOE4 设置高 
	GPIO_SetBits(GPIOE,GPIO_Pin_5);		//GPIOE5 设置高
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
	GPIO_SetBits(GPIOE,GPIO_Pin_12);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
	GPIO_SetBits(GPIOC,GPIO_Pin_3);

	
	GPIO_ResetBits(GPIOC,GPIO_Pin_4); //GPIOC4 设置低
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);

}
