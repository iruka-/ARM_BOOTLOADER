/***********************************************************************
 *		LED�_�łɂ��debug
 ***********************************************************************
void led_init()		//!< LED�֘A������.
void led_on();		//!< LED��On
void led_off();		//!< LED��Off
void led_flip();	//!< LED�𔽓].
void led_blink(int div);	//!< (1<<div) ���������̂��ALED bit0 �𔽓].
 */
#include "common.h"
#include "debug.h"
#include "monitor/gpio.h"

#define	LED_PIN	PF3
#define	LO		0
#define	HI		1

static	int led_dat=0;
static	int led_cnt=0;

#define	SMALL_MEMCPY_MEMSET		(1) // �������[�ߖ�̂��߁A������memcpy������.


void led_init()
{
	pinMode(LED_PIN,OUTPUT);
	digitalWrite(LED_PIN,0);
}

void led_set(int l)
{
	led_dat = l;
	digitalWrite(LED_PIN,led_dat);
}

void led_on(void)
{
	led_set(LO);
}

void led_off(void)
{
	led_set(HI);
}

void led_flip(void)
{
	led_dat ^= 1;
	led_set(led_dat);
}

void led_blink(int div)
{
	led_cnt++;
	if(	led_cnt>=( 1<<div ) ) {
		led_cnt = 0;
		led_flip();
	}
}

//	dummy

void DbgPrintMsg(char *msg)
{
//	dputs(msg);
}
void DbgPrintValue(char *msg,int val)
{
//	dprinthex(msg,val);
}

#if	SMALL_MEMCPY_MEMSET
/***********************************************************************
 *		�������[�ߖ�̂��߁A������memcpy������.
 ***********************************************************************
 */
void *memcpy(void *dst0, const void *src0, size_t len0)
{
	char *dst = (char *) dst0;
	char *src = (char *) src0;

	while(len0--) {
		*dst++ = *src++;
	}
	return dst0;
}

void *memset(void *m,int c, size_t n)
{
	char *s = (char *) m;

	while(n--) {
		*s++ = c;
	}
	return m;
}


#endif
