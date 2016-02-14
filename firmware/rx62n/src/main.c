/********************************************************************
 *	メイン処理
 ********************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iodefine.h"
#include "usb/usb_hal.h"
#include "usb/usb_core.h"
#include "usb/cdc/usb_cdc.h"
#include "monitor/utype.h"
#include "monitor/usbio.h"
#include "monitor/gpio.h"
#include "monitor/monit.h"


/********************************************************************
 *
 ********************************************************************
 */
#define	ECHOBACK_TEST	(0)	// エコーバックテスト
#define	LED_PIN		P1_5

/********************************************************************
 *	周辺クロックを取得する変数
 ********************************************************************
 */
extern	uint		SysPCLK;	// 周辺クロック＝48MHz	(48,000,000)

/********************************************************************
 *	USB切り離し処理：未実装
 ********************************************************************
 */
void usbModuleDisable(void)
{
}
/********************************************************************
 *	ＬＥＤ点灯
 ********************************************************************
 */
void LEDon(void)
{
	digitalWrite(LED_PIN,0);	// LED点灯は 負論理
}

/********************************************************************
 *	ＬＥＤ消灯
 ********************************************************************
 */
void LEDoff(void)
{
	digitalWrite(LED_PIN,1);	// LED点灯は 負論理
}

/********************************************************************
 *	ＬＥＤ点灯を反転
 ********************************************************************
 */
void LEDflip(void)
{
	digitalInvert(LED_PIN);
}

/********************************************************************
 *	ＬＥＤ ピンを出力モードにする.
 ********************************************************************
 */
void init_LED(void)
{
	pinMode(LED_PIN,OUTPUT);
}


#define	LED_MAXCNT	50000	//*	ＬＥＤを一定周期で点滅させる間引きカウンタ.
/********************************************************************
 *	ＬＥＤを一定周期で点滅させる.
 ********************************************************************
 */
static	void	LED_blink(void)
{
	static int cnt=0;
	cnt++;
	if(	cnt >= LED_MAXCNT ) {
		cnt = 0 ;
		LEDflip();
	}
}

/********************************************************************
 *	ＬＥＤを一定周期で点滅させる.別バージョン
 ********************************************************************
 */
void led_blink(int x)
{
	static int cnt=0;
	cnt++;
	if(	cnt >= (1<<x) ) {
		cnt = 0 ;
		LEDflip();
	}
}


#define	SZ 64

volatile int	usb_read_size=0;
volatile int	usb_task_state=0;
static	uchar  	usb_read_buf[SZ]; 

/********************************************************************
 *	USBCDC_Read_Async()の終了コールバック関数
 ********************************************************************
 */
static void read_callback(USB_ERR err, uint32_t size)
{
	if(usb_task_state==1) {
		if((err==USB_ERR_OK)&&(size>0)) {
			USBenqueue(usb_read_buf,size);
			usb_read_size  = size;
		}else{
			usb_read_size  = 0;
		}
		usb_task_state = 0;
		LEDflip();
	}
}
/********************************************************************
 *	ＵＳＢ受信タスク.
 ********************************************************************
 */
void USB_rx_task(void)
{
	int rc;
	switch(usb_task_state) {
	 case 0:
		usb_task_state = 1;
		rc = USBCDC_Read_Async(SZ,usb_read_buf,read_callback);
		if(rc!=USB_ERR_OK) {
			usb_task_state = 0;	// Read_Async() 起動失敗.
		}
		break;
	 default:
		break;
	}
}

/********************************************************************
 *	タイマー初期化.
 ********************************************************************
 */
