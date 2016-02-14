/********************************************************************
 *	簡易モニタ
 ********************************************************************
 */

#include "hidcmd.h"
#include "monit.h"

#include "gpiodef.h"
#include "pindef.h"

#include "picwrt.h"

#if	APPLICATION_MODE


//	ポート読み込み.
#define	inPGC	 	PGC
#define	inPGM	 	PGM
#define	inPGD	 	PGD
#define	inMCLR 		MCLR

//	方向レジスタ (0=出力)
#define	dirPGC	 	PGC
#define	dirPGM	 	PGM
#define	dirPGD	 	PGD
#define	dirMCLR 	MCLR

/********************************************************************
 *
 ********************************************************************
	// PICwriter専用.
	struct{
		uchar  piccmd;
		uchar  picsize;
		uchar  picadrl;
		uchar  picadrh;
		uchar  picadru;
		uchar  piccmd4;
		uchar  picms;
		uchar  picdata[32];
	};
 */
enum CMD_4 {
	b_0000 = 0x00,
	b_0001 = 0x01,
	b_1000 = 0x08,
	b_1001 = 0x09,
	b_1100 = 0x0c,	//
	b_1101 = 0x0d,	// TBLPTR+=2
	b_1111 = 0x0f,	// Write!
};


// JTAG =================================

#define	JCMD_BITCOUNT_MASK	0x3f
#define	JCMD_LAST_TMS		0x40
#define	JCMD_INIT_TMS		0x80

#define	JCMD_ACCL_SET		0x40
#define	JCMD_INIT_DIR		0x80

//#define	TCK_WAIT()		wait_125ns(10)
//#define	TCK_WAIT()		wait_125ns(1)
//#define	TCK_WAIT()		wait_0us()

#define	TCK_WAIT()		wait_125ns(wait_cnt)
static	unsigned short jtag_wait_cnt=1;	// 16bit 待ち時間カウンタ. 単位は125nS

/********************************************************************
 *	データ
 ********************************************************************
 */

//	コマンドの返信が必要なら1をセットする.
extern	uchar ToPcRdy;

//#pragma udata SomeSectionName1
extern	Packet PacketFromPC;			//入力パケット 64byte

#if defined(__18F14K50)
extern	near Packet PacketToPC;				//出力パケット 64byte
#else
//#pragma udata SomeSectionName2
extern	Packet PacketToPC;				//出力パケット 64byte
#endif

#define	Cmd0	PacketFromPC.cmd

#define	get_bit(Port_) 			digitalRead(Port_)

#ifndef	FM3
#define	set_bit(Port_,val_) 	digitalWrite(Port_,val_)
#else
//	FM3のみ.
void 	set_bit(int port,int value)
{
	if(value != 0) value = 1;		// 0=Lo , 1=High
	digitalWrite(port,value);
}
#endif

//	Port方向設定: PIC18F互換 val=0なら出力.
void set_dir(int Port,int val)
{
	if(val) {
		pinMode(Port,INPUT);
	} else {
		pinMode(Port,OUTPUT);
	}
}

//	Port方向設定: Arduino互換 set_dirと論理が逆.
void pin_mode(int Port,int val)
{
	if(val!=0) {val=OUTPUT;}	// 0=INPUT.

	pinMode(Port,val);
}
/********************************************************************
 *	24bitアドレスを8bit ３個に分解するunion.
 ********************************************************************
 */
typedef	union {
	dword adr24;
	uchar adr8[4];
} ADR24;

typedef	union {
	ushort adr16;
	uchar  adr8[2];
} ADR16;


//
//	0  ～200   : 0～200マイクロ秒待つ.
//	201～250   : 1～ 50ミリ秒待つ.
//
//
static	void wait_ums(uchar us)
{
	if(us) {
		if(us>200) 	wait_ms(us-200);
		else 		wait_us(us);
	} else {
		wait_125ns(1);	// 1/8 uS 待つ.
	}
}

