#ifndef __LED_H
#define __LED_H

#define GREEN_LED_ON()           GPIO_ResetBits(GPIOE,GPIO_Pin_7);
#define GREEN_LED_OFF()          GPIO_SetBits(GPIOE,GPIO_Pin_7);
#define GREEN_LED_TOGGLE()       GPIO_ToggleBits(GPIOE,GPIO_Pin_7);

#define RED_LED_ON()             GPIO_ResetBits(GPIOF,GPIO_Pin_14);  
#define RED_LED_OFF()            GPIO_SetBits(GPIOF,GPIO_Pin_14);
#define RED_LED_TOGGLE()         GPIO_ToggleBits(GPIOF,GPIO_Pin_14);

#define POWER1()                  GPIO_SetBits(GPIOH,GPIO_Pin_2);
#define POWER2()                  GPIO_SetBits(GPIOH,GPIO_Pin_3);
#define POWER3()                  GPIO_SetBits(GPIOH,GPIO_Pin_4);
#define POWER4()                  GPIO_SetBits(GPIOH,GPIO_Pin_5);


#define EDOOR1_OPEN()        GPIO_ResetBits(GPIOF,GPIO_Pin_0);  //µÇµº
#define EDOOR1_CLOSE()       GPIO_SetBits(GPIOF,GPIO_Pin_0);

#define EDOOR2_OPEN()        GPIO_ResetBits(GPIOF,GPIO_Pin_1); //×¦×Ó
#define EDOOR2_CLOSE()       GPIO_SetBits(GPIOF,GPIO_Pin_1);

#define EDOOR3_OPEN()        GPIO_ResetBits(GPIOE,GPIO_Pin_4);  //Æ½ÒÆ
#define EDOOR3_CLOSE()       GPIO_SetBits(GPIOE,GPIO_Pin_4);

#define EDOOR4_OPEN()        GPIO_ResetBits(GPIOE,GPIO_Pin_5); //Æ½ÒÆ
#define EDOOR4_CLOSE()       GPIO_SetBits(GPIOE,GPIO_Pin_5);

#define EDOOR5_OPEN()        GPIO_ResetBits(GPIOE,GPIO_Pin_6); //²¹µ¯
#define EDOOR5_CLOSE()       GPIO_SetBits(GPIOE,GPIO_Pin_6);

#define EDOOR6_OPEN()        GPIO_ResetBits(GPIOE,GPIO_Pin_12); //±Û
#define EDOOR6_CLOSE()       GPIO_SetBits(GPIOE,GPIO_Pin_12);

#define EDOOR7_OPEN()        GPIO_ResetBits(GPIOC,GPIO_Pin_2); //ÍÏ³µ
#define EDOOR7_CLOSE()       GPIO_SetBits(GPIOC,GPIO_Pin_2);

#define EDOOR8_OPEN()        GPIO_ResetBits(GPIOC,GPIO_Pin_3); 
#define EDOOR8_CLOSE()       GPIO_SetBits(GPIOC,GPIO_Pin_3);




void Led_Config(void);

#endif
