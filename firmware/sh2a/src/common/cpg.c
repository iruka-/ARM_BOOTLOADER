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
*   File Name   : cpg.c
*   Abstract    : CPG setting process
*   Version     : 1.02.00
*   Device      : SH7262/SH7264
*   Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
*               : C/C++ compiler package for the SuperH RISC engine family
*               :                             (Ver.9.02 Release00).
*   OS          : None
*   H/W Platform: M3A-HS64G50(CPU board)
*   Description : 
********************************************************************************
*   History     : Oct.28,2008 Ver.1.00.00  
*               : Jun.29,2009 Ver.1.01.00 Changed FILE FORMAT
*               : Jul.06,2009 Ver.1.02.00 Added CLK_18MHz
*""FILE COMMENT END""**********************************************************/
#include "iodefine.h"


/* ==== Prototype Declaration ==== */
void io_set_cpg(void);

#pragma section ResetPRG


#define	 CLK_18MHz 1

/*""FUNC COMMENT""**************************************************************
 * ID           : 
 * Outline      : CPG settings
 *------------------------------------------------------------------------------
 * Include      : iodefine.h
 *------------------------------------------------------------------------------
 * Declaration  : void io_set_cpg(void);
 *------------------------------------------------------------------------------
 * Description  : Clock pulse generator (CPG) is set to set to the internal clock 
 *              : (I Clock), peripheral clock (P Clock), bus clock (B Clock), and 
 *              : I Clock = 144MHz, B Clock = 72MHz,  P Clock = 36MHz.
 *              : This setting example is the case that the function's input clock
 *              : is 18MHz and clock mode is 2.
 *------------------------------------------------------------------------------
 * Argument     : void
 *------------------------------------------------------------------------------
 * Return Value : void
 *------------------------------------------------------------------------------
 * Note         : None
 *""FUNC COMMENT END""**********************************************************/
void io_set_cpg(void)
{
	/* ==== CPG Setting ==== */
#if	(CLK_18MHz == 0)
	/*B Clock = 48MHz				*/
	CPG.FRQCR.WORD = 0x1104u;		/* PLL1(x12),I:B:P=12:4:2
									* CKIO:Output at time usually,Output when bus right is opened,output at standby"L"
									* Clockin = 12MHz, CKIO = 48MHz 
									* I Clock = 144MHz, B Clock = 48MHz, 
									* P Clock = 24MHz
									*/
#else

//‚±‚¿‚ç‚ð‘I‘ð
	/*B Clock = 72MHz				*/
	CPG.FRQCR.WORD = 0x1003u;		/* PLL1(x8),I:B:P= 8:4:2 
									* CKIO:Output at time usually,Output when bus right is opened,output at standby"L"
									* Clockin = 18MHz, CKIO = 72MHz 
									* I Clock = 144MHz, B Clock = 72MHz, 
									* P Clock = 36MHz
									*/
#endif

    /* ---- The clock of all modules is permitted. ---- */
	CPG.STBCR3.BYTE = 0x02u;		/* Port level is keep in standby mode						*/
									/* IEBus, MTU2,SDHI0, SDHI1, A/D, [1], RTClock				*/
	CPG.STBCR4.BYTE = 0x00u;		/* SCIF0, SCIF1, SCIF2, SCIF3, SCIF4, SCIF5, SCIF6, SCIF7	*/
	CPG.STBCR5.BYTE = 0x10u;		/* I2C30, I2C31, I2C32, [1], RCAN0, RCAN1, RSPI0, RSPI1		*/
	CPG.STBCR6.BYTE = 0x00u;		/* SSI0, SSI1, SSI2, SSI3, CD-ROMDEC, SRC0, SRC1, USB		*/
	CPG.STBCR7.BYTE = 0x2au;		/* SIOF, RSPDIF, [1], VDC3, [1], CMT, [1], NAND				*/
	CPG.STBCR8.BYTE = 0x7eu;		/* PWM, [1], [1], [1], [1], [1], [1], DECOMP				*/

}

/* End of File */