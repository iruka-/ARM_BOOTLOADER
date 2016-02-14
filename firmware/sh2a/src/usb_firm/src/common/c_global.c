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
 *       System Name : SH7264 USB FUCNTION Sample Program
 *       File Name   : c_global.c
 *       Abstract    : Global variable
 *       Version     : 1.01.00
 *       Device      : SH7264/SH7262
 *       Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
 *                   : C/C++ compiler package for the SuperH RISC engine family
 *                   :                             (Ver.9.02 Release00).
 *       OS          : None
 *       H/W Platform: M3A-HS64G50(CPU board)
 *
 *       History   : May.13.2009 ver.1.00.00
 *                 : Sep.16.2009 ver.1.01.00
 *""FILE COMMENT END""**********************************************************/
#define	__GLOBAL_C__

#include "usb.h"

/* ==== global valiable (COMMON) ==== */
const USB_U16 BitSet[16] = {	0x0001, 0x0002, 0x0004, 0x0008,
								0x0010, 0x0020, 0x0040, 0x0080,
								0x0100, 0x0200, 0x0400, 0x0800,
								0x1000, 0x2000, 0x4000, 0x8000 };

USB_U32 USBDataCount[MAX_PIPE_NO + 1];			/* PIPEn Buffer counter */
USB_U32 USBRcvDataCount[MAX_PIPE_NO + 1];		/* PIPEn receive data counter */
USB_U8  *USBDataPointer[MAX_PIPE_NO + 1];		/* PIPEn Buffer pointer(8bit) */

USB_U16	ConfigNum;

USB_U16	PipeIgnore[ MAX_PIPE_NO + 1 ];			/* Ignore count */
USB_U16	PipeTbl[ MAX_PIPE_NO + 1 ];				/* C/D FIFO | DIR | EPnum */
USB_U16	PipeFlag[ MAX_PIPE_NO + 1 ];			/* data flag */
USB_U32	PipeDataSize[ MAX_PIPE_NO + 1 ];		/* data size */

USB_DMA	USBDmaInfo[2];							/* DMA Setting Infomation */
USB_U16	USBDmaPipe[2];							/* DMA Transfar Pipe */
USB_U16	USBDmaVal[2];							/* VAL(FIFOCTR) set flag after dma end */
USB_U16 USBDmaStatus[2];						/* DMA Transfer Flag */

/* ==== global valiable (FUNCTION) ==== */
/* Idle/Setup Stage */
void (*pbRequest0[13])(USB_U16 Reqtype, USB_U16 Value, USB_U16 Index, USB_U16 Length) = {
	P_Resrv_0,				P_Resrv_0,				P_Resrv_0,
	P_Resrv_0,				P_Resrv_0,				P_Resrv_0,
	P_Resrv_0,				P_Resrv_0,				P_Resrv_0,
	P_Resrv_0,				P_Resrv_0,				P_Resrv_0,
	P_Resrv_0
};

/* Control Read Data Stage */
void (*pbRequest1[13])(USB_U16 Reqtype, USB_U16 Value, USB_U16 Index, USB_U16 Length) = {
	P_GetStatus,			P_Resrv_123,			P_Resrv_123,
	P_Resrv_123,			P_Resrv_123,			P_Resrv_123,
	P_GetDescriptor,		P_Resrv_123,			P_GetConfiguration,
	P_Resrv_123,			P_GetInterface,			P_Resrv_123,
	P_SynchFrame
};

/* Control Write Data Stage */
void (*pbRequest2[13])(USB_U16 Reqtype, USB_U16 Value, USB_U16 Index, USB_U16 Length) = {
	P_Resrv_123,			P_Resrv_123,			P_Resrv_123,
	P_Resrv_123,			P_Resrv_123,			P_Resrv_123,
	P_Resrv_123,			P_SetDescriptor,		P_Resrv_123,
	P_Resrv_123,			P_Resrv_123,			P_Resrv_123,
	P_Resrv_123
};

/* Control Write No Data Status Stage */
void (*pbRequest3[13])(USB_U16 Reqtype, USB_U16 Value, USB_U16 Index, USB_U16 Length) = {
	P_Resrv_123,			P_ClearFeature,			P_Resrv_123,
	P_SetFeature,			P_Resrv_123,			P_SetAddress,
	P_Resrv_123,			P_Resrv_123,			P_Resrv_123,
	P_SetConfiguration,		P_Resrv_123,			P_SetInterface,
	P_Resrv_123
};

/* Control Read Status Stage */
void (*pbRequest4[13])(USB_U16 Reqtype, USB_U16 Value, USB_U16 Index, USB_U16 Length) = {
	P_Resrv_4,				P_Resrv_4,				P_Resrv_4,
	P_Resrv_4,				P_Resrv_4,				P_Resrv_4,
	P_Resrv_4,				P_Resrv_4,				P_Resrv_4,
	P_Resrv_4,				P_Resrv_4,				P_Resrv_4,
	P_Resrv_4
};

/* Control Write Status Stage */
void (*pbRequest5[13])(USB_U16 Reqtype, USB_U16 Value, USB_U16 Index, USB_U16 Length) = {
	P_Resrv_5,				P_Resrv_5,				P_Resrv_5,
	P_Resrv_5,				P_Resrv_5,				P_Resrv_5,
	P_Resrv_5,				P_Resrv_5,				P_Resrv_5,
	P_Resrv_5,				P_Resrv_5,				P_Resrv_5,
	P_Resrv_5
};

USB_U16	 Alternate[ALT_NO];					/* Alternate */
USB_U16	 RemoteWakeupFlag ;					/* Remote Wakeup Enable Flag */
USB_U16	 TestModeFlag ;						/* Test Mode Flag */
USB_U16  TestModeSelectors;					/* Test Mode Selectors */

USB_U16	 ReqType;							/* request type */
USB_U16	 ReqTypeType;						/* request type TYPE */
USB_U16	 ReqTypeRecip;						/* request type RECIPIENT */
USB_U16	 ReqRequest;						/* request */
USB_U16  ReqValue;							/* value */
USB_U16  ReqIndex;							/* index */
USB_U16  ReqLength;							/* length */

USB_U16	 EPTableIndex[MAX_EP_NO + 1];		/* Index of Endpoint Information table */

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Initialize Memory
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_init_mem(void);
*----------------------------------------------------------------------------- 
* Description : USBサンプルドライバ変数を初期化します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_init_mem(void)
{
	USB_U16	i;

	ConfigNum = 0;					/* configuration Number */

	for( i = 0; i < (MAX_PIPE_NO + 1); ++i ) {
		PipeFlag[i]     = PIPE_IDLE;
		PipeDataSize[i] = 0;
	}
}
/* End of File */