static	int  getword(uchar *t)
{
	return t[0] | (t[1]<<8);
}

static	void setword(uchar *t,int d)
{
	t[0] = d;
	t[1] = d>>8;
}
#if	0
static	long getlong(uchar *t)
{
	return 	t[0] | (t[1]<<8) | (t[2]<<16) | (t[3]<<24);
}
#endif
/*********************************************************************
 *	1bit受信.
 *********************************************************************
        +---+
  PGC --+   +---

           ↑ ここで読む.

  PGD [ 入力    ]
 */
#if	0
inline uchar GetCmdb1(void)
{
	uchar b=0;
	set_bit(PGC,1);
	wait_us(1);
//	b = get_port(Pinb) & (1<<PGD);
	if(inPGD) b=1;
	set_bit(PGC,0);
	wait_us(1);
	return b;
}
#endif
/*********************************************************************
 *	n bit 送信.
 *********************************************************************
 */
void SetCmdN(ushort cmdn,uchar len)
{
	uchar i;
	for(i=0; i<len; i++) {
		if(cmdn&1)	set_bit(PGD,1);
		else		set_bit(PGD,0);
		set_bit(PGC,1);
		wait_us(1);
		cmdn >>=1;
		set_bit(PGC,0);
	}
}

/*********************************************************************
 *	4 bit 送信 , 最後の PGC_H時間を指定.
 *********************************************************************
 */
void SetCmdNLong(uchar cmdn,uchar len,uchar ms)
{
	uchar i;	//,b;
	for(i=0; i<(len-1); i++) {
//		b = cmdn & 1;
//		SetCmdb1(b);
//		cmdn >>=1;
		{
			if(cmdn&1)	set_bit(PGD,1);
			else		set_bit(PGD,0);
			set_bit(PGC,1);
			wait_us(1);
			cmdn >>=1;
			set_bit(PGC,0);
		}
	}
	{
//		SetCmdb1Long(cmdn & 1,ms);
		{
			if(cmdn&1)	set_bit(PGD,1);
			else		set_bit(PGD,0);
			set_bit(PGC,1);
			if(ms) wait_ms(ms);
			wait_us(1);
			cmdn >>=1;
			set_bit(PGC,0);
		}
	}
}

/*********************************************************************
 *	PGDビットの入出力方向を切り替える.(0=入力)
 *********************************************************************
 *	f=0 ... PGD=in
 *	f=1 ... PGD=out
 */

void SetPGDDir(uchar f)
{
	if(f) {
//		dirPGD=0;	// out
		pinMode(dirPGD,OUTPUT);
	} else {
//		dirPGD=1;	// in
		pinMode(dirPGD,INPUT);
	}
}


/*********************************************************************
 *	4bitのコマンド と 16bitのデータを送信.
 *********************************************************************
 */
void SetCmd16(uchar cmd4,ushort data16,uchar ms)
{
	SetCmdNLong(cmd4,4,ms);
	SetCmdN(data16,16);
}

/*********************************************************************
 *	TBLPTR[U,H,L] のどれかに8bit値をセットする.
 *********************************************************************
 */
static	void SetAddress8x(uchar adr,uchar inst)
{
	SetCmd16(b_0000,0x0e00 |  adr,0);		// 0E xx  : MOVLW xx
	SetCmd16(b_0000,0x6e00 | inst,0);		// 6E Fx  : MOVWF TBLPTR[U,H,L]
}
/*********************************************************************
 *	TBLPTR に24bitアドレスをセットする.
 *********************************************************************
 */
void setaddress24(void)
{
	SetAddress8x(PacketFromPC.picadru,0xf8);	// TBLPTRU
	SetAddress8x(PacketFromPC.picadrh,0xf7);	// TBLPTRH
	SetAddress8x(PacketFromPC.picadrl,0xf6);	// TBLPTRL
}

