#ifndef	_gpio_h_
#define	_gpio_h_

//#include "../inc/iodefine.h"

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

//	PORT PIN ASSIGN
enum {
	PA0	= 0	,
	PA1		,
	PA2		,
	PA3		,
	PA4		,
	PA5		,
	PA6		,
	PA7		,
	PA8		,
	PA9		,
	PA10	,
	PA11	,
	PA12	,
	PA13	,
	PA14	,
	PA15	,

	PB0	= 32,
	PB1		,
	PB2		,
	PB3		,
	PB4		,
	PB5		,
	PB6		,
	PB7		,
	PB8		,
	PB9		,
	PB10	,
	PB11	,
	PB12	,
	PB13	,
	PB14	,
	PB15	,

	PC0	= 64,
	PC1		,
	PC2		,
	PC3		,
	PC4		,
	PC5		,
	PC6		,
	PC7		,
	PC8		,
	PC9		,
	PC10	,
	PC11	,
	PC12	,
	PC13	,
	PC14	,
	PC15	,

	PD0	= 96,
	PE0	= 128,


	PF0	= 160,
	PF1		,
	PF2		,
	PF3		,
	PF4		,
	PF5		,
	PF6		,
	PF7		,
	PF8		,
	PF9		,
	PF10	,
	PF11	,
	PF12	,
	PF13	,
	PF14	,
	PF15	,
};

//	SET PORT DIRECTION
void	pinMode(int	pin, int mode);

//	OUTPUT PORT
void	digitalWrite(int pin, int value);

//	INPUT  PORT
int		digitalRead(int pin);

//	INPUT  PORT
int		digitalInvert(int pin);

/********************************************************************
 *	GPIOÇÃêßå‰.(ÉCÉìÉâÉCÉìä÷êî)
 ********************************************************************
 */
//=================================================================

#define	JTAG_PORT_ACCEL_ON					\


#define	Set_bit(pin_no_,bit_)	\

#define	Set_bitTCK(pin_no_,bit_)	\

#define	Get_TDO() 				\




#endif
