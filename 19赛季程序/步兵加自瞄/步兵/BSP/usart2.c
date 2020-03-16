////#include "main.h"
////void USART2_Config(void)
////	{
////   //GPIO端口设置
////  GPIO_InitTypeDef GPIO_InitStructure;
////	USART_InitTypeDef USART_InitStructure;
////	NVIC_InitTypeDef NVIC_InitStructure;
////	
////	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
////	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
//// 
////	//串口1对应引脚复用映射
////	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); 
////	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 
////	
////	//USART3端口配置
////  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; 
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
////	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
////	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
////	GPIO_Init(GPIOD,&GPIO_InitStructure); 

////   //USART1 初始化设置
////	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
////	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
////	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
////	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
////	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
////	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
////  USART_Init(USART2, &USART_InitStructure); //初始化串口1
////	
////  USART_Cmd(USART2, ENABLE);  //使能串口1 
////	
////	//USART_ClearFlag(USART3, USART_FLAG_TC);
////	
////	
////	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

////	//Usart1 NVIC 配置
////  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口3中断通道
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
////	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
////	
////}

////int fputc(int ch, FILE *f)
////{
////    while (USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
////    USART_SendData(USART2, (uint8_t)ch);
////    return ch;
////}
////int fgetc(FILE *f)
////{
////		/* 等待串口1输入数据 */
////		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

////		return (int)USART_ReceiveData(USART2);
////}



////void Uart2_Put_Buf(unsigned char *DataToSend , u8 data_num)
////{
////	for(u8 i=0;i<data_num;i++)
////		TxBuffer[count++] = *(DataToSend+i);
////	if(!(USART2->CR1 & USART_CR1_TXEIE))
////		USART_ITConfig(USART2, USART_IT_TXE, ENABLE); 
////}

////u8 usart2_rcvbuf[30]={0};
////u8 usart2_rcvfns=0;//一帧数据接受完毕标志位
////void USART2_IRQHandler(void)
////{
////  	static u8 cnt=0;
////	  u8 usart2_rcvdat;
////		if(USART2->SR & USART_SR_ORE)//ORE中断
////	{
////        USART_ClearFlag(USART2,USART_IT_ORE);
////	}
////	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
////	{  
////  		usart2_rcvdat=USART_ReceiveData(USART2);
////	  if(cnt==0)//帧头起始数据  oxa5
////		{
////			 cnt = (0xA5 != usart2_rcvdat)?0:cnt+1;
////		}
////    else if(cnt==1)//功率热量数据Datalength为14字节
////		{
////			 cnt = (0x0e != usart2_rcvdat)?0:cnt+1;
////		}	
////    else if(cnt==2)//功率热量数据Datalength为14字节
////    {
////			 cnt = (0x00 != usart2_rcvdat)?0:cnt+1;
////		}
////    else if(cnt<5)//包头号和CRC8校验
////    {
////			 cnt++;
////		}			
////    else if(cnt==5)//命令码 0x02  //先接低位   0x0202
////		{
////			 cnt = (0x02 != usart2_rcvdat)?0:cnt+1;
////		}
////    else if(cnt==6)//命令码 0x02  //后接高位
////    {
////			 cnt = (0x02 != usart2_rcvdat)?0:cnt+1;
////		}
////    else if(cnt>6)//枪口数据6个字节
////    {		
////			usart2_rcvbuf[cnt-7]=usart2_rcvdat;//把功率数据存储下来 
////			cnt++;
////			if(cnt>20)//一帧数据接收完毕
////			{
////			   cnt=0;
////         usart2_rcvfns=1;//置位标志位 主函数开始处理数据				
////			}
////		}			
////	} 
////	 
////}


//#include "main.h"

//////串口1中断服务程序
//////注意,读取USARTx->SR能避免莫名其妙的错误   	
//////u16 USART_TX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

