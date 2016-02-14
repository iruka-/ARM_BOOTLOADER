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
 *       File Name   : cfg_usb_sh7264.h
 *       Abstract    : Define User Option
 *       Version     : 1.01.00
 *       Device      : SH7264/SH7262
 *       Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
 *                   : C/C++ compiler package for the SuperH RISC engine family
 *                   :                             (Ver.9.02 Release00).
 *       OS          : None
 *       H/W Platform: M3A-HS64G50(CPU board)
 *
 *       History   : May.13.2009 ver.1.00.00
 *                 : Sep.17.2009 ver.1.01.00 Compilation option CLK_18MHz addition 
 *""FILE COMMENT END""**********************************************************/
#ifndef _CFG_USB_SH7264_H_
#define _CFG_USB_SH7264_H_

/* USB register base address & pointer type */
#define	__SH7264_USB__

/* terminal out mode */
#define	PRINT_USE				0
#define	PRINT_NOT_USE			1
//#define	PRINT_MODE				PRINT_USE
#define	PRINT_MODE				PRINT_NOT_USE
#define	 CLK_18MHz 1

#if	(CLK_18MHz == 0)
	/*B Clock = 48MHz				*/
	#define PBUS_WAIT 	 			1			/* CPU Bus Access Wait Specification */
#else
	/*B Clock = 72MHz				*/
	#define PBUS_WAIT 	 			3			/* CPU Bus Access Wait Specification */
#endif

#define	USBLVL					9			/* interrupt level USB */
#define	IRQLVL					9			/* interrupt level IRQ */
#define	DMACLVL					9			/* interrupt level DMAC */

/* User Data Buffer Size */
#define	P1_BUF_SIZE				512
#define	P2_BUF_SIZE				512
#define	P3_BUF_SIZE				512
#define	P4_BUF_SIZE				512
#define	P5_BUF_SIZE				512
#define	P6_BUF_SIZE				512
#define	P7_BUF_SIZE				512
#define	P8_BUF_SIZE				512
#define	P9_BUF_SIZE				512

#define	P1_DATA_SIZE			512
#define	P2_DATA_SIZE			512
#define	P3_DATA_SIZE			512
#define	P4_DATA_SIZE			512
#define	P5_DATA_SIZE			512
#define	P6_DATA_SIZE			512
#define	P7_DATA_SIZE			512
#define	P8_DATA_SIZE			512
#define	P9_DATA_SIZE			512

/* **** macro define (PERIPHERAL) **** */
#if	0
#define	VENDOR_ID				0x045B				/* Renesas Technology */
#define	PRODUCT_ID				0x0020				/* Product ID */
#else
#define	VENDOR_ID				0x26FF
#define	PRODUCT_ID				0x4912
#endif

#define	PIPEERR					3					/* Max pipe error count */

/* remote wakeup mode */
#define	RWUP_NOT_USE			0x00
#define	RWUP_USE				0x20				/* bmAttributes D5 bit */
#define	RWUP_MODE				RWUP_NOT_USE
//#define	RWUP_MODE				RWUP_USE

#endif  /*  _CFG_USB_SH7264_H_ */

/* End of File */
