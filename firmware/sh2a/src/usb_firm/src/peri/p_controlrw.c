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
 *       File Name   : p_controlrw.c
 *       Abstract    : Peripheral Control Read/Write
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
* Outline     : Control read start
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 P_CtrlReadStart(USB_U32 size, USB_U8 *data);
*----------------------------------------------------------------------------- 
* Description : USBコントロールリード転送を実行します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 size     ; Data Size
*             : USB_U8  *data    ; Data Address
*----------------------------------------------------------------------------- 
* Return Value: WRITEEND         ; End of data write
*             : WRITESHRT        ; End of short data write
*             : WRITING          ; Continue of data write
*             : FIFOERROR        ; FIFO access error
*----------------------------------------------------------------------------- 
* Notice      : USBホスト←USBデバイス
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 P_CtrlReadStart(USB_U32 size, USB_U8 *data)
{
	USB_U16 status,mbw;

	usb_set_pid_nak(PIPE0);									/* Set NAK */

	USBDataCount[PIPE0] = size;
	USBDataPointer[PIPE0] = data;

	mbw = usb_get_mbw( USBDataCount[PIPE0], (USB_U32)USBDataPointer[PIPE0]);
	usb_set_curpipe(PIPE0,CUSE,BITISEL,mbw);
	USB.CFIFOCTR.WORD = BITBCLR;			/* Buffer Clear */

	status	= usb_write_buffer_c(PIPE0);

	/* Peripheral Control sequence */
	switch( status ){
	case	WRITESHRT:						/* End of data write */
	case	WRITEEND:						/* End of data write (not null) */
	case	WRITING:						/* Continue of data write */
		usb_enable_bemp_int(PIPE0);			/* Enable Empty Interrupt */
		usb_set_pid_buf(PIPE0);				/* Set BUF */
		break;
	case	FIFOERROR:						/* FIFO access error */
		break;
	default:
		break;
	}
	return	status;							/* End or Err or Continue */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Control write start
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_CtrlWriteStart(USB_U32 size, USB_U8 *data);
*----------------------------------------------------------------------------- 
* Description : USBコントロールライト転送を実行します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 size         ; Data Size
*             : USB_U8  *data        ; Data Address
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USBホスト→USBデバイス
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_CtrlWriteStart(USB_U32 size, USB_U8 *data)
{
	USB_U16	mbw;

	usb_set_pid_nak(PIPE0);									/* Set NAK */

	USBDataCount[PIPE0] = size;
	USBDataPointer[PIPE0] = data;

	mbw = usb_get_mbw( USBDataCount[PIPE0], (USB_U32)USBDataPointer[PIPE0]);
	usb_set_curpipe(PIPE0,CUSE,BITISEL,mbw);
	USB.CFIFOCTR.WORD = BITBCLR;			/* Buffer Clear */

	usb_enable_brdy_int(PIPE0);
	usb_set_pid_buf(PIPE0);
}

/*===========================================================================*/
/* End of File */
