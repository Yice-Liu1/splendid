#include "door.h"


//cml:电磁阀控制 低电平控制
//初始化PF1和PF0为输出口.并使能这两个口的时钟		    
void DOOR_Init(void)
{  	
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);		//使能GPIOF时钟//电磁阀
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);		//使能GPIOE时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		//使能GPIOC时钟//触碰开关
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
  //GPIO.F1&GPIO.F0初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_10;    //cml：PF1 PF0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     			//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD ;    			//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      			//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);            			//初始化GPIOF
	//GPIO.E4&GPIO.E5初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_12;    //cml：PE4 PE5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     			//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD ;    			//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      			//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);            			//初始化GPIOE
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;    //cml：PF1 PF0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     			//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD ;    			//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      			//上拉
  GPIO_Init(GPIOI, &GPIO_InitStructure);    
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;    //cml：PF1 PF0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     			//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD ;    			//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      			//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);    
	
	//GPIO.C0,C1,C4,C5初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_0|GPIO_Pin_1;    //cml：PE4 PE5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;     			//普通输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;      			//下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);            			//初始化GPIOE
	
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