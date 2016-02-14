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
 *       File Name   : c_intrn.c
 *       Abstract    : pipe interrupt
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
#include "usb.h"

int	USB_read_end_callback(USB_U16 pipe);

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : BRDY interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_brdy_int(USB_U16 Status, USB_U16 Int_enbl);
*----------------------------------------------------------------------------- 
* Description : BRDY割り込み処理を行います。(PIPE1-9)
*             : 割り込みが発生したパイプに応じて、パイプに割り当てられた
*             : バッファからの読み出しまたはバッファへの書き込みを行います。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 Status       ; BRDYSTS Register Value
*             : USB_U16 Int_enbl     ; BRDYENB Register Value
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : BRDY割り込みハンドラ処理にて実行されます。
*             : 本関数にてBRDY割り込みステータスおよびBEMP割り込みステータス
*             : がクリアされます。
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_brdy_int(USB_U16 status, USB_U16 int_enb)
{
	USB_U16 pipe,pipebit;

	for( pipe = PIPE1; pipe <= PIPE9; pipe++ ) {
			pipebit = BitSet[pipe];
		if( (status & pipebit) && (int_enb & pipebit) ) {
			USB.BRDYSTS.WORD = (USB_U16)~pipebit;	/* BRDY Status Clear */
			USB.BEMPSTS.WORD = (USB_U16)~pipebit;	/* BEMP Status Clear */
			USB.PIPESEL.BIT.PIPESEL =  pipe;
//			if((PipeTbl[pipe] & FIFO_USE) == D0_FIFO_DMA ){
//				if(USBDmaStatus[USB_D0FIFO] == USB_DMA_BUSY){
//					usb_dma_interrupt0();
//				}
//				usb_read_dma(pipe);
//			}
//			else if((PipeTbl[pipe] & FIFO_USE) == D1_FIFO_DMA ){
//				if(USBDmaStatus[USB_D1FIFO] == USB_DMA_BUSY){
//					usb_dma_interrupt1();
//				}
//				usb_read_dma(pipe);
//			}
//			else{
			{
				if( USB.PIPECFG.BIT.DIR  == 0 ) {
					usb_read_buffer(pipe);
					USB_read_end_callback(pipe);
				} 
				else {
					usb_write_buffer(pipe);
				}
			}
		}
	}
}


/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : NRDY interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_nrdy_int(USB_U16 status, USB_U16 int_enb);
*----------------------------------------------------------------------------- 
* Description : NRDY割り込み処理を行います。(PIPE1-9)
*             : PIDによりNRDY割り込み要因を確認し、要因がSTALLの場合は、
*             : パイプ状態をSTALLとし、処理を終了します。
*             : 要因がSTALL以外の場合はエラーカウントをインクリメントし、
*             : 再度USB通信を行います。エラーカウントがPIPEERRとなった場合は
*             : パイプ状態をPIPE_NORESとし、処理を終了します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 status       ; NRDYSTS Register Value
*             : USB_U16 int_enb      ; NRDYENB Register Value
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : NRDY割り込みハンドラ処理にて実行されます。
*             : 本関数にてNRDY割り込みステータスがクリアされます。
*""FUNC COMMENT END""*********************************************************/
void usb_nrdy_int(USB_U16 status, USB_U16 int_enb)
{
	USB_U16 pid, pipe,bitcheck;

	bitcheck	= (USB_U16)(status & int_enb);

	USB.NRDYSTS.WORD = (USB_U16)~status;
	for( pipe = PIPE1; pipe <= PIPE9; pipe++ ) {
		if( (bitcheck&BitSet[pipe])==BitSet[pipe] ) {	/* interrupt check */
			if(USB.SYSCFG.BIT.DCFM == 1){
				/* USB Host */
				if(PipeFlag[pipe] == PIPE_WAIT){
					pid = usb_get_pid(pipe);
					if( ( pid == PID_STALL ) || ( pid == PID_STALL2 ) ){	/* STALL ? */
						PipeFlag[pipe] = PIPE_STALL;
					}
					else{
						PipeIgnore[pipe]++;				/* Ignore count */
						if(PipeIgnore[pipe] == PIPEERR){
							PipeFlag[pipe] = PIPE_NORES;
						}
						else{
							usb_set_pid_buf(pipe);		/* PIPE0 Send IN or OUT token */
						}
					}
				}
			}
			else{
				/* USB Function */
			}
		}
	}

}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : BEMP interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_bemp_int(USB_U16 status, USB_U16 int_enb);
*----------------------------------------------------------------------------- 
* Description : BEMP割り込み処理を行います。(PIPE1-9)
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 status       ; BEMPSTS Register Value
*             : USB_U16 int_enb      ; BEMPENB Register Value
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_bemp_int(USB_U16 status, USB_U16 int_enb)
{

	USB_U16 pid, pipe,bitcheck,inbuf;

	bitcheck	= (USB_U16)(status & int_enb);

	USB.BEMPSTS.WORD = (USB_U16)~status;
	for( pipe = PIPE1; pipe <= PIPE9; pipe++ ) {
		if( (bitcheck&BitSet[pipe])==BitSet[pipe] ) {	/* interrupt check */
			pid = usb_get_pid(pipe);
			if( ( pid == PID_STALL ) || ( pid == PID_STALL2 ) ){	/* STALL ? */
				PipeFlag[pipe] = PIPE_STALL;
			}
			else{
				inbuf = usb_get_inbuf( pipe );
				if( inbuf == 0 ) {
					usb_disable_bemp_int( pipe );		/* Disable BEMP Interrupt */
					usb_set_pid_nak(pipe);
					PipeFlag[ pipe ] = PIPE_DONE;		/* End */
				}
			}
		}
	}
}

/*===========================================================================*/
/* End of File */
