/*!*!*!***************************************************************
 *	rxbuffer.c : 非同期（遅延）受信バッファ
 *********************************************************************
 *
 *
int	rxInitBuf(void);						//初期化.
int	rxRequestBuf(uchar *result,int bitcnt);	//非同期受信リクエスト.
int	rxExecuteBuf(void);						//非同期受信リクエストがさばけるならさばく.
int	rxFlushBuf(void);						//非同期受信リクエストを全部さばく.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "bitbang.h"
#include <jtag/interface.h>
#include <jtag/commands.h>
#include "rs232c.h"
#include "packet.h"
#include "rxbuffer.h"

/*********************************************************************
 *	rxbuffer.c : 非同期（遅延）受信バッファ
 *********************************************************************

typedef	struct {
	uchar rxbuf[64];
	uchar *result;
	int   bitcnt;
} AsyncRxBUF;

 *********************************************************************
 *
 *********************************************************************
 *
 */
#define	MAX_RXBUF	128		//2のべき.
#define	MASK_PTR	(MAX_RXBUF-1)

static	AsyncRxBUF	rxBuf[MAX_RXBUF];	// FIFO キュー.
static	int	inPtr = 0;					// FIFO キュー詰め込みポインタ.(先行)
static	int	outPtr = 0;					// FIFO キュー取り出しポインタ.(追っかけ)

#define	JTAG_DUMP		1		// 

/*********************************************************************
 *	キューに溜まったリクエスト数を返す.
 *********************************************************************
 */
static	int	rxAvailable(void)
{
	return (inPtr - outPtr) & MASK_PTR;
}
/*********************************************************************
 *	キューの空きスロット数を返す.
 *********************************************************************
 */
static	int	rxFreeSlots(void)
{
	return MAX_RXBUF - rxAvailable();
}

void	bang_get_result(uint8_t *buffer,uchar *result,int count);

/*********************************************************************
 *	遅延読み込みの実行.
 *********************************************************************
 */
static	int	execute_rx(uchar *result,int bitcnt)
{
	uchar rbuf[256];

	LOGFILE_print("execute_rx(%x,%d)\n",result,bitcnt);

	int rc = hidRead(rbuf , REPORT_LENGTH );
	if(rc == 0) {
		LOGFILE_print("QueryAVR:hidRead error rc=%d\n",rc);
		UsbExit();
		return 0;
	}
	bang_get_result(result,rbuf,bitcnt);

#if	JTAG_DUMP
	if(result!=NULL) {
		int i;
		int rsize = (bitcnt+7)/8;
		LOGFILE_print("DRET:");
		for(i=0; i<rsize; i++) {
			LOGFILE_print(" %02x",result[i]);
		}
		LOGFILE_print("\n");
	}
#endif

	return 0;
}

/*********************************************************************
 *
 *********************************************************************
 */
int	rxInitBuf(void)						//初期化.
{
	inPtr  = 0;
	outPtr = 0;
	return 0;
}

//非同期受信リクエストを１個さばいてキューを減らす.
int	rxFlushBuf1(void)						
{
	while( rxFreeSlots() == 0 ) {
		rxExecuteBuf();
	}
	return 0;
}

int	rxRequestBuf(uchar *result,int bitcnt)	//非同期受信リクエスト.
{
	int ptr;

	LOGFILE_print("rxRequestBuf(%x,%d)\n",result,bitcnt);
	
	// キューが満杯になったら、一旦フラッシュする.
	if( rxFreeSlots() == 0 ) {
		rxFlushBuf1();
	}

	ptr = inPtr;
	{
		AsyncRxBUF *q = &rxBuf[ptr];
			q->result = result;
			q->bitcnt = bitcnt;
			ptr++;	ptr &= MASK_PTR;
	}
	inPtr = ptr;
	return 1;
}

int	rxExecuteBuf_One(void)	//非同期受信リクエストを１個さばく.
{
	int ptr;
	ptr = outPtr;	if(ptr == inPtr) return 0;
	{
		AsyncRxBUF *q = &rxBuf[ptr];
		execute_rx(q->result,q->bitcnt);
		ptr++;	ptr &= MASK_PTR;
	}
	outPtr = ptr;
	return 1;
}

int	rxExecuteBuf(void)		//非同期受信リクエストがさばけるならさばく.
{
	if ( rxAvailable() ) {
		while( RS_checkdata() >= REPORT_LENGTH) {
			rxExecuteBuf_One();
		}
	}
	return 0;
}

int	rxFlushBuf(void)						//非同期受信リクエストを全部さばく.
{
	while( rxAvailable() ) {
		rxExecuteBuf();
	}
	return 0;
}

/*********************************************************************
 *
 *********************************************************************
 */
/*********************************************************************
 *
 *********************************************************************
 */
