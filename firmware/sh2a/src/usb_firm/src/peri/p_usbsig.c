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
 *       File Name   : p_usbsig.c
 *       Abstract    : Peripheral Bus Driver
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
* Outline     : Peripheral mode init
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_ModeInit(void);
*----------------------------------------------------------------------------- 
* Description : USBモジュールをUSBファンクションモードにて初期化します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_ModeInit(void)
{
	usb_set_int_level(USBLVL);
	
	usb_reset_module();				/* reset USB module with setting tranciever	*/
									/* and HSE=1								*/

	usb_init_mem();					/* clear variables							*/

	P_InitModule();					/* select USB Function and Interrupt Enable	*/
									/* Detect USB Device to attach or detach	*/
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Initialize USB Module
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_InitModule(void);
*----------------------------------------------------------------------------- 
* Description : USBモジュールをUSBファンクションモードにて初期化します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_InitModule(void)
{
	USB.SYSCFG.BIT.DCFM  = 0;		/* USB function 			*/

	/* USB clock operation enabled */
	USB.SYSCFG.WORD = BITSCKE;

	/* USB module operation enabled */
	USB.SYSCFG.BIT.USBE = 1;

	USB.SYSCFG.BIT.HSE = 1;			/* Hi-Speed Mode */
	
	/* for power-on 	*/
	if(P_CheckVBUStaus() == USB_ON){	/* monitor VBUS pin			*/
		P_EnableINTModule();			/* Interrupt Enable 		*/
		P_USBAttachCB();				/* pull-up D+ and open D-	*/
	}
	else {
		P_USBDetachCB();				/* USB Detach 				*/
										/* with Interrupt Enable	*/
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Check VBUS status
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 P_CheckVBUStaus(void);
*----------------------------------------------------------------------------- 
* Description : USB-VBUS状態を確認し、USBホストへの接続状態を戻り値として返します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: USB_ON	:	VBUS ON
*             : USB_OFF	:	VBUS OFF
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 P_CheckVBUStaus(void)
{
	USB_U16	buf1, buf2, buf3;

	/* monitor VBUS pins					*/
	do {
		buf1 = USB.INTSTS0.BIT.VBSTS;
		usb_delay_10us(1);
		buf2 = USB.INTSTS0.BIT.VBSTS;
		usb_delay_10us(1);
		buf3 = USB.INTSTS0.BIT.VBSTS;
	} while( (buf1 != buf2) || (buf2 != buf3) );

	if( buf1 == USB_OFF ){
		return USB_OFF;		/* detach		*/
	}
	return USB_ON;			/* attach		*/
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : USB attach
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_USBAttachCB(void);
*----------------------------------------------------------------------------- 
* Description : USBホストへの接続処理を行います。本関数にてD+をPull-upします。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_USBAttachCB(void)
{

#if PRINT_MODE == PRINT_USE
	printf( "Attach to USB Host\n");
	printf( "\n");
	printf( "\n");
	printf( "\n");
	printf( "\n");
#endif

	/* FIFO Clear, etc ... */
	usb_delay_xms(10);

	USB.SYSCFG.BIT.DPRPU = 1;				/* Pull-up D+ and open D-	 */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : USB detach
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_USBDetachCB(void);
*----------------------------------------------------------------------------- 
* Description : USBホストからの切断処理を行います。本関数にてD+/D-をopenします。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_USBDetachCB(void)
{
	USB_U16 pipe;

#if PRINT_MODE == PRINT_USE
	#if	1
	printf( "Detach from USB Host\n");
	printf( "\n");
	printf( "\n");
	printf( "\n");
	printf( "\n");
	#endif
#endif

	/* Terminate all the pipes in which communications on port 	*/
	/* are currently carried out 								*/
	for( pipe = 0; pipe < (MAX_PIPE_NO + 1); ++pipe ) {
		if( PipeFlag[ pipe ] != PIPE_IDLE ){
			usb_stop_transfer( pipe );
		}
	}

	USB.SYSCFG.BIT.DPRPU = 0;				/* open D+ and D-		*/

	USB.SYSCFG.BIT.USBE  = 0;				/* soft reset module	*/
	USB.SYSCFG.BIT.USBE  = 1;

	P_EnableINTModule();					/* Interrupt Enable 	*/

}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : USB bus reset
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_USBBusResetCB(void);
*----------------------------------------------------------------------------- 
* Description : USBデバイスがPOWERD_STATEに遷移した際に本関数が実行されます。
*             : USBリセットハンドシェイクにより決定した接続スピードに応じて、
*             : デバイス・ディスクリプタを設定します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_USBBusResetCB(void)
{

	usb_init_mem();									/* memory clear */

	if( usb_is_hispeed() == HI_SPEED ){
		P_ResetDeviceDescriptor(HI_SPEED);			/* Device Descriptor reset */
	}
	else{
		P_ResetDeviceDescriptor(FULL_SPEED);		/* Device Descriptor reset */
	}
	P_ResetDCP();									/* Default Control PIPE reset */

	/*
	 * user program
	 */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : USB resume
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_USBResumeCB(void);
*----------------------------------------------------------------------------- 
* Description : USBデバイスがレジューム信号を検出した場合に本関数が実行されます。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USBサンプルドライバでは何もしません。
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_USBResumeCB(void)
{
	/* NOP */

}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : USB suspend
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_USBSuspendCB(void);
*----------------------------------------------------------------------------- 
* Description : USBデバイスがサスペンド信号を検出した場合に本関数が実行されます。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USBサンプルドライバでは何もしません。
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_USBSuspendCB(void)
{
	/* NOP */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : USB testmode (Set_Feature).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_USBTestModeCB(void);
*----------------------------------------------------------------------------- 
* Description : USBデバイスがUSB標準リクエストによりTEST_MODEへ遷移した場合、
*             : 本関数が実行されます。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_USBTestModeCB(void)
{
	switch( TestModeSelectors & TEST_SELECT ) {
	case TEST_J:
	case TEST_K:
	case TEST_SE0_NAK:
	case TEST_PACKET:
		USB.TESTMODE.BIT.UTST = (TestModeSelectors >> 8);
		break;
	case TEST_FORCE_ENABLE:
	default:
		break;
	}
}

/*===========================================================================*/
/* End of File */
