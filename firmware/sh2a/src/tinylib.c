/******************************************************************************
	�ȈՃ��C�u����(�R���\�[�����o�͏���)�֐�
******************************************************************************/

#include <stdarg.h>

/* CPU UART���o�̓w�b�_ */
//#include "cpu_uart.h"

/* �ȈՃ��C�u�����w�b�_ */
#include "tinylib.h"

int _t_vsnprintf(char *buffer, unsigned long count, char *format, va_list argptr);


/* ----------------------------------------------------------------------- */

/* strlen */
int t_strlen(char *s)
{
  int l;
  l = 0;
  while(*s++) l++;
  return l;
}

/* strcmp */
int t_strcmp(char *s, char *d)
{
  int i;
  while(*s == *d) {
    if(!*s) return 0;
    s++;
    d++;
  }
  return *s - *d;
}

/* strncpy */
char *t_strncpy(char *d, char *s, int n)
{
	int i;
	char *sd;
	sd=d;
	for(i = 0; i < n; i++) {
		*d=*s;
		d++;
		s++;
	}
	return sd;
}

/* memcmp */
int t_memcmp(char *s, char *d, int n)
{
  int i;
  for(i = 0; i < n; i++) {
    if(*s != *d) return *s - *d;
    s++;
    d++;
  }
  return 0;
}

/* memset */
char *t_memset(char *adr, char c, int n)
{
  int i;
  for(i = 0; i < n; i++) *adr++ = c;
  return adr;
}



/* ----------------------------------------------------------------------- */

/* �Ȉ�printf�֐� */
int t_printf(char *format, ...)
{
	va_list argptr;
	int i;
	char buffer[MAX_PRINTFSIZE + 1];
	va_start(argptr, format);
	i=_t_vsnprintf(buffer, MAX_PRINTFSIZE, format, argptr);
	va_end(argptr);

	CPU_UartPutsL(buffer, i);	/* CPU UART�\�� */

	return i;
}

/* sprintf�֐� */
int t_sprintf(char *buffer, char *format, ...)
{
	va_list argptr;
	int i;
	va_start(argptr, format);
	i=t_vsprintf(buffer, format, argptr);
	va_end(argptr);
	return i;
}

/* vsprintf�֐� */
int t_vsprintf(char *buffer, char *format, va_list argptr)
{
	return _t_vsnprintf(buffer, MAX_SPRINTFSIZE, format, argptr);
}

/* putch() */
int t_putch(int c)
{
	CPU_UartPutch(c);	/* CPU UART�\�� */
	return c;
}

/* puts() */
int t_puts(char *str)
{
	CPU_UartPuts(str);		/* CPU UART�\�� */
	return 0;
}



/* ----------------------------------------------------------------------- */
/* input�n */
int t_kbhit(void)
{
	int result;
	result=CPU_UartRecvCheck();
	return result;
}

int t_getch(void)
{
	int c;
	while(1) {	/* �R���\�[�����͑҂� */
		if (CPU_UartRecvCheck()) {
			c = (int)CPU_UartGetch();
			break;
		}
	}
	return c;
}

/* �G�R�[�o�b�N�L��̕�������� */
char *t_gets(char *buffer)
{
  int bufptr;
  int c;
  char *p;
  bufptr = 0; /* ������ */
  p = buffer;
  
  while(1) {
    /* �R���\�[�����͑҂� */
    /* SERIAL/KB�ǂ�����Ȃ��ƁA�����ŉi�v���[�v�ɂȂ� */
    while(!t_kbhit());
    c = t_getch();
    
    /* t_debug("%02X ", c); */
    
    if(c == 0x0d) { /* ���s�ŏI�� */
      *p = 0;
      t_putch(0x0d);
      break;
    }
    
    /* BS / DEL / ���R���\�[�����͂ɂ����̓o�b�t�@�폜���� */
    if((c == 0x08)||(c == 0x7f)||(c == 0x1d)) {
      bufptr--;
      if(bufptr >= 0){
        *--p = 0;
      t_putch(0x08);
      } else {
        bufptr = 0;
      }
    } else {
      if(bufptr < MAX_GETSSIZE)
      t_putch(c);
      *p++ = c;
      bufptr++;
    }
  } /* while(1) */
  
/* t_debug("t_gets() : len = %d\n", bufptr); */
  
  return buffer;
}



/* ----------------------------------------------------------------------- */
/* printf������͕� */

