/********************************************************************
 *	簡易モニタ
 ********************************************************************
 */
#ifdef	LPC2388


//	NXP arm7tdmi-s
#include <string.h>
#include "hwlib/led.h"
#include "hwlib/LPC23xx.h"
#include "utype.h"
#include "stm32dmy.h"


#else

//	STM32 Cortex-M3
#include <string.h>
#include "libmaple.h"
#include "monit.h"

#endif

#include "hidcmd.h"
#include "monit.h"
#include "picwrt.h"
#include "../libmaple/gpio.h"
#include "hard.h"
#include "util.h"

#include "boards/boards.h"

/********************************************************************
 *	スイッチ
 ********************************************************************
 */

//翻訳表:
//		STM32_library			  | wirish(C++) library
#define	FLASH_ErasePage(adrs)  		flashErasePage(adrs)
#define	FLASH_ProgramWord(adr,p)	flashWriteWord(adr,p)	
#define	FLASH_Lock()				flashLock()
#define	FLASH_Unlock()				flashUnlock()
//
#if   defined(BOARD_CQSTARM)
#	define	CQ_STARM
#elif defined(BOARD_DISCOVERY)
#	define	STM8S_D
#elif defined(BOARD_STBee)
#	define	STBEE
#elif defined(BOARD_STBeeMini)
#	define	STBEE_MINI
#endif



/********************************************************************
 *	定義
 ********************************************************************
 */
#if defined(__18F14K50)
#define	SOFTWARE_SPI_MODE	1
#define	DEVID	DEV_ID_PIC18F14K	//0x14
#else
#define	SOFTWARE_SPI_MODE	1		//18F4550だとSPI受信データが化ける.
#define	DEVID	DEV_ID_ARM_M3		//0xa3
#endif

#define	VER_H	1
#define	VER_L	1

#if	APPLICATION_MODE
#define	DEV_LOADER	0
#else
#define	DEV_LOADER	1
#endif

#define	REPLY_REPORT	0xaa
#define	MONIT_SOF		0x01
extern void user_cmd(int arg);

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
static int test_adr0=0;

Packet PacketFromPC;			//入力パケット 64byte
Packet PacketToPC;				//出力パケット 64byte

#define	Cmd0	PacketFromPC.cmd

/********************************************************************
 *	
 ********************************************************************
 */
extern char __cs3_stm32_vector_table;
extern char __cs3_region_start_ram;

#if	0
extern char g_pfnVectors;
extern char _sdata;
#endif

extern char _etext;
extern char _edata;

