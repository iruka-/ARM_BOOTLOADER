/********************************************************************
 *
 ********************************************************************
 */
#include "common.h"
#include "cpu_config.h"
#include "hw_support.h"
#include "led.h"
#include "dbg_print.h"
#include "../debug.h"
#include "monit.h"

#include "core_cm3.h"

void SetSoftwareResetVector(int *vect);

void usbModuleDisable(void)
{
	/* low layer USB function driver disconnect request */
	USBF_UnAttach();
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
//clock=144MHzと仮定.	最適化オプションやコンパイラが変わったら再計測.
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

//#define	RAM_VECTOR	(int*) 0x1fff0000

//FM3のみ.
void setupVectorTable(int vect)
{
//	int *ram_vect = RAM_VECTOR;
	int *rom_vect = (int*) vect;

	SetSoftwareResetVector(rom_vect);

	__disable_irq();
	wait_ms(500);			// 0.5秒待つ.


	NVIC_SystemReset();		// Software Reset. (See RST_STR リセット要因レジスタ .SRST)
							//		or bFM3_CRG_RST_STR_SRST
	//戻ってきません.

#if	0	// 自力でbootする.

	int stk  = rom_vect[0];
	int adrs = rom_vect[1];

	void (*func)(void);
	func = (void (*)()) adrs;

	// ベクターオフセット設定.
		//
		// ユーザーベクターアドレスをVTORにセット.
		//
	SCB->VTOR = rom_vect;

	// スタックポインタ初期化.
    __asm volatile ("MSR msp, %0\n\t" : : "r" (stk) );

	func();

#endif
}	