/*********************************************************************
 *	TBLPTR から１バイト読み出し. TBLPTRはポスト・インクリメントされる.
 *********************************************************************
 */
static	uchar GetData8(void)
{
	uchar i,data8=0;

	SetCmdN(b_1001,4);
	SetCmdN(0 , 8);

//	return pic18_GetData8b();
	/*********************************************************************
	 *	8 bit 受信.	 LSBファースト.
	 *********************************************************************
	static	uchar pic18_GetData8b(void)
	{
	 */

	SetPGDDir(0);		// PGD=in
//	for(i=0;i<8;i++,mask<<=1) {
//		if( GetCmdb1() ) {
//			data8 |= mask;
//		}
	i=8;
	do {
		set_bit(PGC,1);
		data8>>=1;
		wait_us(1);
//		if(inPGD) {
		if(digitalRead(inPGD)) {
			data8 |= 0x80;
		}
		set_bit(PGC,0);
		wait_us(1);
	} while(--i);
	SetPGDDir(1);		// PGD=out
	return data8;
}


#if	0
void GetData8L(int adr,uchar *buf,int len)
{
	int i,c;
	SetAddress24(adr);
	for(i=0; i<len; i++) {
		c = GetData8();
		*buf++ = c;
//		if(dumpf)printf(" %02x",c);
	}
}
#endif


#if	0
/*********************************************************************
 *	4bitのコマンド と 16bitのデータを送信.
 *********************************************************************
 */
void SetCmd16L(uchar cmd4,ushort *buf,uchar len,uchar ms)
{
	uchar i;
	ushort data16;
	for(i=0; i<len; i++) {
		data16 = *buf++;
		SetCmdNLong(cmd4,4,ms);
		SetCmdN(data16,16);
	}
}
#endif
/********************************************************************
 *	16bitデータ列を(size)ワード分書き込む.
 ********************************************************************
 *	cmd4 は、各16bitデータを書き込む前に与える4bitコマンド.
 *	ms   は、4bitコマンドの最終bit送出後に何ミリ秒待つべきかの値.
 *	ms=0 のときは 1uS 待つ.
 */
void pic_setcmd16L(void)
{
	uchar len = PacketFromPC.picsize;
	uchar cmd4= PacketFromPC.piccmd4;
	uchar ms  = PacketFromPC.picms;
	uchar i;
//	ushort *s = (ushort *) PacketFromPC.picdata;
	uchar *s = PacketFromPC.picdata;
	ushort data16;
	for(i=0; i<len; i++) {
//		data16 = *s++;
		data16 = getword(s);
		s+=2;
		SetCmdNLong(cmd4,4,ms);
		SetCmdN(data16,16);
	}
}
/********************************************************************
 *	24bitアドレスをセットして、 8bitデータ列を(size)バイト分取得する.
 ********************************************************************
 */
void pic_getdata8L(void)
{
	uchar i,len;

	setaddress24();

	len =PacketFromPC.picsize;
	for(i=0; i<len; i++) {
		PacketToPC.raw[i] = GetData8();
	}
}
/********************************************************************
 *	PICプログラムモード
 ********************************************************************
 */
#if	0
void pic_setpgm(void)
{
	uchar f=PacketFromPC.picsize;

	if(f) {
		//全ての信号をLにする.
		set_bit(PGM,0);
		set_bit(PGD,0);
		set_bit(PGC,0);
		set_bit(MCLR,0);

		dirPGM=0;
		dirPGC=0;
		dirMCLR=0;

		SetPGDDir(1);		// PGD=out

		wait_ms(1);
		set_bit(PGM,1);	// PGM=H
		wait_ms(1);
		set_bit(MCLR,1);// MCLR=H
		wait_ms(1);
	} else {
		set_bit(PGD,0);// MCLR=L
		set_bit(PGC,0);// MCLR=L
		wait_ms(1);
		set_bit(MCLR,0);// MCLR=L
		wait_ms(1);
		set_bit(PGM,0);	// PGM=L
		wait_ms(1);
		//全ての信号をLにする.
		set_bit(PGM,0);
		set_bit(PGD,0);
		set_bit(PGC,0);
		set_bit(MCLR,0);

		dirPGM=1;
		dirPGC=1;
		dirMCLR=1;

		SetPGDDir(0);		// PGD=out
	}
}
#endif
/********************************************************************
 *	PIC24プログラムモード
 ********************************************************************
 */
