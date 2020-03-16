#ifndef __LED_H
#define __LED_H

#define GREEN_LED_OFF()           GPIO_ResetBits(GPIOE,GPIO_Pin_7);
#define GREEN_LED_ON()          GPIO_SetBits(GPIOE,GPIO_Pin_7);
#define GREEN_LED_TOGGLE()       GPIO_ToggleBits(GPIOE,GPIO_Pin_7);

#define RED_LED_ON()             GPIO_ResetBits(GPIOF,GPIO_Pin_14);  
#define RED_LED_OFF()            GPIO_SetBits(GPIOF,GPIO_Pin_14);
#define RED_LED_TOGGLE()         GPIO_ToggleBits(GPIOF,GPIO_Pin_14);
#define LASER()                     GPIO_SetBits(GPIOG,GPIO_Pin_13);//¼¤¹â£¿
#define C_ON()                       GPIO_SetBits(GPIOE,GPIO_Pin_5);
#define C_OFF()                      GPIO_ResetBits(GPIOE,GPIO_Pin_5);
#define POWER1_CTRL()                GPIO_SetBits(GPIOH,GPIO_Pin_2);    
#define POWER2_CTRL()                GPIO_SetBits(GPIOH,GPIO_Pin_3);    
#define POWER3_CTRL()                GPIO_SetBits(GPIOH,GPIO_Pin_4);    
#define POWER4_CTRL()                GPIO_SetBits(GPIOH,GPIO_Pin_5);    

void Led_Config(void);

#endif
