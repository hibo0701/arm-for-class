
;定义堆栈的大小
SVC_STACK_LEGTH 		EQU 		0
FIQ_STACK_LEGTH         EQU         0
IRQ_STACK_LEGTH         EQU         9*8     ;每层嵌套需要9个字堆栈，允许8层嵌套
ABT_STACK_LEGTH         EQU         0
UND_STACK_LEGTH         EQU         0

NoInt 					EQU 0x80 			;bit7，I 位
NoFIQ 					EQU 0x40 			;bit6，F 位

USR32Mode  				EQU 0x10			;用户模式
SVC32Mode   			EQU 0x13			;管理模式
SYS32Mode   			EQU 0x1f			;系统模式
IRQ32Mode   			EQU 0x12			;IRQ 中断
FIQ32Mode   			EQU 0x11			;快速中断

;引入的外部标号在这声明
    IMPORT __use_no_semihosting_swi
    IMPORT  FIQ_Exception                   ;快速中断异常处理程序
    IMPORT  __main                          ;C语言主程序入口 
    IMPORT  TargetResetInit                 ;目标板基本初始化

;给外部使用的标号在这声明
    EXPORT  Reset
    EXPORT  __user_initial_stackheap
	EXPORT  bottom_of_heap
	EXPORT	StackUsr
    
    PRESERVE8	 							;用Keil MDK编译时这条语句一定要加
	CODE32
    AREA    vectors,CODE,READONLY
 	ENTRY
