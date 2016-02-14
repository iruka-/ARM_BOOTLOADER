/*
	LPCUSB, an USB device driver for LPC microcontrollers	
	Copyright (C) 2006 Bertrik Sikken (bertrik@sikken.nl)

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	   derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
	Minimal implementation of a USB serial port, using the CDC class.
	This example application simply echoes everything it receives right back
	to the host.

	Windows:
	Extract the usbser.sys file from .cab file in C:\WINDOWS\Driver Cache\i386
	and store it somewhere (C:\temp is a good place) along with the usbser.inf
	file. Then plug in the LPC214x and direct windows to the usbser driver.
	Windows then creates an extra COMx port that you can open in a terminal
	program, like hyperterminal.

	Linux:
	The device should be recognised automatically by the cdc_acm driver,
	which creates a /dev/ttyACMx device file that acts just like a regular
	serial port.

*/


#include <string.h>			// memcpy

#include "type.h"
#include "usbdebug.h"

#include "armVIC.h"

//#include "console.h"
#include "usbapi.h"
#include "hwlib/startup.h"
/*
#include "hwlib/serial_fifo.h"
*/
#include "monitor/fifo.h"
#include "monitor/usbio.h"

#include "hwlib/swi.h"
#include "hwlib/led.h"

#ifdef	LPC214x
#include "hwlib/LPC214x.h"
#else
#include "hwlib/LPC23xx.h"
#endif

//#include "target.h"

#define BAUD_RATE	115200

#define INT_IN_EP		0x81
#define BULK_OUT_EP		0x05
#define BULK_IN_EP		0x82

#define MAX_PACKET_SIZE	64

#define	USB_STRING_DESCRIPTOR_TYPE	DESC_STRING

#if	0
// %DESCRIPTION%=LPC17xxUSB, USB\VID_1FC9&PID_2002 
#define VENDOR_ID		0x1FC9	// LPCxxxx Sample Program (Keil)
#define PRODUCT_ID		0x2002
#else
#define VENDOR_ID		0x26FF
#define PRODUCT_ID		0x4912
#endif

#define LE_WORD(x)		((x)&0xFF),((x)>>8)

// CDC definitions
#define CS_INTERFACE			0x24
#define CS_ENDPOINT				0x25

#define	SET_LINE_CODING			0x20
#define	GET_LINE_CODING			0x21
#define	SET_CONTROL_LINE_STATE	0x22

// data structure for GET_LINE_CODING / SET_LINE_CODING class requests
typedef struct {
	U32		dwDTERate;
	U8		bCharFormat;
	U8		bParityType;
	U8		bDataBits;
} TLineCoding;

static TLineCoding LineCoding = {115200, 0, 0, 8};
static U8 abBulkBuf[64];
static U8 abBulkOutBuf[64];
static U8 abClassReqData[8];

/*
int FIFO_init( FIFO *queue,uchar *buf,int size);	//初期化.
int FIFO_enqueue(FIFO *queue,uchar *buf,int size);	//キューにためる.
int FIFO_dequeue(FIFO *queue,uchar *buf,int size);	//キューから取り出す.
int FIFO_getsize(FIFO *queue);					//キューにたまっているバイト数を得る.
int FIFO_getfreesize(FIFO *queue);				//キューの空き容量バイト数を得る.
*/

#define VCOM_FIFO_SIZE	4096
typedef	FIFO fifo_t;

static U8 txdata[VCOM_FIFO_SIZE];
static U8 rxdata[VCOM_FIFO_SIZE];
static fifo_t txfifo;
static fifo_t rxfifo;
static char TXsend_disable = 0;

static char	flag_BulkOutPending=0;
// forward declaration of interrupt handler
static void USBIntHandler(void) ;	//__attribute__ ((interrupt("IRQ")));



