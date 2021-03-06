/********************************************************************
 *	ユーザー定義関数の実行
 ********************************************************************
 *	cdctool.exeから、"user" コマンドを実行すると、以下の関数が呼び出される.
 *
 *	void user_cmd(int arg);
 *
 *	注意:
 *
 *	・ APPLICATION MODEでのみ、有効.
 *
 *	・ printfなどのstdio関数を呼び出すとコードサイズが大きくなる.
 *		_user_puts()などを使う限りFlashメモリー消費は少なく済む.
 *
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

#include "common.h"
#include "hidcmd.h"
#include "monit.h"
#include "usbio.h"
#include "pindef.h"
#include "gpio.h"

#if	APPLICATION_MODE
/********************************************************************
 *	user_cmd() 関数は、アプリケーションモードでのみ有効.
 ********************************************************************
 */

/********************************************************************
 *	１文字出力 ---> cdctool.exe のWindowsコンソールに表示.
 ********************************************************************
 */
int _user_putc(char c)
{
	USBputch(c);
	return 0;
}

/********************************************************************
 *	文字列出力 ---> cdctool.exe のWindowsコンソールに表示.
 ********************************************************************
 */
int _user_puts(char *s)
{
	while(*s) {
		_user_putc(*s++);
	}
	return 0;
}

/********************************************************************
 *	メモリーダンプ cdctool.exe のWindowsコンソールに表示.
 ********************************************************************
 */
void memdump(char *mesg,void *src , int len )
{
	char buf[16];
	uchar *s=(uchar*)src;
	int i;

	_user_puts(mesg);

	for(i=0; i<len; i++) {
		sprintf(buf,"%02x ",*s);
		s++;
		_user_puts(buf);
	}
	_user_puts("\n");
}
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
/********************************************************************
 *
 ********************************************************************
 */
#endif