void pic_bitbang(void)
{
	uchar cnt =PacketFromPC.raw[1];
	uchar wait=PacketFromPC.raw[2];
	uchar *p =&PacketFromPC.raw[3];
	uchar *t =&PacketToPC.raw[0];
	uchar c;
	while(cnt) {
		c = *p++;
//	ポート出力データ.
		set_bit(MCLR,c & 0x08);
		set_bit(PGM ,c & 0x04);
		set_bit(PGD ,c & 0x02);
		set_bit(PGC ,c & 0x01);
//	方向レジスタ (0=出力)
		set_dir(dirMCLR,c & 0x80);
		set_dir(dirPGM ,c & 0x40);
		set_dir(dirPGD ,c & 0x20);
		set_dir(dirPGC ,c & 0x10);
		if(Cmd0==PICSPX_BITREAD) {
			c = 0;
			if(digitalRead(inMCLR)) c|=0x08;
			if(digitalRead(inPGM))  c|=0x04;
			if(digitalRead(inPGD))  c|=0x02;
			if(digitalRead(inPGC))  c|=0x01;
			*t++ = c;
		}
		wait_ums(wait);
		wait_us(1);
		cnt--;
	}
}

#if	INCLUDE_JTAG_CMD

/********************************************************************
 *	JTAG:
 ********************************************************************
 */

//	LSB first.
void jtag_command(int read_flag)
{
	uchar *p = &PacketFromPC.raw[1];	// jtag ストリーム(for WRITE)
	uchar *q = &PacketToPC.raw[0];		// jtag ストリーム(for READ)
	int		wait_cnt = jtag_wait_cnt;
	uchar   cnt,c; 						// 処理するビット数.
	uchar 	bitcnt=0;					// cnt のmod 7 が入る.
	uchar 	tdi_byte=0;
	uchar 	tdo_byte=0;

	uchar 	jcmd;						// jtag command
	uchar 	maxcnt;
	uchar   mask_bit=1;					// 初期値=1 、毎回 左シフト、8bit処理したら 1に戻る.

	JTAG_PORT_ACCEL_ON;

	while(1) {
		jcmd =*p++;						// jtag command
		if(jcmd==0) return;
		maxcnt=jcmd & JCMD_BITCOUNT_MASK;
		if(jcmd & JCMD_INIT_TMS) {
			for(cnt=0; cnt<maxcnt; cnt++) {
				c = *p++;

				Set_bit(TMS ,c & (JCMD_bitTMS<<4));
				Set_bit(TDI ,c & (JCMD_bitTDI<<4));
				Set_bit(TCK ,c & (JCMD_bitTCK<<4));
				TCK_WAIT();

				Set_bit(TMS ,c & JCMD_bitTMS);
				Set_bit(TDI ,c & JCMD_bitTDI);
				Set_bit(TCK ,c & JCMD_bitTCK);
				TCK_WAIT();
			}
		} else {
			bitcnt=0;				// cnt のmod 7 が入る.
			tdi_byte=0;
			tdo_byte=0;
			mask_bit=1;				// 初期値=1 、毎回 左シフト、8bit処理したら 1に戻る.

			Set_bit(TMS ,0);

			for(cnt=0; cnt<maxcnt; cnt++) {

//	==== TCK = LOW ====
//		ISP_OUT &= ~(1 << TCK);		/* TCK low */
//		TCK=0;

				if( bitcnt ==0 ) {		// fetch data.
					tdi_byte = *p++;
					tdo_byte = 0;
					mask_bit  = 1;
				}

				if(cnt == (maxcnt-1)) {
					if(jcmd & JCMD_LAST_TMS) {
						Set_bit(TMS ,1);
					}
				}
				Set_bit(TDI ,tdi_byte & mask_bit);


				Set_bit(TCK ,0);	// このタイミングでTDOが更新される.

				TCK_WAIT();

				if(read_flag) {
					if(	Get_TDO() ) {
						tdo_byte |= mask_bit;
					}
				}
//	==== TCK = HIGH ====
//		ISP_OUT |=  (1 << TCK);		/* TCK High */
//		TCK=1;
				Set_bit(TCK ,1);	// このタイミングでTDIが読まれる.

				mask_bit <<=1;
				bitcnt++;
				if( bitcnt == 8 ) {			// store data.
					*q++ = tdo_byte;
					bitcnt=0;
				}

				TCK_WAIT();
			}
//			Set_bit(TCK ,0);
			//Set_bit(TMS ,0);
			*q = tdo_byte;
		}
	}
}


