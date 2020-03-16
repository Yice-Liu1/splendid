#include "main.h"
void USART2_Config(void)
	{
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
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
	
  USART_Cmd(USART2, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
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

u8 usart2_rcvbuf[30];
u8 usart2_rcvfns=0;//一帧数据接受完毕标志位
u8 cnt=0;
void USART2_IRQHandler(void)
{
  	static u8 cnt=0;
	  u8 usart2_rcvdat;
		if(USART2->SR & USART_SR_ORE)//ORE中断
	{
        USART_ClearFlag(USART3,USART_IT_ORE);
	}
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{  
  		usart2_rcvdat=USART_ReceiveData(USART2);
	  	  if(cnt==0)//帧头起始数据  oxa5
		{
			 cnt = (0xA5 != usart2_rcvdat)?0:cnt+1;
		}
    else if(cnt==1)//功率热量数据Datalength为14字节
		{
			 cnt = (0x0e != usart2_rcvdat)?0:cnt+1;
		}	
    else if(cnt==2)//功率热量数据Datalength为14字节
    {
			 cnt = (0x00 != usart2_rcvdat)?0:cnt+1;
		}
    else if(cnt<5)//包头号和CRC8校验
    {
			 cnt++;
		}			
    else if(cnt==5)//命令码 0x02  //先接低位   0x0202
		{
			 cnt = (0x02 != usart2_rcvdat)?0:cnt+1;
		}
    else if(cnt==6)//命令码 0x02  //后接高位
    {
			 cnt = (0x02 != usart2_rcvdat)?0:cnt+1;
		}
    else if(cnt>6)//枪口数据6个字节
    {		
			usart2_rcvbuf[cnt-7]=usart2_rcvdat;//把功率数据存储下来 
			cnt++;
			if(cnt>20)//一帧数据接收完毕
			{
			   cnt=0;
         usart2_rcvfns=1;//置位标志位 主函数开始处理数据				
			}
		}					
	} 
	 
}


