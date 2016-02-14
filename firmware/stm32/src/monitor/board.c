/********************************************************************
 *	ARM基板依存 の 初期化
 ********************************************************************
#BASEBOARD = STM8S_D
#BASEBOARD = CQ_STARM
#BASEBOARD = STBEE
#BASEBOARD = STBEE_MINI
 *
 *
 *
 *
void Board_Init(void);			// 基板種別判定. (0x010c)

void GPIO_Configuration(void);	// GPIO初期化.
void led_on(void);				// LED 点灯.
void led_off(void);				// LED 消灯.
void led_blink(int interval);	// LED 点滅.
 *
 ********************************************************************
 */
#include <string.h>

#include "stm32f10x.h"
#include "utype.h"
#include "platform_config.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "stm32_eval.h"
#include "monit.h"
#include "gpio.h"

enum {	//HWTYPE	0:STM8S 1:STBEE 2:CQ-STARM 3:STBEE-MINI
	HW_STM8S,
	HW_STBEE,
	HW_CQ_STARM,
	HW_STBEE_MINI,
};

enum {	//HWTYPE	0:STM8S 1:STBEE 2:CQ-STARM 3:STBEE-MINI
	_8MHz,
	_12MHz,
};

#define	SMALL_MEMCPY	(1)	// サイズの小さなmemcpy()に置き換える.


#ifdef	STM8S_D
#define BOOT_PIN 		PB6

#define	HWTYPE	0			// 0:STM8S 1:STBEE 2:CQ-STARM 3:STBEE-MINI
#define	ROMSIZE	(64/32)		// 32kBの倍数.
#define	RAMSIZE	(20/4)		//  4kBの倍数.
#define	SIGNATURESTRING	"STM8S-D"

#endif

#ifdef	STBEE
#define BOOT_PIN 		PA(0)
#define	USB_PULLUP_PORT	GPIOD
#define	USB_PULLUP_BIT  GPIO_Pin_3

#define	HWTYPE	1
#define	ROMSIZE	(512/32)	// 32kBの倍数.
#define	RAMSIZE	(20/4)		//  4kBの倍数.
#define	SIGNATURESTRING	"STBEE"

#endif

#ifdef	CQ_STARM
#define BOOT_PIN 		PB9	//PB(9)

#define	HWTYPE	2
#define	ROMSIZE	(128/32)	// 32kBの倍数.
#define	RAMSIZE	(20/4)		//  4kBの倍数.
#define	SIGNATURESTRING	"CQ-STARM"

#endif

#ifdef	STBEE_MINI
#define BOOT_PIN 		PC13
#define	USB_PULLUP_PORT	GPIOA
#define	USB_PULLUP_BIT  GPIO_Pin_14

#define	HWTYPE	3
#define	ROMSIZE	(128/32)	// 32kBの倍数.
#define	RAMSIZE	(20/4)		//  4kBの倍数.
#define	SIGNATURESTRING	"STBEE-MINI"

#endif

#define	_BV(n)		( 1 << (n) )

//
//	16byte の 署名:	機種判別用
//
typedef	struct {
	uchar mark[4];	// "ARM"
	uchar signid;	// 0xa3 : ARM Cortex-M3
	uchar hwtype;	// ハードウェアタイプ.
	uchar romsize;	// ROM容量.
	uchar ramsize;	// RAM容量.
	uchar name[12];	// 名称文字列.
} SIGNATURE;
SIGNATURE *HW_Signature=(SIGNATURE *) 0x10c;

int	hwtype=HWTYPE;

