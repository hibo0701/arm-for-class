/********************************************************************************************************/
#ifndef IN_TARGET 
extern void __irq IRQ_Exception(void);
extern void FIQ_Exception(void);
extern void TargetInit(void);
extern void TargetResetInit(void);
extern void Reset(void);
#endif

// 开/关中断函数
__swi(0x00) void SwiHandle1(int Handle);
#define IRQDisable() SwiHandle1(0) // 禁止IRQ 中断
#define IRQEnable() SwiHandle1(1) // 使能IRQ 中断
#define FIQDisable() SwiHandle1(2) // 禁止FIQ 中断
#define FIQEnable() SwiHandle1(3) // 使能FIQ 中断
