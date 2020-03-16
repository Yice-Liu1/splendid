#include "main.h"

/*    PE7  �̵�     
      PF14  ���       */
			
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
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);		//ʹ��GPIOFʱ��//��ŷ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);		//ʹ��GPIOEʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		//ʹ��GPIOCʱ��//��������
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
  //GPIO.F1&GPIO.F0��ʼ������
  gpio.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_10;    //cml��PF1 PF0
  gpio.GPIO_Mode = GPIO_Mode_OUT;     			//��ͨ���ģʽ
  gpio.GPIO_OType = GPIO_OType_OD ;    			//�������
  gpio.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  gpio.GPIO_PuPd = GPIO_PuPd_UP;      			//����
  GPIO_Init(GPIOF, &gpio);            			//��ʼ��GPIOF
	//GPIO.E4&GPIO.E5��ʼ������
	gpio.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_12;    //cml��PE4 PE5
  gpio.GPIO_Mode = GPIO_Mode_OUT;     			//��ͨ���ģʽ
  gpio.GPIO_OType = GPIO_OType_OD ;    			//�������
  gpio.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  gpio.GPIO_PuPd = GPIO_PuPd_UP;      			//����
  GPIO_Init(GPIOE, &gpio);            			//��ʼ��GPIOE
	
	 gpio.GPIO_Pin = GPIO_Pin_9;    //cml��PF1 PF0
  gpio.GPIO_Mode = GPIO_Mode_OUT;     			//��ͨ���ģʽ
  gpio.GPIO_OType = GPIO_OType_OD ;    			//�������
  gpio.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  gpio.GPIO_PuPd = GPIO_PuPd_UP;      			//����
  GPIO_Init(GPIOI, &gpio);    
	
	
	gpio.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;    //cml��PF1 PF0
  gpio.GPIO_Mode = GPIO_Mode_OUT;     			//��ͨ���ģʽ
  gpio.GPIO_OType = GPIO_OType_OD ;    			//�������
  gpio.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  gpio.GPIO_PuPd = GPIO_PuPd_UP;      			//����
  GPIO_Init(GPIOC, &gpio);    
	
	//GPIO.C0,C1,C4,C5��ʼ������
	gpio.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_0|GPIO_Pin_1;    //cml��PE4 PE5
  gpio.GPIO_Mode = GPIO_Mode_IN;     			//��ͨ���ģʽ
  gpio.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  gpio.GPIO_PuPd = GPIO_PuPd_DOWN;      			//����
  GPIO_Init(GPIOC, &gpio);            			//��ʼ��GPIOE
	
	//cml:��ʼ��ʱ��ŷ�δ��
	GPIO_SetBits(GPIOF,GPIO_Pin_1);		//GPIOF1 ���ø�
	GPIO_SetBits(GPIOF,GPIO_Pin_0);		//GPIOF0 ���ø�
	GPIO_SetBits(GPIOE,GPIO_Pin_4);		//GPIOE4 ���ø� 
	GPIO_SetBits(GPIOE,GPIO_Pin_5);		//GPIOE5 ���ø�
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
	GPIO_SetBits(GPIOE,GPIO_Pin_12);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
	GPIO_SetBits(GPIOC,GPIO_Pin_3);

	
	GPIO_ResetBits(GPIOC,GPIO_Pin_4); //GPIOC4 ���õ�
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);

}
