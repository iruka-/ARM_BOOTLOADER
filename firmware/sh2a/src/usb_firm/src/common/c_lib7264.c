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
 *       System Name : SH7264 USB Sample Program
 *       File Name   : c_lib7264.c
 *       Abstract    : sh7264 usb register routine
 *       Version     : 1.01.00
 *       Device      : SH7264/SH7262
 *       Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
 *                   : C/C++ compiler package for the SuperH RISC engine family
 *                   :                             (Ver.9.02 Release00).
 *       OS          : None
 *       H/W Platform: M3A-HS64G50(CPU board)
 *
 *       History   : May.13.2009 ver.1.00.00
 *                 : Nov.10,2009 Ver.1.03.00
 *                 :    Changed bit name
 *                 :     USB.USBACSWR1.PREENA -> USB.USBACSWR1.UACS25
 *""FILE COMMENT END""**********************************************************/
#include "usb.h"
#include "iodefine.h"

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : set interrupt level
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_set_int_level(void);
*----------------------------------------------------------------------------- 
* Description : USBモジュール割り込みレベルを設定します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_set_int_level(USB_U8 level)
{
	INTC.IPR10.BIT._USB = level;	/* Set USB interrupt level */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : reset USB Module
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_reset_module(void);
*----------------------------------------------------------------------------- 
* Description : USBモジュールを初期化します。
*             : USBモジュールAC特性切り替えレジスタ(USBACSWR1)設定および
*             : USBモジュールへのクロック供給を有効にします。
*             : また、USBバスウェイトレジスタを設定します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_reset_module(void)
{
	/* Setting up ATEST:UACS25	*/
	USB.USBACSWR1.BIT.UACS25 = 1;

	USB.SYSCFG.WORD 	= BITSCKE;
	USB.BUSWAIT.WORD 	= ( USB_U16 )( PBUS_WAIT & BITBWAIT );
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Get Buf size
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_get_buf_size(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプのバッファサイズまたは当該パイプを使用している
*             : USBデバイスのマックスパケットサイズを取得します。
*             : 引数にてPIPE0が指定された場合は、当該パイプを使用している
*             : USBデバイスのマックスパケットサイズを取得します。
*             : 引数にてPIPE0以外が指定された場合は、当該パイプが連続転送モード
*             : の場合、当該パイプに割り当てられているバッファサイズを、
*             : 非連続転送モードの場合、マックスパケットサイズを取得します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 ; pipe Number
*----------------------------------------------------------------------------- 
* Return Value: マックスパケットサイズまたはバッファサイズ
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_get_buf_size(USB_U16 pipe)
{
	USB_U16 size, bufsize;

	if( pipe == PIPE0 ) {
		size = USB.DCPMAXP.BIT.MXPS;				/* Max Packet Size */
	} 
	else {
		USB.PIPESEL.BIT.PIPESEL = pipe;				/* pipe select */
		if( USB.PIPECFG.BIT.CNTMD == 1 ) {			/* read CNTMD */
			bufsize = USB.PIPEBUF.BIT.BUFSIZE;
			size = (USB_U16)((bufsize + 1) * PIPExBUF);	/* Buffer Size */
		} 
		else {
			size = USB.PIPEMAXP.BIT.MXPS;			/* Max Packet Size */
		}
	}
	return size;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Get max packet size
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_get_mxps(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にして指定したパイプを使用している
*             : USBデバイスのマックスパケットサイズを取得します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: Max Packet Size
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_get_mxps(USB_U16 pipe)
{
	USB_U16 size;

	if( pipe == PIPE0 ) {
		size = USB.DCPMAXP.BIT.MXPS;			/* Max Packet Size */
	} 
	else {
		USB.PIPESEL.BIT.PIPESEL = pipe;			/* pipe select */
		size = USB.PIPEMAXP.BIT.MXPS;			/* Max Packet Size */
	}
	return size;
}

/*===========================================================================*/
/* End of File */
