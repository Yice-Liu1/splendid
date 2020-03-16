#ifndef __DOOR_H
#define __DOOR_H
#include "stm32f4xx.h"



//cml: ��ŷ��˿ڶ���

#define EDOOR1_OPEN()        GPIO_ResetBits(GPIOF,GPIO_Pin_0);  //�ǵ�
#define EDOOR1_CLOSE()       GPIO_SetBits(GPIOF,GPIO_Pin_0);

#define EDOOR2_OPEN()        GPIO_ResetBits(GPIOF,GPIO_Pin_1); //צ��
#define EDOOR2_CLOSE()       GPIO_SetBits(GPIOF,GPIO_Pin_1);

#define EDOOR3_OPEN()        GPIO_ResetBits(GPIOE,GPIO_Pin_4);  //ƽ��
#define EDOOR3_CLOSE()       GPIO_SetBits(GPIOE,GPIO_Pin_4);

#define EDOOR4_OPEN()        GPIO_ResetBits(GPIOE,GPIO_Pin_5); //ƽ��
#define EDOOR4_CLOSE()       GPIO_SetBits(GPIOE,GPIO_Pin_5);

#define EDOOR5_OPEN()        GPIO_ResetBits(GPIOE,GPIO_Pin_6); //����
#define EDOOR5_CLOSE()       GPIO_SetBits(GPIOE,GPIO_Pin_6);

#define EDOOR6_OPEN()        GPIO_ResetBits(GPIOE,GPIO_Pin_12); //��
#define EDOOR6_CLOSE()       GPIO_SetBits(GPIOE,GPIO_Pin_12);

#define EDOOR7_OPEN()        GPIO_ResetBits(GPIOC,GPIO_Pin_2); //�ϳ�
#define EDOOR7_CLOSE()       GPIO_SetBits(GPIOC,GPIO_Pin_2);

#define EDOOR8_OPEN()        GPIO_ResetBits(GPIOC,GPIO_Pin_3); 
#define EDOOR8_CLOSE()       GPIO_SetBits(GPIOC,GPIO_Pin_3);


//#define TravelSwitch1        GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4);
//#define TravelSwitch2        GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5);

void DOOR_Init(void);    //��ʼ��

#endif
