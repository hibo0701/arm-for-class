
;�����ջ�Ĵ�С
SVC_STACK_LEGTH 		EQU 		0
FIQ_STACK_LEGTH         EQU         0
IRQ_STACK_LEGTH         EQU         9*8     ;ÿ��Ƕ����Ҫ9���ֶ�ջ������8��Ƕ��
ABT_STACK_LEGTH         EQU         0
UND_STACK_LEGTH         EQU         0

NoInt 					EQU 0x80 			;bit7��I λ
NoFIQ 					EQU 0x40 			;bit6��F λ

USR32Mode  				EQU 0x10			;�û�ģʽ
SVC32Mode   			EQU 0x13			;����ģʽ
SYS32Mode   			EQU 0x1f			;ϵͳģʽ
IRQ32Mode   			EQU 0x12			;IRQ �ж�
FIQ32Mode   			EQU 0x11			;�����ж�

;������ⲿ�����������
    IMPORT __use_no_semihosting_swi
    IMPORT  FIQ_Exception                   ;�����ж��쳣�������
    IMPORT  __main                          ;C������������� 
    IMPORT  TargetResetInit                 ;Ŀ��������ʼ��

;���ⲿʹ�õı����������
    EXPORT  Reset
    EXPORT  __user_initial_stackheap
	EXPORT  bottom_of_heap
	EXPORT	StackUsr
    
    PRESERVE8	 							;��Keil MDK����ʱ�������һ��Ҫ��
	CODE32
    AREA    vectors,CODE,READONLY
 	ENTRY
