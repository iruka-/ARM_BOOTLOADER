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
 *       System Name : SH7264 USB Sample Program
 *       File Name   : c_dataio.c
 *       Abstract    : fifo read/write
 *       Version     : 1.02.00
 *       Device      : SH7264/SH7262
 *       Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
 *                   : C/C++ compiler package for the SuperH RISC engine family
 *                   :                             (Ver.9.02 Release00).
 *       OS          : None
 *       H/W Platform: M3A-HS64G50(CPU board)
 *
 *       History   : May.13.2009 ver.1.00.00
 *                 : Sep.16.2009 ver.1.01.00
 *                 : Dec.02.2009 ver.1.02.00
 *""FILE COMMENT END""**********************************************************/
#include "usb.h"

static USBmbw[ ( MAX_PIPE_NO + 1 )];

/* ==== prototype declaration ==== */
static void 	usb_start_recieve_trns_c(USB_U16 pipe, USB_U32 size, USB_U8 *data);
static void 	usb_start_recieve_trns_d0(USB_U16 pipe, USB_U32 size, USB_U8 *data);
static void 	usb_start_recieve_trns_d1(USB_U16 pipe, USB_U32 size, USB_U8 *data);
//static void 	usb_start_recieve_dma_d0(USB_U16 pipe, USB_U32 size, USB_U8 *data);
//static void 	usb_start_recieve_dma_d1(USB_U16 pipe, USB_U32 size, USB_U8 *data);
//static USB_U16 	usb_read_dma_d0(USB_U16 pipe);
//static USB_U16 	usb_read_dma_d1(USB_U16 pipe);
//static USB_U16 	usb_write_dma_d0(USB_U16 pipe);
//static USB_U16 	usb_write_dma_d1(USB_U16 pipe);

static void 	usb_read_c_fifo(USB_U16 pipe,USB_U16 count);
static void 	usb_write_c_fifo(USB_U16 Pipe,USB_U16 count);
static void 	usb_read_d0_fifo(USB_U16 pipe,USB_U16 count);
static void 	usb_write_d0_fifo(USB_U16 pipe,USB_U16 count);
static void 	usb_read_d1_fifo(USB_U16 pipe,USB_U16 count);
static void 	usb_write_d1_fifo(USB_U16 pipe,USB_U16 count);

static void 	usb_clear_transaction_counter(USB_U16 pipe);
static void 	usb_set_transaction_counter(USB_U16 pipe, USB_U32 count);