//	=========				HW_STM8S,	HW_STBEE,	HW_CQ_STARM	,HW_STBEE_MINI,
void 	*led1_port_tab[]=	{GPIOA		,GPIOD		,GPIOC		,GPIOA,};			//LEDポート.
int		 led1_pin_tab[]=	{GPIO_Pin_8	,GPIO_Pin_4	,GPIO_Pin_6	,GPIO_Pin_13,};		//LEDピン番号.
void 	*led2_port_tab[]=	{GPIOA		,GPIOD		,GPIOC		,GPIOA,};			//LEDポート.
int		 led2_pin_tab[]=	{GPIO_Pin_8	,GPIO_Pin_4	,GPIO_Pin_6	,GPIO_Pin_15,};		//LEDピン番号.
void 	*usb_port_tab[]=	{0			,GPIOD		,0			,GPIOA,};			//USBプルアップポート.(不要なら0)
int		 usb_pin_tab[]=		{0			,GPIO_Pin_3	,0			,GPIO_Pin_14,};		//USBプルアップピン番号.
int		 clock12M_tab[]=	{_8MHz		,_12MHz		,_8MHz		,_12MHz};			//クロック(12MHzなら1)


#ifdef	STM8S_D

int	getBootSW(void)
{
	pinMode(PB5,OUTPUT);
	// GPIO_B.5 (SWIM RESET#) pull-up
	digitalWrite(PB5,1);
	
	// GND <=> RESET#(SWIM=PB6)間のジャンパークローズを見る.
	int boot_jumper = digitalRead( BOOT_PIN );

	// pull-upを戻す.
	digitalWrite(PB5,0);

	return boot_jumper;
}

#else
//#endif
// #if	defined(CQ_STARM)||defined(STBEE)||defined(STBEE_MINI)

int	getBootSW(void)
{
	// GND <=> RESET#(SWIM=PB6)間のジャンパークローズを見る.
	int boot_jumper = digitalRead( BOOT_PIN );

	return boot_jumper;
}

#endif


#if	BOOTLOADER_MODE

static void check_jumper_sw(int sp)
{
	int	boot_jumper = getBootSW();
//	if(	check_cold(sp) ) 
	{
		if(	boot_jumper ) {	//ジャンパーオープン.

			int *resvec = (int *) 0x08003000;	// reset vector
			int adrs = resvec[1];
			if((adrs & 0xfff00000) == 0x08000000) {	// 0x0800_0000 ～ 0x080f_ffff までの間にjump.
				call_func(adrs);
			}

		}
	}
}

#else

static void check_jumper_sw(int sp)
{
	(void)sp;
}

#endif

/********************************************************************
 *
 ********************************************************************
 */
int	Board_Init(void)
{
#if	0
	if (HW_Signature->mark[0]=='A') {
		if (HW_Signature->mark[1]=='R') {
			if (HW_Signature->mark[2]=='M') {
				if (HW_Signature->signid == 0xa3) {
					hwtype = HW_Signature->hwtype;
				}
			}
		}
	}
#endif
	return clock12M_tab[hwtype];
}

#ifdef	STBEE_MINI
//	LED 負論理:
#define	LED_SetBits		GPIO_ResetBits
#define	LED_ResetBits	GPIO_SetBits
#else
//	LED 正論理:
#define	LED_SetBits		GPIO_SetBits
#define	LED_ResetBits	GPIO_ResetBits
#endif
/********************************************************************
 *
 ********************************************************************
 */
void led_on(void)
{
	LED_SetBits(led1_port_tab[hwtype],led1_pin_tab[hwtype]);
}

/********************************************************************
 *
 ********************************************************************
 */
void led_off(void)
{
	LED_ResetBits(led1_port_tab[hwtype],led1_pin_tab[hwtype]);
}

/********************************************************************
 *
 ********************************************************************
 */
void led2_on(void)
{
	LED_SetBits(led2_port_tab[hwtype],led2_pin_tab[hwtype]);
}

/********************************************************************
 *
 ********************************************************************
 */
void led2_off(void)
{
	LED_ResetBits(led2_port_tab[hwtype],led2_pin_tab[hwtype]);
}

/********************************************************************
 *
 ********************************************************************
 */
void led_blink(int interval)
{
	static int led_cnt=0;
	int mask = 1<<interval;

	led_cnt++;
	if (led_cnt & mask) {
		led_on();
	} else {
		led_off();
	}
}

