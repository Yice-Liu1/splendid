#ifndef __ULTRASOUND_H
#define __ULTRASOUND_H
#include "main.h"

#define Ultrasound_Trig_Port					GPIOD
#define Ultrasound_Echo_Port					GPIOA
#define Ultrasound_Trig_RCC_Periph		RCC_AHB1Periph_GPIOD
#define Ultrasound_Echo_RCC_Periph		RCC_AHB1Periph_GPIOA
#define Ultrasound_Trig_Pin_f           GPIO_Pin_12
#define Ultrasound_Trig_Pin_b           GPIO_Pin_13
#define Ultrasound_Trig_Pin_l           GPIO_Pin_14
#define Ultrasound_Trig_Pin_r           GPIO_Pin_15

#define Ultrasound_Echo_Pin_f           GPIO_Pin_0
#define Ultrasound_Echo_Pin_b           GPIO_Pin_1
#define Ultrasound_Echo_Pin_l        	GPIO_Pin_2
#define Ultrasound_Echo_Pin_r           GPIO_Pin_3


extern u16 Ultrasound_distant_f;
extern u16 Ultrasound_distant_b;
extern u16 Ultrasound_distant_l;
extern u16 Ultrasound_distant_r;
extern volatile u16 Ultrasound_x;
extern volatile u16 Ultrasound_y;
extern volatile u32 Ultrasound_time0;

void Ultrasound_Init(void);
u16 Ultrasound_measure(void);
void Ultrasound_Update(void);
void Ultrasound_Getcoordinate(void);
u16 Ultrasound_CheckDiff(u16 x, u16 y, float theta, u8 a, u8 b, u8 c, u8 d);


#endif

