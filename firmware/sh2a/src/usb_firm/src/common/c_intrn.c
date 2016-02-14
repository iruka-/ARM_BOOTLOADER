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
* Description : BRDY���荞�ݏ������s���܂��B(PIPE1-9)
*             : ���荞�݂����������p�C�v�ɉ����āA�p�C�v�Ɋ��蓖�Ă�ꂽ
*             : �o�b�t�@����̓ǂݏo���܂��̓o�b�t�@�ւ̏������݂��s���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 Status       ; BRDYSTS Register Value
*             : USB_U16 Int_enbl     ; BRDYENB Register Value
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : BRDY���荞�݃n���h�������ɂĎ��s����܂��B
*             : �{�֐��ɂ�BRDY���荞�݃X�e�[�^�X�����BEMP���荞�݃X�e�[�^�X
*             : ���N���A����܂��B
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
* Description : NRDY���荞�ݏ������s���܂��B(PIPE1-9)
*             : PID�ɂ��NRDY���荞�ݗv�����m�F���A�v����STALL�̏ꍇ�́A
*             : �p�C�v��Ԃ�STALL�Ƃ��A�������I�����܂��B
*             : �v����STALL�ȊO�̏ꍇ�̓G���[�J�E���g���C���N�������g���A
*             : �ēxUSB�ʐM���s���܂��B�G���[�J�E���g��PIPEERR�ƂȂ����ꍇ��
*             : �p�C�v��Ԃ�PIPE_NORES�Ƃ��A�������I�����܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 status       ; NRDYSTS Register Value
*             : USB_U16 int_enb      ; NRDYENB Register Value
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : NRDY���荞�݃n���h�������ɂĎ��s����܂��B
*             : �{�֐��ɂ�NRDY���荞�݃X�e�[�^�X���N���A����܂��B
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
* Description : BEMP���荞�ݏ������s���܂��B(PIPE1-9)
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
