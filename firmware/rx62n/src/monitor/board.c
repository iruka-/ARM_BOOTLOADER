/********************************************************************
 *	ARM��ˑ� �� ������
 ********************************************************************
 *
 *
 *
 *
void Board_Init(void);			// ���ʔ���. (0x010c)

void GPIO_Configuration(void);	// GPIO������.
void led_on(void);				// LED �_��.
void led_off(void);				// LED ����.
void led_blink(int interval);	// LED �_��.
 *
 ********************************************************************
 */
#include <string.h>


#include "monit.h"
#include "utype.h"

#if	0
#include "stm32f10x.h"
#include "platform_config.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "stm32_eval.h"

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

#ifdef	STM8S_D
#define	LED_PORT		GPIOA
#define	LED_PIN			GPIO_Pin_8

#define	HWTYPE	0			// 0:STM8S 1:STBEE 2:CQ-STARM 3:STBEE-MINI
#define	ROMSIZE	(64/32)		// 32kB�̔{��.
#define	RAMSIZE	(20/4)		//  4kB�̔{��.
#define	SIGNATURESTRING	"STM8S-D"

#endif

#ifdef	STBEE
#define	LED_PORT		GPIOD
#define	LED_PIN			GPIO_Pin_4
#define	USB_PULLUP_PORT	GPIOD
#define	USB_PULLUP_BIT  GPIO_Pin_3

#define	HWTYPE	1
#define	ROMSIZE	(512/32)	// 32kB�̔{��.
#define	RAMSIZE	(20/4)		//  4kB�̔{��.
#define	SIGNATURESTRING	"STBEE"

#endif

#ifdef	CQ_STARM
#define	LED_PORT		GPIOC
#define	LED_PIN			GPIO_Pin_6

#define	HWTYPE	2
#define	ROMSIZE	(128/32)	// 32kB�̔{��.
#define	RAMSIZE	(20/4)		//  4kB�̔{��.
#define	SIGNATURESTRING	"CQ-STARM"

#endif

#ifdef	STBEE_MINI
#define	LED_PORT		GPIOA
#define	LED_PIN			GPIO_Pin_15
#define	USB_PULLUP_PORT	GPIOA
#define	USB_PULLUP_BIT  GPIO_Pin_14

#define	HWTYPE	3
#define	ROMSIZE	(128/32)	// 32kB�̔{��.
#define	RAMSIZE	(20/4)		//  4kB�̔{��.
#define	SIGNATURESTRING	"STBEE-MINI"

#endif

#define	_BV(n)		( 1 << (n) )

//
//	16byte �� ����:	�@�픻�ʗp
//
typedef	struct {
	uchar mark[4];	// "ARM"
	uchar signid;	// 0xa3 : ARM Cortex-M3
	uchar hwtype;	// �n�[�h�E�F�A�^�C�v.
	uchar romsize;	// ROM�e��.
	uchar ramsize;	// RAM�e��.
	uchar name[12];	// ���̕�����.
} SIGNATURE;
SIGNATURE *HW_Signature=(SIGNATURE *) 0x10c;

int	hwtype=HWTYPE;

