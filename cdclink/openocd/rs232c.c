/*********************************************************************
 *	WIN32: RS232C �̐���.
 *********************************************************************
 * �������E�I��:
int		RS_init(int portno,int baudrate);
void	RS_exit(void);

 * �f�[�^���M:
int		RS_putdata(	unsigned char *buf , int cnt );
int		RS_putc(int c);
int		RS_puts(u_char *buf,int len);

 * �f�[�^��M:
int		RS_checkdata();
int		RS_getdata(unsigned char *buf,int cnt);
int		RS_getc();

 * �|�[�g�T��:
int		RS_printCommInfo(int pr,char *comXX);
int 	RS_printinfo(int port_no,int baudrate,int pr);

 *
 *********************************************************************
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <signal.h>
#include <time.h>

#include "packet.h"
#include "rs232c.h"

/*********************************************************************
 *	��`
 *********************************************************************
 */
#define	    COM_DEVICENAME  "\\\\.\\COM%d"
// ���� "\\.\"��t���Ȃ��ꍇ�� COM�|�[�g1�`9�܂ł����Ή��ł��Ȃ�.
//				�t�����ꍇ��1�`99�܂�.

#define	OVERLAP_READWRITE	(0)		// ReadFile/WriteFile�̃I�[�o�[���b�v.

#define	TX_BUFFERING		(1)		// WriteFile���܂Ƃ߂�.

#define	RESULT_LENGTH_MASK	0x7f	//��M�������̗L���}�X�N�r�b�g.
#define	TXBUF_FULL_MASK		0x80	//���M�o�b�t�@�t���������r�b�g.

#define	RS_EMPTY			(-9)

/*********************************************************************
 *	�O���[�o��
 *********************************************************************
 */
HANDLE		thread;
DWORD		thid;
HANDLE		comHandle;


#if	OVERLAP_READWRITE	// ReadFile/WriteFile�̃I�[�o�[���b�v.
OVERLAPPED	ovlrd;
OVERLAPPED	ovlwr;
HANDLE		ovlrd_hEvent;
HANDLE		ovlwr_hEvent;
#endif

#if	TX_BUFFERING				// WriteFile���܂Ƃ߂�.
#define	TX_BUFFER_SIZE	(60*4)	//(60*7)
static unsigned char tx_put_buff[TX_BUFFER_SIZE+1];
static int	tx_put_cnt=0;
#endif

// mary�̎�M�o�b�t�@(2048byte)����ꂳ���Ȃ��悤�ɂ���.
static int	mary_put_bytes=0;	//�A�����M���ꂽ�o�C�g�����J�E���g����.


#if	OVERLAP_READWRITE	// ReadFile/WriteFile�̃I�[�o�[���b�v.
/*********************************************************************
 *	�q�r�Q�R�Q�b ������f�[�^ �o��.
 *********************************************************************
 */
int	RS_putdata_sub(	unsigned char *buf , int cnt )
{
	DWORD dwWritten=0;
	DWORD wcnt = 0;

	memset(&ovlwr,0,sizeof(OVERLAPPED));
	ovlwr.hEvent = ovlwr_hEvent;

//	buf[cnt]=0; printf("%02x\n",buf[0]);

	int rc = WriteFile(comHandle, buf , cnt , &dwWritten, &ovlwr);
	if (rc == 0) {
		if((rc = GetLastError() ) == ERROR_IO_PENDING) {
//			WaitForSingleObject(ovlwr.hEvent, INFINITE);
			GetOverlappedResult(comHandle,&ovlwr,&wcnt,FALSE);
			//printf("putComPort() data = %x\n",buf[0]);
			return cnt;
		}
	}
	return cnt;
}
#else
int	RS_putdata_sub(	unsigned char *buf , int cnt )
{
	DWORD dwWritten=0;
	int rc = WriteFile(comHandle, buf , cnt , &dwWritten, NULL);
	(void)rc;

	memdump("RS_putdata",buf,cnt);

	// mary�̎�M�o�b�t�@(2048byte)����ꂳ���Ȃ��悤�ɂ���.
	mary_put_bytes += cnt;
	if( UsbIsMary() ) {
		if(	mary_put_bytes>=512) {
			mary_put_bytes=0;
			Sleep(1);
		}
	}

	if(cnt != dwWritten) {
		RS_error_exit("cnt != dwWritten");
	}
	return dwWritten;
}
#endif


