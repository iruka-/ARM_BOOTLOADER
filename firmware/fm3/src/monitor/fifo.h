#ifndef	_fifo_h_
#define	_fifo_h_

#include "utype.h"

/****************************************************************
 *	■ fifo ■ 先入れ先出し
 ****************************************************************
 *	
 */
typedef	struct {
	uchar *buf;			// FIFOバッファ.
	int    size;		// FIFOバッファのサイズ.
	int    inPtr;		// 詰め込みポインタ. (0 .. size-1)
	int    outPtr;		// 取り出しポインタ. (0 .. size-1)
} FIFO;

int FIFO_init( FIFO *queue,uchar *buf,int size);	//初期化.
int FIFO_enqueue(FIFO *queue,uchar *buf,int size);	//キューにためる.
int FIFO_dequeue(FIFO *queue,uchar *buf,int size);	//キューから取り出す.
int FIFO_getsize(FIFO *queue);					//キューにたまっているバイト数を得る.
int FIFO_getfreesize(FIFO *queue);				//キューの空き容量バイト数を得る.

//int FIFO_reset( FIFO *queue );

#define	FIFO_reset( queue )	(queue)->outPtr=(queue)->inPtr
int FIFO_enqueue_fast(FIFO *queue,ushort *buf,int size);	//キューにためる.

/****************************************************************
 *	
 ****************************************************************
 */

#endif
