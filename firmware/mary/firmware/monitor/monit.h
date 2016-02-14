#ifndef	_monit_h_
#define	_monit_h_

#include "utype.h"

#if	_ROMADRS || _APPLICATION
//	0x0800_3000以降に配置:application mode
#define	BOOTLOADER_MODE		0
#define	APPLICATION_MODE	1

#else
//	0x0800_0000に配置:bootloader firmware
#define	BOOTLOADER_MODE		1
#define	APPLICATION_MODE	0

#endif


#ifdef	SH2A
#define	BSWAP16(i)	bswap16(i)
#define	BSWAP32(i)	bswap32(i)
#else
#define	BSWAP16(i)  (i)
#define	BSWAP32(i)  (i)
#endif


void UserInit(void);
void ProcessIO(void);
void USBtask(void);
void USBmonit(void);

void mon_int_handler(void);

/********************************************************************
 *	定義
 ********************************************************************
 */
#define	PACKET_SIZE		60		//64

typedef union {
	uchar raw[PACKET_SIZE];

	struct{
		uchar  cmd;
		uchar  subcmd;
		ushort size;

		uint   adrs;
		int    data[(PACKET_SIZE/4)-2];
	};

	int rawint[PACKET_SIZE/4];

	// PICwriter専用.
	struct{
		uchar  piccmd;
		uchar  picsize;
		uchar  picadrl;
		uchar  picadrh;
		uchar  picadru;
		uchar  piccmd4;
		uchar  picms;
		uchar  picdata[PACKET_SIZE - 7];
	};
} Packet;

enum {
	MEM_BYTE  = 0,
	MEM_HALF  = 1,
	MEM_WORD  = 2,
};



void wait_ms(int ms);
void wait_us(int us);
void wait_125ns(int ns125);
void wait_0us(void);

void led_on(void);
void led_off(void);
void led_blink(int interval);
void usbModuleDisable(void);
void setupVectorTable(int vect);	//FM3のみ.
void Init_Monitor(void);

#endif

