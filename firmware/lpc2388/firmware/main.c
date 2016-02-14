/*
*/

#include <string.h>
#include "type.h"
#include "usbdebug.h"

#include "console.h"
#include "usbapi.h"
#include "startup.h"

#include "hwlib/led.h"
#include "monitor/utype.h"

#ifdef	LPC214x
#include "hwlib/LPC214x.h"
#else
#include "hwlib/LPC23xx.h"
#endif

#include "monitor/monit.h"
#include "monitor/usbio.h"
#include "hwlib/swi.h"

#define	ECHOBACK_TEST	(0)

// LED1 CQ-FRK-NXP
#define LED1_FIOPIN  FIO1PIN
#define LED1_FIOSET  FIO1SET
#define LED1_FIOCLR  FIO1CLR
#define LED1_MASK    (1UL<<18)	//0x00040000; P1[18]
#define UART_NUM     0			// uart0:

void USBmonit(void);


#if	ECHOBACK_TEST
int loop(void)
{
	int c = USBgetch();
	USBputch(c);
}
#else

int loop(void)
{
    while (1) {
		int ch = USBgetch();
		if(	ch == 0xaa) {
			ch = USBgetch();
			if(	ch == 0x55) {
				USBmonit();
			}else if(ch == 0xa5) {
				USBputch(0x55);
			}
		}
	}
}
#endif

/*************************************************************************
	main
**************************************************************************/
int main(void)
{
	// PLL and MAM
	Initialize();

	// init DBG
//	ConsoleInit(60000000 / (16 * BAUD_RATE));
	USBIOinit();
	TIMER0_Init();
	LED_On();

	IntEnable();
//	IrqEnable();
	GPIOInitDir(4,0x00000000);	// P4_0 ` P4_31 All Input.

	// call USB interrupt handler continuously
	while (1) {
//		LED_Flip(16);
		loop();
	}
	return 0;
}

