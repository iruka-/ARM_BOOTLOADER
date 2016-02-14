/*********************************************************************
 *	packet.c
 *********************************************************************
 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>
#include <signal.h>
#include <time.h>
#include "rs232c.h"
#include "hidcmd.h"
#include "packet.h"
#include "rxbuffer.h"

#define	BUFF_SIZE		256
#define	PACKET_SIZE		60

#define DEBUG_PKTDUMP  	1		// パケットをダンプする.
#define	DEBUG_STREAM	0		// ストリームをダンプする.
#define	JTAG_DUMP		1		// 
#define	POKE_DUMP		1		// 

#define	SAFE_TEST		0

static	int dev_id        = 0;	// ターゲットID: STM32=0xa3 LPC1114=0xa0 LPC2388=0xa7
static	int dev_version   = 0;	// ターゲットバージョン hh.ll
static	int dev_bootloader= 0;	// ターゲットにbootloader機能がある.

static	int	flash_size     = 0;	// FLASH_ROMの最終アドレス+1
static	int	flash_end_addr = 0;	// FLASH_ROMの使用済み最終アドレス(4の倍数切捨)
static	int	flash_err_count= 0;	// FLASH_ROMの書き込みエラー.


#define	ErrPr	fprintf

/*!
 *	60バイトのパケットをCDCデバイスに送出する.
 *	----------------------------------------------------
 *	送信バッファの先頭の1バイトにReportID を入れる処理は
 *	この関数内で行うので、先頭1バイトを予約しておくこと.
 *
 *	引数:
 *		buf[60];
 *		cnt = PACKET_SIZE (60固定)
 *	戻り値:
 *		1= 成功.
 *		0= 失敗.
 */
static int hidWrite(uchar *buf, int cnt)
{
	buf[0]=0x01;

#if	DEBUG_PKTDUMP
	memdump("WR", (char*) buf, cnt);
#endif

	RS_putdata(	buf , cnt );
	return 1;
}

/*!
 *	60バイトのパケットをCDCデバイスから受け取る.
 *	----------------------------------------------------
 *	受信データは60バイト丸々使用できる.
 *
 *	引数:
 *		buf[60];
 *		cnt = PACKET_SIZE (60固定)
 *	戻り値:
 *		1= 成功.
 *		0= 失敗.
 */
int hidRead(uchar *buf, int cnt)
{
#if	DEBUG_PKTDUMP
	LOGFILE_print("RD: size=%d\n",cnt);
#endif

#if	0
	int rc;
	while(1) {
		if( RS_checkdata() >= cnt ) {
			rc = RS_getdata(buf,cnt);
			if(rc>0) break;
		}
	}
#else
	RS_getdata(buf,cnt);
#endif

#if	DEBUG_PKTDUMP
	memdump("RD", (char*) buf, cnt);
#endif
	return 1;
}

/*********************************************************************
 *	CDCデバイスに処理コマンドを送って、必要なら結果を受け取る.
 *********************************************************************
 *	cmdBuf   *cmd : 処理コマンド
 *	uchar    *buf : 結果を受け取る領域.
 *  int reply_len : 結果の必要サイズ. (0の場合は結果を受け取らない)
 *戻り値
 *		0	: 失敗
 *	   !0   : 成功
 */
static	int QueryAVR(cmdBuf *cmd,uchar *buf,int reply_len)
{
	int rc = 0;
	uchar result[256];

	rc = hidWrite((uchar*) cmd , REPORT_LENGTH );
	if(rc == 0) {
		LOGFILE_print("hidWrite error\n");
		UsbExit();
		return 0;
	}

	if(reply_len) {
		rc = hidRead(result , REPORT_LENGTH );
		if(rc == 0) {
			{
				LOGFILE_print("QueryAVR:hidRead error rc=%d\n",rc);
				UsbExit();
				return 0;
			}
		}
		memcpy(buf, result , reply_len);
	}
	return rc;
}

int	sendJtagCtrl(int command,uchar *stream)
{
	cmdBuf cmd;
	int	reply_len=0;
	uchar *result = NULL;

	jcmd_write_flash();

	memset(&cmd,0, 	  sizeof(cmdBuf));
	memcpy(cmd.buffer,stream,REPORT_LENGTH-2);	// 01,CMD,stream[...]

	cmd.cmd   = command;
	QueryAVR((cmdBuf *)&cmd,result,reply_len);

#if	DEBUG_STREAM			// 1:データ列をデバッグする.
	memdump("CTRL", (char*) &cmd, 32);
#endif
	return 0;
}

