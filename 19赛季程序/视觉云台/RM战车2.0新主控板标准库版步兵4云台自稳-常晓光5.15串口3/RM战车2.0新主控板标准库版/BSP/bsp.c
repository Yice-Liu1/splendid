#include "main.h"

void USART6_Config(void)
{
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART6); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_USART6); 
	
	
	//USART3端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOG,&GPIO_InitStructure); 

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART6, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART6, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}


//void USART3_IRQHandler(void)
//{
//	u8 ca;
//	
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//	{ 	
//			ca = USART_ReceiveData(USART3);
//	  	printf( "%c", ca );    //将接受到的数据直接返回打印	
//	} 
//	 
//}

void USART6_IRQHandler(void)
{
	if(USART6->SR & USART_SR_ORE)//ORE中断
	{
		u8 com_data = USART6->DR;//USART_ClearFlag(USART3,USART_IT_ORE);
	}
	//发送中断
	if((USART6->SR & (1<<7))&&(USART6->CR1 & USART_CR1_TXEIE))//if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)
	{
		USART6->DR = TxBuffer[TxCounter++]; //写DR清除中断标志          
		if(TxCounter == count)
		{
			USART6->CR1 &= ~USART_CR1_TXEIE;		//关闭TXE中断//USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
		}
	}
	//接收中断 (接收寄存器非空) /////////////////////////////////////////////////////////////////////////////////////////
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


