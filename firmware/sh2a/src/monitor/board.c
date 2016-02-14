/********************************************************************
 *
 ********************************************************************
 */
#include "../includes.h"
#include "monit.h"
void set_imask( int imask )   ;

void usbModuleDisable(void)
{
	wait_ms(100);
	led_off();
	/* ==== Interrupt mask level change ==== */
	set_imask(15);				//(0x000000F0);

	/* USBコントローラディセーブル */
	INTC.IPR10.BIT._USB = 0;	/* Set USB interrupt level */
	USB.SYSCFG.WORD = 0x0481;	/* DPRUP off */
	wait_ms(30);
	USB.SYSCFG.WORD = 0x0401;	/* HSE off */
	wait_ms(30);
	USB.SYSCFG.WORD = 0x0400;	/* USB off */
	wait_ms(30);
	USB.SYSCFG.WORD = 0x0;		/* SCKE off */
	usb_reset_module();			/* USBモジュール リセット */

	wait_ms(1200);
	INTC.IBNR.WORD = 0x0000;	/* レジスタバングディセーブル */
	cache_disable();
}









/********************************************************************
 *	( ms ) * 1mS 待つ.
 ********************************************************************
 */
void wait_ms(int ms)
{
	int i;
	for(i=0; i<ms; i++) {
		wait_125ns(1000*8);
	}
}

/********************************************************************
 *	( us ) * 1uS 待つ.
 ********************************************************************
 */
void wait_us(int us)
{
	wait_125ns(us*8);
}
/********************************************************************
 *	( us * 1/8 ) uS 待つ. clock=72MHzと仮定.
 ********************************************************************
 8002186:	bf00      	nop
 8002188:	bf00      	nop
 800218a:	bf00      	nop
 800218c:	bf00      	nop
 800218e:	bf00      	nop
 8002190:	3301      	adds	r3, #1
 8002192:	4283      	cmp	r3, r0
 8002194:	dbf7      	blt.n	8002186 <wait_u8s+0x4>
 */
#ifdef	MARY
//clock=48MHzと仮定.	最適化オプションやコンパイラが変わったら再計測.
void wait_125ns(int ns)
{
	int i;
	for(i=0; i<ns; i++) {
		asm("nop");
	}
}
#else

#ifdef	SH2A
//clock=144MHzと仮定.	最適化オプションやコンパイラが変わったら再計測.
//	1 loop = 1/8us = 18cycles
void wait_125ns(int ns)
{
	int i;
	for(i=0; i<ns; i++) {
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");

		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");

		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");

		asm("nop");
	}
}
#else

//clock=72MHzと仮定.	最適化オプションやコンパイラが変わったら再計測.
//	1 loop = 1/8us = 9cycles
void wait_125ns(int ns)
{
	int i;
	for(i=0; i<ns; i++) {
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
	}
}
#endif

#endif
/********************************************************************
 *	( ms ) * 1mS 待つ.
 ********************************************************************
 */
void wait_0us(void)
{

}

