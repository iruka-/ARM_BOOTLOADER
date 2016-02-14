#include "includes.h"

#include "monitor/gpio.h"
#include "monitor/monit.h"
#include "cpu.h"

//PF6  = BLUE  LED
//PC5  = WHITE LED

#define	BLUELED	 PC5
#define	WHITELED PF6
void	CDC_read_kick(void);
void	CDC_start_check(void);
void	cache_enable();


//タイマー割り込みで更新するカウンタ.
volatile int g_TimerTick=0;

#define	CMTLVL	8		//タイマー割り込みの優先度(1..15)

/********************************************************************
 *	TIMER の制御:タイマー割り込みで更新するカウンタ値を取得.
 ********************************************************************
 */
int	TIMER_getTick(void)
{
	return g_TimerTick;	// １秒間に10000カウント.
}

/********************************************************************
 *	TIMER の制御:初期化	10kHz割り込み.
 ********************************************************************
 */
void init_cmt0(int lvl)
{
	/* スタンバイコントロールレジスタ7 */
	CPG.STBCR7.BIT.MSTP72 = 0;			/* コンペアマッチタイマへクロックを供給 */

	/* コンペアマッチタイマスタートレジスタ */
	CMT.CMSTR.BIT.STR0 = 0;				/* CMCNT_0はカウントを停止 */

	/* コンペアマッチタイマコントロール/ステータスレジスタ */
	if(1 == CMT.CMCSR0.BIT.CMF)
	{
		CMT.CMCSR0.BIT.CMF = 0;			/* コンペアマッチフラグをクリア */
	}
	CMT.CMCSR0.BIT.CMIE = 1;			/* コンペアマッチ割込みを許可 */
	CMT.CMCSR0.BIT.CKS = 0;				/* クロックセレクト Pφ/8 */

	/* コンペアマッチコンスタントレジスタ */
	CMT.CMCOR0.WORD = 24000000/8/1000 - 1;		/* 1KHz */
//	CMT.CMCOR0.WORD = 24000000/8/10000 - 1;		/* 10KHz */
//	CMT.CMCOR0.WORD = 24000000/8/40000 - 1;		/* 10KHz */

	/* コンペアマッチタイマスタートレジスタ */
	CMT.CMSTR.BIT.STR0 = 1;				/* CMCNT_0はカウントを開始 */

	/* 割り込み優先レベル設定レジスタ10 */
	INTC.IPR10.BIT._CMT0 = lvl;			/* コンペアマッチタイマ チャネル1 */
}

/********************************************************************
 *	TIMER の制御:停止	10kHz割り込み.
 ********************************************************************
 */
void reset_cmt0(void)
{
	/* スタンバイコントロールレジスタ7 */
	CPG.STBCR7.BIT.MSTP72 = 1;			/* コンペアマッチタイマへクロックを供給 */

	/* コンペアマッチタイマスタートレジスタ */
	CMT.CMSTR.BIT.STR0 = 1;				/* CMCNT_0はカウントを停止 */

	CMT.CMCSR0.BIT.CMIE = 0;			/* コンペアマッチ割込みを許可 */
	CMT.CMCSR0.BIT.CKS = 0;				/* クロックセレクト Pφ/8 */

	/* コンペアマッチタイマスタートレジスタ */
	CMT.CMSTR.BIT.STR0 = 0;				/* CMCNT_0はカウントを開始 */

	/* 割り込み優先レベル設定レジスタ10 */
	INTC.IPR10.BIT._CMT0 = 0;			/* コンペアマッチタイマ チャネル1 */

	return;
}


/********************************************************************
 *	TIMER の制御:割り込み処理
 ********************************************************************
 */
void int_cmt_cmi0(void)
{
	if(	CMT.CMCSR0.BIT.CMF == 1) {
		CMT.CMCSR0.BIT.CMF = 0;			/* コンペアマッチフラグをクリア */
	}
	g_TimerTick++;
#if	0
	if(	g_TimerTick & 0x400) {
		digitalWrite(BLUELED,1);
	}else{
		digitalWrite(BLUELED,0);
	}
#endif

#if	1
	CDC_read_kick();	// USB-CDC読み込み開始キック.
#endif
}

#if	1
static int wled_data=0;
static int bled_data=0;

void BLUELED_flip()
{
	bled_data ++;
	digitalWrite(BLUELED	,bled_data&1);
}

static int wcnt=0;
void WHITELED_flip(int sft)
{
	wcnt++;
	if(	wcnt >= (1<<sft) ) {
		wcnt = 0;
		wled_data++;
		digitalWrite(WHITELED	,wled_data&1);
	}
}

void BLUELED_on()
{
	bled_data=0;
	digitalWrite(BLUELED	,bled_data&1);
}

void WHITELED_on()
{
	wcnt = 0;
	wled_data=0;
	digitalWrite(WHITELED	,wled_data&1);
}

#endif

/********************************************************************
 *	LED の制御:初期化
 ********************************************************************
 */
void LED_init(void)
{
	pinMode(PC8,OUTPUT);
	pinModeEx(PC8,0);

	pinMode(WHITELED,OUTPUT);
	pinMode(BLUELED	,OUTPUT);

	init_cmt0(CMTLVL);
	
	digitalWrite(WHITELED	,1);	//off
	digitalWrite(BLUELED	,1);	//off
}

static int  led_data =0;

/********************************************************************
 *	LED の制御:反転
 ********************************************************************
 */
void LED_flip()
{
	led_data++;
	digitalWrite(PC8		,led_data&1);
}

/********************************************************************
 *	LED の制御:ON
 ********************************************************************
 */
void led_on()
{
	led_data=0;
	digitalWrite(PC8		,led_data&1);
}

/********************************************************************
 *	LED の制御:OFF
 ********************************************************************
 */
void led_off()
{
	led_data=1;
	digitalWrite(PC8		,led_data&1);
}

/********************************************************************
 *	LED の制御:一定期間で反転 (sftは 2のべきに直される)
 ********************************************************************
 */
void led_blink(int sft)
{
	static int cnt=0;
	cnt++;
	if(	cnt >= (1<<sft) ) {
		cnt = 0;
		LED_flip();
	}
}

/********************************************************************
 *	各種デバッグ機能の初期化.
 ********************************************************************
 */
void DEBUG_init(void)
{
	cache_enable();
	LED_init();
	CDC_start_check();
#if	0
	int i;

	for(i=0;i<32;i++) {
		LED_flip();
		wait_ms(20);
	}
#endif
	init_cmt0(CMTLVL);
}

