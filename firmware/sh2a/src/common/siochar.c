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
*   File Name   : siochar.c
*   Abstract    : sio interface getchar / putchar
*   Version     : 1.02.00
*   Device      : SH7262/SH7264
*   Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
*               : C/C++ compiler package for the SuperH RISC engine family
*               :                             (Ver.9.02 Release00).
*   OS          : None
*   H/W Platform: M3A-HS64G50(CPU board)
*   Description : 
********************************************************************************
*   History     : Feb.02,2009 Ver.1.00.00  
*               : Jun.29,2009 Ver.1.01.00 Changed FILE FORMAT
*               : Jul.06,2009 Ver.1.02.00 Added CLK_18MHz 
*               : Apr.25.2010 Interface SH-2A
*""FILE COMMENT END""**********************************************************/

#include "iodefine.h"


/* ====  Prototype function ==== */
void io_init_scif2(void);
int io_recvcheck(void);
char io_getchar(void);
void io_putchar(char c);

/*""FUNC COMMENT""**************************************************************
 * ID           : 
 * Outline      : Initialize SCFI0
 *------------------------------------------------------------------------------
 * Include      : 
 *------------------------------------------------------------------------------
 * Declaration  : void io_init_scif0(void);
 *------------------------------------------------------------------------------
 * Description  : This function initializes SCIF channel 0 as UART mode.
 *              : The transmit and the receive of SCIF channel 0 are enabled.
 *------------------------------------------------------------------------------
 * Argument     : void
 *------------------------------------------------------------------------------
 * Return Value : void
 *------------------------------------------------------------------------------
 * Note         : None
 *""FUNC COMMENT END""**********************************************************/
void io_init_scif2(void)
{
	/* ==== SCIF2 initial setting ==== */
	/* ---- Serial control register (SCSCRi) setting ---- */
	SCIF2.SCSCR.WORD = 0x0000;	/* SCIF2 transmitting and receiving operations stop */

	/* ---- FIFO control register (SCFCRi) setting ---- */
	SCIF2.SCFCR.BIT.TFRST = 1;	/* Transmit FIFO reset */

	/* ---- FIFO control register (SCFCRi) setting ---- */
	SCIF2.SCFCR.BIT.RFRST = 1;	/* Receive FIFO data register reset */

	/* ---- Serial status register(SCFSRi) setting ---- */
	SCIF2.SCFSR.WORD &= 0xff6e;	/* ER,BRK,DR bit clear */

	/* ---- Line status register (SCLSRi) setting ---- */
	SCIF2.SCLSR.BIT.ORER = 0;	/* ORER bit clear */

	/* ---- Serial control register (SCSCRi) setting ---- */
	SCIF2.SCSCR.BIT.CKE = 0x0;	/* B'00 : Internal CLK */

	/* ---- Serial mode register (SCSMRi) setting ---- */
	SCIF2.SCSMR.WORD = 0x0000;
								/*	Communication mode 	0: Asynchronous mode	      */
								/*	Character length   	0: 8-bit data			      */
								/*	Parity enable      	0: Add and check are disabled */
								/*	Stop bit length   	0: 1 stop bit 				  */
								/*	Clock select       	0: Table value 			      */

	/* ---- Bit rate register (SCBRRi) setting ---- */
	SCIF2.SCEMR.WORD = 0x0081;	/* BGDM=1(î{ë¨ÉÇÅ[Éh) ABCS=1(bpsÇÃ8î{) */
	SCIF2.SCBRR.BYTE = 25;		/* 115200bps */

	/* ---- FIFO control register (SCFCRi) setting ---- */
	SCIF2.SCFCR.WORD = 0x0030;	
								/* RTS output active trigger        :Initial value */
								/* Receive FIFO data trigger        :14-data       */
								/* Modem control enable             :Disabled      */
								/* Receive FIFO data register reset :Disabled      */
								/* Loop-back test                   :Disabled      */

	/* ---- Serial port register (SCSPTRi) setting ---- */
	SCIF2.SCSPTR.WORD |= 0x0003;	/* Serial port  break output(SPB2IO)  1: Enabled */
									/* Serial port break data(SPB2DT)  1: High-level */

	/* ==== Pin function controller (PFC) setting ==== */
	PORT.PFCR0.BIT.PF1MD = 0x4;	/* Switch to TxD2 pin */

	/* ---- Pin function controller (PFC) setting ---- */
	PORT.PFCR0.BIT.PF2MD = 0x4;	/* Switch to RxD2 pin */

	/* ---- Serial control register (SCSCRi) setting ---- */
	SCIF2.SCSCR.WORD = 0x0030;
							/* SCIF2 transmitting and receiving operations are enabled */

}


