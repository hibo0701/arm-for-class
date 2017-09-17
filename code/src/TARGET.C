#define IN_TARGET
#include "config.h"
#define LED1 1 << 18 	// P0.18 控制LED1

/*********************************************************************************************************
** 函数名称: IRQ_Exception
** 功能描述: 中断异常处理程序，用户根据需要自己改变程序
********************************************************************************************************/
void __irq IRQ_Exception(void)
	{
    while(1);                   // 这一句替换为自己的代码
	}

/*********************************************************************************************************
** 函数名称: FIQ_Exception
** 功能描述: 快速中断异常处理程序，用户根据需要自己改变程序
********************************************************************************************************/
void FIQ_Exception(void)
{ 	uint32 i;
	i = IO0SET; 		// 读取当前LED1的控制值
	if ((i & LED1) == 0) IO0SET = LED1;
	else IO0CLR = LED1;
/* 等待外部中断信号恢复为高电平,若信号保持为低电平，中断标志会一直置位 */
	while ((EXTINT & 0x01) != 0)
	{ EXTINT = 0x01; 	// 清除EINT0 中断标志
	}
}
/*********************************************************************************************************
** 函数名称: TargetInit
** 功能描述: 目标板初始化代码，在需要的地方调用，根据需要改变
********************************************************************************************************/
void TargetInit(void)
{
    /* 添加自己的代码 */
}

/*********************************************************************************************************
** 函数名称: TargetResetInit
** 功能描述: 调用main函数前目标板初始化代码，根据需要改变，不能删除
********************************************************************************************************/
void TargetResetInit(void)
{
/* 设置存储器映射方式 */
#ifdef DEBUG_RAM 				// 如果在片内RAM 调试
MEMMAP = 0x02; 					// 映射到片内RAM
#endif
#ifdef DEBUG_FLASH	 			// 如果在片内FLASH 调试
MEMMAP = 0x01; 					// 映射到片内FLASH
#endif
#ifdef IN_CHIP 					// 如果在片内FLASH 发布
MEMMAP = 0x01; 					// 映射到片内FLASH
#endif

/* 设置系统各部分时钟 */
    PLLCON = 1;					// 设置激活但未连接PLL
#if ((Fcclk / 4) / Fpclk) == 1	// 此值由系统硬件决定
    VPBDIV = 0;
#endif
#if ((Fcclk / 4) / Fpclk) == 2	// 此值由系统硬件决定
    VPBDIV = 2;
#endif
#if ((Fcclk / 4) / Fpclk) == 4	// 此值由系统硬件决定
    VPBDIV = 1;
#endif

/* 设定PLL 的乘因子M 和除因子P 的值 */
#if (Fcco / Fcclk) == 2
    PLLCFG = ((Fcclk / Fosc) - 1) | (0 << 5);
#endif
#if (Fcco / Fcclk) == 4
    PLLCFG = ((Fcclk / Fosc) - 1) | (1 << 5);
#endif
#if (Fcco / Fcclk) == 8
    PLLCFG = ((Fcclk / Fosc) - 1) | (2 << 5);
#endif
#if (Fcco / Fcclk) == 16
    PLLCFG = ((Fcclk / Fosc) - 1) | (3 << 5);
#endif
    PLLFEED = 0xaa;						// 发送PLL馈送序列，执行设定PLL的动作
    PLLFEED = 0x55;
    while((PLLSTAT & (1 << 10)) == 0); 	// 等待PLL锁定
    PLLCON = 3;						   	// 设置激活并连接PLL
    PLLFEED = 0xaa;					   	// 发送PLL 馈送序列，执行激活和连接动作
    PLLFEED = 0x55;
    
/* 设置存储器加速模块 */
    MAMCR = 0;							// 禁止MAM功能
#if Fcclk < 20000000
    MAMTIM = 1;							// 系统时钟低于20M，建议设置为1
#else
#if Fcclk < 40000000
    MAMTIM = 2;							// 系统时钟在20M～40M之间，建议设置为2
#else
    MAMTIM = 3;							// 系统时钟高于40M，建议设置为3
#endif
#endif
    MAMCR = 2;							// 使能MAM功能
       
/* 初始化VIC */
    VICIntEnClr = 0xffffffff;           // 清零所有中断
    VICVectAddr = 0;					// 默认向量地址寄存器
    VICIntSelect = 0;					// 0：对应的中断请求分配为IRQ,  1：对应的中断请求分配为FIQ。

    /* 添加自己的代码 */

}

/*******************************************************************************************
** 以下为一些与系统相关的库函数的实现，用户可以根据自己的要求修改
*******************************************************************************************/
#include "rt_sys.h"
#include "stdio.h"

#pragma import(__use_no_semihosting_swi)
#pragma import(__use_two_region_memory)

int __rt_div0(int a)
{
    a = a;
    return 0;
}

int fputc(int ch,FILE *f)
{
    ch = ch;
    f = f;
    return 0;
}

int fgetc(FILE *f)
{
    f = f;
    return 0;
}


int _sys_close(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}

int _sys_write(FILEHANDLE fh, const unsigned char * buf,
                      unsigned len, int mode)
{
    fh = fh;
    buf = buf;
    len =len;
    mode = mode;
    return 0;
}
int _sys_read(FILEHANDLE fh, unsigned char * buf,
                     unsigned len, int mode)
{
    fh = fh;
    buf = buf;
    len =len;
    mode = mode;
    return 0;
}

void _ttywrch(int ch)
{
    ch = ch;
}

int _sys_istty(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}

int _sys_seek(FILEHANDLE fh, long pos)
{
    fh = fh;
    return 0;
}

int _sys_ensure(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}

long _sys_flen(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}

int _sys_tmpnam(char * name, int sig, unsigned maxlen)
{
    name = name;
    sig = sig;
    maxlen = maxlen;
    return 0;
}

void _sys_exit(int returncode)
{
    returncode = returncode;
}

char *_sys_command_string(char * cmd, int len)
{
    cmd = cmd;
    len = len;
    return 0;
}

