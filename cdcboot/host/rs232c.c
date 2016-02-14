/*********************************************************************
 *	WIN32: RS232C の制御.
 *********************************************************************

 * 初期化・終了:
int		RS_init(char *portno,int baudrate);
void	RS_exit(void);

 * 無手順端末の実行:
void	RS_terminal(char *port_no,int baudrate);

 * データ送信:
int		RS_putdata(	unsigned char *buf , int cnt );
int		RS_putc(int c);
 * データ受信:
int		RS_checkdata();
int		RS_getdata(unsigned char *buf,int cnt);

 * キーボード入力:
int		RS_keyInput(void);

void	memdump(void *ptr,int len,int off);
int		UsbInit(int verbose,int enable_bulk);	
int		UsbExit(void);
void 	UsbBench(int cnt,int psize);
void	UsbDump(int adr,int arena,int size);
void 	UsbPoke(int adr,int arena,int data0,int data1);
int 	UsbPeek(int adr,int arena);
int 	UsbRead(int adr,int arena,uchar *buf,int size);
void 	UsbBench2(int cnt,int psize);
void 	UsbSetBaudRate(int ubrr,int bits);
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


#include "monit.h"
#include "w32term.h"
#include "rs232c.h"
/*********************************************************************
 *	定義
 *********************************************************************
 */
#define	    COM_DEVICENAME  "\\\\.\\COM%s"	
// 注意 "\\.\"を付けない場合は COMポート1～9までしか対応できない.
//				付けた場合は1～99まで.

#define		KEY_ESCAPE_CH	0x01

#define	MAX_RESULT_LENGTH	32		//受信バッファ長.

#define	RESULT_LENGTH_MASK	0x7f	//受信文字数の有効マスクビット.
#define	TXBUF_FULL_MASK		0x80	//送信バッファフルを示すビット.

#define	RS_EMPTY			(-9)

//#define	BENCH_LENGTH		80		//ベンチマーク: １行送信の長さ.
#define	BENCH_LENGTH		450		//ベンチマーク: １行送信の長さ.
#define	BENCH_COUNT			250		//ベンチマーク: １行送信の回数.
/*********************************************************************
 *	グローバル
 *********************************************************************
 */
static char	COM_Device[256];
HANDLE		thread;
DWORD		thid;
HANDLE		comHandle;
OVERLAPPED	ovlrd;
OVERLAPPED	ovlwr;
HANDLE		ovlrd_hEvent;
HANDLE		ovlwr_hEvent;

//void	Term_Log(int c);
//usb_dev_handle *usb_dev; /* the device handle */
//extern char *upfile;	//アップロードファイル名.
FILE   *upload_fp=NULL;
int		opt_d;


void	memdump(void *ptr,int len,int off);
/*********************************************************************
 *	ＲＳ２３２Ｃ 文字列データ 出力.
 *********************************************************************
 */
int	RS_putdata(	unsigned char *buf , int cnt )
{
	DWORD dwWritten=0;
	DWORD wcnt = 0;

	memset(&ovlwr,0,sizeof(OVERLAPPED));
	ovlwr.hEvent = ovlwr_hEvent;

//	buf[cnt]=0; printf("%02x\n",buf[0]);

	int rc = WriteFile(comHandle, buf , cnt , &dwWritten, &ovlwr);
	if (rc == 0) {
		if((rc = GetLastError() ) == ERROR_IO_PENDING) 
		{
//			WaitForSingleObject(ovlwr.hEvent, INFINITE);
			GetOverlappedResult(comHandle,&ovlwr,&wcnt,FALSE);
			//printf("putComPort() data = %x\n",buf[0]);
			return cnt;
		}
	}
	return cnt;
}

/*********************************************************************
 *	ＲＳ２３２Ｃ文字出力.
 *********************************************************************
 */
