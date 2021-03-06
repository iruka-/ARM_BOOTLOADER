/********************************************************************
 *	
 ********************************************************************
 */
#include <string.h>

#if	0
#include "platform_config.h"
#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "stm32_eval.h"
#include "hw_config.h"
#endif

#include "hidcmd.h"
#include "monit.h"

#include "usbio.h"
#include "../uart.h"

#define	UART_NUM	0
#define BAUD_RATE	230400	// 115200

//#define	RX_SIZE		1024
//static  uchar RxBuff[RX_SIZE];
//static	FIFO RxQueue;

int	LPCCDC_init(void);

int	usbSendBytes(uchar *data,int size)
{
	UARTSend(UART_NUM,data,size);
	return size;
}
//-----------------------------------------------
void USBIOinit(void)
{
	LPCCDC_init();
}


//-----------------------------------------------
void  USBputch(int ch)
{
	uchar buf[4];
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
//		LED_Flip(16);
		ch = USBinkey();
		if(ch>=0) break;
	}
	return ch;
}

//-----------------------------------------------

