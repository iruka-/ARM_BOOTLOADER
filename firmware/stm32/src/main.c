/********************************************************************
 *	ÉÅÉCÉìèàóù
 ********************************************************************
 */
#include <stdio.h>
#include <string.h>

#include "platform_config.h"
#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "stm32_eval.h"
#include "hw_config.h"
#include "monitor/hidcmd.h"
#include "monitor/monit.h"
#include "monitor/usbio.h"

#define	ECHOBACK_TEST	(0)

/********************************************************************
 *
 ********************************************************************
 */
void setup(void)
{
	Set_System();
	USBIOinit();

	USB_Interrupts_Config();
	Set_USBClock();
	USB_Init();
}


#if	ECHOBACK_TEST
/********************************************************************
 *
 ********************************************************************
 */
#define	BUF_SIZE	256

void USBtask_Echoback(void)
{
	uchar buf[BUF_SIZE];

	led2_blink(0);

	int rxsize = USBavailable();
	if( rxsize >= BUF_SIZE ) {
		rxsize  = BUF_SIZE;
	}

	if( rxsize ) {
		USBread( buf, rxsize );
		USBwrite( buf, rxsize );
	}
}
void loop()
{
	USBtask_Echoback();
}

#else
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
/********************************************************************
 *
 ********************************************************************
 */
int main(void)
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
