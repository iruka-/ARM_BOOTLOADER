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
 *       File Name   : p_main.c
 *       Abstract    : Peripheral Sample Program Main
 *       Version     : 1.01.00
 *       Device      : SH7264/SH7262
 *       Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
 *                   : C/C++ compiler package for the SuperH RISC engine family
 *                   :                             (Ver.9.02 Release00).
 *       OS          : None
 *       H/W Platform: M3A-HS64G50(CPU board)
 *
 *       History     : May.13,2009 ver.1.00.00
 *                   : Dec.25,2009 ver.1.01.00 CDC�Ή�
 *""FILE COMMENT END""**********************************************************/
#include <machine.h>
#include <stdio.h>
#include "usb.h"
#include "../../monitor/usbio.h"
#include "../../monitor/monit.h"

/* ==== �}�N����` ==== */
#define BULK_OUT_PIPE	PIPE1
#define BULK_IN_PIPE	PIPE2
#define RCV_BUFSIZE		512

/* ��M�o�b�t�@ �i�[�o�C�g���ێ��ϐ�(�O���[�o���ϐ���) */
static int recvcheck = 0;					/* ��M�`�F�b�N�X�e�[�g�ϐ�(�V�K�ǉ�) */
static int rsz = 0;							// ��M�T�C�Y.

static unsigned char recvbuf[RCV_BUFSIZE];	// ��M�o�b�t�@.


/* ==== �v���g�^�C�v�錾 ==== */
int  CDC_start_snd( unsigned long size, unsigned char *top );	// ���M�J�n.
int  CDC_start_rcv( unsigned long size, unsigned char *buf );	// ��M�J�n.
int  CDC_check_complete( unsigned short Pipe );					//(��M)�����҂�.









/*----------------------------------------------------------------------------- 
 * Description : 
 				 USB-CDC�f�o�C�X Configure���I������܂ő҂�.
 *----------------------------------------------------------------------------- 
 */

void	CDC_start_check(void)
{
	while ( P_IsConfigured() != USB_YES ) {
		led_blink(15);
	}
	led_on();
}

void usbModuleDisable(void);

/*----------------------------------------------------------------------------- 
 * Description : USB-CDC�ǂݍ��݊J�n�L�b�N.

   �Ăяo�����^�C�~���O.
	�i�P�jCMT0���荞�݂���̌Ăяo��. ( 1mS�Ԋu . �ŏ��̂���������� �^�i�Q�j�̒��f����ĊJ�����邽��)
	�i�Q�j��M����callback��.         (�A����MFIFO����t�ɂȂ����ꍇ�͒��f����)

 *----------------------------------------------------------------------------- 
 */
void CDC_recv_kick(void)
{
	// ��M�L���[��RCV_BUFSIZE �ȉ��̏ꍇ�́Akick�o���Ȃ�.
	//		(�t�H�A�O���E���h�����f�[�^����������܂ł����Ƒ҂�.)
	if( USBgetfreesize() < RCV_BUFSIZE ) { return; }

	if (recvcheck == 0) {
		if ( rsz <= 0 ) {
			/* ��M�J�n���� */
			if( CDC_start_rcv( RCV_BUFSIZE, recvbuf ) == 0 ){
				recvcheck = 1;
			}
		}
	}
}

/*----------------------------------------------------------------------------- 
 * Description : BULK_OUT_PIPE�Ɏ�M���ꂽ�f�[�^�̃o�C�g����Ԃ��܂�.
 				 ����p�Ƃ��āA�t���O recvcheck ���N���A���܂�.
 *----------------------------------------------------------------------------- 
 */
static int CDC_recv_rsz(void)
{
	/* ��M�����m�F */
	if (CDC_check_complete(BULK_OUT_PIPE)){
		/* ��M���� */
		rsz = RCV_BUFSIZE - USBDataCount[ BULK_OUT_PIPE ];
		recvcheck = 0;
		return rsz;
	} else {
		return 0;
	}
}
/*----------------------------------------------------------------------------- 
 * Description : 
                 ��M����USB-CDC�f�[�^��FIFO�ɗ��߂܂�.
 *----------------------------------------------------------------------------- 
 */
int CDC_rcv_queue(int size)
{
	/* ==== �o�b�t�@�����M�f�[�^�� size �o�C�g���o�� ==== */
	if(size >= rsz) {size=rsz;}
	USBenqueue(recvbuf,size);
	rsz = 0;
	return size;
}

//
// USB-CDC�ǂݍ��݊J�n�L�b�N.
//		CMT0���荞�݂���̌Ăяo��.
//
void CDC_read_kick(void)
{
	if( P_IsConfigured() != USB_YES ) return;

	CDC_recv_kick();
}

/*----------------------------------------------------------------------------- 
 * Description : hostPC���瑗���Ă���BULK�]���̓ǂݏo���������ɌĂяo����܂�.
                (usb_firm\src\common\c_intrn.c:100 ����̌Ăяo��).
                 ��M����USB-CDC�f�[�^��FIFO�ɗ��߂āA���̎�M���L�b�N���܂�.
 *----------------------------------------------------------------------------- 
 */