//////接收状态
//////bit15，	接收完成标志
//////bit14，	接收到0x0d
//////bit13~0，	接收到的有效字节数目
//u16 USART_RX_STA=0;       //接收状态标记	
//void USART2_Config(void){
//   //GPIO端口设置
//  GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
// 
//	//串口1对应引脚复用映射
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART2); 
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART2); 
//	
//	
//	//USART3端口配置
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
//	GPIO_Init(GPIOD,&GPIO_InitStructure); 

//   //USART1 初始化设置
//	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//  USART_Init(USART2, &USART_InitStructure); //初始化串口1
//	
//  USART_Cmd(USART2, ENABLE);  //使能串口1 
//	
//	//USART_ClearFlag(USART3, USART_FLAG_TC);
//	
//	
//	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);//开启相关中断

//	//Usart1 NVIC 配置
//  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口3中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
//	
//	


//	
//}


//////void USART2_IRQHandler(void)
//////{	u16 t;
//////	u8 ca;
//////	
//////	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
//////	{ 	
//////			USART_SendData(USART2,USART_TX_BUF[t]);
//////		t++;
//////	  	
//////	} 
//////	 
//////}

//u8 Rx_Buf2[2][32];

//void USART2_IRQHandler(void)
//{
//	if(USART2->SR & USART_SR_ORE)//ORE中断
//	{
//		u8 com_data = USART2->DR;//USART_ClearFlag(USART3,USART_IT_ORE);
//	}
//	//发送中断
//	if((USART2->SR & (1<<7))&&(USART2->CR1 & USART_CR1_TXEIE))//if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)
//	{
//		USART2->DR = TxBuffer[TxCounter++]; //写DR清除中断标志          
//		if(TxCounter == count)
//		{
//			USART2->CR1 &= ~USART_CR1_TXEIE;		//关闭TXE中断//USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
//		}
//	}
//	//接收中断 (接收寄存器非空) /////////////////////////////////////////////////////////////////////////////////////////
//	if(USART2->SR & (1<<5))  
//	{
//		u8 com_data = USART2->DR;
//		Dataframe_Process(com_data);
//	}
//}

//int fputc(int ch, FILE *f)
//{
//    while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
//    USART_SendData(USART3, (uint8_t)ch);
//    return ch;
//}
//int fgetc(FILE *f)
//{
//		/* 等待串口1输入数据 */
//		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(USART2);
//}



///*void Uart2_Put_Buf(unsigned char *DataToSend , u8 data_num)
//{
//	for(u8 i=0;i<data_num;i++)
//		TxBuffer[count++] = *(DataToSend+i);
//	if(!(USART3->CR1 & USART_CR1_TXEIE))
//		USART_ITConfig(USART3, USART_IT_TXE, ENABLE); 
//}
//*/

#include "main.h"
static uint8_t manifold_rx_buffer[2][18];
u8 data;

u16 x_coordinate=235;  //cml:带符号的230
u16 x_coordinate_old=0;
u16 y_coordinate=135; //170

u8 coordinate_flag=0;
int iiii;

u8 uart_data[9]={0,0,0,0,0,0,0,0,0};  //串口接受的原始数据，0-5分别代表XYZ
u8 dd;
int uart_times=0;      //接受六次数据

void USART2_Config(void)
	{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 
	
	//USART3端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOD,&GPIO_InitStructure); 

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口1
	


	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	//DMA
	 USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
    
    DMA_DeInit(DMA1_Stream5);
    DMA_StructInit(&DMA);
    DMA.DMA_Channel = DMA_Channel_4;
    DMA.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);
    DMA .DMA_Memory0BaseAddr = (uint32_t)&manifold_rx_buffer[0][0];
    DMA .DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA .DMA_BufferSize = sizeof(manifold_rx_buffer)/2;
    DMA .DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA .DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA .DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA .DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA .DMA_Mode = DMA_Mode_Circular;
    DMA .DMA_Priority = DMA_Priority_Medium;
    DMA.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA .DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA .DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA .DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream5, &DMA);
    
    //配置Memory1,Memory0是第一个使用的Memory
    DMA_DoubleBufferModeConfig(DMA1_Stream5, (uint32_t)&manifold_rx_buffer[1][0], DMA_Memory_0);   //first used memory configuration
    DMA_DoubleBufferModeCmd(DMA1_Stream5, ENABLE);
    
    DMA_Cmd(DMA1_Stream5, ENABLE);

   USART_Cmd(USART2, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
	
USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);  //开启 USART1 总线空闲中断

    
	
}



