#include "main.h"
//extern u16 Fspeed_fdb,Uspeed_fdb,speed_fdb,Yp,Yy;
//extern uint8_t jiaoduh,jiaodul,zhuansuh,zhhuansul,shiji_currenth,shiji_currentl,temp,null;
 
 uint32_t Std2;
// int  fenzhuang,y;//quanshu¨º??|2¨¢??¡Áa¦Ì?¨¨|¨ºy

float M2_7;
void CAN2_Config()
{

  	GPIO_InitTypeDef       GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef       NVIC_InitStructure;

    //¨º1?¨¹?¨¤1?¨º¡À?¨®
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//¨º1?¨¹PORTA¨º¡À?¨®	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//¨º1?¨¹CAN2¨º¡À?¨®	
	
	
    //3?¨º??¡¥GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//?¡ä¨®?1|?¨¹
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//¨ª?¨ª¨¬¨º?3?
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//¨¦?¨¤-
    GPIO_Init(GPIOB, &GPIO_InitStructure);//3?¨º??¡¥PA11,PA12
	
	  //¨°y???¡ä¨®?¨®3¨¦?????
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2); //GPIOA11?¡ä¨®??aCAN1
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2); //GPIOA12?¡ä¨®??aCAN1
	  
  	//CAN¦Ì£¤?a¨¦¨¨??
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//¡¤?¨º¡À??¡ä£¤¡¤¡é¨ª¡§D??¡ê¨º?   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//¨¨¨ª?t¡Á??¡¥¨¤???1¨¹¨¤¨ª	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//?¡¥???¡ê¨º?¨ª¡§1y¨¨¨ª?t??D?(??3yCAN->MCR¦Ì?SLEEP??)
  	CAN_InitStructure.CAN_NART=ENABLE;	//???1¡À¡§??¡Á??¡¥¡ä??¨ª 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//¡À¡§??2????¡§,D?¦Ì??2???¨¦¦Ì?  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//¨®??¨¨??¨®¨¦¡À¡§??¡À¨º¨º?¡¤????¡§ 
  	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal ;	 //?¡ê¨º?¨¦¨¨?? 
  	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq ;	//??D?¨ª?2?¨¬????¨ª?¨¨(Tsjw)?atsjw+1??¨º¡À??¦Ì£¤?? CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=CAN_BS1_6tq; //Tbs1¡¤??¡ìCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;//Tbs2¡¤??¡ìCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=3;  //¡¤??¦Ì?¦Ì¨ºy(Fdiv)?abrp+1	
  	CAN_Init(CAN2, &CAN_InitStructure);   // 3?¨º??¡¥CAN1 
    
		//????1y???¡Â
 	  CAN_FilterInitStructure.CAN_FilterNumber=14;	  //1y???¡Â0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32?? 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32??ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32??MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//1y???¡Â01?¨¢a¦Ì?FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //?¡è??1y???¡Â0
  	CAN_FilterInit(&CAN_FilterInitStructure);//??2¡§?¡Â3?¨º??¡¥
		

	
	  CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0???¡é1¨°o??D???¨ºD¨ª.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // ?¡Â¨®??¨¨???a1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // ¡ä?¨®??¨¨???a0
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
      //  CanReceiveMsgProcess(&rx_message);				//?¨®¨º?¦Ì?¦Ì??¨²¦Ì?D??¡é ??DD¡ä|¨¤¨ª	
		Std2=rx_message.StdId;
	  jiaoduh = rx_message.Data[0] ;
    jiaodul = rx_message.Data[1];
    zhuansuh =  rx_message.Data[2];
    zhhuansul = rx_message.Data[3];
    shiji_currenth = rx_message.Data[4];
    shiji_currentl = rx_message.Data[5];
    temp =rx_message.Data[6];
    null = rx_message.Data[7];
			y=((jiaoduh << 8)|jiaodul);//??¨¬¡§¡¤¡ä¨¤??¦Ì¡ä|¨¤¨ª		
			speed_fdb=((zhuansuh << 8)|zhhuansul);  //¡ä?¡¤??¨°¦Ì??¨´?¨¨
			fenzhuang=(speed_fdb&0X8000);
            fenzhuang =(fenzhuang>>15);
			Uspeed_fdb=(speed_fdb&0x1fff);         //2?¡ä?¡¤?o?¦Ì??¨´?¨¨
			Fspeed_fdb=65535-speed_fdb;
			
				/*********¦Ì??¨²¡¤¡ä¨¤?****************/
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


