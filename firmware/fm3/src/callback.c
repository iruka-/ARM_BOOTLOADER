/***********************************************************************
 *		USB転送完了コールバック
 ***********************************************************************
void 	UsbReadKick(void);
int		usbSendBytes(uchar *buf,int size);
void	USB_loop(void);
 */
#include "common.h"
#include "cpu_config.h"
#include "hw_support.h"
#include "usbf.h"
#include "usbf_cdc.h"
#include "timer.h"
#include "led.h"
#include "usbfunc_demoapp_config.h"
#include "dbg_print.h"
#include "debug.h"
#include "callback.h"
#include "monitor/fifo.h"
#include "monitor/usbio.h"
#include "monitor/monit.h"

#define	USBREAD_USE_INTERRUPT		(1)		// 割り込みによるUSB受信.
#define	ECHOBACK_TEST				(0)

//
///	転送終了コールバックにより、書き換える構造体.(EP16個分を用意しておく)
//
static TRANS_STATUS trans_status[16];

//
//	受信バッファ
//
static	uchar	recv_buffer[64];

//
///	転送終了コールバックにより、書き換える構造体のアドレスを得る.
//
static	TRANS_STATUS *UsbGetStatusP(int epid)
{
	return &trans_status[epid & 0x0f];
}

//
///	転送終了コールバックの構造体をリセットする.
//
void UsbClrEpSema(int epid,int flag)
{
	TRANS_STATUS *p = UsbGetStatusP(epid);
	
	p->in_use = flag;
	p->status = 0;
	p->size = 0;
}

//
///	使用中なら 1 , 転送終了したら 2 を返す.
//
static	int UsbGetEpSema(int epid)
{
	TRANS_STATUS *p = UsbGetStatusP(epid);
	return p->in_use;	//
}

#if	0
//
///	転送終了したら パケットサイズ(byte) を返す.
//
static	int  UsbGetEpTransSize(int epid)
{
	TRANS_STATUS *p = UsbGetStatusP(epid);
	return p->size;	// 1=Done!
}
#endif
//
///	転送終了コールバック.
//
void cdc_trans_complete(int epid,uint16_t count, uint8_t complete_status)
{
	TRANS_STATUS *p = UsbGetStatusP(epid);
    if ((complete_status != USBF_TRANS_CANCELED) &&
        (complete_status != USBF_TRANS_NG))
    {
		p->in_use = TRANS_DONE;
        p->status = complete_status;
        p->size = count;

		// 割り込みによるUSB受信.（成功）
		if(epid == USBF_CDC_BULKOUT_EP) {
			//led_xor_pat(0x10);
			USBenqueue(recv_buffer,count);	//!< 受信済みデータをFIFOに溜める.
			UsbReadKick();					//!< 次の受信を開始する.
		}
    }else{	// 割り込みによるUSB受信.（失敗）
		if(epid == USBF_CDC_BULKOUT_EP) {
			//led_xor_pat(0x80);
			UsbClrEpSema(USBF_CDC_BULKOUT_EP, TRANS_IDLE);	//!< ステータスをリセットする.
		}
	}
    return;
}



//
///	 次の受信を開始する.
//
void UsbReadKick(void)
{
	int epid = USBF_CDC_BULKOUT_EP;
	int rc;

	//! FIFOが満杯のときは、何もしない.
	if(	USBgetfreesize() < 64 ) return;	// キューに空きが無い.

	//! エンドポイント使用中のときも何もしない.
	if(	UsbGetEpSema(epid) != TRANS_INUSE ) 
	{
		UsbClrEpSema(epid, TRANS_INUSE);			//!< EP使用中にする.
		rc = USBF_Receive(epid, recv_buffer , 64);	//!< 受信キックを実行.
		if(rc != USBF_RET_OK) {
			UsbClrEpSema(epid, TRANS_IDLE);			//!< 受信要求は失敗.ステートリセット.
		}
	}
}


//
///	USBパケットをホストPCへ送る関数.
//

//	戻り値：ホストへ送信したバイト数.(重要!)

int	usbSendBytes(uchar *buf,int size)
{
	int epid = USBF_CDC_BULKIN_EP;
	int rc;
	uint8_t status;
	static uchar sendbuf[64];	//即時リターンする為に、データをコピーする.

	//!	EP使用中の場合は、転送完了を待つ.
	while(1) {
//		if(	UsbGetEpSema(epid) != TRANS_INUSE ) break;
		status = 0;
		USBF_GetEpStatus(epid,&status);
		if(status != USBF_EPSTATUS_TRANS ) break;
	}

	//! 一度に送信できるバイト数は64まで.
	if(size>64) size = 64;

	memcpy(sendbuf,buf,64);

	UsbClrEpSema(epid,TRANS_INUSE);
	rc = USBF_Send(epid , sendbuf , size );		//!< 送りっぱなし。完了待ちしない.
	if(rc != USBF_RET_OK) {
		UsbClrEpSema(epid, TRANS_IDLE);		//!<	送信要求は失敗.
		return 0;							//!<	転送したバイト数＝０.
	}

	return size;
}

#if	ECHOBACK_TEST

#define	BUF_SIZE	256

//
//	エコーバック・タスク.
//
void USBtask_Echoback(void)
{
	uchar buf[BUF_SIZE];

	int rxsize = USBavailable();
	if( rxsize >= BUF_SIZE ) {
		rxsize  = BUF_SIZE;
	}

	if( rxsize ) {
		USBread( buf, rxsize );		//!< 受信FIFOから rxsize バイトを取り出し,
		USBwrite( buf, rxsize );	//!< ホストＰＣにそのまま返送する.
	}
}

//
//	エコーバック・ループで呼び出される関数.
//
void USB_loop(void)
{
	USBtask_Echoback();
}

#else
//
//	モニター・ループで呼び出される関数.
//
void USB_loop(void)
{
	while (1) {
		int ch = USBgetch();		//!< １文字受信.
		if(	ch == 0xaa) {
			ch = USBgetch();		//!< さらに１文字受信.
			if(	ch == 0x55) {		//!< 0xaa , 0x55 が来たらmonitorコマンド受付.
				JTAG_disable();
				USBmonit();
				led_on();
			} else if(ch == 0xa5) {	//!< 0xaa , 0xa5 が来たら 0x55 を返す(合言葉).
				USBputch(0x55);
			}
		}
	}
}
#endif


