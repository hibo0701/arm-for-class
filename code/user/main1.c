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
  #define	 LEDR	(1<<26)	

//====================================================
//函数名称：main()
//函数功能：灯闪烁 实验
//====================================================
int main(void)
{	
	PINSEL1	= 0x00000000;	//设置管脚连接GPIO
	IO0DIR	= IO0DIR|LED;	//设置LED控制口输出	
	IO0DIR	= IO0DIR|LEDR;
	while (1)
	{
		IO0SET |= LED;  //P0.25 置高电平，灯灭
		Delayms(500);
		IO0CLR |= LED;  //P0.25 置低电平，灯亮
		Delayms(500);
		
		IO0CLR |= LEDR;  //P0.25 置低电平，灯亮
		Delayms(500);		
	}   
}

