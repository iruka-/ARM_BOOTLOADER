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

//	�ЂƂ�port�� 0x20 �o�C�g.   x	  (PA,PB,PC,PD,PE,PF)
typedef struct {
	unsigned short cr[7];	// +0000	cr7,cr6,cr5,cr4,cr3,cr2,cr1,cr0 �̏�.
	unsigned short cr0[1];	// +001e	�����@�\�̑I�� 4bit�P��.

	unsigned short ior1[1];	// +0010
	unsigned short ior0[1];	// +0012	�������W�X�^.

	unsigned short dr1[1];	// +0014
	unsigned short dr0[1];	// +0016	�o�̓��W�X�^.

	unsigned short pr1[1];	// +0018
	unsigned short pr0[1];	// +001a	���̓��W�X�^.

	unsigned short rsv[2];	// +001c
} GPIO_TypeDef;


static GPIO_TypeDef *GPIO_base = (GPIO_TypeDef *) 0xFFFE3800;

static GPIO_TypeDef *port_p(int port)
{
	return &GPIO_base[ (port >> 5) & 7 ];
}

/********************************************************************
 *	GPIO�̐���.
 ********************************************************************
 *	mode : 0=input 1=output
 */
void	pinMode(int	pin, int mode)
{
	GPIO_TypeDef *p = port_p(pin);
	int mask = 1 << (pin & 15);
	int n = -( (pin >> 4) & 1 );		// pin�� 16�`31�Ȃ� -1�ɂȂ�.

	//DDR(Data Direction Reg.)
	if(mode) {
		p->ior0[n] |= mask;
	}else{
		p->ior0[n] &= ~mask;
	}
}
/********************************************************************
 *	GPIO�̐���.
 ********************************************************************
 *	mode : 0=input 1=output
 */
void	pinModeEx(int pin, int mode)
{
	GPIO_TypeDef *p = port_p(pin);
	int shft = ((pin & 3) * 4 ) ;	// pin�ԍ��� 4�Ŋ������]�� ( x4 )
	int mask = 0x000f << shft;		// 000f,00f0,0f00,f000 ��4�p�^��.
	int n = -( (pin >> 2) & 7 );	// pin�ԍ��� 4�Ŋ������l. (0..7)

	// �Y������ PxCRy �� 4bit�̂ݏ�������.
	p->cr0[n] = ( p->cr0[n] & ( ~mask ) ) | ( (mode & 0x000f) << shft );
}
/********************************************************************
 *	GPIO�̐���.
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
 *	GPIO�̐���.
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
 *	GPIO�̐���.
 ********************************************************************
 */
int		digitalRead(int pin)
{
	GPIO_TypeDef *p = port_p(pin);
	return (p->pr0[0] >> (pin & 15)) & 1;
}

