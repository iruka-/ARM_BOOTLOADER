/********************************************************************
 *	簡易モニタ
 ********************************************************************
 */
#include <string.h>

#include "hidcmd.h"
#include "monit.h"
#include "picwrt.h"
#include "gpiodef.h"

#include "sampling.h"
#include "usbio.h"


/********************************************************************
 *	スイッチ
 ********************************************************************
 */

/********************************************************************
 *	定義
 ********************************************************************
 */
#if defined(MARY)
#define	DEVID	0xa0
#endif

#if defined(LPC2388)
#define	DEVID	0xa7
#endif

#if defined(FM3)
#define	DEVID	0xf3
#endif

#if defined(RX62N)
#define	DEVID	0x62
#endif

#if defined(SH2A)
#define	DEVID	0x2a
#endif

#ifndef	DEVID
#define	DEVID	DEV_ID_ARM_M3		//0xa3
#endif

#define	SOFTWARE_SPI_MODE	1

#define	VER_H	1
#define	VER_L	1

#if	_ROMADRS
#define	DEV_LOADER	0
#else
#define	DEV_LOADER	1
#endif

#define	MONIT_SOF		0x01
#define	REPLY_REPORT	0xaa

extern void user_cmd(int arg);

#define	SAMPLING_SIZE	2048	//1040			// 16+1024
uchar	sampling_buf[SAMPLING_SIZE];

//
// ===        STM32以外でのFlash Programming 実装.
//
#if	0	//ndef	SUBMDL_STM32F103

// LPC1114 / LPC2388
//翻訳表:
//		STM32_library			  | wirish(C++) library
#define	FLASH_ErasePage(adrs)  		/*flashErasePage(adrs)*/
#define	FLASH_ProgramWord(adr,p)	/*flashWriteWord(adr,p)	*/
#define	FLASH_Lock()				/*flashLock()*/
#define	FLASH_Unlock()				/*flashUnlock()*/
//
void	FLASH_ProgramPage(int adr);

#endif	//	SUBMDL_STM32F103

/********************************************************************
 *	データ
 ********************************************************************
 */

//	AVR書き込み用コンテクスト.
uchar  page_mode;
uchar  page_addr;
uchar  page_addr_h;

//	データ引取りモードの設定.
uchar   poll_mode;	// 00=無設定  0xa0=アナログサンプル  0xc0=デジタルサンプル
// 0xc9=runコマンドターミネート
uchar  *poll_addr;	//

//	コマンドの返信が必要なら1をセットする.
uchar  ToPcRdy;

uchar	puts_mode;

uchar 	puts_buf[64];
uchar 	puts_ptr;

uchar	poll_wptr;
uchar	poll_rptr;
uchar	poll_buf[256];

int		firm_top=0x8000000;
int		firm_end=0x8002000;
int		total_errs=0;
static  int test_adr0=0;

static  volatile char s_linecoding = 0;		//	baudrateが変更された.


Packet PacketFromPC;			//入力パケット 64byte
Packet PacketToPC;				//出力パケット 64byte

#define	Cmd0	PacketFromPC.cmd

/********************************************************************
 *
 ********************************************************************
 */
#ifdef	SUBMDL_STM32F103
extern char g_pfnVectors;
extern char _etext;
extern char _sdata;
extern char _edata;
#else
char  g_pfnVectors;
char  _etext;
char  _sdata;
char  _edata;
#endif
/********************************************************************
 *
 ********************************************************************
 */
int	align_size(int adr,int align)
{
	return 	(adr + align - 1) & (-align) ;	// align byte境界に合わせる.
}

/********************************************************************
 *
 ********************************************************************
 */
int	USBputpacket(uchar *buf,int len)
{
	USBwrite(buf,len);
	return len;
}
/********************************************************************
 *
 ********************************************************************
 */
void Init_Monitor(void)
{
	firm_top = (int) &g_pfnVectors;
	firm_end = (int) &_etext + (&_edata - &_sdata);

	firm_end = align_size(firm_end,1024);
	test_adr0= 0;
}

