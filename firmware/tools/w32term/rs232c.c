/*********************************************************************
 *	WIN32: RS232C の制御.
 *********************************************************************
 * 初期化・終了:
int		RS_init(int portno,int baudrate);
void	RS_exit(void);

 * データ送信:
int		RS_putdata(	unsigned char *buf , int cnt );
int		RS_putc(int c);
int		RS_puts(uchar *buf,int len);

 * データ受信:
int		RS_checkdata();
int		RS_getdata(unsigned char *buf,int cnt);
int		RS_getc();

 * ポート探索:
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
#include "fifo.h"
#include "rs232c.h"

/*********************************************************************
 *	定義
 *********************************************************************
 */
#define	    COM_DEVICENAME  "\\\\.\\COM%d"
// 注意 "\\.\"を付けない場合は COMポート1～9までしか対応できない.
//				付けた場合は1～99まで.

#define	OVERLAP_READWRITE	(0)		// ReadFile/WriteFileのオーバーラップ.

#define	TX_BUFFERING		(0)		// WriteFileをまとめる.

#define	RS_EMPTY			(-9)

#define	USE_THREAD			(0)		// Read用Threadを作成する.

#define	AAA5_negotiation	(0)
/*********************************************************************
 *	グローバル
 *********************************************************************
 */
HANDLE		thread;
DWORD		thid;
HANDLE		comHandle;


HANDLE 		hThread;
#define		RX_FIFO_SIZE	4096
char		rx_fifo_data[RX_FIFO_SIZE];
FIFO		rx_fifo;
int			kill_thread=0;

#if	OVERLAP_READWRITE	// ReadFile/WriteFileのオーバーラップ.
OVERLAPPED	ovlrd;
OVERLAPPED	ovlwr;
HANDLE		ovlrd_hEvent;
HANDLE		ovlwr_hEvent;
#endif

#if	TX_BUFFERING				// WriteFileをまとめる.
#define	TX_BUFFER_SIZE	(60*4)	//(60*7)
static unsigned char tx_put_buff[TX_BUFFER_SIZE+1];
static int	tx_put_cnt=0;
#endif


#if	OVERLAP_READWRITE	// ReadFile/WriteFileのオーバーラップ.
/*********************************************************************
 *	ＲＳ２３２Ｃ 文字列データ 出力.
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

	if( UsbIsMary() ) {
		Sleep(1);
	}

	if(cnt != dwWritten) {
		RS_error_exit("cnt != dwWritten");
	}
	return dwWritten;
}
#endif


#if	TX_BUFFERING				// WriteFileをまとめる.
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
	
	{	//ためる.
		memcpy(tx_put_buff + tx_put_cnt,buf,cnt);
		tx_put_cnt += cnt;
	}
	return cnt;
}
#else
int	RS_putdata(	unsigned char *buf , int cnt )
{
	// そのまま出力する.
	return RS_putdata_sub(buf,cnt);
}
#endif

/*********************************************************************
 *	ＲＳ２３２Ｃ送信バッファをフラッシュ.
 *********************************************************************
 */
