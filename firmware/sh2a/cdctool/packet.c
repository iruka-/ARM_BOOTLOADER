/*********************************************************************
 *	
 *********************************************************************
 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <signal.h>
#include <time.h>
#include "w32term.h"
#include "rs232c.h"
//#include "../../firmware/monitor/hidcmd.h"
#include "hidcmd.h"
#include "packet.h"

#define	BUFF_SIZE		256
#define DEBUG_PKTDUMP  	0		// �p�P�b�g���_���v����.


static	int dev_id       = 0;	// �^�[�Q�b�gID: 0x25 �������� 0x14 ���������e.
static	int dev_version  = 0;	// �^�[�Q�b�g�o�[�W���� hh.ll
static	int dev_bootloader= 0;	// �^�[�Q�b�g��bootloader�@�\������.

//static	int have_isp_cmd = 0;	// ISP����̗L��.

static	int	flash_size     = 0;	// FLASH_ROM�̍ŏI�A�h���X+1
static	int	flash_end_addr = 0;	// FLASH_ROM�̎g�p�ςݍŏI�A�h���X(4�̔{���؎�)
static	int	flash_err_count= 0;	// FLASH_ROM�̏������݃G���[.

char	reboot_done = 0;

void ChangePortListLPC1114(void);

/*
 *	HID�f�o�C�X�� HID Report �𑗐M����.
 *	���M�o�b�t�@�̐擪��1�o�C�g��ReportID �����鏈����
 *	���̊֐����ōs���̂ŁA�擪1�o�C�g��\�񂵂Ă�������.
 *
 *	id �� Length �̑g�̓f�o�C�X���Œ�`���ꂽ���̂łȂ���΂Ȃ�Ȃ�.
 *
 *	�߂�l��HidD_SetFeature�̖߂�l( 0 = ���s )
 *
 */
static int hidWrite(uchar *buf, int cnt)
{
	buf[0]=0x01;

#if	DEBUG_PKTDUMP
	memdump("WR", buf, cnt);
#endif

	RS_putdata(	buf , cnt );
	return 1;
}

int	hidWriteBuffer(uchar *buf, int cnt)
{
	buf[0]=0x01;

#if	DEBUG_PKTDUMP
	memdump("WB", buf, cnt);
#endif

	RS_putdata(	buf , cnt );
	return 1;
}

int	hidWriteBuffer2(uchar *buf, int cnt)
{

#if	DEBUG_PKTDUMP
	memdump("W2", buf, cnt);
#endif

	RS_putdata(	buf , cnt );
	return 1;
}

#if	0
// ����read
static int hidRead(uchar *buf, int cnt)
{
#if	DEBUG_PKTDUMP
	printf("RD: size=%d\n",cnt);
#endif
	int size;
	int rc;

	while(cnt) {
		if( RS_checkdata() ) {
			size = cnt;
			if(size>=60) size = 60;
			printf("RD[%d]) size=%d\n",cnt,size);
			rc = RS_getdata(buf,size);
			if(rc>0) {
				printf("RD[%d]: size=%d\n",cnt,size);
				buf += size;
				cnt -= size;
			}else{
				Sleep(1);
			}
		}else{
			printf("RD: size=%d ...\n",cnt);
			Sleep(10);
		}
	}
#if	DEBUG_PKTDUMP
	memdump("RD", buf, cnt);
#endif
	return 1;
}
#else
static int hidRead(uchar *buf, int cnt)
{
#if	DEBUG_PKTDUMP
	printf("RD: size=%d\n",cnt);
#endif

	int rc;
	while(1) {
		if( RS_checkdata()>=cnt ) {
			rc = RS_getdata(buf,cnt);
			if(rc>0) break;
		}
		Sleep(1);
	}
#if	DEBUG_PKTDUMP
	memdump("RD", buf, cnt);
#endif
	return 1;
}
#endif


int hidGetChars(void)
{
	int rc,ch;
	uchar buf[4];

	while(1) {
		Sleep(10);
		while( RS_checkdata() ) {
			rc = RS_getdata(buf,1);
			if(rc>0) {
				ch = buf[0];
				if(ch==0) {
					return 1;
				}else {
					putchar(ch);
				}
			}
		}
	}
}

int	hidReadPoll(uchar *buf, int Length, int id)
{
	return hidRead(buf, Length);
}

int	hidReadBuffer(uchar *buf, int len)
{
	return	hidRead(buf,len);
}
int hidCommand(int cmd,int arg1,int arg2,int arg3)
{
	unsigned char buf[BUFF_SIZE];

	memset(buf , 0, sizeof(buf) );

	buf[1] = cmd;
	buf[2] = arg1;
	buf[3] = arg2;
	buf[4] = arg3;

	return hidWrite(buf, PACKET_SIZE);
}