/********************************************************************
 *
 ********************************************************************
 */
int check_flash_adrs(int adrs)
{
	if( (adrs >= firm_top)&&(adrs < firm_end) )	return 0;	// INVALID!
	return 1;	// VALID.
}
/********************************************************************
 *	ISPモジュール インクルード.
 ********************************************************************
 */
#if	0
#include "usi_pic18s.h"

static void isp_command(uchar *data)
{
	uchar i;
	for (i=0; i<4; i++) {
		PacketToPC.raw[i]=usi_trans(data[i]);
	}
}
#endif

#ifdef	LPC2388
#define	FLASH_END_ADR	0x7e000		// 512k-8k
#endif

#ifdef	LPC1343
#define	FLASH_END_ADR	0x00008000	// 32k
#endif

#ifdef	STM8S_D
#define	FLASH_END_ADR	0x08010000	// 64k
#endif

#ifdef	STBEE
#define	FLASH_END_ADR	0x08080000	// 512k
#endif

#ifdef	CQ_STARM
#define	FLASH_END_ADR	0x08020000	// 128k
#endif

#ifdef	STBEE_MINI
#define	FLASH_END_ADR	0x08020000	// 128k
#endif

#ifdef	MARY
#define	FLASH_END_ADR	0x00008000	// 32k
#endif

#ifdef	FM3
#define	FLASH_END_ADR	0x00100000	// 1MB
#endif

#ifdef	RX62N
#define	FLASH_END_ADR	0xFFFE0000	// 32k
#endif

#ifdef	SH2A
#define	FLASH_END_ADR	0xFFFF0000	// 32k
#endif

/********************************************************************
 *
 ********************************************************************
 */
int	search_flash_end(int ea)
{
	int *p = (int *)ea;

	while(1) {
		p--;
		if(*p != (-1)) break;
	}
	return (int)p;
}

/********************************************************************
 *	接続テストの返答
 ********************************************************************
 */
void cmd_echo(void)
{
	int *fl_stat = (int*) &PacketToPC.rawint[2];
	PacketToPC.raw[1]=DEVID;				// DeviceID
	PacketToPC.raw[2]=VER_L;				// version.L
	PacketToPC.raw[3]=VER_H;				// version.H
	PacketToPC.raw[4]=DEV_LOADER;			// bootloader
	PacketToPC.raw[5]=PacketFromPC.raw[1];	// ECHOBACK TEST

	fl_stat[0] = BSWAP32(FLASH_END_ADR);
	fl_stat[1] = BSWAP32(search_flash_end(FLASH_END_ADR));
	fl_stat[2] = BSWAP32(total_errs);

	total_errs=0;
	ToPcRdy = 1;
}

/********************************************************************
 *	メモリー読み出し（32byte以上一括）
 ********************************************************************
 */
void cmd_peek_large_block(int adrs,int size)
{
	uchar *p = (uchar *)adrs;
	USBputpacket(p,size);

}

/********************************************************************
 *	メモリー読み出し
 ********************************************************************
 */
void cmd_peek(void)
{
	Packet *t = &PacketFromPC;
	int  size = BSWAP16(t->size);
	int  adrs = BSWAP32(t->adrs);
	int    i,subcmd;
	uchar  *pb;
	ushort *ph;
	uint   *pw;

	uchar  *tb;
	ushort *th;
	uint   *tw;

	subcmd = t->subcmd;

	if(size > PACKET_SIZE) {// 60byte以上の連続読み出しは別処理.
		cmd_peek_large_block(adrs,size);
		return;
	}

	switch(subcmd) {         // 読み出し幅指定に応じた処理に
	default:
	case MEM_BYTE:
		pb = (uchar *)adrs;
		tb = (uchar *)PacketToPC.raw;
		for(i=0; i<size; i++) {
			*tb++ = *pb++;
		}
		break;
	case MEM_HALF:
		ph = (ushort *)adrs;
		th = (ushort *)PacketToPC.rawint;
		for(i=0; i<size; i+=2) {
			*th++ = *ph++;
		}
		break;
	case MEM_WORD:
		pw = (uint *)adrs;
		tw = (uint *)PacketToPC.rawint;
		for(i=0; i<size; i+=4) {
			*tw++ = *pw++;
		}
		break;
	}

	USBputpacket(PacketToPC.raw,PACKET_SIZE);

//	ToPcRdy = 1;
}
/********************************************************************
 *	メモリー書き込み
 ********************************************************************
 */