/********************************************************************
 *	JTAG: control
 ********************************************************************
 *	bit assign:
 	b7 Vcc2
 	b6 Vcc1
 	b5 nTRST
 	b4 nSRST
 	b3 TMS
 	b2 TDI
 	b1 TDO
 	b0 TCK
 *	set direction define
	0 input
	1 output
 */

void jtag_control(int read_flag)
{
	uchar *p = &PacketFromPC.raw[1];	// jtag ストリーム(for WRITE)
	uchar *q = &PacketToPC.raw[0];		// jtag ストリーム(for READ)
	int		wait_cnt = jtag_wait_cnt;

	uchar   cnt,c; 						// 処理するビット数.
	uchar 	tdo_byte=0;
	uchar 	jcmd;						// jtag command
	uchar 	maxcnt;

	JTAG_PORT_ACCEL_ON;

	while(1) {
		jcmd =*p++;						// jtag command
		if(jcmd==0) return;
		maxcnt=jcmd & JCMD_BITCOUNT_MASK;
		if(jcmd & JCMD_INIT_DIR) {
			if(maxcnt==0) {
				int lo=*p++;
				int hi=*p++;
				jtag_wait_cnt = lo | (hi<<8);	// 16bit 待ち時間カウンタ.
				wait_cnt = jtag_wait_cnt;
			}else{
				for(cnt=0; cnt<maxcnt; cnt++) {
					c = *p++;
					pin_mode(TRST ,c & JCMD_bitTRST);
					pin_mode(SRST ,c & JCMD_bitSRST);
					pin_mode(TMS  ,c & JCMD_bitTMS);
					pin_mode(TDI  ,c & JCMD_bitTDI);
					pin_mode(TCK  ,c & JCMD_bitTCK);
					pin_mode(TDO  ,c & JCMD_bitTDO);
				}
			}
		}else if(jcmd & JCMD_ACCL_SET) {
			// 高速版. TRST SRSTなし.
			for(cnt=0; cnt<maxcnt; cnt++) {
				c = *p++;
				Set_bit(TMS  ,c & JCMD_bitTMS);
				Set_bit(TDI  ,c & JCMD_bitTDI);
				Set_bit(TCK  ,c & JCMD_bitTCK);
				set_bit(TDO  ,c & JCMD_bitTDO);
				tdo_byte = 0;
				if(read_flag) {
					if(	get_bit(TMS ) ) {tdo_byte |= JCMD_bitTMS; }
					if(	get_bit(TDI ) ) {tdo_byte |= JCMD_bitTDI; }
					if(	get_bit(TCK ) ) {tdo_byte |= JCMD_bitTCK; }
					if(	get_bit(TDO ) ) {tdo_byte |= JCMD_bitTDO; }
					*q++ = tdo_byte;
				}
				TCK_WAIT();
			}
		} else {
			// 低速版. TRST SRSTあり.
			for(cnt=0; cnt<maxcnt; cnt++) {
				c = *p++;
				set_bit(TRST ,c & JCMD_bitTRST);
				set_bit(SRST ,c & JCMD_bitSRST);
				Set_bit(TMS  ,c & JCMD_bitTMS);
				Set_bit(TDI  ,c & JCMD_bitTDI);
				Set_bit(TCK  ,c & JCMD_bitTCK);
				set_bit(TDO  ,c & JCMD_bitTDO);
				tdo_byte = 0;
				if(read_flag) {
					if(	get_bit(Vcc2) ) {tdo_byte |= JCMD_bitVcc2; }
					if(	get_bit(Vcc1) ) {tdo_byte |= JCMD_bitVcc1; }
					if(	get_bit(TRST) ) {tdo_byte |= JCMD_bitTRST; }
					if(	get_bit(SRST) ) {tdo_byte |= JCMD_bitSRST; }
					if(	get_bit(TMS ) ) {tdo_byte |= JCMD_bitTMS; }
					if(	get_bit(TDI ) ) {tdo_byte |= JCMD_bitTDI; }
					if(	get_bit(TCK ) ) {tdo_byte |= JCMD_bitTCK; }
					if(	get_bit(TDO ) ) {tdo_byte |= JCMD_bitTDO; }
					*q++ = tdo_byte;
				}
				TCK_WAIT();
			}// for.
		}// if.
	}//while.
}

