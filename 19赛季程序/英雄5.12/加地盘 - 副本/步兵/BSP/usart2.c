#include "main.h"
void USART2_Config(void)
	{
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 
	
	//USART3�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; 
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
  USART_Init(USART2, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}

int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
    USART_SendData(USART2, (uint8_t)ch);
    return ch;
}
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
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
u8 usart2_rcvfns=0;//һ֡���ݽ�����ϱ�־λ
u8 cnt=0;
void USART2_IRQHandler(void)
{
  	static u8 cnt=0;
	  u8 usart2_rcvdat;
		if(USART2->SR & USART_SR_ORE)//ORE�ж�
	{
        USART_ClearFlag(USART3,USART_IT_ORE);
	}
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{  
  		usart2_rcvdat=USART_ReceiveData(USART2);
	  	  if(cnt==0)//֡ͷ��ʼ����  oxa5
		{
			 cnt = (0xA5 != usart2_rcvdat)?0:cnt+1;
		}
    else if(cnt==1)//������������DatalengthΪ14�ֽ�
		{
			 cnt = (0x0e != usart2_rcvdat)?0:cnt+1;
		}	
    else if(cnt==2)//������������DatalengthΪ14�ֽ�
    {
			 cnt = (0x00 != usart2_rcvdat)?0:cnt+1;
		}
    else if(cnt<5)//��ͷ�ź�CRC8У��
    {
			 cnt++;
		}			
    else if(cnt==5)//������ 0x02  //�Ƚӵ�λ   0x0202
		{
			 cnt = (0x02 != usart2_rcvdat)?0:cnt+1;
		}
    else if(cnt==6)//������ 0x02  //��Ӹ�λ
    {
			 cnt = (0x02 != usart2_rcvdat)?0:cnt+1;
		}
    else if(cnt>6)//ǹ������6���ֽ�
    {		
			usart2_rcvbuf[cnt-7]=usart2_rcvdat;//�ѹ������ݴ洢���� 
			cnt++;
			if(cnt>20)//һ֡���ݽ������
			{
			   cnt=0;
         usart2_rcvfns=1;//��λ��־λ ��������ʼ��������				
			}
		}					
	} 
	 
}