int	USBputpacket(uchar *buf,int len);
int	USBgetpacket(uchar *buf,int len);
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
void Init_Monitor(void)
{
#if	0
	firm_top = (int) &g_pfnVectors;
	firm_end = (int) &_etext + (&_edata - &_sdata);
#endif
	firm_top = (int) &__cs3_stm32_vector_table;
	firm_end = (int) &_etext + (&_edata - &__cs3_region_start_ram);

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
//#pragma code

#include "usi_pic18s.h"


static void isp_command(uchar *data) {
	uchar i;
	for (i=0;i<4;i++) {
		PacketToPC.raw[i]=usi_trans(data[i]);
	}
}



#if	0
/********************************************************************
 *	初期化.
 ********************************************************************
 */
void UserInit(void)
{
    mInitAllLEDs();
	timer2_interval(5);	// '-d5'
	poll_mode = 0;
	poll_addr = 0;
	puts_ptr = 0;
	ToPcRdy = 0;

#if	TIMER2_INT_SAMPLE			// タイマー２割り込みでPORTサンプル.
	poll_wptr=0;
	poll_rptr=0;
#endif
}
#endif


/********************************************************************
 *	ポート・サンプラ
 ********************************************************************
 */
#if	TIMER2_INT_SAMPLE			// タイマー２割り込みでPORTサンプル.
void mon_int_handler(void)
{
	uchar c;

	if(	poll_mode == 0) return;

	if(	poll_mode == POLL_ANALOG) {
		// ANALOG
        while(ADCON0bits.NOT_DONE);     // Wait for conversion

		c = ADRESL;			//ADC ポート読み取り,
		poll_buf[poll_wptr] = c;
		poll_wptr++;
		c = ADRESH;			//ADC ポート読み取り,
		poll_buf[poll_wptr] = c;
		poll_wptr++;

        ADCON0bits.GO = 1;              // Start AD conversion
		return;
	}else{
		// DIGITAL
		c = *poll_addr;			//ポート読み取り,
		poll_buf[poll_wptr] = c;
		poll_wptr++;
	}
}

int mon_read_sample(void)
{
	uchar c;
	if(	poll_rptr != poll_wptr) {
		c = poll_buf[poll_rptr];
		poll_rptr++;
		return c;
	}else{
		return -1;
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
	PacketToPC.raw[1]=DEVID;				// PIC25/14K
	PacketToPC.raw[2]=VER_L;				// version.L
	PacketToPC.raw[3]=VER_H;				// version.H
	PacketToPC.raw[4]=DEV_LOADER;			// bootloader
	PacketToPC.raw[5]=PacketFromPC.raw[1];	// ECHOBACK

	fl_stat[0]=FLASH_END_ADR;
	fl_stat[1]=search_flash_end(FLASH_END_ADR);
	fl_stat[2]=total_errs;

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
	int    i,size,subcmd;

	uchar  *pb;
	ushort *ph;
	uint   *pw;

	uchar  *tb;
	ushort *th;
	uint   *tw;

 	size   = t->size;
	subcmd = t->subcmd;

	if(size > PACKET_SIZE) {
		cmd_peek_large_block(t->adrs,size);
		return;
	}

	switch(subcmd) {
	 default:
	 case MEM_BYTE:
	 	pb = (uchar *)t->adrs;
		tb = (uchar *)PacketToPC.raw;
		for(i=0;i<size;i++) {
			*tb++ = *pb++;
		}
		break;
	 case MEM_HALF:
	 	ph = (ushort *)t->adrs;
		th = (ushort *)PacketToPC.rawint;
		for(i=0;i<size;i+=2) {
			*th++ = *ph++;
		}
		break;
	 case MEM_WORD:
	 	pw = (uint *)t->adrs;
		tw = (uint *)PacketToPC.rawint;
		for(i=0;i<size;i+=4) {
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
	uchar i,size,subcmd;

	uchar  *pb;
	ushort *ph;
	uint   *pw;

	uchar  *sb;
	ushort *sh;
	uint   *sw;

 	size   = t->size;
	subcmd = t->subcmd;

	switch(subcmd) {
	 default:
	 case MEM_BYTE:
	 	pb = (uchar *)t->adrs;
		sb = (uchar *)t->data;
		for(i=0;i<size;i++) {
			*pb++ = *sb++;
		}
		break;
	 case MEM_HALF:
	 	ph = (ushort *)t->adrs;
		sh = (ushort *)t->data;
		for(i=0;i<size;i+=2) {
			*ph++ = *sh++;
		}
		break;
	 case MEM_WORD:
	 	pw = (uint *)t->adrs;
		sw = (uint *)t->data;
		for(i=0;i<size;i+=4) {
			*pw++ = *sw++;
		}
		break;
	}
}

/********************************************************************
 *	
 ********************************************************************
 */
void cmd_page_erase()
{
	if( check_flash_adrs(PacketFromPC.adrs)) {
		FLASH_ErasePage(PacketFromPC.adrs);
	}
}
/********************************************************************
 *	
 ********************************************************************
 */
void cmd_flash_lock()
{
 	if(PacketFromPC.size) {
		FLASH_Lock();
	}else{
		/* Unlock the internal flash */
		FLASH_Unlock();
	}
}
/********************************************************************
 *	
 ********************************************************************
 */
int	flash_write(int adr,int size,int *p)
{
	int errs=0;
	int rc,i;
	int	wsize = (size+3) >> 2;	// wsizeは整数のワード数に変換されるので注意.

	
	for(i=0;i<wsize;i++) {
		if( check_flash_adrs(adr) ) {
			rc = FLASH_ProgramWord(adr, *p);
			if(rc) errs++;
		}
		p++;
		adr+=4;
	}
	return errs;
}

#define	BLK_SIZE	60

/********************************************************************
 *	
 ********************************************************************
 */
void cmd_page_write()
{
	int size = PacketFromPC.size;
 	int  adr = PacketFromPC.adrs;
	int   *p = PacketFromPC.data;

#if	defined(LPC1343)||defined(LPC2388)
	if(size==0) {
		FLASH_ProgramPage(adr);
		return ;
	}
#endif
	if(	size <= (48) ) {
		total_errs += flash_write(adr,size,p);
		return;
	}

	while(size>0) {
		int buf[64/4];

		int len = size;
		if( len >= BLK_SIZE ) {len = BLK_SIZE;}

		USBgetpacket((uchar*)buf,len);

		total_errs += flash_write(adr, len ,buf);
		size -= len;
		adr  += len;
	}
}

/********************************************************************
 *	
 ********************************************************************
 */
void cmd_test()
{
	int  size = PacketFromPC.size;
 	int   adr = PacketFromPC.adrs;

	if(adr != test_adr0) {
		__error();
	}
	test_adr0++;

	if(size<60) return;

	while(size>0) {
		int   buf[64/4];

		int len = size;
		if( len >= BLK_SIZE ) {len = BLK_SIZE;}
		USBgetpacket((uchar*)buf,len);
		size -= len;
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
/********************************************************************
 *	番地指定の実行
 ********************************************************************
 */
void cmd_exec(int adrs,int bootflag)
{
	if(	bootflag ) {
		usbModuleDisable();		// USBを一旦切り離す.
		wait_ms(1200);			// 1.2秒待つ.
	}
	call_func(adrs);
}



#if	0
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
#endif
#if	APPLICATION_MODE
/********************************************************************
 *	ユーザー定義関数の実行.
 ********************************************************************
 */
void cmd_user_cmd(void)
{
	user_cmd(PacketFromPC.adrs);
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
	for(i=0;i<60;i++) {
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

	}else{
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
	poll_mode = PacketFromPC.size;
	poll_addr = (uchar  *) PacketFromPC.adrs;

	if(	poll_mode == POLL_ANALOG) {
//		mInitPOT();
//		ADCON0bits.GO = 1;              // Start AD conversion
	}

	make_report();
}

#if	1
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
	}else{
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
	for(i=0;i<size;i++) {
		usi_trans(page_mode);
		usi_trans(page_addr_h);
		usi_trans(page_addr);
		PacketToPC.raw[i]=usi_trans(PacketFromPC.raw[i+2]);

		if (page_mode & 0x88) { // EEPROM or FlashH
			page_addr++;
			if(page_addr==0) {page_addr_h++;}
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


#define	mHIDTxIsBusy()	(0)
#define	mHIDRxIsBusy()	(1)


/********************************************************************
 *	モニタコマンド受信と実行.
 ********************************************************************
 */
void ProcessIO(void)
{
	// 返答パケットが空であること、かつ、
	// 処理対象の受信データがある.
	{
		//受信データがあれば、受信データを受け取る.
		PacketToPC.raw[0]=Cmd0;		// CMD ECHOBACK
#if	0
		//コマンドに対応する処理を呼び出す.
			 if(Cmd0 >= HIDASP_PAGE_TX)  {cmd_avrspx();}	// AVRライターコマンド.
		else if(Cmd0 >= PICSPX_SETADRS24){cmd_picspx();}	// PICライターコマンド.
		else 
#endif
		switch(Cmd0) {
		 case HIDASP_PEEK: 		{cmd_peek();break;}	// メモリー読み出し.
		 case HIDASP_POKE: 		{cmd_poke();break;}	// メモリー書き込み.
		 case HIDASP_JMP: 		{cmd_exec(PacketFromPC.adrs,PacketFromPC.size);break;}	// 実行.
		 case HIDASP_PAGE_ERASE:{cmd_page_erase();break;}	//Flash消去.
		 case HIDASP_PAGE_WRITE:{cmd_page_write();break;}	//Flash書込.
		 case HIDASP_FLASH_LOCK:{cmd_flash_lock();break;}	//FlashLock.
		 case HIDASP_SET_MODE:  {cmd_set_mode();break;}
		 case HIDASP_BOOT_RWW: 	{cmd_test();break;}			// 接続テスト.
		 case HIDASP_TEST: 		{cmd_echo();break;}			// 接続テスト.

#if	APPLICATION_MODE
//		 case HIDASP_GET_STRING:{cmd_get_string();break;}
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
/********************************************************************
 *
 ********************************************************************
 */
int	USBputpacket(uchar *buf,int len)
{
	USBwrite(buf,len);
	return len;
}
#if	0
/********************************************************************
 *
 ********************************************************************
 */
int	getBootSW(void);

void blink_test()
{
	int sw=getBootSW();
	int ledon=0;
	if(sw) {
		ledon=1;
	}
	DigitalWrite(BOARD_LED_PIN,ledon);
}
#endif
/********************************************************************
 *
 ********************************************************************
 */
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

int	USBgetpacket(uchar *buf,int len)
{
	int rc;
	while(1) {
		BlinkTask();
		rc = USBpeekpacket(buf,len);
		if(rc!=0) return rc;
	}
}
void *get_signature();
/********************************************************************
 *	メイン処理
 ********************************************************************
 */
void USBmonit(void)
{
	uchar buf[PACKET_SIZE];

	int rc=0;
	get_signature();
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
