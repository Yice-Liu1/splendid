#include "main.h"
#define lenth_side   2000
//触发并读取超声波脉冲信息
u16 Ultrasound_distant_f;
u16 Ultrasound_distant_b;
u16 Ultrasound_distant_l;
u16 Ultrasound_distant_r;
volatile u16 Ultrasound_x;
volatile u16 Ultrasound_y;
volatile u32 Ultrasound_time0;
u8 Ultrasound_flag = 0;				//1 2 3 4 前后左右

void Ultrasound_Init(void)
{
    GPIO_InitTypeDef gpio;

   	RCC_AHB1PeriphClockCmd(Ultrasound_Trig_RCC_Periph , ENABLE);				//开启Ultrasound_Port时钟
	RCC_AHB1PeriphClockCmd(Ultrasound_Echo_RCC_Periph , ENABLE);				//开启Ultrasound_Port时钟
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
//测量单方向距离
u16 Ultrasound_measure(void)
{
//	Ultrasound_time0 = Get_Time_Micros();	//记录开始时间
//	GPIO_SetBits(Ultrasound_Trig_Port, Ultrasound_Trig_Pin_f | Ultrasound_Trig_Pin_b | Ultrasound_Trig_Pin_l | Ultrasound_Trig_Pin_r);	//触发引脚
//	delay_us(20);
//	GPIO_ResetBits(Ultrasound_Trig_Port, Ultrasound_Trig_Pin_f | Ultrasound_Trig_Pin_b | Ultrasound_Trig_Pin_l | Ultrasound_Trig_Pin_r);//关闭引脚
//	while((GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_f) |  \
//		   GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_b) |  \
//		   GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_l) |  \
//		   GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_r) ) == Bit_RESET)		//等待引脚变高电平
//		if((Get_Time_Micros() - Ultrasound_time0 > 1000))return 0xFFFF;	//超过1ms无反应则返回错误0xffff
//	TIM7_Start();//打开定时器不断检测引脚 周期20us
//	return 0;	//触发成功
	Ultrasound_flag++;//每次触发 都转向下一个传感器
	Ultrasound_time0 = Get_Time_Micros();
	switch(Ultrasound_flag)
	{
		case 1:
			GPIO_SetBits(Ultrasound_Trig_Port, Ultrasound_Trig_Pin_f);
			delay_us(20);
			GPIO_ResetBits(Ultrasound_Trig_Port,Ultrasound_Trig_Pin_f);
			while(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_f) == Bit_RESET)//等待引脚变高电平
				if(Get_Time_Micros() -Ultrasound_time0 >1000) 
					return 0xFFFF;
			TIM7_Start();break;
		case 2:
			GPIO_SetBits(Ultrasound_Trig_Port, Ultrasound_Trig_Pin_b);
			delay_us(20);
			GPIO_ResetBits(Ultrasound_Trig_Port,Ultrasound_Trig_Pin_b);
			while(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_b) == Bit_RESET)//等待引脚变高电平
				if(Get_Time_Micros() -Ultrasound_time0 >1000) return 0xFFFF;
			TIM7_Start();break;
		case 3:
			GPIO_SetBits(Ultrasound_Trig_Port, Ultrasound_Trig_Pin_l);
			delay_us(20);
			GPIO_ResetBits(Ultrasound_Trig_Port,Ultrasound_Trig_Pin_l);
			while(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_l) == Bit_RESET)//等待引脚变高电平
				if(Get_Time_Micros() -Ultrasound_time0 >1000) return 0xFFFF;
			TIM7_Start();break;
		case 4:
			GPIO_SetBits(Ultrasound_Trig_Port, Ultrasound_Trig_Pin_r);
			delay_us(20);
			GPIO_ResetBits(Ultrasound_Trig_Port,Ultrasound_Trig_Pin_r);
			while(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_r) == Bit_RESET)//等待引脚变高电平
				if(Get_Time_Micros() -Ultrasound_time0 >1000) return 0xFFFF;
			TIM7_Start();break;		
	}
	if(Ultrasound_flag > 4)
		Ultrasound_flag = 0;
	return 0;
}
//更新距离数据
void Ultrasound_Update(void)
{
//	static u8 Updata_FLAG_f = 0;	//0表示未更新
//	static u8 Updata_FLAG_b = 0;
//	static u8 Updata_FLAG_l = 0;
//	static u8 Updata_FLAG_r = 0;
	static u8 Updata_FLAG = 0;	//更新成功标志
	switch (Ultrasound_flag)
	{
		case 1:
			if(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_f) == Bit_RESET)	//引脚变化后后 且 该方向没有更新过
			{
			//	if(Get_Time_Micros() - Ultrasound_time0 > 300 && Get_Time_Micros() - Ultrasound_time0 <10000 )//除去过大过小值
				Ultrasound_distant_f = (Get_Time_Micros() - Ultrasound_time0)*340/1000/2;				//更新
				Updata_FLAG = 1;																		//已更新 标识1
			}break;
		case 2:
			if(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_b) == Bit_RESET)	//引脚变化后后 且 该方向没有更新过
			{
				//if(Get_Time_Micros() - Ultrasound_time0 > 300 && Get_Time_Micros() - Ultrasound_time0 <10000 )//除去过大过小值
				Ultrasound_distant_b = (Get_Time_Micros() - Ultrasound_time0)*340/1000/2;				//更新
				Updata_FLAG = 1;																		//已更新 标识1
			}break;	
		case 3:
			if(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_l) == Bit_RESET)	//引脚变化后后 且 该方向没有更新过
			{
				//if(Get_Time_Micros() - Ultrasound_time0 > 300 && Get_Time_Micros() - Ultrasound_time0 <10000 )//除去过大过小值
				Ultrasound_distant_l = (Get_Time_Micros() - Ultrasound_time0)*340/1000/2;				//更新
				Updata_FLAG = 1;																		//已更新 标识1
			}break;	
		case 4:
			if(GPIO_ReadInputDataBit(Ultrasound_Echo_Port, Ultrasound_Echo_Pin_r) == Bit_RESET)	//引脚变化后后 且 该方向没有更新过
			{
				//if(Get_Time_Micros() - Ultrasound_time0 > 300 && Get_Time_Micros() - Ultrasound_time0 <10000 )//除去过大过小值
				Ultrasound_distant_r = (Get_Time_Micros() - Ultrasound_time0)*340/1000/2;				//更新
				Updata_FLAG = 1;																		//已更新 标识1
			}break;	
	}
	if(Updata_FLAG == 1)
	{	
		Updata_FLAG = 0;
		TIM7_Off();
	}
}
//计算坐标值
void Ultrasound_Getcoordinate(void)
{
	u8 i,j = 0;			//循环变量
	u8 a,b,c,d;			//分配后的四向测量值
	u16 Diff = 0;
	u16 x[4] = {0};		//根据测量值估算的坐标，但其中有假值
	u16 y[4] = {0};
	float theta = yaw_angle*M_PI/360;	//逆时针偏的角度
	
	if(theta< (M_PI)/2)
	{
		a = Ultrasound_distant_f;
		b = Ultrasound_distant_r;
		c = Ultrasound_distant_b;
		d = Ultrasound_distant_l;
	}
	else if(theta< (M_PI))
	{
		theta -= (M_PI)/2;				//把坐标轴掰正 从左上角数逆时针abcd
		a = Ultrasound_distant_r;		//根据位置赋值
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
	//根据测量值估算的坐标，但其中有假值
	y[0] = 2000-a*cos(theta);
	y[1] = d*sin(theta);
	y[2] = c*cos(theta);
	y[3] = 2000-b*sin(theta);
	
	x[0] = a*sin(theta);
	x[1] = d*cos(theta);
	x[2] = 2000-c*sin(theta);
	x[3] = 2000-b*cos(theta);
	
	Diff = Ultrasound_CheckDiff(x[0],y[0], theta, a, b, c, d);//初始差值
	for(i = 0; i<4;i++)
	{
		for(j = 1; j<4;j++)
		{
			if(Ultrasound_CheckDiff(x[i],x[j],theta, a, b, c, d) < Diff)//如果偏差值比上一次更小，则更新偏差值和坐标
				Diff = Ultrasound_CheckDiff(x[i],x[j],theta, a, b, c, d);
				Ultrasound_x = x[i];
				Ultrasound_y = y[j];
		}
	}
	
}

u16 Ultrasound_CheckDiff(u16 x,u16 y, float theta,u8 a,u8 b,u8 c,u8 d)	//返回平均偏差值
{
	u16 temp_a, temp_b,temp_c,temp_d;
	(theta < atan(x*1.0/2000-y))  ? (a = (2000-y)/cos(theta)) : (a = x/sin(theta));
	(theta < atan(y*1.0/x)) 	  ? (d = x*1.0/cos(theta)) 	  : (d = y/sin(theta));
	(theta < atan(2000-x/y)) 	  ? (c = y*1.0/cos(theta)) 	  : (c = (2000-x)/sin(theta));
	(theta < atan(2000-y/2000-x)) ? (b = (2000-x)/cos(theta)) : (b = (2000-y)/sin(theta));
	
	return (u16)(fabs(temp_a - a)+fabs(temp_b - b)+fabs(temp_c - c)+fabs(temp_d - d));
}









