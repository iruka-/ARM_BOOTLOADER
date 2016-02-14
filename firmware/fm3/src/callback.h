/***********************************************************************
 *		USB転送完了コールバック
 ***********************************************************************
 */
#ifndef	callback_h_
#define	callback_h_

#include "monitor/utype.h"
//
///	in_use に入れる値.	転送ステート.
//
enum {
	TRANS_IDLE =0,		// 転送 未使用.
	TRANS_INUSE=1,		// 転送 使用中.
	TRANS_DONE =2,		// 転送 完了.
};

//
///	転送終了コールバックにより、書き換える構造体.(EP1個あたり)
//
typedef	struct {
	volatile short in_use;		// 0=idle 1=in use. 2=done.
	volatile short status;
	volatile int   size;
} TRANS_STATUS;

void 	UsbReadKick(void);
int		usbSendBytes(uchar *buf,int size);
void	USB_loop(void);

#endif

