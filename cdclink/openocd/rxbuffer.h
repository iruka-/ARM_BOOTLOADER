#ifndef	rxbuffer_h_
#define	rxbuffer_h_
/*********************************************************************
 *	rxbuffer.c : 非同期（遅延）受信バッファ
 *********************************************************************
 */

//!	非同期（遅延）受信バッファ
typedef	struct {
	uchar rxbuf[64];
	uchar *result;
	int   bitcnt;
} AsyncRxBUF;

/*********************************************************************
 *
 *********************************************************************
 */
int	rxInitBuf(void);						//!<初期化.
int	rxRequestBuf(uchar *result,int bitcnt);	//!<非同期受信リクエスト.
int	rxExecuteBuf(void);						//!<非同期受信リクエストがさばけるならさばく.
int	rxFlushBuf(void);						//!<非同期受信リクエストを全部さばく.

#endif

