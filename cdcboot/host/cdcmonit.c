/****************************************************************************
 *	cdc monit
 ****************************************************************************
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rs232c.h"
#include "w32term.h"

char HELP_USAGE[]=
"* cdcmonit\n"
"Options\n"
"  -c<comNumber> ... Set COM Port Number. (1..99) \n"
"  -b<BaudRate>  ... Set Baud rate. \n"
"  -l<logfile>   ... Save RS232C log file. \n"
"  -u<uploadfile>... upload file.\n"
"  -p            ... Print COM-Port information.\n"

;



//
//	�R�}���h��͗p���[�N.
//
static  char  comXX[32];
static  char *com_port = "9";	// "COM9:" ���f�t�H���g.
static  int   baudrate = 9600;
char   *upfile = NULL;
int		opt_d=0;

//
//	�I�v�V�����������[�N.
//
static	FILE *log_fp = NULL;	//�[�����O���t�@�C���ɏ�������.


/*********************************************************************
 *	���� c ���󔒕����Ȃ�P��Ԃ�.
 *********************************************************************
 */
int is_space(int c)
{
	if((c==' ') || (c=='\t')) return 1;
	return 0;
}
/*********************************************************************
 *	������ *buf �̐擪�]����ǂݔ�΂�.
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
 *	�P�s����.�@�t�@�C���������̓R���\�[���̗��������e.
 *********************************************************************
 */
void Term_Log(int c)
{
	if( log_fp != NULL) {
		putc(c,log_fp);
	}
}
/*********************************************************************
 *	�I�v�V�����ݒ�
 *********************************************************************
 */
void optionCheck()
{
	if(IsOpt('h')||IsOpt('?')||IsOpt('/')||IsOpt('-')) {
		usage();exit(0);
	}
	if(IsOpt('l')) {
		log_fp = fopen( Opt('l') , "wb" );
	}
	if(IsOpt('u')) {
		upfile = sp_skip(Opt('u'));		// upload file.
	}
	if(IsOpt('b')) {
		sscanf(Opt('b'),"%d",&baudrate); // Get BaudRate
		printf("%d\n",baudrate);
	}
	if(IsOpt('d')) {
		opt_d=1;
	}
	if(IsOpt('c')) {
		com_port = Opt('c');			// COM Port No.
	}else{
		com_port = comXX;
		sprintf(comXX,"%d",RS_printinfo(0));
		printf("Open COM%s:\n",comXX);
	}
}
/*********************************************************************
 *	���C��
 *********************************************************************
 */
int main(int argc,char **argv)
{
	//�I�v�V�������.
	Getopt(argc,argv,"bl");
	optionCheck();
	if(IsOpt('p')) {
		RS_printinfo(1);
		return 0;
	}
	
	RS_terminal(com_port,baudrate);

	if(log_fp!=NULL) fclose(log_fp);
	return 0;
}
/*********************************************************************
 *
 *********************************************************************
 */

