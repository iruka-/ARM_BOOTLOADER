/********************************************************************
 *	ユーザー定義関数の実行
 ********************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

#include "hidcmd.h"
#include "monit.h"
#include "usbio.h"


#define	PRINTF_TEST	(1)		// printf()の動作テスト.


int _user_putc(char c)
{
	USBputch(c);
	return 0;
}


int _user_puts(char *s)
{
	while(*s) {
		_user_putc(*s++);
	}
	return 0;
}




#if	BOOTLOADER_MODE
void user_cmd(int arg)	{}
#else
	//APPLICATION_MODE
//	#undef	putc
//	#undef	fputs
//	#define	putc(c,fp)  _user_putc(c)
//	#define	fputs(s,fp) _user_puts(s)


/********************************************************************
 *	定義
 ********************************************************************
 */
/********************************************************************
 *	データ
 ********************************************************************
 */
/********************************************************************
 *
 ********************************************************************
 */
void _user_puthex1(int h1)
{
	int c = h1 & 0x0f;
	if(c>=10) c += ('a' - ('9'+1));
	_user_putc('0'+c);
}

void _user_puthex(int h,int digit)
{
	int i;
	for(i=digit-1;i>=0;i--) {
		_user_puthex1(h >> (i*4));
	}
}

/********************************************************************
 *
 ********************************************************************
 */
void memdump(char *mesg,void *src , int len )
{
	char buf[16];
	uchar *s=(uchar*)src;
	int i;

	_user_puts(mesg);

	for(i=0; i<len; i++) {
		//printf(buf,"%02x ",*s++);

		_user_puthex(*s++,2);
		_user_puts(" ");
	}
	_user_puts("\n");
}


#if	PRINTF_TEST		// printf()の動作テスト.

#define	FLOAT double

	FLOAT a = 3.141592653589793238;
	FLOAT b = 2.0;
	FLOAT c;
	FLOAT d = 128.0;
	double n=1.0;

	float q=2;


#ifdef	USE_FPU
extern	int	__fpscr_values[2];
#endif
/********************************************************************
 *
 ********************************************************************
 */
void user_cmd(int arg)
{
	char buf[256];

//	set_fpscr(0x1c0001);
#ifdef	USE_FPU
	memdump( "__fpscr_values" , __fpscr_values , 16 );
#endif

#if	1
	//
	//	malloc()テストの実行.
	//
	int *t;
	t=malloc(1);
//	_user_puthex(t,8);
	sprintf(buf,"t=%x\n",(int) t );
	fputs(buf,stdout);

	t=malloc(16);
//	_user_puthex(t,8);
	sprintf(buf,"t=%x\n",(int) t );
	fputs(buf,stdout);

	t=malloc(16);
//	_user_puthex(t,8);
	sprintf(buf,"t=%x\n",(int) t );
	fputs(buf,stdout);
	_user_putc('\n');
#endif

	//
	//	printf()テストの実行.
	//
	printf("Hello World\n");
#if	1
	//
	//	fputs()テストの実行.
	//
	ushort i,j;
	for(j=0; j<16; j++) {
		fputs("Hello :",stdout);
		for(i=0; i<50; i++) {
			putc( i + ' ' ,stdout );
		}
		fputs("\n",stdout);
	}
#endif

	int nan = isnan(n);
	sprintf(buf,"nan=%x\n",nan );
	fputs(buf,stdout);

#if	1
	printf("isnan(n)=%d\n",isnan(n));
	printf("isnanf(n)=%d\n",isnanf(q));

#endif

//	int decpt;
//	ecvt(3.14,16,&decpt,buf);
//	fputs(buf,stdout);

	memdump( "a=" , &a , 8 );
	c=a*b;
	memdump( "b=" , &b , 8 );
	//
	//	double 型のメモリーダンプテスト.
	//
	c = a*b;
	d = d * q;

	memdump( "c=" , &c , 8 );
	memdump( "d=" , &d , 8 );

#if	1
	printf("a=%16.10g\n",a);
	printf("b=%16.10g\n",b);
	printf("c=%16.10g\n",c);
	printf("d=%16.10g\n",d);

	printf("e %e\n" , a);
	printf("f %f\n" , a);
	printf("g %g\n" , a);
#endif
}

#else	//PRINTF_TEST	printf()の動作テスト.


/********************************************************************
 *	_user_putc()  _user_puts()テストの実行.
 ********************************************************************
 */
void user_cmd(int arg)
{
	_user_puts("Hello World\n");
	ushort i,j;
	for(j=0; j<16; j++) {
		_user_puts("Hello :");
		for(i=0; i<50; i++) {
			_user_putc( i + ' ' );
		}
		_user_puts("\n");
	}
}


#endif	//PRINTF_TEST	printf()の動作テスト.
/********************************************************************
 *
 ********************************************************************
 */
#endif
