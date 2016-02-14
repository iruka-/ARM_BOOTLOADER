/****************************************************************************
 *	w32term.c  :  W32term   WIN32専用ターミナル.
 ****************************************************************************
 *
 *  これは、任意の COMポート に接続し、データの送受信を行なうフレームワーク.
 *
 *	機能:
 *		w32term     無手順端末として起動する.
 *		w32term -e  エコーバックサーバーとして起動する.
 *		w32term -t  ベンチマークテスト.
 *
 *	オプション:
"  -c<comNumber> ... Set COM Port Number.\n"
"  -b<BaudRate>  ... Set Baud rate.\n"
"  -l<logfile>   ... Save RS232C log file.\n"
"  -u<uploadfile>... upload file.\n"
 *
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rs232c.h"
#include "w32term.h"
#include "rsbench.h"

char HELP_USAGE[]=
"* w32term     無手順端末として起動する.\n"
"* w32term -e  エコーバックサーバーとして起動する.\n"
"* w32term -p  接続されているCOMポートの一覧.\n"
"* w32term -t  ベンチマークテスト.\n"
"Options\n"
"  -c<comNumber> ... Set COM Port Number. (1..99) \n"
"  -b<BaudRate>  ... Set Baud rate. \n"
"  -l<logfile>   ... Save RS232C log file. \n"
"  -u<uploadfile>... upload file.\n"
"  -U<uploadfile>... upload file after 'UB\\n'.\n"
"  -p            ... Print COM-Port information.\n"

;

//
//	コマンド解析用ワーク.
//
static  char *com_port = NULL;
//static  int   baudrate = BAUD_RATE;

char   *upfile = NULL;
int		opt_d=0;

//
//	オプション処理ワーク.
//
static	FILE *log_fp = NULL;	//端末ログをファイルに書き込む.


/*********************************************************************
 *	文字 c が空白文字なら１を返す.
 *********************************************************************
 */
int is_space(int c)
{
	if((c==' ') || (c=='\t')) return 1;
	return 0;
}
/*********************************************************************
 *	文字列 *buf の先頭余白を読み飛ばす.
 *********************************************************************
 */
char *sp_skip(char *buf)
{
	while(*buf) {
		if(is_space(*buf)==0) return buf;
		buf++;
	}
	return buf;
}
/*********************************************************************
 *	
 *********************************************************************
 */
void usage(void)
{
	printf( HELP_USAGE );
}

/*********************************************************************
 *	１行入力.　ファイルもしくはコンソールの両方を許容.
 *********************************************************************
 */
void Term_Log(int c)
{
	if( log_fp != NULL) {
		putc(c,log_fp);
	}
}
/*********************************************************************
 *	メイン
 *********************************************************************
 */
int main(int argc,char **argv)
{
	int com_n = 0;
	int baudrate= BAUD_RATE;
	int portno  = 0;

	//オプション解析.
	Getopt(argc,argv,"bl");
	if(IsOpt('h')||IsOpt('?')||IsOpt('/')||IsOpt('-')) {
		usage();exit(0);
	}
	if(IsOpt('l')) {
		log_fp = fopen( Opt('l') , "wb" );
	}
	if(IsOpt('u')) {
		upfile = sp_skip(Opt('u'));		// upload file.
	}
	if(IsOpt('U')) {
		upfile = sp_skip(Opt('U'));		// upload file.
	}
	if(IsOpt('c')) {
		com_port = Opt('c');			// COM Port No.
		sscanf(com_port,"%d",&portno);
	}
	if(IsOpt('p')) {
		com_n = RS_printinfoAll(portno,baudrate,1,1);
		return 0;
	}
	if(IsOpt('b')) {
		sscanf(Opt('b'),"%d",&baudrate); // Get BaudRate
		printf("%d\n",baudrate);
	}
	if(IsOpt('d')) {
		opt_d=1;
	}
	
	if( portno == 0 ) {
		com_n = RS_printinfo(portno,baudrate,1);
		portno = com_n;
	}
	
	if(IsOpt('e')) {RS_echohost( portno,baudrate);}else 
	if(IsOpt('t')) {RS_benchmark(portno,baudrate);}else 
	{				RS_terminal( portno,baudrate);}

	if(log_fp!=NULL) fclose(log_fp);
	return 0;
}
/*********************************************************************
 *
 *********************************************************************
 */