#if	TX_BUFFERING				// WriteFile���܂Ƃ߂�.
int	RS_putdata(	unsigned char *buf , int cnt )
{
#if	0
	if( UsbIsMary() ) {
		int rc = RS_putdata_sub(buf,cnt);
		Sleep(1);
		return rc;
	}
#endif
	if( (tx_put_cnt+cnt) > TX_BUFFER_SIZE ) {
		RS_put_flush();
	}
	
	{	//���߂�.
		memcpy(tx_put_buff + tx_put_cnt,buf,cnt);
		tx_put_cnt += cnt;
	}
	return cnt;
}
#else
int	RS_putdata(	unsigned char *buf , int cnt )
{
	// ���̂܂܏o�͂���.
	return RS_putdata_sub(buf,cnt);
}
#endif

/*********************************************************************
 *	�q�r�Q�R�Q�b���M�o�b�t�@���t���b�V��.
 *********************************************************************
 */
#if	TX_BUFFERING				// WriteFile���܂Ƃ߂�.
int	RS_put_flush(void)
{
	if(tx_put_cnt) {
		// �o�͂���.
		int rc = RS_putdata_sub(tx_put_buff , tx_put_cnt);
		tx_put_cnt = 0;
		return rc;
	}
	return 0;
}
#else
int	RS_put_flush(void)
{
	return 0;
}
#endif
/*********************************************************************
 *	�q�r�Q�R�Q�b�����o��.
 *********************************************************************
 */
int	RS_putc(int c)
{
	u_char buf[16];
	buf[0]=c;
	RS_putdata(buf,1);
	return 0;
}
#if	OVERLAP_READWRITE	// ReadFile/WriteFile�̃I�[�o�[���b�v.
/*********************************************************************
 *	COMM�f�o�C�X����P�o�C�g����M����.
 *********************************************************************
 *	�߂�l:
 *		0 ��M�Ȃ�.
 *		1 ��M�n�j.
 *
 *
 */
int	RS_getdata(unsigned char *buf,int cnt)
{
	DWORD	r=0;
	DWORD	rcnt = 0;

	RS_put_flush();

	memset(&ovlrd,0,sizeof(OVERLAPPED));
	ovlrd.hEvent = ovlrd_hEvent;

	int	rc = ReadFile( comHandle, buf , cnt , &r , &ovlrd );
	if (rc) {
		if(r == cnt) {
			// �����ǂݍ��ݐ���.
			return cnt;	//��M����.
		}
		//printf("RxError1 rc = %d\n",rc);
		return 0;
	} else {

		// �x���ǂݍ��ݒ��E�E�E.
		if(( rc =GetLastError() ) == ERROR_IO_PENDING) {
			//printf("RxError2 rc = PENDING size=%d\n",cnt);
			WaitForSingleObject(ovlrd.hEvent, INFINITE);
			rc = GetOverlappedResult(comHandle,&ovlrd,&rcnt,FALSE);
			if(rc) {
				if(rcnt == cnt) {
					return cnt;	// ��M�n�j.
				}
			}
			//printf("RxError3 rc = %d\n",rc);
			return 0;
		}
		//printf("RxError4 rc = %d\n",rc);
		return 0;
	}
	return 0;
}
#else
int	RS_getdata(unsigned char *buf,int cnt)
{
	int ret = cnt;
	DWORD	r;
	int rc;

	RS_put_flush();
	mary_put_bytes=0;

	while(cnt) {
		r=0;
		rc = ReadFile( comHandle, buf , cnt , &r , NULL );
		buf += r;
		cnt -= r;
	}
	(void)rc;
	return ret;
}
#endif
/*********************************************************************
 *
 *********************************************************************
 */
int	RS_checkdata(void)
{
	DWORD DErr=0;
	COMSTAT commstat;

	RS_put_flush();

	ClearCommError(comHandle,&DErr,&commstat);
	return commstat.cbInQue ;

}

/*********************************************************************
 *
 *********************************************************************
 */
int	RS_getc(void)
{
	int c;
	unsigned char buf[16];

	if(	RS_checkdata() ) {
		RS_getdata(buf,1);
		c = buf[0];
		return c & 0xff;
	}

	return RS_EMPTY;	// Empty
}

/*********************************************************************
 *	�b�n�l�|�[�g�̏�����
 *********************************************************************
 */
