/*!*!*!***************************************************************
 *	rxbuffer.c : �񓯊��i�x���j��M�o�b�t�@
 *********************************************************************
 *
 *
int	rxInitBuf(void);						//������.
int	rxRequestBuf(uchar *result,int bitcnt);	//�񓯊���M���N�G�X�g.
int	rxExecuteBuf(void);						//�񓯊���M���N�G�X�g�����΂���Ȃ炳�΂�.
int	rxFlushBuf(void);						//�񓯊���M���N�G�X�g��S�����΂�.
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
 *	rxbuffer.c : �񓯊��i�x���j��M�o�b�t�@
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
#define	MAX_RXBUF	128		//2�ׂ̂�.
#define	MASK_PTR	(MAX_RXBUF-1)

static	AsyncRxBUF	rxBuf[MAX_RXBUF];	// FIFO �L���[.
static	int	inPtr = 0;					// FIFO �L���[�l�ߍ��݃|�C���^.(��s)
static	int	outPtr = 0;					// FIFO �L���[���o���|�C���^.(�ǂ�����)

#define	JTAG_DUMP		1		// 

/*********************************************************************
 *	�L���[�ɗ��܂������N�G�X�g����Ԃ�.
 *********************************************************************
 */
static	int	rxAvailable(void)
{
	return (inPtr - outPtr) & MASK_PTR;
}
/*********************************************************************
 *	�L���[�̋󂫃X���b�g����Ԃ�.
 *********************************************************************
 */
static	int	rxFreeSlots(void)
{
	return MAX_RXBUF - rxAvailable();
}

void	bang_get_result(uint8_t *buffer,uchar *result,int count);

/*********************************************************************
 *	�x���ǂݍ��݂̎��s.
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
int	rxInitBuf(void)						//������.
{
	inPtr  = 0;
	outPtr = 0;
	return 0;
}

//�񓯊���M���N�G�X�g���P���΂��ăL���[�����炷.
int	rxFlushBuf1(void)						
{
	while( rxFreeSlots() == 0 ) {
		rxExecuteBuf();
	}
	return 0;
}

int	rxRequestBuf(uchar *result,int bitcnt)	//�񓯊���M���N�G�X�g.
{
	int ptr;

	LOGFILE_print("rxRequestBuf(%x,%d)\n",result,bitcnt);
	
	// �L���[�����t�ɂȂ�����A��U�t���b�V������.
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

int	rxExecuteBuf_One(void)	//�񓯊���M���N�G�X�g���P���΂�.
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

int	rxExecuteBuf(void)		//�񓯊���M���N�G�X�g�����΂���Ȃ炳�΂�.
{
	if ( rxAvailable() ) {
		while( RS_checkdata() >= REPORT_LENGTH) {
			rxExecuteBuf_One();
		}
	}
	return 0;
}

int	rxFlushBuf(void)						//�񓯊���M���N�G�X�g��S�����΂�.
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
