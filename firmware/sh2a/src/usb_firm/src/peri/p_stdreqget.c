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
 *       File Name   : p_stdreqget.c
 *       Abstract    : Peripheral Standard Device Get Requests
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
* Outline     : Get Status
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_GetStatus(USB_U16 type, USB_U16 value,
*             :                  USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : USB標準リクエスト(GET_STATUS)のリードデータステージを実行します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type		    ; Recipient (bmRequestType)
*             : USB_U16 value			; wValue
*             : USB_U16 index			; wIndex
*             : USB_U16 length		    ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_GetStatus(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	static USB_U8	tbl[2];
	USB_U16			ep;
	USB_U16			buffer;
	USB_U16			pipe;

	if( (value == 0) && (length == 2) ) {
		tbl[0] = 0;
		tbl[1] = 0;
		switch( type ) {
		case DEVICE:
			if( index == 0 ) {
				tbl[0] = P_GetCurrentPower(ConfigNum);	/* SelfPowered / BusPowered */
				if( RemoteWakeupFlag == USB_ON ){		/* Support Remote Wakeup ? */
					tbl[0] |= GS_REMOTEWAKEUP;
				}
				P_CtrlReadStart(2, tbl);
			} 
			else {
				usb_set_pid_stall(PIPE0);				/* Req Error */
			}
			break;
		case INTERFACE:
			if( P_IsConfigured() == USB_YES ) {
				if( index < P_GetInterfaceNum(ConfigNum) ) {
					P_CtrlReadStart(2, tbl);			/* Return 0 */
				} 
				else {
					usb_set_pid_stall(PIPE0);			/* Req Error */
				}
			} 
			else {
				usb_set_pid_stall(PIPE0);				/* Req Error */
			}
			break;
		case ENDPOINT:
			ep = (USB_U16)(index & EPNUMFIELD);			/* Endpoint Number */
			if( ep == 0 ) {								/* Endpoint 0 */
				if( USB.DCPCTR.BIT.PID == PID_STALL ) {
				   tbl[0] = GS_HALT;					/* Halt set */
				}
				P_CtrlReadStart(2, tbl);
			} 
			else if( ep <= MAX_EP_NO ) {				/* EP1 to max */
				if( P_IsConfigured() == USB_YES ) {
					pipe = P_EpToPipe(index);
					if( pipe == EP_ERROR ) {
						usb_set_pid_stall(PIPE0);
					} 
					else {
						buffer = usb_get_pid(pipe);
						if( (buffer & PID_STALL) == PID_STALL ) {
							tbl[0] = GS_HALT;			/* Halt set */
						}
						P_CtrlReadStart(2, tbl);
					}
				} 
				else {
					usb_set_pid_stall(PIPE0);
				}
			} 
			else {
				usb_set_pid_stall(PIPE0);
			}
			break;
		default:
			usb_set_pid_stall(PIPE0);
			break;
		}
	} 
	else {
		usb_set_pid_stall(PIPE0);
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Get Descriptor
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_GetDescriptor(USB_U16 type, USB_U16 value,
*             :                      USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : USB標準リクエスト(GET_DESCRIPTOR)のリードデータステージを実行します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type		        ; Recipient (bmRequestType)
*             : USB_U16 value			; wValue
*             : USB_U16 index			; wIndex
*             : USB_U16 length		    ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_GetDescriptor(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16		len, idx;

	if( type == DEVICE ) {
		idx = (USB_U16)(value & DT_INDEX);
		switch( GET_DT_TYPE(value) ) {
		case DT_DEVICE:									/* device Descriptor */
			if( (index == 0) && (idx == 0) ) {
				if( length < DeviceDescriptor[0] ) {
					P_CtrlReadStart((USB_U32)length, DeviceDescriptor);
				} 
				else {
					P_CtrlReadStart((USB_U32)DeviceDescriptor[0], DeviceDescriptor);
				}
			} 
			else {
				usb_set_pid_stall(PIPE0);				/* Req Error */
			}
			break;
		case DT_CONFIGURATION:							/* Configuration Descriptor */
			if( index == 0 ) {
				len  = (USB_U16)(*(ConPtr[idx]+2));
				len |= (USB_U16)((USB_U16)(*(ConPtr[idx]+3)) << 8);
				if( length < len ) {					/* Descriptor > wLength */
					P_CtrlReadStart((USB_U32)length, ConPtr[idx]);
				} 
				else {
					P_CtrlReadStart((USB_U32)len, ConPtr[idx]);
				}
			} 
			else {
				usb_set_pid_stall(PIPE0);				/* Req Error */
			}
			break;
		case DT_STRING:									/* String Descriptor */
			len = (USB_U16)(*(StrPtr[idx]+0));
			if( length < len ) {
				P_CtrlReadStart((USB_U32)length, StrPtr[idx]);
			} 
			else {
				P_CtrlReadStart((USB_U32)len, StrPtr[idx]);
			}
			break;
		case DT_INTERFACE:								/* Interface Descriptor */
			usb_set_pid_stall(PIPE0);
			break;
		case DT_ENDPOINT:								/* Endpoint Descriptor */
			usb_set_pid_stall(PIPE0);
			break;
		case DT_DEVICE_QUALIFIER:
			if( (usb_is_hispeed_enable() == USB_YES) && 
				(index == 0) && (idx == 0) ) {
				if( length < QualifierDescriptor[0] ) {
					P_CtrlReadStart((USB_U32)length, QualifierDescriptor);
				} 
				else {
					P_CtrlReadStart((USB_U32)QualifierDescriptor[0], 
												QualifierDescriptor);
				}
			} 
			else {
				usb_set_pid_stall(PIPE0);				/* Req Error */
			}
			break;
		case DT_OTHER_SPEED_CONFIGURATION:
			if( (usb_is_hispeed_enable() == USB_YES) &&
				 index == 0 ) {
				len  = (USB_U16)(*(ConPtr_Other[idx]+2));
				len |= (USB_U16)((USB_U16)(*(ConPtr_Other[idx]+3)) << 8);
				if( length < len ) {					/* Descriptor > wLength */
					P_CtrlReadStart((USB_U32)length, ConPtr_Other[idx]);
				} 
				else {
					P_CtrlReadStart((USB_U32)len, ConPtr_Other[idx]);
				}
			} 
			else {
				usb_set_pid_stall(PIPE0);				/* Req Error */
			}
			break;
		case DT_INTERFACE_POWER:
			usb_set_pid_stall(PIPE0);					/* Not Support */
			break;
		default:
			usb_set_pid_stall(PIPE0);
			break;
		}
	} 
	else {
		usb_set_pid_stall(PIPE0);
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Get Configuration
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_GetConfiguration(USB_U16 type,  USB_U16 value,
*             :                         USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : USB標準リクエスト(GET_CONFIGURATION)のリードデータステージを実行します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type		    ; Recipient (bmRequestType)
*             : USB_U16 value			; wValue
*             : USB_U16 index			; wIndex
*             : USB_U16 length		    ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_GetConfiguration(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	static USB_U8 tbl[1];

	if( (type == DEVICE) && (value == 0) && (index == 0) && (length == 1) ) {
		tbl[0] = (USB_U8)ConfigNum;
		P_CtrlReadStart(1, tbl);
	} 
	else {
		usb_set_pid_stall(PIPE0);
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Get Interface
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_GetInterface(USB_U16 type,  USB_U16 value,
*             :                     USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : USB標準リクエスト(GET_INTERFACE)のリードデータステージを実行します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type		        ; Recipient (bmRequestType)
*             : USB_U16 value			; wValue
*             : USB_U16 index			; wIndex
*             : USB_U16 length		    ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_GetInterface(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	static USB_U8	tbl[1];

	if( (type == INTERFACE) && (value == 0) && (length == 1) ) {
		if( index < ALT_NO ) {
			tbl[0] = (USB_U8)Alternate[index];
			P_CtrlReadStart(1, tbl);						/* Start control read */
		} 
		else {
			usb_set_pid_stall(PIPE0);						/* Req Error */
		}
	} 
	else {
		usb_set_pid_stall(PIPE0);							/* Req Error */
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Get currently power
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U8 P_GetCurrentPower(void);
*----------------------------------------------------------------------------- 
* Description : コンフィギュレーション・ディスクリプタを参照し、
*             : バスパワード/セルフパワードのどちらのデバイスであるかを
*             : 戻り値に返します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: GS_SELFPOWERD : Device is currently self-powered
*             : GS_BUSPOWERD  : Device is currently bus-powered
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U8 P_GetCurrentPower(USB_U16 ConfigNum)
{
	/*
	 * Please answer the currently power of your system.
	 */

    USB_U8	tmp, current;
    USB_U16	conf;

	conf = ConfigNum;
	if( conf < 1 ){
		conf = 1;									/* Address state */
	}

	/* Standard Configuration Descriptor */
	tmp = *(ConPtr[(conf-1)]+7);
	if( tmp & CF_SELF ){
		current = GS_SELFPOWERD;
	}
	else{
		current = GS_BUSPOWERD;
	}

	/* check Currently Powered */

	return current;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Reserved (Idle/Setup Stage);
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Resrv_0(USB_U16 type, USB_U16 value,
*             :                USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : 標準コントロール転送/セットアップステージ処理を行います。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type		    ; Recipient (bmRequestType)
*             : USB_U16 value			; wValue
*             : USB_U16 index			; wIndex
*             : USB_U16 length		    ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USBサンプルドライバにてサポートしている標準リクエストでは
*             : セットアップステージに何もしません。
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_Resrv_0(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;
	/*==== Reserved ====*/
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Reserved (Control Read Data Stage, Control Write Data Stage,
*             :           Control Write No Data Status Stage)
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Resrv_123(USB_U16 type, USB_U16 value,
*             :                  USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : USBサンプルドライバにてサポートしていない標準コントロール転送/データステージに
*             : 本関数が実行されます。
*             : USBホストに対してSTALL応答します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type		    ; Recipient (bmRequestType)
*             : USB_U16 value			; wValue
*             : USB_U16 index			; wIndex
*             : USB_U16 length		    ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_Resrv_123(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;

	/*==== Reserved ====*/
	usb_set_pid_stall(PIPE0);
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Reserved (Control Read Status Stage).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Resrv_4(USB_U16 type, USB_U16 value,
*                             USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : 標準コントロール転送/コントロールリード用ステータスステージ処理を行います。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type		    ; Recipient (bmRequestType)
*             : USB_U16 value			; wValue
*             : USB_U16 index			; wIndex
*             : USB_U16 length		    ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USBサンプルドライバにてサポートしている標準リクエストでは
*             : ステータスステージに何もしません。
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_Resrv_4(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;
	/*==== Reserved ====*/
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Reserved (Control Write Status Stage).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Resrv_5(USB_U16 type, USB_U16 value,
*             :                USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : 標準コントロール転送/コントロールライト用ステータスステージ処理を行います。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type		    ; Recipient (bmRequestType)
*             : USB_U16 value			; wValue
*             : USB_U16 index			; wIndex
*             : USB_U16 length		    ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USBサンプルドライバにてサポートしている標準リクエストでは
*             : ステータスステージに何もしません。
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_Resrv_5(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;
	/*==== Reserved ====*/
}

/*===========================================================================*/
/* End of File */