u8 Rx_Buf2[2][32];
void USART2_IRQHandler(void)
{
	iiii++;
	static uint32_t this_time_rx_len = 0;
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		//clear the idle pending flag 
		(void)USART2->SR;
		(void)USART2->DR;
		
	
	
		//  data= USART2->DR;//USART_ClearFlag(USART3,USART_IT_ORE);
		//	Dataframe_Process(data);
	  if(DMA_GetCurrentMemoryTarget(DMA1_Stream5) == 0)
			{
				DMA_Cmd(DMA1_Stream5, DISABLE);
				this_time_rx_len = BSP_USART2_DMA_RX_BUF_LEN - DMA_GetCurrDataCounter(DMA1_Stream5);
				DMA1_Stream5->NDTR = (uint16_t)BSP_USART2_DMA_RX_BUF_LEN;     //relocate the dma memory pointer to the beginning position
				DMA1_Stream5->CR |= (uint32_t)(DMA_SxCR_CT);                  //enable the current selected memory is Memory 1
				DMA_Cmd(DMA1_Stream5, ENABLE);
				if(this_time_rx_len == manifold_FRAME_LENGTH)
				{
					MANIFOLD_DATA_PROCESS(manifold_rx_buffer[0]);				
				}
				else 
			  {
					DMA_Cmd(DMA1_Stream5, DISABLE);
					this_time_rx_len = BSP_USART2_DMA_RX_BUF_LEN - DMA_GetCurrDataCounter(DMA1_Stream5);
					DMA1_Stream5->NDTR = (uint16_t)BSP_USART2_DMA_RX_BUF_LEN;      //relocate the dma memory pointer to the beginning position
					DMA1_Stream5->CR &= ~(uint32_t)(DMA_SxCR_CT);                  //enable the current selected memory is Memory 0
					DMA_Cmd(DMA1_Stream5, ENABLE);
					if(this_time_rx_len == manifold_FRAME_LENGTH)
					{
						MANIFOLD_DATA_PROCESS(manifold_rx_buffer[1]);				
					}
				}	
		  }
	  }
}

	


int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
    USART_SendData(USART2, (uint8_t)ch);
    return ch;
}
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART2);
}



void Uart2_Put_Buf(unsigned char *DataToSend , u8 data_num)
{
	for(u8 i=0;i<data_num;i++)
		TxBuffer[count++] = *(DataToSend+i);
	if(!(USART2->CR1 & USART_CR1_TXEIE))
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE); 
}
void MANIFOLD_DATA_PROCESS(uint8_t *pData)
{
    if(pData == NULL)
    {
        return;
    }
    
   if(pData[0]==0xFF&&pData[8]==0XFE)
	 {
    x_coordinate =640-( ((int16_t)pData[1]) | ((int16_t)pData[2] << 8));
		 // x_coordinate =( ((int16_t)pData[1]) | ((int16_t)pData[2] << 8));
    y_coordinate = ((int16_t)pData[3]) | ((int16_t)pData[4] << 8);
		// uart_data[0] = ((int16_t)pData[10]) | ((int16_t)pData[11] << 8);    
		RED_LED_ON();
		 
		 if(x_coordinate>630) x_coordinate=630;
		 if(x_coordinate<6) x_coordinate=6;
		 
		 //cml:在此处对x_coordinate动手
//		 if(fabs(x_coordinate-x_coordinate_old)>=10)
//		 {
//			 coordinate_flag=1;
//		 }
//		 else coordinate_flag=0;
//		 x_coordinate_old=x_coordinate;
	 }
}




