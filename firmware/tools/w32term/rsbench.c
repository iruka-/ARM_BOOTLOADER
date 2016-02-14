/*********************************************************************
 *	WIN32: RS232C �̐���.
 *********************************************************************
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <signal.h>
//#include <usb.h>
#include <time.h>


#include "w32term.h"
#include "rs232c.h"
#include "rsbench.h"
#include "util.h"

#define	KEY_ESCAPE_CH		0x01
#define	BENCH_SAFEMODE		0		//1�p�P�b�g�P�ʂŃG�R�[�o�b�N��҂�.

//#define	BENCH_LENGTH		80		//�x���`�}�[�N: �P�s���M�̒���.
#define	BENCH_LENGTH		128		//�x���`�}�[�N: �P�s���M�̒���.
//#define	BENCH_LENGTH		450		//�x���`�}�[�N: �P�s���M�̒���.
#define	BENCH_COUNT			250		//�x���`�}�[�N: �P�s���M�̉�.

extern char *upfile;	//�A�b�v���[�h�t�@�C����.
FILE *upload_fp=NULL;
int	  upload_UB=0;
int		opt_d;
char *upload_STRING="\rUB\r";

void Term_Log(int c);

int	RS_puts(uchar *buf,int len)
{
	RS_putdata(buf,len);
	return 0;
}
/*********************************************************************
 *	�R���\�[������
 *********************************************************************
 */
int	RS_keyInput(void)
{
	if(kbhit())	{
		return getch();
	}
	return -1;	//EOF
}

void Sleep_1()
{
	static int cnt=0;
	cnt++;
	if( cnt>=8) {
		cnt=0;
		Sleep(1);
	}
}
/*********************************************************************
 *	�t�@�C���A�b�v���[�h�@�\���܂ރR���\�[������
 *********************************************************************
 */
int	RS_keyInputUP(void)
{
	if(upload_UB) {
		int a = upload_STRING[upload_UB-1];
		Sleep(32);
		upload_UB++;
		if(a!=0) {
			return a;
		}
		upload_UB=0;
	}

	if(upload_fp) {	//�A�b�v���[�h��.
		int c = fgetc(upload_fp);	//�t�@�C�����當�����擾.
		if(c != EOF) {
//			Sleep_1();
			return c;
		}
		fclose(upload_fp);
		upload_fp = NULL;
	}
	return RS_keyInput();
}
/*********************************************************************
 *	�G�R�[�o�b�N�E�T�[�o�[.
 *********************************************************************
 *	CTRL+A �ŒE�o����.
 *
 */
void RS_echohost(int port_no,int baudrate)
{
	int c;
	unsigned char buf[16];
	RS_init(port_no,baudrate);			// 'COMx:' ���I�[�v������.
	printf("Entering EchobackMode: Escape=^%c\n", '@' +	KEY_ESCAPE_CH );

	while(1) {
		c = RS_keyInput();if(c == KEY_ESCAPE_CH) break;	//�E�o.
		if(	RS_checkdata() ) {
			RS_getdata(buf,1);
			c = buf[0];
			RS_putc(c);
			putchar(c);
			if(c == '\r') {
				c = '\n';
				RS_putc(c);
				putchar(c);
			}
		}
	}
	RS_exit();
}

void bench_tx()
{
	int i,c=' ';
	uchar buf[BENCH_LENGTH+16];
	for(i=0;i<BENCH_LENGTH;i++,c++) {
		buf[i]=c;
	}
	RS_puts(buf,BENCH_LENGTH);
}

int bench_rx(int record)
{
	int i,c=' ',r=0,errs=0;
	uchar buf[BENCH_LENGTH+1024];
	memset(buf,0,BENCH_LENGTH+1024);
	
	RS_getdata(buf,BENCH_LENGTH);
	for(i=0;i<BENCH_LENGTH;i++,c++) {
		if(opt_d) {
			printf(" %02x",r);
		}
		if(buf[i] != (c & 0xff) ) {
			errs++;
		}
	}
	if(errs) {
		printf("[%d]Recv Errors = %2d/%2d:\n", record,errs,BENCH_LENGTH);
		memdump(buf,BENCH_LENGTH,0);
	}

	if(opt_d) {
		printf("\n");
	}

//	putchar('+');	fflush(stdout);

	return errs;
}


