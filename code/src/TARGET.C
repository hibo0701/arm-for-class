#define IN_TARGET
#include "config.h"
#define LED1 1 << 18 	// P0.18 ����LED1

/*********************************************************************************************************
** ��������: IRQ_Exception
** ��������: �ж��쳣���������û�������Ҫ�Լ��ı����
********************************************************************************************************/
void __irq IRQ_Exception(void)
	{
    while(1);                   // ��һ���滻Ϊ�Լ��Ĵ���
	}

/*********************************************************************************************************
** ��������: FIQ_Exception
** ��������: �����ж��쳣���������û�������Ҫ�Լ��ı����
********************************************************************************************************/
void FIQ_Exception(void)
{ 	uint32 i;
	i = IO0SET; 		// ��ȡ��ǰLED1�Ŀ���ֵ
	if ((i & LED1) == 0) IO0SET = LED1;
	else IO0CLR = LED1;
/* �ȴ��ⲿ�ж��źŻָ�Ϊ�ߵ�ƽ,���źű���Ϊ�͵�ƽ���жϱ�־��һֱ��λ */
	while ((EXTINT & 0x01) != 0)
	{ EXTINT = 0x01; 	// ���EINT0 �жϱ�־
	}
}
/*********************************************************************************************************
** ��������: TargetInit
** ��������: Ŀ����ʼ�����룬����Ҫ�ĵط����ã�������Ҫ�ı�
********************************************************************************************************/
void TargetInit(void)
{
    /* �����Լ��Ĵ��� */
}

/*********************************************************************************************************
** ��������: TargetResetInit
** ��������: ����main����ǰĿ����ʼ�����룬������Ҫ�ı䣬����ɾ��
********************************************************************************************************/
void TargetResetInit(void)
{
/* ���ô洢��ӳ�䷽ʽ */
#ifdef DEBUG_RAM 				// �����Ƭ��RAM ����
MEMMAP = 0x02; 					// ӳ�䵽Ƭ��RAM
#endif
#ifdef DEBUG_FLASH	 			// �����Ƭ��FLASH ����
MEMMAP = 0x01; 					// ӳ�䵽Ƭ��FLASH
#endif
#ifdef IN_CHIP 					// �����Ƭ��FLASH ����
MEMMAP = 0x01; 					// ӳ�䵽Ƭ��FLASH
#endif

/* ����ϵͳ������ʱ�� */
    PLLCON = 1;					// ���ü��δ����PLL
#if ((Fcclk / 4) / Fpclk) == 1	// ��ֵ��ϵͳӲ������
    VPBDIV = 0;
#endif
#if ((Fcclk / 4) / Fpclk) == 2	// ��ֵ��ϵͳӲ������
    VPBDIV = 2;
#endif
#if ((Fcclk / 4) / Fpclk) == 4	// ��ֵ��ϵͳӲ������
    VPBDIV = 1;
#endif

/* �趨PLL �ĳ�����M �ͳ�����P ��ֵ */
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
    PLLFEED = 0xaa;						// ����PLL�������У�ִ���趨PLL�Ķ���
    PLLFEED = 0x55;
    while((PLLSTAT & (1 << 10)) == 0); 	// �ȴ�PLL����
    PLLCON = 3;						   	// ���ü������PLL
    PLLFEED = 0xaa;					   	// ����PLL �������У�ִ�м�������Ӷ���
    PLLFEED = 0x55;
    
/* ���ô洢������ģ�� */
    MAMCR = 0;							// ��ֹMAM����
#if Fcclk < 20000000
    MAMTIM = 1;							// ϵͳʱ�ӵ���20M����������Ϊ1
#else
#if Fcclk < 40000000
    MAMTIM = 2;							// ϵͳʱ����20M��40M֮�䣬��������Ϊ2
#else
    MAMTIM = 3;							// ϵͳʱ�Ӹ���40M����������Ϊ3
#endif
#endif
    MAMCR = 2;							// ʹ��MAM����
       
/* ��ʼ��VIC */
    VICIntEnClr = 0xffffffff;           // ���������ж�
    VICVectAddr = 0;					// Ĭ��������ַ�Ĵ���
    VICIntSelect = 0;					// 0����Ӧ���ж��������ΪIRQ,  1����Ӧ���ж��������ΪFIQ��

    /* �����Լ��Ĵ��� */

}

/*******************************************************************************************
** ����ΪһЩ��ϵͳ��صĿ⺯����ʵ�֣��û����Ը����Լ���Ҫ���޸�
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
