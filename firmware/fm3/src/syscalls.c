/********************************************************************
 *	printfやputchar関数の出力先を ユーザー実装の_user_putc() にする。
 ********************************************************************
 */

#define	EXTERN	extern char
#define	END_RAM	(char *)0x2000f000

int _user_putc(char c);

//	open()したファイルハンドルは、コンソール(tty)であるかどうか知る.
//	ここでは fd <= 2 のときは tty であることを通知する.
int _isatty (int fd)
{
	return (fd <= 2) ? 1 : 0;  /* one of stdin, stdout, stderr */
}

//	open()は、常に 2 (stdout) を返す. (printf専用)
int _open (const char * path,int  flags,...)
{
	return 2;	//未実装.
}

//	close()は、常に成功する.
int _close (int file)
{
	return 0;
}

//	lseek()は何もしない.
int _lseek (int file,int ptr,int dir)
{
	return 0;
}

//	write()は、printfの出力を受け取り、ユーザー実装の１文字出力を呼ぶ.
int _write (int file,char * ptr,int    len)
{
	//仮実装.
	if(file<3) {
		while(len) {
			_user_putc(*ptr++);
			len--;
		}
	}
	return len;	
}

//	read()は何もしない.
int _read (int file,char * ptr,int    len)
{
	return 0;
}

//	fstatは何もしない.
int _fstat (int file,char * ptr,int    len)
{
	return 0;
}


EXTERN	_ebss	;/* end address for the .bss section.     */

static char *sbrk_ptr = (char *) &_ebss;

//	malloc()から呼び出される可能性がある.
char * _sbrk(int size)
{
    char *ret;
    if(sbrk_ptr + size > END_RAM ) {
		return (char *) (-1);	// メモリーがない.
    }
    else {
        ret = sbrk_ptr;
        sbrk_ptr += size;		// sbrk_ptrを進める.
    }
    return ret;
}

/********************************************************************
 *
 ********************************************************************
 */