int	RS_init(int portno,int baudrate)
{
	static	DCB	com_dcb;
	DWORD 		DErr;

	char	COM_Device[32];
	sprintf(COM_Device,COM_DEVICENAME,portno);
	comHandle =	CreateFile(
	                COM_Device,								/* �V���A���|�[�g�̕����� */
	                GENERIC_READ | GENERIC_WRITE   ,			/* �A�N�Z�X���[�h */
	                0,
	                NULL,										/* �Z�L�����e�B���� */
	                OPEN_EXISTING,								/* �쐬�t���O	*/
#if	OVERLAP_READWRITE	// ReadFile/WriteFile�̃I�[�o�[���b�v.
					FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, /* ���� */
#else
	                FILE_ATTRIBUTE_NORMAL,						/* ���� */
#endif
	                NULL									/* �e���v���[�g�̃n���h�� */
	            );

	if(comHandle ==	INVALID_HANDLE_VALUE) {
		fprintf(stderr,	"Can't Open COM Port (%s:).\n",COM_Device + strlen("\\\\.\\") );
		exit(1);
	}
#if	OVERLAP_READWRITE	// ReadFile/WriteFile�̃I�[�o�[���b�v.
	ovlrd_hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	ovlwr_hEvent = CreateEvent(NULL,TRUE,TRUE ,NULL);
#endif
	{
		ClearCommError(comHandle,&DErr,NULL);
		SetupComm(comHandle,8192,2048);
		PurgeComm(comHandle,PURGE_TXABORT | PURGE_RXABORT |PURGE_TXCLEAR |	PURGE_RXCLEAR);

		memset(&com_dcb,0,sizeof(com_dcb));
		com_dcb.DCBlength = sizeof(DCB);
		GetCommState(comHandle,&com_dcb);

		com_dcb.BaudRate = baudrate;
		com_dcb.ByteSize = 8;
		com_dcb.Parity   = NOPARITY;
		com_dcb.StopBits = ONESTOPBIT;
		SetCommState(comHandle,&com_dcb);

		SetCommMask(comHandle,0);
		SetCommMask(comHandle,EV_RXCHAR);

		EscapeCommFunction(comHandle, CLRBREAK);
#if	0	// MARY�ł͂�������ƃ��Z�b�g�����.
		EscapeCommFunction(comHandle, SETDTR);
		EscapeCommFunction(comHandle, SETRTS);
#endif
	}
	return 0;
}

/*********************************************************************
 *
 *********************************************************************
 */

void RS_exit(void)
{
#if	OVERLAP_READWRITE	// ReadFile/WriteFile�̃I�[�o�[���b�v.
	CloseHandle(ovlwr_hEvent);
	CloseHandle(ovlrd_hEvent);
#endif
	CloseHandle(comHandle);
}
/*********************************************************************
 *
 *********************************************************************
 */
void	RS_error_exit(char *msg)
{
	fprintf(stderr,	"FATAL: COM Port Read/Write Error. %s\n",msg);
	RS_exit();
	exit(-1);
}

/*********************************************************************
 *	"COMXX:" �|�[�g�̃v���p�e�B������.
 *********************************************************************
 */
int RS_printCommInfo(int pr,int port_no)
{
	BOOL	rc;
	HANDLE	com_handle;
	COMMPROP CommProp;
	char comXX[32];
	sprintf(comXX,"COM%d",port_no);
	char	COM_Device[32];
	sprintf(COM_Device,COM_DEVICENAME,port_no);

	if(pr) {
		printf("%s:",comXX);
	}
	com_handle = CreateFile(
	                 COM_Device,									/* �V���A���|�[�g�̕����� */
	                 GENERIC_READ | GENERIC_WRITE   ,			/* �A�N�Z�X���[�h */
	                 0,
	                 NULL,										/* �Z�L�����e�B���� */
	                 OPEN_EXISTING,								/* �쐬�t���O	*/
	                 0,											/* ���� */
	                 NULL										/* �e���v���[�g�̃n���h��	*/
	             );
	if(	comHandle == INVALID_HANDLE_VALUE) {
		if(pr) {
			printf(" Can't Open .");
		}
	} else {
		rc = GetCommProperties(com_handle,&CommProp);
		if(rc && pr ) {
			printf("TxQue=%ld,RxQue=%ld,Baud=0x%lx,BaudMax=0x%lx,Bits=0x%x,SubType=%lx,Capa=%lx "

			       ,CommProp.dwCurrentTxQueue	// �o�C�g�P�ʂ̑��M�o�b�t�@�T�C�Y
			       ,CommProp.dwCurrentRxQueue  // �o�C�g�P�ʂ̎�M�o�b�t�@�T�C�Y
			       ,CommProp.dwSettableBaud    // �{�[���[�g�ő�l
			       ,CommProp.dwMaxBaud     	// �{�[���[�g�ő�l
			       ,CommProp.wSettableData     // DATABITS 5,6,7,8,16,16x

			       ,CommProp.dwProvSubType		// ����v���o�C�_�[�^�C�v
			       ,CommProp.dwProvCapabilities// �T�|�[�g���ꂽ�֐�
			      );
		}

		CloseHandle(com_handle);
	}

	if(pr) {
		printf("\n");
	}

	return 1;
}

