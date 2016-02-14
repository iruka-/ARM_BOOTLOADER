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
 *       File Name   : p_status.c
 *       Abstract    : Peripheral Internal status
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
* Outline     : Get Configuration number
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 P_GetConfigNum(void);
*----------------------------------------------------------------------------- 
* Description : デバイス・ディスクリプタに記載されているコンフィギュレーション数を参照し、
*             : コンフィギュレーション数を戻り値として返します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: Number of possible configurations (bNumConfigurations).
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 P_GetConfigNum(void)
{
	return (USB_U16)DeviceDescriptor[DEV_NUM_CONFIG];
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Get Interface number
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 P_GetInterfaceNum(USB_U16 num);
*----------------------------------------------------------------------------- 
* Description : コンフィギュレーション・ディスクリプタに記載されているインタフェース数を参照し、
*             : インタフェース数を戻り値として返します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 Con_Num		; Configuration Number
*----------------------------------------------------------------------------- 
* Return Value: Number of this interface (bNumInterfaces).
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 P_GetInterfaceNum(USB_U16 num)
{
	return (USB_U16)(*(ConPtr[num-1]+4));
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Get Alternate setting number
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 P_GetAltNum(USB_U16 Con_Num, USB_U16 Int_Num);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定されたコンフィギュレーション番号、インタフェース番号の
*             : Alternate設定値を戻り値として返します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 Con_Num		; Configuration Number
*             : USB_U16 Int_Num		; Interface Number
*----------------------------------------------------------------------------- 
* Return Value: Value used to select this alternate setting(bAlternateSetting).
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 P_GetAltNum(USB_U16 Con_Num, USB_U16 Int_Num)
{
	USB_U16	i;
	USB_U16	alt_num = 0;
	USB_U8	*ptr;
	USB_U16	length;

	ptr = (USB_U8 *)(ConPtr[Con_Num-1]);				/* ConfigurationDescriptor[0] */
	i = ptr[0];
	ptr += ptr[0];									/* InterfaceDescriptor[0] */
	length  = (USB_U16)(*(ConPtr[Con_Num-1]+2));
	length |= (USB_U16)((USB_U16)(*(ConPtr[Con_Num-1]+3)) << 8);
	for( ; i < length; ) {							/* Search Descriptor Table size */
		switch( ptr[1] ) {							/* Descriptor Type ? */
		case DT_INTERFACE:							/* Interface */
			if( Int_Num == ptr[2] ) {
				alt_num = (USB_U16)ptr[3];				/* Alternate Number count */
			}
			i += ptr[0];
			ptr += ptr[0];
			break;
		case DT_DEVICE:								/* Device */
		case DT_CONFIGURATION:						/* Configuration */
		case DT_STRING:								/* String */
		case DT_ENDPOINT:							/* Endpoint */
		default:									/* Class, Vendor, else */
			i += ptr[0];
			ptr += ptr[0];
			break;
		}
	}
	return alt_num;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Endpoint table initialize
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_InitEPTable(USB_U16 Con_Num, USB_U16 Int_Num, USB_U16 Alt_Num);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定されたコンフィギュレーション番号、インタフェース番号、
*             : Alternate設定値にてエンドポイントを設定します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 Con_Num		; Configuration Number
*             : USB_U16 Int_Num		; Interface Number
*             : USB_U16 Alt_Num		; Alternate Setting
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_InitEPTable(USB_U16 Con_Num, USB_U16 Int_Num, USB_U16 Alt_Num)
{
	USB_U8		*ptr;
	USB_U16		i, j, length;
	USB_U16		start, numbers, ep;

	ptr = (USB_U8 *)ConPtr[Con_Num-1];					/* ConfigurationDescriptor */
	i = *ptr;
	length = (USB_U16)((USB_U16)*(ptr + 3) << 8 | (USB_U16)*(ptr + 2));
	ptr += *ptr;
	start = 0;
	numbers = 0;
	j = 0;

	for( ; i < length; ) {
		switch( *(ptr + 1) ) {						/* Descriptor Type ? */
		case DT_INTERFACE:							/* Interface */
			if( (*(ptr + 2) == Int_Num) && (*(ptr + 3) == Alt_Num) ) {
				numbers = *(ptr + 4);
			} 
			else {
				start += *(ptr + 4);
			}
			i += *ptr;
			ptr += *ptr;
			break;
		case DT_ENDPOINT:							/* Endpoint */
			if( j < numbers ) {
				ep = (USB_U16)(*(ptr + 2) & 0x0f);
				EPTableIndex[ep] = (USB_U16)(start + j);
				++j;
			}
			i += *ptr;
			ptr += *ptr;
			break;
		case DT_DEVICE:								/* Device */
		case DT_CONFIGURATION:						/* Configuration */
		case DT_STRING:								/* String */
		default:									/* Class, Vendor, else */
			i += *ptr;
			ptr += *ptr;
			break;
		}
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Check remote wakeup bit (bmAttributes);
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 P_CheckRemoteWakeup(void);
*----------------------------------------------------------------------------- 
* Description : コンフィギュレーション・ディスクリプタを参照し、
*             : リモートウェイクアップ機能がサポートされているかを戻り値として返します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: USB_ON	:	Support Remote Wakeup
*             : USB_OFF	:	not Support Remote Wakeup
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 P_CheckRemoteWakeup(void)
{
	USB_U8	atr;

	if( ConfigNum == 0 ){
		return USB_OFF;
	}

	atr = *(ConPtr[ConfigNum - 1] + 7);
	if( atr & CF_RWUP ){
		return USB_ON;
	}
	return USB_OFF;
}

/*===========================================================================*/
/* End of File */
