/********************************************************************************************************/
#ifndef IN_TARGET 
extern void __irq IRQ_Exception(void);
extern void FIQ_Exception(void);
extern void TargetInit(void);
extern void TargetResetInit(void);
extern void Reset(void);
#endif

// ��/���жϺ���
__swi(0x00) void SwiHandle1(int Handle);
#define IRQDisable() SwiHandle1(0) // ��ֹIRQ �ж�
#define IRQEnable() SwiHandle1(1) // ʹ��IRQ �ж�
#define FIQDisable() SwiHandle1(2) // ��ֹFIQ �ж�
#define FIQEnable() SwiHandle1(3) // ʹ��FIQ �ж