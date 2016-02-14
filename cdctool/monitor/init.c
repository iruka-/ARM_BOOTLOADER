/********************************************************************
 *	������
 ********************************************************************
#BASEBOARD = STM8S_D
#BASEBOARD = CQ_STARM
#BASEBOARD = STBEE
#BASEBOARD = STBEE_MINI
 */
#include "hard.h"
#include "boards/boards.h"
#include "systick.h"
#include "timer.h"
#include "utype.h"

#include "util.h"

#include "nvic.h"

#define	digitalWrite DigitalWrite
#define digitalRead DigitalRead
#define pinMode PinMode


#if   defined(BOARD_CQSTARM)
#	define	CQ_STARM
#elif defined(BOARD_DISCOVERY)
#	define	STM8S_D
#elif defined(BOARD_STBee)
#	define	STBEE
#elif defined(BOARD_STBeeMini)
#	define	STBEE_MINI
#endif


#ifdef	STM8S_D
//--------------------------------------------------------
//		STM8S_D
//--------------------------------------------------------
#define	LED_PORT		GPIOA
#define	LED_PIN			GPIO_Pin_8
#define BOOT_PIN 		PB(6)
#define	USB_D_plus 		PA(11)
#define	USB_D_minus		PA(12)

#define	HWTYPE	0			// 0:STM8S 1:STBEE 2:CQ-STARM 3:STBEE-MINI
#define	ROMSIZE	(64/32)		// 32kB�̔{��.
#define	RAMSIZE	(20/4)		//  4kB�̔{��.
#define	SIGNATURESTRING	"STM8S-D"

#endif

#ifdef	STBEE
//--------------------------------------------------------
//		STBEE
//--------------------------------------------------------
#define	LED_PORT		GPIOD
#define	LED_PIN			GPIO_Pin_4
#define BOOT_PIN 		PA(0)
//#define	USB_PULLUP_PORT	GPIOD
//#define	USB_PULLUP_BIT  GPIO_Pin_3
#define	USB_PULLUP_PIN	PD(3)

#define	HWTYPE	1
#define	ROMSIZE	(512/32)	// 32kB�̔{��.
#define	RAMSIZE	(20/4)		//  4kB�̔{��.
#define	SIGNATURESTRING	"STBEE"

#endif

#ifdef	CQ_STARM
//--------------------------------------------------------
//		CQ_STARM
//--------------------------------------------------------
#define	LED_PORT		GPIOC
#define	LED_PIN			GPIO_Pin_6
#define BOOT_PIN 		PB(9)
#define	USB_D_plus 		PA(11)
#define	USB_D_minus		PA(12)

#define	HWTYPE	2
#define	ROMSIZE	(128/32)	// 32kB�̔{��.
#define	RAMSIZE	(20/4)		//  4kB�̔{��.
#define	SIGNATURESTRING	"CQ-STARM"

#endif


#ifdef	STBEE_MINI
//--------------------------------------------------------
//		STBEE_MINI
//--------------------------------------------------------
#define	LED_PORT		GPIOA
#define	LED_PIN			GPIO_Pin_15
#define BOOT_PIN 		PC13	//PC(13)
//#define	USB_PULLUP_PORT	GPIOA
//#define	USB_PULLUP_BIT  GPIO_Pin_14
#define	USB_PULLUP_PIN	PA(14)

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



//__attribute__ ((section(".isr_vector")))

__attribute__ ((section(".signature")))
const SIGNATURE HW_Signature=
{
	"ARM",
	0xa3,
	HWTYPE,
	ROMSIZE,
	RAMSIZE,
	SIGNATURESTRING
//	"01234567890"
};

//-----------------------------------------------
//	
//-----------------------------------------------
int BlinkPeriod=200000;


void *get_signature()
{
	return (void*) &HW_Signature;
}


/*******************************************************************************
* Function Name  : USB_Cable_Config.
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : NewState: new state.
* Output         : None.
* Return         : None.
*******************************************************************************/
#ifdef	STBEE_MINI

