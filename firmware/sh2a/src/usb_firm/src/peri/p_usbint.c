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
 *       File Name   : p_usbint.c
 *       Abstract    : Peripheral USB Interrupt
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
* Outline     : USB interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_USBInterrupt(void);
*----------------------------------------------------------------------------- 
* Description : USB割り込み処理を実行します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_USBInterrupt(void)
{
	USB_U16	int_sts0, int_sts1, int_sts2, int_sts3;		/* interrupt status */
	USB_U16	int_enb0, int_enb2, int_enb3, int_enb4;		/* interrupt enable */
	volatile USB_U16	dumy_sts;

	int_sts0 = USB.INTSTS0.WORD;
	if( !(int_sts0 & (BITVBINT|BITRESM|BITSOFR|BITDVST|
						BITCTRT|BITBEMP|BITNRDY|BITBRDY)) ) {
		return;
	}

	int_sts1 = USB.BRDYSTS.WORD;
	int_sts2 = USB.NRDYSTS.WORD;
	int_sts3 = USB.BEMPSTS.WORD;
	int_enb0 = USB.INTENB0.WORD;
	int_enb2 = USB.BRDYENB.WORD;
	int_enb3 = USB.NRDYENB.WORD;
	int_enb4 = USB.BEMPENB.WORD;

	if( (int_sts0 & BITRESM) && (int_enb0 & BITRSME) ) {
		USB.INTSTS0.WORD = (USB_U16)~BITRESM;	/* Status Clear */
		USB.INTENB0.BIT.RSME = 0;				/* RESM Interrupt Disable */
		P_USBResumeCB();
	}
	else if( (int_sts0 & BITVBINT) && (int_enb0 & BITVBSE) ) {
		USB.INTSTS0.WORD = (USB_U16)~BITVBINT;	/* Status Clear */
		if( P_CheckVBUStaus() == USB_ON ) {
			P_USBAttachCB();							/* USB Attach */
		} 
		else {
			P_USBDetachCB();							/* USB Detach */
		}
	}
	else if( (int_sts0 & BITSOFR) && (int_enb0 & BITSOFE) ) {
		USB.INTSTS0.WORD = (USB_U16)~BITSOFR;
		/* User program */
	}
	else if( (int_sts0 & BITDVST) && (int_enb0 & BITDVSE) ) {
		USB.INTSTS0.WORD = (USB_U16)~BITDVST;
		switch( int_sts0 & BITDVSQ ) {
		case DS_POWR:								/* Power state	*/
			break;
		case DS_DFLT:								/* Default state  */
			P_USBBusResetCB();
			break;
		case DS_ADDS:								/* Address state  */
			break;
		case DS_CNFG:								/* Configured state  */
			break;
		case DS_SPD_POWR:							/* Power	  Suspend state */
		case DS_SPD_DFLT:							/* Default	  Suspend state */
		case DS_SPD_ADDR:							/* Address	  Suspend state */
		case DS_SPD_CNFG:							/* Configured Suspend state */
			P_USBSuspendCB();
			break;
		default:									/* error */
			break;
		}
	}

	/* Processing PIPE0 data */
	else if( (int_sts0 & BITBEMP) && (int_enb0 & BITBEMP) && 
						((int_sts3 & int_enb4) & BITBEMP0) ) {
		P_BEMPInterrupt(int_sts3, int_enb4);
	}
	else if( (int_sts0 & BITBRDY) && (int_enb0 & BITBRDY) && 
						((int_sts1 & int_enb2) & BITBRDY0) ) {
		P_BRDYInterrupt(int_sts1, int_enb2);
	}
	else if( (int_sts0 & BITNRDY) && (int_enb0 & BITNRDY) && 
						((int_sts2 & int_enb3) & BITNRDY0) ) {
		P_NRDYInterrupt(int_sts2, int_enb3);
	}

	else if( (int_sts0 & BITCTRT) && (int_enb0 & BITCTRE) ) {
		int_sts0 = USB.INTSTS0.WORD;					/* CTSQ read */
		USB.INTSTS0.WORD = (USB_U16)~BITCTRT;
		switch( int_sts0 & BITCTSQ ) {
		case CS_IDST:								/* idle or setup stage */
			if( TestModeFlag == USB_YES ) {
				/*
				 * Test Mode
				 */
				P_USBTestModeCB();
			}
			switch( ReqTypeType ) {
			case STANDARD:							/* standard request */
				(*pbRequest0[ReqRequest])(ReqTypeRecip, ReqValue, ReqIndex, ReqLength);
				break;
			case CLASS:								/* class request */
				P_Class0(ReqType, ReqRequest, ReqValue, ReqIndex, ReqLength);
				break;
			case VENDOR:							/* user request */
				P_Vendor0(ReqType, ReqRequest, ReqValue, ReqIndex, ReqLength);
				break;
			default:								/* request error */
				break;
			}
			break;
		case CS_RDDS:								/* control read data stage */
			P_SaveRequest();						/* save request register */
			switch( ReqTypeType ) {
			case STANDARD:							/* standard request */
				(*pbRequest1[ReqRequest])(ReqTypeRecip, ReqValue, ReqIndex, ReqLength);
				break;
			case CLASS:								/* class request */
				P_Class1(ReqType, ReqRequest, ReqValue, ReqIndex, ReqLength);
				break;
			case VENDOR:							/* user request */
				P_Vendor1(ReqType, ReqRequest, ReqValue, ReqIndex, ReqLength);
				break;
			default:								/* request error */
				break;
			}
			break;
		case CS_WRDS:							/* control write data stage */
			P_SaveRequest();					/* save request register */
			switch( ReqTypeType ) {
			case STANDARD:						/* standard request */
				(*pbRequest2[ReqRequest])(ReqTypeRecip, ReqValue, ReqIndex, ReqLength);
				break;
			case CLASS:							/* calss request */
				P_Class2(ReqType, ReqRequest, ReqValue, ReqIndex, ReqLength);
				break;
			case VENDOR:						/* user request */
				P_Vendor2(ReqType, ReqRequest, ReqValue, ReqIndex, ReqLength);
				break;
			default:							/* request error */
				break;
			}
			break;
		case CS_WRND:							/* control write nodata status stage */
			P_SaveRequest();					/* save request register */
			switch( ReqTypeType ) {
			case STANDARD:						/* standard request */
				(*pbRequest3[ReqRequest])(ReqTypeRecip, ReqValue, ReqIndex, ReqLength);
				break;
			case CLASS:							/* class request */
				P_Class3(ReqType, ReqRequest, ReqValue, ReqIndex, ReqLength);
				break;
			case VENDOR:						/* user request */
				P_Vendor3(ReqType, ReqRequest, ReqValue, ReqIndex, ReqLength);
				break;
			default:							/* request error */
				break;
			}
			USB.DCPCTR.BIT.CCPL = 1;
			break;
		case CS_RDSS:							/* control read status stage */
			switch( ReqTypeType ) {
			case STANDARD:						/* standard request */
				(*pbRequest4[ReqRequest])(ReqTypeRecip, ReqValue, ReqIndex, ReqLength);
				break;
			case CLASS:							/* class request */
				P_Class4(ReqType, ReqRequest, ReqValue, ReqIndex, ReqLength);
				break;
			case VENDOR:						/* user request */
				P_Vendor4(ReqType, ReqRequest, ReqValue, ReqIndex, ReqLength);
				break;
			default:							/* request error */
				break;
			}
			USB.DCPCTR.BIT.CCPL = 1;
			break;
		case CS_WRSS:							/* control write status stage */
			switch( ReqTypeType ) {
			case STANDARD:						/* standard request */
				(*pbRequest5[ReqRequest])(ReqTypeRecip, ReqValue, ReqIndex, ReqLength);
				break;
			case CLASS:							/* class request */
				P_Class5(ReqType, ReqRequest, ReqValue, ReqIndex, ReqLength);
				break;
			case VENDOR:						/* user request */
				P_Vendor5(ReqType, ReqRequest, ReqValue, ReqIndex, ReqLength);
				break;
			default:							/* request error */
				break;
			}
			USB.DCPCTR.BIT.CCPL = 1;
			break;
		case CS_SQER:							/* control sequence error */
			usb_set_pid_stall(PIPE0);
			break;
		default:								/* illegal */
			usb_set_pid_stall(PIPE0);
			break;
		}
	}

	/* Processing whithout PIPE0 data */
	else if( (int_sts0 & BITBEMP) && (int_enb0 & BITBEMP) && (int_sts3 & int_enb4) ) {
		P_BEMPInterrupt(int_sts3, int_enb4);
	}
	else if( (int_sts0 & BITBRDY) && (int_enb0 & BITBRDY) && (int_sts1 & int_enb2) ) {
		P_BRDYInterrupt(int_sts1, int_enb2);
	}
	else if( (int_sts0 & BITNRDY) && (int_enb0 & BITNRDY) && (int_sts2 & int_enb3) ) {
		P_NRDYInterrupt(int_sts2, int_enb3);
	}

	/* 割り込み要因クリアのダミーリード3回 */
	dumy_sts = USB.INTSTS0.WORD;
	dumy_sts = USB.INTSTS0.WORD;
	dumy_sts = USB.INTSTS0.WORD;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : save request register
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_SaveRequest(void);
*----------------------------------------------------------------------------- 
* Description : USBリクエスト情報を変数に保持します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_SaveRequest(void)
{
	USB_U16 buf;

	USB.INTSTS0.WORD 	= (USB_U16)~BITVALID;
	buf 				= USB.USBREQ.WORD;
	ReqType      		= (USB_U16)(buf & bmRequestType);
	ReqTypeType  		= (USB_U16)(buf & bmRequestTypeType);
	ReqTypeRecip 		= (USB_U16)(buf & bmRequestTypeRecip);
	ReqRequest   		= (USB_U16)((buf & bRequest) >> 8);
	ReqValue 			= USB.USBVAL;
	ReqIndex 			= USB.USBINDX;
	ReqLength 			= USB.USBLENG;
}

/*===========================================================================*/
/* End of File */
