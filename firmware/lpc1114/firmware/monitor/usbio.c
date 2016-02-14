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
#include "../type.h"
#include "../uart.h"



#if	0

#define	RX_SIZE		1024
static uchar RxBuff[RX_SIZE];
static	FIFO RxQueue;
int	usbSendBytes(uchar *data,int size);	// usb_endp.c
//-----------------------------------------------
void USBIOinit(void)
{
	FIFO_init(&RxQueue,RxBuff,RX_SIZE);
}
//-----------------------------------------------
void USBenqueue(uchar *buf,int size)
{
	FIFO_enqueue(&RxQueue,buf,size);
}
#endif
//-----------------------------------------------
int USBavailable(void)
{
	return UARTReceive_Check();
//	return FIFO_getsize(&RxQueue);		//キューにたまっているバイト数を得る.
}
//-----------------------------------------------
int USBread(uchar *buf,int len)
{
    if (!buf) {
        return 0;
    }

	int i;
	for(i=0;i<len;i++) {
		*buf++ = UARTReceive_Byte();
	}

#if	0
    int rxed = 0;
    while (rxed < len) {
		while( USBavailable()==0 ) { /**/ }

        rxed += FIFO_dequeue(&RxQueue, buf + rxed, len - rxed);
    }

    return rxed;
#endif
	return len;
}

//-----------------------------------------------
int	USBwrite(uchar *data,int size)
{
	if(size==0) return size;

	UARTSend(data,size);
	return size;
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
		ch = USBinkey();
		if(ch>=0) break;
	}
	return ch;
}

//-----------------------------------------------

