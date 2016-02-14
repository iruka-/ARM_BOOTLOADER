/***********************************************************************
 *		USB�]�������R�[���o�b�N
 ***********************************************************************
 */
#ifndef	callback_h_
#define	callback_h_

#include "monitor/utype.h"
//
///	in_use �ɓ����l.	�]���X�e�[�g.
//
enum {
	TRANS_IDLE =0,		// �]�� ���g�p.
	TRANS_INUSE=1,		// �]�� �g�p��.
	TRANS_DONE =2,		// �]�� ����.
};

//
///	�]���I���R�[���o�b�N�ɂ��A����������\����.(EP1������)
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