/*********************************************************************
 *
 *********************************************************************
 */
int RS_test_target(int portno,int baudrate)
{
	int rc = 0;
	uchar buf[256];
	RS_init(portno , baudrate);			// 'COMx:' ���I�[�v������.
	{
#if	0	// SYNC?
		int i;
		for(i=0;i<60;i++) {
			RS_putc(0xff);
		}
#endif
		RS_putc(0xaa);
		RS_putc(0xa5);
		RS_put_flush();
		Sleep(8);
		if(	RS_checkdata() ) {
			RS_getdata(buf,1);
			if(buf[0]==0x55) rc = 1;	// Ok.
		}
	}
	RS_exit();
	return rc;
}


/*********************************************************************
 *	WIN32: ���W�X�g�����������āA�o�b�ɑ��݂���COM Port��񋓂��Ă݂�.
 *********************************************************************
 *����:
 	int pr :  1 �Ȃ猋�� �� print����.
 *�߂�l:
 	�f�o�C�X�����݂��Ȃ��ꍇ�� 0 ��Ԃ�.
 	���݂���ꍇ�́A�ڑ�����COMxx ���ԍ�. 1�`99
 */
int RS_printinfo(int port_no,int baudrate,int pr)
{
	int   i,rc;
	HKEY  hKey;
	char  szName[256];
	char  comXX[256];

	char  com_port_list[100];	// COM ���� �X�R�A�{�[�h. 00�`99
	int   com_N=0;
	DWORD cbName, cbData, dwType;

	memset(com_port_list,0,sizeof(com_port_list));

	if(pr)printf("==== COM Port List====\n");

	//
	// �Ƃ肠�������W�X�g����S���񋓂���.
	//
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,"HARDWARE\\DEVICEMAP\\SERIALCOMM",0,KEY_READ,&hKey) != ERROR_SUCCESS) {
		return 0;		//���W�X�g���ɑ��݂��Ȃ�����.
	}
	for(i=0; i<99 ; i++) {
		cbName = cbData = 256;
		if(RegEnumValue( hKey, i, szName, &cbName, NULL, &dwType, (BYTE *)comXX, &cbData ) != ERROR_SUCCESS ) {
			break;
		}
		if( dwType == REG_SZ ) {
			// ���݂���|�[�g�ԍ���com_port_list[n] �ɂ��邵������.
			com_N = 0;
			sscanf(comXX+3,"%d",&com_N);
			if((com_N>=1)&&(com_N<=99)) com_port_list[com_N]=1;	//���݂���.
		}
	}
	RegCloseKey(hKey);

	//���[�U�[�w�肪����΂�����ŗD��.(���݂���ꍇ�̂�)
	if((port_no>0)&&(port_no<=99)) {
		com_N = port_no;
		if(com_port_list[com_N]==1) {
			if(pr) {
				RS_printCommInfo(pr,com_N);
			}
			rc = RS_test_target(com_N,baudrate);
			if(rc) {
				return com_N;	//��┭��.
			}
		}
	}

	// COM99����COM1�Ɍ������āA�����i��.
	for(com_N = 99; com_N >0 ; com_N--) {
		if(com_port_list[com_N]==1) {
			if(pr) {
				RS_printCommInfo(pr,com_N);
			}
			rc = RS_test_target(com_N,baudrate);
			if(rc) {
				return com_N;	//��┭��.
			}
		}
	}

	return 0;	//��△��.
}
/*********************************************************************
 *
 *********************************************************************
 */

