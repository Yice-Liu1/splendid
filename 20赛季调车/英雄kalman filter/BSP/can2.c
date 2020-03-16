#include "main.h"
//extern u16 Fspeed_fdb,Uspeed_fdb,speed_fdb,Yp,Yy;
//extern uint8_t jiaoduh,jiaodul,zhuansuh,zhhuansul,shiji_currenth,shiji_currentl,temp,null;
 
 uint32_t Std2;
// int  fenzhuang,y;//quanshu��??|2��??��a��?��|��y

float M2_7;
void CAN2_Config()
{

  	GPIO_InitTypeDef       GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef       NVIC_InitStructure;

    //��1?��?��1?����?��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//��1?��PORTA����?��	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//��1?��CAN2����?��	
	
	
    //3?��??��GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//?�䨮?1|?��
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//��?������?3?
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//��?��-
    GPIO_Init(GPIOB, &GPIO_InitStructure);//3?��??��PA11,PA12
	
	  //��y???�䨮?��3��?????
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2); //GPIOA11?�䨮??aCAN1
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2); //GPIOA12?�䨮??aCAN1
	  
  	//CAN�̣�?a����??
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//��?����??�䣤���騪��D??�꨺?   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//����?t��??����???1������	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//?��???�꨺?����1y����?t??D?(??3yCAN->MCR��?SLEEP??)
  	CAN_InitStructure.CAN_NART=ENABLE;	//???1����??��??����??�� 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//����??2????��,D?��??2???����?  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//��??��??��������??������?��????�� 
  	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal ;	 //?�꨺?����?? 
  	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq ;	//??D?��?2?��????��?��(Tsjw)?atsjw+1??����??�̣�?? CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=CAN_BS1_6tq; //Tbs1��??��CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;//Tbs2��??��CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=3;  //��??��?�̨�y(Fdiv)?abrp+1	
  	CAN_Init(CAN2, &CAN_InitStructure);   // 3?��??��CAN1 
    
		//????1y???��
 	  CAN_FilterInitStructure.CAN_FilterNumber=14;	  //1y???��0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32?? 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32??ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32??MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//1y???��01?��a��?FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //?��??1y???��0
  	CAN_FilterInit(&CAN_FilterInitStructure);//??2��?��3?��??��
		

	
	  CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0???��1��o??D???��D��.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // ?�¨�??��???a1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // ��?��??��???a0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

}  

void CAN2_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET) 
    {
			
       
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
        CAN_Receive(CAN2, CAN_FIFO0, &rx_message);
      //  CanReceiveMsgProcess(&rx_message);				//?����?��?��??����?D??�� ??DD��|����	
		Std2=rx_message.StdId;
	  jiaoduh = rx_message.Data[0] ;
    jiaodul = rx_message.Data[1];
    zhuansuh =  rx_message.Data[2];
    zhhuansul = rx_message.Data[3];
    shiji_currenth = rx_message.Data[4];
    shiji_currentl = rx_message.Data[5];
    temp =rx_message.Data[6];
    null = rx_message.Data[7];
			y=((jiaoduh << 8)|jiaodul);//??�������䨤??�̡�|����		
			speed_fdb=((zhuansuh << 8)|zhhuansul);  //��?��??����??��?��
			fenzhuang=(speed_fdb&0X8000);
            fenzhuang =(fenzhuang>>15);
			Uspeed_fdb=(speed_fdb&0x1fff);         //2?��?��?o?��??��?��
			Fspeed_fdb=65535-speed_fdb;
			
				/*********��??�����䨤?****************/
		if(Std2==0x207)
				{
			M2_7=Uspeed_fdb;
					if(fenzhuang==1)
					{
            M2_7=Fspeed_fdb;
						M2_7=-M2_7;
					}
	
        }

   }
}