void cmd_poke(void)
{
	Packet *t = &PacketFromPC;
	int  size = BSWAP16(t->size);
	int  adrs = BSWAP32(t->adrs);
	int  i,subcmd;

	uchar  *pb;
	ushort *ph;
	uint   *pw;

	uchar  *sb;
	ushort *sh;
	uint   *sw;

	subcmd = t->subcmd;

	switch(subcmd) {	// 書き込み幅指定に応じた処理に
	default:
	case MEM_BYTE:
		pb = (uchar *)adrs;
		sb = (uchar *)t->data;
		for(i=0; i<size; i++) {
			*pb++ = *sb++;
		}
		break;
	case MEM_HALF:
		ph = (ushort *)adrs;
		sh = (ushort *)t->data;
		for(i=0; i<size; i+=2) {
			*ph++ = *sh++;
		}
		break;
	case MEM_WORD:
		pw = (uint *)adrs;
		sw = (uint *)t->data;
		for(i=0; i<size; i+=4) {
			*pw++ = *sw++;
		}
		break;
	}
}

/********************************************************************
 *	FLASHのページ単位消去.	(引数はPageの先頭adrs) 消去サイズは暗黙.
 ********************************************************************
 */
void cmd_page_erase()
{
	int adrs=BSWAP32(PacketFromPC.adrs);
	if( check_flash_adrs(adrs)) {
		FLASH_ErasePage(adrs);
	}
}
/********************************************************************
 *	FLASHの書き込みをロック/アンロックする.(引数sizeがゼロならアンロック)
 ********************************************************************
 */
void cmd_flash_lock()
{
	if(BSWAP16(PacketFromPC.size)) {
		FLASH_Lock();
	} else {
		/* Unlock the internal flash */
		FLASH_Unlock();
	}
}
/********************************************************************
 *	FLASHのページ単位書き込み.	(引数は書き込みadrsとsize)
 ********************************************************************
  STM32では、word単位で書き込みが可能. (flush動作不要)

  他のマイコン(LPCなど)では、
  	実際にはRAM上のページバッファにword単位でためておいて、
  	最後にsize=0を指定してページバッファを一発書き込みする実装になることもある.
 

 */
void cmd_page_write()
{
	int  *p = PacketFromPC.data;
	int   i;
	int   size = BSWAP16(PacketFromPC.size);
	int	   adr = BSWAP32(PacketFromPC.adrs);

	// ここでbyte数をword数に変換している.
	size = (size+3) >> 2;		// size は4byte単位に限る.

//#if	defined(LPC1343)||defined(LPC2388)
	if(size==0) {				// size が 0 のときは書き込みflush動作をする.
		FLASH_ProgramPage(adr);	// これはSTM32に限り不要(ダミー関数)
		return ;
	}
//#endif

	for(i=0; i<size; i++) {
		if( check_flash_adrs(adr) ) {
			FLASH_ProgramWord(adr, *p);	// STM32では直接FLUSHに書く.
		}								// 他のマイコンでは、ページバッファに記録.
		p++;
		adr+=4;
	}
}

/********************************************************************
 *	番地指定の実行
 ********************************************************************
 */
void call_func(int adrs)
{
	void (*func)(void);
	func = (void (*)()) adrs;

	func();
}