/********************************************************************
 *
 ********************************************************************
 */
void led2_blink(int interval)
{
	static int led_cnt=0;
	int mask = 1<<interval;

	led_cnt++;
	if (led_cnt & mask) {
		led2_on();
	} else {
		led2_off();
	}
}


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration_STM8S_D(void)
{
	GPIO_InitTypeDef GPIO_InitStructureA;
	GPIO_InitTypeDef GPIO_InitStructureB;
	GPIO_InitTypeDef GPIO_InitStructureC;
	GPIO_InitTypeDef GPIO_InitStructureD;

	// JTAGを無効にします。
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
//	AFIO->MAPR = _BV(26);

	// GPIO A,B,C,D にクロックを与える.
	RCC_APB2PeriphClockCmd(
	    RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO|
	    RCC_APB2Periph_GPIOB |
	    RCC_APB2Periph_GPIOC | /* == RCC_APB2Periph_GPIO_IOAIN      */
	    RCC_APB2Periph_GPIOD   /* == RCC_APB2Periph_GPIO_DISCONNECT */
	    , ENABLE);

	AFIO->MAPR = _BV(26);

	// GPIO_B.5 (SWIM RESET#) pull-up , B.4 =JNTRST
	GPIO_InitStructureB.GPIO_Pin   = 0;//GPIO_Pin_5;	//|GPIO_Pin_4;
	GPIO_InitStructureB.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureB.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructureB);

#if	BOOTLOADER_MODE
	GPIO_SetBits(GPIOB , GPIO_Pin_5);
#endif
//	GPIO_SetBits(GPIOB , GPIO_Pin_4);	// JNTRST=High

#if	0
	GPIO_InitStructureC.GPIO_Pin   = 0;
	GPIO_InitStructureC.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureC.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructureC);
