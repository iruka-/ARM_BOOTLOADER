/********************************************************************
 *	printf��putchar�֐��̏o�͐�� ���[�U�[������_user_putc() �ɂ���B
 ********************************************************************
 */

#define	EXTERN	extern char
#define	END_RAM	(char *)0x2000f000

int _user_putc(char c);

//	open()�����t�@�C���n���h���́A�R���\�[��(tty)�ł��邩�ǂ����m��.
//	�����ł� fd <= 2 �̂Ƃ��� tty �ł��邱�Ƃ�ʒm����.
int _isatty (int fd)
{
	return (fd <= 2) ? 1 : 0;  /* one of stdin, stdout, stderr */
}

//	open()�́A��� 2 (stdout) ��Ԃ�. (printf��p)
int _open (const char * path,int  flags,...)
{
	return 2;	//������.
}

//	close()�́A��ɐ�������.
int _close (int file)
{
	return 0;
}

//	lseek()�͉������Ȃ�.
int _lseek (int file,int ptr,int dir)
{
	return 0;
}

//	write()�́Aprintf�̏o�͂��󂯎��A���[�U�[�����̂P�����o�͂��Ă�.
int _write (int file,char * ptr,int    len)
{
	//������.
	if(file<3) {
		while(len) {
			_user_putc(*ptr++);
			len--;
		}
	}
	return len;	
}

//	read()�͉������Ȃ�.
int _read (int file,char * ptr,int    len)
{
	return 0;
}

//	fstat�͉������Ȃ�.
int _fstat (int file,char * ptr,int    len)
{
	return 0;
}


EXTERN	_ebss	;/* end address for the .bss section.     */

static char *sbrk_ptr = (char *) &_ebss;

//	malloc()����Ăяo�����\��������.
char * _sbrk(int size)
{
    char *ret;
    if(sbrk_ptr + size > END_RAM ) {
		return (char *) (-1);	// �������[���Ȃ�.
    }
    else {
        ret = sbrk_ptr;
        sbrk_ptr += size;		// sbrk_ptr��i�߂�.
    }
    return ret;
}

/********************************************************************
 *
 ********************************************************************
 */