;�쳣������
Reset
        LDR     PC, ResetAddr				;0x00����λ
        LDR     PC, UndefinedAddr			;0x04��δ�����ַ
        LDR     PC, SWI_Addr				;0x08������ж�
        LDR     PC, PrefetchAddr			;0x0C��Ԥȡָ��ֹ
        LDR     PC, DataAbortAddr			;0x10��������ֹ
        DCD     0xb9205f80					;0x14������
        LDR     PC, [PC, #-0xff0]			;0x18��IRQ �ж�
        LDR     PC, FIQ_Addr				;0x1C�������ж�

ResetAddr 		DCD ResetInit 				;��λ��ʼ����������ַ
UndefinedAddr 	DCD Undefined 				;δ����ָ�������ַ
SWI_Addr 		DCD SoftwareInterrupt 		;����жϴ�������ַ
PrefetchAddr 	DCD PrefetchAbort 			;Ԥȡָ��ֹ��������ַ
DataAbortAddr 	DCD DataAbort 				;������ֹ��������ַ
Nouse 			DCD 0 						;δʹ��
IRQ_Addr 		DCD 0 						;IRQ �жϣ�����LDR PC, [PC, #-0xff0]�д�����
FIQ_Addr 		DCD FIQ_Handler 			;�����жϴ�������ַ

Undefined									;δ����ָ��
        B       Undefined
PrefetchAbort								;ȡָ����ֹ
        B       PrefetchAbort
DataAbort									;ȡ������ֹ
        B       DataAbort
FIQ_Handler									;�����ж�
        STMFD   SP!, {R0-R3, LR}
        BL      FIQ_Exception
        LDMFD   SP!, {R0-R3, LR}
        SUBS    PC,  LR,  #4

;���жϣ��жϺ� 0��3 �Ѿ�ռ��
SoftwareInterrupt
		CMP 	R0, #4 						;�жϴ������Ĳ����Ƿ����4
		LDRLO 	PC, [PC, R0, LSL #2] 		;С�ڵĻ�(������ȷ)�����в��
		MOVS 	PC, LR 						;���ڻ��ߵ���(��������)�򷵻�
SwiFunction
		DCD 	IRQDisable 					;0 �ŵ��ã���ֹIRQ�ж�
		DCD 	IRQEnable 					;1 �ŵ��ã�ʹ��IRQ�ж�
		DCD 	FIQDisable 					;2 �ŵ��ã���ֹFIQ�ж�
		DCD 	FIQEnable 					;3 �ŵ��ã�ʹ��FIQ�ж�
IRQDisable 									;��ֹIRQ�ж�
		MRS 	R0, SPSR 					;��ȡSPSR��ֵ
		ORR 	R0, R0, #NoInt 				;��λIλ�����ù�IRQ��
		MSR 	SPSR_c, R0 					;��дSPSR
		MOVS 	PC, LR 						;����
IRQEnable 									;ʹ��IRQ�ж�
		MRS 	R0, SPSR
		BIC 	R0, R0, #NoInt 				;����Iλ�����ÿ�IRQ�ж�
		MSR 	SPSR_c, R0
		MOVS 	PC, LR
FIQDisable 									;��ֹFIQ�ж�
		MRS 	R0, SPSR
		ORR 	R0, R0, #NoFIQ 				;��λFλ�����ù�FIQ�ж�
		MSR 	SPSR_c, R0
		MOVS 	PC, LR
FIQEnable 									;ʹ��FIQ�ж�
		MRS 	R0, SPSR
		BIC 	R0, R0, #NoFIQ 				;����Fλ�����ÿ�FIQ�ж�
		MSR 	SPSR_c, R0
		MOVS 	PC, LR
									
; ��ʼ����ջ
InitStack    
        MOV     R0, LR
        MSR     CPSR_c, #0xd2		        ;�����ж�ģʽ��ջ
        LDR     SP, StackIrq
        MSR     CPSR_c, #0xd1				;���ÿ����ж�ģʽ��ջ
        LDR     SP, StackFiq
        MSR     CPSR_c, #0xd7				;������ֹģʽ��ջ
        LDR     SP, StackAbt
        MSR     CPSR_c, #0xdb				;����δ����ģʽ��ջ
        LDR     SP, StackUnd
        MSR     CPSR_c, #0xdf				;����ϵͳģʽ��ջ
        LDR     SP, =StackUsr
        BX 		R0
		
; ��λ���
ResetInit       
        BL      InitStack               	;��ʼ����ջ
        BL      TargetResetInit         	;Ŀ��������ʼ��
        B       __main          			;��ת��c�������

;������ܴ���
	IF :DEF: EN_CRP
        IF  . >= 0x1fc
        INFO    1,"\nThe data at 0x000001fc must be 0x87654321.\nPlease delete some source before this line."
        ENDIF
CrpData
    	WHILE . < 0x1fc
    	NOP									;ѭ����NOP��䣬ֱ��0x1FC
   	 	WEND
CrpData1
    	DCD     0x87654321          		;������Ϊ0x87654321ʱ���û����뱻���� 
    ENDIF

; �⺯����ʼ���Ѻ�ջ������ɾ��
__user_initial_stackheap    
    	LDR   	R0,=bottom_of_heap
    	BX 		LR

StackSvc DCD SvcStackSpace + (SVC_STACK_LEGTH - 1)* 4 	;����ģʽ��ջ
StackIrq DCD IrqStackSpace + (IRQ_STACK_LEGTH - 1)* 4 	;IRQ ģʽ��ջ
StackFiq DCD FiqStackSpace + (FIQ_STACK_LEGTH - 1)* 4 	;FIQ ģʽ��ջ
StackAbt DCD AbtStackSpace + (ABT_STACK_LEGTH - 1)* 4 	;��ֹģʽ��ջ
StackUnd DCD UndtStackSpace + (UND_STACK_LEGTH - 1)* 4 	;δ����ģʽ��ջ

; �����ջ�ռ� 
        AREA    MyStacks, DATA, NOINIT, ALIGN=2		 	;MyStacks ͨ����ɢ�����ļ���λ
SvcStackSpace 	SPACE 	SVC_STACK_LEGTH * 4  			;����ģʽ��ջ�ռ�
IrqStackSpace   SPACE   IRQ_STACK_LEGTH * 4  			;�ж�ģʽ��ջ�ռ�
FiqStackSpace   SPACE   FIQ_STACK_LEGTH * 4  			;�����ж�ģʽ��ջ�ռ�
AbtStackSpace   SPACE   ABT_STACK_LEGTH * 4  			;��ֹ��ģʽ��ջ�ռ�
UndtStackSpace  SPACE   UND_STACK_LEGTH * 4  			;δ����ģʽ��ջ

        AREA    Heap, DATA, NOINIT	   					;Heap ͨ����ɢ�����ļ���λ
bottom_of_heap    SPACE   1
        AREA    Stacks, DATA, NOINIT   					;Stacks ͨ����ɢ�����ļ���λ
StackUsr		  SPACE   1

    END													;�ļ�����
