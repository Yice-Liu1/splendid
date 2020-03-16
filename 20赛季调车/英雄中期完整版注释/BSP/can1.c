#include "main.h"
u16 Fspeed_fdb,Uspeed_fdb,speed_fdb,Ucurrent_fdb,Fcurrent_fdb;
float Yp,Yy,REAL_YP,REAL_YY;//��̨�Ƕȷ���,yp yy�ֱ��Ӧ������̨�ĽǶ�fadeback,MX�ǵ����FEDBACK,ȫ�ֱ�����main.h
u16 current_fdb=0;
float current=0.0,shiji_current[4]={0.0},power_fdb=0.0,dianliu=0.0;
int fenzhuang_current;
uint8_t jiaoduh,jiaodul,zhuansuh,zhhuansul,shiji_currenth,shiji_currentl,temp,null;
 uint32_t Std;
 int  fenzhuang,y;//quanshu��Ħ����ת��Ȧ��
float rotationangle;
 float M1,M2,M3,M4,M5,M6;
void CAN1_Config()
{

  	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef  NVIC_InitStructure;

    //ʹ�����ʱ��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��PORTAʱ��	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
	
	
    //��ʼ��GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��PA11,PA12
	
	  //���Ÿ���ӳ������
	  GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_CAN1); //GPIOA11����ΪCAN1
	  GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_CAN1); //GPIOA12����ΪCAN1
	  
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
  	CAN_Init(CAN1, &CAN_InitStructure);   // ��ʼ��CAN1 
    
		//���ù�����
 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //������0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
		

	
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

}  
//void CAN1_TX_IRQHandler(void) 
//{
//    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
//	{
//		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
//    }
//}
int xue=0;
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
    {
			
       
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
    CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
      //  CanReceiveMsgProcess(&rx_message);				//���յ��������Ϣ ���д���	
		Std=rx_message.StdId;
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
			xue=10069;
			//Ѧ�����Է�������
//			current_fdb=((shiji_currenth << 8)|shiji_currentl);
//			fenzhuang_current=current_fdb&0x8000;
//			fenzhuang_current=fenzhuang_current>>15;
//			Ucurrent_fdb=current_fdb&0x1fff;
//			Fcurrent_fdb=65535-current_fdb;//current_fdbȡ��+1
//						current=Ucurrent_fdb;
//					if(fenzhuang_current==1)
//					{
//            current=Fcurrent_fdb;
//						current=-current;
//					}
			current_fdb=((shiji_currenth << 8)|shiji_currentl);
			fenzhuang_current=current_fdb&0x8000;
			fenzhuang_current=fenzhuang_current>>15;
			Ucurrent_fdb=current_fdb&0x1fff;//����13λ
			Fcurrent_fdb=65535-current_fdb;//current_fdbȡ��+1 ����Ǹ���
						current_fdb=Ucurrent_fdb;
					if(fenzhuang_current==1)
					{
            current_fdb=Fcurrent_fdb;
				//		current=-current;
					}
//      current_fdb=current_fdb&0x7fff;
//       current=(float)(current_fdb/16384.0*20.0);
					current=(float)(current_fdb);
			if(Std>=0x201&&Std<=0x204)
			{
				shiji_current[Std-0x201]=current/16384.0*20.0;
			}
			dianliu=(shiji_current[0]+shiji_current[1]+shiji_current[2]+shiji_current[3]);
			power_fdb=dianliu*24.0;
			//
			/********************
������̨����
*******************/
				if(Std==0x205)
				{
			Yy=y;
						REAL_YY=360*Yy/8192;
					if(y>=0xFFF)
		{
				GREEN_LED_OFF();		
			
		}
		else if(y<0XFFF)
		{
		
			
			GREEN_LED_ON();
		}
    }
						if(Std==0x206)
				{
					//EncoderProcess(&GMPitchEncoder,&rx_message);
				Yp=y;
					REAL_YP=360*Yp/8192;
		
					if(y>=0x292)
		{
					
			RED_LED_ON();
		}
		else if(y<0X292)
		{
		
		
			RED_LED_OFF();
		}
    }
				/*********�������****************/
		if(Std==0x201)
				{
			M1=Uspeed_fdb;
					if(fenzhuang==1)
					{
            M1=Fspeed_fdb;
						M1=-M1;
					}
	
    }
					if(Std==0x202)
				{
			M2=Uspeed_fdb;
					if(fenzhuang==1)
					{
            M2=Fspeed_fdb;
						M2=-M2;
						}
	
    }
						if(Std==0x203)
				{
			M3=Uspeed_fdb;
					if(fenzhuang==1)
					{
						M3=Fspeed_fdb;
					M3=(-M3);
					}
					
    }
						if(Std==0x204)
				{
			M4=Uspeed_fdb;
					if(fenzhuang==1)
					{
						M4=Fspeed_fdb;
						M4=-M4;
					}

    }
				if(Std==0x207)
				{
					int n;//ת��Ȧ��
			M5=Uspeed_fdb;
					if(fenzhuang==1)
					{
            M5=Fspeed_fdb;//����
						M5=-M5;
					}
				}
				EncoderProcess(&HANDMOTOR,&rx_message);
					rotationangle=HANDMOTOR.ecd_angle;
					if(Std==0x208)
				{
					int n;//ת��Ȧ��
		      	M6=Uspeed_fdb;
					if(fenzhuang==1)
					{
            M6=Fspeed_fdb;//����
						M6=-M6;
					}
				}
					


}
				}
	
float map(float val, float I_Min, float I_Max, float O_Min, float O_Max){
	return(val/(I_Max-I_Min)*(O_Max-O_Min) + O_Min);}