static const U8 abDescriptors[] = {

// device descriptor
	0x12,
	DESC_DEVICE,
	LE_WORD(0x0101),			// bcdUSB
	0x02,						// bDeviceClass
	0x00,						// bDeviceSubClass
	0x00,						// bDeviceProtocol
	MAX_PACKET_SIZE0,			// bMaxPacketSize
	LE_WORD(VENDOR_ID),			// idVendor
	LE_WORD(PRODUCT_ID),		// idProduct
	LE_WORD(0x0100),			// bcdDevice
	0x01,						// iManufacturer
	0x02,						// iProduct
	0x03,						// iSerialNumber
	0x01,						// bNumConfigurations

// configuration descriptor
	0x09,
	DESC_CONFIGURATION,
	LE_WORD(67),				// wTotalLength
	0x02,						// bNumInterfaces
	0x01,						// bConfigurationValue
	0x00,						// iConfiguration
	0xC0,						// bmAttributes
	0x32,						// bMaxPower
// control class interface
	0x09,
	DESC_INTERFACE,
	0x00,						// bInterfaceNumber
	0x00,						// bAlternateSetting
	0x01,						// bNumEndPoints
	0x02,						// bInterfaceClass
	0x02,						// bInterfaceSubClass
	0x01,						// bInterfaceProtocol, linux requires value of 1 for the cdc_acm module
	0x00,						// iInterface
// header functional descriptor
	0x05,
	CS_INTERFACE,
	0x00,
	LE_WORD(0x0110),
// call management functional descriptor
	0x05,
	CS_INTERFACE,
	0x01,
	0x01,						// bmCapabilities = device handles call management
	0x01,						// bDataInterface
// ACM functional descriptor
	0x04,
	CS_INTERFACE,
	0x02,
	0x02,						// bmCapabilities
// union functional descriptor
	0x05,
	CS_INTERFACE,
	0x06,
	0x00,						// bMasterInterface
	0x01,						// bSlaveInterface0
// notification EP
	0x07,
	DESC_ENDPOINT,
	INT_IN_EP,					// bEndpointAddress
	0x03,						// bmAttributes = intr
	LE_WORD(8),					// wMaxPacketSize
	0x0A,						// bInterval
// data class interface descriptor
	0x09,
	DESC_INTERFACE,
	0x01,						// bInterfaceNumber
	0x00,						// bAlternateSetting
	0x02,						// bNumEndPoints
	0x0A,						// bInterfaceClass = data
	0x00,						// bInterfaceSubClass
	0x00,						// bInterfaceProtocol
	0x00,						// iInterface
// data EP OUT
	0x07,
	DESC_ENDPOINT,
	BULK_OUT_EP,				// bEndpointAddress
	0x02,						// bmAttributes = bulk
	LE_WORD(MAX_PACKET_SIZE),	// wMaxPacketSize
	0x00,						// bInterval
// data EP in
	0x07,
	DESC_ENDPOINT,
	BULK_IN_EP,					// bEndpointAddress
	0x02,						// bmAttributes = bulk
	LE_WORD(MAX_PACKET_SIZE),	// wMaxPacketSize
	0x00,						// bInterval

//=============================================================
#if	1
/* USB String Descriptor (optional) */
//const uint8_t USB_StringDescriptor[] = {
/* Index 0x00: LANGID Codes */
  0x04,                              /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  LE_WORD(0x0409), /* US English */    /* wLANGID */
/* Index 0x01: Manufacturer */
  (13*2 + 2),                        /* bLength (13 Char + Type + lenght) */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  'N',0,
  'X',0,
  'P',0,
  ' ',0,
  'S',0,
  'E',0,
  'M',0,
  'I',0,
  'C',0,
  'O',0,
  'N',0,
  'D',0,
  ' ',0,
/* Index 0x02: Product */
  (17*2 + 2),                        /* bLength ( 17 Char + Type + lenght) */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  'N',0,
  'X',0,
  'P',0,
  ' ',0,
  'L',0,
  'P',0,
  'C',0,
  '1',0,
  '7',0,
  'x',0,
  'x',0,
  ' ',0,
  'V',0,
  'C',0,
  'O',0,
  'M',0,
  ' ',0,
/* Index 0x03: Serial Number */
  (12*2 + 2),                        /* bLength (12 Char + Type + lenght) */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  'D',0,
  'E',0,
  'M',0,
  'O',0,
  '0',0,
  '0',0,
  '0',0,
  '0',0,
  '0',0,
  '0',0,
  '0',0,
  '0',0,
/* Index 0x04: Interface 0, Alternate Setting 0 */
  ( 4*2 + 2),                        /* bLength (4 Char + Type + lenght) */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  'V',0,
  'C',0,
  'O',0,
  'M',0,
   0, 0,
#else
	// string descriptors
	0x04,
	DESC_STRING,
	LE_WORD(0x0409),

	0x0E,
	DESC_STRING,
	'L', 0, 'P', 0, 'C', 0, 'U', 0, 'S', 0, 'B', 0,

	0x14,
	DESC_STRING,
	'U', 0, 'S', 0, 'B', 0, 'S', 0, 'e', 0, 'r', 0, 'i', 0, 'a', 0, 'l', 0,

	0x12,
	DESC_STRING,
	'D', 0, 'E', 0, 'A', 0, 'D', 0, 'C', 0, '0', 0, 'D', 0, 'E', 0,

// terminating zero
	0
#endif
};