/***************************/
/* SCIF2 Recv buffer check */
/***************************/
int io_recvcheck(void)
{
	/* Confirming receive error(ER,DR,BRK) */
	if (SCIF2.SCFSR.WORD & 0x09c) {
		/* Detect receive error */
		SCIF2.SCSCR.BIT.RE = 0;		/* Disable reception */
		SCIF2.SCFCR.BIT.RFRST = 1;	/* Reset receiving FIFO */
		SCIF2.SCFCR.BIT.RFRST = 0;	/* Clearing FIFO reception reset */
		SCIF2.SCFSR.WORD &= ~0x9c;	/* Error bit clear */
		SCIF2.SCSCR.BIT.RE = 1;		/* Enable reception */
		return 0;
	}

	/* Is there receive FIFO data? */
	if (SCIF2.SCFSR.BIT.RDF == 0) return 0;
	return -1;
}



/*""FUNC COMMENT""**************************************************************
 * ID           : 
 * Outline      : Obtaining character
 *------------------------------------------------------------------------------
 * Include      : 
 *------------------------------------------------------------------------------
 * Declaration  : char io_getchar(void);
 *------------------------------------------------------------------------------
 * Description  : One character is received from SCIF2, and it's data is returned.
 *              : This function keeps waiting until it can obtain the receiving data.
 *------------------------------------------------------------------------------
 * Argument     : void
 *------------------------------------------------------------------------------
 * Return Value : Character to receive (Byte).
 *------------------------------------------------------------------------------
 * Note         : None
 *""FUNC COMMENT END""**********************************************************/
char io_getchar(void)
{
	char data;

	/* Confirming receive error(ER,DR,BRK) */
	if (SCIF2.SCFSR.WORD & 0x09c) {
		/* Detect receive error */
		SCIF2.SCSCR.BIT.RE = 0;		/* Disable reception */
		SCIF2.SCFCR.BIT.RFRST = 1;	/* Reset receiving FIFO */
		SCIF2.SCFCR.BIT.RFRST = 0;	/* Clearing FIFO reception reset */
		SCIF2.SCFSR.WORD &= ~0x9c;	/* Error bit clear */
		SCIF2.SCSCR.BIT.RE = 1;		/* Enable reception */
		return 0;
	}

	/* Is there receive FIFO data? */
	while(SCIF2.SCFSR.BIT.RDF == 0){
		/* WAIT */
	}

	/* Read receive data */
	data = SCIF2.SCFRDR.BYTE;
	/* Clear RDF */
	SCIF2.SCFSR.BIT.RDF = 0;

	/* Is it overflowed? */
	if(SCIF2.SCLSR.BIT.ORER == 1){
		SCIF2.SCLSR.BIT.ORER = 0;		/* ORER clear */
	}

	return data;
}



/*""FUNC COMMENT""**************************************************************
 * ID           : 
 * Outline      : Character output
 *------------------------------------------------------------------------------
 * Include      : 
 *------------------------------------------------------------------------------
 * Declaration  : void io_putchar(char c);
 *------------------------------------------------------------------------------
 * Description  : Character c is output to SCIF2.
 *              : This function keeps waiting until it becomes the transmission
 *              : enabled state.
 *------------------------------------------------------------------------------
 * Argument     : char c : character to output
 *------------------------------------------------------------------------------
 * Return Value : void
 *------------------------------------------------------------------------------
 * Note         : None
 *""FUNC COMMENT END""**********************************************************/
void io_putchar(char c)
{
	// Check if it is possible to transmit (TDFE flag)
	while(SCIF2.SCFSR.BIT.TDFE == 0){
		/* Wait */
	}

	/* Write the receiving data in TDR */
	SCIF2.SCFTDR.BYTE = c;

	/* Clear TDRE and TEND flag */
	SCIF2.SCFSR.WORD &= ~0x0060 ;

}

/* End of File */