#if	0
void usbModuleDisable()		// USBを一旦切り離す.
{
	// 未実装.
}
#endif
/********************************************************************
 *	番地指定の実行
 ********************************************************************
 */
void cmd_exec(int adrs,int bootflag)
{
	led_off();
	if(	bootflag ) {
		usbModuleDisable();		// USBを一旦切り離す.
#ifdef	FM3
		setupVectorTable(PacketFromPC.data[0]);	//FM3のみ.
#else
		wait_ms(1200);			// 1.2秒待つ.
#endif
	}
	call_func(adrs);
}



#if	APPLICATION_MODE
/********************************************************************
 *	puts 文字列をホストに返送する.
 ********************************************************************
 */
void cmd_get_string(void)
{
	PacketToPC.raw[0] =  puts_mode;	//'p';		/* コマンド実行完了をHOSTに知らせる. */
	PacketToPC.raw[1] =  puts_ptr;	// 文字数.
	memcpy( (void*)&PacketToPC.raw[2] , (void*)puts_buf , puts_ptr & 0x3f);	//文字列.
	puts_ptr = 0;
	ToPcRdy = 1;
}
/********************************************************************
 *	ユーザー定義関数の実行.
 ********************************************************************
 */
void func1(int arg)
{
	puts_ptr = 0;
	puts_mode = 'p';
	user_cmd(arg);
	puts_mode = 'q';
}
/********************************************************************
 *	ユーザー定義関数の実行.
 ********************************************************************
 */
void cmd_user_cmd(void)
{
	user_cmd(BSWAP32(PacketFromPC.adrs));
	USBputch(0);
}
#endif
/********************************************************************
 *	1mS単位の遅延を行う.
 ********************************************************************
 */
void cmd_wait_msec(void)
{
//	ushort ms = PacketFromPC.size;
	uchar ms = PacketFromPC.raw[1];	// '-dN'
	if(ms) {
		wait_ms(ms);
	}
}

/********************************************************************
 *	データの連続送信実行.
 ********************************************************************
 */
void make_report(void)
{
#if	TIMER2_INT_SAMPLE			// タイマー２割り込みでPORTサンプル.
	uchar i;
	uchar cnt=0;
	int c;
	//サンプル値を最大60個まで返す実装.
	PacketToPC.raw[0] =  REPLY_REPORT;		/* コマンド実行完了をHOSTに知らせる. */
	for(i=0; i<60; i++) {
		c = mon_read_sample();
		if(c<0) break;
		PacketToPC.raw[2+i]=c;
		cnt++;
	}
	PacketToPC.raw[1] =  cnt;
#else
	//サンプル値を１個だけ返す実装.
	PacketToPC.raw[0] =  REPLY_REPORT;		/* コマンド実行完了をHOSTに知らせる. */

	if(	poll_mode == POLL_ANALOG) {
		PacketToPC.raw[1] = 2;
//        while(ADCON0bits.NOT_DONE);     // Wait for conversion
//		PacketToPC.raw[2] = ADRESL;
//		PacketToPC.raw[3] = ADRESH;

	} else {
		PacketToPC.raw[1] = 1;
		PacketToPC.raw[2] = *poll_addr;
	}
#endif
	ToPcRdy = 1;
}
/********************************************************************
 *	データ引取りモードの設定
 ********************************************************************
 */
void cmd_set_mode(void)
{
	poll_mode = BSWAP16( PacketFromPC.size );
	poll_addr = (uchar  *) BSWAP32( PacketFromPC.adrs );

	if(	poll_mode == POLL_ANALOG) {
//		mInitPOT();
//		ADCON0bits.GO = 1;              // Start AD conversion
	}

	make_report();
}

/********************************************************************
 *	サンプリング・コマンド
 ********************************************************************
   HIDASP_SAMPLING(param,count) ---> return  header[16] + packet[count];
-------------
	a/d	|
	a	| rate | count | ch(s) | trig-mode | trig-ch | port-sel | rep | 
	d	| rate | count | ch(s) | trig-mode | trig-ch | port-sel | rep |
 */
