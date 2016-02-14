/* ***********************************************************************************
	crt.s						STARTUP  ASSEMBLY  CODE 
								-----------------------
	Module includes the interrupt vectors and start-up code.
  ************************************************************************************ */

/* Stack Sizes */
.set  UND_STACK_SIZE, 0x00000040	/* stack for "undefined instruction" interrupts				*/
.set  ABT_STACK_SIZE, 0x00000040	/* stack for "abort" interrupts								*/
.set  FIQ_STACK_SIZE, 0x00000040	/* stack for "FIQ" interrupts								*/
.set  IRQ_STACK_SIZE, 0X00000100	/* stack for "IRQ" normal interrupts						*/
.set  SVC_STACK_SIZE, 0x00000400	/* stack for "SVC" supervisor mode							*/



/* Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs (program status registers) */
.set  MODE_USR, 0x10            /* Normal User Mode 										*/
.set  MODE_FIQ, 0x11            /* FIQ Processing Fast Interrupts Mode 						*/
.set  MODE_IRQ, 0x12            /* IRQ Processing Standard Interrupts Mode 					*/
.set  MODE_SVC, 0x13            /* Supervisor Processing Software Interrupts Mode 			*/
.set  MODE_ABT, 0x17            /* Abort Processing memory Faults Mode 						*/
.set  MODE_UND, 0x1B            /* Undefined Processing Undefined Instructions Mode 		*/
.set  MODE_SYS, 0x1F            /* System Running Priviledged Operating System Tasks  Mode	*/

.set  I_BIT, 0x80               /* when I bit is set, IRQ is disabled (program status registers) */
.set  F_BIT, 0x40               /* when F bit is set, FIQ is disabled (program status registers) */


.set LPC_BASE_VIC,		0xFFFFF000
.set VIC_IntSelect,		0x00C
.set VIC_IntEnable,		0x010
.set VIC_IntEnClear,	0x014
.set VIC_Protection,	0x020
.set VIC_SWPriorityMask,0x024
.set VIC_VectAddr0,		0x100
.set VIC_VectPriority0,	0x200
.set VIC_VectAddr,		0xF00


/* ************************************************************************************ */
# .text
.section .VECTOR

.arm

.global	Reset_Handler
.global _startup

.global g_pfnVectors

.func   _startup

_startup:


# Exception Vectors
g_pfnVectors:
_vectors:   ldr     PC, Reset_Addr         
            ldr     PC, Undef_Addr
            ldr     PC, SWI_Addr
            ldr     PC, PAbt_Addr
            ldr     PC, DAbt_Addr
/*			.word	0		* Reserved Vector (holds Philips ISP checksum) */
			.word	0xb8a06f60
            ldr     PC, IRQ_Addr
            ldr     PC, FIQ_Addr

Reset_Addr: .word   Reset_Handler		/* defined in this module below  */
Undef_Addr: .word   UNDEF_Routine		/* defined in main.c  */
SWI_Addr:   .word   SWI_Routine			/* defined in main.c  */
PAbt_Addr:  .word   UNDEF_Routine		/* defined in main.c  */
DAbt_Addr:  .word   UNDEF_Routine		/* defined in main.c  */
IRQ_Addr:   .word   IRQ_Routine			/* defined in main.c  */
FIQ_Addr:   .word   FIQ_Routine			/* defined in main.c  */
            .word   0		/* rounds the vectors and ISR addresses to 64 bytes total  */

/* ************************************************************************************ */

IRQ_Routine:

