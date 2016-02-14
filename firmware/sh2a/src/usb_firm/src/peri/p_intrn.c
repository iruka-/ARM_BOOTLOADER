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
 *       File Name   : p_intrn.c
 *       Abstract    : Peripheral Pipe Interrupt
 *       Version     : 1.01.00
 *       Device      : SH7264/SH7262
 *       Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
 *                   : C/C++ compiler package for the SuperH RISC engine family
 *                   :                             (Ver.9.02 Release00).
 *       OS          : None
 *       H/W Platform: M3A-HS64G50(CPU board)
 *
 *       History   : May.13.2009 ver.1.00.00
 *                 : Sep.17.2009 ver.1.01.00
 *""FILE COMMENT END""**********************************************************/
#include "usb.h"

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : BRDY interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_BRDYInterrupt(USB_U16 status, USB_U16 intenb);
*----------------------------------------------------------------------------- 
* Description : BRDY割り込み処理を実行します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 status       ; BRDYSTS Register Value
*             : USB_U16 intenb       ; BRDYENB Register Value
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_BRDYInterrupt(USB_U16 status, USB_U16 intenb)
{
	volatile USB_U16	dumy_sts;

	if( (status & BITBRDY0) && (intenb & BITBRDY0) ) {
		USB.BRDYSTS.WORD = (USB_U16)~BITBRDY0;
		USB.CFIFOSEL.BIT.CURPIPE = PIPE0;
		switch( usb_read_buffer_c(PIPE0) ) {
		case	READING:						/* Continue of data read */
		case	READEND:						/* End of data read */
			usb_set_pid_buf(PIPE0);				/* PID = BUF */
			break;
		case	READSHRT:						/* End of data read */
			usb_disable_brdy_int(PIPE0);
			usb_set_pid_buf(PIPE0);				/* PID = BUF */
			break;
		case	READOVER:						/* FIFO access error */
			USB.CFIFOCTR.WORD = BITBCLR;		/* Buffer Clear */
			usb_disable_brdy_int(PIPE0);
			usb_set_pid_stall(PIPE0);			/* Req Error */
			break;
		case	FIFOERROR:						/* FIFO access error */
		default:
			usb_disable_brdy_int(PIPE0);
			usb_set_pid_stall(PIPE0);			/* Req Error */
			break;
		}
	} 
	else {
		usb_brdy_int(status, intenb);
	}
	/* 割り込み要因クリアのダミーリード3回 */
	dumy_sts = USB.BRDYSTS.WORD;
	dumy_sts = USB.BRDYSTS.WORD;
	dumy_sts = USB.BRDYSTS.WORD;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : NRDY interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_NRDYInterrupt(USB_U16 status, USB_U16 intenb);
*----------------------------------------------------------------------------- 
* Description : NRDY割り込み処理を実行します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 status       ; NRDYSTS Register Value
*             : USB_U16 intenb       ; NRDYENB Register Value
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_NRDYInterrupt(USB_U16 status, USB_U16 intenb)
{
	volatile USB_U16	dumy_sts;

	if( (status & BITNRDY0) && (intenb & BITNRDY0) ) {
		USB.NRDYSTS.WORD = (USB_U16)~BITNRDY0;
	} 
	else {
		usb_nrdy_int(status, intenb);
	}
	/* 割り込み要因クリアのダミーリード3回 */
	dumy_sts = USB.NRDYSTS.WORD;
	dumy_sts = USB.NRDYSTS.WORD;
	dumy_sts = USB.NRDYSTS.WORD;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : BEMP interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_BEMPInterrupt(USB_U16 status, USB_U16 intenb);
*----------------------------------------------------------------------------- 
* Description : BEMP割り込み処理を実行します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 status       ; BEMPSTS Register Value
*             : USB_U16 intenb       ; BEMPENB Register Value
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_BEMPInterrupt(USB_U16 status, USB_U16 intenb)
{
	volatile USB_U16	dumy_sts;

	if( (status & BITBEMP0) && (intenb & BITBEMP0) ) {
		USB.BEMPSTS.WORD = (USB_U16)~BITBEMP0;
		USB.CFIFOSEL.BIT.CURPIPE = PIPE0;
		usb_write_buffer_c(PIPE0);
	} 
	else {
		usb_bemp_int(status, intenb);
	}
	/* 割り込み要因クリアのダミーリード3回 */
	dumy_sts = USB.BEMPSTS.WORD;
	dumy_sts = USB.BEMPSTS.WORD;
	dumy_sts = USB.BEMPSTS.WORD;
}

/*===========================================================================*/
/* End of File */
