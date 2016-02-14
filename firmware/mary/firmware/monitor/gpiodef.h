#ifndef	_gpiodef_h_
#define	_gpiodef_h_

#include "flash.h"
#include "../gpio.h"

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


enum {
	P0_0,
	P0_1,
	P0_2,
	P0_3,
	P0_4,
	P0_5,
	P0_6,
	P0_7,
	P0_8,
	P0_9,
	P0_10,
	P0_11,
	P0_12,
	P0_13,
	P0_14,
	P0_15,
	
	P1_0,
	P1_1,
	P1_2,
	P1_3,
	P1_4,
	P1_5,
	P1_6,
	P1_7,
	P1_8,
	P1_9,
	P1_10,
	P1_11,
	P1_12,
	P1_13,
	P1_14,
	P1_15,
	
	P2_0,
	P2_1,
	P2_2,
	P2_3,
	P2_4,
	P2_5,
	P2_6,
	P2_7,
	P2_8,
	P2_9,
	P2_10,
	P2_11,
	P2_12,
	P2_13,
	P2_14,
	P2_15,
	
};

//	SET PORT DIRECTION
void	pinMode(int	pin, int mode);

//	OUTPUT PORT
void	digitalWrite(int pin, int value);

//	INPUT  PORT
int		digitalRead(int pin);

#if	0
//=================================================================

#define	JTAG_PORT_ACCEL_ON					\
	LPC_GPIO_TypeDef *jport_out = GPIOPortAdr( TCK >> 4 );			\
	LPC_GPIO_TypeDef *jport_in  = GPIOPortAdr( TDI >> 4 );			\


#define	Set_bit(pin_no_,bit_)	\
	if(bit_) jport_out->MASKED_ACCESS[(1<<(pin_no_ & 0x0f))] = (1<<(pin_no_ & 0x0f));	\
	else     jport_out->MASKED_ACCESS[(1<<(pin_no_ & 0x0f))] = (0);

#define	Set_bitTDI(pin_no_,bit_)	\
	if(bit_) jport_in->MASKED_ACCESS[(1<<(pin_no_ & 0x0f))] = (1<<(pin_no_ & 0x0f));	\
	else     jport_in->MASKED_ACCESS[(1<<(pin_no_ & 0x0f))] = (0);


#define	Get_TDO() 				\
 (   (jport_out->MASKED_ACCESS[(1<<(TDO & 0x0f))]) >> (TDO & 0x0f) )

//#define	Set_bit(pin_no_,bit_)	set_bit(pin_no_,bit_)
//#define	Get_TDO() 				digitalRead( TDO )

#else

#define	JTAG_PORT_ACCEL_ON		/* */
#define	Set_bit(Port_,val_) 	digitalWrite(Port_,val_)
#define	Get_TDO() 				digitalRead(TDO)

#endif

#endif
