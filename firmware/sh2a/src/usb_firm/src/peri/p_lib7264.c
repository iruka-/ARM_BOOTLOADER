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
 *       File Name   : p_lib7264.c
 *       Abstract    : Peripheral SH7264 USB Register
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
#include "p_def_ep.h"

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Reset default control pipe
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_ResetDCP(void);
*----------------------------------------------------------------------------- 
* Description : デフォルトコントロールパイプ(DCP)を初期化します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_ResetDCP(void)
{
	USB.DCPCFG.WORD = 0;						/* DCP Configuration Register  (0x5C) */
	USB.DCPMAXP.WORD = DeviceDescriptor[7];	/* DCP Maxpacket Size Register (0x5E) */

	/* xFIFOSEL initialization */
	USB.CFIFOSEL.WORD =  ( USB_U16 )(           BITMBW_16 | BITBYTE_BIG );
	USB.D0FIFOSEL.WORD = ( USB_U16 )(           BITMBW_16 | BITBYTE_BIG );
	USB.D1FIFOSEL.WORD = ( USB_U16 )(           BITMBW_16 | BITBYTE_BIG );

	/*
	 * DCP FIFO Clear, etc ...
	 */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Reset endpoint
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_ResetEP(USB_U16 num);
*----------------------------------------------------------------------------- 
* Description : エンドポイントを初期化します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 num          ; Configuration Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_ResetEP(USB_U16 num)
{
	USB_U16		pipe, ep, index, buf;
	USB_U16		*tbl;

	/* Pipe Setting */
	tbl = (USB_U16 *)(EndPntPtr[num-1]);

	for( ep = 1; ep <= MAX_EP_NO; ++ep ) {
		if( EPTableIndex[ep] != EP_ERROR ) {
			index = (USB_U16)(EPL * EPTableIndex[ep]);
			pipe = (USB_U16)(tbl[index+0] & BITCURPIPE);		/* Pipe Number */

			PipeTbl[pipe] = (USB_U16)(((tbl[index+1] & BITDIR) << 3) | 
										ep | 
										(tbl[index+0] & FIFO_USE));

			if((tbl[index+1] & BITDIR) == DIR_P_OUT){
				tbl[index+1]	|= SHTNAKON;
			}

			/* Interrupt Disable */
			buf = USB.BRDYENB.WORD;
			buf &= (USB_U16)~(BitSet[pipe]);
			USB.BRDYENB.WORD = buf;				/* Ready Interrupt Disable */
			buf = USB.NRDYENB.WORD;
			buf &= (USB_U16)~(BitSet[pipe]);
			USB.NRDYENB.WORD = buf;				/* NotReady Interrupt Disable */
			buf = USB.BEMPENB.WORD;
			buf &= (USB_U16)~(BitSet[pipe]);
			USB.BEMPENB.WORD = buf;				/* Empty/SizeErr Interrupt Disable */

			usb_set_pid_nak(pipe);					/* PID = NAK */

			/* CurrentPIPE Clear */
			if( USB.CFIFOSEL.BIT.CURPIPE == pipe ){
				USB.CFIFOSEL.BIT.CURPIPE = 0;
			}
			if( USB.D0FIFOSEL.BIT.CURPIPE == pipe ){
				USB.D0FIFOSEL.BIT.CURPIPE = 0;
			}
			if( USB.D1FIFOSEL.BIT.CURPIPE == pipe ){
				USB.D1FIFOSEL.BIT.CURPIPE = 0;
			}

			/* PIPE Configuration */
			USB.PIPESEL.WORD = pipe;
			USB.PIPECFG.WORD = tbl[index+1];
			USB.PIPEBUF.WORD = tbl[index+2];
			USB.PIPEMAXP.WORD = tbl[index+3];
			USB.PIPEPERI.WORD = tbl[index+4];

			/* Buffer Clear */
			usb_set_sqclr(pipe);							/* SQCLR=1 */
			usb_aclrm(pipe);

			/* init Global */
			PipeFlag[pipe]     = PIPE_IDLE;
			PipeDataSize[pipe] = 0;
		}
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Device descriptor table initialize
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_ResetDeviceDescriptor(USB_U16 mode);
*----------------------------------------------------------------------------- 
* Description : USBデバイスディスクリプタを初期化します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 mode ; FULL_SPEED
*             :              ; HI_SPEED
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_ResetDeviceDescriptor(USB_U16 mode)
{
	if( mode == HI_SPEED ) {
		/* Hi-Speed Mode */
		Configuration_Hi_1[1]	= DT_CONFIGURATION;
		Configuration_Hi_2[1]	= DT_CONFIGURATION;
		Configuration_Hi_3[1]	= DT_CONFIGURATION;
		Configuration_Hi_4[1]	= DT_CONFIGURATION;
		Configuration_Hi_5[1]	= DT_CONFIGURATION;
		Configuration_Full_1[1]	= DT_OTHER_SPEED_CONFIGURATION;
		Configuration_Full_2[1]	= DT_OTHER_SPEED_CONFIGURATION;
		Configuration_Full_3[1]	= DT_OTHER_SPEED_CONFIGURATION;
		Configuration_Full_4[1]	= DT_OTHER_SPEED_CONFIGURATION;
		Configuration_Full_5[1]	= DT_OTHER_SPEED_CONFIGURATION;
		ConPtr[0]				= Configuration_Hi_1;
		ConPtr[1]				= Configuration_Hi_2;
		ConPtr[2]				= Configuration_Hi_3;
		ConPtr[3]				= Configuration_Hi_4;
		ConPtr[4]				= Configuration_Hi_5;
		ConPtr_Other[0]			= Configuration_Full_1;
		ConPtr_Other[1]			= Configuration_Full_2;
		ConPtr_Other[2]			= Configuration_Full_3;
		ConPtr_Other[3]			= Configuration_Full_4;
		ConPtr_Other[4]			= Configuration_Full_5;
		EPtbl_1[3]				= 512;
		EPtbl_1[8]				= 512;
		EPtbl_1[13]				= 512;
		EPtbl_1[18]				= 512;
		EPtbl_1[23]				= 512;
	} 
	else {
		/* Full-Speed Mode */
		Configuration_Hi_1[1]	= DT_OTHER_SPEED_CONFIGURATION;
		Configuration_Hi_2[1]	= DT_OTHER_SPEED_CONFIGURATION;
		Configuration_Hi_3[1]	= DT_OTHER_SPEED_CONFIGURATION;
		Configuration_Hi_4[1]	= DT_OTHER_SPEED_CONFIGURATION;
		Configuration_Hi_5[1]	= DT_OTHER_SPEED_CONFIGURATION;
		Configuration_Full_1[1]	= DT_CONFIGURATION;
		Configuration_Full_2[1]	= DT_CONFIGURATION;
		Configuration_Full_3[1]	= DT_CONFIGURATION;
		Configuration_Full_4[1]	= DT_CONFIGURATION;
		Configuration_Full_5[1]	= DT_CONFIGURATION;
		ConPtr[0]				= Configuration_Full_1;
		ConPtr[1]				= Configuration_Full_2;
		ConPtr[2]				= Configuration_Full_3;
		ConPtr[3]				= Configuration_Full_4;
		ConPtr[4]				= Configuration_Full_5;
		ConPtr_Other[0]			= Configuration_Hi_1;
		ConPtr_Other[1]			= Configuration_Hi_2;
		ConPtr_Other[2]			= Configuration_Hi_3;
		ConPtr_Other[3]			= Configuration_Hi_4;
		ConPtr_Other[4]			= Configuration_Hi_5;
		EPtbl_1[3]				= 64;
		EPtbl_1[8]				= 64;
		EPtbl_1[13]				= 64;
		EPtbl_1[18]				= 64;
		EPtbl_1[23]				= 64;
	}
}

/*===========================================================================*/
/* End of File */