int	USB_read_end_callback(USB_U16 pipe)
{
	if( pipe != BULK_OUT_PIPE) return 0;

	int size = CDC_recv_rsz();
	if(	size > 0) {
		CDC_rcv_queue(size);
		rsz=0;
		CDC_recv_kick();
	}
	return 0;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : �f�[�^���M�J�n
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : int CDC_start_snd( unsigned long size, unsigned char *top );
*----------------------------------------------------------------------------- 
* Description : �o���NIN�]�����g�p���āA�����Ŏw�肳�ꂽ�f�[�^�T�C�Y���̑��M
*             : �f�[�^��USB�z�X�g�ɑ��M���܂��B���M�̊����͑҂����ɌĂяo������
*             : �߂�܂��B
*----------------------------------------------------------------------------- 
* Argument    : unsigned long size : I ; ���M�f�[�^�T�C�Y
*             : unsigned char *top : I ; ���M�f�[�^�̐擪�A�h���X
*----------------------------------------------------------------------------- 
* Return Value:  0 : ���M�N������
*             : -1 : ���M�G���[
*----------------------------------------------------------------------------- 
* Notice      : 
*""FUNC COMMENT END""*********************************************************/
int CDC_start_snd( unsigned long size, unsigned char *top )
{
	USB_U16		buffer;
	USB_U16 	pipe = BULK_IN_PIPE;

	/* ---- STALL�����̏ꍇ�̓G���[��Ԃ� ---- */
	buffer = usb_get_pid(pipe);
	if( ( buffer == PID_STALL ) || (buffer == PID_STALL2) ) {
		return -1;
	}
	/* ---- �O��̎�M�������������Ă��Ȃ��ꍇ�̓G���[��Ԃ� ---- */
	if( (PipeFlag[pipe] != PIPE_IDLE) && (PipeFlag[pipe] != PIPE_DONE) ){
		return -1;
	}
	/* ---- ���M�J�n�i�o���NIN�]�������j ---- */
	PipeFlag[pipe] = PIPE_WAIT;
	usb_start_send_transfer(pipe, size, top);
	return 0;
}
/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : �f�[�^��M�J�n
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : int CDC_start_rcv( unsigned long size, unsigned char *buf );
*----------------------------------------------------------------------------- 
* Description : �o���NOUT�]�����g�p���āA�����Ŏw�肳�ꂽ�f�[�^�T�C�Y���̎�M
*             : �f�[�^��USB�z�X�g�����M���܂��B��M�̊����͑҂����ɌĂяo������
*             : �߂�܂��B
*----------------------------------------------------------------------------- 
* Argument    : unsigned long size : I ; ��M�f�[�^�T�C�Y
*             : unsigned char *buf : O ; ��M�o�b�t�@�̐擪�A�h���X
*----------------------------------------------------------------------------- 
* Return Value:  0 : ��M�N������
*             : -1 : ��M�G���[
*----------------------------------------------------------------------------- 
* Notice      : 
*""FUNC COMMENT END""*********************************************************/
int CDC_start_rcv( unsigned long size, unsigned char *buf )
{
	USB_U16		buffer;
	USB_U16 	pipe = BULK_OUT_PIPE;
	
	/* ---- STALL�����̏ꍇ�̓G���[��Ԃ� ---- */
	buffer = usb_get_pid(pipe);
	if( ( buffer == PID_STALL ) || (buffer == PID_STALL2) ) {
//		BLUELED_on();
		return -1;
	}
	/* ---- �O��̎�M�������������Ă��Ȃ��ꍇ�̓G���[��Ԃ� ---- */
	if( (PipeFlag[pipe] != PIPE_IDLE) && (PipeFlag[pipe] != PIPE_DONE) ){
		return -1;
	}
//	WHITELED_on();
	/* ---- ��M�J�n�i�o���NOUT�]�������j ---- */
	PipeFlag[pipe] = PIPE_WAIT;
	usb_start_recieve_transfer( pipe, size, buf );
	return 0;
}
/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : �]�������̊m�F
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : int CDC_check_complete( unsigned short pipe );
*----------------------------------------------------------------------------- 
* Description : �����Ŏw�肵���p�C�v�̓]�����������Ă���ꍇ�́A�]��������Ԃ��܂��B
*----------------------------------------------------------------------------- 
* Argument    : unsigned short pipe : I ; �p�C�v�ԍ�
*----------------------------------------------------------------------------- 
* Return Value: 1 : �]������
*             : 0 : �]���� �܂��� STALL/�G���[���
*----------------------------------------------------------------------------- 
* Notice      : 
*""FUNC COMMENT END""*********************************************************/
int CDC_check_complete( unsigned short pipe )
{
	if( (PipeFlag[pipe] == PIPE_IDLE) || (PipeFlag[pipe] == PIPE_DONE) ){
		return 1;
	}
	return 0;
}

/* End of File */
