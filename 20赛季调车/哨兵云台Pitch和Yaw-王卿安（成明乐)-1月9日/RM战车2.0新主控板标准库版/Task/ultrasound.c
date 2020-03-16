#include "main.h"
#define lenth_side   2000
//��������ȡ������������Ϣ
u16 Ultrasound_distant_f;
u16 Ultrasound_distant_b;
u16 Ultrasound_distant_l;
u16 Ultrasound_distant_r;
volatile u16 Ultrasound_x;
volatile u16 Ultrasound_y;
volatile u32 Ultrasound_time0;
u8 Ultrasound_flag = 0;				//1 2 3 4 ǰ������

void Ultrasound_Init(void)
{
    GPIO_InitTypeDef gpio;

   	RCC_AHB1PeriphClockCmd(Ultrasound_Trig_RCC_Periph , ENABLE);				//����Ultrasound_Portʱ��
	RCC_AHB1PeriphClockCmd(Ultrasound_Echo_RCC_Periph , ENABLE);				//����Ultrasound_Portʱ��
	gpio.GPIO_Pin = Ultrasound_Trig_Pin_f | Ultrasound_Trig_Pin_b | Ultrasound_Trig_Pin_l | Ultrasound_Trig_Pin_r ;										
    gpio.GPIO_Speed = GPIO_Speed_100MHz;								
	gpio.GPIO_Mode = GPIO_Mode_OUT;										
	gpio.GPIO_PuPd = GPIO_PuPd_DOWN;										
	GPIO_Init(Ultrasound_Trig_Port,&gpio);												
	
	gpio.GPIO_Pin = Ultrasound_Echo_Pin_f | Ultrasound_Echo_Pin_b | Ultrasound_Echo_Pin_l | Ultrasound_Echo_Pin_r ;									
    gpio.GPIO_Speed = GPIO_Speed_100MHz;							
	gpio.GPIO_Mode = GPIO_Mode_IN;										
	gpio.GPIO_PuPd = GPIO_PuPd_DOWN;										
	GPIO_Init(Ultrasound_Echo_Port,&gpio);
}
//�������������
u16 Ultrasound_measure(void)
{
//	Ultrasound_time0 = Get_Time_Micros();	//��¼��ʼʱ��
//	GPIO_SetBits(Ultrasound_Trig_Port, Ultrasound_Trig_Pin_f | Ultrasound_Trig_Pin_b | Ultrasound_Trig_Pin_l | Ultrasound_Trig_Pin_r);	//��������
//	delay_us(20);
//	GPIO_ResetBits(Ultrasound_Trig_Port, Ultrasound_Trig_Pin_f | Ultrasound_Trig_Pin_b | Ultrasound_Trig_Pin_l | Ultrasound_Trig_Pin_r);//�ر�����
//	while((GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_f) |  \
//		   GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_b) |  \
//		   GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_l) |  \
//		   GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_r) ) == Bit_RESET)		//�ȴ����ű�ߵ�ƽ
//		if((Get_Time_Micros() - Ultrasound_time0 > 1000))return 0xFFFF;	//����1ms�޷�Ӧ�򷵻ش���0xffff
//	TIM7_Start();//�򿪶�ʱ�����ϼ������ ����20us
//	return 0;	//�����ɹ�
	Ultrasound_flag++;//ÿ�δ��� ��ת����һ��������
	Ultrasound_time0 = Get_Time_Micros();
	switch(Ultrasound_flag)
	{
		case 1:
			GPIO_SetBits(Ultrasound_Trig_Port, Ultrasound_Trig_Pin_f);
			delay_us(20);
			GPIO_ResetBits(Ultrasound_Trig_Port,Ultrasound_Trig_Pin_f);
			while(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_f) == Bit_RESET)//�ȴ����ű�ߵ�ƽ
				if(Get_Time_Micros() -Ultrasound_time0 >1000) 
					return 0xFFFF;
			TIM7_Start();break;
		case 2:
			GPIO_SetBits(Ultrasound_Trig_Port, Ultrasound_Trig_Pin_b);
			delay_us(20);
			GPIO_ResetBits(Ultrasound_Trig_Port,Ultrasound_Trig_Pin_b);
			while(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_b) == Bit_RESET)//�ȴ����ű�ߵ�ƽ
				if(Get_Time_Micros() -Ultrasound_time0 >1000) return 0xFFFF;
			TIM7_Start();break;
		case 3:
			GPIO_SetBits(Ultrasound_Trig_Port, Ultrasound_Trig_Pin_l);
			delay_us(20);
			GPIO_ResetBits(Ultrasound_Trig_Port,Ultrasound_Trig_Pin_l);
			while(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_l) == Bit_RESET)//�ȴ����ű�ߵ�ƽ
				if(Get_Time_Micros() -Ultrasound_time0 >1000) return 0xFFFF;
			TIM7_Start();break;
		case 4:
			GPIO_SetBits(Ultrasound_Trig_Port, Ultrasound_Trig_Pin_r);
			delay_us(20);
			GPIO_ResetBits(Ultrasound_Trig_Port,Ultrasound_Trig_Pin_r);
			while(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_r) == Bit_RESET)//�ȴ����ű�ߵ�ƽ
				if(Get_Time_Micros() -Ultrasound_time0 >1000) return 0xFFFF;
			TIM7_Start();break;		
	}
	if(Ultrasound_flag > 4)
		Ultrasound_flag = 0;
	return 0;
}
//���¾�������
void Ultrasound_Update(void)
{
//	static u8 Updata_FLAG_f = 0;	//0��ʾδ����
//	static u8 Updata_FLAG_b = 0;
//	static u8 Updata_FLAG_l = 0;
//	static u8 Updata_FLAG_r = 0;
	static u8 Updata_FLAG = 0;	//���³ɹ���־
	switch (Ultrasound_flag)
	{
		case 1:
			if(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_f) == Bit_RESET)	//���ű仯��� �� �÷���û�и��¹�
			{
			//	if(Get_Time_Micros() - Ultrasound_time0 > 300 && Get_Time_Micros() - Ultrasound_time0 <10000 )//��ȥ�����Сֵ
				Ultrasound_distant_f = (Get_Time_Micros() - Ultrasound_time0)*340/1000/2;				//����
				Updata_FLAG = 1;																		//�Ѹ��� ��ʶ1
			}break;
		case 2:
			if(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_b) == Bit_RESET)	//���ű仯��� �� �÷���û�и��¹�
			{
				//if(Get_Time_Micros() - Ultrasound_time0 > 300 && Get_Time_Micros() - Ultrasound_time0 <10000 )//��ȥ�����Сֵ
				Ultrasound_distant_b = (Get_Time_Micros() - Ultrasound_time0)*340/1000/2;				//����
				Updata_FLAG = 1;																		//�Ѹ��� ��ʶ1
			}break;	
		case 3:
			if(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_l) == Bit_RESET)	//���ű仯��� �� �÷���û�и��¹�
			{
				//if(Get_Time_Micros() - Ultrasound_time0 > 300 && Get_Time_Micros() - Ultrasound_time0 <10000 )//��ȥ�����Сֵ
				Ultrasound_distant_l = (Get_Time_Micros() - Ultrasound_time0)*340/1000/2;				//����
				Updata_FLAG = 1;																		//�Ѹ��� ��ʶ1
			}break;	
		case 4:
			if(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_r) == Bit_RESET)	//���ű仯��� �� �÷���û�и��¹�
			{
				//if(Get_Time_Micros() - Ultrasound_time0 > 300 && Get_Time_Micros() - Ultrasound_time0 <10000 )//��ȥ�����Сֵ
				Ultrasound_distant_r = (Get_Time_Micros() - Ultrasound_time0)*340/1000/2;				//����
				Updata_FLAG = 1;																		//�Ѹ��� ��ʶ1
			}break;	
	}
	if(Updata_FLAG == 1)
	{	
		Updata_FLAG = 0;
		TIM7_Off();
	}
}
//��������ֵ
void Ultrasound_Getcoordinate(void)
{
	u8 i,j = 0;			//ѭ������
	u8 a,b,c,d;			//�������������ֵ
	u16 Diff = 0;
	u16 x[4] = {0};		//���ݲ���ֵ��������꣬�������м�ֵ
	u16 y[4] = {0};
	float theta = yaw_angle*M_PI/360;	//��ʱ��ƫ�ĽǶ�
	
	if(theta< (M_PI)/2)
	{
		a = Ultrasound_distant_f;
		b = Ultrasound_distant_r;
		c = Ultrasound_distant_b;
		d = Ultrasound_distant_l;
	}
	else if(theta< (M_PI))
	{
		theta -= (M_PI)/2;				//������������ �����Ͻ�����ʱ��abcd
		a = Ultrasound_distant_r;		//����λ�ø�ֵ
		b = Ultrasound_distant_b;
		c = Ultrasound_distant_l;
		d = Ultrasound_distant_f;
	}
	else if(theta< 3*(M_PI)/2)
	{
		theta -= (M_PI);				//
		a = Ultrasound_distant_b;
		b = Ultrasound_distant_l;
		c = Ultrasound_distant_f;
		d = Ultrasound_distant_r;
	}
	else if(theta< 2*(M_PI))
	{
		theta -= 3*(M_PI)/2;				//
		a = Ultrasound_distant_l;
		b = Ultrasound_distant_f;
		c = Ultrasound_distant_r;
		d = Ultrasound_distant_b;
	}
	//���ݲ���ֵ��������꣬�������м�ֵ
	y[0] = 2000-a*cos(theta);
	y[1] = d*sin(theta);
	y[2] = c*cos(theta);
	y[3] = 2000-b*sin(theta);
	
	x[0] = a*sin(theta);
	x[1] = d*cos(theta);
	x[2] = 2000-c*sin(theta);
	x[3] = 2000-b*cos(theta);
	
	Diff = Ultrasound_CheckDiff(x[0],y[0], theta, a, b, c, d);//��ʼ��ֵ
	for(i = 0; i<4;i++)
	{
		for(j = 1; j<4;j++)
		{
			if(Ultrasound_CheckDiff(x[i],x[j],theta, a, b, c, d) < Diff)//���ƫ��ֵ����һ�θ�С�������ƫ��ֵ������
				Diff = Ultrasound_CheckDiff(x[i],x[j],theta, a, b, c, d);
				Ultrasound_x = x[i];
				Ultrasound_y = y[j];
		}
	}
	
}

u16 Ultrasound_CheckDiff(u16 x,u16 y, float theta,u8 a,u8 b,u8 c,u8 d)	//����ƽ��ƫ��ֵ
{
	u16 temp_a, temp_b,temp_c,temp_d;
	(theta < atan(x*1.0/2000-y))  ? (a = (2000-y)/cos(theta)) : (a = x/sin(theta));
	(theta < atan(y*1.0/x)) 	  ? (d = x*1.0/cos(theta)) 	  : (d = y/sin(theta));
	(theta < atan(2000-x/y)) 	  ? (c = y*1.0/cos(theta)) 	  : (c = (2000-x)/sin(theta));
	(theta < atan(2000-y/2000-x)) ? (b = (2000-x)/cos(theta)) : (b = (2000-y)/sin(theta));
	
	return (u16)(fabs(temp_a - a)+fabs(temp_b - b)+fabs(temp_c - c)+fabs(temp_d - d));
}









