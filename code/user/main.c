//----------------------------------------------------------------------------
#include "config.h"
//----------------------------------------------------------------------------
//宏定义
#define uchar   unsigned char
#define uint   unsigned int
#define uint8   unsigned char
#define uint32   unsigned int

//子函数定义
void Delayms(unsigned int n)    //大约1毫秒延时
{
	unsigned int i,j;
	for(i=0;i<n;i++)
	   for(j=1;j<2500;j++); //5000
}

//子函数定义
void Delayus(unsigned int n)    //大约1微秒延时
{
	unsigned int  i,j;
	for(i=0;i<8;i++)
	   for(j=1;j<n;j++);
}



//----------------------------------------------------------------------------
//                        以下为用户自己的功能程序   
//----------------------------------------------------------------------------

#define	 LED	(1<<25)		// 低电平点亮
#define	 LEDR1	(1<<26)		// 低电平点亮
#define	 LEDR2	(1<<27)	
#define	 LEDR3	(1<<28)	

#define	 KEY1	(1<<16)		// 按键位 P0.16
#define	 KEY2	(1<<17)	
#define	 KEY3	(1<<18)	
uchar i=0;
uchar flag=0;

//宏定义
const uint8 DISP_TAB[16]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,
                          0x88,0x83,0xC6,0xA1,0x86,0x8E};		//共阳数码管
//================================================================
//74HC595 管脚定义
//================================================================
#define   SPI_CS    (1<<29)           //P0.29
#define   SPI_DATA  (1<<6)            //P0.6
#define   SPI_CLK   (1<<4)            //P0.4
//================================================================
//名称:HC595_SendDat() 
//功能:向74HC595发送一字节数据
//说明: 发送数据时,高位先发送
//================================================================
void HC595_SendDat(uchar dat)
{
	uchar i;
	IO0CLR |= SPI_CS;  		//SPI_CS=0
	for (i=0;i<8;i++)       //发送8位数据
	{
		IO0CLR |= SPI_CLK;  //SPI_CLK=0

		//设置SPI的DATA输出值
		if((dat&0x80)!=0) IO0SET |= SPI_DATA;
		else  IO0CLR |= SPI_DATA;
		dat<<=1;
		IO0SET |= SPI_CLK;  //SPI_CLK=1
  	}
	IO0SET |= SPI_CS;       //SPI_CS=1,输出显示数据
}


//================================================================
//名称: IRQ_T0() 
//功能: 定时器0中断入口函数
//================================================================
void __irq IRQ_T0(void)
{
	HC595_SendDat(0xff-DISP_TAB[i]);     //输出LED显示数据
	i++;	
	if(i==16) i=0;

	T0IR=0x02;			//清除MR1中断标志
	VICVectAddr=0x00;	//通知VIC中断处理结束
}
//================================================================
//名称: T0_INT() 
//功能: 定时器0
//说明: 
//================================================================
void T0_init()
{		
	T0TC  = 0;				// 定时器0初始化，计数复位为0，见书第153页
	T0PR  = 99;             // 设置定时器0分频为100分频，得0.12MHz
	T0MCR = 0x03<<3;        // 匹配通道MR1匹配中断时：MR1+MR0:011000 => T0MCR=0x03<<3
	T0MR1 = 60024;          // 比较值（0.5s定时值）
	T0TCR = 3;           	// 计数器使能+计数器复位T0TC
	T0TCR = 1;				// 计数器使能     
	T0IR  = 0x02;

	VICIntSelect=0x00;				// 所有中断通道设置为IRQ中断--P75
	VICVectCntl0=0x24;				// T0中断通道分配最高优先级
	VICVectAddr0=(uint32)IRQ_T0;	// 设置中断服务程序地址向量
	VICIntEnable=(1<<4);			// 使能T0中断---P80   4-T0, 5-T1

	IRQEnable();					// 使能IRQ中断	
}

//================================================================
//函数名称：keyscan()
//函数功能：
//================================================================
void keyscan()
{
	   if((IO0PIN&KEY1)==0) 
	   {
	   		 flag = 1;
	   }
	    else if((IO0PIN&KEY2)==0) 
	   {
	   		 flag = 2;
	   }
	   else if((IO0PIN&KEY3)==0) 
	   {
	   		 flag = 3;
	   }
	   else flag = 0;
}

//================================================================
//函数名称：main()
//函数功能：数码管显示0-F实验
//================================================================
int main(void)
{	
	PINSEL0	= 0x00000000;					//设置所有管脚连接GPIO
	PINSEL1	= 0x00000000;					
	T0_init();								//初始化T0
	IO0DIR |= SPI_DATA|SPI_CS|SPI_CLK;		//设置相应控制口为输出方向	

	IO0DIR	= IO0DIR|LED;					//设置LED控制口为输出方向


	IO0DIR	= IO0DIR|LEDR1;	IO0DIR	= IO0DIR|LEDR2;IO0DIR	= IO0DIR|LEDR3;
	IO0DIR	= IO0DIR&(~KEY1);
	while (1)
	{		
		keyscan();
		switch(flag)
		{
			case 0: 
				{
					IO0SET |= LEDR1;
					IO0SET |= LEDR2;
					IO0SET |= LEDR3;		
					break;
				}
			case 1: IO0CLR |= LEDR1;	break;
			case 2:	IO0CLR |= LEDR2;	break;
			case 3:	IO0CLR |= LEDR3;	break;
		}
	}   
}