#endif


#if	SUPPORT_PIC24F

/*********************************************************************
 *	１クロック分の PGD 送信
 *********************************************************************

   PGD <データ1bit>

   PGC _____|~~~~~~
 */
/********************************************************************
 *	PIC24F:任意bit(最大8bit)のPGDデータを送る.
 ********************************************************************
 */
void add_data(uchar c,uchar bits)
{
	uchar b;
	for(b=0; b<bits; b++) {	// LSBから bits ビット送る.
		set_bit(PGC ,0);
//		PGC = 0;

		set_bit(PGD ,c & 1);
//		if( (c & 1)) PGD = 1;
//		if(!(c & 1)) PGD = 0;

		wait_us(1);
		c>>=1;
		set_bit(PGC ,1);
//		PGC = 1;
		wait_us(1);
	}
}
/********************************************************************
 *	PIC24F: 4bit分のPGDデータを送る.
 ********************************************************************
 */
void add_data4(uchar c)
{
	add_data(c,4);
}
/********************************************************************
 *	PIC24F: 8bit分のPGDデータを送る.
 ********************************************************************
 */
void add_data8(uchar c)
{
	add_data(c,8);
}
#if	0
/********************************************************************
 *	PIC24F: 3byte分のPGDデータを送る.
 ********************************************************************
 */
void add_data24(uchar u,uchar h,uchar l)
{
	add_data4(b_0000);	// send SIX command '0000'
	add_data8(l);
	add_data8(h);
	add_data8(u);
	set_bit(PGC ,0);
	//PGC = 0;
}
#endif
/********************************************************************
 *	PIC24F: 24bit分のPGDデータを送る.
 ********************************************************************
 */
void sendData24(dword c)
{
	ADR24 adr;
	adr.adr24 = c;
	add_data4(b_0000);	// send SIX command '0000'
	add_data8(adr.adr8[0]);	// l
	add_data8(adr.adr8[1]);	// h
	add_data8(adr.adr8[2]);	// u
	set_bit(PGC ,0);
//	PGC = 0;
}
void send_nop1()
{
	sendData24(0x000000);				// NOP
}
void send_nop2()
{
	send_nop1();
	send_nop1();
}
void send_goto200()
{
	sendData24(0x040200);				// GOTO 0x200
	send_nop1();
}

#define	W6	6
#define	W7	7

