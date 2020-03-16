#include "main.h"
void USART3_Config(void)
	{
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); 
	
	
	//USART3端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; 
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
  USART_Init(USART3, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART3, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口3中断通道
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

u8 usart3_power[4]={0},usart3_rest[2]={0};
u8 usart3_power_flag=0,usart3_rest_flag=0;//一帧数据接受完毕标志位
u8 Rx_Buf[2][32];
int xu=0;
void USART3_IRQHandler(void)
{
	u8 usart3_rcvdat;
	 static u8 ct=0;
		if(USART3->SR & USART_SR_ORE)//ORE中断
	{
        USART_ClearFlag(USART3,USART_IT_ORE);
	}
	//接收中断 (接收寄存器非空) /////////////////////////////////////////////////////////////////////////////////////////
if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{  
		xu++;
  		usart3_rcvdat=USART_ReceiveData(USART3);
	  if(ct==0)
		{
			 ct = (0xb5 != usart3_rcvdat)?0:ct+1;
		}
    else if(ct==1)
		{
			 ct = (0x00 != usart3_rcvdat)?0:ct+1;
		}	
    else if(ct==2)
    {
			if(usart3_rcvdat==0x01) { usart3_power_flag=1;ct++;}
			else if(usart3_rcvdat==0x02) {usart3_rest_flag=1;ct++;}
		}
    else if(ct>2)
    {
			 if(usart3_power_flag)
			 {
				 usart3_power[ct-3]=usart3_rcvdat;ct++;
				 if(ct>6) {ct=0;usart3_power_flag=0;}
			 }
			if(usart3_rest_flag)
			{
				 usart3_rest[ct-3]=usart3_rcvdat;ct++;
				 if(ct>4) {ct=0;usart3_rest_flag=0;}
			}
		}					
	}
}

int fputc3(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
    USART_SendData(USART3, (uint8_t)ch);
    return ch;
}
int fgetc3(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART3);
}



void Uart3_Put_Buf(unsigned char *DataToSend , u8 data_num)
{
	for(u8 i=0;i<data_num;i++)
		TxBuffer[count++] = *(DataToSend+i);
	if(!(USART3->CR1 & USART_CR1_TXEIE))
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE); 
}


