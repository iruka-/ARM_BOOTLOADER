/***********************************************************************
 *		USB ���C������
 ***********************************************************************
void	USB_loop(void);

int		usbSendBytes(uchar *buf,int size);
 */
#include "includes.h"
#include "usb.h"

#include "monitor/utype.h"
#include "monitor/usbio.h"
#include "monitor/monit.h"

/* ==== �}�N����` ==== */
#define BULK_OUT_PIPE	PIPE1
#define BULK_IN_PIPE	PIPE2
#define RCV_BUFSIZE		512

void CDC_snd_char(unsigned char c);
int CDC_rcv_buffercheck(void);  /* �V�K�ǉ� */
unsigned char CDC_rcv_char(void);
int CDC_start_snd( unsigned long size, unsigned char *top );
int CDC_start_rcv( unsigned long size, unsigned char *buf );
int CDC_check_complete( unsigned short Pipe );

#define	ECHOBACK_TEST				(0)


#define	MAX_SEND_SIZE	512	//63
//
///	USB�p�P�b�g���z�X�gPC�֑���֐�.
//

//	�߂�l�F�z�X�g�֑��M�����o�C�g��.(�d�v!)

int	usbSendBytes(uchar *buf,int size)
{
	if(size>=MAX_SEND_SIZE) size = MAX_SEND_SIZE;

	int rc = CDC_start_snd(size,buf);
#if	1
	if(	rc < 0 ) {
		return 0;			//���M���s.
	}

	while( !(CDC_check_complete(BULK_IN_PIPE)) ){
		/* wait */
	}
#endif
	return size;
}



#if	ECHOBACK_TEST

#define	BUF_SIZE	512

//
//	�G�R�[�o�b�N�E���[�v�ŌĂяo�����֐�.
//
void USB_loop(void)
{
	uchar buf[BUF_SIZE];
#if	0
	int i;
	for(i=0;i<50;i++) {
		WHITELED_flip(1);
		wait_ms(30);
	}
#endif

 while(1) {
	led_blink(18);
	int rxsize = USBavailable();
	if( rxsize >= BUF_SIZE ) {
		rxsize  = BUF_SIZE;
	}

	if( rxsize ) {
		USBread( buf, rxsize );		//!< ��MFIFO���� rxsize �o�C�g�����o��,
		USBwrite( buf, rxsize );	//!< �z�X�g�o�b�ɂ��̂܂ܕԑ�����.
	}
 }
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
				USBmonit();
			} else if(ch == 0xa5) {	//!< 0xaa , 0xa5 �������� 0x55 ��Ԃ�(�����t).
				USBputch(0x55);
			}
		}
	}
}
#endif


