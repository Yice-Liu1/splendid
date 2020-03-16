#include "main.h"
#include "picture.h"


int H=2000;
int v1=30;
int v0=20000;
int Gg=98000;

float qw=0;
float b;
float wt;

float piture_pitch;

float pic_out;

#define pi 3.1415926535897932




int Picture()
{
		 qw=(90.0-angle[1])/180*pi;
		 
	   b=-atan((H*Gg*sin(qw))/(v0*cos(qw - pi/2)*(v1*sin(qw) - cos(qw - pi/2)* sqrt(2*H*Gg + v0*v0*cos(qw)*cos(qw) + v1*v1 - 2*v0*v1*cos(qw)) + v0*sin(qw)*sin(qw - pi/2))));
		 wt=b/pi*180;
	
		 piture_pitch = (float)stcAngle.Angle[1]/32768*180;
		 
		 if(wt-piture_pitch>5)   //duoji
		{
			pic_out=pic_out+0.5;
			delay_ms(1);
			if(pic_out>=2050) pic_out = 2050;
		}
		else if(wt-piture_pitch<-5)
		{
			pic_out=pic_out-0.5;
			delay_ms(1);
		  if(pic_out<=1600) pic_out = 1600;
		}	
    
    return 		pic_out;
	}
