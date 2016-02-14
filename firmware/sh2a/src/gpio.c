#include <stdio.h>
#include <string.h>
#include <machine.h>
#include "../inc/iodefine.h"       /* SH7264 iodefine  */
#include "m3a_hs64.h"
#include "usb_firm.h"

#include "gpio.h"
#include "cpu.h"

#define	BLUELED	 PC5
#define	WHITELED PF6

//	ひとつのportは 0x20 バイト.   x	  (PA,PB,PC,PD,PE,PF)
typedef struct {
	unsigned short cr[7];	// +0000	cr7,cr6,cr5,cr4,cr3,cr2,cr1,cr0 の順.
	unsigned short cr0[1];	// +001e	複数機能の選択 4bit単位.

	unsigned short ior1[1];	// +0010
	unsigned short ior0[1];	// +0012	方向レジスタ.

	unsigned short dr1[1];	// +0014
	unsigned short dr0[1];	// +0016	出力レジスタ.

	unsigned short pr1[1];	// +0018
	unsigned short pr0[1];	// +001a	入力レジスタ.

	unsigned short rsv[2];	// +001c
} GPIO_TypeDef;


static GPIO_TypeDef *GPIO_base = (GPIO_TypeDef *) 0xFFFE3800;

static GPIO_TypeDef *port_p(int port)
{
	return &GPIO_base[ (port >> 5) & 7 ];
}

/********************************************************************
 *	GPIOの制御.
 ********************************************************************
 *	mode : 0=input 1=output
 */
void	pinMode(int	pin, int mode)
{
	GPIO_TypeDef *p = port_p(pin);
	int mask = 1 << (pin & 15);
	int n = -( (pin >> 4) & 1 );		// pinが 16～31なら -1になる.

	//DDR(Data Direction Reg.)
	if(mode) {
		p->ior0[n] |= mask;
	}else{
		p->ior0[n] &= ~mask;
	}
}
/********************************************************************
 *	GPIOの制御.
 ********************************************************************
 *	mode : 0=input 1=output
 */
void	pinModeEx(int pin, int mode)
{
	GPIO_TypeDef *p = port_p(pin);
	int shft = ((pin & 3) * 4 ) ;	// pin番号を 4で割った余り ( x4 )
	int mask = 0x000f << shft;		// 000f,00f0,0f00,f000 の4パタン.
	int n = -( (pin >> 2) & 7 );	// pin番号を 4で割った値. (0..7)

	// 該当する PxCRy の 4bitのみ書き換え.
	p->cr0[n] = ( p->cr0[n] & ( ~mask ) ) | ( (mode & 0x000f) << shft );
}
/********************************************************************
 *	GPIOの制御.
 ********************************************************************
 */
void	digitalWrite(int pin, int value)
{
	GPIO_TypeDef *p = port_p(pin);	// port select

	int mask = 1 << (pin & 15);

	if(value) {
		p->dr0[0] |= mask;
	}else{
		p->dr0[0] &= ~mask;
	}
}

/********************************************************************
 *	GPIOの制御.
 ********************************************************************
 */
int	digitalInvert(int pin)
{
	GPIO_TypeDef *p = port_p(pin);	// port select

	int mask = 1 << (pin & 15);

	p->dr0[0] ^= mask;

	return (p->pr0[0] >> (pin & 15)) & 1;
}

/********************************************************************
 *	GPIOの制御.
 ********************************************************************
 */
int		digitalRead(int pin)
{
	GPIO_TypeDef *p = port_p(pin);
	return (p->pr0[0] >> (pin & 15)) & 1;
}

