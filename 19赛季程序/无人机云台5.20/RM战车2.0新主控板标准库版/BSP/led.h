#ifndef __LED_H
#define __LED_H

#define GREEN_LED_ON()           GPIO_ResetBits(GPIOE,GPIO_Pin_7);
#define GREEN_LED_OFF()          GPIO_SetBits(GPIOE,GPIO_Pin_7);
#define GREEN_LED_TOGGLE()       GPIO_ToggleBits(GPIOE,GPIO_Pin_7);

#define RED_LED_ON()             GPIO_ResetBits(GPIOF,GPIO_Pin_14);  
#define RED_LED_OFF()            GPIO_SetBits(GPIOF,GPIO_Pin_14);
#define RED_LED_TOGGLE()         GPIO_ToggleBits(GPIOF,GPIO_Pin_14);
#define LASER()                     GPIO_SetBits(GPIOG,GPIO_Pin_13);

void Led_Config(void);

#endif
