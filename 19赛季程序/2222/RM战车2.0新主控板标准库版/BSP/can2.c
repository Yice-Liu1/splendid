#include "main.h"


int check_can1=0;
extern u16 Fspeed_fdb,Uspeed_fdb,speed_fdb,Yp,Yy;//��̨�Ƕȷ���,yp yy�ֱ��Ӧ������̨�ĽǶ�fadeback,MX�ǵ����FEDBACK,ȫ�ֱ�����main.h
extern uint8_t jiaoduh,jiaodul,zhuansuh,zhhuansul,shiji_currenth,shiji_currentl,temp,null;
extern int fenzhuang,y;
int H_fdb,Hcount,diff,last_H_fdb;
 uint32_t Std2;
int timecount=0;
// int  fenzhuang,y;//quanshu��Ħ����ת��Ȧ��

float M9,M10;
volatile Encoder HANDEncoder = {0,0,0,0,0,0,0,0,0};

void CAN2_Config()
{

  	GPIO_InitTypeDef       GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef       NVIC_InitStructure;

    //ʹ�����ʱ��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��PORTAʱ��	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//ʹ��CAN2ʱ��	
	
	
    //��ʼ��GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��PA11,PA12
	
	  //���Ÿ���ӳ������
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2); //GPIOA11����ΪCAN1
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2); //GPIOA12����ΪCAN1
	  
  	//CAN��Ԫ����
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//��ʱ�䴥��ͨ��ģʽ   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//����Զ����߹���	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
  	CAN_InitStructure.CAN_NART=ENABLE;	//��ֹ�����Զ����� 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//���ȼ��ɱ��ı�ʶ������ 
  	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal ;	 //ģʽ���� 
  	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq ;	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=CAN_BS1_6tq; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;//Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=3;  //��Ƶϵ��(Fdiv)Ϊbrp+1	
  	CAN_Init(CAN2, &CAN_InitStructure);   // ��ʼ��CAN1 
    
		//���ù�����
 	  CAN_FilterInitStructure.CAN_FilterNumber=14;	  //������0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
		

	
	  CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // �����ȼ�Ϊ0
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
      //  CanReceiveMsgProcess(&rx_message);				//���յ��������Ϣ ���д���	
		Std2=rx_message.StdId;
	  jiaoduh = rx_message.Data[0] ;
    jiaodul = rx_message.Data[1];
    zhuansuh =  rx_message.Data[2];
    zhhuansul = rx_message.Data[3];
    shiji_currenth = rx_message.Data[4];
    shiji_currentl = rx_message.Data[5];
    temp =rx_message.Data[6];
    null = rx_message.Data[7];
			y=((jiaoduh << 8)|jiaodul);//��̨����ֵ����		
			speed_fdb=((zhuansuh << 8)|zhhuansul);  //��������ٶ�
			fenzhuang=(speed_fdb&0X8000);
            fenzhuang =(fenzhuang>>15);
			Uspeed_fdb=(speed_fdb&0x1fff);         //�������ŵ��ٶ�
			Fspeed_fdb=65535-speed_fdb;
			
				/*********�������****************/
		if(Std2==0x201)
				{
			M9=Uspeed_fdb;
					if(fenzhuang==1)
					{
            M9=Fspeed_fdb;
						M9=-M9;
					}
	
    }
					if(Std2==0x202)
				{check_can1++;
					timecount++;
					(timecount<=50) ? GetEncoderBias(&HANDEncoder ,&rx_message):EncoderProcess(&HANDEncoder ,&rx_message); 
//					last_H_fdb=H_fdb;
//		    	H_fdb=y;
//					diff=H_fdb-last_H_fdb;
//					if(diff<-7000)  Hcount++;
//					else if(diff>7000)  Hcount--;
					
					
			M10=Uspeed_fdb;
					if(fenzhuang==1)
					{
            M10=Fspeed_fdb;
						M10=-M10;
						}
	
    }
//						if(Std2==0x203)
//				{
//			M3=Uspeed_fdb;
//					if(fenzhuang==1)
//					{
//						M3=Fspeed_fdb;
//					M3=(-M3);
//					}
//					
//    }
//						if(Std2==0x204)
//				{
//			M4=Uspeed_fdb;
//					if(fenzhuang==1)
//					{
//						M4=Fspeed_fdb;
//						M4=-M4;
//					}

//    }
//				if(Std2==0x207)
//				{
//					int n;//ת��Ȧ��
//			M5=Uspeed_fdb;
//					if(fenzhuang==1)
//					{
//            M5=Fspeed_fdb;//����
//						M5=-M5;
//					}
//				}
//					if(Std2==0x208)
//				{
//					int n;//ת��Ȧ��
//			M6=Uspeed_fdb;
//					if(fenzhuang==1)
//					{
//            M6=Fspeed_fdb;//����
//						M6=-M6;
//					}
//				}
//					

//	
//    }
//				
				

}
}






