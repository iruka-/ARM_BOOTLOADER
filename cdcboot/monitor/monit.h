/******************************************************************************
 
 *****************************************************************************/
#ifndef _MONIT_H_
#define _MONIT_H_

#include "libmaple_types.h"
//#include "util.h"

#ifdef __cplusplus
extern "C"{
#endif


void USBmonit(void);



//------------------------------------
uint32 USBavailable(void);
uint32 USBread(void *buf,int len);
void  USBwrite(void *buf,int len);
void  USBputch(int ch);
int   USBgetch(void);
int   USBinkey(void);
//------------------------------------


#include "utype.h"

#if	ROMADRS
//	0x0800_3000以降に配置:application mode
#define	BOOTLOADER_MODE		0
#define	APPLICATION_MODE	1

#else
//	0x0800_0000に配置:bootloader firmware
#define	BOOTLOADER_MODE		1
#define	APPLICATION_MODE	0

#endif


void UserInit(void);
void ProcessIO(void);
void USBtask(void);

void mon_int_handler(void);

/********************************************************************
 *	定義
 ********************************************************************
 */
#define	PACKET_SIZE		60	//64

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

//	ポート出力データ.    		  	JTAG  PIC   AVR
#define	PGC	 		PB11			// TCK  PGC =(AVR-SCK)
#define	PGM	 		PB10			// TDI  PGM =(AVR-MOSI)
#define	PGD	 		PB9				// TDO  PGD =(AVR-MISO)
#define	MCLR 		PB8				// TMS  MCLR=(AVR-RST)

//	ポート読み込み.
#define	inPGC	 	PB11
#define	inPGM	 	PB10
#define	inPGD	 	PB9
#define	inMCLR 		PB8

//	方向レジスタ (0=出力)
#define	dirPGC	 	PB11
#define	dirPGM	 	PB10
#define	dirPGD	 	PB9
#define	dirMCLR 	PB8


void wait_us(int us);
void wait_ms(int ms);
void wait_u8s(int u8s);
void led_on(void);
void led_off(void);
void led_blink(int interval);
void usbModuleDisable(void);
void Init_Monitor(void);


#ifdef __cplusplus
} // extern "C"
#endif

#endif


