/****************************************************************
 *	�� fifo �� ������o��
 ****************************************************************
int FIFO_init( FIFO *queue,uchar *buf,int size);	//������.
int FIFO_enqueue(FIFO *queue,uchar *buf,int size);	//�L���[�ɂ��߂�.
int FIFO_dequeue(FIFO *queue,uchar *buf,int size);	//�L���[������o��.
int FIFO_getsize(FIFO *queue);					//�L���[�ɂ��܂��Ă���o�C�g���𓾂�.
int FIFO_getfreesize(FIFO *queue);				//�L���[�̋󂫗e�ʃo�C�g���𓾂�.
 */
#include "fifo.h"

/****************************************************************
 *	���[�N
 ****************************************************************
#define	RXBUF_SIZE	1024			// ��MFIFO�T�C�Y.
static uchar rxbuf[RXBUF_SIZE];	// ��MFIFO.
 */
/****************************************************************
 *	������
 ****************************************************************
 */
int FIFO_init( FIFO *queue,uchar *buf,int size)
{
	queue->buf = buf;
	queue->size = size;
	queue->inPtr = 0;
	queue->outPtr = 0;
	return 0;
}
#if	0
int FIFO_reset( FIFO *queue )
{
	queue->outPtr = queue->inPtr;
	return 0;
}
#endif
/****************************************************************
 *	�����
 ****************************************************************
 *	�����F
 *			queue   : FIFO�L���[
 *			data  : �l�ߍ��ރf�[�^.
 *			size  : �l�ߍ��ރo�C�g��.
 *	�߂�l�F�l�ߍ��݂ɐ��������o�C�g��. (0 �������� size)
 *			�L���[�ɏ[���ȋ󂫂��Ȃ��Ƃ��͋l�ߍ��݂̓L�����Z��.
 */
int FIFO_enqueue(FIFO *queue,uchar *data,int size)
{
	int i;
	int n = queue->inPtr;				// FIFO�̏������݃|�C���^.
	for(i=0; i<size; i++) {
		queue->buf[n] = *data++;
		n++;
		if(	n >= queue->size) {
			n = 0;
		}
		// FIFO���o���|�C���^�ɒǂ�������A�|�C���^���X�V���Ȃ��ŃL�����Z��.
		if(n == queue->outPtr) {
			return 0;
		}
	}
	queue->inPtr = n;
	return i;
}
/****************************************************************
 *	���܂��Ă���o�C�g����Ԃ�.
 ****************************************************************
 *	�߂�l�F queue �ɗ��܂��Ă���o�C�g��
 */
int FIFO_getsize(FIFO *queue)
{
	int n = queue->inPtr - queue->outPtr;
	if(n<0) {
		n = n + queue->size;
	}
	return n;
}
/****************************************************************
 *	�󂢂Ă���o�C�g����Ԃ�.
 ****************************************************************
 *	�߂�l�F queue �̋󂢂Ă���o�C�g��
 */
int FIFO_getfreesize(FIFO *queue)
{
	return queue->size - FIFO_getsize(queue);
}
/****************************************************************
 *	��o��
 ****************************************************************
 *	�����F
 *			queue   : FIFO�L���[
 *			result: ���o�����f�[�^�̎󂯎��ꏊ.
 *			size  : ���o�������o�C�g��.
 *	�߂�l�F���o���ɐ��������o�C�g��. (0 .. size)
 */
int FIFO_dequeue(FIFO *queue,uchar *result,int size)
{
	int	n = queue->outPtr;					// FIFO�̎��o���|�C���^.
	int	i;
	for(i=0; i<size; i++) {
		if( n == queue->inPtr ) break;		// FIFO�̃g�b�v�ɒǂ�����.
		result[i] = queue->buf[n];			// ��M�������R�s�[����.
		n++;
		if(	n >= queue->size) {				// ����o�b�t�@����.
			n = 0;
		}
	}
	queue->outPtr = n;						// FIFO�̎��o���|�C���^�X�V.
	return i;
}

/****************************************************************
 *	�����
 ****************************************************************
 *	�����F
 *			queue   : FIFO�L���[
 *			data  : �l�ߍ��ރf�[�^.
 *			size  : �l�ߍ��ރo�C�g��.
 *	�߂�l�F�l�ߍ��݂ɐ��������o�C�g��. (0 �������� size)
 *			�L���[�ɏ[���ȋ󂫂��Ȃ��Ƃ��͋l�ߍ��݂̓L�����Z��.
 *	note:	�L���[��2�o�C�g�Â��߂�.���o����2�̔{���łȂ��Ƒʖ�.
 */
int FIFO_enqueue_fast(FIFO *queue,ushort *data,int size)
{
	int i;
	int n = queue->inPtr;				// FIFO�̏������݃|�C���^.
	ushort *t;
	for(i=0; i<size; i++) {
		t = (ushort *) &queue->buf[n];
		*t = *data++;
		n+=2;
		if(	n >= queue->size) {
			n = 0;
		}
		// FIFO���o���|�C���^�ɒǂ�������A�|�C���^���X�V���Ȃ��ŃL�����Z��.
		if(n == queue->outPtr) {
			return 0;
		}
	}
	queue->inPtr = n;
	return i;
}
/******************************************************************
 *
 ******************************************************************
 */