loop1:  nop
/*		b		loop1 */

        sub     LR, LR, #4              /* Adjust LR_irq and push it */
        stmfd   SP!, {LR}

        mrs     LR, SPSR                /* Save SPSR need to be saved for nested interrupt */
        stmfd   SP!, {R0-R3,IP,LR}      /* Push scratch/used registers and SPSR */
        ldr     R0, =LPC_BASE_VIC       /* Get the ISR address pointed by VIC_VectAddr */
        ldr     R0, [R0, #VIC_VectAddr]
        msr     CPSR_c, #MODE_SVC       /* Enter SVC mode and enable Irq and Fiq */

        stmfd   SP!, {LR}               /* Call the ISR */
        mov     LR, PC
        bx      R0
        ldmia   SP!, {LR}

        msr     CPSR_c, #MODE_IRQ | I_BIT  /* Enter IRQ mode and disable Irq */
        ldmia   SP!, {R0-R3,IP,LR}      /* Restore scratch/used registers and SPSR */
        msr     SPSR_cxsf, LR           /* Restore SPSR_irq */

        ldr     LR, =LPC_BASE_VIC       /* Issue EOI command to the VIC */
        str     LR, [LR, #VIC_VectAddr]

        ldmia   SP!, {PC}^              /* Returen from the IRQ handler */




/* ************************************************************************************ */
# Reset Handler

Reset_Handler:

/* Setup a stack for each mode - note that this only sets up a usable stack
   for User mode.   Also each mode is setup with interrupts initially disabled. */

    	ldr   r0, =_stack_end
    	msr   CPSR_c, #MODE_UND|I_BIT|F_BIT 	/* Undefined Instruction Mode  */
    	mov   sp, r0
    	sub   r0, r0, #UND_STACK_SIZE
    	msr   CPSR_c, #MODE_ABT|I_BIT|F_BIT 	/* Abort Mode */
    	mov   sp, r0
    	sub   r0, r0, #ABT_STACK_SIZE
    	msr   CPSR_c, #MODE_FIQ|I_BIT|F_BIT 	/* FIQ Mode */
    	mov   sp, r0	
   		sub   r0, r0, #FIQ_STACK_SIZE
    	msr   CPSR_c, #MODE_IRQ|I_BIT|F_BIT 	/* IRQ Mode */
    	mov   sp, r0
    	sub   r0, r0, #IRQ_STACK_SIZE
    	msr   CPSR_c, #MODE_SVC|I_BIT|F_BIT 	/* Supervisor Mode */
    	mov   sp, r0
    	sub   r0, r0, #SVC_STACK_SIZE
    	msr   CPSR_c, #MODE_SYS|I_BIT|F_BIT 	/* User Mode */
    	mov   sp, r0

		/* copy .data section (Copy from ROM to RAM) */
        ldr     R1, =_etext
        ldr     R2, =_data
        ldr     R3, =_edata
1:      cmp     R2, R3
        ldrlo   R0, [R1], #4
        strlo   R0, [R2], #4
        blo     1b

		/* Clear .bss section (Zero init)  */
        mov     R0, #0
        ldr     R1, =_bss_start
        ldr     R2, =_bss_end
2:		cmp     R1, R2
        strlo   R0, [R1], #4
        blo     2b

		/* Enter the C code  */
        b       main



/*****************************************************************************
//*   swi_handler.s: SWI handler file for NXP LPC230x Family Microprocessors
//*
//*   Copyright(C) 2006, NXP Semiconductor
//*   All rights reserved.
//*
//*   History
//*   2006.09.01  ver 1.00    Prelimnary version, first Release
//*
//*****************************************************************************/

/* modified by Martin Thomas to extrace SWI-number from
   SWI instruction parameter */

.equ SWI_IRQ_DIS,  0
.equ SWI_IRQ_EN,   1
.equ SWI_FIQ_DIS,  2
.equ SWI_FIQ_EN,   3
.equ SWI_CLR_VECT, 4
.equ SWI_REG_VECT, 5
.equ NUM_SWI,	   6

/*	The exported labels and functions */
	.global SoftwareInterrupt    
	.global IntEnable
	.global IntDisable
	.global ClearVector
	.global RegisterVector

/********************************************************************
//*  SWI interrupt handler											*
//*  Function : SoftwareInterrupt(SWI_Number)						*
//*  The SWI_Number is 0 through 3									*
//*  See below "SwiFunction" table below							*
//*  Parameters:    None											*
//*  input  :       SWI_Number										*
//*  output :       None											*
//*  mthomas: Expects that SWI instruction is called from ARM-mode  *
//*           (not thumb). This is the case if the functions below  *
//*           (IntEnable/IntDisable) get used.
//*******************************************************************/
SWI_Routine:
SoftwareInterrupt:
	LDR		IP, [LR, #-4]		@ Get swi instruction code (assuming ARM state)
	AND		IP, #0xFF			@ Get swi comment field (lower 8 bit)
	CMP		IP, #NUM_SWI		@ Check range
	LDRLO	PC, [PC, IP, LSL #2]	@ Jump each service function when code is valid
	MOVS	PC, LR				@ Otherwise return

SwiFunction:
	.word IRQDisable
	.word IRQEnable
	.word FIQDisable
	.word FIQEnable
	.word ClearVect
	.word RegVect

	LDR		R0, [LR, #-4]          /* get swi instruction code (ARM-mode) */
	BIC		R0, R0, #0xff000000    /* clear top 8 bits leaving swi "comment field"=number */
	CMP		R0, #NUM_SWI           /* range check */
	LDRLO	PC, [PC, R0, LSL #2]   /* get jump-address from table */
	MOVS	PC, LR                 /* if out of range: do nothing and return */

IRQDisable:
	MRS		R0, SPSR
	ORR		R0, R0, #I_BIT
	MSR		SPSR_c, R0
	MOVS	PC, LR

IRQEnable:
	MRS		R0, SPSR
	BIC		R0, R0, #I_BIT
	MSR		SPSR_c, R0
	MOVS	PC, LR

FIQDisable:
	MRS		R0, SPSR
	ORR		R0, R0, #F_BIT
	MSR		SPSR_c, R0
	MOVS	PC, LR

FIQEnable:
	MRS		R0, SPSR
	BIC		R0, R0, #F_BIT
	MSR		SPSR_c, R0
	MOVS	PC, LR

ClearVect:
	LDR		IP, =LPC_BASE_VIC
	MVN		R0, #0						/* Disable all interrupts */
	STR		R0, [IP, #VIC_IntEnClear]
	MOV		R0, R0, LSR #16				/* Unmask all interrupt levels */
	STR		R0, [IP, #VIC_SWPriorityMask]
	MOV		R0, #1						/* Enable protection */
	STR		R0, [IP, #VIC_Protection]
	STR		R0, [IP, #VIC_VectAddr]		/* Issule EOI command */
	MOVS	PC, LR

RegVect:
	CMP		R0, #32						/* Range check */
	MOVCSS	PC, LR
	LDR		IP, =(LPC_BASE_VIC+VIC_VectAddr0)
	STR		R1, [IP, R0, LSL #2]		/* Set VICVectVectAddr<n> */
	LDR		IP, =(LPC_BASE_VIC+VIC_VectPriority0)
	STR		R2, [IP, R0, LSL #2]		/* Set VICVectPriority<n> */
	MOV		R1, #1
	MOV		R1, R1, LSL R0
	LDR		IP, =LPC_BASE_VIC
	LDR		R2, [IP, #VIC_IntSelect]	/* Set corresponding bit in the VICIntSelect */
	BIC		R2, R1
	CMP		R3, #1
	ORREQ	R2, R1
	STR		R2, [IP, #VIC_IntSelect]
	STR		R1, [IP, #VIC_IntEnable]	/* Enable corresponding interrupt */
	MOVS	PC, LR


/***********************************************************************
//*  Call SWI to enable IRQ                                            *
//*  Function : void IntEnable(void)                                   *
//*  Parameters:      None                                             *
//*  input  :         None                                             *
//*  output :         None                                             *
//**********************************************************************/
IntEnable:
	SWI     SWI_IRQ_EN
	BX      lr

.global IrqEnable
IrqEnable:
	SWI	SWI_IRQ_EN
	BX	LR

/***********************************************************************
//*  Call SWI to disable IRQ                                           *
//*  Function : void IntDisable(void)                                  *
//*  Parameters     : None                                             *
//*  input          : None                                             *
//*  output         : None                                             *
//**********************************************************************/
IntDisable:
	SWI     SWI_IRQ_DIS
	BX      lr


ClearVector:
	SWI	SWI_CLR_VECT
	BX	LR

RegisterVector:
	SWI	SWI_REG_VECT
	BX	LR

.endfunc

/* ************************************************************************************ */

.end