#if	0
int bench_rx(void)
{
	int i,c=' ',r,errs=0;
	uchar buf[BENCH_LENGTH+1024];
	memset(buf,0,BENCH_LENGTH+1024);
	
	for(i=0;i<BENCH_LENGTH;i++,c++) {
		do {
			r = RS_getc();
		}while (r == RS_EMPTY);

		buf[i]=r;
		//memdump(buf+i,1,0);
		if(opt_d) {
			printf(" %02x",r);
		}

		if(r != (c & 0xff) ) {
			errs++;
		}
	}
	if(errs) {
		printf("Recv Errors = %2d:\n", errs);
		memdump(buf,BENCH_LENGTH,0);
	}

	if(opt_d) {
		printf("\n");
	}

	return errs;
}
#endif
#define	TIMEOUT	30000
#define	PRE_TX_CNT	0
/*********************************************************************
 *	
 *********************************************************************
 *	CTRL+A �ŒE�o����.
 *
 */
void RS_benchmark(int port_no,int baudrate)
{
	int c=0,i,rc,timeout=0;
	int count=BENCH_COUNT;
	int time1, rate;
	int total;


	RS_init(port_no,baudrate);			// 'COMx:' ���I�[�v������.
	printf("Entering Benchmark: Escape=^%c\n", '@' + KEY_ESCAPE_CH );
	int time0 = clock();
	int rxcnt=0;

	for(i=0;i<PRE_TX_CNT;i++) {
		bench_tx();
	}

	for(	;i<count;i++) {
		bench_tx();
		c = RS_keyInput();if(c == KEY_ESCAPE_CH) break;	//�E�o.
#if	BENCH_SAFEMODE			//1�p�P�b�g�P�ʂŃG�R�[�o�b�N��҂�.
		while(	RS_checkdata() < BENCH_LENGTH ) 
		{
		};
		bench_rx(rxcnt);rxcnt++;
#else
		while(	(rc = RS_checkdata()) >= BENCH_LENGTH ) {
			bench_rx(rxcnt);rxcnt++;
			//printf("(%d)",rc);
		}
#endif
		putchar('.');	fflush(stdout);
	}
	if(c != KEY_ESCAPE_CH) {
	while(rxcnt < count) {
		c = RS_keyInput();if(c == KEY_ESCAPE_CH) break;	//�E�o.
		while(	RS_checkdata() >= BENCH_LENGTH ) {
			bench_rx(rxcnt);rxcnt++;
		}
		timeout++;if(timeout>=TIMEOUT) {
			printf("\nBenchmark TIMEOUT. rxcnt %d < count %d ,RS_checkdata()=%d\n"
					,rxcnt,count,RS_checkdata()
			);
			goto escape1;
		}
	}
	}

escape1:
	printf("\nBenchmark end. ");
	time1 = clock() - time0;
	if (time1 == 0) {
		time1 = 2;
	}
	total = count * BENCH_LENGTH;
	rate = total * 1000 / time1;
   	printf("%d bytes/%d ms,  %d bytes/s (%d bps)\n",total,time1,rate,rate*10);

	RS_exit();
}
/*********************************************************************
 *	
 *********************************************************************
 *	CTRL+A �ŒE�o����.
 *
 */
void RS_terminal(int port_no,int baudrate)
{
	int c;
	unsigned char buf[16];

	if( upfile != NULL) {
		upload_fp = fopen(upfile,"rb");
	}

	RS_init(port_no,baudrate);			// 'COMx:' ���I�[�v������.
	printf("Entering TerminalMode: Escape=^%c\n", '@' +	KEY_ESCAPE_CH );
	signal(SIGINT,SIG_IGN);	// ^C �𖳌��ɂ���.


	if(IsOpt('U')) {
		upload_UB=1;
	}
	if(IsOpt('s')) {
		upload_STRING="\r\r";
	}

	while(1) {
		c = RS_keyInputUP();
		if(c>=0) {
			if((upload_fp==NULL) && (c == KEY_ESCAPE_CH)) break;	//�E�o.
			RS_putc(c);
		}
		if(	RS_checkdata() ) {
			RS_getdata(buf,1);
			Term_Log(buf[0]);
			putchar(buf[0]);
		}
	}
	signal(SIGINT,SIG_DFL);
	RS_exit();
}
/*********************************************************************
 *
 *********************************************************************
 */

