#include "monitor/monit.h"
//#include "type.h"
//#include "color_led.h"

void usbModuleDisable(void){}
//void FLASH_ProgramPage(void){}
void LED_Flip(int interval);

/********************************************************************
 *
 ********************************************************************
 */
void led_blink(int interval)
{
	LED_Flip(interval);
#if	0
// MARY
	static int led_cnt=0;
	static int led_col=7;
	int mask = 1<<interval;

	led_cnt++;
	if (led_cnt >=mask) {
		led_cnt = 0;
		Set_Color_LED(led_col++);
	}
#endif
}

/********************************************************************
 *	( ms ) * 1mS 待つ.
 ********************************************************************
 */
void wait_ms(int ms)
{
	int i;
	for(i=0;i<ms;i++) {
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
	for(i=0;i<ns;i++) {
		asm("nop");
	}
}
#else
//clock=72MHzと仮定.	最適化オプションやコンパイラが変わったら再計測.
void wait_125ns(int ns)
{
	int i;
	for(i=0;i<ns;i++) {
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
	}
}
#endif
/********************************************************************
 *	( ms ) * 1mS 待つ.
 ********************************************************************
 */
void wait_0us(void)
{

}

