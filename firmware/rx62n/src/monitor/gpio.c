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

#if	0
#include "stm32f10x.h"
#include "platform_config.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "stm32_eval.h"
#endif

#include "monit.h"
#include "gpio.h"

/********************************************************************
 *	GPIOÇÃêßå‰.
 ********************************************************************
 *	mode : 0=input 1=output
 */
void	pinMode(int	pin, int mode)
{
	GPIO_TypeDef *p = port_p(pin);
	int mask = 1 << (pin & 7);

	//DDR(Data Direction Reg.)
	if(mode) {
		p->DDR.BYTE |= mask;
	}else{
		p->DDR.BYTE &= ~mask;
	}
}
/********************************************************************
 *	GPIOÇÃêßå‰.
 ********************************************************************
 */
void	digitalWrite(int pin, int value)
{
	GPIO_TypeDef *p = port_p(pin);	// port select

	int mask = 1 << (pin & 7);

	//DDR(Data Direction Reg.)
	if(value) {
		p->DR.BYTE |= mask;
	}else{
		p->DR.BYTE &= ~mask;
	}
}

/********************************************************************
 *	GPIOÇÃêßå‰.
 ********************************************************************
 */
int	digitalInvert(int pin)
{
	GPIO_TypeDef *p = port_p(pin);	// port select

	int mask = 1 << (pin & 7);

	p->DR.BYTE ^= mask;

	return (p->DR.BYTE >> (pin & 7)) & 1;
}

/********************************************************************
 *	GPIOÇÃêßå‰.
 ********************************************************************
 */
int		digitalRead(int pin)
{
	GPIO_TypeDef *p = port_p(pin);

	int mask = 1 << (pin & 7);
	if(p->PORT.BYTE & mask) {
		return 1;
	} else {
		return 0;
	}
}

/********************************************************************
 *	GPIOÇÃêßå‰.
 ********************************************************************
 */
