#include "main.h"

u8 isMPU6050_is_DRY = 0;
void MPU6050_IntConfig(void)			//�ж�����
{
	GPIO_InitTypeDef    gpio;
    NVIC_InitTypeDef    nvic;
    EXTI_InitTypeDef    exti;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	//����
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,  ENABLE);  //����GPIOAʱ��

	gpio.GPIO_Pin = GPIO_Pin_1;							
    gpio.GPIO_Mode = GPIO_Mode_IN;							//����ģʽ
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;							//��������
    gpio.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_Init(GPIOE, &gpio);								//��ʼ��
  
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,GPIO_PinSource1); //GPIOE_1�������ж�����
    exti.EXTI_Line = EXTI_Line1;								
    exti.EXTI_Mode = EXTI_Mode_Interrupt;						//�ж�ģʽ
    exti.EXTI_Trigger = EXTI_Trigger_Falling;//�½����ж�
    exti.EXTI_LineCmd = ENABLE;									//�����ж�ʹ��
    EXTI_Init(&exti);											//�жϳ�ʼ��
    
    nvic.NVIC_IRQChannel = EXTI1_IRQn;							//���ж�Ƶ��
    nvic.NVIC_IRQChannelPreemptionPriority = 2;					//����ռ���ȼ�Ϊ2
    nvic.NVIC_IRQChannelSubPriority = 0;						//��ռ���ȼ�Ϊ0
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);											//��ʼ��
}
//�ж�����
void EXTI1_IRQHandler(void)         //�ж�Ƶ��1KHz
{   
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {    
        EXTI_ClearFlag(EXTI_Line1);				//����жϱ�־λ          
        EXTI_ClearITPendingBit(EXTI_Line1);		//����жϱ�־λ
        //��ȡԭʼ����
        isMPU6050_is_DRY = 1;   //mpu6050�жϱ�־
//IMU_getYawPitchRoll(angle) ;       
			GetPitchYawGxGyGz();//��ȡ��̬����,�����Ѿ������������ʽ
    }
}

