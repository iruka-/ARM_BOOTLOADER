#ifndef	rxbuffer_h_
#define	rxbuffer_h_
/*********************************************************************
 *	rxbuffer.c : �񓯊��i�x���j��M�o�b�t�@
 *********************************************************************
 */

//!	�񓯊��i�x���j��M�o�b�t�@
typedef	struct {
	uchar rxbuf[64];
	uchar *result;
	int   bitcnt;
} AsyncRxBUF;

/*********************************************************************
 *
 *********************************************************************
 */
int	rxInitBuf(void);						//!<������.
int	rxRequestBuf(uchar *result,int bitcnt);	//!<�񓯊���M���N�G�X�g.
int	rxExecuteBuf(void);						//!<�񓯊���M���N�G�X�g�����΂���Ȃ炳�΂�.
int	rxFlushBuf(void);						//!<�񓯊���M���N�G�X�g��S�����΂�.

#endif

