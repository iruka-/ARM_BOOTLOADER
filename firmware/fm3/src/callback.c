/***********************************************************************
 *		USB�]�������R�[���o�b�N
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

#define	USBREAD_USE_INTERRUPT		(1)		// ���荞�݂ɂ��USB��M.
#define	ECHOBACK_TEST				(0)

//
///	�]���I���R�[���o�b�N�ɂ��A����������\����.(EP16����p�ӂ��Ă���)
//
static TRANS_STATUS trans_status[16];

//
//	��M�o�b�t�@
//
static	uchar	recv_buffer[64];

//
///	�]���I���R�[���o�b�N�ɂ��A����������\���̂̃A�h���X�𓾂�.
//
static	TRANS_STATUS *UsbGetStatusP(int epid)
{
	return &trans_status[epid & 0x0f];
}

//
///	�]���I���R�[���o�b�N�̍\���̂����Z�b�g����.
//
void UsbClrEpSema(int epid,int flag)
{
	TRANS_STATUS *p = UsbGetStatusP(epid);
	
	p->in_use = flag;
	p->status = 0;
	p->size = 0;
}

//
///	�g�p���Ȃ� 1 , �]���I�������� 2 ��Ԃ�.
//
static	int UsbGetEpSema(int epid)
{
	TRANS_STATUS *p = UsbGetStatusP(epid);
	return p->in_use;	//
}

#if	0
//
///	�]���I�������� �p�P�b�g�T�C�Y(byte) ��Ԃ�.
//
static	int  UsbGetEpTransSize(int epid)
{
	TRANS_STATUS *p = UsbGetStatusP(epid);
	return p->size;	// 1=Done!
}
#endif
//
///	�]���I���R�[���o�b�N.
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

		// ���荞�݂ɂ��USB��M.�i�����j
		if(epid == USBF_CDC_BULKOUT_EP) {
			//led_xor_pat(0x10);
			USBenqueue(recv_buffer,count);	//!< ��M�ς݃f�[�^��FIFO�ɗ��߂�.
			UsbReadKick();					//!< ���̎�M���J�n����.
		}
    }else{	// ���荞�݂ɂ��USB��M.�i���s�j
		if(epid == USBF_CDC_BULKOUT_EP) {
			//led_xor_pat(0x80);
			UsbClrEpSema(USBF_CDC_BULKOUT_EP, TRANS_IDLE);	//!< �X�e�[�^�X�����Z�b�g����.
		}
	}
    return;
}



//
///	 ���̎�M���J�n����.
//
void UsbReadKick(void)
{
	int epid = USBF_CDC_BULKOUT_EP;
	int rc;

	//! FIFO�����t�̂Ƃ��́A�������Ȃ�.
	if(	USBgetfreesize() < 64 ) return;	// �L���[�ɋ󂫂�����.

	//! �G���h�|�C���g�g�p���̂Ƃ����������Ȃ�.
	if(	UsbGetEpSema(epid) != TRANS_INUSE ) 
	{
		UsbClrEpSema(epid, TRANS_INUSE);			//!< EP�g�p���ɂ���.
		rc = USBF_Receive(epid, recv_buffer , 64);	//!< ��M�L�b�N�����s.
		if(rc != USBF_RET_OK) {
			UsbClrEpSema(epid, TRANS_IDLE);			//!< ��M�v���͎��s.�X�e�[�g���Z�b�g.
		}
	}
}


//
///	USB�p�P�b�g���z�X�gPC�֑���֐�.
//

//	�߂�l�F�z�X�g�֑��M�����o�C�g��.(�d�v!)

int	usbSendBytes(uchar *buf,int size)
{
	int epid = USBF_CDC_BULKIN_EP;
	int rc;
	uint8_t status;
	static uchar sendbuf[64];	//�������^�[������ׂɁA�f�[�^���R�s�[����.

	//!	EP�g�p���̏ꍇ�́A�]��������҂�.
	while(1) {
//		if(	UsbGetEpSema(epid) != TRANS_INUSE ) break;
		status = 0;
		USBF_GetEpStatus(epid,&status);
		if(status != USBF_EPSTATUS_TRANS ) break;
	}

	//! ��x�ɑ��M�ł���o�C�g����64�܂�.
	if(size>64) size = 64;

	memcpy(sendbuf,buf,64);

	UsbClrEpSema(epid,TRANS_INUSE);
	rc = USBF_Send(epid , sendbuf , size );		//!< ������ςȂ��B�����҂����Ȃ�.
	if(rc != USBF_RET_OK) {
		UsbClrEpSema(epid, TRANS_IDLE);		//!<	���M�v���͎��s.
		return 0;							//!<	�]�������o�C�g�����O.
	}

	return size;
}

#if	ECHOBACK_TEST

#define	BUF_SIZE	256

//
//	�G�R�[�o�b�N�E�^�X�N.
//
void USBtask_Echoback(void)
{
	uchar buf[BUF_SIZE];

	int rxsize = USBavailable();
	if( rxsize >= BUF_SIZE ) {
		rxsize  = BUF_SIZE;
	}

	if( rxsize ) {
		USBread( buf, rxsize );		//!< ��MFIFO���� rxsize �o�C�g�����o��,
		USBwrite( buf, rxsize );	//!< �z�X�g�o�b�ɂ��̂܂ܕԑ�����.
	}
}

//
//	�G�R�[�o�b�N�E���[�v�ŌĂяo�����֐�.
//
void USB_loop(void)
{
	USBtask_Echoback();
}

#else
//
//	���j�^�[�E���[�v�ŌĂяo�����֐�.
//
void USB_loop(void)
{
	while (1) {
		int ch = USBgetch();		//!< �P������M.
		if(	ch == 0xaa) {
			ch = USBgetch();		//!< ����ɂP������M.
			if(	ch == 0x55) {		//!< 0xaa , 0x55 ��������monitor�R�}���h��t.
				JTAG_disable();
				USBmonit();
				led_on();
			} else if(ch == 0xa5) {	//!< 0xaa , 0xa5 �������� 0x55 ��Ԃ�(�����t).
				USBputch(0x55);
			}
		}
	}
}
#endif