int	RS_putc(int c)
{
	u_char buf[16];
	buf[0]=c;
	RS_putdata(buf,1);
	return 0;
}
int	RS_puts(u_char *buf,int len)
{
	RS_putdata(buf,len);
	return 0;
}
/*********************************************************************
 *	コンソール入力
 *********************************************************************
 */
int	RS_keyInput(void)
{
	if(kbhit())	{
		return getch();
	}
	return 0;
}
/*********************************************************************
 *	ファイルアップロード機能を含むコンソール入力
 *********************************************************************
 */
int	RS_keyInputUP(void)
{
	if(upload_fp) {	//アップロード中.
		int c = fgetc(upload_fp);	//ファイルから文字を取得.
		if(c != EOF) {
			return c;
		}
		fclose(upload_fp);
		upload_fp = NULL;
	}
	return RS_keyInput();
}
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
	}else{

		// 遅延読み込み中・・・.
		if(( rc =GetLastError() ) == ERROR_IO_PENDING) {
			//printf("RxError2 rc = PENDING size=%d\n",cnt);
			WaitForSingleObject(ovlrd.hEvent, INFINITE);
			rc = GetOverlappedResult(comHandle,&ovlrd,&rcnt,FALSE);
			if(rc) {
				if(rcnt == cnt) 
				{
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
/*********************************************************************
 *	
 *********************************************************************
 */
int	RS_checkdata()
{
	DWORD DErr=0;
	COMSTAT commstat;

	ClearCommError(comHandle,&DErr,&commstat);
	return commstat.cbInQue ;

}

/*********************************************************************
 *	
 *********************************************************************
 */
int	RS_getc()
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
 *	ＣＯＭポートの初期化
 *********************************************************************
 */
int	RS_init(char *portno,int baudrate)
{
	static	DCB	com_dcb;
	DWORD 		DErr;

	sprintf(COM_Device,COM_DEVICENAME,portno);
	comHandle =	CreateFile(
		COM_Device,									/* シリアルポートの文字列 */
		GENERIC_READ | GENERIC_WRITE   ,			/* アクセスモード */
		0,
		NULL,										/* セキュリティ属性 */
		OPEN_EXISTING,								/* 作成フラグ	*/
//		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, /* 属性 */
		0,											/* 属性 */
		NULL										/* テンプレートのハンドル	*/
	);

	if(comHandle ==	INVALID_HANDLE_VALUE) {
		fprintf(stderr,	"Can't Open COM Port (%s:).\n",COM_Device + strlen("\\\\.\\") );
		exit(1);
	}

	ovlrd_hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	ovlwr_hEvent = CreateEvent(NULL,TRUE,TRUE ,NULL);

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
		EscapeCommFunction(comHandle, SETDTR);
		EscapeCommFunction(comHandle, SETRTS);
	}
	return 0;
}

/*********************************************************************
 *
 *********************************************************************
 */

void RS_exit(void)
{
	CloseHandle(ovlwr_hEvent);
	CloseHandle(ovlrd_hEvent);
	CloseHandle(comHandle);
}
/*********************************************************************
 *	エコーバック・サーバー.
 *********************************************************************
 *	CTRL+A で脱出する.
 *
 */
void RS_echohost(char *port_no,int baudrate)
{
	int c;
	unsigned char buf[16];
	RS_init(port_no,baudrate);			// 'COMx:' をオープンする.
	printf("Entering EchobackMode: Escape=^%c\n", '@' +	KEY_ESCAPE_CH );

	while(1) {
		c = RS_keyInput();if(c == KEY_ESCAPE_CH) break;	//脱出.
		if(	RS_checkdata() ) {
			RS_getdata(buf,1);
			c = buf[0];
			RS_putc(c);
			putchar(c);
			if(c == '\r') {
				c = '\n';
				RS_putc(c);
				putchar(c);
			}
		}
	}
	RS_exit();
}

void bench_tx()
{
	int i,c=' ';
	
#if	0
	for(i=0;i<BENCH_LENGTH;i++,c++) {
		RS_putc(c);
	}
#else
	u_char buf[BENCH_LENGTH+16];
	for(i=0;i<BENCH_LENGTH;i++,c++) {
		buf[i]=c;
	}
	RS_puts(buf,BENCH_LENGTH);
//	printf("Send data=\n");
//	memdump(buf,BENCH_LENGTH,0);
#endif
}

int bench_rx(void)
{
	int i,c=' ',r,errs=0;
	u_char buf[BENCH_LENGTH+1024];
	memset(buf,0,BENCH_LENGTH+1024);
	
	for(i=0;i<BENCH_LENGTH;i++,c++) {
		do {
			r = RS_getc();
		}while (r == RS_EMPTY);

		buf[i]=r;
		//memdump(buf+i,1,0);
		if(opt_d) {
			printf(" %02x",r);
		}

		if(r != (c & 0xff) ) {
			errs++;
		}
	}
	if(errs) {
		printf("Recv Error = %2d:\n", errs);
		memdump(buf,BENCH_LENGTH,0);
	}

	if(opt_d) {
		printf("\n");
	}

	return errs;
}

/*********************************************************************
 *	
 *********************************************************************
 *	CTRL+A で脱出する.
 *
 */
void RS_benchmark(char *port_no,int baudrate)
{
	int c,i;
	int count=BENCH_COUNT;
	int time1, rate;
	int total;

	RS_init(port_no,baudrate);			// 'COMx:' をオープンする.
	printf("Entering Benchmark: Escape=^%c\n", '@' + KEY_ESCAPE_CH );
	int time0 = clock();

	bench_tx();
	for(i=0;i<count;i++) {
		bench_tx();
		c = RS_keyInput();if(c == KEY_ESCAPE_CH) break;	//脱出.
		bench_rx();
		putchar('.');
		fflush(stdout);
	}
	bench_rx();
	
	printf("\nBenchmark end. ");
	time1 = clock() - time0;
	if (time1 == 0) {
		time1 = 2;
	}
	total = count * BENCH_LENGTH;
	rate = total * 1000 / time1;
   	printf("%d bytes/%d ms,  %d bytes/s (%d bps)\n",total,time1,rate,rate*10);

	RS_exit();
}
/*********************************************************************
 *	"COMXX:" ポートのプロパティを見る.
 *********************************************************************
 */
void RS_printCommInfo(char *comXX)
{
	BOOL	rc;
	HANDLE	com_handle;
	COMMPROP CommProp;

	printf("%s:",comXX);
	
	com_handle = CreateFile(
		comXX,									/* シリアルポートの文字列 */
		GENERIC_READ | GENERIC_WRITE   ,			/* アクセスモード */
		0,
		NULL,										/* セキュリティ属性 */
		OPEN_EXISTING,								/* 作成フラグ	*/
		0,											/* 属性 */
		NULL										/* テンプレートのハンドル	*/
	);
	if(	comHandle == INVALID_HANDLE_VALUE) {
		printf(" Can't Open .");
	}else{
		rc = GetCommProperties(com_handle,&CommProp);
		if(rc) {
			printf("TxQue=%ld,RxQue=%ld,Baud=0x%x,BaudMax=0x%x,Bits=0x%x,SubType=%x,Capa=%x "

				,CommProp.dwCurrentTxQueue	// バイト単位の送信バッファサイズ
				,CommProp.dwCurrentRxQueue  // バイト単位の受信バッファサイズ
				,CommProp.dwSettableBaud     // ボーレート最大値
				,CommProp.dwMaxBaud     // ボーレート最大値
				,CommProp.wSettableData      // DATABITS 5,6,7,8,16,16x

				,CommProp.dwProvSubType		 // 特定プロバイダータイプ
				,CommProp.dwProvCapabilities	 // サポートされた関数
			);
		}

#if	1
		static	DCB	com_dcb;
		memset(&com_dcb,0,sizeof(com_dcb));

		com_dcb.DCBlength = sizeof(DCB);
		rc = GetCommState(comHandle,&com_dcb);
//		if(rc) 
		{
//		com_dcb.BaudRate = baudrate;
//		com_dcb.ByteSize = 8;
//		com_dcb.Parity   = NOPARITY;
//		com_dcb.StopBits = ONESTOPBIT;
			printf("rc=%d Baud=%ld bits=%d parity=%d stop=%d",rc
				,com_dcb.BaudRate 
				,com_dcb.ByteSize 
				,com_dcb.Parity   
				,com_dcb.StopBits 
			);
		}
#endif
		CloseHandle(com_handle);

	}
	printf("\n");
}

#if	0
/* SERIAL_COMMPROP.SettableParams flags */
#define SERIAL_SP_PARITY                  0x0001
#define SERIAL_SP_BAUD                    0x0002
#define SERIAL_SP_DATABITS                0x0004
#define SERIAL_SP_STOPBITS                0x0008
#define SERIAL_SP_HANDSHAKING             0x0010
#define SERIAL_SP_PARITY_CHECK            0x0020
#define SERIAL_SP_CARRIER_DETECT          0x0040

/* SERIAL_COMMPROP.ProvCapabilities flags */
#define SERIAL_PCF_DTRDSR                 0x00000001
#define SERIAL_PCF_RTSCTS                 0x00000002
#define SERIAL_PCF_CD                     0x00000004
#define SERIAL_PCF_PARITY_CHECK           0x00000008
#define SERIAL_PCF_XONXOFF                0x00000010
#define SERIAL_PCF_SETXCHAR               0x00000020
#define SERIAL_PCF_TOTALTIMEOUTS          0x00000040
#define SERIAL_PCF_INTTIMEOUTS            0x00000080
#define SERIAL_PCF_SPECIALCHARS           0x00000100
#define SERIAL_PCF_16BITMODE              0x00000200
#endif

/*********************************************************************
 *	WIN32: レジストリを検索して、ＰＣに存在するCOM Portを列挙してみる.
 *********************************************************************
 */
int RS_printinfo(int pr)
{
	int   i;
	HKEY  hKey;
	char  szName[256];
	char  comXX[256];
	int   com_N=0;
	DWORD cbName, cbData, dwType;

	if(pr)printf("==== COM Port List====\n");

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,"HARDWARE\\DEVICEMAP\\SERIALCOMM",0,KEY_READ,&hKey) != ERROR_SUCCESS) {
		return 0;
	}
	for(i=0; /* nothing */ ;i++) {
		cbName = cbData = 256;
		if(RegEnumValue( hKey, i, szName, &cbName, NULL, &dwType, (BYTE *)comXX, &cbData ) != ERROR_SUCCESS ) {
			break;
		}
		if( dwType == REG_SZ ) {
			if(pr) RS_printCommInfo(comXX);
			sscanf(comXX+3,"%d",&com_N);
		}
	}
	RegCloseKey(hKey);
	return com_N;
}



/*********************************************************************
 *	
 *********************************************************************
 *	CTRL+A で脱出する.
 *
 */
void RS_terminal(char *port_no,int baudrate)
{
	int c;
	unsigned char buf[16];

//	if( upfile != NULL) {
//		upload_fp = fopen(upfile,"rb");
//	}

	RS_init(port_no,baudrate);			// 'COMx:' をオープンする.
	printf("Entering TerminalMode: Escape=^%c\n", '@' +	KEY_ESCAPE_CH );
	signal(SIGINT,SIG_IGN);	// ^C を無効にする.

	while(1) {
		c = RS_keyInputUP();
		if(c) {
			if(c == KEY_ESCAPE_CH) break;	//脱出.
			RS_putc(c);
		}
		if(	RS_checkdata() ) {
			RS_getdata(buf,1);
//			Term_Log(buf[0]);
			putchar(buf[0]);
		}
	}
	signal(SIGINT,SIG_DFL);
	RS_exit();
}
/*********************************************************************
 *
 *********************************************************************
 */