void cmd_sampling(void)
{
	int   size = BSWAP16(PacketFromPC.size);
	sample_init(&PacketFromPC);
	sample_read(sampling_buf,size);
	USBwrite(sampling_buf,size);
}

#if	PICAVR_WRITER
/********************************************************************
 *	AVRライターの設定
 ********************************************************************
 */
void cmd_set_status(void)
{
	/* ISP用のピンをHi-Z制御 */
	/* ISP移行の手順を、ファーム側で持つ */
	if(PacketFromPC.raw[2] & 0x10) {// RST解除の場合
		ispDisconnect();
	} else {
		if(PacketFromPC.raw[2] & 0x80) {// RST状態でSCK Hは SCKパルス要求
			ispSckPulse();
		} else {
			ispConnect();
		}
	}
	PacketToPC.raw[0] = REPLY_REPORT;	/* コマンド実行完了をHOSTに知らせる. */
	ToPcRdy = 1;
}
/********************************************************************
 *	PORTへの出力制御
 ********************************************************************
 */
void cmd_tx(void)
{
	isp_command( &PacketFromPC.raw[1]);
	ToPcRdy = Cmd0 & 1;	// LSBがOnなら返答が必要.
}
/********************************************************************
 *	ページアドレスの設定.
 ********************************************************************
 */
void cmd_set_page(void)
{
	page_mode = PacketFromPC.raw[1];
	page_addr = PacketFromPC.raw[2];
	page_addr_h = PacketFromPC.raw[3];
}
/********************************************************************
 *	ISP書き込みクロックの設定.
 ********************************************************************
 */
void cmd_set_delay(void)
{
	usi_set_delay(PacketFromPC.raw[1]);	// '-dN'
}

/********************************************************************
 *	AVR書き込み(Fusion)コマンドの実行
 ********************************************************************
 *	Cmd0 : 0x20～0x27
 */
void cmd_page_tx(void)
{
	uchar i;
	uchar size=PacketFromPC.raw[1];
	//
	//	page_write開始時にpage_addrをdata[1]で初期化.
	//
	if( Cmd0 & (HIDASP_PAGE_TX_START & MODE_MASK)) {
		page_mode = 0x40;
		page_addr = 0;
		page_addr_h = 0;
	}
	//
	//	page_write (またはpage_read) の実行.
	//
	for(i=0; i<size; i++) {
		usi_trans(page_mode);
		usi_trans(page_addr_h);
		usi_trans(page_addr);
		PacketToPC.raw[i]=usi_trans(PacketFromPC.raw[i+2]);

		if (page_mode & 0x88) { // EEPROM or FlashH
			page_addr++;
			if(page_addr==0) {
				page_addr_h++;
			}
			page_mode&=~0x08;
		} else {
			page_mode|=0x08;
		}
	}
	//
	//	isp_command(Flash書き込み)の実行.
	//
	if( Cmd0 & (HIDASP_PAGE_TX_FLUSH & MODE_MASK)) {
		isp_command( &PacketFromPC.raw[size+2]);
	}
	ToPcRdy = Cmd0 & 1;	// LSBがOnなら返答が必要.
}

/********************************************************************
 *	AVRライター系コマンド受信と実行.
 ********************************************************************
 *	Cmd0 : 0x20～0x2F
 */
void cmd_avrspx(void)
{
	if(Cmd0 < (HIDASP_CMD_TX) ) 	{cmd_page_tx();}	// 0x20～0x27

	// 0x28～0x2F
	else if(Cmd0==HIDASP_SET_STATUS){cmd_set_status();}
	else if(Cmd0==HIDASP_SET_PAGE) 	{cmd_set_page();}
	else if(Cmd0==HIDASP_CMD_TX) 	{cmd_tx();}
	else if(Cmd0==HIDASP_CMD_TX_1) 	{cmd_tx();}
	else if(Cmd0==HIDASP_SET_DELAY) {cmd_set_delay();}
	else if(Cmd0==HIDASP_WAIT_MSEC) {cmd_wait_msec();}
}