static USB_U32	usb_com_get_dmasize(USB_U32 trncount,USB_U32 dtptr);

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Start IN Transfer (send data to host).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_start_send_transfer(USB_U16 pipe, USB_U32 size, USB_U8 *data);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプを使用してUSBデータ通信(送信)を開始します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe    ; Pipe Number
*             : USB_U32 size     ; Data Size
*             : USB_U8  *data    ; Data data Address
*----------------------------------------------------------------------------- 
* Return Value: READEND          ; Read end
*             : READSHRT         ; short data
*             : READING          ; Continue of data read
*             : READOVER         ; buffer over
*             : FIFOERROR        ; FIFO error
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_start_send_transfer(USB_U16 pipe, USB_U32 size, USB_U8 *data)
{
	USB_U16 status;

	USBDataCount[pipe]   = size;
	USBDataPointer[pipe] = (USB_U8 *)data;
	PipeFlag[pipe]       = PIPE_WAIT;

	USB.BEMPSTS.WORD = (USB_U16)~(BitSet[pipe]);
	USB.BRDYSTS.WORD = (USB_U16)~(BitSet[pipe]);

	usb_clear_transaction_counter( pipe );

	status = usb_write_buffer(pipe);
	if(status !=FIFOERROR){
		usb_set_pid_buf(pipe);						/* Set BUF */
	}

	return status;									/* End or Err or Continue */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer Write
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_write_buffer(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファにデータを書き込みます。
*             : 使用するFIFOはパイプ定義テーブルにて設定したFIFOを使用します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe      ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: WRITEEND          ; Write end
*             : WRITESHRT         ; short data
*             : WRITING           ; Continue of data write
*             : WRITEDMA          ; Write DMA
*             : FIFOERROR         ; FIFO status
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_write_buffer(USB_U16 pipe)
{
	USB_U16	status;
	USB_U16 usefifo;

	PipeIgnore[pipe]	= 0;			/* Ignore count clear */
	usefifo = (USB_U16)(PipeTbl[pipe] & FIFO_USE);
	switch(usefifo){
		case	D0_FIFO_USE:
			status	= usb_write_buffer_d0(pipe);
			break;
		case	D1_FIFO_USE:
			status	= usb_write_buffer_d1(pipe);
			break;
//		case	D0_FIFO_DMA:
//			status	= usb_write_dma_d0(pipe);
//			break;
//		case	D1_FIFO_DMA:
//			status	= usb_write_dma_d1(pipe);
//			break;
		default:
			status	= usb_write_buffer_c(pipe);
			break;
	};

	switch( status ){
	case	WRITING:					/* Continue of data write */
		usb_enable_brdy_int(pipe);		/* Enable Ready Interrupt */
		usb_enable_nrdy_int(pipe);		/* Error (NORES or STALL) */
		break;
	case	WRITEEND:					/* End of data write */
	case	WRITESHRT:					/* End of data write */
		usb_disable_brdy_int(pipe);		/* Disable Ready Interrupt */
		usb_enable_bemp_int(pipe);		/* Enable Empty Interrupt */
		usb_enable_nrdy_int(pipe);		/* Error (NORES or STALL) */
		break;
//	case	WRITEDMA:					/* DMA write */
//		usb_enable_nrdy_int(pipe);		/* Error (NORES or STALL) */
//		break;
	case	FIFOERROR:					/* FIFO access status */
	default:
		usb_disable_brdy_int(pipe);		/* Disable Ready Interrupt */
		usb_disable_bemp_int(pipe);		/* Disable Empty Interrupt */
		PipeFlag[pipe] = PIPE_IDLE;
		break;
	}
	return	status;						/* End or Err or Continue */

}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer Write CFIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_write_buffer_c(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファにデータを書き込みます。
*             : CFIFOを使用し、CPU転送にてデータを書き込みます。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe      ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: WRITEEND          ; Write end
*             : WRITESHRT         ; short data
*             : WRITING           ; Continue of data write
*             : WRITEDMA          ; Write DMA
*             : FIFOERROR         ; FIFO status
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_write_buffer_c(USB_U16 pipe)
{
	USB_U16	size, count, buffer, mxps;
	USB_U16	status, mbw;

	mbw = usb_get_mbw( USBDataCount[pipe], (USB_U32)USBDataPointer[pipe]);
	if(pipe==PIPE0) {
		buffer	= usb_change_fifo_port(pipe,CUSE,BITISEL, mbw);
	}
	else{
		buffer	= usb_change_fifo_port(pipe,CUSE,USB_NO, mbw);
	}
	if(buffer==FIFOERROR) {							/* FIFO access status */
		return	FIFOERROR;
	}
	size	= usb_get_buf_size(pipe);				/* Data buffer size */
	mxps	= usb_get_mxps(pipe);					/* Max Packet Size */

	if( USBDataCount[pipe] <= (USB_U32)size) {
		status = WRITEEND;							/* write continues */
		count = (USB_U16)USBDataCount[pipe];
		if ( count == 0 ) {
			status = WRITESHRT;						/* Null Packet is end of write */
		}
		if ( (count%mxps)!=0 ) {
			status = WRITESHRT;						/* Short Packet is end of write */
		}
	} 
	else {
		status = WRITING;							/* write continues */
		count = size;
	}

	usb_write_c_fifo(pipe,count);

	if( USBDataCount[pipe] < (USB_U32)size) {
		USBDataCount[pipe] = 0;
		if(USB.CFIFOCTR.BIT.BVAL == 0){
			USB.CFIFOCTR.WORD = BITBVAL;			/* Short Packet */
		}
	} 
	else {
		USBDataCount[pipe] -= count;
	}
	return	status;									/* End or Err or Continue */

}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer Write D0FIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_write_buffer_d0(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファにデータを書き込みます。
*             : D0FIFOを使用し、CPU転送にてデータを書き込みます。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe      ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: WRITEEND          ; Write end
*             : WRITESHRT         ; short data
*             : WRITING           ; Continue of data write
*             : WRITEDMA          ; Write DMA
*             : FIFOERROR         ; FIFO status
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_write_buffer_d0(USB_U16 pipe)
{
	USB_U16 size, count, buffer, mxps;
	USB_U16 status, mbw;

	mbw = usb_get_mbw( USBDataCount[pipe], (USB_U32)USBDataPointer[pipe]);
	buffer = usb_change_fifo_port(pipe,D0USE,USB_NO, mbw);
	if(buffer==FIFOERROR) {							/* FIFO access status */
		return	FIFOERROR;
	}
	size = usb_get_buf_size(pipe);					/* Data buffer size */
	mxps = usb_get_mxps(pipe);						/* Max Packet Size */

	if( USBDataCount[pipe] <= (USB_U32)size) {
		status = WRITEEND;							/* write continues */
		count = (USB_U16)USBDataCount[pipe];
		if ( count == 0 ) {
			status = WRITESHRT;						/* Null Packet is end of write */
		}
		if ( (count%mxps)!=0 ) {
			status = WRITESHRT;						/* Short Packet is end of write */
		}
	} 
	else {
		status = WRITING;							/* write continues */
		count = size;
	}

	usb_write_d0_fifo(pipe,count);

	if( USBDataCount[pipe] < (USB_U32)size) {
		USBDataCount[pipe] = 0;
		if(USB.D0FIFOCTR.BIT.BVAL == 0){
			USB.D0FIFOCTR.WORD = BITBVAL;			/* Short Packet */
		}
	} 
	else {
		USBDataCount[pipe] -= count;
	}
	return	status;									/* End or Err or Continue */

}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer Write D1FIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_write_buffer_d1(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファにデータを書き込みます。
*             : D1FIFOを使用し、CPU転送にてデータを書き込みます。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe      ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: WRITEEND          ; Write end
*             : WRITESHRT         ; short data
*             : WRITING           ; Continue of data write
*             : WRITEDMA          ; Write DMA
*             : FIFOERROR         ; FIFO status
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_write_buffer_d1(USB_U16 pipe)
{
	USB_U16 size, count, buffer, mxps;
	USB_U16 status, mbw;

	mbw = usb_get_mbw( USBDataCount[pipe], (USB_U32)USBDataPointer[pipe]);
	buffer = usb_change_fifo_port(pipe,D1USE,USB_NO, mbw);
	if(buffer==FIFOERROR) {							/* FIFO access status */
		return	FIFOERROR;
	}
	size = usb_get_buf_size(pipe);					/* Data buffer size */
	mxps = usb_get_mxps(pipe);						/* Max Packet Size */

	if( USBDataCount[pipe] <= (USB_U32)size) {
		status = WRITEEND;							/* write continues */
		count = (USB_U16)USBDataCount[pipe];
		if ( count == 0 ) {
			status = WRITESHRT;						/* Null Packet is end of write */
		}
		if ( (count%mxps)!=0 ) {
			status = WRITESHRT;						/* Short Packet is end of write */
		}
	} 
	else {
		status = WRITING;							/* write continues */
		count = size;
	}

	usb_write_d1_fifo(pipe,count);

	if( USBDataCount[pipe] < (USB_U32)size) {
		USBDataCount[pipe] = 0;
		if(USB.D1FIFOCTR.BIT.BVAL == 0){
			USB.D1FIFOCTR.WORD = BITBVAL;			/* Short Packet */
		}
	} 
	else {
		USBDataCount[pipe] -= count;
	}
	return	status;									/* End or Err or Continue */

}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer DMA Write D0FIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static USB_U16 usb_write_dma_d0(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファにデータを書き込みます。
*             : D0FIFOを使用し、DMA転送にてデータを書き込みます。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe     ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: WRITEEND          : Write end
*             : WRITESHRT         : short data
*             : WRITING           : Continue of data write
*             : WRITEDMA          : Write DMA
*             : FIFOERROR         : FIFO status
*----------------------------------------------------------------------------- 
* Notice      : 使用するDMA-chはusb_userdef_start_dma()にて指定します。
*             : 
*""FUNC COMMENT END""*********************************************************/
#if 0
static USB_U16 usb_write_dma_d0(USB_U16 pipe)
{
	USB_U32 size, count, buffer, mxps;
	USB_U16 status, mbw;

	mbw = usb_get_mbw( USBDataCount[pipe], (USB_U32)USBDataPointer[pipe]);
	buffer = usb_change_fifo_port(pipe,D0DMA,USB_NO, mbw);
	if(buffer==FIFOERROR) {							/* FIFO access status */
		return	FIFOERROR;
	}
	size  = usb_get_buf_size(pipe);					/* Data buffer size */
	mxps  = usb_get_mxps(pipe);						/* Max Packet Size */
	count = USBDataCount[pipe];
	if( count != 0){
		USBDmaPipe[USB_D0FIFO] = pipe;
		if((count % size ) != 0){
			USBDmaVal[USB_D0FIFO] = 1;
		}
		else{
			USBDmaVal[USB_D0FIFO] = 0;
		}
		USBDmaInfo[USB_D0FIFO].fifo 	= D0_FIFO_DMA;
		USBDmaInfo[USB_D0FIFO].dir 		= USB_BUF2FIFO;
		USBDmaInfo[USB_D0FIFO].buffer	= (USB_U32)USBDataPointer[pipe];
		USBDmaInfo[USB_D0FIFO].bytes 	= count;
		USBDmaInfo[USB_D0FIFO].size		= usb_com_get_dmasize(count,
												(USB_U32)USBDmaInfo[USB_D0FIFO].buffer);
		usb_userdef_start_dma(&USBDmaInfo[USB_D0FIFO]);

		USBDataCount[pipe] = 0;
		USBDataPointer[pipe] += count;

		USB.D0FIFOSEL.BIT.MBW = USBDmaInfo[USB_D0FIFO].size;

		/* DMA transfer enable */
		USB.D0FIFOSEL.BIT.DREQE = 1;

		status = WRITEDMA;							/* DMA write  */
	}
	else{
		if( USB.D0FIFOCTR.BIT.BVAL == 0 ) {
			USB.D0FIFOCTR.BIT.BVAL =  1;			/* Short Packet */
		}
		status = WRITESHRT;							/* Short Packet is end of write */
	}

	return	status;									/* End or Err or Continue */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer DMA Write D1FIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static USB_U16 usb_write_dma_d1(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファにデータを書き込みます。
*             : D1FIFOを使用し、DMA転送にてデータを書き込みます。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe     ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: WRITEEND          : Write end
*             : WRITESHRT         : short data
*             : WRITING           : Continue of data write
*             : WRITEDMA          : Write DMA
*             : FIFOERROR         : FIFO status
*----------------------------------------------------------------------------- 
* Notice      : 使用するDMA-chはusb_userdef_start_dma()にて指定します。
*             : 
*""FUNC COMMENT END""*********************************************************/
static USB_U16 usb_write_dma_d1(USB_U16 pipe)
{
	USB_U32 size, count, buffer, mxps;
	USB_U16 status, mbw;

	mbw = usb_get_mbw( USBDataCount[pipe], (USB_U32)USBDataPointer[pipe]);
	buffer = usb_change_fifo_port(pipe,D1DMA,USB_NO, mbw);
	if(buffer==FIFOERROR) {								/* FIFO access status */
		return	FIFOERROR;
	}
	size  = usb_get_buf_size(pipe);						/* Data buffer size */
	mxps  = usb_get_mxps(pipe);							/* Max Packet Size */
	count = USBDataCount[pipe];

	if( count != 0){
		USBDmaPipe[USB_D1FIFO]		= pipe;
		if((count % size ) != 0){
			USBDmaVal[USB_D1FIFO]	= 1;
		}
		else{
			USBDmaVal[USB_D1FIFO]	= 0;
		}
		USBDmaInfo[USB_D1FIFO].fifo 	= D1_FIFO_DMA;
		USBDmaInfo[USB_D1FIFO].dir 		= USB_BUF2FIFO;
		USBDmaInfo[USB_D1FIFO].buffer 	= (USB_U32)USBDataPointer[pipe];
		USBDmaInfo[USB_D1FIFO].bytes 	= count;
		USBDmaInfo[USB_D1FIFO].size 	= usb_com_get_dmasize(count,
												(USB_U32)USBDmaInfo[USB_D1FIFO].buffer);
		usb_userdef_start_dma(&USBDmaInfo[USB_D1FIFO]);

		USBDataCount[pipe] = 0;
		USBDataPointer[pipe] += count;

		USB.D1FIFOSEL.BIT.MBW = USBDmaInfo[USB_D1FIFO].size;

		/* DMA transfer enable */
		USB.D1FIFOSEL.BIT.DREQE = 1;

		status = WRITEDMA;							/* DMA write  */
	}
	else{
		if( USB.D1FIFOCTR.BIT.BVAL == 0 ) {
			USB.D1FIFOCTR.BIT.BVAL =  1;			/* Short Packet */
		}
		status = WRITESHRT;							/* Short Packet is end of write */
	}

	return	status;									/* End or Err or Continue */
}
#endif
/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Start OUT Transfer (receive data from host).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_start_recieve_transfer(USB_U16 pipe, USB_U32 size, USB_U8 *data);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプを使用してUSBデータ通信(受信)を開始します。
*             : 使用するFIFOはパイプ定義テーブルにて設定したFIFOを使用します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe     ; Pipe Number
*             : USB_U32 size      ; Data Size
*             : USB_U8  *data     ; Data data Address
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_start_recieve_transfer(USB_U16 pipe, USB_U32 size, USB_U8 *data)
{
	USB_U16 usefifo;

	usefifo = (USB_U16)(PipeTbl[pipe] & FIFO_USE);
	switch(usefifo){
		case	D0_FIFO_USE:
			usb_start_recieve_trns_d0(pipe, size, data);
			break;
		case	D1_FIFO_USE:
			usb_start_recieve_trns_d1(pipe, size, data);
			break;
//		case	D0_FIFO_DMA:
//			usb_start_recieve_dma_d0(pipe, size, data);
//			break;
//		case	D1_FIFO_DMA:
//			usb_start_recieve_dma_d1(pipe, size, data);
//			break;
		default:
			usb_start_recieve_trns_c(pipe, size, data);
			break;
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Receive Data start CFIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void usb_start_recieve_trns_c(USB_U16 pipe, USB_U32 size,
*             :                                      USB_U8 *data);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファからデータ
*             : 読み出しを開始します。
*             : CFIFOを使用し、CPU転送にてデータを読み出します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe     ; Pipe Number
*             : USB_U32 size      ; Data Size
*             : USB_U8  *data     ; Data data Address
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数ではバッファからデータ読み出しを行いません。
*             : 引数にて指定したパイプに割り当てられているバッファにデータが
*             : 格納されるとBRDY割り込みが発生し、割り込み処理内にてバッファ
*             : からデータ読み出しが行われます。
*""FUNC COMMENT END""*********************************************************/
static void usb_start_recieve_trns_c(USB_U16 pipe, USB_U32 size, USB_U8 *data)
{
	USB_U16 mbw;

	usb_set_pid_nak(pipe);							/* Set NAK */
	USBDataCount[pipe] = size;
	USBDataPointer[pipe] = (USB_U8 *)data;
	PipeIgnore[pipe]	= 0;						/* Ignore count clear */

	USBRcvDataCount[pipe] = size;
	PipeDataSize[pipe] = size;
	PipeFlag[pipe] = PIPE_WAIT;

	mbw = usb_get_mbw( size, (USB_U32)data);
	usb_set_curpipe(pipe, CUSE, USB_NO, mbw);
	USB.CFIFOCTR.WORD = BITBCLR;					/* Buffer Clear */

	usb_set_transaction_counter( pipe, size );

	usb_set_pid_buf(pipe);							/* Set BUF */
	usb_enable_brdy_int(pipe);						/* Enable Ready Interrupt */
	usb_enable_nrdy_int(pipe);						/* Error (NORES or STALL) */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Receive Data start D0FIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void usb_start_recieve_trns_d0(USB_U16 pipe, USB_U32 size,
*             :                                       USB_U8 *data);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファからデータ
*             : 読み出しを開始します。
*             : D0FIFOを使用し、CPU転送にてデータを読み出します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe     ; Pipe Number
*             : USB_U32 size      ; Data Size
*             : USB_U8  *data     ; Data data Address
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数ではバッファからデータ読み出しを行いません。
*             : 引数にて指定したパイプに割り当てられているバッファにデータが
*             : 格納されるとBRDY割り込みが発生し、割り込み処理内にてバッファ
*             : からデータ読み出しが行われます。
*""FUNC COMMENT END""*********************************************************/
static void usb_start_recieve_trns_d0(USB_U16 pipe, USB_U32 size, USB_U8 *data)
{
	USB_U16 mbw;

	usb_set_pid_nak(pipe);							/* Set NAK */
	USBDataCount[pipe] = size;
	USBDataPointer[pipe] = (USB_U8 *)data;
	USBRcvDataCount[pipe] = size;
	PipeDataSize[pipe] = size;
	PipeFlag[pipe] = PIPE_WAIT;
	PipeIgnore[pipe]	= 0;						/* Ignore count clear */

	mbw = usb_get_mbw( size, ( USB_U32)data );
	usb_set_curpipe(pipe, D0USE, USB_NO, mbw);

	usb_set_transaction_counter( pipe, size );

	usb_set_pid_buf(pipe);							/* Set BUF */
	usb_enable_brdy_int(pipe);						/* Enable Ready Interrupt */
	usb_enable_nrdy_int(pipe);						/* Error (NORES or STALL) */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Receive Data start D1FIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void usb_start_recieve_trns_d1(USB_U16 pipe, USB_U32 size,
*             :                                       USB_U8 *data);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファからデータ
*             : 読み出しを開始します。
*             : D1FIFOを使用し、CPU転送にてデータを読み出します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe     ; Pipe Number
*             : USB_U32 size      ; Data Size
*             : USB_U8  *data     ; Data data Address
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数ではバッファからデータ読み出しを行いません。
*             : 引数にて指定したパイプに割り当てられているバッファにデータが
*             : 格納されるとBRDY割り込みが発生し、割り込み処理内にてバッファ
*             : からデータ読み出しが行われます。
*""FUNC COMMENT END""*********************************************************/
static void usb_start_recieve_trns_d1(USB_U16 pipe, USB_U32 size, USB_U8 *data)
{
	USB_U16 mbw;

	usb_set_pid_nak(pipe);							/* Set NAK */
	USBDataCount[pipe] = size;
	USBDataPointer[pipe] = (USB_U8 *)data;
	USBRcvDataCount[pipe] = size;
	PipeDataSize[pipe] = size;
	PipeFlag[pipe] = PIPE_WAIT;
	PipeIgnore[pipe]	= 0;						/* Ignore count clear */

	mbw = usb_get_mbw( size, ( USB_U32)data );
	usb_set_curpipe(pipe, D1USE, USB_NO, mbw);

	usb_set_transaction_counter( pipe, size );

	usb_set_pid_buf(pipe);							/* Set BUF */
	usb_enable_brdy_int(pipe);						/* Enable Ready Interrupt */
	usb_enable_nrdy_int(pipe);						/* Error (NORES or STALL) */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Receive Data DMA D0FIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void usb_start_recieve_dma_d0(USB_U16 pipe, USB_U32 size,
*             :                                      USB_U8 *data);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファからデータ
*             : 読み出しを開始します。
*             : D0FIFOを使用し、DMA転送にてデータを読み出します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe     ; Pipe Number
*             : USB_U32 size      ; Data Size
*             : USB_U8  *data     ; Data data Address
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数ではバッファからデータ読み出しを行いません。
*             : 引数にて指定したパイプに割り当てられているバッファにデータが
*             : 格納されるとDMACに読み出し要求が通知され、DMACによりバッファ
*             : からデータ読み出しが行われます。
*""FUNC COMMENT END""*********************************************************/
#if 0
static void usb_start_recieve_dma_d0(USB_U16 pipe, USB_U32 size, USB_U8 *data)
{
	USB_U16 mbw;

	usb_set_pid_nak(pipe);							/* Set NAK */
	USBDataCount[pipe] = size;
	USBDataPointer[pipe] = (USB_U8 *)data;
	USBRcvDataCount[pipe] = size;
	PipeDataSize[pipe] = size;
	PipeFlag[pipe] = PIPE_WAIT;
	PipeIgnore[pipe]	= 0;						/* Ignore count clear */

	mbw = usb_get_mbw( size, (USB_U32)data);
	usb_set_curpipe(pipe, D0USE, USB_NO, mbw);

	usb_set_transaction_counter( pipe, size );

	usb_set_pid_buf(pipe);							/* Set BUF */
	usb_enable_brdy_int(pipe);						/* Enable Ready Interrupt */
	usb_enable_nrdy_int(pipe);						/* Error (NORES or STALL) */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Receive Data DMA D0FIF1
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void usb_start_recieve_dma_d1(USB_U16 pipe, USB_U32 size,
*             :                                      USB_U8 *data);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファからデータ
*             : 読み出しを開始します。
*             : D0FIFOを使用し、DMA転送にてデータを読み出します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe     ; Pipe Number
*             : USB_U32 size      ; Data Size
*             : USB_U8  *data     ; Data data Address
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数ではバッファからデータ読み出しを行いません。
*             : 引数にて指定したパイプに割り当てられているバッファにデータが
*             : 格納されるとDMACに読み出し要求が通知され、DMACによりバッファ
*             : からデータ読み出しが行われます。
*""FUNC COMMENT END""*********************************************************/
static void usb_start_recieve_dma_d1(USB_U16 pipe, USB_U32 size, USB_U8 *data)
{
	USB_U16 mbw;

	usb_set_pid_nak(pipe);							/* Set NAK */
	USBDataCount[pipe] = size;
	USBDataPointer[pipe] = (USB_U8 *)data;
	USBRcvDataCount[pipe] = size;
	PipeDataSize[pipe] = size;
	PipeFlag[pipe] = PIPE_WAIT;
	PipeIgnore[pipe]	= 0;						/* Ignore count clear */

	mbw = usb_get_mbw( size, (USB_U32)data);
	usb_set_curpipe(pipe, D1USE, USB_NO, mbw );

	usb_set_transaction_counter( pipe, size );

	usb_set_pid_buf(pipe);							/* Set BUF */
	usb_enable_brdy_int(pipe);						/* Enable Ready Interrupt */
	usb_enable_nrdy_int(pipe);						/* Error (NORES or STALL) */
}
#endif
/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer Read
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_read_buffer(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファからデータを
*             : 読み出します。
*             : 使用するFIFOはパイプ定義テーブルにて設定したFIFOを使用します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe     ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: READEND          ; Read end
*             : READSHRT         ; short data
*             : READING          ; Continue of data read
*             : READOVER         ; buffer over
*             : FIFOERROR        ; FIFO status
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_read_buffer(USB_U16 pipe)
{
	USB_U16	status;

	PipeIgnore[pipe]	= 0;							/* Ignore count clear */
	if( (PipeTbl[pipe] & FIFO_USE) == D0_FIFO_USE ) {
		status	= usb_read_buffer_d0(pipe);
	} 
	else if( (PipeTbl[pipe] & FIFO_USE) == D1_FIFO_USE ) {
		status	= usb_read_buffer_d1(pipe);
	} 
	else {
		status	= usb_read_buffer_c(pipe);
	}

	switch( status ) {
	case	READING:									/* Continue of data read */
		break;
	case	READEND:									/* End of data read */
	case	READSHRT:									/* End of data read */
		usb_disable_brdy_int(pipe);
		PipeFlag[pipe] = PIPE_DONE;
		PipeDataSize[pipe] -= USBDataCount[pipe];
		USBRcvDataCount[pipe] -= USBDataCount[pipe];	/* Data Count */
		break;
	case	READOVER:									/* buffer over */
		USB.CFIFOCTR.WORD = BITBCLR;					/* Clear BVAL */
		usb_disable_brdy_int(pipe);						/* Disable Ready Interrupt */
		PipeFlag[pipe] = PIPE_IDLE;
		PipeDataSize[pipe] -= USBDataCount[pipe];
		USBRcvDataCount[pipe] -= USBDataCount[pipe];	/* Data Count */
		break;
	case	FIFOERROR:									/* FIFO access status */
	default:
		usb_disable_brdy_int(pipe);						/* Disable Ready Interrupt */
		PipeFlag[pipe] = PIPE_IDLE;
		break;
	}
	return	status;										/* End or Err or Continue */

}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer Read CFIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_read_buffer_c(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファからデータを
*             : 読み出します。
*             : CFIFOを使用し、CPU転送にてデータを読み出します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe     ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: READEND          ; Read end
*             : READSHRT         ; short data
*             : READING          ; Continue of data read
*             : READOVER         ; buffer over
*             : FIFOERROR        ; FIFO status
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_read_buffer_c(USB_U16 pipe)
{
	USB_U16 size, count, buffer, mxps, dtln;
	USB_U16 status, mbw;

	mbw = usb_get_mbw( USBDataCount[pipe], (USB_U32)USBDataPointer[pipe]);
	buffer = usb_change_fifo_port(pipe,CUSE,USB_NO, mbw );
	if(buffer==FIFOERROR) {							/* FIFO access status */
		return	FIFOERROR;
	}
	dtln = (USB_U16)(buffer & BITDTLN);
	size = usb_get_buf_size(pipe);					/* Data buffer size */
	mxps = usb_get_mxps(pipe);						/* Max Packet Size */

	if( USBDataCount[pipe] < dtln ) {				/* Buffer Over ? */
		status = READOVER;
		usb_set_pid_nak(pipe);						/* Set NAK */
		count = (USB_U16)USBDataCount[pipe];
	} 
	else if( USBDataCount[pipe] == dtln ) {			/* just Receive Size */
		status = READEND;
		usb_set_pid_nak(pipe);						/* Set NAK */
		count = dtln;
		if ( count == 0 ) {
			status = READSHRT;						/* Null Packet receive */
		}
		if ( (count%mxps)!=0 ) {
			status = READSHRT;						/* Short Packet receive */
		}
	} 
	else {											/* continus Receive data */
		status = READING;
		count = dtln;
		if ( count == 0 ) {
			status = READSHRT;						/* Null Packet receive */
			usb_set_pid_nak(pipe);					/* Set NAK */
		}
		if ( (count%mxps)!=0 ) {
			status = READSHRT;						/* Short Packet receive */
			usb_set_pid_nak(pipe);					/* Set NAK */
		}
	}

	if( count == 0 ) {								/* 0 length packet */
		USB.CFIFOCTR.WORD = BITBCLR;				/* Clear BVAL */
	} 
	else {
		usb_read_c_fifo(pipe,count);
	}

	USBDataCount[pipe] -= count;
	return	status;									/* End or Err or Continue */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer Read D0FIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_read_buffer_d0(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファからデータを
*             : 読み出します。
*             : D0FIFOを使用し、CPU転送にてデータを読み出します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe     ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: READEND          ; Read end
*             : READSHRT         ; short data
*             : READING          ; Continue of data read
*             : READOVER         ; buffer over
*             : FIFOERROR        ; FIFO status
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_read_buffer_d0(USB_U16 pipe)
{
	USB_U16	size, count, buffer, mxps, dtln;
	USB_U16	status, mbw;

	mbw = usb_get_mbw( USBDataCount[pipe], (USB_U32)USBDataPointer[pipe]);
	buffer	= usb_change_fifo_port(pipe,D0USE,USB_NO, mbw);
	if(buffer==FIFOERROR) {							/* FIFO access status */
		return	FIFOERROR;
	}
	dtln	= (USB_U16)(buffer & BITDTLN);
	size	= usb_get_buf_size(pipe);				/* Data buffer size */
	mxps	= usb_get_mxps(pipe);					/* Max Packet Size */

	if( USBDataCount[pipe] < dtln ) {				/* Buffer Over ? */
		status = READOVER;
		usb_set_pid_nak(pipe);						/* Set NAK */
		count = (USB_U16)USBDataCount[pipe];
	} 
	else if( USBDataCount[pipe] == dtln ) {			/* just Receive Size */
		status = READEND;
		usb_set_pid_nak(pipe);						/* Set NAK */
		count = dtln;
		if ( count == 0 ) {
			status = READSHRT;						/* Null Packet receive */
		}
		if ( (count%mxps)!=0 ) {
			status = READSHRT;						/* Short Packet receive */
		}
	} 
	else {											/* continus Receive data */
		status = READING;
		count = dtln;
		if ( count == 0 ) {
			status = READSHRT;						/* Null Packet receive */
			usb_set_pid_nak(pipe);					/* Set NAK */
		}
		if ( (count%mxps)!=0 ) {
			status = READSHRT;						/* Short Packet receive */
			usb_set_pid_nak(pipe);					/* Set NAK */
		}
	}

	if( count == 0 ) {								/* 0 length packet */
		USB.D0FIFOCTR.WORD = BITBCLR;			/* Clear BVAL */
	} 
	else {
		usb_read_d0_fifo(pipe,count);
	}

	USBDataCount[pipe] -= count;
	return	status;									/* End or Err or Continue */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer Read D1FIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_read_buffer_d1(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファからデータを
*             : 読み出します。
*             : D1FIFOを使用し、CPU転送にてデータを読み出します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe     ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: READEND          ; Read end
*             : READSHRT         ; short data
*             : READING          ; Continue of data read
*             : READOVER         ; buffer over
*             : FIFOERROR        ; FIFO status
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_read_buffer_d1(USB_U16 pipe)
{
	USB_U16	size, count, buffer, mxps, dtln;
	USB_U16	status, mbw;

	mbw = usb_get_mbw( USBDataCount[pipe], (USB_U32)USBDataPointer[pipe]);
	buffer	= usb_change_fifo_port(pipe,D1USE,USB_NO, mbw );
	if(buffer==FIFOERROR) {							/* FIFO access status */
		return	FIFOERROR;
	}
	dtln	= (USB_U16)(buffer & BITDTLN);
	size	= usb_get_buf_size(pipe);				/* Data buffer size */
	mxps	= usb_get_mxps(pipe);					/* Max Packet Size */

	if( USBDataCount[pipe] < dtln ) {				/* Buffer Over ? */
		status = READOVER;
		usb_set_pid_nak(pipe);						/* Set NAK */
		count = (USB_U16)USBDataCount[pipe];
	} 
	else if( USBDataCount[pipe] == dtln ) {			/* just Receive Size */
		status = READEND;
		usb_set_pid_nak(pipe);						/* Set NAK */
		count = dtln;
		if ( count == 0 ) {
			status = READSHRT;						/* Null Packet receive */
		}
		if ( (count%mxps)!=0 ) {
			status = READSHRT;						/* Short Packet receive */
		}
	} 
	else {											/* continus Receive data */
		status = READING;
		count = dtln;
		if ( count == 0 ) {
			status = READSHRT;						/* Null Packet receive */
			usb_set_pid_nak(pipe);					/* Set NAK */
		}
		if ( (count%mxps)!=0 ) {
			status = READSHRT;						/* Short Packet receive */
			usb_set_pid_nak(pipe);					/* Set NAK */
		}
	}

	if( count == 0 ) {								/* 0 length packet */
		USB.D1FIFOCTR.WORD = BITBCLR;			/* Clear BVAL */
	} 
	else {
		usb_read_d1_fifo(pipe,count);
	}

	USBDataCount[pipe] -= count;
	return	status;									/* End or Err or Continue */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer Read
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration: USB_U16 usb_read_dma(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファからデータを
*             : 読み出します。
*             : D0FIFOまたはD1FIFOを使用し、DMA転送にてデータを読み出します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe     ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: READEND          ; Read end
*             : READSHRT         ; short data
*             : READING          ; Continue of data read
*             : READOVER         ; buffer over
*             : FIFOERROR        ; FIFO status
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
#if 0
USB_U16 usb_read_dma(USB_U16 pipe)
{
	USB_U16	status;

	PipeIgnore[pipe]	= 0;							/* Ignore count clear */
	if( (PipeTbl[pipe] & FIFO_USE) == D0_FIFO_DMA ) {
		status	= usb_read_dma_d0(pipe);
	} 
	else {
		status	= usb_read_dma_d1(pipe);
	}

	switch( status ) {
	case	READING:									/* Continue of data read */
		break;
	case	READEND:									/* End of data read */
	case	READSHRT:									/* End of data read */
		usb_disable_brdy_int(pipe);
		PipeDataSize[pipe] -= USBDataCount[pipe];
		USBRcvDataCount[pipe] -= USBDataCount[pipe];	/* Data Count */
		USBDataCount[pipe] = 0;
		break;
	case	READOVER:									/* buffer over */
		usb_disable_brdy_int(pipe);						/* Disable Ready Interrupt */
		PipeDataSize[pipe] -= USBDataCount[pipe];
		USBRcvDataCount[pipe] -= USBDataCount[pipe];	/* Data Count */
		USBDataCount[pipe] = 0;
		break;
	case	FIFOERROR:									/* FIFO access status */
	default:
		usb_disable_brdy_int(pipe);						/* Disable Ready Interrupt */
		PipeFlag[pipe] = PIPE_IDLE;
		break;
	}
	return	status;										/* End or Err or Continue */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer Read DMA D0FIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_read_dma_d0(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファからデータを
*             : 読み出します。
*             : D0FIFOを使用し、DMA転送にてデータを読み出します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe     ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: READEND          ; Read end
*             : READSHRT         ; short data
*             : READING          ; Continue of data read
*             : READOVER         ; buffer over
*             : FIFOERROR        ; FIFO status
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
static USB_U16 usb_read_dma_d0(USB_U16 pipe)
{
	USB_U16	size, count, buffer, mxps, dtln;
	USB_U16	status, mbw;

	mbw = usb_get_mbw( USBDataCount[pipe], (USB_U32)USBDataPointer[pipe]);
	buffer	= usb_change_fifo_port(pipe,D0USE,USB_NO, mbw );
	if(buffer==FIFOERROR) {							/* FIFO access status */
		return	FIFOERROR;
	}
	dtln	= (USB_U16)(buffer & BITDTLN);
	size	= usb_get_buf_size(pipe);				/* Data buffer size */
	mxps	= usb_get_mxps(pipe);					/* Max Packet Size */

	if( USBDataCount[pipe] < dtln ) {				/* Buffer Over ? */
		status = READOVER;
		count = (USB_U16)USBDataCount[pipe];
	} 
	else if( USBDataCount[pipe] == dtln ) {			/* just Receive Size */
		status = READEND;
		count = dtln;
		if ( count == 0 ) {
			status = READSHRT;						/* Null Packet receive */
		}
		if ( (count%mxps)!=0 ) {
			status = READSHRT;						/* Short Packet receive */
		}
	} 
	else {											/* continus Receive data */
		status = READING;
		count = dtln;
		if ( count == 0 ) {
			status = READSHRT;						/* Null Packet receive */
		}
		if ( (count%mxps)!=0 ) {
			status = READSHRT;						/* Short Packet receive */
		}
	}

	if( count == 0 ) {								/* 0 length packet */
		USB.D0FIFOCTR.WORD = BITBCLR;			/* Clear BVAL */
	} 
	else {
		USBDmaPipe[USB_D0FIFO]			= pipe;
		USBDmaVal[USB_D0FIFO]			= 0;
		USBDmaInfo[USB_D0FIFO].fifo 	= D0_FIFO_DMA;
		USBDmaInfo[USB_D0FIFO].dir 		= USB_FIFO2BUF;
		USBDmaInfo[USB_D0FIFO].buffer 	= (USB_U32)USBDataPointer[pipe];
		USBDmaInfo[USB_D0FIFO].size 	= usb_com_get_dmasize((USB_U32)USBDataCount[pipe],
												(USB_U32)USBDmaInfo[USB_D0FIFO].buffer);
		if(USBDmaInfo[USB_D0FIFO].size == 0){
			USBDmaInfo[USB_D0FIFO].bytes 	= count;
		}
		else if(USBDmaInfo[USB_D0FIFO].size == 1){
			USBDmaInfo[USB_D0FIFO].bytes 	= ((count+1)/2)*2;
		}
		else if(USBDmaInfo[USB_D0FIFO].size == 2){
			USBDmaInfo[USB_D0FIFO].bytes 	= ((count+3)/4)*4;
		}

		USBDmaStatus[USB_D0FIFO] = USB_DMA_BUSY;

		usb_userdef_start_dma(&USBDmaInfo[USB_D0FIFO]);

		USB.D0FIFOSEL.BIT.MBW = USBDmaInfo[USB_D0FIFO].size;
		/* DMA transfer enable */
		USB.D0FIFOSEL.BIT.DREQE = 1;
	}

	USBDataCount[pipe] -= count;
	return	status;									/* End or Err or Continue */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer Read DMA D1FIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_read_dma_d1(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに割り当てられているバッファからデータを
*             : 読み出します。
*             : D1FIFOを使用し、DMA転送にてデータを読み出します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe     ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: READEND          ; Read end
*             : READSHRT         ; short data
*             : READING          ; Continue of data read
*             : READOVER         ; buffer over
*             : FIFOERROR        ; FIFO status
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
static USB_U16 usb_read_dma_d1(USB_U16 pipe)
{
	USB_U16	size, count, buffer, mxps, dtln;
	USB_U16	status, mbw;

	mbw = usb_get_mbw( USBDataCount[pipe], (USB_U32)USBDataPointer[pipe]);
	buffer	= usb_change_fifo_port(pipe,D1USE,USB_NO, mbw);
	if(buffer==FIFOERROR) {							/* FIFO access status */
		return	FIFOERROR;
	}
	dtln	= (USB_U16)(buffer & BITDTLN);
	size	= usb_get_buf_size(pipe);				/* Data buffer size */
	mxps	= usb_get_mxps(pipe);					/* Max Packet Size */

	if( USBDataCount[pipe] < dtln ) {				/* Buffer Over ? */
		status = READOVER;
		count = (USB_U16)USBDataCount[pipe];
	} 
	else if( USBDataCount[pipe] == dtln ) {			/* just Receive Size */
		status = READEND;
		count = dtln;
		if ( count == 0 ) {
			status = READSHRT;						/* Null Packet receive */
		}
		if ( (count%mxps)!=0 ) {
			status = READSHRT;						/* Short Packet receive */
		}
	} 
	else {											/* continus Receive data */
		status = READING;
		count = dtln;
		if ( count == 0 ) {
			status = READSHRT;						/* Null Packet receive */
		}
		if ( (count%mxps)!=0 ) {
			status = READSHRT;						/* Short Packet receive */
		}
	}

	if( count == 0 ) {								/* 0 length packet */
		USB.D1FIFOCTR.WORD = BITBCLR;			/* Clear BVAL */
	} 
	else {
		USBDmaPipe[USB_D1FIFO]			= pipe;
		USBDmaVal[USB_D1FIFO]			= 0;
		USBDmaInfo[USB_D1FIFO].fifo 	= D1_FIFO_DMA;
		USBDmaInfo[USB_D1FIFO].dir 		= USB_FIFO2BUF;
		USBDmaInfo[USB_D1FIFO].buffer 	= (USB_U32)USBDataPointer[pipe];
		USBDmaInfo[USB_D1FIFO].size		= usb_com_get_dmasize((USB_U32)USBDataCount[pipe],
												(USB_U32)USBDmaInfo[USB_D1FIFO].buffer);
		if(USBDmaInfo[USB_D1FIFO].size == 0){
			USBDmaInfo[USB_D1FIFO].bytes 	= count;
		}
		else if(USBDmaInfo[USB_D1FIFO].size == 1){
			USBDmaInfo[USB_D1FIFO].bytes 	= ((count+1)/2)*2;
		}
		else if(USBDmaInfo[USB_D1FIFO].size == 2){
			USBDmaInfo[USB_D1FIFO].bytes 	= ((count+3)/4)*4;
		}

		USBDmaStatus[USB_D1FIFO] = USB_DMA_BUSY;

		usb_userdef_start_dma(&USBDmaInfo[USB_D1FIFO]);

		USB.D1FIFOSEL.BIT.MBW = USBDmaInfo[USB_D1FIFO].size;
		/* DMA transfer enable */
		USB.D1FIFOSEL.BIT.DREQE = 1;
	}

	USBDataCount[pipe] -= count;
	return	status;									/* End or Err or Continue */
}
#endif
/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : pipe Change
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_change_fifo_port(USB_U16 pipe, USB_U16 fifosel,
*             :                              USB_U16 isel, USB_U16 mbw);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに引数にて指定したFIFOを割り当てます。
*             : FIFO割り当て後、当該パイプがレディーとなるまでソフトウェアにて
*             : ウェイトします。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe     ; Pipe Number
*             : USB_U16 fifosel   ; Select FIFO
*             : USB_U16 isel      ; FIFO Access Direction
*             : USB_U16 mbw       ; FIFO Port Access Bit Width
*----------------------------------------------------------------------------- 
* Return Value: FIFOERROR         ; Error
*             : Others            ; CFIFOCTR/D0FIFOCTR/D1FIFOCTR Register Value
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_change_fifo_port(USB_U16 pipe, USB_U16 fifosel, USB_U16 isel, USB_U16 mbw )
{
	USB_U16 buffer;
	int i;
	volatile int j;			/* 1:5nsec wait count @200MHz */

	usb_set_curpipe(pipe,fifosel,isel, mbw );

	for(i=0;i<4;i++) {
		switch(fifosel){
		case	CUSE:
			buffer = USB.CFIFOCTR.WORD;
			break;
		case	D0USE:
//		case	D0DMA:
			buffer = USB.D0FIFOCTR.WORD;
			break;
		case	D1USE:
//		case	D1DMA:
			buffer = USB.D1FIFOCTR.WORD;
			break;
		default:
			buffer	= 0;
			break;
		}
		if((USB_U16)(buffer & BITFRDY) == BITFRDY) {
			return	buffer;
		}
		j = 25;
		while(j-- > 0){
		/* wait */
		}
	}
	return	FIFOERROR;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : set current pipe
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_set_curpipe(USB_U16 pipe, USB_U16 fifosel,
*             :                      USB_U16 isel, USB_U16 mbw);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプに引数にて指定したFIFOを割り当てます。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe     ; Pipe Number
*             : USB_U16 fifosel   ; Select FIFO
*             : USB_U16 isel      ; FIFO Access Direction
*             : USB_U16 mbw       ; FIFO Port Access Bit Width
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_set_curpipe(USB_U16 pipe, USB_U16 fifosel, USB_U16 isel, USB_U16 mbw )
{
	USB_U16	buffer;
	int	i;
	volatile int j;			/* 1:5nsec wait count @200MHz */

	USBmbw[ pipe ] = mbw;

	switch(fifosel){
	case	CUSE:
		buffer = USB.CFIFOSEL.WORD;
		buffer &= (USB_U16)~(BITISEL | BITCURPIPE );
		buffer |= (USB_U16)(~isel & BITISEL );
		USB.CFIFOSEL.WORD = buffer;			/* ISEL = ~isel CURPIPE=0 */
		for(i=0;i<4;i++) {
			if( ( USB.CFIFOSEL.WORD & (BITISEL | BITCURPIPE ) ) == 
								( buffer & ( BITISEL | BITCURPIPE ) ) ){
				break;
			}
			j = 100;
			while(j-- > 0){
				/* wait */
			}
		}
		buffer &= (USB_U16)~(BITISEL | BITCURPIPE | BITMBW );
		buffer |= (USB_U16)(isel | pipe | mbw );
		USB.CFIFOSEL.WORD = buffer;				/* ISEL= isel, CURPIPE=pipe */
		for(i=0;i<4;i++) {
			if( ( USB.CFIFOSEL.WORD & (BITISEL | BITCURPIPE ) ) == 
								( buffer & ( BITISEL | BITCURPIPE ) ) ){
				break;
			}
			j = 100;
			while(j-- > 0){
				/* wait */
			}
		}
		break;
	case	D0USE:
//	case	D0DMA:
		buffer = USB.D0FIFOSEL.WORD;
		buffer &= (USB_U16)~(BITCURPIPE );
		USB.D0FIFOSEL.WORD = buffer;			/* CURPIPE=0 */
		for(i=0;i<4;i++) {
			if( ( USB.D0FIFOSEL.WORD & BITCURPIPE ) == ( buffer & BITCURPIPE ) ){
				break;
			}
			j = 100;
			while(j-- > 0){
				/* wait */
			}
		}
		buffer &= (USB_U16)~(BITCURPIPE | BITMBW );
		buffer |= (USB_U16)(pipe | mbw );
		USB.D0FIFOSEL.WORD = buffer;
		for(i=0;i<4;i++) {
			if( ( USB.D0FIFOSEL.WORD & BITCURPIPE ) == ( buffer & BITCURPIPE ) ){
				break;
			}
			j = 100;
			while(j-- > 0){
				/* wait */
			}
		}
		break;
	case	D1USE:
//	case	D1DMA:
		buffer = USB.D1FIFOSEL.WORD;
		buffer &= (USB_U16)~( BITCURPIPE );
		USB.D1FIFOSEL.WORD = buffer;			/* CURPIPE=0 */
		for(i=0;i<4;i++) {
			if( ( USB.D1FIFOSEL.WORD & BITCURPIPE ) == ( buffer & BITCURPIPE ) ){
				break;
			}
			j = 100;
			while(j-- > 0){
				/* wait */
			}
		}
		buffer &= (USB_U16)~( BITCURPIPE | BITMBW );
		buffer |= (USB_U16)( pipe | mbw );
		USB.D1FIFOSEL.WORD = buffer;
		for(i=0;i<4;i++) {
			if( ( USB.D1FIFOSEL.WORD & BITCURPIPE ) == ( buffer & BITCURPIPE ) ){
				break;
			}
			j = 100;
			while(j-- > 0){
				/* wait */
			}
		}
		break;
	default:
		break;
	}
	/* Cautions !!!
	 * Depending on the external bus speed of CPU, you may need to wait for 450ns here.
	 * For details, please look at the data sheet.	 */
	j = 100;
	while(j-- > 0){
		/* wait */
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : CFIFO write
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void usb_write_c_fifo(USB_U16 pipe,USB_U16 count);
*----------------------------------------------------------------------------- 
* Description : CFIFOにデータを書き込みます。
*             : 引数にて指定されたパイプに対するアクセスサイズに応じて
*             : BYTE/WORD/LONGにてデータを書き込みます。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe     ; Pipe Number
*             : USB_U16 count     ; Data Size(Byte)
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数実行前に引数にて指定されたパイプへのCFIFOの割付が完了している
*             : 必要があります。
*             : 本関数実行前に引数にて指定されてパイプに対するアクセスサイズが決定し、
*             : USBmbw[]に設定が完了している必要があります。
*""FUNC COMMENT END""*********************************************************/
static void usb_write_c_fifo(USB_U16 pipe,USB_U16 count)
{
	register USB_U16 even;
	
	if(USBmbw[ pipe ] == BITMBW_8){
		for( even = count; even; --even ) {
			USB.CFIFO.BYTE = *USBDataPointer[pipe];
			USBDataPointer[pipe] += 1;
		}
	}
	else if(USBmbw[ pipe ] == BITMBW_16){
		for( even = (USB_U16)(count/2); even; --even ) {
			USB.CFIFO.WORD = *((USB_U16 *)USBDataPointer[pipe]);
			USBDataPointer[pipe] += 2;
		}
	}
	else{
		for( even = (USB_U16)(count/4); even; --even ) {
			USB.CFIFO.LONG = *((USB_U32 *)USBDataPointer[pipe]);
			USBDataPointer[pipe] += 4;
		}
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : CFIFO read
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void usb_read_c_fifo(USB_U16 pipe,USB_U16 count);
*----------------------------------------------------------------------------- 
* Description : CFIFOからデータを読み出します。
*             : 引数にて指定されたパイプに対するアクセスサイズに応じて
*             : BYTE/WORD/LONGにてデータを読み出します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe     ; Pipe Number
*             : USB_U16 count     ; Data Size(Byte)
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数実行前に引数にて指定されたパイプへのCFIFOの割付が完了している
*             : 必要があります。
*             : 本関数実行前に引数にて指定されてパイプに対するアクセスサイズが決定し、
*             : USBmbw[]に設定が完了している必要があります。
*""FUNC COMMENT END""*********************************************************/
static void usb_read_c_fifo(USB_U16 pipe,USB_U16 count)
{
	register USB_U16	even;
	
	if(USBmbw[ pipe ] == BITMBW_8){
		for( even = count; even; --even ) {
			*USBDataPointer[pipe] = USB.CFIFO.BYTE;
			USBDataPointer[pipe] += 1;
		}
	}
	else if(USBmbw[ pipe ] == BITMBW_16){
		for( even = (USB_U16)((count+1)/2); even; --even ) {
			*((USB_U16 *)USBDataPointer[pipe]) = USB.CFIFO.WORD;
			USBDataPointer[pipe] += 2;
		}
	}
	else{
		for( even = (USB_U16)((count+3)/4); even; --even ) {
			*((USB_U32 *)USBDataPointer[pipe]) = USB.CFIFO.LONG;
			USBDataPointer[pipe] += 4;
		}
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : D0FIFO write
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void usb_write_d0_fifo(USB_U16 pipe,USB_U16 count);
*----------------------------------------------------------------------------- 
* Description : D0FIFOにデータを書き込みます。
*             : 引数にて指定されたパイプに対するアクセスサイズに応じて
*             : BYTE/WORD/LONGにてデータを書き込みます。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe     ; Pipe Number
*             : USB_U16 count     ; Data Size(Byte)
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数実行前に引数にて指定されたパイプへのD0FIFOの割付が完了している
*             : 必要があります。
*             : 本関数実行前に引数にて指定されてパイプに対するアクセスサイズが決定し、
*             : USBmbw[]に設定が完了している必要があります。
*""FUNC COMMENT END""*********************************************************/
static void usb_write_d0_fifo(USB_U16 pipe,USB_U16 count)
{
	register USB_U16	even;
	
	if(USBmbw[ pipe ] == BITMBW_8){
		for( even = count; even; --even ) {
			USB.D0FIFO.BYTE = *USBDataPointer[pipe];
			USBDataPointer[pipe] += 1;
		}
	}
	else if(USBmbw[ pipe ] == BITMBW_16){
		for( even = (USB_U16)(count/2); even; --even ) {
			USB.D0FIFO.WORD = *((USB_U16 *)USBDataPointer[pipe]);
			USBDataPointer[pipe] += 2;
		}
	}
	else{
		for( even = (USB_U16)(count/4); even; --even ) {
			USB.D0FIFO.LONG = *((USB_U32 *)USBDataPointer[pipe]);
			USBDataPointer[pipe] += 4;
		}
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : D0FIFO read
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void usb_read_d0_fifo(USB_U16 pipe,USB_U16 count);
*----------------------------------------------------------------------------- 
* Description : D0FIFOからデータを読み出します。
*             : 引数にて指定されたパイプに対するアクセスサイズに応じて
*             : BYTE/WORD/LONGにてデータを読み出します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  Pipe     ; Pipe Number
*             : USB_U16 count     ; Data Size(Byte)
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数実行前に引数にて指定されたパイプへのD0FIFOの割付が完了している
*             : 必要があります。
*             : 本関数実行前に引数にて指定されてパイプに対するアクセスサイズが決定し、
*             : USBmbw[]に設定が完了している必要があります。
*""FUNC COMMENT END""*********************************************************/
static void usb_read_d0_fifo(USB_U16 pipe,USB_U16 count)
{
	register USB_U16	even;
	
	if(USBmbw[ pipe ] == BITMBW_8){
		for( even = count; even; --even ) {
			*USBDataPointer[pipe] = USB.D0FIFO.BYTE;
			USBDataPointer[pipe] += 1;
		}
	}
	else if(USBmbw[ pipe ] == BITMBW_16){
		for( even = (USB_U16)((count+1)/2); even; --even ) {
			*((USB_U16 *)USBDataPointer[pipe]) = USB.D0FIFO.WORD;
			USBDataPointer[pipe] += 2;
		}
	}
	else{
		for( even = (USB_U16)((count+3)/4); even; --even ) {
			*((USB_U32 *)USBDataPointer[pipe]) = USB.D0FIFO.LONG;
			USBDataPointer[pipe] += 4;
		}
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : D1FIFO write
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void usb_write_d1_fifo(USB_U16 pipe,USB_U16 count);
*----------------------------------------------------------------------------- 
* Description : D1FIFOにデータを書き込みます。
*             : 引数にて指定されたパイプに対するアクセスサイズに応じて
*             : BYTE/WORD/LONGにてデータを書き込みます。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe     ; Pipe Number
*             : USB_U16 count     ; Data Size(Byte)
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数実行前に引数にて指定されたパイプへのD1FIFOの割付が完了している
*             : 必要があります。
*             : 本関数実行前に引数にて指定されてパイプに対するアクセスサイズが決定し、
*             : USBmbw[]に設定が完了している必要があります。
*""FUNC COMMENT END""*********************************************************/
static void usb_write_d1_fifo(USB_U16 pipe,USB_U16 count)
{
	register USB_U16	even;
	
	if(USBmbw[ pipe ] == BITMBW_8){
		for( even = count; even; --even ) {
			USB.D1FIFO.BYTE = *USBDataPointer[pipe];
			USBDataPointer[pipe] += 1;
		}
	}
	else if(USBmbw[ pipe ] == BITMBW_16){
		for( even = (USB_U16)(count/2); even; --even ) {
			USB.D1FIFO.WORD = *((USB_U16 *)USBDataPointer[pipe]);
			USBDataPointer[pipe] += 2;
		}
	}
	else{
		for( even = (USB_U16)(count/4); even; --even ) {
			USB.D1FIFO.LONG = *((USB_U32 *)USBDataPointer[pipe]);
			USBDataPointer[pipe] += 4;
		}
	}
}
/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : D1FIFO read
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void usb_read_d1_fifo(USB_U16 pipe,USB_U16 count);
*----------------------------------------------------------------------------- 
* Description : D1FIFOからデータを読み出します。
*             : 引数にて指定されたパイプに対するアクセスサイズに応じて
*             : BYTE/WORD/LONGにてデータを読み出します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe     ; Pipe Number
*             : USB_U16 count     ; Data Size(Byte)
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数実行前に引数にて指定されたパイプへのD1FIFOの割付が完了している
*             : 必要があります。
*             : 本関数実行前に引数にて指定されてパイプに対するアクセスサイズが決定し、
*             : USBmbw[]に設定が完了している必要があります。
*""FUNC COMMENT END""*********************************************************/
static void usb_read_d1_fifo(USB_U16 pipe,USB_U16 count)
{
	register USB_U16 even;
	
	if( USBmbw[ pipe ] == BITMBW_8 ){
		for( even = count; even; --even ) {
			*USBDataPointer[pipe] = USB.D1FIFO.BYTE;
			USBDataPointer[pipe] += 1;
		}
	}
	else if(USBmbw[ pipe ] == BITMBW_16){
		for( even = (USB_U16)((count+1)/2); even; --even ) {
			*((USB_U16 *)USBDataPointer[pipe]) = USB.D1FIFO.WORD;
			USBDataPointer[pipe] += 2;
		}
	}
	else{
		for( even = (USB_U16)((count+3)/4); even; --even ) {
			*((USB_U32 *)USBDataPointer[pipe]) = USB.D1FIFO.LONG;
			USBDataPointer[pipe] += 4;
		}
	}
}

/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     : DMA Transfer Size
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : static USB_U32 usb_com_get_dmasize(USB_U32 trncount,USB_U32 dtptr);
*-----------------------------------------------------------------------------
* Description : 引数によりDMA転送のアクセス幅を計算し、戻り値として返します。
*-----------------------------------------------------------------------------
* Argument    : USB_U32 trncount   : transfer byte
*             : USB_U32 dtptr      : transfer data pointer
*-----------------------------------------------------------------------------
* Return Value: DMA transfer size    : 0   8bit
*             :                      : 1  16bit
*             :                      : 2  32bit
*-----------------------------------------------------------------------------
* Note        : 
*""FUNC COMMENT END""********************************************************/
static USB_U32	usb_com_get_dmasize(USB_U32 trncount,USB_U32 dtptr)
{
	USB_U32 size;
	
	
	if( ((trncount & 0x0001) != 0) || ((dtptr & 0x00000001) != 0) ) {
		/*  When transfer byte count is odd 		*/
		/* or transfer data area is 8-bit alignment */
		size = 0;			/* 8bit */
	}
	else if( ((trncount & 0x0003) != 0) || ((dtptr & 0x00000003) != 0) ) {
		/* When the transfer byte count is multiples of 2 */
		/* or the transfer data area is 16-bit alignment */
		size = 1;			/* 16bit */
	}
	else {
		/* When the transfer byte count is multiples of 4 */
		/* or the transfer data area is 32-bit alignment */
		size = 2;			/* 32bit */
	}
	return size;
}
/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     : FIFO Transfer Size
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : static USB_U32 usb_get_mbw(USB_U32 trncount,USB_U32 dtptr);
*-----------------------------------------------------------------------------
* Description : 引数によりDMA転送のアクセス幅を計算し、MBWに設定する値を
*             : 戻り値として返します。
*-----------------------------------------------------------------------------
* Argument    : USB_U32 trncount   : transfer byte
*             : USB_U32 dtptr      : transfer data pointer
*-----------------------------------------------------------------------------
* Return Value: FIFO transfer size   : BITMBW_8    8bit
*             :                      : BITMBW_16  16bit
*             :                      : BITMBW_32  32bit
*-----------------------------------------------------------------------------
* Note        : 
*""FUNC COMMENT END""********************************************************/
USB_U16	usb_get_mbw(USB_U32 trncount,USB_U32 dtptr)
{
	USB_U32 size;
	USB_U16 mbw;
	
	size = usb_com_get_dmasize(trncount, dtptr);
	if( size == 0 ) {
		/* 8bit */
		mbw = BITMBW_8;
	}
	else if( size == 1 ) {
		/* 16bit */
		mbw = BITMBW_16;
	}
	else {
		/* 32bit */
		mbw = BITMBW_32;
	}
	return mbw;
}

/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     :  Transaction counter setting 
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : static USB_VOID usb_set_transaction_counter( USB_U16 pipe, USB_U32 bsize );
*-----------------------------------------------------------------------------
* Description : 引数によりトランザクションカウンタ(PIPEnTRN)を設定します。
*             : 設定前にトランザクションクリアを行い、トランザクションカウンタ設定
*             : を有効にします。
*             : 
*-----------------------------------------------------------------------------
* Argument    : USB_U16 pipe     ; Pipe number
*             : USB_U32 bsize    : Data transfer size
*-----------------------------------------------------------------------------
* Return Value: void
*-----------------------------------------------------------------------------
* Note        : 
*""FUNC COMMENT END""********************************************************/
static void usb_set_transaction_counter( USB_U16 pipe, USB_U32 bsize )
{
	USB_U16	mxps;
	USB_U16 cnt;
	
	if( bsize  == 0 ){
		return;
	}

	mxps = usb_get_mxps( pipe );			/* Max Packet Size */

	if( ( bsize % mxps ) == 0 ){
		/* Trancnt */
		cnt =  ( USB_U16 )( bsize / mxps );
	} 
	else {
		cnt =  ( USB_U16 )( ( bsize / mxps ) + 1 );
	}

	switch	( pipe ) {
		case	PIPE1:
			USB.PIPE1TRE.BIT.TRCLR 	= 1;
			USB.PIPE1TRN 			= cnt;
			USB.PIPE1TRE.BIT.TRENB 	= 1;
			break;
		case	PIPE2:
			USB.PIPE2TRE.BIT.TRCLR 	= 1;
			USB.PIPE2TRN 			= cnt;
			USB.PIPE2TRE.BIT.TRENB 	= 1;
			break;
		case	PIPE3:
			USB.PIPE3TRE.BIT.TRCLR 	= 1;
			USB.PIPE3TRN			= cnt;
			USB.PIPE3TRE.BIT.TRENB 	= 1;
			break;
		case	PIPE4:
			USB.PIPE4TRE.BIT.TRCLR 	= 1;
			USB.PIPE4TRN			= cnt;
			USB.PIPE4TRE.BIT.TRENB 	= 1;
			break;
		case	PIPE5:
			USB.PIPE5TRE.BIT.TRCLR 	= 1;
			USB.PIPE5TRN			= cnt;
			USB.PIPE5TRE.BIT.TRENB 	= 1;
			break;
		default:
			break;
	}
}

/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     : Transaction counter clear
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : static USB_VOID  usb_clear_transaction_counter( USB_U16 pipe );
*-----------------------------------------------------------------------------
* Description : 引数によりトランザクションカウンタをクリアします。
*             : 
*-----------------------------------------------------------------------------
* Argument    : USB_U16 pipe     ; Pipe number
*-----------------------------------------------------------------------------
* Return Value: void
*-----------------------------------------------------------------------------
* Note        : 本関数実行後、トランザクションカウンタは無効になっています。
*""FUNC COMMENT END""********************************************************/
void  usb_clear_transaction_counter( USB_U16 pipe )
{
	switch	( pipe ) {
	case	PIPE1:
		USB.PIPE1TRE.BIT.TRENB = 0;
		USB.PIPE1TRE.BIT.TRCLR = 1;
		break;
	case	PIPE2:
		USB.PIPE2TRE.BIT.TRENB = 0;
		USB.PIPE2TRE.BIT.TRCLR = 1;
		break;
	case	PIPE3:
		USB.PIPE3TRE.BIT.TRENB = 0;
		USB.PIPE3TRE.BIT.TRCLR = 1;
		break;
	case	PIPE4:
		USB.PIPE4TRE.BIT.TRENB = 0;
		USB.PIPE4TRE.BIT.TRCLR = 1;
		break;
	case	PIPE5:
		USB.PIPE5TRE.BIT.TRENB = 0;
		USB.PIPE5TRE.BIT.TRCLR = 1;
		break;
	default:
		break;
	}

}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Stop Transfer (receive data from host).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_stop_transfer(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプのUSB転送を停止します。
*             : USB転送を停止した後、当該パイプに割り当てられたバッファを
*             : クリアします。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16  pipe     ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 本関数実行後、FIFOへの割付はPIPE0(=無効)となります。
*             : 本関数実行後、当該パイプのBRDY/NRDY/BEMP割り込みは無効となり、
*             : シーケンスビットもクリアされます。
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_stop_transfer( USB_U16 pipe )
{
	USB_U16 usefifo;
	USB_U32	remain;

	usb_set_pid_nak( pipe );

	usefifo = (USB_U16)(PipeTbl[pipe] & FIFO_USE);
	switch(usefifo){
		case	D0USE:
			usb_clear_transaction_counter( pipe );
			USB.D0FIFOCTR.WORD = BITBCLR;		/* Buffer Clear */
			break;
		case	D1USE:
			usb_clear_transaction_counter( pipe );
			USB.D1FIFOCTR.WORD = BITBCLR;		/* Buffer Clear */
			break;
//		case	D0DMA:
//			remain = usb_userdef_stop_dma0();
//			usb_dma_stop_d0( pipe, remain );
//			usb_clear_transaction_counter( pipe );
//			USB.D0FIFOCTR.WORD = BITBCLR;		/* Buffer Clear */
//			break;
//		case	D1DMA:
//			remain = usb_userdef_stop_dma1();
//			usb_dma_stop_d1( pipe, remain );
//			usb_clear_transaction_counter( pipe );
//			USB.D1FIFOCTR.WORD = BITBCLR;		/* Buffer Clear */
//			break;
		default:
			usb_clear_transaction_counter( pipe );
			USB.CFIFOCTR.WORD =  BITBCLR;		/* Buffer Clear */
			break;
	}
	usb_change_fifo_port( PIPE0,usefifo,USB_NO, BITMBW_16 );
	/* Because of obtaining the data receive byte count  */

	/* Interrupt of pipe set is disabled */
	usb_disable_brdy_int(pipe);
	usb_disable_nrdy_int(pipe);
	usb_disable_bemp_int(pipe);

	USB.PIPESEL.WORD =  PIPE0;
	usb_aclrm(pipe);
	usb_set_csclr(pipe);
}

/*===========================================================================*/
/* End of File */
