/* Support files for GNU libc.  Files in the system namespace go here.
   Files in the C namespace (ie those that do not start with an
   underscore) go in .c.  */

#include <stdio.h>

#if	0
#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <reent.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#endif

/*****************************************************************************
 *	___trap34()�֐��o�R�� syscall ���󂯎����s����.
 *****************************************************************************
    ___trap34() �̓f�t�H���g�ł� trapa #34 �𔭍s���Ă���B
    �����ύX���āAsh2fpint.S���ŁA���L�̃n���h���[�ɕ��򂷂�悤�ɂ����B
 */
int		trap_f00	(){return 0;}
int		trap_exit	(){return 0;}
int		trap_fork	(){return 0;}
int		trap_read	(){return 0;}
//int		trap_write	(){return 0;}
//int		trap_open	(){return 0;}
//int		trap_close	(){return 0;}
int		trap_wait4	(){return 0;}
int		trap_creat	(){return 0;}
int		trap_link	(){return 0;}
int		trap_unlink	(){return 0;}
int		trap_execv	(){return 0;}
int		trap_chdir	(){return 0;}
int		trap_mknod	(){return 0;}
int		trap_chmod	(){return 0;}
int		trap_chown	(){return 0;}
//int		trap_lseek	(){return 0;}
int		trap_getpid	(){return 0;}
//int		trap_isatty (){return 0;}
int		trap_fstat  (){return 0;}
int		trap_time 	(){return 0;}


int trap_isatty (int fd)
{
	return (fd <= 2) ? 1 : 0;  /* one of stdin, stdout, stderr */
}

int trap_open (const char * path,int  flags,...)
{
	return 2;	//������.
}

int trap_close (int file)
{
	return 0;
}

int trap_lseek (int file,int ptr,int dir)
{
	return 0;
}

int trap_write (int file,char * ptr,int    len)
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