void init_TMR0(void)
{
	//module stop
	MSTP(TMR0) = 0;			//remove module stop bit (iodefine.h)

	//INT level:7(0:lowest - 15:highest) (CMT0 at IPRJ:15-12)
	IPR(TMR0, CMIA0) = 2;	//lower priority

	TMR0.TCNT = 0;					//(Compare-Match Count Reg.) count-up from 0
//	TMR0.TCORA = MSEC1-1;			//(Compare-Match Constant Reg.) 1msec
//	TMR0.TCORA = (SysPCLK/1024)/1000+1;	//(Compare-Match Constant Reg.) 1msec
#define	Tick_KHz	50000	// 50KHz割り込みしたい.
	TMR0.TCORA = (SysPCLK/1024)/Tick_KHz+1;//	1くらい.
	
	//clock source
	TMR0.TCCR.BYTE = 0x0d;	//1/1024
	
	//CMCSR(Compare-Match Control/Status Reg.)
	//bit 1-0	CKS		(*00,01,10,11)=(Pclk/8,Pclk/32.Pclk/128,Pclk/512)
	//    6		CMIE	(0/*1)=(disable/enable) INT
	//    7		CMF		(0/1) match flag
	TMR0.TCR.BIT.CCLR = 1;		//clear CNT on (0/1/2/3)=(none/CMA/CMB/Ext)
	TMR0.TCR.BIT.CMIEA = 1;		//enable INT of Compare-Match A

	IEN(TMR0, CMIA0) = 1;
}


/********************************************************************
 *	タイマー割り込み
 ********************************************************************
 */
void INT_Excep_TMR0_CMIA0(void) __attribute__ ((interrupt));
void INT_Excep_TMR0_CMIA0(void)
{
	//CMCSR(Compare Match Control/Status Reg)
	IR(TMR0, CMIA0) = 0;		//

	//automatical CMCNT=0, then count up
//	__asm__("setpsw I");		//enable multiple INT
#if	1
	USB_rx_task();
	LED_blink();
#endif
}







/********************************************************************
 *
 ********************************************************************
 */
int	usbSendBytes(uchar *buf,int size)
{
	int rc=size;
	int sz;
	while(size>0) {
		sz = size;
//		if(sz>63) sz=63;
		if(sz>64) sz=64;
		USBCDC_Write(sz,buf);
		buf  += sz;
		size -= sz;
	}
	return rc;
}

#if	0	//OLD_VERSION_ECHOBACK

void	loop(void)
{
	uchar  		  buf[SZ]; 
	unsigned long size;
	while(1) {
		size=0;
		USBCDC_Read(SZ,buf,&size);
		usbSendBytes(buf,size);
	}
}

#else
/********************************************************************
 *
 ********************************************************************
 */
#if	ECHOBACK_TEST		// エコーバックテスト

#define	BUF_SIZE	256

void loop(void)
{
	uchar buf[BUF_SIZE];

//	led2_blink(0);

	int rxsize = USBavailable();
	if( rxsize >= BUF_SIZE ) {
		rxsize  = BUF_SIZE;
	}

	if( rxsize ) {
		USBread( buf, rxsize );
		USBwrite( buf, rxsize );
	}
}


#else

void	USBmonit(void);
/********************************************************************
 *
 ********************************************************************
 */
void loop()
{
	while (1) {
		int ch = USBgetch();
		if(	ch == 0xaa) {
			ch = USBgetch();
			if(	ch == 0x55) {
				USBmonit();
			} else if(ch == 0xa5) {
				USBputch(0x55);
			}
		}
	}
}

#endif
#endif

/********************************************************************
 *
 ********************************************************************
 */
void setup(void)
{
	init_LED();
	IPR(ICU, SWINT) = 1;	//interrupt priority (0:noINT - 15:highest)
	IEN(ICU, SWINT) = 1;	//enable
	ICU.IER[3].BIT.IEN2 = 1;//IER03.IEN2 (swi26)
	USBCDC_Init();
	USBIOinit();
	init_TMR0();
}

/********************************************************************
 *
 ********************************************************************
 */
int main()
{
	setup();
	while(1) {
		loop();
	}
	return 0;
}

/********************************************************************
 *
 ********************************************************************
 */
