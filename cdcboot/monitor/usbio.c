/********************************************************************
 *	
 ********************************************************************
 */
#include <string.h>
#include "libmaple.h"
#include "monit.h"

#include "hidcmd.h"
#include "monit.h"
#include "picwrt.h"
#include "../libmaple/gpio.h"

#include "util.h"
#include "usb.h"
#include "systick.h"

#define USB_TIMEOUT 50

void		BlinkTask();
/**
 * Returns time (in milliseconds) since the beginning of program
 * execution. On overflow, restarts at 0.
 * @see micros()
 */
static inline uint32 millis(void) 
{
    return systick_uptime();
}

//-----------------------------------------------
uint32 USBavailable(void)
{
    return usbBytesAvailable();
}

//-----------------------------------------------
uint32 USBread(void *buf,int len)
{
    if (!buf) {
        return 0;
    }

    uint32 rxed = 0;
    while (rxed < len) {
		while( USBavailable()==0 ) { /**/ }

        rxed += usbReceiveBytes((uint8*)buf + rxed, len - rxed);
    }

    return rxed;
}

//-----------------------------------------------
void   USBwrite(void *buf,int len)
{
    if (!(usbIsConnected() && usbIsConfigured()) || !buf) {
        return;
    }

    uint32 txed = 0;
    uint32 old_txed = 0;
    uint32 start = millis();

    while (txed < len && (millis() - start < USB_TIMEOUT)) {
        txed += usbSendBytes((const uint8*)buf + txed, len - txed);
        if (old_txed != txed) {
            start = millis();
        }
        old_txed = txed;
    }
}
//-----------------------------------------------
void  USBputch(int ch)
{
	char buf[4];
	buf[0]=ch;
	USBwrite(buf,1);
}

//-----------------------------------------------
int  USBinkey(void)
{
	int len;
	uchar ch[4];

	len = USBavailable();
	if(len) {
       	USBread(ch,1);
		return ch[0];
	}
	return -1;
}

//-----------------------------------------------
int USBgetch(void)
{
	int ch = -1;
	while(1) {
		ch = USBinkey();
		if(ch>=0) break;
#if	APPLICATION_MODE
		BlinkTask();
		wait_u8s(40);
#endif
	}
	return ch;
}

//-----------------------------------------------

