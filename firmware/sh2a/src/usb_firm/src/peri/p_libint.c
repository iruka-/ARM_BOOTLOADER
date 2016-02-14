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
 *       File Name   : p_libint.c
 *       Abstract    : Peripheral USB Interrupt Control
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
* Outline     : Enable USB Module interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_EnableINTModule(void);
*----------------------------------------------------------------------------- 
* Description : USB割り込みを許可します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_EnableINTModule(void)
{
	USB_U16	buf;
	
	buf = USB.INTENB0.WORD;
	buf |= (BITVBSE | BITDVSE | BITCTRE | BITBEMP | BITNRDY | BITBRDY );
	USB.INTENB0.WORD = buf;

	usb_enable_bemp_int(PIPE0);
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Check state ( Configured ).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 P_IsConfigured(void);
*----------------------------------------------------------------------------- 
* Description : USBデバイスがコンフィガード状態かどうかを確認し、結果を戻り値として返します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: USB_YES : Configured
*             : USB_NO  : not Configured
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 P_IsConfigured(void)
{
	USB_U16		dvst;

	dvst = P_GetDeviceState();			/* check device state from DVSQ		*/
	if( dvst == DVST_CONFIGURED ){
		return USB_YES;
	}
	return USB_NO;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : get state ( Configured ).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 P_GetDeviceState(void);
*----------------------------------------------------------------------------- 
* Description : USBデバイスの状態を戻り値として返します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: Device States
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 P_GetDeviceState(void)
{
	USB_U16 dvsq,dvst;
	
	dvsq = USB.INTSTS0.WORD;
	switch( dvsq & BITDVSQ ) {
	case DS_POWR:						/* Power state			*/	/* power-on		*/
		dvst = DVST_POWERED;
		break;
	case DS_DFLT:						/* Default state  		*/	/* bus-reset	*/
		dvst = DVST_DEFAULT;
		break;
	case DS_ADDS:						/* Address state  		*/
		dvst = DVST_ADDRESS;
		break;
	case DS_CNFG:						/* Configured state  	*/
		dvst = DVST_CONFIGURED;
		break;
	case DS_SPD_POWR:					/* Power	  Suspend state */
	case DS_SPD_DFLT:					/* Default	  Suspend state */
	case DS_SPD_ADDR:					/* Address	  Suspend state */
	case DS_SPD_CNFG:					/* Configured Suspend state */
		dvst = DVST_SUSPEND;
		break;
	default:							/* error */
		dvst = DVST_SUSPEND;
		break;
	}
	return dvst;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Endpoint number to pipe number
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 P_EpToPipe(USB_U16 ep);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定されたエンドポイントが割り当てられているパイプを戻り値として返します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 ep       ; Direction + Endpoint Number
*----------------------------------------------------------------------------- 
* Return Value: EP_ERROR         : Error
*             : Others           : Pipe Number
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 P_EpToPipe(USB_U16 ep)
{
	USB_U16 pipe;

	for( pipe = 1; pipe <= MAX_PIPE_NO; pipe++ ) {
		if( (PipeTbl[pipe] & 0x00ff) == ep ){
			return pipe;
		}
	}
	return EP_ERROR;
}

/*===========================================================================*/
/* End of File */