//	=========				HW_STM8S,	HW_STBEE,	HW_CQ_STARM	,HW_STBEE_MINI,
void 	*led1_port_tab[]=	{GPIOA		,GPIOD		,GPIOC		,GPIOA,};			//LED�|�[�g.
int		 led1_pin_tab[]=	{GPIO_Pin_8	,GPIO_Pin_4	,GPIO_Pin_6	,GPIO_Pin_13,};		//LED�s���ԍ�.
void 	*led2_port_tab[]=	{GPIOA		,GPIOD		,GPIOC		,GPIOA,};			//LED�|�[�g.
int		 led2_pin_tab[]=	{GPIO_Pin_8	,GPIO_Pin_4	,GPIO_Pin_6	,GPIO_Pin_15,};		//LED�s���ԍ�.
void 	*usb_port_tab[]=	{0			,GPIOD		,0			,GPIOA,};			//USB�v���A�b�v�|�[�g.(�s�v�Ȃ�0)
int		 usb_pin_tab[]=		{0			,GPIO_Pin_3	,0			,GPIO_Pin_14,};		//USB�v���A�b�v�s���ԍ�.
int		 clock12M_tab[]=	{_8MHz		,_12MHz		,_8MHz		,_12MHz};			//�N���b�N(12MHz�Ȃ�1)

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
//	LED ���_��:
#define	LED_SetBits		GPIO_ResetBits
#define	LED_ResetBits	GPIO_SetBits
#else
//	LED ���_��:
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

	// JTAG�𖳌��ɂ��܂��B
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
//	AFIO->MAPR = _BV(26);

	// GPIO A,B,C,D �ɃN���b�N��^����.
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
	GPIO_InitStructureD.GPIO_Pin   = GPIO_Pin_9;			// PD9 �� USB PullUp�炵��.
	GPIO_InitStructureD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureD.GPIO_Mode  = GPIO_Mode_Out_PP; 		//GPIO_Mode_Out_OD;
	GPIO_Init(GPIOD, &GPIO_InitStructureD);

	// GPIO_A.8 = LED
	GPIO_InitStructureA.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStructureA.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureA.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructureA);

	GPIO_SetBits(GPIOA , GPIO_Pin_8);	// LED�_��.
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

	// JTAG�𖳌��ɂ��܂��B
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	AFIO->MAPR = _BV(26);

	// GPIO A,B,C,D �ɃN���b�N��^����.
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

	GPIO_SetBits(GPIOC , GPIO_Pin_6);	// LED�_��.


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

	// GPIO A,B,C,D �ɃN���b�N��^����.
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
	GPIO_InitStructureD.GPIO_Pin   = GPIO_Pin_3|GPIO_Pin_4;	// PD3 �� USB PullUp�炵��.
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

	// JTAG�𖳌��ɂ��܂��B
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	AFIO->MAPR = _BV(26);

	// GPIO A,B,C,D �ɃN���b�N��^����.
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
}


//	�ȉ���pullup����PIN�����_��(L�ŃA�N�e�B�u).
//		CQ_STARM
//		STBEE

//	�ȉ��́Apullup����PIN���g���Ă��Ȃ�.
//		STM8_D
//
void USB_Cable_Config(FunctionalState NewState)
{
	void* usb_pullup_port = usb_port_tab[hwtype];
	int   usb_pullup_bit  = usb_pin_tab[hwtype];
	int rev=0;

	if (hwtype==HW_STBEE_MINI) {
		rev=1;    // pull-up���t�ɐ�.
	}

	if (usb_pullup_port == 0) return;	// pull-up����o���Ȃ����.

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
//	USB_PULLUP_PORT ������o���Ȃ��@��.
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

	GPIOA->ODR = 0;		// USB D+ D- ��GND�ɂ���.

}

#else

//
//	USB_PULLUP_PORT �𐧌�o����@��.
//

//	STBEE
//	STBEE_MINI

void usbModuleDisable(void)
{
	PowerOff();					// Disconnect USB.
	USB_Cable_Config(DISABLE);
}

#endif

#endif







#if	APPLICATION_MODE


#define	PUTBUF_SIZE	(64-4)

uchar puts_buf[PUTBUF_SIZE];
uchar puts_ptr;

//#pragma code
#if	0
/********************************************************************
 *
 ********************************************************************
 */
int _user_putc(char c)
{
	uchar flush = 0;
	if( c == 0x0a) {
		flush = 1;
	}
	if( puts_ptr >= PUTBUF_SIZE ) {
		flush = 1;
	}

	if( flush ) {
		while(puts_ptr) {
			wait_ms(1);
		}
	}

	if(	puts_ptr < PUTBUF_SIZE ) {
		puts_buf[puts_ptr++]=c;
	}

	return 1;
}
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
	return t;
}
#endif

#endif	//APPLICATION_MODE

/********************************************************************
 *	( ms ) * 1mS �҂�.
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
 *	( us ) * 1uS �҂�.
 ********************************************************************
 */
void wait_us(int us)
{
	wait_125ns(us*8);
}
/********************************************************************
 *	( us * 1/8 ) uS �҂�. clock=72MHz�Ɖ���.
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
//clock=48MHz�Ɖ���.	�œK���I�v�V������R���p�C�����ς������Čv��.
void wait_125ns(int ns)
{
	int i;
	for(i=0; i<ns; i++) {
		asm("nop");
	}
}
#else
//clock=72MHz�Ɖ���.	�œK���I�v�V������R���p�C�����ς������Čv��.
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
 *	( ms ) * 1mS �҂�.
 ********************************************************************
 */
void wait_0us(void)
{

}

