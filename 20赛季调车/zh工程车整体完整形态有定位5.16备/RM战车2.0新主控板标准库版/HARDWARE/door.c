#include "door.h"


//cml:��ŷ����� �͵�ƽ����
//��ʼ��PF1��PF0Ϊ�����.��ʹ���������ڵ�ʱ��		    
void DOOR_Init(void)
{  	
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);		//ʹ��GPIOFʱ��//��ŷ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);		//ʹ��GPIOEʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		//ʹ��GPIOCʱ��//��������
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
  //GPIO.F1&GPIO.F0��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_10;    //cml��PF1 PF0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     			//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD ;    			//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      			//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);            			//��ʼ��GPIOF
	//GPIO.E4&GPIO.E5��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_12;    //cml��PE4 PE5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     			//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD ;    			//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      			//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);            			//��ʼ��GPIOE
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;    //cml��PF1 PF0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     			//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD ;    			//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      			//����
  GPIO_Init(GPIOI, &GPIO_InitStructure);    
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;    //cml��PF1 PF0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     			//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD ;    			//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      			//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);    
	
	//GPIO.C0,C1,C4,C5��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_0|GPIO_Pin_1;    //cml��PE4 PE5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;     			//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;      			//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);            			//��ʼ��GPIOE
	
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