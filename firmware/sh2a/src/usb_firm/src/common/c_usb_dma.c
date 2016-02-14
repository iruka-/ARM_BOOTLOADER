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
 *       File Name   : c_usb_dma.c
 *       Abstract    : USB dma contorl
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
#include "iodefine.h"
#include "usb.h"

#ifndef	KPIT_GCC	// KPIT�ł��Ƃ肠�����R���p�C����ʂ�����.
#define CACHE_WRITEBACK
#endif

extern int io_cwb( USB_U32 start, USB_U32 end);

static void usb_dmaint(USB_U16 fifo); 
static void usb_dmaint_buf2fifo(USB_U16 pipe);
static void usb_dmaint_fifo2buf(USB_U16 pipe);
static void usb_enable_dmac0(USB_U32 src, USB_U32 dst, USB_U32 count,
							USB_U32 size, USB_U32 dir, USB_U32 fifo);
static void usb_enable_dmac1(USB_U32 src, USB_U32 dst, USB_U32 count,
							USB_U32 size, USB_U32 dir, USB_U32 fifo);

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : DxFIFO - Buffer data DMA transfer start
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_userdef_start_dma(USB_DMA *dma) 
*----------------------------------------------------------------------------- 
* Description : �����ɂĎw�肵����������DMA�]���\��Ԃɂ��܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_DMA *dma   : DMA parameter
*             :  typedef struct{
*             :      USB_U32 fifo;    �g�p����FIFO
*             :      USB_U32 buffer;  �]����/���o�b�t�@�擪�A�h���X
*             :      USB_U32 bytes;   �]���T�C�Y(Byte)
*             :      USB_U32 dir;     �]������(0:Buffer��FIFO, 1:FIFO��Buffer)
*             :      USB_U32 size;    DMA�]���T�C�Y
*             :   } USB_DMA;
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : �{�֐��ɂ�DMAC���W�X�^��ݒ肵�ADMA�]���\��Ԃɂ��Ă��������B
*             : �{�֐����s���USB���W���[���̐ݒ���s���ADMA�]�����J�n���܂��̂ŁA
*             : DMA�]����DMA�]�������҂����s��Ȃ��ł��������B
*             : 
*""FUNC COMMENT END""*********************************************************/
void	usb_userdef_start_dma(USB_DMA *dma)
{

	USB_U32			trncount;
	USB_U32 		src, dst;
	USB_U32			size;
	USB_U32			dir;
	USB_U32			ptr;

	trncount = dma->bytes;
	dir      = dma->dir;
	if(dir == USB_FIFO2BUF){
		/* DxFIFO determination */
		if(dma->fifo == D0_FIFO_DMA){
			src = (USB_U32)(&USB.D0FIFO.LONG);
		}
		else{
			src = (USB_U32)(&USB.D1FIFO.LONG);
		}
		dst = dma->buffer;
	}
	else{
		src = dma->buffer;
		/* DxFIFO determination */
		if(dma->fifo == D0_FIFO_DMA){
			dst = (USB_U32)(&USB.D0FIFO.LONG);
		}
		else{
			dst = (USB_U32)(&USB.D1FIFO.LONG);
		}
	}

	size = dma->size;

#ifdef CACHE_WRITEBACK
	ptr = (USB_U32)dma->buffer;
	if((ptr & 0x20000000ul) == 0 ){
		io_cwb((USB_U32)ptr,(USB_U32)(ptr)+trncount);
	}
#endif

	if(dma->fifo == D0_FIFO_DMA){
		usb_enable_dmac0(src, dst, trncount, size, dir, dma->fifo);
	}
	else{
		usb_enable_dmac1(src, dst, trncount, size, dir, dma->fifo);
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : setting DMA for D0FIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void usb_enable_dmac0(USB_U32 src, USB_U32 dst, USB_U32 count,
*                                           USB_U32 size, USB_U32 dir, USB_U32 fifo)
*----------------------------------------------------------------------------- 
* Description : �����ɂĎw�肵����������DMA�]���\��Ԃɂ��܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U32 src   : src address
*             : USB_U32 dst   : dst address
*             : USB_U32 count : transfer byte
*             : USB_U32 size  : transfer size
*             : USB_U32 dir   : direction
*             : USB_U32 fifo  : FIFO(D0FIFO or D1FIFO)
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
static void usb_enable_dmac0(USB_U32 src, USB_U32 dst, USB_U32 count,
							 USB_U32 size, USB_U32 dir, USB_U32 fifo)
{
	if( count == 0 ){
		return;
	}
	DMAC.CHCR5.BIT.HE     = 0;		/* Half end flag clear */
	DMAC.CHCR5.BIT.TE     = 0;		/* Transfer end flag clear */

	/* ---- DMA5 channel control register setting ---- */
	DMAC.CHCR5.BIT.DE = 0;					/* DMA disable */
	/* ---- DMA5 source address register setting ---- */
	DMAC.SAR5.LONG = (USB_U32)src;
	/* ---- DMA5 reload source address register setting ---- */
	DMAC.RSAR5.LONG = 0x0;
	/* ---- DMA5 destination address register setting ---- */
	DMAC.DAR5.LONG = (USB_U32)dst;
	/* ---- DMA5 reload destination address register setting ---- */
	DMAC.RDAR5.LONG = 0x0;

	/* ---- DMA5 transfer count register setting ---- */
	if(size == 0){
		DMAC.DMATCR5.LONG = count;
	}
	else if(size == 1){
		DMAC.DMATCR5.LONG = count/2;
	}
	else if(size == 2){
		DMAC.DMATCR5.LONG = count/4;
	}
	else{
		while(1){
			/* size error */
		}
	}

	/* ---- DMA5 reload transfer count register setting ---- */
	DMAC.RDMATCR5.LONG = 0;

	/* ---- DMA5 channel control register setting ---- */
	DMAC.CHCR5.BIT.TC     = 0;		/* Transfer once by one time transfer request  */
	DMAC.CHCR5.BIT.RLDSAR = 0;		/* Reload function for SAR and DMATCR is disabled */
	DMAC.CHCR5.BIT.RLDDAR = 0;		/* Reload function for DAR and DMATCR is disabled */
	DMAC.CHCR5.BIT.TEMASK = 0;		/* DMA transfer is stop when TE bit is set */
	DMAC.CHCR5.BIT.HIE    = 0;		/* Half end interrupt request is disabled */
	if(dir == USB_FIFO2BUF){
		DMAC.CHCR5.BIT.DM     = 1;	/*  Destination address is added */
		DMAC.CHCR5.BIT.SM     = 0;	/* Source address is fixed */
	}
	else if(dir == USB_BUF2FIFO){
		DMAC.CHCR5.BIT.DM     = 0;	/* Destination address is fixed */
		DMAC.CHCR5.BIT.SM     = 1;	/* Source address is added */
	}
	else{
		while(1){
			/* direction error */
		}
	}

	DMAC.CHCR5.BIT.RS     = 8;		/* DMA expansion resource selector */
	DMAC.CHCR5.BIT.TB     = 0;		/* Cycle steal mode */
	DMAC.CHCR5.BIT.TS     = size; 	/* Long word unit  */
	DMAC.CHCR5.BIT.IE     = 1;		/* Interrupt request is enabled */

	/* ==== DMA5 factor select ==== */
	if(fifo == D0_FIFO_DMA){
		DMAC.DMARS2.BYTE.CH5 = 0x03;	/* Factor select USB0 D0FIFO */
	}
	else{
		DMAC.DMARS2.BYTE.CH5 = 0x07;	/* Factor select USB1 D1FIFO */
	}

	/* ==== DMA5 interrupt enabled ==== */
	INTC.IPR07.BIT._DMAC5 = 9;		/* Set to level 9 */

	/* ---- DMA5 completion interrupt enabled ---- */
	DMAC.CHCR5.BIT.DE     = 1;		/* DMA transfer is enabled */

	/* ---- DMA transfer enabled ---- */
	DMAC.DMAOR.BIT.DME = 1;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : setting DMA for D1FIFO
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : static void usb_enable_dmac1(USB_U32 src, USB_U32 dst, USB_U32 count,
*                                           USB_U32 size, USB_U32 dir, USB_U32 fifo)
*----------------------------------------------------------------------------- 
* Description : �����ɂĎw�肵����������DMA�]���\��Ԃɂ��܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U32 src   : src address
*             : USB_U32 dst   : dst address
*             : USB_U32 count : transfer byte
*             : USB_U32 size  : transfer size
*             : USB_U32 dir   : direction
*             : USB_U32 fifo  : FIFO(D0FIFO or D1FIFO)
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
static void usb_enable_dmac1(USB_U32 src, USB_U32 dst, USB_U32 count,
							 USB_U32 size, USB_U32 dir, USB_U32 fifo)
{
	if( count == 0 ){
		return;
	}
	DMAC.CHCR6.BIT.HE     = 0;		/* Half end flag clear */
	DMAC.CHCR6.BIT.TE     = 0;		/* Transfer end flag clear */

	/* ---- DMA6 channel control register setting ---- */
	DMAC.CHCR6.BIT.DE = 0;					/* DMA disable */
	/* ---- DMA6 source address register setting ---- */
	DMAC.SAR6.LONG = (USB_U32)src;
	/* ---- DMA6 reload source address register setting ---- */
	DMAC.RSAR6.LONG = 0x0;
	/* ---- DMA6 destination address register setting ---- */
	DMAC.DAR6.LONG = (USB_U32)dst;
	/* ---- DMA6 reload destination address register setting ---- */
	DMAC.RDAR6.LONG = 0x0;

	/* ---- DMA6 transfer count register setting ---- */
	if(size == 0){
		DMAC.DMATCR6.LONG = count;
	}
	else if(size == 1){
		DMAC.DMATCR6.LONG = count/2;
	}
	else if(size == 2){
		DMAC.DMATCR6.LONG = count/4;
	}
	else{
		while(1){
			/* size error */
		}
	}

	/* ---- DMA6 reload transfer count register setting ---- */
	DMAC.RDMATCR6.LONG = 0;

	/* ---- DMA6 channel control register setting ---- */
	DMAC.CHCR6.BIT.TC     = 0;		/* Transfer once by one time transfer request  */
	DMAC.CHCR6.BIT.RLDSAR = 0;		/* Reload function for SAR and DMATCR is disabled */
	DMAC.CHCR6.BIT.RLDDAR = 0;		/* Reload function for DAR and DMATCR is disabled */
	DMAC.CHCR6.BIT.TEMASK = 0;		/* DMA transfer is stop when TE bit is set */
	DMAC.CHCR6.BIT.HIE    = 0;		/* Half end interrupt request is disabled */
	if(dir == USB_FIFO2BUF){
		DMAC.CHCR6.BIT.DM     = 1;	/*  Destination address is added */
		DMAC.CHCR6.BIT.SM     = 0;	/* Source address is fixed */
	}
	else if(dir == USB_BUF2FIFO){
		DMAC.CHCR6.BIT.DM     = 0;	/* Destination address is fixed */
		DMAC.CHCR6.BIT.SM     = 1;	/* Source address is added */
	}
	else{
		while(1){
			/* direction error */
		}
	}

	DMAC.CHCR6.BIT.RS     = 8;		/* DMA expansion resource selector */
	DMAC.CHCR6.BIT.TB     = 0;		/* Cycle steal mode */
	DMAC.CHCR6.BIT.TS     = size; 	/* Long word unit  */
	DMAC.CHCR6.BIT.IE     = 1;		/* Interrupt request is enabled */

	/* ==== DMA6 factor select ==== */
	if(fifo == D0_FIFO_DMA){
		DMAC.DMARS3.BYTE.CH6 = 0x03;	/* Factor select USB0 D0FIFO */
	}
	else{
		DMAC.DMARS3.BYTE.CH6 = 0x07;	/* Factor select USB1 D1FIFO */
	}

	/* ==== DMA6 interrupt enabled ==== */
	INTC.IPR07.BIT._DMAC6 = 9;		/* Set to level 9 */

	/* ---- DMA6 completion interrupt enabled ---- */
	DMAC.CHCR6.BIT.DE     = 1;		/* DMA transfer is enabled */

	/* ---- DMA transfer enabled ---- */
	DMAC.DMAOR.BIT.DME = 1;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer to C0FIFO data write DMA stop
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U32  usb_userdef_stop_dma0(void);
*----------------------------------------------------------------------------- 
* Description : DMA�]�����֎~��Ԃɂ��܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* ReturnValue : USB_U32 return Transfer Cnouter register(DMATCRn) value
*             : regarding to the bus width.
*----------------------------------------------------------------------------- 
* Notice      : �{�֐���dma->fifo�ɂ�D0_FIFO_DMA���w�肳�ꂽ�ۂɎ��s����DMAC��
*             : �΂��čs���Ă��������B
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U32		usb_userdef_stop_dma0(void)
{

	USB_U32		size = 0;

	if(DMAC.CHCR5.BIT.DE == 1){
		DMAC.CHCR5.BIT.DE    = 0;		/* DMA transfer disabled */
		if(	DMAC.CHCR5.BIT.TE != 1){
//			DEBUGPRINT("usb_userdef_stopdma: DMAC not end!\n.");
		}
		DMAC.CHCR5.BIT.TE     = 0;		/* Transfer end flag clear */
	}

	if( DMAC.CHCR5.BIT.DM == 1){
		/* DataIn		*/
	}
	else{
		/* DataOut		*/
	}

	if( DMAC.CHCR5.BIT.TS == 0 ) {
		size = DMAC.DMATCR5.LONG;				/* 8bit		*/
	}
	else if( DMAC.CHCR5.BIT.TS == 1 ){
		size = (DMAC.DMATCR5.LONG * 2);			/* 16bit	*/
	}
	else if( DMAC.CHCR5.BIT.TS == 2 ){
		size = (DMAC.DMATCR5.LONG * 4);			/* 32bit	*/
	}
	return size;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Buffer to C1FIFO data write DMA stop
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U32  usb_userdef_stop_dma1(void);
*----------------------------------------------------------------------------- 
* Description : DMA�]�����֎~��Ԃɂ��܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: USB_U32 return Transfer Cnouter register(DMATCRn) value
*             : regarding to the bus width.
*----------------------------------------------------------------------------- 
* Notice      : �{�֐���dma->fifo�ɂ�D1_FIFO_DMA���w�肳�ꂽ�ۂɎ��s����DMAC��
*             : �΂��čs���Ă��������B
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U32		usb_userdef_stop_dma1(void)
{
	USB_U32		size = 0;

	if(DMAC.CHCR6.BIT.DE == 1){
		DMAC.CHCR6.BIT.DE    = 0;		/* DMA transfer disabled */
		if(	DMAC.CHCR6.BIT.TE != 1){
//			DEBUGPRINT("usb_userdef_stopdma: DMAC not end!\n.");
		}
		DMAC.CHCR6.BIT.TE     = 0;		/* Transfer end flag clear */
	}

	if( DMAC.CHCR6.BIT.DM == 1){
		/* DataIn		*/
	}
	else{
		/* DataOut		*/
	}

	if( DMAC.CHCR6.BIT.TS == 0 ) {
		size = DMAC.DMATCR6.LONG;				/* 8bit		*/
	}
	else if( DMAC.CHCR6.BIT.TS == 1 ){
		size = (DMAC.DMATCR6.LONG * 2);			/* 16bit	*/
	}
	else if( DMAC.CHCR6.BIT.TS == 2 ){
		size = (DMAC.DMATCR6.LONG * 4);			/* 32bit	*/
	}
	return size;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Clear DMA Interrupt Request
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void  usb_userdef_clear_dmaint0(void)
*----------------------------------------------------------------------------- 
* Description : DMA���荞�݂��֎~���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : �{�֐���dma->fifo�ɂ�D0_FIFO_DMA���w�肳�ꂽ�ۂɎ��s����DMAC��
*             : �΂��čs���Ă��������B
*             : 
*""FUNC COMMENT END""*********************************************************/
void	usb_userdef_clear_dmaint0(void)
{
	/* DMA interrupt Control */
	INTC.IPR07.BIT._DMAC5 = 0;	/* DMA5���荞�݃��x���ݒ�(D0FIFO) */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Clear DMA Interrupt Request
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void  usb_userdef_clear_dmaint1(void)
*----------------------------------------------------------------------------- 
* Description : DMA���荞�݂��֎~���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : �{�֐���dma->fifo�ɂ�D1_FIFO_DMA���w�肳�ꂽ�ۂɎ��s����DMAC��
*             : �΂��čs���Ă��������B
*             : 
*""FUNC COMMENT END""*********************************************************/
void	usb_userdef_clear_dmaint1(void)
{
	/* DMA interrupt Control */
	INTC.IPR07.BIT._DMAC6 = 0;	/* DMA5���荞�݃��x���ݒ�(D0FIFO) */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : DMA interrupt handler
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_dma_interrupt0(void);
*----------------------------------------------------------------------------- 
* Description : This function is DMA interrupt handler entry.
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : �{�֐�����DMA���荞�݂��֎~��Ausb_dmaint()�����s���Ă��������B
*             : DMA���荞�݋֎~��dma->fifo�ɂ�D0_FIFO_DMA���w�肳�ꂽ
*             : �ۂɎ��s����DMAC�ɑ΂��čs���Ă��������B
*             : �{�֐���DMA�������荞�݂Ƃ��ēo�^���Ă��������B
*""FUNC COMMENT END""*********************************************************/
void usb_dma_interrupt0(void)
{
	/* DMA interrupt request clear */
	usb_userdef_clear_dmaint0();

	usb_dmaint(USB_D0FIFO);

	USBDmaStatus[USB_D0FIFO] = USB_DMA_READY;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : DMA interrupt handler
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_dma_interrupt1(void);
*----------------------------------------------------------------------------- 
* Description : This function is DMA interrupt handler entry.
*----------------------------------------------------------------------------- 
* Argumen t   : void
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : �{�֐�����DMA���荞�݂��֎~��Ausb_dmaint()�����s���Ă��������B
*             : DMA���荞�݋֎~��dma->fifo�ɂ�D1_FIFO_DMA���w�肳�ꂽ
*             : �ۂɎ��s����DMAC�ɑ΂��čs���Ă��������B
*             : �{�֐���DMA�������荞�݂Ƃ��ēo�^���Ă��������B
*""FUNC COMMENT END""*********************************************************/
void usb_dma_interrupt1(void)
{
	/* DMA interrupt request clear */
	usb_userdef_clear_dmaint1();

	usb_dmaint(USB_D1FIFO);

	USBDmaStatus[USB_D1FIFO] = USB_DMA_READY;
}

/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     : DMA Transfar end interrupt
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : void usb_dmaint(void);
*-----------------------------------------------------------------------------
* Description : This function is DMA transfer end interrupt�B
*             : 
*             : 
*-----------------------------------------------------------------------------
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: void
*-----------------------------------------------------------------------------
* Note        : 
*""FUNC COMMENT END""********************************************************/
static void usb_dmaint(USB_U16 fifo) 
{
	USB_U16	pipe;

	pipe	= USBDmaPipe[fifo];

	if(USBDmaInfo[fifo].dir == USB_BUF2FIFO){
		/* buffer to fifo */
		usb_dmaint_buf2fifo(pipe);
	} 
	else {
		/* fifo to buffer */
		usb_dmaint_fifo2buf(pipe);
	}
}

/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     : DMA transer end ( fifo to buffer)
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : static void usb_dmaint_fifo2buf(USB_U16 pipe);
*-----------------------------------------------------------------------------
* Description : DMAC�ɂ��FIFO����̓ǂݏo���������������s�����܂��B
*             : 
*-----------------------------------------------------------------------------
* Argument    : USB_U16 pipeno     : pipe number
*-----------------------------------------------------------------------------
* Return Value: void
*-----------------------------------------------------------------------------
* Note        : 
*""FUNC COMMENT END""********************************************************/
static void usb_dmaint_fifo2buf(USB_U16 pipe)
{
	USB_U16 			useport;
	USB_U32				remain;

	useport = (USB_U16)(PipeTbl[pipe] & FIFO_USE);

	if(useport == D0_FIFO_DMA){
		/* D0FIFO DMA stop   */
		remain = usb_userdef_stop_dma0();
		usb_dma_stop_d0( pipe, remain );
		if( USBDataCount[pipe] == 0 ){
			USB.D0FIFOCTR.WORD = BITBCLR;
			PipeFlag[pipe] = PIPE_DONE;
		}
	}
	else{
		/* D1FIFO DMA stop   */
		remain = usb_userdef_stop_dma1();
		usb_dma_stop_d1( pipe, remain );
		if( USBDataCount[pipe] == 0 ){
			USB.D1FIFOCTR.WORD = BITBCLR;
			PipeFlag[pipe] = PIPE_DONE;
		}
	}
}

/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     : DMA transer end ( buffer to fifo)
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : static void usb_dmaint_buf2fifo(USB_U16 pipe);
*-----------------------------------------------------------------------------
* Description : DMAC�ɂ��FIFO�ւ̏������݊������������s�����܂��B
*             : 
*-----------------------------------------------------------------------------
* Argument    : USB_U16 pipeno     : pipe number
*-----------------------------------------------------------------------------
* Return Value: void
*-----------------------------------------------------------------------------
* Note        : 
*""FUNC COMMENT END""********************************************************/
static void usb_dmaint_buf2fifo(USB_U16 pipe)
{
	USB_U16 			useport;
	USB_U32				remain;

	useport = (USB_U16)(PipeTbl[pipe] & FIFO_USE);

	if(useport == D0_FIFO_DMA){
		/* D0FIFO DMA stop   */
		remain = usb_userdef_stop_dma0();
		usb_dma_stop_d0( pipe, remain );

		if( USBDmaVal[USB_D0FIFO] != 0 ) {
			USB.D0FIFOCTR.BIT.BVAL = 1;
		}
	}
	else{
		/* D1FIFO DMA stop   */
		remain = usb_userdef_stop_dma1();
		usb_dma_stop_d1( pipe, remain );
	
		if( USBDmaVal[USB_D1FIFO] != 0 ) {
			USB.D1FIFOCTR.BIT.BVAL = 1;
		}
	}

	usb_enable_bemp_int(pipe);				/* Enable Empty Interrupt */
}

/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     : D0FIFO DMA stop
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : void   usb_dma_stop_d0( USB_U16 pipe, USB_U32 remain );
*-----------------------------------------------------------------------------
* Description : D0FIFO DMA stop
*             : 
*-----------------------------------------------------------------------------
* Argument    : USB_U16 pipe     : pipe number
*             : USB_U32 remain   : transfer byte
*-----------------------------------------------------------------------------
* Return Value: void
*-----------------------------------------------------------------------------
* Note        : 
*""FUNC COMMENT END""********************************************************/
void	usb_dma_stop_d0( USB_U16 pipe, USB_U32 remain )
{

	PipeDataSize[pipe] -= remain;
	USB.D0FIFOSEL.BIT.DREQE = 0;
}

/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     : D1FIFO DMA stop
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : void    usb_dma_stop_d1( USB_U16 pipe, USB_U32 remain );
*-----------------------------------------------------------------------------
* Description : D1FIFO DMA stop
*             : 
*-----------------------------------------------------------------------------
* Argument    : USB_U16 pipe     : pipe number
*             : USB_U32 remain   : transfer byte
*-----------------------------------------------------------------------------
* Return Value: void
*-----------------------------------------------------------------------------
* Note        : 
*""FUNC COMMENT END""********************************************************/
void	usb_dma_stop_d1( USB_U16 pipe, USB_U32 remain )
{
	PipeDataSize[pipe] -= remain;
	USB.D1FIFOSEL.BIT.DREQE = 0;
}

/* End of File */