int	flash_get_status()
{
	uchar stream[BUFF_SIZE];
	uchar rd_data[BUFF_SIZE];
	memset(stream , 0, sizeof(stream) );

	sendJtagCtrl(HIDASP_TEST,stream);	// Connection test
	hidRead(rd_data , PACKET_SIZE);
	{
		int *fl_size;
		char *sBoot="(Application)";

		dev_id         = rd_data[1];
		dev_version    = rd_data[2] | (rd_data[3]<<8) ;
		dev_bootloader = rd_data[4];

		fl_size = (int*)&rd_data[8];
		flash_size     = fl_size[0];
		flash_end_addr = fl_size[1];
		flash_err_count= fl_size[2];

		if(dev_bootloader==1) {
			sBoot="(Bootloader)";
		}
		ErrPr(stderr, "TARGET DEV_ID=%x VER=%d.%d%s FLASH=%x,%x\n",dev_id
		        ,dev_version>>8,dev_version & 0xff,sBoot
		        ,flash_end_addr,flash_size
		       );
	}
	return flash_err_count;
}

void strconv(int *ret,const char *str)
{
	if(str==NULL) return;
	if((str[0]=='C')||(str[0]=='c')) {
		str += 3;	// skip "COM"
	}
	sscanf(str,"%d",ret);
}

//	成功=0を返す.
int hidasp_init(int type, const char *serial, const char *baud)
{
	int baudrate= BAUD_RATE;
	int portno  = 0;

	strconv(&baudrate,baud);	// 19200
	strconv(&portno,serial);	// comXX

	int com_n = RS_printinfo(portno,baudrate,1);
	if( com_n==0) return -1;		// NG
	RS_init(com_n,baudrate);			// 'COMx:' をオープンする.

#if	0
	int i;
	for(i=0; i<160; i++) {
		RS_putc(0xff);
	}
#endif

	RS_putc(0xaa);
	RS_putc(0x55);
	RS_put_flush();
	return flash_get_status();
}

//----------------------------------------------------------------------------
int	UsbGetDevCaps(int *pflash_size,int *pflash_used)
{
	if(pflash_size)	{
		*pflash_size = flash_size;
	}
	if(pflash_used)	{
		*pflash_used = flash_end_addr;
	}

	return dev_bootloader;
}

//----------------------------------------------------------------------------
//  終了.
//----------------------------------------------------------------------------
void hidasp_close()
{
	unsigned char buf[BUFF_SIZE];
	memset(buf , 0xff, sizeof(buf) );
//	hidWrite(buf, PACKET_SIZE);
	RS_putdata(buf, PACKET_SIZE);
	RS_put_flush();
	Sleep(4);
	RS_exit();
}


void memdump(char *msg, char *buf, int len)
{
	int j;
	LOGFILE_print( "%s", msg);
	for (j = 0; j < len; j++) {
		LOGFILE_print(" %02x", buf[j] & 0xff);
		if((j & 0xf)==  7)
			LOGFILE_print( " ");

		if((j & 0xf)== 15)
			LOGFILE_print( "\n +");
	}
	LOGFILE_print( "\n");
}

int hidasp_list(char * string)
{
	return 0;
}

/*********************************************************************
 *
 *********************************************************************
 */


#define	MAX_JCMD_SIZE	(REPORT_LENGTH-4)	//58

/*********************************************************************
 *	初期化
 *********************************************************************
 */
int UsbInit(int verbose,int enable_bulk, char *serial,char *baud)
{
	int type = 0;

//	ErrPr(stderr, "UsbInit('%s','%s')\n",serial,baud);

	if(	hidasp_init(type,serial,baud) != 0) {
		if (verbose) {
			if(serial==NULL) serial="";
			if(baud  ==NULL) baud  ="";

			ErrPr(stderr, "error: cdclink COM Port %s %s device not found!\n",serial,baud);
		}
		return 0;
	} else {
		return dev_id;	//1;	// OK.
	}
}
/*********************************************************************
 *	終了
 *********************************************************************
 */
int UsbExit(void)
{
	hidasp_close();
	return 0;
}
int UsbIsMary(void)
{
	if( dev_id == 0xa0 ) return 1;
	return 0;
}


/*********************************************************************
 *	JTAGコマンド発行
 *********************************************************************
 */
static	uchar j_buf[128];
static	int	  j_index=0;
static	int	  j_anchor=0;


void	bang_get_result(uint8_t *buffer,uchar *result,int count);

/*********************************************************************
 *	バッファリングしない 即時 JTAG コマンド発行
 *********************************************************************
 */
int jcmd_write_nb(uchar *stream,int size,uchar *result,int bitcnt)
{
	cmdBuf cmd;
	uchar *t;
	int  rc=0;

//	int  rsize=0;
//	if(result!=NULL) rsize = size;

#if	RX_DEFFERED				//遅延読み込みを許可.
	rxExecuteBuf();
#endif

#if	JTAG_DUMP
	{
		int i;
		LOGFILE_print("JTAG:");
		for(i=0; i<size; i++) {
			LOGFILE_print(" %02x",stream[i]);
			if( (i % 24) == 23 ) {
				LOGFILE_print("\n    +");
			}
		}
		LOGFILE_print("\n");
	}
#endif

	t = (uchar*)&cmd;
	memset(t,0,sizeof(cmdBuf));
	if(result==NULL) {
		cmd.cmd   = HIDASP_JTAG_WRITE;
	} else {
		cmd.cmd   = HIDASP_JTAG_READ;
	}
	memcpy(t+2,stream,size);

//
//	rc = QueryAVR(&cmd,result,rsize);
//
	rc = hidWrite(t , REPORT_LENGTH );
	if(rc == 0) {
		LOGFILE_print("hidWrite error\n");
		UsbExit();
		return 0;
	}

#if	RX_DEFFERED				//遅延読み込みを許可.
	if(result) {
		rxRequestBuf(result,bitcnt);	//非同期受信リクエスト.
//		rxExecuteBuf();
//		rxFlushBuf();
	}
#else
	if(result) {
		uchar rbuf[256];
		rc = hidRead(rbuf , REPORT_LENGTH );
		if(rc == 0) {
			LOGFILE_print("QueryAVR:hidRead error rc=%d\n",rc);
			UsbExit();
			return 0;
		}
		bang_get_result(result,rbuf,bitcnt);
	}
#endif


#if	JTAG_DUMP
	if(result!=NULL) {
		int i;
		int dsize = (bitcnt+7)/8;
		LOGFILE_print("RET :");
		for(i=0; i<dsize; i++) {
			LOGFILE_print(" %02x",result[i]);
		}
		LOGFILE_print("\n");
	}
#endif

	return rc;
}

/*********************************************************************
 *	USB側へ送るJTAGコマンドがキューに残っていたら全部送信する.
 *********************************************************************
 */
int jcmd_write_flash()
{
	if(	j_index ) {
		jcmd_write_nb(j_buf,j_index,NULL,0);
		j_index=0;
	}
	return 0;
}

/*********************************************************************
 *	JTAGコマンドを連結する処理.
 *********************************************************************
 */
int jcmd_add(uchar *stream,int size)
{
	int c1,c2;
	// 初回連結: 初期データが空の場合.
	if(	j_index == 0) {
		j_anchor = 0;				//直前のコントロールワードの場所.
		memcpy(j_buf,stream,size);	//丸コピーする.
		j_index = size;
		return size;
	}
	// 初回でない連結: 初期データが存在する場合.
	c1 = j_buf[j_anchor];
	c2 = stream[0];
	if((c1 & 0x80) && (c2 & 0x80)) {	//両方ともbitbangストリームの場合は１つのストリームにしてしまう.
		memcpy(j_buf+j_index-1,stream+1,size-1);	// 初期データのNULターミネート位置にbitbangをコピー.
		j_buf[j_anchor] = 0x80 | ((c1&0x7f)+(c2&0x7f));	//長さを書き直す.
		j_index += size-2;							// 接続部分のNUL文字とbitbang制御コードが１つづつ減るので-2
		// anchorの位置は変わらない.
		return j_index;
	}

	// 初回でない連結: bitbang同士で無い場合.
	{
		j_anchor = j_index-1;
		memcpy(j_buf+j_anchor,stream,size);		// 初期データのNULターミネート位置にコピー.
		j_index += size-1;						// 接続部分のNUL文字が１つ減るので-1
		return j_index;
	}
}


/*********************************************************************
 *	USB側へ送るJTAGコマンドをキューイングする.
 *********************************************************************
 */
int jcmd_write(uchar *stream,int size,uchar *result,int bitcnt)
{
	//読み書きを伴う場合はバッファ不可.
	if(result) {
		jcmd_write_flash();
		return jcmd_write_nb(stream,size,result,bitcnt);
	}

	// 連結後のサイズがMAX_JCMD_SIZEを越えるのが分かっている場合は
	//   いったん吐き出す.
	if( (j_index+size)>=MAX_JCMD_SIZE ) {
		jcmd_write_flash();
	}

	// 連結を実行する.
	jcmd_add(stream,size);
#if	SAFE_TEST
		jcmd_write_flash();
#endif
	return j_index;
}

//  BitBangデータビット.

#define	bitMCLR	0x08
#define	bitPGM	0x04
#define	bitPGD	0x02
#define	bitPGC	0x01

//	方向レジスタ (0=出力)

#define	dirMCLR	0x80
#define	dirPGM	0x40
#define	dirPGD	0x20
#define	dirPGC	0x10

/*********************************************************************
 *	TCK ,TDI ,TDO ,TMS ポートの入出力方向を決める.
 *********************************************************************
 */
int	SetJtagPort(int dir,int data0)
{
	uchar stream[REPORT_LENGTH-2];

#if	JTAG_DUMP
	LOGFILE_print("SetJtagPort(%x,%x)\n",dir,data0);
#endif

	memset(stream,0,sizeof(stream));
	stream[0]= 0x80 | 1;	// set dir:1 | count:1 
	stream[1]= dir & 0x3f;	// bit5:4 = TRST,NRST , bit3:0 = TMS,TDI,TDO,TCK

	stream[2]= 0x00 | 1;	// set dir:0 | count:1 
	stream[3]= data0 & 0x3f;// bit5:4 = TRST,NRST , bit3:0 = TMS,TDI,TDO,TCK

	stream[4]=0;
	return sendJtagCtrl(HIDASP_JTAG_CTRLW,stream);
}

//
//	speed = 64000 / khz 
//		6000khz = 10
//		3000khz = 21
//		1000khz = 64
//		 100khz = 640
//		  10khz = 6400
//		   1khz = 64000 (65535 max)
//
int	SetJtagSpeed(int speed)
{
	uchar stream[REPORT_LENGTH-2];

#if	JTAG_DUMP
	LOGFILE_print("SetJtagSpeed(%d)\n",speed);
#endif

	speed = (speed * 4) / 64;	// 1000 khzのとき 4になる.
	speed -= 3;					// subtract overhead.

	if(speed < 0)  speed = 0;
	if(speed >400) speed = 400;

	//
	//	speed = 250ns単位の待ち. (4Mhzの逆数)
	//
	memset(stream,0,sizeof(stream));
	stream[0]= 0x80;	// set dir:1 | count:1 
	stream[1]= speed & 0xff;
	stream[2]=(speed>>8) & 0xff;
	stream[3]=0;

	return sendJtagCtrl(HIDASP_JTAG_CTRLW,stream);
}

//	たぶん2mS単位.
static void jtag_wait_msec(int ms)
{
	int i;
	uchar stream[BUFF_SIZE];
	uchar rd_data[BUFF_SIZE];

	memset(stream , 0, sizeof(stream) );

	for(i=0;i<ms;i++) {
		sendJtagCtrl(HIDASP_TEST,stream);	// Connection test
		hidRead(rd_data , PACKET_SIZE);
	}
}

void jtag_sleep(int us)
{
	int msec = us / 2000;
	if( msec == 0 ) msec = 1;

#if	JTAG_DUMP
	LOGFILE_print("jtag_sleep(%d uSec)\n",us);
#endif
	jtag_wait_msec(msec);
}

#if	0
/*********************************************************************
 *	TCK ,TDI ,TDO ,TMS ポートの入出力方向を決める.
 *********************************************************************
 */
int set_ddrb(int d)
{
	return SetJtagPort( (d >> 4) & 0x0f , 0 );
}
#endif
/*********************************************************************
 *
 *********************************************************************
 */
