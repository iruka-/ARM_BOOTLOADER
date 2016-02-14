/*********************************************************************
 *	ＣＱ付録ＮＸＰ-ＡＲＭ 基板用	ＬＥＤモニタ.
 *********************************************************************
 */


#include <string.h>

#ifdef	LPC214x
#include "LPC214x.h"
#else
#include "LPC23xx.h"
#endif

#include "swi.h"
#include "led.h"

typedef	unsigned int	uint;
volatile uint Timer;		/* 1kHz increment timer */

/*********************************************************************
 *	基板上のＬＥＤ点滅処理.
 *********************************************************************
 */
/*
#define FIO_BASE_ADDR		0x3FFFC000
// FGPIOコントローラのレジスタ 
#define FGPIO_FIO1DIR	((volatile uint *)(0x3FFFC020))
#define FGPIO_FIO1MASK	((volatile uint *)(0x3FFFC030))
#define FGPIO_FIO1PIN	((volatile uint *)(0x3FFFC034))
#define FGPIO_FIO1CLR	((volatile uint *)(0x3FFFC03C))
*/
#define	LED_PORT	1	// PIN1_18
#define	LED_PIN		18


static FGPIO_PORT *LPC_GPIO[5]=
{ LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3 , LPC_GPIO4 };

FGPIO_PORT *GPIOPortAdr(int portNum)
{
	return LPC_GPIO[portNum];
}


//===========================
// GPIO Set Direction
//===========================
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir )
{
    if(dir)
        LPC_GPIO[portNum]->DIR |= 1<<bitPosi;
    else
        LPC_GPIO[portNum]->DIR &= ~(1<<bitPosi);
}

//===========================
// GPIO Set Value
//===========================
void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal )
{
    if(bitVal)
        LPC_GPIO[portNum]->SET |= (1<<bitPosi);
    else
        LPC_GPIO[portNum]->CLR |= (1<<bitPosi);
}

//===========================
// GPIO Get Value
//===========================
uint32_t GPIOGetValue( uint32_t portNum, uint32_t bitPosi)
{
    uint32_t bitVal;
    bitVal = LPC_GPIO[portNum]->PIN;
    bitVal = (bitVal >> bitPosi) & 1;
    return bitVal;
}

//===========================
// GPIO Init Direction (32bit)
//===========================
void GPIOInitDir( uint32_t portNum, uint32_t dir )
{
	LPC_GPIO[portNum]->DIR = dir;
}


/* LEDを点灯する処理 */
void LED_On(void)
{
//	*FGPIO_FIO1PIN=0x00000000; /* P1[18] を'0'に（LEDを点ける） */
	GPIOSetValue( LED_PORT,LED_PIN, 0 );
}

/* LEDを消灯する処理 */
void LED_Off(void)
{
//	*FGPIO_FIO1PIN=0x00040000; /* P1[18] を'1'に（LEDを消す）   */
	GPIOSetValue( LED_PORT,LED_PIN, 1 );
}

void led_off(void)
{
	LED_Off();
}
	static int ledcnt = 0;

void LED_Flip(int period)
{
	int	maskbit=(1<<period);
	ledcnt++;
	if((ledcnt & (maskbit-1))==0) {
    	if(ledcnt & maskbit) LED_On();
    	else 				 LED_Off();
	}
}

void LED_Flip_Loop(int period,int cnt)
{
#if	0
	while(1) LED_Flip(period);
#else
	int i,j,flip=(2<<period);
	ledcnt=0;
	for(i=0;i<cnt;i++) {
		for(j=0;j<flip;j++) {
			LED_Flip(period);
		}
	}
	ledcnt=0;
	for(i=0;i<2;i++) {
		for(j=0;j<flip;j++) {
			LED_Flip(31);
		}
	}
#endif
}


/*********************************************************************
 *	タイマー割り込み：１ｍＳ単位
 *********************************************************************
 */
void Isr_TIMER0(void)
{
	T0IR = 1;	/* Clear irq flag */
	Timer++;

#if	0	// Debug
	LED_Flip(6);
	while(1) ;
#endif
}


void TIMER0_Init(void)
{
	/* Initialize Timer0 as 1kHz interval timer */
	RegisterVector(TIMER0_INT, Isr_TIMER0, PRI_LOWEST, CLASS_IRQ);

	T0CTCR = 0;
	T0MR0 = 18000 - 1;	/* 18M / 1k = 18000 */
	T0MCR = 0x3;		/* Clear TC and Interrupt on MR0 match */
	T0TCR = 1;
}
/*********************************************************************
 *	
 *********************************************************************
 */