#define FC_MINUS (1)
#define FC_PLUS  (2)
#define FC_ZERO  (4)
#define FC_BLANK (8)
#define FC_SHARP (16)

/* �������ǂ��� */
static int _isdigit(char c)
{
  return ((c>='0')&&(c<='9'));
}

/* ���ӂꂽ��m��Ȃ� */
int _atoi_ptr(char **ptr)
{
  long l;
  int minus;
  char *p;
  p = *ptr;
  
  l = 0L;
  minus = 0;
  if(*p == '-') {
    minus = 1;
    p++;
  }
  
  while(_isdigit(*p)){
    l *= 10;
    l += (*p-'0');
    p++;
  }
  if(minus) l = -l;
  *ptr = p; /* �����߂� */
  /* printf("debug: %ld, %s\n", l, p); */
  return (int)l;
}

/* atoi */
int t_atoi(char *p)
{
	return _atoi_ptr(&p);
}

/* ���l���當����ɕϊ�(�������t��) */
int ntorstru(char *buf, unsigned long num, int base)
{
  unsigned long l, m;
  char *b0;
  char *cvt;
  char *cvt0 = "0123456789abcdef";
  char *cvt1 = "0123456789ABCDEF";
  
  b0 = buf; /* �ۑ� */
  cvt = cvt0;
  
  if(base<0) {
    cvt= cvt1;
    base = -base;
  }
  
  l = num;
  m = 0;
  
  if(l==0) *buf++ = '0';
  while(l) {
    m = l % base;
    l /= base;
    *buf++ = cvt[(int)m];
  }
  *buf = '\0';
  return buf-b0;
}


