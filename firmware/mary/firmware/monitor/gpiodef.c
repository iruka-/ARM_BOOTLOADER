/********************************************************************
 *	GPIO‚Ì§Œä.
 ********************************************************************
 *
 *	ˆÈ‰º‚ÌArduino•—ŠÖ”‚ðŽÀ‘•‚·‚é.
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
#include "gpiodef.h"
#include "../gpio.h"

/*
#include "../gpio.c"
*/


/********************************************************************
 *	GPIO‚Ì§Œä.
 ********************************************************************
 */
void	pinMode(int	pin, int mode) 
{
	uint32_t portNum =  pin >> 4;
	uint32_t bitPosi =  pin & 15;
	GPIOSetDir( portNum, bitPosi, mode );
}
/********************************************************************
 *	GPIO‚Ì§Œä.
 ********************************************************************
 */
void	digitalWrite(int pin, int value) 
{
	uint32_t portNum =  pin >> 4;
	uint32_t bitPosi =  pin & 15;
	int       val = 0;
	if(value) val = 1;
	
	GPIOSetValue(portNum, bitPosi, val );
}

/********************************************************************
 *	GPIO‚Ì§Œä.
 ********************************************************************
 */
int		digitalRead(int pin) 
{
	uint32_t portNum =  pin >> 4;
	uint32_t bitPosi =  pin & 15;
	
	return GPIOGetValue(portNum, bitPosi);
}
/********************************************************************
 *	
 ********************************************************************
 */

