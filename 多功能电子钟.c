#include<reg52.h>
sbit W38_A=P2^2;		 //定义138译码器ABC输入
sbit W38_B=P2^3;
sbit W38_C=P2^4;
unsigned char code distab[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07, 0x7f,0x6f}; 		//数码管段码0-9
unsigned int site1=0;//标记key3按压
unsigned int site2=0;//标记key4按压
unsigned int site3=0;//标记，用于记录计时器暂停时间
unsigned int site4=0;//标记日期是否经过修正
unsigned int year=0;
unsigned int month=0;
unsigned int day=0;
unsigned int time=0;
unsigned int hour=0;
unsigned int minute=0;
unsigned int second=0;
unsigned int hour_ps=0;
unsigned int minute_ps=0;
unsigned int second_ps=0;
void display(unsigned int h,unsigned int m,unsigned int s);
void delay(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=114;y>0;y--);
}
main()
{
//===================初始化==================
	TMOD= 0x11;	//设置定时器模式	
	TL0 = 0x18;	//设置定时初值	
	TH0 = 0xFC;		
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	EA  = 1;		//开启总中断
	ET0 = 1;		//开启定时器0中断
	EX0 = 1;	  //开外部中断0
	IT0 = 1;		//设置下降沿触发
	EX1 = 1;	  //开外部中断1
	IT1 = 1;		//设置下降沿触发


while(1)
	{	
  display(hour,minute,second);
	}
}


	
void timer0(void)interrupt 1
{
	TL0 = 0x18;		//重装定时器初值1ms
	TH0 = 0xFC;		
	time++;
	if(time>=1000)
	{second++;
	time=0;
	}
	if(second>=60)
	{minute++;
	second=0;
	}
	if(minute>=60)
	{hour++;
	minute=0;
	}
	if(hour>=24)
	{hour=0;
	}
	
}


void int0()interrupt 0  //外部中断0入口
{
	if(site2==0)
	{
	  ET0=0;//关闭计时器
		site1++;
		site3++;
	  if(site1==3)
		{
		site1=0;
		site3=0;
		ET0=1;//开启计时器
		}
	 }
		
	

		if(site2==1)
		{
			hour++;  			//年
			site4++;
		}
		
		if(site2==2)
		{
			minute++;   //月
			site4++;
		}
		if(site2==3)
		{
			second++;   //日
			site4++;
		}
		
		
}
		

void int1()interrupt 2  //外部中断1入口
{
   if(site1==1)
		{	
			hour++;
		}
		if(site1==2)
		{
			minute++;
		}
		
		
    if(site3==0)
		{
			ET0=0;
			site3++;
			hour_ps=hour;        //保留时间
			minute_ps=minute;
			second_ps=second; 
			
			
			if(site4==0)         //日期初值
			{
			hour=19;
			minute=06;
			second=23;
			}
			if(site4!=0)
			{
				hour=year;
				minute=month;
				second=day;
			}
		}
		if(site1==0)
		{
			
			site2++;
			
		}
		
		if(site2==4)
		{
			year=hour;            //保留更改后的日期
			month=minute;
			day=second;
			hour=hour_ps;          //恢复时间
			minute=minute_ps;
			second=second_ps;
			site2=0;
			site3=0;
			ET0=1;
		}
}


void display(unsigned int hour,unsigned int minute,unsigned int second)
{
	
	W38_A=0;W38_B=0;W38_C=1;
	P0=distab[hour%10]|0x80;  //时，年led5
	delay(3);
	
	W38_A=1;W38_B=0;W38_C=1;
	P0=distab[hour/10];  //时，年led6
	delay(3);
	
	W38_A=0;W38_B=1;W38_C=0;
	P0=distab[minute%10]|0x80;  //分，月led3
	delay(3);
	

	W38_A=1;W38_B=1;W38_C=0;
	P0=distab[minute/10];  //分，月led4
	delay(3);
	
	W38_A=0;W38_B=0;W38_C=0;
	P0=distab[second%10];  //秒，日led1
	delay(3);


	W38_A=1;W38_B=0;W38_C=0;
	P0=distab[second/10];    //秒，日led2
	delay(3);
}

