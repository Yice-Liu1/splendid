#include "main.h"
void USART3_Config(void)
	{
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); 
	
	
	//USART3�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOD,&GPIO_InitStructure); 

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����3�ж�ͨ��
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

u8 usart3_power[4]={0},usart3_rest[2]={0};
u8 usart3_power_flag=0,usart3_rest_flag=0;//һ֡���ݽ�����ϱ�־λ
u8 Rx_Buf[2][32];
int xu=0;
void USART3_IRQHandler(void)
{
	u8 usart3_rcvdat;
	 static u8 ct=0;
		if(USART3->SR & USART_SR_ORE)//ORE�ж�
	{
        USART_ClearFlag(USART3,USART_IT_ORE);
	}
	//�����ж� (���ռĴ����ǿ�) /////////////////////////////////////////////////////////////////////////////////////////
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
		/* �ȴ�����1�������� */
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