;异常向量表
Reset
        LDR     PC, ResetAddr				;0x00，复位
        LDR     PC, UndefinedAddr			;0x04，未定义地址
        LDR     PC, SWI_Addr				;0x08，软件中断
        LDR     PC, PrefetchAddr			;0x0C，预取指中止
        LDR     PC, DataAbortAddr			;0x10，数据中止
        DCD     0xb9205f80					;0x14，保留
        LDR     PC, [PC, #-0xff0]			;0x18，IRQ 中断
        LDR     PC, FIQ_Addr				;0x1C，快速中断

ResetAddr 		DCD ResetInit 				;复位初始化处理程序地址
UndefinedAddr 	DCD Undefined 				;未定义指令处理程序地址
SWI_Addr 		DCD SoftwareInterrupt 		;软件中断处理程序地址
PrefetchAddr 	DCD PrefetchAbort 			;预取指中止处理程序地址
DataAbortAddr 	DCD DataAbort 				;数据中止处理程序地址
Nouse 			DCD 0 						;未使用
IRQ_Addr 		DCD 0 						;IRQ 中断，已在LDR PC, [PC, #-0xff0]中处理了
FIQ_Addr 		DCD FIQ_Handler 			;快速中断处理程序地址

Undefined									;未定义指令
        B       Undefined
PrefetchAbort								;取指令中止
        B       PrefetchAbort
DataAbort									;取数据中止
        B       DataAbort
FIQ_Handler									;快速中断
        STMFD   SP!, {R0-R3, LR}
        BL      FIQ_Exception
        LDMFD   SP!, {R0-R3, LR}
        SUBS    PC,  LR,  #4

;软中断，中断号 0～3 已经占用
SoftwareInterrupt
		CMP 	R0, #4 						;判断传过来的参数是否大于4
		LDRLO 	PC, [PC, R0, LSL #2] 		;小于的话(参数正确)，进行查表
		MOVS 	PC, LR 						;大于或者等于(参数出错)则返回
SwiFunction
		DCD 	IRQDisable 					;0 号调用，禁止IRQ中断
		DCD 	IRQEnable 					;1 号调用，使能IRQ中断
		DCD 	FIQDisable 					;2 号调用，禁止FIQ中断
		DCD 	FIQEnable 					;3 号调用，使能FIQ中断
IRQDisable 									;禁止IRQ中断
		MRS 	R0, SPSR 					;读取SPSR的值
		ORR 	R0, R0, #NoInt 				;置位I位，设置关IRQ断
		MSR 	SPSR_c, R0 					;回写SPSR
		MOVS 	PC, LR 						;返回
IRQEnable 									;使能IRQ中断
		MRS 	R0, SPSR
		BIC 	R0, R0, #NoInt 				;清零I位，设置开IRQ中断
		MSR 	SPSR_c, R0
		MOVS 	PC, LR
FIQDisable 									;禁止FIQ中断
		MRS 	R0, SPSR
		ORR 	R0, R0, #NoFIQ 				;置位F位，设置关FIQ中断
		MSR 	SPSR_c, R0
		MOVS 	PC, LR
FIQEnable 									;使能FIQ中断
		MRS 	R0, SPSR
		BIC 	R0, R0, #NoFIQ 				;清零F位，设置开FIQ中断
		MSR 	SPSR_c, R0
		MOVS 	PC, LR
									
; 初始化堆栈
InitStack    
        MOV     R0, LR
        MSR     CPSR_c, #0xd2		        ;设置中断模式堆栈
        LDR     SP, StackIrq
        MSR     CPSR_c, #0xd1				;设置快速中断模式堆栈
        LDR     SP, StackFiq
        MSR     CPSR_c, #0xd7				;设置中止模式堆栈
        LDR     SP, StackAbt
        MSR     CPSR_c, #0xdb				;设置未定义模式堆栈
        LDR     SP, StackUnd
        MSR     CPSR_c, #0xdf				;设置系统模式堆栈
        LDR     SP, =StackUsr
        BX 		R0
		
; 复位入口
ResetInit       
        BL      InitStack               	;初始化堆栈
        BL      TargetResetInit         	;目标板基本初始化
        B       __main          			;跳转到c语言入口

;代码加密处理
	IF :DEF: EN_CRP
        IF  . >= 0x1fc
        INFO    1,"\nThe data at 0x000001fc must be 0x87654321.\nPlease delete some source before this line."
        ENDIF
CrpData
    	WHILE . < 0x1fc
    	NOP									;循环用NOP填充，直到0x1FC
   	 	WEND
CrpData1
    	DCD     0x87654321          		;当此数为0x87654321时，用户代码被保护 
    ENDIF

; 库函数初始化堆和栈，不能删除
__user_initial_stackheap    
    	LDR   	R0,=bottom_of_heap
    	BX 		LR

StackSvc DCD SvcStackSpace + (SVC_STACK_LEGTH - 1)* 4 	;管理模式堆栈
StackIrq DCD IrqStackSpace + (IRQ_STACK_LEGTH - 1)* 4 	;IRQ 模式堆栈
StackFiq DCD FiqStackSpace + (FIQ_STACK_LEGTH - 1)* 4 	;FIQ 模式堆栈
StackAbt DCD AbtStackSpace + (ABT_STACK_LEGTH - 1)* 4 	;中止模式堆栈
StackUnd DCD UndtStackSpace + (UND_STACK_LEGTH - 1)* 4 	;未定义模式堆栈

; 分配堆栈空间 
        AREA    MyStacks, DATA, NOINIT, ALIGN=2		 	;MyStacks 通过分散加载文件定位
SvcStackSpace 	SPACE 	SVC_STACK_LEGTH * 4  			;管理模式堆栈空间
IrqStackSpace   SPACE   IRQ_STACK_LEGTH * 4  			;中断模式堆栈空间
FiqStackSpace   SPACE   FIQ_STACK_LEGTH * 4  			;快速中断模式堆栈空间
AbtStackSpace   SPACE   ABT_STACK_LEGTH * 4  			;中止义模式堆栈空间
UndtStackSpace  SPACE   UND_STACK_LEGTH * 4  			;未定义模式堆栈

        AREA    Heap, DATA, NOINIT	   					;Heap 通过分散加载文件定位
bottom_of_heap    SPACE   1
        AREA    Stacks, DATA, NOINIT   					;Stacks 通过分散加载文件定位
StackUsr		  SPACE   1

    END													;文件结束
