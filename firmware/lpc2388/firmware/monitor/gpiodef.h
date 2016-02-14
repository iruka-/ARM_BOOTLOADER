#ifndef	_gpiodef_h_
#define	_gpiodef_h_

#include "../hwlib/led.h"

//	LOW = 0 Volt HIGH=3.3Volt
enum {
	LOW  = 0,
	HIGH = 1,
};

//	PORT DIRECTION
enum {
	INPUT = 0,
	OUTPUT= 1,
};

#define	PIO_(port_num,pin_num)  	(port_num*32+pin_num)

//	SET PORT DIRECTION
void	pinMode(int	pin, int mode);

//	OUTPUT PORT
void	digitalWrite(int pin, int value);

//	INPUT  PORT
int		digitalRead(int pin);

//=================================================================
FGPIO_PORT *GPIOPortAdr(int portNum);

#define	JTAG_PORT_ACCEL_ON					\
	FGPIO_PORT *jport_out = GPIOPortAdr( TDI >> 5 );			\


#define	Set_bit(pin_no_,bit_)	\
	if(bit_) jport_out->SET = (1<<(pin_no_ & 0x1f));	\
	else	 jport_out->CLR = (1<<(pin_no_ & 0x1f));


#define	Get_TDO() 				\
	( jport_out->PIN & (1<< (TDO & 0x1f) ) )


#endif
