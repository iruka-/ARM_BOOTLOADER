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
 *       File Name   : p_stdreqset.c
 *       Abstract    : Peripheral Standard Device Set Requests
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

/* ==== prototype ==== */
static void clear_pipe_tbl(void);
static void clear_ep_table_index(void);

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Clear Feature
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_ClearFeature(USB_U16 type, USB_U16 value,
*             :                     USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : USB標準リクエスト(CLEAR_FEATURE)のノーデータ用ステータスステージを実行します。
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
void P_ClearFeature(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 pipe, ep;

	if( length == 0 ) {
		switch( type ) {
		case DEVICE:
			if( value == DEVICE_REMOTE_WAKEUP && index == 0 ) {
				if( P_CheckRemoteWakeup() == USB_ON ) {
					RemoteWakeupFlag = USB_OFF;
					usb_set_pid_buf(PIPE0);					/* OK */
				} 
				else {
					/* Not Support RemoteWakeup */
					usb_set_pid_stall(PIPE0);				/* Req Error */
				}
			} 
			else {
				usb_set_pid_stall(PIPE0);					/* Not Spec */
			}
			break;
		case INTERFACE:
			usb_set_pid_stall(PIPE0);						/* Req Error */
			break;
		case ENDPOINT:
			ep = (USB_U16)(index & EPNUMFIELD);				/* Endpoint Number */
			if( value == ENDPOINT_HALT ) {
				if( ep == 0 ) {								/* EP0 */
					usb_set_pid_buf(PIPE0);					/* Clear STALL & Set BUF */
				} 
				else if( ep <= MAX_EP_NO ) {				/* EP1 to max */
					pipe = P_EpToPipe(index);
					if( pipe == EP_ERROR ) {
						usb_set_pid_stall(PIPE0);			/* Req Error */
					} 
					else {
						usb_set_pid_nak(pipe);				/* Clear STALL & Set NAK */
						usb_set_sqclr(pipe);				/* SQCLR=1 */
						usb_clear_pid_stall(pipe);
						usb_set_pid_buf(pipe);				/* Set BUF */
						usb_set_pid_buf(PIPE0);
					}
				} 
				else {
					usb_set_pid_stall(PIPE0);				/* Req Error */
				}
			} 
			else {
				usb_set_pid_stall(PIPE0);					/* Req Error */
			}
			break;
		default:
			usb_set_pid_stall(PIPE0);
			break;
		}
	} 
	else {
		usb_set_pid_stall(PIPE0);							/* Not Spec */
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Set Feature
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_SetFeature(USB_U16 type, USB_U16 value, 
*             :                   USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : USB標準リクエスト(SET_FEATURE)のノーデータ用ステータスステージを実行します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type		    ; Recipient (bmRequestType)
*             : USB_U16 value			; wValue
*             : USB_U16 index			; wIndex
*             : USB_U16 length		    ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice       : 
*            : 
*""FUNC COMMENT END""*********************************************************/
void P_SetFeature(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16		pipe, ep;

	if( length == 0 ) {
		switch( type ) {
		case DEVICE:
			switch( value ) {
			case DEVICE_REMOTE_WAKEUP:
				if( index == 0 ) {
					if( P_CheckRemoteWakeup() == USB_ON ) {
						RemoteWakeupFlag = USB_ON;
						usb_set_pid_buf(PIPE0);				/* OK */
					} 
					else {
						/* Not Support RemoteWakeup */
						usb_set_pid_stall(PIPE0);			/* Req Error */
					}
				} 
				else {
					usb_set_pid_stall(PIPE0);				/* Not Spec */
				}
				break;
			case TEST_MODE:
				if( usb_is_hispeed() == HI_SPEED ) {
					if( index < TEST_Reserved || TEST_VSTModes <= index ) {
						TestModeFlag = USB_YES;
						TestModeSelectors = index;
						usb_set_pid_buf(PIPE0);				/* OK */
					} 
					else {
						usb_set_pid_stall(PIPE0);			/* Not Spec */
					}
				} 
				else {
					usb_set_pid_stall(PIPE0);				/* Not Spec */
				}
				break;
			default:
				usb_set_pid_stall(PIPE0);					/* Not Spec */
				break;
			}
			break;
		case INTERFACE:
			usb_set_pid_stall(PIPE0);
			break;
		case ENDPOINT:
			ep = (USB_U16)(index & EPNUMFIELD);				/* Endpoint Number */
			if( value == ENDPOINT_HALT ) {
				if( ep == 0 ) {								/* EP0 */
					usb_set_pid_buf(PIPE0);
				} 
				else if( ep <= MAX_EP_NO ) {				/* EP1 to max */
					pipe = P_EpToPipe(index);
					if( pipe == EP_ERROR ) {
						usb_set_pid_stall(PIPE0);			/* Req Error */
					} 
					else {
						usb_set_pid_stall(pipe);			/* Set STALL */
						usb_set_pid_buf(PIPE0);				/* OK */
					}
				} 
				else {
					usb_set_pid_stall(PIPE0);				/* Req Error */
				}
			} 
			else {
				usb_set_pid_stall(PIPE0);					/* Not Spec */
			}
			break;

		default:
			usb_set_pid_stall(PIPE0);						/* Req Error */
			break;
		}
	} 
	else {
		usb_set_pid_stall(PIPE0);							/* Req Error */
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Set Address
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_SetAddress(USB_U16 type, USB_U16 value, 
*             :                   USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : USB標準リクエスト(SET_ADDRESS)のノーデータ用ステータスステージを実行します。
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
void P_SetAddress(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	if( type == DEVICE ) {
		if( (index == 0) && (length == 0) ) {
			if( value <= 127 ) {
				usb_set_pid_buf(PIPE0);						/* OK */
			} 
			else {
				usb_set_pid_stall(PIPE0);					/* Not Spec */
			}
		} 
		else {
			usb_set_pid_stall(PIPE0);						/* Not Spec */
		}
	} 
	else {
		usb_set_pid_stall(PIPE0);							/* Req Error */
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Set Descriptor
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_SetDescriptor(USB_U16 type, USB_U16 value, 
*             :                      USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : USB標準リクエスト(SET_DESCRIPTOR)のライトデータステージを実行します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type		    ; Recipient (bmRequestType)
*             : USB_U16 value			; wValue
*             : USB_U16 index			; wIndex
*             : USB_U16 length		    ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USBサンプルドライバではSET_DESCRIPTORをサポートしていないので、
*             : USBホストにストール応答します。
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_SetDescriptor(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;
	

	usb_set_pid_stall(PIPE0);								/* Not Spec */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Alternate table clear
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_clear_alt(void);
*----------------------------------------------------------------------------- 
* Description : Alternate設定領域を初期化します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_clear_alt(void)
{
	int i;

	for( i = 0; i < ALT_NO; ++i ) {
		Alternate[i] = 0;							/* Alternate */
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Set Configuration
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_SetConfiguration(USB_U16 type, USB_U16 value,
*             :                         USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : USB標準リクエスト(SET_CONFIGURATION)のノーデータ用ステータスステージを実行します。
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
void P_SetConfiguration(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 i, ifc;

	if( type == DEVICE ) {
		if( (value <= P_GetConfigNum()) && (index == 0) && (length == 0) ) {
			ConfigNum = value;							/* Configuration Number set */
			usb_clear_alt();							/* Alternate setting clear */
			usb_set_pid_buf(PIPE0);
			if( ConfigNum > 0 ) {
				clear_ep_table_index();
				ifc = P_GetInterfaceNum(ConfigNum);
				for( i = 0; i < ifc; ++i ) {
					P_InitEPTable(ConfigNum, i, 0);
				}
				clear_pipe_tbl();
				P_ResetEP(ConfigNum);
			}
			P_ChangeConfigCB(ConfigNum);
		}
		else {
			usb_set_pid_stall(PIPE0);					/* Req Error */
		}
	}
	else {
		usb_set_pid_stall(PIPE0);						/* Req Error */
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Set Interface
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_SetInterface(USB_U16 type, USB_U16 value,
*             :                     USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : USB標準リクエスト(SET_INTERFACE)のノーデータ用ステータスステージを実行します。
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
void P_SetInterface(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	if( (P_IsConfigured() == USB_YES) && (type == INTERFACE) ) {	/* Configured ? */
		if( (index <= P_GetInterfaceNum(ConfigNum)) && (length == 0) ) {
			if( value <= P_GetAltNum(ConfigNum, index) ) {
				Alternate[index] = (USB_U16)(value & ALT_SET);
				usb_set_pid_buf(PIPE0);
				clear_ep_table_index();
				/* Search endpoint setting */
				P_InitEPTable(ConfigNum, index, Alternate[index]);
				P_ResetEP(ConfigNum);
				P_ChangeInterfaceCB(ConfigNum, index, Alternate[index]);
			} 
			else {
				usb_set_pid_stall(PIPE0);					/* Req Error */
			}
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
* Outline     : Set Frame
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_SynchFrame(USB_U16 type, USB_U16 value,
*             :                   USB_U16 index, USB_U16 length);
*----------------------------------------------------------------------------- 
* Description : USB標準リクエスト(SYNCH_FRAME)のリードデータステージを実行します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type		    ; Recipient (bmRequestType)
*             : USB_U16 value			; wValue
*             : USB_U16 index			; wIndex
*             : USB_U16 length		    ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USBサンプルドライバではSYNCH_FRAMEをサポートしていないので、
*             : USBホストにストール応答します。
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_SynchFrame(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;

	usb_set_pid_stall(PIPE0);
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Pipe table clear
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void clear_pipe_tbl(void);
*----------------------------------------------------------------------------- 
* Description : パイプ定義テーブルを初期化します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
static void clear_pipe_tbl(void)
{
	int i;

	for( i = 0; i < (MAX_PIPE_NO + 1); ++i ) {
		PipeTbl[i] = 0;
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Endpoint index table clear
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void clear_ep_table_index(void);
*----------------------------------------------------------------------------- 
* Description : エンドポイントテーブルインデックスを初期化します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
static void clear_ep_table_index(void)
{
	int i;

	for( i = 0; i <= MAX_EP_NO; ++i ) {
		EPTableIndex[i] = EP_ERROR;
	}
}

/*===========================================================================*/
/* End of File */
