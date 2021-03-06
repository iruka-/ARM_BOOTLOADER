/********************************************************************
 *	ユーザー定義関数の実行
 ********************************************************************
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include "hidcmd.h"
#include "monit.h"
#include "usbio.h"

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

	for(i=0;i<len;i++) {
		sprintf(buf,"%02x ",*s);s++;
		_user_puts(buf);
	}
	_user_puts("\n");
}


void wait_1sec(void)
{
	wait_ms(1000);
}

void _user_puthex1(int h1)
{
	int c = h1 & 0x0f;
	if(c>=10) c += ('a' - ('9'+1));
	_user_putc('0'+c);
}

void print_sec(int j)
{
	_user_puthex1(j / 10);
	_user_puthex1(j % 10);
	_user_putc('\n');
}
/********************************************************************
 *	_user_putc()  _user_puts()テストの実行.
 ********************************************************************
 */
void user_cmd(int arg)
{
	int j;
	_user_puts("Hello World\n");
	for(j=0;j<60;j++) {
		print_sec(j);
		wait_1sec();
	}
}


/********************************************************************
 *	
 ********************************************************************
 */
