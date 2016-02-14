/********************************************************************
 *	GPIO�̐���.
 ********************************************************************
 *
 *	�ȉ���Arduino���֐�����������.
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
#include "../type_t.h"
#include "../hwlib/led.h"

/*
#include "../gpio.c"
*/

/********************************************************************
 *	GPIO�̐���.
 ********************************************************************
 */
void	pinMode(int	pin, int mode) 
{
	uint32_t portNum =  pin >> 5;
	uint32_t bitPosi =  pin & 31;
	GPIOSetDir( portNum, bitPosi, mode );
}
/********************************************************************
 *	GPIO�̐���.
 ********************************************************************
 */
void	digitalWrite(int pin, int value) 
{
	uint32_t portNum =  pin >> 5;
	uint32_t bitPosi =  pin & 31;
	int       val = 0;
	if(value) val = 1;
	
	GPIOSetValue(portNum, bitPosi, val );
}

/********************************************************************
 *	GPIO�̐���.
 ********************************************************************
 */
int		digitalRead(int pin) 
{
	uint32_t portNum =  pin >> 5;
	uint32_t bitPosi =  pin & 31;
	
	return GPIOGetValue(portNum, bitPosi);
}
/********************************************************************
 *	
 ********************************************************************
 */