#if	TX_BUFFERING				// WriteFileをまとめる.
int	RS_put_flush(void)
{
	if(tx_put_cnt) {
		// 出力する.
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
 *	ＲＳ２３２Ｃ文字出力.
 *********************************************************************
 */
int	RS_putc(int c)
{
	uchar buf[16];
	buf[0]=c;
	RS_putdata(buf,1);
	return 0;
}
#if	OVERLAP_READWRITE	// ReadFile/WriteFileのオーバーラップ.
/*********************************************************************
 *	COMMデバイスから１バイトを受信する.
 *********************************************************************
 *	戻り値:
 *		0 受信なし.
 *		1 受信ＯＫ.
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
			// 即時読み込み成功.
			return cnt;	//受信完了.
		}
		//printf("RxError1 rc = %d\n",rc);
		return 0;
	} else {

		// 遅延読み込み中・・・.
		if(( rc =GetLastError() ) == ERROR_IO_PENDING) {
			//printf("RxError2 rc = PENDING size=%d\n",cnt);
			WaitForSingleObject(ovlrd.hEvent, INFINITE);
			rc = GetOverlappedResult(comHandle,&ovlrd,&rcnt,FALSE);
			if(rc) {
				if(rcnt == cnt) {
					return cnt;	// 受信ＯＫ.
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
#if	USE_THREAD					// Read用Threadを作成する.
	return FIFO_dequeue(&rx_fifo,buf,cnt);
#else
	DWORD	r=0;
//	DWORD	rcnt = 0;

	RS_put_flush();

	int	rc = ReadFile( comHandle, buf , cnt , &r , NULL );
	(void)rc;
	return r;
#endif
}
#endif
/*********************************************************************
 *
 *********************************************************************
 */
int	RS_checkdata(void)
{
	RS_put_flush();

#if	USE_THREAD					// Read用Threadを作成する.
	return FIFO_getsize(&rx_fifo);
#else
	int rc;
	DWORD DErr=0;
	COMSTAT commstat;
	rc = ClearCommError(comHandle,&DErr,&commstat);
	if(rc==0) return 0;			// commstat 取得失敗.
	return commstat.cbInQue ;
#endif
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

#if	USE_THREAD					// Read用Threadを作成する.
void put_data(uchar *rx_data,int rxbytes)
{
	int i,c;
	for(i=0;i<rxbytes;i++) {
		c = *rx_data++;
		putchar(c);
	}
}


DWORD WINAPI rx_thread(LPVOID parms)
{
	uchar rx_data[1024];
	while (kill_thread==0) {
		DWORD rxbytes=0;
		ReadFile(comHandle, rx_data, sizeof(rx_data), &rxbytes, NULL);
		if(rxbytes) {
			FIFO_enqueue(&rx_fifo,rx_data,rxbytes);
//			put_data(rx_data,rxbytes);
		}
	}
	return 0;
}
#endif
/*********************************************************************
 *	ＣＯＭポートの初期化
 *********************************************************************
 */
int	RS_init(int portno,int baudrate)
{
	static	DCB	com_dcb;
	DWORD 		DErr;

	char	COM_Device[32];
	sprintf(COM_Device,COM_DEVICENAME,portno);
	comHandle =	CreateFile(
	                COM_Device,								/* シリアルポートの文字列 */
	                GENERIC_READ | GENERIC_WRITE   ,			/* アクセスモード */
	                0,
	                NULL,										/* セキュリティ属性 */
	                OPEN_EXISTING,								/* 作成フラグ	*/
#if	OVERLAP_READWRITE	// ReadFile/WriteFileのオーバーラップ.
					FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, /* 属性 */
#else
	                FILE_ATTRIBUTE_NORMAL,						/* 属性 */
#endif
	                NULL									/* テンプレートのハンドル */
	            );

	if(comHandle ==	INVALID_HANDLE_VALUE) {
		fprintf(stderr,	"Can't Open COM Port (%s:).\n",COM_Device + strlen("\\\\.\\") );
		exit(1);
	}
#if	OVERLAP_READWRITE	// ReadFile/WriteFileのオーバーラップ.
	ovlrd_hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	ovlwr_hEvent = CreateEvent(NULL,TRUE,TRUE ,NULL);
#endif
	{
		ClearCommError(comHandle,&DErr,NULL);
//		SetupComm(comHandle,8192,2048);
		SetupComm(comHandle,16384,1024);
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
#if	1	// MARYではこれをやるとリセットされる.
		EscapeCommFunction(comHandle, SETDTR);
		EscapeCommFunction(comHandle, SETRTS);
#endif
	}


#if	USE_THREAD					// Read用Threadを作成する.
/*	typedef struct _COMMTIMEOUTS {
		DWORD ReadIntervalTimeout;
		DWORD ReadTotalTimeoutMultiplier;
		DWORD ReadTotalTimeoutConstant;
		DWORD WriteTotalTimeoutMultiplier;
		DWORD WriteTotalTimeoutConstant;
	} COMMTIMEOUTS,*LPCOMMTIMEOUTS;
*/
	COMMTIMEOUTS timeout = { 0, 1, 1, 1, 1000};
	SetCommTimeouts(comHandle, &timeout);
	FIFO_init(&rx_fifo,rx_fifo_data,RX_FIFO_SIZE);

	DWORD ThreadID;
	hThread = CreateThread(NULL, 0, rx_thread, 0, 0, &ThreadID);
	if (hThread == INVALID_HANDLE_VALUE) {
		CloseHandle(comHandle);
		return -1;
	}
#endif
	return 0;
}

/*********************************************************************
 *
 *********************************************************************
 */

void RS_exit(void)
{
#if	OVERLAP_READWRITE	// ReadFile/WriteFileのオーバーラップ.
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
 *	"COMXX:" ポートのプロパティを見る.
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
	                 COM_Device,									/* シリアルポートの文字列 */
	                 GENERIC_READ | GENERIC_WRITE   ,			/* アクセスモード */
	                 0,
	                 NULL,										/* セキュリティ属性 */
	                 OPEN_EXISTING,								/* 作成フラグ	*/
	                 0,											/* 属性 */
	                 NULL										/* テンプレートのハンドル	*/
	             );
	if(	comHandle == INVALID_HANDLE_VALUE) {
		if(pr) {
			printf(" Can't Open .");
		}
	} else {
		rc = GetCommProperties(com_handle,&CommProp);
		if(rc && pr ) {
			printf("TxQue=%ld,RxQue=%ld,Baud=0x%lx,BaudMax=0x%lx,Bits=0x%x,SubType=%lx,Capa=%lx "

			       ,CommProp.dwCurrentTxQueue	// バイト単位の送信バッファサイズ
			       ,CommProp.dwCurrentRxQueue  // バイト単位の受信バッファサイズ
			       ,CommProp.dwSettableBaud    // ボーレート最大値
			       ,CommProp.dwMaxBaud     	// ボーレート最大値
			       ,CommProp.wSettableData     // DATABITS 5,6,7,8,16,16x

			       ,CommProp.dwProvSubType		// 特定プロバイダータイプ
			       ,CommProp.dwProvCapabilities// サポートされた関数
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
	RS_init(portno , baudrate);			// 'COMx:' をオープンする.
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
		Sleep(32);
		if(	RS_checkdata() ) {
			RS_getdata(buf,1);
			if(buf[0]==0x55) rc = 1;	// Ok.
		}
	}
	RS_exit();
	return rc;
}


/*********************************************************************
 *	WIN32: レジストリを検索して、ＰＣに存在するCOM Portを列挙してみる.
 *********************************************************************
 *引数:
 	int pr :  1 なら結果 を printする.
 *戻り値:
 	デバイスが存在しない場合は 0 を返す.
 	存在する場合は、接続するCOMxx 候補番号. 1～99
 */
int RS_printinfoAll(int port_no,int baudrate,int pr,int all)
{
	int   i,rc;
	HKEY  hKey;
	char  szName[256];
	char  comXX[256];

	char  com_port_list[100];	// COM 存在 スコアボード. 00～99
	int   com_N=0;
	DWORD cbName, cbData, dwType;

	memset(com_port_list,0,sizeof(com_port_list));

	if(pr)printf("==== COM Port List====\n");

	//
	// とりあえずレジストリを全部列挙する.
	//
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,"HARDWARE\\DEVICEMAP\\SERIALCOMM",0,KEY_READ,&hKey) != ERROR_SUCCESS) {
		return 0;		//レジストリに存在しなかった.
	}
	for(i=0; i<99 ; i++) {
		cbName = cbData = 256;
		if(RegEnumValue( hKey, i, szName, &cbName, NULL, &dwType, (BYTE *)comXX, &cbData ) != ERROR_SUCCESS ) {
			break;
		}
		if( dwType == REG_SZ ) {
			// 存在するポート番号のcom_port_list[n] にしるしをつける.
			com_N = 0;
			sscanf(comXX+3,"%d",&com_N);
			if((com_N>=1)&&(com_N<=99)) com_port_list[com_N]=1;	//存在する.
		}
	}
	RegCloseKey(hKey);

	//ユーザー指定があればそれを最優先.(実在する場合のみ)
	if((port_no>0)&&(port_no<=99)) {
		com_N = port_no;
		if(com_port_list[com_N]==1) {
			if(pr) {
				RS_printCommInfo(pr,com_N);
			}
			rc = RS_test_target(com_N,baudrate);
			if(rc) {
				return com_N;	//候補発見.
			}
		}
	}

	// COM99からCOM1に向かって、候補を絞る.
	for(com_N = 99; com_N >0 ; com_N--) {
		if(com_port_list[com_N]==1) {
			if(pr) {
				RS_printCommInfo(pr,com_N);
			}
#if	AAA5_negotiation
			rc = RS_test_target(com_N,baudrate);
			if(rc) {
				return com_N;	//候補発見.
			}
#else
			if(all == 0) {
				return com_N;	//候補発見.
			}
#endif
		}
	}

	return 0;	//候補無し.
}
/*********************************************************************
 *
 *********************************************************************
 */
int RS_printinfo(int port_no,int baudrate,int pr)
{
	return RS_printinfoAll(port_no,baudrate,pr,0);
}
/*********************************************************************
 *
 *********************************************************************
 */

