#ifndef	packet_h_
#define	packet_h_

#include "hidcmd.h"

#define	RX_DEFFERED		(1)		//遅延読み込みを許可.

//	for DEBUG
#define	USE_LOGFILE		(1)		// LOGFILE_print()をファイルに出力する.



#include "utype.h"
#if	0
typedef unsigned char   uchar;          // 8-bit
typedef unsigned char   byte;           // 8-bit

typedef unsigned short  ushort;         // 16-bit
typedef unsigned short  word;           // 16-bit

typedef unsigned int    uint;           // 32-bit
typedef unsigned int    dword;          // 32-bit
#endif

//
//
//
#define	REPORT_LENGTH	60		// USB 1Packet分の長さ(固定長、コマンドの場合は先頭にSOF)

typedef	struct {
	uchar report_id;	// 0x01固定.
	uchar cmd;
	uchar buffer[REPORT_LENGTH-2];
} cmdBuf;
//	sizeof(cmdBuf) == REPORT_LENGTH

#define	ZZ	printf("%s:%d: ZZ\n",__FILE__,__LINE__);

int		hidRead(uchar *buf, int len);
int		hidCommand(int cmd,int arg1,int arg2,int arg3);
int		flash_get_status();
void	strconv(int *ret,const char *str);
int		hidasp_init(int type, const char *serial, const char *baud);
int		UsbGetDevCaps(int *pflash_size,int *pflash_used);
void	hidasp_close();
void	memdump(char *msg, char *buf, int len);
int		hidasp_list(char * string);
int		UsbInit(int verbose,int enable_bulk, char *serial,char *baud);
int		UsbExit(void);
int 	UsbIsMary(void);

int		jcmd_write_flash();
int		jcmd_add(uchar *stream,int size);
int 	jcmd_write(uchar *stream,int size,uchar *result,int bitcnt);
void	dumpStream(uchar *p,int cnt,int wait);
int		SetJtagPort(int dir,int data0);
int		SetJtagSpeed(int speed);
int		set_ddrb(int d);


#if	USE_LOGFILE
FILE *LOGFILE_open(char *file);
int   LOGFILE_close(void);
int   LOGFILE_print(const char *format, ...);
#else
#define LOGFILE_open(file)   /**/
#define LOGFILE_close()      /**/
#define LOGFILE_print(...)   /**/
#endif

#endif

