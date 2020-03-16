#include "main.h"


int check_can1=0;
extern u16 Fspeed_fdb,Uspeed_fdb,speed_fdb,Yp,Yy;//云台角度反馈,yp yy分别对应两个云台的角度fadeback,MX是电机的FEDBACK,全局变量在main.h
extern uint8_t jiaoduh,jiaodul,zhuansuh,zhhuansul,shiji_currenth,shiji_currentl,temp,null;
extern int fenzhuang,y;
int H_fdb,Hcount,diff,last_H_fdb;
 uint32_t Std2;
int timecount=0;
// int  fenzhuang,y;//quanshu是摩擦轮转的圈数

float M9,M10;
volatile Encoder HANDEncoder = {0,0,0,0,0,0,0,0,0};

void CAN2_Config()
{

  	GPIO_InitTypeDef       GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef       NVIC_InitStructure;

    //使能相关时钟
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PORTA时钟	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//使能CAN2时钟	
	
	
    //初始化GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化PA11,PA12
	
	  //引脚复用映射配置
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2); //GPIOA11复用为CAN1
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2); //GPIOA12复用为CAN1
	  
  	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//非时间触发通信模式   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//软件自动离线管理	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART=ENABLE;	//禁止报文自动传送 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
  	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal ;	 //模式设置 
  	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq ;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=CAN_BS1_6tq; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=3;  //分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN2, &CAN_InitStructure);   // 初始化CAN1 
    
		//配置过滤器
 	  CAN_FilterInitStructure.CAN_FilterNumber=14;	  //过滤器0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
		

	
	  CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // 次优先级为0
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
      //  CanReceiveMsgProcess(&rx_message);				//接收到电机的信息 进行处理	
		Std2=rx_message.StdId;
	  jiaoduh = rx_message.Data[0] ;
    jiaodul = rx_message.Data[1];
    zhuansuh =  rx_message.Data[2];
    zhhuansul = rx_message.Data[3];
    shiji_currenth = rx_message.Data[4];
    shiji_currentl = rx_message.Data[5];
    temp =rx_message.Data[6];
    null = rx_message.Data[7];
			y=((jiaoduh << 8)|jiaodul);//云台反馈值处理		
			speed_fdb=((zhuansuh << 8)|zhhuansul);  //带方向的速度
			fenzhuang=(speed_fdb&0X8000);
            fenzhuang =(fenzhuang>>15);
			Uspeed_fdb=(speed_fdb&0x1fff);         //不带符号的速度
			Fspeed_fdb=65535-speed_fdb;
			
				/*********电机反馈****************/
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
//					int n;//转子圈数
//			M5=Uspeed_fdb;
//					if(fenzhuang==1)
//					{
//            M5=Fspeed_fdb;//反向
//						M5=-M5;
//					}
//				}
//					if(Std2==0x208)
//				{
//					int n;//转子圈数
//			M6=Uspeed_fdb;
//					if(fenzhuang==1)
//					{
//            M6=Fspeed_fdb;//反向
//						M6=-M6;
//					}
//				}
//					

//	
//    }
//				
				

}
}






