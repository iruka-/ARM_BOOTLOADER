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
 *       File Name   : c_usbdly.c
 *       Abstract    : USB user define software wait
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

#define DUMMY_ACCESS	(*(volatile unsigned long *)(0x20000000))
/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Delay 1msec
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_delay_1ms(void);
*----------------------------------------------------------------------------- 
* Description : 1msのソフトウェア待ちを行います。
*             : 
*----------------------------------------------------------------------------- 
* Argumen t   : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数はシステムに応じて変更してください。
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_delay_1ms(void)
{
	volatile int i;
	volatile unsigned long tmp;

	/*
	 * Wait 1ms (Please change for your MCU).
	 */
	for( i = 0; i < 14400; ++i ){
		tmp = DUMMY_ACCESS;
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Delay msec
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_delay_xms(USB_U32 msec);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定した時間ソフトウェア待ちを行います。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U32 msec ; Wait Time (msec)
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数はシステムに応じて変更してください。
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_delay_xms(USB_U32 msec)
{
	volatile unsigned short i;

	for( i = 0; i < msec; ++i ) {
		usb_delay_1ms();
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Delay 10micro-second
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_delay_10us(USB_U32 usec);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定した時間ソフトウェア待ちを行います。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U32 usec ; Wait Time(x 10usec)
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数はシステムに応じて変更してください。
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_delay_10us(USB_U32 usec)
{
	volatile int i;
	volatile unsigned long tmp;

	/*
	 * Wait 10us (Please change for your MCU)
	 */

	for( i = 0; i < 144 * usec ; ++i ){
		tmp = DUMMY_ACCESS;
	}
}

/* End of File */
