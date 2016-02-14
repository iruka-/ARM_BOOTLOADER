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
 *                   : Dec.25,2009 ver.1.01.00 CDC対応
 *""FILE COMMENT END""**********************************************************/
#include <machine.h>
#include <stdio.h>
#include "usb.h"
#include "../../monitor/usbio.h"
#include "../../monitor/monit.h"

/* ==== マクロ定義 ==== */
#define BULK_OUT_PIPE	PIPE1
#define BULK_IN_PIPE	PIPE2
#define RCV_BUFSIZE		512

/* 受信バッファ 格納バイト数保持変数(グローバル変数化) */
static int recvcheck = 0;					/* 受信チェックステート変数(新規追加) */
static int rsz = 0;							// 受信サイズ.

static unsigned char recvbuf[RCV_BUFSIZE];	// 受信バッファ.


/* ==== プロトタイプ宣言 ==== */
int  CDC_start_snd( unsigned long size, unsigned char *top );	// 送信開始.
int  CDC_start_rcv( unsigned long size, unsigned char *buf );	// 受信開始.
int  CDC_check_complete( unsigned short Pipe );					//(受信)完了待ち.









/*----------------------------------------------------------------------------- 
 * Description : 
 				 USB-CDCデバイス Configureが終了するまで待つ.
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
 * Description : USB-CDC読み込み開始キック.

   呼び出されるタイミング.
	（１）CMT0割り込みからの呼び出し. ( 1mS間隔 . 最初のきっかけ作り ／（２）の中断から再開させるため)
	（２）受信完了callback時.         (但し受信FIFOが一杯になった場合は中断する)

 *----------------------------------------------------------------------------- 
 */
void CDC_recv_kick(void)
{
	// 受信キューがRCV_BUFSIZE 以下の場合は、kick出来ない.
	//		(フォアグラウンド側がデータを消化するまでずっと待つ.)
	if( USBgetfreesize() < RCV_BUFSIZE ) { return; }

	if (recvcheck == 0) {
		if ( rsz <= 0 ) {
			/* 受信開始処理 */
			if( CDC_start_rcv( RCV_BUFSIZE, recvbuf ) == 0 ){
				recvcheck = 1;
			}
		}
	}
}

/*----------------------------------------------------------------------------- 
 * Description : BULK_OUT_PIPEに受信されたデータのバイト数を返します.
 				 副作用として、フラグ recvcheck をクリアします.
 *----------------------------------------------------------------------------- 
 */
static int CDC_recv_rsz(void)
{
	/* 受信完了確認 */
	if (CDC_check_complete(BULK_OUT_PIPE)){
		/* 受信完了 */
		rsz = RCV_BUFSIZE - USBDataCount[ BULK_OUT_PIPE ];
		recvcheck = 0;
		return rsz;
	} else {
		return 0;
	}
}
/*----------------------------------------------------------------------------- 
 * Description : 
                 受信したUSB-CDCデータをFIFOに溜めます.
 *----------------------------------------------------------------------------- 
 */
int CDC_rcv_queue(int size)
{
	/* ==== バッファから受信データを size バイト取り出す ==== */
	if(size >= rsz) {size=rsz;}
	USBenqueue(recvbuf,size);
	rsz = 0;
	return size;
}

//
// USB-CDC読み込み開始キック.
//		CMT0割り込みからの呼び出し.
//
void CDC_read_kick(void)
{
	if( P_IsConfigured() != USB_YES ) return;

	CDC_recv_kick();
}

/*----------------------------------------------------------------------------- 
 * Description : hostPCから送られてくるBULK転送の読み出し完了時に呼び出されます.
                (usb_firm\src\common\c_intrn.c:100 からの呼び出し).
                 受信したUSB-CDCデータをFIFOに溜めて、次の受信をキックします.
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
* Outline     : データ送信開始
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : int CDC_start_snd( unsigned long size, unsigned char *top );
*----------------------------------------------------------------------------- 
* Description : バルクIN転送を使用して、引数で指定されたデータサイズ分の送信
*             : データをUSBホストに送信します。送信の完了は待たずに呼び出し元に
*             : 戻ります。
*----------------------------------------------------------------------------- 
* Argument    : unsigned long size : I ; 送信データサイズ
*             : unsigned char *top : I ; 送信データの先頭アドレス
*----------------------------------------------------------------------------- 
* Return Value:  0 : 送信起動成功
*             : -1 : 送信エラー
*----------------------------------------------------------------------------- 
* Notice      : 
*""FUNC COMMENT END""*********************************************************/
int CDC_start_snd( unsigned long size, unsigned char *top )
{
	USB_U16		buffer;
	USB_U16 	pipe = BULK_IN_PIPE;

	/* ---- STALL応答の場合はエラーを返す ---- */
	buffer = usb_get_pid(pipe);
	if( ( buffer == PID_STALL ) || (buffer == PID_STALL2) ) {
		return -1;
	}
	/* ---- 前回の受信処理が完了していない場合はエラーを返す ---- */
	if( (PipeFlag[pipe] != PIPE_IDLE) && (PipeFlag[pipe] != PIPE_DONE) ){
		return -1;
	}
	/* ---- 送信開始（バルクIN転送を許可） ---- */
	PipeFlag[pipe] = PIPE_WAIT;
	usb_start_send_transfer(pipe, size, top);
	return 0;
}
/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : データ受信開始
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : int CDC_start_rcv( unsigned long size, unsigned char *buf );
*----------------------------------------------------------------------------- 
* Description : バルクOUT転送を使用して、引数で指定されたデータサイズ分の受信
*             : データをUSBホストから受信します。受信の完了は待たずに呼び出し元に
*             : 戻ります。
*----------------------------------------------------------------------------- 
* Argument    : unsigned long size : I ; 受信データサイズ
*             : unsigned char *buf : O ; 受信バッファの先頭アドレス
*----------------------------------------------------------------------------- 
* Return Value:  0 : 受信起動成功
*             : -1 : 受信エラー
*----------------------------------------------------------------------------- 
* Notice      : 
*""FUNC COMMENT END""*********************************************************/
int CDC_start_rcv( unsigned long size, unsigned char *buf )
{
	USB_U16		buffer;
	USB_U16 	pipe = BULK_OUT_PIPE;
	
	/* ---- STALL応答の場合はエラーを返す ---- */
	buffer = usb_get_pid(pipe);
	if( ( buffer == PID_STALL ) || (buffer == PID_STALL2) ) {
//		BLUELED_on();
		return -1;
	}
	/* ---- 前回の受信処理が完了していない場合はエラーを返す ---- */
	if( (PipeFlag[pipe] != PIPE_IDLE) && (PipeFlag[pipe] != PIPE_DONE) ){
		return -1;
	}
//	WHITELED_on();
	/* ---- 受信開始（バルクOUT転送を許可） ---- */
	PipeFlag[pipe] = PIPE_WAIT;
	usb_start_recieve_transfer( pipe, size, buf );
	return 0;
}
/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : 転送完了の確認
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : int CDC_check_complete( unsigned short pipe );
*----------------------------------------------------------------------------- 
* Description : 引数で指定したパイプの転送が完了している場合は、転送完了を返します。
*----------------------------------------------------------------------------- 
* Argument    : unsigned short pipe : I ; パイプ番号
*----------------------------------------------------------------------------- 
* Return Value: 1 : 転送完了
*             : 0 : 転送中 または STALL/エラー状態
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
