/******************************************************************************
	簡易ライブラリ(コンソール入出力処理)関数
******************************************************************************/

#include <stdarg.h>

/* CPU UART入出力ヘッダ */
//#include "cpu_uart.h"

/* 簡易ライブラリヘッダ */
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

/* 簡易printf関数 */
int t_printf(char *format, ...)
{
	va_list argptr;
	int i;
	char buffer[MAX_PRINTFSIZE + 1];
	va_start(argptr, format);
	i=_t_vsnprintf(buffer, MAX_PRINTFSIZE, format, argptr);
	va_end(argptr);

	CPU_UartPutsL(buffer, i);	/* CPU UART表示 */

	return i;
}

/* sprintf関数 */
int t_sprintf(char *buffer, char *format, ...)
{
	va_list argptr;
	int i;
	va_start(argptr, format);
	i=t_vsprintf(buffer, format, argptr);
	va_end(argptr);
	return i;
}

/* vsprintf関数 */
int t_vsprintf(char *buffer, char *format, va_list argptr)
{
	return _t_vsnprintf(buffer, MAX_SPRINTFSIZE, format, argptr);
}

/* putch() */
int t_putch(int c)
{
	CPU_UartPutch(c);	/* CPU UART表示 */
	return c;
}

/* puts() */
int t_puts(char *str)
{
	CPU_UartPuts(str);		/* CPU UART表示 */
	return 0;
}



/* ----------------------------------------------------------------------- */
/* input系 */
int t_kbhit(void)
{
	int result;
	result=CPU_UartRecvCheck();
	return result;
}

int t_getch(void)
{
	int c;
	while(1) {	/* コンソール入力待ち */
		if (CPU_UartRecvCheck()) {
			c = (int)CPU_UartGetch();
			break;
		}
	}
	return c;
}

/* エコーバック有りの文字列入力 */
char *t_gets(char *buffer)
{
  int bufptr;
  int c;
  char *p;
  bufptr = 0; /* 文字数 */
  p = buffer;
  
  while(1) {
    /* コンソール入力待ち */
    /* SERIAL/KBどちらもないと、ここで永久ループになる */
    while(!t_kbhit());
    c = t_getch();
    
    /* t_debug("%02X ", c); */
    
    if(c == 0x0d) { /* 改行で終了 */
      *p = 0;
      t_putch(0x0d);
      break;
    }
    
    /* BS / DEL / ←コンソール入力による入力バッファ削除処理 */
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
/* printf書式解析部 */

#define FC_MINUS (1)
#define FC_PLUS  (2)
#define FC_ZERO  (4)
#define FC_BLANK (8)
#define FC_SHARP (16)

/* 数字かどうか */
static int _isdigit(char c)
{
  return ((c>='0')&&(c<='9'));
}

/* あふれたら知らない */
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
  *ptr = p; /* 書き戻し */
  /* printf("debug: %ld, %s\n", l, p); */
  return (int)l;
}

/* atoi */
int t_atoi(char *p)
{
	return _atoi_ptr(&p);
}

/* 数値から文字列に変換(ただし逆順) */
int ntorstru(char *buf, unsigned long num, int base)
{
  unsigned long l, m;
  char *b0;
  char *cvt;
  char *cvt0 = "0123456789abcdef";
  char *cvt1 = "0123456789ABCDEF";
  
  b0 = buf; /* 保存 */
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
    出力の格納位置
  count
    書き込む最大文字数
  format
    書式の指定
  argptr
    引数リストへのポインタ
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
  char numbuf[30];      /* longで最長でどれくらい？ */
  int symbol, minus;
  long v;
  unsigned long uv;
  
  ptr = buffer;         /* current output pointer */
  eob = (char *)(ptr + count -1); /* end of buffer */
  
  pf = format;
  while(*pf){
    if(ptr >= eob) break; /* バッファはみ出す… */
    
    if(*pf != '%') {
      *ptr++ = *pf++;   /* %じゃないところはそのまま出力 */
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
/*        case '#': 実装せず
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
      
      case 'c': /* 幅指定は無視…してもいいよね */
        if(ptr < eob) *ptr++ = (char)va_arg(argptr, int);
        continue;
        /* break; */
      
      case 's':
        /* printf("debug: s\n"); */
        s = va_arg(argptr, char *);
        len = t_strlen(s);
        /* width.precision による幅調整 */
        if((precision != -1)&&(len > precision)) len = precision;
        if(width == -1) width = len;
        if(width < len) width = len; /* 全部出力する */
        
        if(!(flags & FC_MINUS)) { /* 右詰めの場合 */
          for(i = 0; i < width - len; i++) {
            if(ptr < eob) *ptr++ = ' ';
          }
        }
        for(i = 0; i < len; i++) *ptr++ = *s++; /* 文字列部分を出力 */
        if((flags & FC_MINUS)) { /* 左詰めの場合 */
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
        len = ntorstru(numbuf, uv, -16); /* base<0はa-fが大文字*/
        minus = 0;
        break;
      
      default:
        /* フラグと解釈されたのは戻らないが… */
        if(ptr < eob) *ptr++ = '%';
        if(ptr < eob) *ptr++ = *(pf-1);
        continue;
        /*break;*/
    }
    
    /* 以下は数値のみ */
    
    /* i = va_arg(argptr, int); */
    /* len = ntorstr(numbuf, i, 10); */
    
    len2 = len; /* 符号分を含めた出力に必要な長さ */
    symbol = 0; /* 符号部分 */
    if(minus) {
      symbol = '-';
    } else {
      if(flags & FC_BLANK) symbol = ' ';
      if(flags & FC_PLUS)  symbol = '+'; /* ' 'を上書きする */
    }
    if(symbol) len2++; 
    
    if(width < precision) width = precision; /* width.precision 区別してない…これでいいのか？ */
    if(width < len2) width = len2;           /* 最低限必要な幅に拡張(指定なき場合を含む) */
    
    if(!(flags & FC_MINUS)) { /* 左寄せでない場合 */
      if(flags & FC_ZERO) { /* ZERO-FILL */
        if(symbol) {
          if(ptr < eob) *ptr++ = symbol;              /* 符号をつけてから… */
        }
        for(i=0;i<width-len2;i++) {
          if(ptr < eob) *ptr++ = '0';  /* 隙間を0で埋める */
        }
      } else {                 /* 左寄せの場合 */
        for(i=0;i<width-len2;i++) {
          if(ptr < eob) *ptr++ = ' ';  /* 隙間を空白で埋めてから */
        }
        if(symbol) {
          if(ptr < eob) *ptr++ = symbol;              /* 符号をつける */
        }
      }
    } else {
      if(symbol) {
        if(ptr < eob) *ptr++ = symbol;                /* 必要なら符号を出力 */
      }
    }
    for(i = len - 1; i >= 0; i--) {
      if(ptr < eob) *ptr++ = numbuf[i]; /* (逆順の文字列を)出力する */
    }
    if(flags & FC_MINUS) { /* 左寄せで幅指定があれば空白を出力する */
      for(i = 0; i < width - len2; i++) {
        if(ptr < eob) *ptr++ = ' ';
      }
    }
    
    /* continue; */
  } /* while(*pf) */
  *ptr = '\0';
  return (ptr - buffer); /* 出力した文字列長 */
}