#endif

	/* PD.09 used as USB pull-up */
	GPIO_InitStructureD.GPIO_Pin   = GPIO_Pin_9;			// PD9 は USB PullUpらしい.
	GPIO_InitStructureD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureD.GPIO_Mode  = GPIO_Mode_Out_PP; 		//GPIO_Mode_Out_OD;
	GPIO_Init(GPIOD, &GPIO_InitStructureD);

	// GPIO_A.8 = LED
	GPIO_InitStructureA.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStructureA.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureA.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructureA);

	GPIO_SetBits(GPIOA , GPIO_Pin_8);	// LED点灯.
}


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration_CQ_STARM(void)
{
	GPIO_InitTypeDef GPIO_InitStructureA;
	GPIO_InitTypeDef GPIO_InitStructureB;
	GPIO_InitTypeDef GPIO_InitStructureC;
	GPIO_InitTypeDef GPIO_InitStructureD;
	GPIO_InitTypeDef GPIO_InitStructureE;

	// JTAGを無効にします。
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	AFIO->MAPR = _BV(26);

	// GPIO A,B,C,D にクロックを与える.
	RCC_APB2PeriphClockCmd(
	    RCC_APB2Periph_GPIOA |
	    RCC_APB2Periph_GPIOB |
	    RCC_APB2Periph_GPIOC | /* == RCC_APB2Periph_GPIO_IOAIN      */
	    RCC_APB2Periph_GPIOD | /* == RCC_APB2Periph_GPIO_IOAIN      */
	    RCC_APB2Periph_GPIOE   /* == RCC_APB2Periph_GPIO_DISCONNECT */
	    , ENABLE);

	// GPIO_A
	GPIO_InitStructureA.GPIO_Pin   = 0;
	GPIO_InitStructureA.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureA.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructureA);

	// GPIO_B
	GPIO_InitStructureB.GPIO_Pin   = 0;
	GPIO_InitStructureB.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureB.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructureB);

	// GPIO_C.6 = LED
	GPIO_InitStructureC.GPIO_Pin   = GPIO_Pin_6;
	GPIO_InitStructureC.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureC.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructureC);

	GPIO_SetBits(GPIOC , GPIO_Pin_6);	// LED点灯.


	// GPIO_D
	GPIO_InitStructureD.GPIO_Pin   = 0;
	GPIO_InitStructureD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureD.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructureD);

	// GPIO_E
	GPIO_InitStructureE.GPIO_Pin   = 0;
	GPIO_InitStructureE.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureE.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructureE);
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration_STBEE(void)
{
//	GPIO_InitTypeDef GPIO_InitStructureA;
	GPIO_InitTypeDef GPIO_InitStructureB;
	GPIO_InitTypeDef GPIO_InitStructureC;
	GPIO_InitTypeDef GPIO_InitStructureD;

	// GPIO A,B,C,D にクロックを与える.
	RCC_APB2PeriphClockCmd(
	    RCC_APB2Periph_GPIOA |
	    RCC_APB2Periph_GPIOB |
	    RCC_APB2Periph_GPIOC | /* == RCC_APB2Periph_GPIO_IOAIN      */
	    RCC_APB2Periph_GPIOD   /* == RCC_APB2Periph_GPIO_DISCONNECT */
	    , ENABLE);

	// GPIO_B
	GPIO_InitStructureB.GPIO_Pin   = 0;
	GPIO_InitStructureB.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureB.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructureB);

	/* PD.3 used as USB pull-up (LOW=pull-up) */
	/* PD.4 = LED */
	GPIO_InitStructureD.GPIO_Pin   = GPIO_Pin_3|GPIO_Pin_4;	// PD3 は USB PullUpらしい.
	GPIO_InitStructureD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureD.GPIO_Mode  = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOD, &GPIO_InitStructureD);

	// GPIO_C.6 = LED
	GPIO_InitStructureC.GPIO_Pin   = GPIO_Pin_6;
	GPIO_InitStructureC.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureC.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructureC);

}
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration_STBEE_MINI(void)
{
	GPIO_InitTypeDef GPIO_InitStructureA;
	GPIO_InitTypeDef GPIO_InitStructureB;
	GPIO_InitTypeDef GPIO_InitStructureC;
//	GPIO_InitTypeDef GPIO_InitStructureD;

	// JTAGを無効にします。
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	AFIO->MAPR = _BV(26);

	// GPIO A,B,C,D にクロックを与える.
	RCC_APB2PeriphClockCmd(
	    RCC_APB2Periph_GPIOA |
	    RCC_APB2Periph_GPIOB |
	    RCC_APB2Periph_GPIOC | /* == RCC_APB2Periph_GPIO_IOAIN      */
	    RCC_APB2Periph_GPIOD   /* == RCC_APB2Periph_GPIO_DISCONNECT */
	    , ENABLE);

	// GPIO_A 13,15=LED 14=USB_Pullup 9=USART1_Tx
	GPIO_InitStructureA.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_9;
	GPIO_InitStructureA.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureA.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructureA);

	// GPIO_B
	GPIO_InitStructureB.GPIO_Pin   = 0;
	GPIO_InitStructureB.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureB.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructureB);

	// GPIO_C.6 = LED
	GPIO_InitStructureC.GPIO_Pin   = GPIO_Pin_6;
	GPIO_InitStructureC.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureC.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructureC);

}


/********************************************************************
 *
 ********************************************************************
 */
void GPIO_Configuration(void)
{
	int sp;

	switch (hwtype) {
	default:
	case HW_STM8S:
		GPIO_Configuration_STM8S_D();
		break;
	case HW_STBEE:
		GPIO_Configuration_STBEE();
		break;
	case HW_CQ_STARM:
		GPIO_Configuration_CQ_STARM();
		break;
	case HW_STBEE_MINI:
		GPIO_Configuration_STBEE_MINI();
		break;
	}

	check_jumper_sw( (int) &sp);
}


//	以下はpullup制御PINが負論理(Lでアクティブ).
//		CQ_STARM
//		STBEE

