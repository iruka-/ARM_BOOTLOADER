/******************************************************************************
	�ȈՃ��C�u����(�R���\�[�����o�͏���)�֐� �w�b�_
******************************************************************************/

#ifndef _TINYLIB_H_
#define _TINYLIB_H_

#include <stdarg.h>

/* �e��o�b�t�@�T�C�Y */
#define MAX_GETSSIZE (255)
#define MAX_PRINTFSIZE (255)
#define MAX_SPRINTFSIZE (1024)


/* string�֘A */
extern int t_strlen(char *s);
extern int t_strcmp(char *s, char *d);
extern char *t_strncpy(char *d, char *s, int n);
extern int t_memcmp(char *s, char *d, int n);
extern char *t_memset(char *adr, char c, int n);

/* atoi */
int t_atoi(char *p);

/* printf�֘A */
extern int t_printf(char *format, ...);
extern int t_sprintf(char *buffer, char *format, ...);
extern int t_vsprintf(char *buffer, char *format, va_list argptr);

/* get/put�֘A */
extern int  t_kbhit(void);
extern int  t_getch(void);
extern char *t_gets(char *buffer);
extern int  t_putch(int c);
extern int  t_puts(char *str);

#endif
