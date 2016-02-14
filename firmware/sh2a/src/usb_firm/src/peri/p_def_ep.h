/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Technology Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Technology Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* TECHNOLOGY CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
********************************************************************************
* Copyright (C) 2009. Renesas Technology Corp., All Rights Reserved.           *
********************************************************************************/

/*""FILE COMMENT""*********** Technical reference data *************************
 *
 *       System Name : SH7264 USB FUNCTION Sample Program
 *       File Name   : p_def_ep.h
 *       Abstract    : Define Peripheral Endpoint
 *       Version     : 1.00.00
 *       Device      : SH7264/SH7262
 *       Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
 *                   : C/C++ compiler package for the SuperH RISC engine family
 *                   :                             (Ver.9.02 Release00).
 *       OS          : None
 *       H/W Platform: M3A-HS64G50(CPU board)
 *
 *       History   : May.13.2009 ver.1.00.00
 *""FILE COMMENT END""**********************************************************/

/*
 ******************************************************************************
 * Endpoint Configuration Data Format
 ******************************************************************************
 *	LINE1: Pipe Window Select Register (0x64)
 *		CPU Access PIPE					: PIPE1 to PIPE9
 *		Select FIFO port				: C_FIFO_USE / D0_FIFO_USE / D1_FIFO_USE
 *                                      : D0_FIFO_DMA / D1_FIFO_DMA
 *	LINE2: Pipe Configuration Register (0x66)
 *		Transfer Type					: ISO / INT / BULK
 *		Double Buffer Mode				: DBLB / OFF		(PIPE1-5 only)
 *		Continuous Transmit/Receive Mode: CNTMD / OFF		(PIPE1-5 only)
 *		Transfer Direction				: DIR_P_IN / DIR_P_OUT
 *		Endpoint Number					: EP1 to EP15
 *	LINE3: Pipe Buffer Configuration Register (0x68)
 *		Buffer Size						: BUF_SIZE(x)		(PIPE1-5: x:64 to 2048)
 *										:					(PIPE6-9: x:64)
 *		Buffer Top Number				: 4 to 79
 * 										:		(PIPE6:4, PIPE7:5, PIPE8:6, PIPE9:7)
 *	LINE4: Pipe Maxpacket Size Register (0x6A)
 *		Max Packet Size					: 0 to 1024
 *	LINE5: Pipe Cycle Configuration Register (0x6C)
 *		ISO Buffer Flush Mode			: IFIS / OFF
 *		ISO Interval Value				: 0 to 7
 ******************************************************************************
 */

/*
 *	|--- Configuration 1
 *	|		|--- Interface 1-0-0
 *	|		|		|--- Endpoint 1-0-0-0
 */
/* Configuration 1 */
USB_U16  EPtbl_1[] = {

	/* Pipe Window Select Register (0x64) */
	PIPE6,
	/* Pipe Configuration Register (0x66) */
	INT | DIR_P_IN | EP6,
	/* Pipe Buffer Configuration Register (0x68) */
	BUF_SIZE(64) | 4,
	/* Pipe Maxpacket Size Register (0x6A) */
	64,
	/* Pipe Cycle Configuration Register (0x6C) */
	0,
	
	/* Pipe Window Select Register (0x64) */
	PIPE1 | D0_FIFO_USE,
	/* Pipe Configuration Register (0x66) */
	BULK |                    DIR_P_OUT | EP1,
	/* Pipe Buffer Configuration Register (0x68) */
	BUF_SIZE(512) | 8,
	/* Pipe Maxpacket Size Register (0x6A) */
//	512,
	64,
	/* Pipe Cycle Configuration Register (0x6C) */
	USB_OFF | 0,

	/* Pipe Window Select Register (0x64) */
	PIPE2 | D1_FIFO_USE,
	/* Pipe Configuration Register (0x66) */
	BULK |                    DIR_P_IN | EP2,
	/* Pipe Buffer Configuration Register (0x68) */
	BUF_SIZE(512) | 24,
	/* Pipe Maxpacket Size Register (0x6A) */
//	512,
	64,
	/* Pipe Cycle Configuration Register (0x6C) */
	USB_OFF | 0,

};

const USB_U16  EPtbl_2[] = { 0 };				/* Configuration 2 */
const USB_U16  EPtbl_3[] = { 0 };				/* Configuration 3 */
const USB_U16  EPtbl_4[] = { 0 };				/* Configuration 4 */
const USB_U16  EPtbl_5[] = { 0 };				/* Configuration 5 */

const USB_U16 *EndPntPtr[] = {
	EPtbl_1,
	EPtbl_2,
	EPtbl_3,
	EPtbl_4,
	EPtbl_5
};

/*===========================================================================*/
/* End of File */