int _t_vsnprintf(char *buffer, unsigned long count, char *format, va_list argptr)
/*
  buffer
    �o�͂̊i�[�ʒu
  count
    �������ލő啶����
  format
    �����̎w��
  argptr
    �������X�g�ւ̃|�C���^
*/
{
  char *ptr, *eob;
  unsigned flags;       /* format control : flag characters */
  int ef;
  char *pf;
  int width;
  int precision;
  int longf;
  int len, len2;
  char *s;
  int i;
  char numbuf[30];      /* long�ōŒ��łǂꂭ�炢�H */
  int symbol, minus;
  long v;
  unsigned long uv;
  
  ptr = buffer;         /* current output pointer */
  eob = (char *)(ptr + count -1); /* end of buffer */
  
  pf = format;
  while(*pf){
    if(ptr >= eob) break; /* �o�b�t�@�͂ݏo���c */
    
    if(*pf != '%') {
      *ptr++ = *pf++;   /* %����Ȃ��Ƃ���͂��̂܂܏o�� */
      continue;
    }
    
    pf++; /* skip '%' */
    
    /* correct flags */
    flags = 0;
    ef = 0;
    while(!ef){
      switch(*pf){
        case '-': 
          flags |= FC_MINUS;
          break;
        case '+': 
          flags |= FC_PLUS;
          break;
        case '0': 
          flags |= FC_ZERO;
          break;
        case ' ': 
          flags |= FC_BLANK;
          break;
/*        case '#': ��������
          flags |= FC_SHARP;
          break;
*/
        default:
          ef = 1;
          break;
      }
      if(!ef) pf++;
    }
    
    /* width */
    width = -1; /* none */
    if(_isdigit(*pf)) width = _atoi_ptr(&pf);
    
    /* printf("debug: %s\n", pf); */
    
    precision = -1; /* none */
    if(*pf == '.') {
      pf++;
      precision = _atoi_ptr(&pf);
    }
    
    /* size flag */
    longf = 0;
    if(*pf == 'l') {
      pf++;
      longf = 1;
    }
    
    /* output */
    switch(*pf++) {
      case '%':
        if(ptr < eob) *ptr++ = '%';
        continue;
        /* break; */
      
      case 'c': /* ���w��͖����c���Ă�������� */
        if(ptr < eob) *ptr++ = (char)va_arg(argptr, int);
        continue;
        /* break; */
      
      case 's':
        /* printf("debug: s\n"); */
        s = va_arg(argptr, char *);
        len = t_strlen(s);
        /* width.precision �ɂ�镝���� */
        if((precision != -1)&&(len > precision)) len = precision;
        if(width == -1) width = len;
        if(width < len) width = len; /* �S���o�͂��� */
        
        if(!(flags & FC_MINUS)) { /* �E�l�߂̏ꍇ */
          for(i = 0; i < width - len; i++) {
            if(ptr < eob) *ptr++ = ' ';
          }
        }
        for(i = 0; i < len; i++) *ptr++ = *s++; /* �����񕔕����o�� */
        if((flags & FC_MINUS)) { /* ���l�߂̏ꍇ */
          for(i = 0; i < width - len; i++) {
            if(ptr < eob) *ptr++ = ' ';
          }
        }
        continue;
        /*break;*/
      
      
      case 'd':
      case 'i':
        if(longf) {
          v = (long)va_arg(argptr, long);
        } else {
          v = (int)va_arg(argptr, int);
        }
        minus = (v < 0)?1:0;
        if(minus) v = -v;
        len = ntorstru(numbuf, v, 10);
        break;
      
      case 'u':
        if(longf) {
          uv = (unsigned long)va_arg(argptr, unsigned long);
        } else {
          uv = (unsigned int)va_arg(argptr, unsigned int);
        }
        len = ntorstru(numbuf, uv, 10);
        minus = 0;
        break;
      
      case 'o':
        if(longf) {
          uv = (unsigned long)va_arg(argptr, unsigned long);
        } else {
          uv = (unsigned int)va_arg(argptr, unsigned int);
        }
        len = ntorstru(numbuf, uv, 8);
        minus = 0;
        break;
      
      case 'x':
        if(longf) {
          uv = (unsigned long)va_arg(argptr, unsigned long);
        } else {
          uv = (unsigned int)va_arg(argptr, unsigned int);
        }
        len = ntorstru(numbuf, uv, 16);
        minus = 0;
        break;
      
      case 'X':
        if(longf) {
          uv = (unsigned long)va_arg(argptr, unsigned long);
        } else {
          uv = (unsigned int)va_arg(argptr, unsigned int);
        }
        len = ntorstru(numbuf, uv, -16); /* base<0��a-f���啶��*/
        minus = 0;
        break;
      
      default:
        /* �t���O�Ɖ��߂��ꂽ�͖̂߂�Ȃ����c */
        if(ptr < eob) *ptr++ = '%';
        if(ptr < eob) *ptr++ = *(pf-1);
        continue;
        /*break;*/
    }
    
    /* �ȉ��͐��l�̂� */
    
    /* i = va_arg(argptr, int); */
    /* len = ntorstr(numbuf, i, 10); */
    
    len2 = len; /* ���������܂߂��o�͂ɕK�v�Ȓ��� */
    symbol = 0; /* �������� */
    if(minus) {
      symbol = '-';
    } else {
      if(flags & FC_BLANK) symbol = ' ';
      if(flags & FC_PLUS)  symbol = '+'; /* ' '���㏑������ */
    }
    if(symbol) len2++; 
    
    if(width < precision) width = precision; /* width.precision ��ʂ��ĂȂ��c����ł����̂��H */
    if(width < len2) width = len2;           /* �Œ���K�v�ȕ��Ɋg��(�w��Ȃ��ꍇ���܂�) */
    
    if(!(flags & FC_MINUS)) { /* ���񂹂łȂ��ꍇ */
      if(flags & FC_ZERO) { /* ZERO-FILL */
        if(symbol) {
          if(ptr < eob) *ptr++ = symbol;              /* ���������Ă���c */
        }
        for(i=0;i<width-len2;i++) {
          if(ptr < eob) *ptr++ = '0';  /* ���Ԃ�0�Ŗ��߂� */
        }
      } else {                 /* ���񂹂̏ꍇ */
        for(i=0;i<width-len2;i++) {
          if(ptr < eob) *ptr++ = ' ';  /* ���Ԃ��󔒂Ŗ��߂Ă��� */
        }
        if(symbol) {
          if(ptr < eob) *ptr++ = symbol;              /* ���������� */
        }
      }
    } else {
      if(symbol) {
        if(ptr < eob) *ptr++ = symbol;                /* �K�v�Ȃ畄�����o�� */
      }
    }
    for(i = len - 1; i >= 0; i--) {
      if(ptr < eob) *ptr++ = numbuf[i]; /* (�t���̕������)�o�͂��� */
    }
    if(flags & FC_MINUS) { /* ���񂹂ŕ��w�肪����΋󔒂��o�͂��� */
      for(i = 0; i < width - len2; i++) {
        if(ptr < eob) *ptr++ = ' ';
      }
    }
    
    /* continue; */
  } /* while(*pf) */
  *ptr = '\0';
  return (ptr - buffer); /* �o�͂��������� */
}