/********************************************************************
 *	PIC24F: レジスタペア (W0,reg) にホストPC指定のアドレス24bitを設定
 ********************************************************************
 *	ホストPC側で、あらかじめアドレスを 1/2にしておくこと(device adr化).
 */
void send_addr24(uchar reg)
{
	ADR24 adr;

	adr.adr8[0] = PacketFromPC.picadru;
	adr.adr8[1] = 0;
	adr.adr8[2] = 0;
	adr.adr8[3] = 0;

	sendData24(0x200000| ( adr.adr24 <<4) );	// MOV  #adr_h, W0
	sendData24(0x880190);				// MOV  W0, TBLPAG

	adr.adr8[0] = PacketFromPC.picadrl;
	adr.adr8[1] = PacketFromPC.picadrh;

	sendData24(0x200000|reg| ( adr.adr24 <<4) );	// MOV  #adr_l, W6
}

/*********************************************************************
 *	共通ルーチン・アドレスセットアップ
 *********************************************************************
 */
static	void setup_addr24()		//int adr)
{
	send_nop1();
	send_goto200();
	send_addr24(W6);
	sendData24(0x207847);				// MOV  #VLSI, W7
	send_nop1();
}
static	void setup_addr24write(void)	//int adr,int nvmcon)
{
	send_nop1();
	send_goto200();
	sendData24(0x24001A);				// MOV  #0x4001, W10
	sendData24(0x883B0A);				// MOV  W10, NVMCON
	send_addr24(W7);
}
static	void bset_nvmcon(void)
{
	send_nop2();
	sendData24(0xA8E761);					// BSET NVMCON
	send_nop2();
}
/*********************************************************************
 *	8bitデータの受信
 *********************************************************************

   PGC _____|~~~~~~

   PGD <データ1bit>
			 ^
			 |
		サンプリングpoint
 */
static	uchar pic24_getData8(void)
{
	uchar c=0;
	uchar b;

//	PGC = 0;
	set_bit(PGC ,0);
	for(b=0; b<8; b++) {	// LSB から 8 ビット受け取る.

		//PGC = 1;
		set_bit(PGC ,1);
		c>>=1;

		wait_us(1);
//		if(inPGD) {
		if(digitalRead(inPGD)) {
			c|=0x80;
		}
		//PGC = 0;
		set_bit(PGC ,0);
		wait_us(1);
	}
	return c;
}
/*********************************************************************
 *	16bitデータの受信
 *********************************************************************
 */
static	int	pic24_getData16(void)
{
	ADR16 adr;
	adr.adr8[0]=pic24_getData8();
	adr.adr8[1]=pic24_getData8();
	return adr.adr16;
}
/*********************************************************************
 *	16bitデータの受信
 *********************************************************************
 */
int	recvData16(void)
{
	int rc;
	add_data4(b_0001);	// send SIX command '0000'
	add_data8(0x00  );	// send 1 byte '0000_0000'

//	PGC=0;
	set_bit(PGC ,0);

//	dirPGD=1;			// INPUT
	set_dir(dirPGD ,1);

	wait_us(1);
	rc = pic24_getData16();

//	dirPGD=0;			// OUTPUT
	set_dir(dirPGD ,0);
	wait_us(1);
	send_nop1();
	return rc;
}


static	void WaitComplete(void)
{
	int r;
	bset_nvmcon();
	do {
		send_goto200();
		sendData24(0x803B00);	// MOV NVMCON, W0
		sendData24(0x883C20);	// MOV W0, VLSI
		send_nop1();
		r = recvData16();
	} while (r & 0x8000);
}
/********************************************************************
 *	PIC24F:Flash書き込み
 ********************************************************************
 */
