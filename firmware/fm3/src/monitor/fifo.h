#ifndef	_fifo_h_
#define	_fifo_h_

#include "utype.h"

/****************************************************************
 *	�� fifo �� ������o��
 ****************************************************************
 *	
 */
typedef	struct {
	uchar *buf;			// FIFO�o�b�t�@.
	int    size;		// FIFO�o�b�t�@�̃T�C�Y.
	int    inPtr;		// �l�ߍ��݃|�C���^. (0 .. size-1)
	int    outPtr;		// ���o���|�C���^. (0 .. size-1)
} FIFO;

int FIFO_init( FIFO *queue,uchar *buf,int size);	//������.
int FIFO_enqueue(FIFO *queue,uchar *buf,int size);	//�L���[�ɂ��߂�.
int FIFO_dequeue(FIFO *queue,uchar *buf,int size);	//�L���[������o��.
int FIFO_getsize(FIFO *queue);					//�L���[�ɂ��܂��Ă���o�C�g���𓾂�.
int FIFO_getfreesize(FIFO *queue);				//�L���[�̋󂫗e�ʃo�C�g���𓾂�.

//int FIFO_reset( FIFO *queue );

#define	FIFO_reset( queue )	(queue)->outPtr=(queue)->inPtr
int FIFO_enqueue_fast(FIFO *queue,ushort *buf,int size);	//�L���[�ɂ��߂�.

/****************************************************************
 *	
 ****************************************************************
 */

#endif