//
//	mask ��   0 �̏ꍇ�́A addr �� data0 ��1�o�C�g��������.
//	mask �� ��0 �̏ꍇ�́A addr �� data0 �� mask �̘_���ς���������.
//		�A���A���̏ꍇ�� mask bit�� 0 �ɂȂ��Ă��镔���ɉe����^���Ȃ��悤�ɂ���.
//
//	��:	PORTB �� 8bit �� data����������.
//		hidPokeMem( PORTB , data , 0 );
//	��:	PORTB �� bit2 ������ on
//		hidPokeMem( PORTB , 1<<2 , 1<<2 );
//	��:	PORTB �� bit2 ������ off
//		hidPokeMem( PORTB ,    0 , 1<<2 );
//
int hidPokeMem(int addr,int data0,int mask)
{
	unsigned char buf[BUFF_SIZE];
	memset(buf , 0, sizeof(buf) );

	buf[1] = HIDASP_POKE;
	buf[2] = 0;
	buf[3] = addr;
	buf[4] =(addr>>8);
	if( mask ) {
		buf[5] = data0 & mask;
		buf[6] = ~mask;
	}else{
		buf[5] = data0;
		buf[6] = 0;
	}
	return hidWrite(buf, PACKET_SIZE);
}

int hidPeekMem(int addr)
{
	unsigned char buf[BUFF_SIZE];
	memset(buf , 0, sizeof(buf) );

	buf[1] = HIDASP_PEEK;
	buf[2] = 1;
	buf[3] = addr;
	buf[4] =(addr>>8);

	hidWrite(buf, PACKET_SIZE);
	hidReadBuffer( buf, PACKET_SIZE );
	return buf[1];
}

int	flash_get_status()
{
	uchar rd_data[BUFF_SIZE];

	hidCommand(HIDASP_TEST,0,0,0);	// Connection test
	hidRead(rd_data , PACKET_SIZE);
	{
		int *fl_size;
		char *sBoot="(Application)";

		dev_id         = rd_data[1];
		dev_version    = rd_data[2] | (rd_data[3]<<8) ;
		dev_bootloader = rd_data[4];

		fl_size = (int*)&rd_data[8];
		flash_size     = fl_size[0];
		flash_end_addr = fl_size[1];
		flash_err_count= fl_size[2];

		if(dev_bootloader==1) {sBoot="(Bootloader)";}
		fprintf(stderr, "TARGET DEV_ID=%x VER=%d.%d%s FLASH=%x,%x\n",dev_id
				,dev_version>>8,dev_version & 0xff,sBoot
				,flash_end_addr,flash_size
				);

		if(dev_id == 0xa0) {
			ChangePortListLPC1114();
		}
	}
	return flash_err_count;
}


int hidasp_init(int type, const char *serial)
{
	int com_n = RS_printinfo(1);
	if( com_n==0) return -1;		// NG

	RS_init(com_n  ,BAUD_RATE);			// 'COMx:' ���I�[�v������.

#if	1
	int i;
	for(i=0;i<160;i++) {
		RS_putc(0xff);
	}
#endif

	RS_putc(0xaa);
	RS_putc(0x55);

	return flash_get_status();
}

//----------------------------------------------------------------------------
int	UsbGetDevCaps(int *pflash_size,int *pflash_used)
{
	if(pflash_size)	{*pflash_size = flash_size;}
	if(pflash_used)	{*pflash_used = flash_end_addr;}

	return dev_bootloader;
}

//----------------------------------------------------------------------------
//  �I��.
//----------------------------------------------------------------------------
void hidasp_close()
{
	if( reboot_done == 0 ) {
		unsigned char buf[BUFF_SIZE];
		memset(buf , 0xff, sizeof(buf) );
		hidWriteBuffer2(buf, PACKET_SIZE);
		Sleep(1);
	}
	RS_exit();
}

//----------------------------------------------------------------------------
//  ISP�R�}���h���s.
//----------------------------------------------------------------------------
int hidasp_cmd(const unsigned char cmd[4], unsigned char res[4])
{
#if	0
	char buf[128];
	int r;

	memset(buf , 0, sizeof(buf) );
	if (res != NULL) {
		buf[1] = HIDASP_CMD_TX_1;
	} else {
		buf[1] = HIDASP_CMD_TX;
	}
	memcpy(buf + 2, cmd, 4);

	r = hidWrite(hHID, buf, REPORT_LENGTH1 , REPORT_ID1);
#if DEBUG
	fprintf(stderr, "hidasp_cmd %02X, cmd: %02X %02X %02X %02X ",
		buf[1], cmd[0], cmd[1], cmd[2], cmd[3]);
#endif

	if (res != NULL) {
		r = hidRead(hHID, buf, REPORT_LENGTH1 , REPORT_ID1);
		memcpy(res, buf + 1, 4);
#if DEBUG
		fprintf(stderr, " --> res: %02X %02X %02X %02X\n",
				res[0], res[1], res[2], res[3]);
#endif
	}
#endif
	return 1;
}


void memdump(char *msg, char *buf, int len)
{
	int j;
	fprintf(stderr, "%s", msg);
	for (j = 0; j < len; j++) {
		fprintf(stderr, " %02x", buf[j] & 0xff);
		if((j & 0xf)==  7)
			fprintf(stderr, " ");

		if((j & 0xf)== 15)
			fprintf(stderr, "\n +");
	}
	fprintf(stderr, "\n");
}

int hidasp_list(char * string)
{
	return 0;
}
/*********************************************************************
 *
 *********************************************************************
 */
