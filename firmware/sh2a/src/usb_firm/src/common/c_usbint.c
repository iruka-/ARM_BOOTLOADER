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
 *       File Name   : c_usbint.c
 *       Abstract    : USB interrupt
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
#include "usb.h"

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : USB interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_interrupt(void);
*----------------------------------------------------------------------------- 
* Description : USB割り込み処理を実行します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数をUSB割り込みハンドラに登録してください。
*             : 本関数実行後にCFIFOを割り込み処理前に設定されていたパイプに
*             : 設定する必要があります。
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_interrupt(void)
{
	USB_U16 savepipe1,savepipe2;
	USB_U16 buffer;

	savepipe1 = USB.CFIFOSEL.WORD;
	savepipe2 = USB.PIPESEL.WORD;
	P_USBInterrupt();

	/* Control transmission changes ISEL within interruption processing. */
	/* For this reason, write return of ISEL cannot be performed. */
	buffer = USB.CFIFOSEL.WORD;
	buffer &= (USB_U16)~(BITCURPIPE);
	buffer |= (USB_U16)(savepipe1 & BITCURPIPE);
	USB.CFIFOSEL.WORD = buffer;
	USB.PIPESEL.WORD = savepipe2;

}

/*===========================================================================*/
/* End of File */
