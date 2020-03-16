#include "main.h"
#include <string.h>

//struct STime		stcTime;
//struct SAcc 		stcAcc;
//struct SGyro 		stcGyro;
//struct SAngle 	stcAngle;
//struct SMag 		stcMag;
//struct SDStatus stcDStatus;
//struct SPress 	stcPress;
//struct SLonLat 	stcLonLat;
//struct SGPSV 		stcGPSV;
//struct SQ       stcQ;

void USART6_INIT(void)
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
 
	//����6 ��Ӧ���Ÿ���ӳ�� -> PG14 PG9
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); 
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); 
	
	//USART6�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOG,&GPIO_InitStructure); 

   //USART6 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART6, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���1 
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}


void CopeSerial2Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
//	LED_REVERSE();					//���յ����ݣ�LED����˸һ��
//	USB_TxWrite(&ucData,1);			//��USB-HID�˿�ת���յ��Ĵ������ݣ���ֱ���ý���λ������ģ����������ݡ�
	ucRxBuffer[ucRxCnt++]=ucData;	//���յ������ݴ��뻺������
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
		{
//			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
//			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
//			case 0x54:	memcpy(&stcMag,&ucRxBuffer[2],8);break;
//			case 0x55:	memcpy(&stcDStatus,&ucRxBuffer[2],8);break;
//			case 0x56:	memcpy(&stcPress,&ucRxBuffer[2],8);break;
//			case 0x57:	memcpy(&stcLonLat,&ucRxBuffer[2],8);break;
//			case 0x58:	memcpy(&stcGPSV,&ucRxBuffer[2],8);break;
//			case 0x59:	memcpy(&stcQ,&ucRxBuffer[2],8);break;
		}
		ucRxCnt=0;//��ջ�����
	}
}



void USART6_IRQHandler(void)
{
//	if(USART2->SR & USART_SR_ORE)//ORE�ж�
//	{
//		u8 com_data = USART2->DR;//USART_ClearFlag(USART3,USART_IT_ORE);
//	}
//	//�����ж�
//	if((USART2->SR & (1<<7))&&(USART2->CR1 & USART_CR1_TXEIE))//if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)
//	{
//		USART2->DR = TxBuffer[TxCounter++]; //дDR����жϱ�־          
//		if(TxCounter == count)
//		{
//			USART2->CR1 &= ~USART_CR1_TXEIE;		//�ر�TXE�ж�//USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
//		}
//	}
//	//�����ж� (���ռĴ����ǿ�) /////////////////////////////////////////////////////////////////////////////////////////
//	if(USART2->SR & (1<<5))  
//	{
//		u8 com_data = USART2->DR;
//		Dataframe_Process(com_data);
//	}
	  if(USART_GetITStatus(USART6, USART_IT_TXE) != RESET)
  {   
    USART_SendData(USART6, TxBuffer[TxCounter++]); 
    USART_ClearITPendingBit(USART6, USART_IT_TXE);
    if(TxCounter == count) USART_ITConfig(USART6, USART_IT_TXE, DISABLE);
  }
  else if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
  {
		CopeSerial2Data((unsigned char)USART6->DR);//��������
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
  }
	
	USART_ClearITPendingBit(USART6,USART_IT_ORE);
	
}



