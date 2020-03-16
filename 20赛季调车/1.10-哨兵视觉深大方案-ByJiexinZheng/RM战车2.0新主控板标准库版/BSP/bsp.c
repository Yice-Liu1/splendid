#include "main.h"

void USART6_Config(void)
{
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART6); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_USART6); 
	
	
	//USART3�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOG,&GPIO_InitStructure); 

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART6, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}


//void USART3_IRQHandler(void)
//{
//	u8 ca;
//	
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//	{ 	
//			ca = USART_ReceiveData(USART3);
//	  	printf( "%c", ca );    //�����ܵ�������ֱ�ӷ��ش�ӡ	
//	} 
//	 
//}

void USART6_IRQHandler(void)
{
	if(USART6->SR & USART_SR_ORE)//ORE�ж�
	{
		u8 com_data = USART6->DR;//USART_ClearFlag(USART3,USART_IT_ORE);
	}
	//�����ж�
	if((USART6->SR & (1<<7))&&(USART6->CR1 & USART_CR1_TXEIE))//if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)
	{
		USART6->DR = TxBuffer[TxCounter++]; //дDR����жϱ�־          
		if(TxCounter == count)
		{
			USART6->CR1 &= ~USART_CR1_TXEIE;		//�ر�TXE�ж�//USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
		}
	}
	//�����ж� (���ռĴ����ǿ�) /////////////////////////////////////////////////////////////////////////////////////////
	if(USART6->SR & (1<<5))  
	{
		u8 com_data = USART6->DR;
		
		
		//Dataframe_Process(com_data);
	}
}




void BSP_Init(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     
	PWM_Config();
	Led_Config();            
////	Laser_Config();
	TIM2_Config();	
	SPI5_Init();	
	MPU6050_Init();
	MPU6050_IntConfig();  
	TIM6_Config();		
////	Quad_Encoder_Config();		
	CAN1_Config();           
////	CAN2_Config();            
	USART1_Config();        
	USART3_Config(); 
  USART2_Config(); 
	delay_ms(100);
	Encoder_Start();
	TIM4_Config();
	//TIM6_Stop(); 
	MPU6050_IntConfig();

}