void USB_Cable_Config (FunctionalState NewState)
{
#ifdef	USB_PULLUP_PORT
	if (NewState == DISABLE) {
//		GPIO_ResetBits(USB_PULLUP_PORT, USB_PULLUP_BIT);
		digitalWrite(USB_PULLUP_PIN , 0);
	} else {
//		GPIO_SetBits(  USB_PULLUP_PORT, USB_PULLUP_BIT);
		digitalWrite(USB_PULLUP_PIN , 1);
	}
#endif
}

#else
//	�ȉ���pullup����PIN�����_��(L�ŃA�N�e�B�u).
//		CQ_STARM
//		STBEE

//	�ȉ��́Apullup����PIN���g���Ă��Ȃ�.
//		STM8_D
//
void USB_Cable_Config (FunctionalState NewState)
{
#ifdef	USB_PULLUP_PORT
	if (NewState != DISABLE) {
//		GPIO_ResetBits(USB_PULLUP_PORT, USB_PULLUP_BIT);
		digitalWrite(USB_PULLUP_PIN , 0);
	} else {
//		GPIO_SetBits(  USB_PULLUP_PORT, USB_PULLUP_BIT);
		digitalWrite(USB_PULLUP_PIN , 1);
	}
#endif
}
#endif



#ifdef	USB_PULLUP_PIN

//
//	USB_PULLUP_PORT �𐧌�o����@��.
//

//	STBEE
//	STBEE_MINI

void usbModuleDisable(void)
{
	usbPowerOff();					// usb�@�\���~.
	USB_Cable_Config(DISABLE);		// usb pull-up ����~.
}


#else

//
//	USB_PULLUP_PORT ������o���Ȃ��@��.
//

//	CQ_STARM
//	STM8S_D

void usbModuleDisable(void)
{
	usbPowerOff();					// usb�@�\���~.

//	USB_Cable_Config(DISABLE);		// usb pull-up ����~.
//	������ USB_D+ , D- ��low���x���ɂ���.
	pinMode(USB_D_plus , OUTPUT);
    pinMode(USB_D_minus, OUTPUT);
	digitalWrite(USB_D_plus , 0);
	digitalWrite(USB_D_minus, 0);
}

#endif

#ifdef	STM8S_D

int	getBootSW(void)
{
	PinMode(PB(5),OUTPUT);
	// GPIO_B.5 (SWIM RESET#) pull-up
	DigitalWrite(PB(5),1);
	
	// GND <=> RESET#(SWIM=PB6)�Ԃ̃W�����p�[�N���[�Y������.
	int boot_jumper = DigitalRead( BOOT_PIN );

	// pull-up��߂�.
	DigitalWrite(PB(5),0);

	return boot_jumper;
}

#else
//#endif
// #if	defined(CQ_STARM)||defined(STBEE)||defined(STBEE_MINI)

int	getBootSW(void)
{
	// GND <=> RESET#(SWIM=PB6)�Ԃ̃W�����p�[�N���[�Y������.
	int boot_jumper = DigitalRead( BOOT_PIN );

	return boot_jumper;
}

#endif

//-----------------------------------------------
//	LED �𔽓]
//-----------------------------------------------
void toggleLED()
{
	TogglePin(BOARD_LED_PIN);
}
//-----------------------------------------------
//	
//-----------------------------------------------
void LEDtest()
{
//    pinMode(BOARD_LED_PIN, OUTPUT);

	while(1) {
		toggleLED();
		wait_ms(500);
	}
}

//-----------------------------------------------
//	(BlinkPeriod)��Ă΂ꂽ��A LED �𔽓]
//-----------------------------------------------
void BlinkTask()
{
	static int cnt=0;
	cnt++;
	if(	cnt >= BlinkPeriod) {
		cnt=0;
	    toggleLED();
	}
}

//-----------------------------------------------
//	
//-----------------------------------------------