void pic_write24f(void)
{
	uchar i;
	uchar cmd4=PacketFromPC.piccmd4;	// 0=address setupする. 1=継続書き込み 2=complete処理あり.
	uchar len =PacketFromPC.size;		// ループ回数. ＝データbyte数 / 6 と同じ.
//	ushort *wordp=(ushort *)PacketFromPC.picdata;
	uchar *wordp = PacketFromPC.picdata;

	if(cmd4==0) {
		setup_addr24write();	//(adr>>1,0x4001);
	}

	for(i=0; i<len; i++) {
		sendData24(0xEB0300);					// CLR W6
		sendData24(0x200000| (getword(wordp+0)<<4) );	// MOV  #w0, W0
		sendData24(0x200001| (getword(wordp+2)<<4) );	// MOV  #w1, W1
		sendData24(0x200002| (getword(wordp+4)<<4) );	// MOV  #w2, W2
		sendData24(0xBB0BB6);					// TBLWTL [W6++], [W7]
		send_nop2();
		sendData24(0xBBDBB6);					// TBLWTH.B [W6++], [W7++]
		send_nop2();
		sendData24(0xBBEBB6);					// TBLWTH.B [W6++], [++W7]
		send_nop2();
		sendData24(0xBB1BB6);					// TBLWTL [W6++], [W7++]
		send_nop2();
//		wordp+=3;
		wordp+=3*2;
	}

	if(cmd4==2) {
		WaitComplete();
		send_goto200();
	}
}
/********************************************************************
 *	PIC24F:Flash読み出し
 ********************************************************************
 */
void pic_read24f(void)
{
	uchar i;
	uchar len=PacketFromPC.size;
//	ushort *wordp=(ushort *)PacketToPC.raw;
	uchar *wordp=PacketToPC.raw;

	setup_addr24();			//(adr>>1);

	for(i=0; i<len; i++) {
		sendData24(0xBA0B96);				// TBLRDL [W6], [W7]
		send_nop2();
//		wordp[0] = recvData16();
		setword(wordp , recvData16());

		sendData24(0xBADBB6);				// TBLRDH.B [W6++], [W7++]
		send_nop2();
		sendData24(0xBAD3D6);				// TBLRDH.B [++W6], [W7--]
		send_nop2();
//		wordp[1] = recvData16();
		setword(wordp+2 , recvData16());

		sendData24(0xBA0BB6);				// TBLRDL [W6++], [W7]
		send_nop2();
//		wordp[2] = recvData16();
		setword(wordp+4 , recvData16());

//		wordp +=  3;
		wordp +=  3*2;
	}
}

#endif


/********************************************************************
 *	PICライター系コマンド受信と実行.
 ********************************************************************
 *	Cmd0 : 0x10～0x1F
 */
void cmd_picspx(void)
{
	led_blink(8);

#if	PICAVR_WRITER
	if(		Cmd0==PICSPX_SETCMD16L) {	pic_setcmd16L();	} 
	else if(Cmd0==PICSPX_SETADRS24) {	setaddress24();		} 
	else if(Cmd0==PICSPX_GETDATA8L) {	pic_getdata8L();	} 
	else if(Cmd0==PICSPX_BITBANG)   {	pic_bitbang();      } 
	else if(Cmd0==PICSPX_BITREAD)   {	pic_bitbang();		}
#if	SUPPORT_PIC24F
	else if(Cmd0==PICSPX_WRITE24F) 	{	pic_write24f();		} 
	else if(Cmd0==PICSPX_READ24F) 	{	pic_read24f();		}
#endif
	else 
#endif

#if	INCLUDE_JTAG_CMD
		 if(Cmd0==HIDASP_JTAG_WRITE) {	jtag_command(0);	} 
	else if(Cmd0==HIDASP_JTAG_READ)  {	jtag_command(1);	}
	else if(Cmd0==HIDASP_JTAG_CTRLW) {	jtag_control(0);	}
	else if(Cmd0==HIDASP_JTAG_CTRLR) {	jtag_control(1);	}
#endif

//	else if(Cmd0==PICSPX_SETPGM) 	{pic_setpgm();}

	ToPcRdy = Cmd0 & 1;	// LSBがOnなら返答が必要.
}


#endif	//APPLICATION_MODE
