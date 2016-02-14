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
*   Copyright (C) 2009. Renesas Technology Corp., All Rights Reserved.
*""FILE COMMENT""*********** Technical reference data **************************
*   System Name : SH7264 Sample Program
*   File Name   : hwsetup.c
*   Abstract    : Hardware initialization function
*   Version     : 1.01.00
*   Device      : SH7262/SH7264
*   Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
*               : C/C++ compiler package for the SuperH RISC engine family
*               :                             (Ver.9.02 Release00).
*   OS          : None
*   H/W Platform: M3A-HS64G50(CPU board)
*   Description : 
********************************************************************************
*   History     : Jan.13,2009 Ver.1.00.00  
*               : Jun.29,2009 Ver.1.01.00 Changed FILE FORMAT
*""FILE COMMENT END""**********************************************************/
#include "iodefine.h"

/* ==== Prototype declaration ==== */
void  HardwareSetup(void);

/* ==== referenced external Prototype declaration ==== */
extern void io_set_cpg(void);
//extern void io_init_bsc_cs0(void);
//extern void io_init_sdram(void);
//extern void io_init_cache(void);
//static void init_puram_section(void);
void set_acswr(void);

#pragma section ResetPRG
/*""FUNC COMMENT""**************************************************************
 * ID           : 
 * Outline      : Hardware initialization function
 *------------------------------------------------------------------------------
 * Include      : iodefine.h
 *------------------------------------------------------------------------------
 * Declaration  : void  HardwareSetup(void);
 *------------------------------------------------------------------------------
 * Description  : The initial settings of CPG, PFC, and BSC (Flash memory  
 *              : access control and SDRAM initialization) are processed.
 *------------------------------------------------------------------------------
 * Argument     : void
 *------------------------------------------------------------------------------
 * Return Value : void
 *------------------------------------------------------------------------------
 * Note         : None
 *""FUNC COMMENT END""**********************************************************/
void  HardwareSetup(void)
{
	/*====CPG setting====*/
	io_set_cpg();
	
	/*====CS0 initialization====*/	
//	io_init_bsc_cs0();
	
	/*====SDRAM area initialization====*/
	/* ---- Switches AC characteristics ---- */
//	init_puram_section();
//	set_acswr();
	
//	io_init_sdram();

	/*====Cache setting====*/
//	io_init_cache();
}

/*""FUNC COMMENT""**************************************************************
 * ID           : 
 * Outline      : URAM section transfer from ROM to internal RAM
 *------------------------------------------------------------------------------
 * Include      : iodefine.h
 *------------------------------------------------------------------------------
 * Declaration  : static void init_puram_section(void);
 *------------------------------------------------------------------------------
 * Description  : Transfers the program in the URAM section from 
 *              : ROM to internal RAM.
 *              : Transfer must be executed before setting the SDRAM.
 *              : This function transfers the URAM section separately before
 *              : initializing other sections.
 *------------------------------------------------------------------------------
 * Argument     : void
 *------------------------------------------------------------------------------
 * Return Value : void
 *------------------------------------------------------------------------------
 * Note         : None
 *""FUNC COMMENT END""**********************************************************/
static void init_puram_section(void)
{
	unsigned long *src, *end, *dst;
	
	src = (unsigned long *)__sectop("PURAM");
	end = (unsigned long *)__secend("PURAM");
	dst = (unsigned long *)__sectop("RPURAM");
	
	while(src < end){
		*dst++ = *src++;
	}	
}

#pragma section URAM
/*""FUNC COMMENT""**************************************************************
 * ID           : 
 * Outline      : AC characteristics switch function
 *------------------------------------------------------------------------------
 * Include      : iodefine.h
 *------------------------------------------------------------------------------
 * Declaration  : void set_acswr(void);
 *------------------------------------------------------------------------------
 * Description  : Extends the AC characteristics delay time.
 *------------------------------------------------------------------------------
 * Argument     : void
 *------------------------------------------------------------------------------
 * Return Value : void
 *------------------------------------------------------------------------------
 * Note         : None
 *""FUNC COMMENT END""**********************************************************/
void set_acswr(void)
{
	volatile unsigned long reg;
	
	/* ==== Sequence to write to the ACSWR register ==== */
	do{
		BSC.ACKEYR = 0;
		BSC.ACKEYR = 0;
		BSC.ACSWR.LONG = 0x2;		/* Extends the delay time   */
	}while(BSC.ACSWR.LONG != 0x2 );	/* Checks ACSWR register correctly written */

}

/* End of File */
