/******************************************************************************
*   DISCLAIMER
*
*   This software is supplied by Renesas Technology Corp. and is only 
*   intended for use with Renesas products. No other uses are authorized.
*
*   This software is owned by Renesas Technology Corp. and is protected under 
*   all applicable laws, including copyright laws.
*
*   THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
*   REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
*   INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
*   PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
*   DISCLAIMED.
*
*   TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
*   TECHNOLOGY CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
*   FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
*   FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
*   AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
*   Renesas reserves the right, without notice, to make changes to this 
*   software and to discontinue the availability of this software.
*   By using this software, you agree to the additional terms and 
*   conditions found by accessing the following link: 
*   http://www.renesas.com/disclaimer
********************************************************************************
*   Copyright (C) 2008(2009). Renesas Technology Corp., All Rights Reserved.
*""FILE COMMENT""*********** Technical reference data **************************
*   System Name : SH7264 Sample Program
*   File Name   : resetprg.c
*   Abstract    : Reset Program
*   Version     : 1.01.00
*   Device      : SH7262/SH7264
*   Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
*               : C/C++ compiler package for the SuperH RISC engine family
*               :                             (Ver.9.02 Release00).
*   OS          : None
*   H/W Platform: M3A-HS64G50(CPU board)
*   Description : 
********************************************************************************
*   History     : Dec.03,2008 Ver.1.00.00  
*               : Jun.29,2009 Ver.1.01.00 Changed FILE FORMAT
*""FILE COMMENT END""**********************************************************/
#include <machine.h>
#include <_h_c_lib.h>
#include "stacksct.h"
#include "iodefine.h"

#define FPSCR_Init    0x00040001

#define SR_Init    0x000000F0
#define INT_OFFSET 0x10

extern unsigned int INT_Vectors;
void PowerON_Reset_PC(void);
void Manual_Reset_PC(void);

extern void main(void);
extern void HardwareSetup(void);
extern void io_cache_writeback(void);
extern void _INIT_IOLIB(void);
void set_imask( int imask )   ;

extern	int	__fpscr_values[2];

//extern void srand(unsigned int);	// Remove the comment when you use rand()
//extern char *_s1ptr;			// Remove the comment when you use strtok()		

/*==== Switch section name to ResetPRG ====*/
#pragma section ResetPRG

/*==== Specify the entry function ====*/
#pragma entry PowerON_Reset_PC

/*""FUNC COMMENT""**************************************************************
 * ID           : 
 * Outline      : CPU initialization function
 *------------------------------------------------------------------------------
 * Include      : iodefine.h
 *------------------------------------------------------------------------------
 * Declaration  : void PowerON_Reset_PC(void);
 *------------------------------------------------------------------------------
 * Description  : It is the CPU initialization process to register the power on 
 *              : reset exception vector table.
 *              : This function is firstly executed after power on reset.
 *              : Enable the processes that are commented depending on its needs.
 *------------------------------------------------------------------------------
 * Argument     : void
 *------------------------------------------------------------------------------
 * Return Value : void
 *------------------------------------------------------------------------------
 * Note         : None
 *""FUNC COMMENT END""**********************************************************/
void PowerON_Reset_PC(void)
{ 	
	set_fpscr(FPSCR_Init);

#ifdef	KPIT_GCC
	init();
#endif

	/*==== B and D sections initialization ====*/ 
	_INITSCT();

	/*==== HardwareSetup function====*/	 
	HardwareSetup();	// Use Hardware Setup
		
//	io_cache_writeback();

#ifdef	USE_FPU

#ifndef	KPIT_GCC
#define FPSCR_Init0  0x00040001
#define FPSCR_Init1  0x001c0001 //bit20=SZ(ì]ëóÉTÉCÉY=double) bit19=PR(ââéZê∏ìx=dounle)
	__fpscr_values[0]=FPSCR_Init0;
	__fpscr_values[1]=FPSCR_Init1;

#else
	__set_fpscr(0x40000);
#endif

#endif


	/*==== Vector base register (VBR) setting ====*/
	set_vbr((void *)((char *)&INT_Vectors - INT_OFFSET));
	
//	_INIT_IOLIB();				// Use stdio I/O

//	errno=0;					// Remove the comment when you use errno
//	srand(1);					// Remove the comment when you use rand()
//	_s1ptr=NULL;				// Remove the comment when you use strtok()

	/*==== Status register setting ====*/ 
	set_cr(SR_Init);
	nop();

	/* ==== Bank number register setting ==== */
#ifdef	USE_RESBANK
	INTC.IBNR.BIT.BE = 0x01;	/* Use the register bank in all interrupts */
#else
	INTC.IBNR.BIT.BE = 0;		/* Disable register bank in all interrupts */
#endif

	/* ==== Interrupt mask level change ==== */
	set_imask(0);

	/*==== Function call of main function ====*/ 	
	main();
	
	/*==== sleep instruction execution ====*/ 
	sleep();
}

//#pragma entry Manual_Reset_PC		// Remove the comment when you use Manual Reset
/*""FUNC COMMENT""**************************************************************
 * ID           : 
 * Outline      : Manual reset process
 *------------------------------------------------------------------------------
 * Include      : 
 *------------------------------------------------------------------------------
 * Declaration  : void Manual_Reset_PC(void);
 *------------------------------------------------------------------------------
 * Description  : It is the function to register the manual reset exception vector table.
 *              : The process is not defined in the reference program.
 *              : Add the processes depending on its needs
 *------------------------------------------------------------------------------
 * Argument     : void
 *------------------------------------------------------------------------------
 * Return Value : void
 *------------------------------------------------------------------------------
 * Note         : None
 *""FUNC COMMENT END""**********************************************************/
void Manual_Reset_PC(void)	
{
	/* NOP */
}
/* END of File */