void fifo_init(fifo_t *fifo, U8 *buf)
{
	FIFO_init(fifo,buf,VCOM_FIFO_SIZE);
}
BOOL fifo_put(fifo_t *fifo, U8 c)
{
	uchar buf[4];
	buf[0]=c;
	FIFO_enqueue(fifo,buf,1);	//キューにためる.
	return 1;
}
BOOL fifo_get(fifo_t *fifo, U8 *pc)
{
	uchar buf[4];
	int rc = FIFO_dequeue(fifo,buf,1);	//キューから取り出す.
	if(rc) {
		*pc = buf[0];
		return 1;		//受信あり.
	}
	return 0;			//受信なし.
}
int fifo_avail(fifo_t *fifo)
{
	return FIFO_getsize(fifo);					//キューにたまっているバイト数を得る.
}
int fifo_free(fifo_t *fifo)
{
	return FIFO_getfreesize(fifo);				//キューの空き容量バイト数を得る.
}

#define	RxQueue	rxfifo

//-----------------------------------------------
void USBenqueue(uchar *buf,int size)
{
	FIFO_enqueue(&RxQueue,buf,size);
}
//-----------------------------------------------
int USBavailable(void)
{
	return FIFO_getsize(&RxQueue);		//キューにたまっているバイト数を得る.
}
//-----------------------------------------------
int USBread(uchar *buf,int len)
{
    if (!buf) {
        return 0;
    }

    int rxed = 0;
    while (rxed < len) {
		while( USBavailable()==0 ) { /**/ }
        rxed += FIFO_dequeue(&RxQueue, buf + rxed, len - rxed);
    }
    return rxed;
}

//-----------------------------------------------
int	USBwrite(uchar *data,int size)
{
	if(size==0) return size;

	FIFO_enqueue(&txfifo,data,size);	//キューにためる.

//	UARTSend(UART_NUM,data,size);
	return size;
}

/**
	Local function to handle incoming bulk data
		ホストPC ---> Device へ来るパケット.
	@param [in] bEP
	@param [in] bEPStatus
 */
static void BulkOut(U8 bEP, U8 bEPStatus)
{
	int iLen;

	// get data from USB into intermediate buffer
//	iLen = USBHwEPRead(bEP, abBulkBuf, sizeof(abBulkBuf));

	if (fifo_free(&rxfifo) < MAX_PACKET_SIZE) {
		flag_BulkOutPending=1;
		// may not fit into fifo
		return;
	}

	// get data from USB into intermediate buffer
	iLen = USBHwEPRead(bEP, abBulkOutBuf, sizeof(abBulkOutBuf));
	FIFO_enqueue(&rxfifo,abBulkOutBuf,iLen);		//キューにためる.
	flag_BulkOutPending=0;
}

#if	0
	for (i = 0; i < iLen; i++) {
		// put into FIFO
		if (!fifo_put(&rxfifo, abBulkBuf[i])) {
			// overflow... :(
//			ASSERT(FALSE);
//			break;
		}
	}
#endif


/**
	Local function to handle outgoing bulk data
		Device ---> ホストPC へ送るパケット.
	@param [in] bEP
	@param [in] bEPStatus
 */
static void BulkIn(U8 bEP, U8 bEPStatus)
{
	int i, iLen;

	if (fifo_avail(&txfifo) == 0) {
		// no more data, disable further NAK interrupts until next USB frame
		USBHwNakIntEnable(0);
		TXsend_disable = 1;
		return;
	}

	// get bytes from transmit FIFO into intermediate buffer
//	for (i = 0; i < MAX_PACKET_SIZE; i++) {
	for (i = 0; i < (MAX_PACKET_SIZE-1) ; i++) {
		if (!fifo_get(&txfifo, &abBulkBuf[i])) {
			break;
		}
	}
	iLen = i;
	
	// send over USB
	if (iLen > 0) {
		USBHwEPWrite(bEP, abBulkBuf, iLen);
	}
}


/**
	Local function to handle the USB-CDC class requests
		
	@param [in] pSetup
	@param [out] piLen
	@param [out] ppbData
 */
static BOOL HandleClassRequest(TSetupPacket *pSetup, int *piLen, U8 **ppbData)
{
	switch (pSetup->bRequest) {

	// set line coding
	case SET_LINE_CODING:
DBG("SET_LINE_CODING\n");
		memcpy((U8 *)&LineCoding, *ppbData, 7);
		*piLen = 7;
DBG("dwDTERate=%u, bCharFormat=%u, bParityType=%u, bDataBits=%u\n",
	LineCoding.dwDTERate,
	LineCoding.bCharFormat,
	LineCoding.bParityType,
	LineCoding.bDataBits);
		break;

	// get line coding
	case GET_LINE_CODING:
DBG("GET_LINE_CODING\n");
		*ppbData = (U8 *)&LineCoding;
		*piLen = 7;
		break;

	// set control line state
	case SET_CONTROL_LINE_STATE:
		// bit0 = DTR, bit = RTS
DBG("SET_CONTROL_LINE_STATE %X\n", pSetup->wValue);
		break;

	default:
		return FALSE;
	}
	return TRUE;
}