//	以下は、pullup制御PINが使われていない.
//		STM8_D
//
void USB_Cable_Config(FunctionalState NewState)
{
	void* usb_pullup_port = usb_port_tab[hwtype];
	int   usb_pullup_bit  = usb_pin_tab[hwtype];
	int rev=0;

	if (hwtype==HW_STBEE_MINI) {
		rev=1;    // pull-upが逆極性.
	}

	if (usb_pullup_port == 0) return;	// pull-up制御出来ない基板.

	if (rev) {
		if (NewState == DISABLE) {
			GPIO_ResetBits(usb_pullup_port,usb_pullup_bit);
		} else {
			GPIO_SetBits(  usb_pullup_port,usb_pullup_bit);
		}
	} else {
		if (NewState != DISABLE) {
			GPIO_ResetBits(usb_pullup_port,usb_pullup_bit);
		} else {
			GPIO_SetBits(  usb_pullup_port,usb_pullup_bit);
		}
	}
}



#ifndef	USB_PULLUP_PORT

//
//	USB_PULLUP_PORT が制御出来ない機種.
//

//	CQ_STARM
//	STM8S_D

void usbModuleDisable(void)
{
	PowerOff();			// Disconnect USB.
	GPIO_InitTypeDef GPIO_InitStructureA;

	// GPIO_A.8 = LED  11=USB D+ 12=USB D-
	GPIO_InitStructureA.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12; // LED,USB D+,D-
	GPIO_InitStructureA.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureA.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructureA);

	GPIOA->ODR = 0;		// USB D+ D- をGNDにする.

}

#else

//
//	USB_PULLUP_PORT を制御出来る機種.
//

//	STBEE
//	STBEE_MINI

void usbModuleDisable(void)
{
	PowerOff();					// Disconnect USB.
	USB_Cable_Config(DISABLE);
}

#endif







#if	SMALL_MEMCPY
/********************************************************************
 *
 ********************************************************************
 */
void *memcpy(void *dst,const void *src,size_t size)
{
	char *t = (char*)dst;
	char *s = (char*)src;
	while(size--) {
		*t++=*s++;
	}
	return dst;
}
#endif
/********************************************************************
 *	LPC1343/LPC2388 only. STM32ではダミー関数.
 ********************************************************************
 *	ページ単位でのFLASHプログラミング.(BOOT-ROM関数呼び出しなどの実行)
 */
void FLASH_ProgramPage(int adr)
{
}


/********************************************************************
 *	( ms ) * 1mS 待つ.
 ********************************************************************
 */
void wait_ms(int ms)
{
	int i;
	for(i=0; i<ms; i++) {
		wait_125ns(1000*8);
	}
}

/********************************************************************
 *	( us ) * 1uS 待つ.
 ********************************************************************
 */
void wait_us(int us)
{
	wait_125ns(us*8);
}
/********************************************************************
 *	( us * 1/8 ) uS 待つ. clock=72MHzと仮定.
 ********************************************************************
 8002186:	bf00      	nop
 8002188:	bf00      	nop
 800218a:	bf00      	nop
 800218c:	bf00      	nop
 800218e:	bf00      	nop
 8002190:	3301      	adds	r3, #1
 8002192:	4283      	cmp	r3, r0
 8002194:	dbf7      	blt.n	8002186 <wait_u8s+0x4>
 */
#ifdef	MARY
//clock=48MHzと仮定.	最適化オプションやコンパイラが変わったら再計測.
void wait_125ns(int ns)
{
	int i;
	for(i=0; i<ns; i++) {
		asm("nop");
	}
}
#else
//clock=72MHzと仮定.	最適化オプションやコンパイラが変わったら再計測.
void wait_125ns(int ns)
{
	int i;
	for(i=0; i<ns; i++) {
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
	}
}
#endif
/********************************************************************
 *	( ms ) * 1mS 待つ.
 ********************************************************************
 */
void wait_0us(void)
{

}