#endif



#define	mHIDRxIsBusy()	(1)


/********************************************************************
 *	モニタコマンド受信と実行.
 ********************************************************************
 */
void ProcessIO(void)
{
	// 返答パケットが空であること、かつ、
	// 処理対象の受信データがある.
	{	//if((ToPcRdy == 0)) {
		//受信データがあれば、受信データを受け取る.
		PacketToPC.raw[0]=Cmd0;		// CMD ECHOBACK

#if	APPLICATION_MODE
#if	PICAVR_WRITER
		//コマンドに対応する処理を呼び出す.
			 if(Cmd0 >= HIDASP_PAGE_TX)  {cmd_avrspx();}	// AVRライターコマンド.
		else 
#endif
			 if(Cmd0 >= PICSPX_SETADRS24){cmd_picspx();}	// PICライターコマンド.
		else 
#endif

		switch(Cmd0) {
		 case HIDASP_PEEK: 		{cmd_peek();break;}	// メモリー読み出し.
		 case HIDASP_POKE: 		{cmd_poke();break;}	// メモリー書き込み.
		 case HIDASP_JMP: 		{cmd_exec(
		 						BSWAP32(PacketFromPC.adrs),BSWAP16(PacketFromPC.size)
		 			);
		 			break;}	// 実行.
		 case HIDASP_PAGE_ERASE:{cmd_page_erase();break;}	//Flash消去.
		 case HIDASP_PAGE_WRITE:{cmd_page_write();break;}	//Flash書込.
		 case HIDASP_FLASH_LOCK:{cmd_flash_lock();break;}	//FlashLock.
		 case HIDASP_SET_MODE:  {cmd_set_mode();break;}
		 case HIDASP_SAMPLING: 	{cmd_sampling();break;}
		 case HIDASP_TEST: 		{cmd_echo();break;}			// 接続テスト.

#if	APPLICATION_MODE
		 case HIDASP_GET_STRING:{cmd_get_string();break;}
		 case HIDASP_USER_CMD:  {cmd_user_cmd();break;}
#endif
		 default: break;
		}
	}

	// 必要なら、返答パケットをインタラプト転送(EP1)でホストPCに返却する.
	if( ToPcRdy ) {
		USBputpacket(PacketToPC.raw,PACKET_SIZE);
		ToPcRdy = 0;

		if(poll_mode!=0) {
			if(mHIDRxIsBusy()) {	//コマンドが来ない限り送り続ける.
				make_report();
			}
		}
	}
}


void	BlinkTask();

/********************************************************************
 *
 ********************************************************************
 */
int	USBpeekpacket(uchar *buf,int len)
{
	int	size = USBavailable();
	if(	size > 0) {
		USBread(buf,len);
		return len;
	}
	return 0;
}

void USB_set_linecoding_callback( int bitrate )
{
	(void) bitrate;
	s_linecoding = 1;
}

int	USBgetpacket(uchar *buf,int len)
{
	int rc;
	while(1) {
//		led_blink(18);
		rc = USBpeekpacket(buf,len);
		if(rc!=0) return rc;
		
		// baudrateが変更されたら、エラーとする.
		if(s_linecoding) return -1;
	}
}
/********************************************************************
 *	メイン処理
 ********************************************************************
 */
void USBmonit(void)
{
	uchar buf[PACKET_SIZE];

	int rc=0;
	s_linecoding = 0;
	Init_Monitor();
	while(1) {
		rc = USBgetpacket(buf,PACKET_SIZE);
		if( (rc<0) || (buf[0] != MONIT_SOF) ) return;
		memcpy(PacketFromPC.raw,buf+1,PACKET_SIZE-1);
		ProcessIO();
	}
}

/********************************************************************
 *
 ********************************************************************
 */