/**
	Initialises the VCOM port.
	Call this function before using VCOM_putchar or VCOM_getchar
 */
void VCOM_init(void)
{
	fifo_init(&txfifo, txdata);
	fifo_init(&rxfifo, rxdata);
}


/**
	Writes one character to VCOM port
	
	@param [in] c character to write
	@returns character written, or EOF if character could not be written
 */
int VCOM_putchar(int c)
{
	return fifo_put(&txfifo, c) ? c : -1;
}


/**
	Reads one character from VCOM port
	
	@returns character read, or EOF if character could not be read
 */
int VCOM_getchar(void)
{
	U8 c;
	
	return fifo_get(&rxfifo, &c) ? c : -1;
}


/**
	Interrupt handler
	
	Simply calls the USB ISR, then signals end of interrupt to VIC
 */
static void USBIntHandler(void)
{
	USBHwISR();
	VICVectAddr = 0x00;    // dummy write to VIC to signal end of ISR 	
}


static void USBFrameHandler(U16 wFrame)
{
	if( TXsend_disable ) {
	 if (fifo_avail(&txfifo) > 0) {
		// data available, enable NAK interrupt on bulk in
		USBHwNakIntEnable(INACK_BI);
		TXsend_disable = 0;
	 }
	}
	
	if( flag_BulkOutPending ){
		if (fifo_free(&rxfifo) >= MAX_PACKET_SIZE) {
			U8 bEP = BULK_OUT_EP;
			// get data from USB into intermediate buffer
			int iLen = USBHwEPRead(bEP, abBulkOutBuf, sizeof(abBulkOutBuf));
			FIFO_enqueue(&rxfifo,abBulkOutBuf,iLen);		//キューにためる.
			flag_BulkOutPending=0;
		}
	}
}

int	LPCCDC_init(void)
{
	{
	SCS |= 1;				/* Enable FIO0 and FIO1 */

	FIO1PIN2 = 0x04;		/* -|-|-|-|-|LED|-|- */
	FIO1DIR2 = 0x04;
	PINMODE3 = 0x00000020;
	}

	// init DBG
//	ConsoleInit(60000000 / (16 * BAUD_RATE));

	DBG("Initialising USB stack\n");

	// initialise stack
	USBInit();

	// register descriptors
	USBRegisterDescriptors(abDescriptors);

	// register class request handler
	USBRegisterRequestHandler(REQTYPE_TYPE_CLASS, HandleClassRequest, abClassReqData);

	// register endpoint handlers
	USBHwRegisterEPIntHandler(INT_IN_EP, NULL);
	USBHwRegisterEPIntHandler(BULK_IN_EP, BulkIn);
	USBHwRegisterEPIntHandler(BULK_OUT_EP, BulkOut);
	
	// register frame handler
	USBHwRegisterFrameHandler(USBFrameHandler);

	// enable bulk-in interrupts on NAKs
	USBHwNakIntEnable(INACK_BI);

	// initialise VCOM
	VCOM_init();

	DBG("Starting USB communication\n");

#ifdef	LPC214x
	// set up USB interrupt
	VICIntSelect &= ~(1<<22);               // select IRQ for USB
	VICIntEnable |= (1<<22);

	(*(&VICVectCntl0+INT_VECT_NUM)) = 0x20 | 22; // choose highest priority ISR slot 	
	(*(&VICVectAddr0+INT_VECT_NUM)) = (int)USBIntHandler;
#else
	RegisterVector(USB_INT, USBIntHandler, PRI_HIGHEST, CLASS_IRQ);
#endif
	
//	enableIRQ();
	IntEnable();

	// connect to bus
	USBHwConnect(TRUE);

	return 0;
}

/*************************************************************************
	main
	====
**************************************************************************/
#if	0
int main(void)
{
	int c;
	
	// PLL and MAM
	Initialize();

	LPCCDC_init();

	// echo any character received (do USB stuff in interrupt)
	while (1) {

		LED_Flip(16);

		c = VCOM_getchar();
		if (c != (-1)) {
//			LED_Flip(1);
		//	c=tolower(c);
			VCOM_putchar(c);
		}
	}

	return 0;
}
#endif
