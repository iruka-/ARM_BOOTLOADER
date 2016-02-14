/********************************************************************
 *	GPIOÇÃêßå‰.
 ********************************************************************
 *
 *	à»â∫ÇÃArduinoïóä÷êîÇé¿ëïÇ∑ÇÈ.
 *
void	pinMode(int	pin, int mode)
void	digitalWrite(int pin, int value)
int		digitalRead(int pin)
 *
 ********************************************************************
 */

#include "stm32f10x.h"
#include "platform_config.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "stm32_eval.h"
#include "monit.h"
#include "gpio.h"

/********************************************************************
 *	GPIOÇÃêßå‰.
 ********************************************************************
 */
void	pinMode(int	pin, int mode)
{
	GPIO_TypeDef *p = port_p(pin);
	int ddr;			// a copy of data direction register.
	int lsr = (pin & 7) * 4;
	int mask,val=0x04;	// ddr mask. , ddr value.
	if(mode) val=0x03;	// OUTPUT.
	pin &= 15;

	mask = 0x0f << lsr;
	val  = val  << lsr;

	if(pin >= 8) {
		ddr = p->CRH;
		ddr = ( ddr & (~mask) ) | val;
		p->CRH = ddr;
	} else {
		ddr = p->CRL;
		ddr = ( ddr & (~mask) ) | val;
		p->CRL = ddr;
	}
}
/********************************************************************
 *	GPIOÇÃêßå‰.
 ********************************************************************
 */
void	digitalWrite(int pin, int value)
{
	GPIO_TypeDef *p = port_p(pin);	// port select
	int mask = 1 << (pin & 15);		// mask 0b0000_1111  (0..15)
	if(value) {
		p->BSRR = mask;		// bit set pattern
	} else {
		p->BRR  = mask;		// bit reset pattern
	}
}

/********************************************************************
 *	GPIOÇÃêßå‰.
 ********************************************************************
 */
int		digitalRead(int pin)
{
	GPIO_TypeDef *p = port_p(pin);
	int mask = 1 << (pin & 15);
	if(p->IDR & mask) {
		return 1;
	} else {
		return 0;
	}
}

/********************************************************************
 *	GPIOÇÃêßå‰.
 ********************************************************************
 */
