#ifndef	_gpio_h_
#define	_gpio_h_

#include "../inc/iodefine.h"

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
	P0_0	= 0	,
	P0_1		,
	P0_2		,
	P0_3		,
	P0_4		,
	P0_5		,
	P0_6		,
	P0_7		,

	P1_0		,
	P1_1		,
	P1_2		,
	P1_3		,
	P1_4		,
	P1_5		,
	P1_6		,
	P1_7		,

	P2_0		,
	P2_1		,
	P2_2		,
	P2_3		,
	P2_4		,
	P2_5		,
	P2_6		,
	P2_7		,

	P3_0		,
	P3_1		,
	P3_2		,
	P3_3		,
	P3_4		,
	P3_5		,
	P3_6		,
	P3_7		,

	P4_0		,
	P4_1		,
	P4_2		,
	P4_3		,
	P4_4		,
	P4_5		,
	P4_6		,
	P4_7		,

	P5_0		,
	P5_1		,
	P5_2		,
	P5_3		,
	P5_4		,
	P5_5		,
	P5_6		,
	P5_7		,

	P6_0		,
	P6_1		,
	P6_2		,
	P6_3		,
	P6_4		,
	P6_5		,
	P6_6		,
	P6_7		,

	P7_0		,
	P7_1		,
	P7_2		,
	P7_3		,
	P7_4		,
	P7_5		,
	P7_6		,
	P7_7		,

	P8_0		,
	P8_1		,
	P8_2		,
	P8_3		,
	P8_4		,
	P8_5		,
	P8_6		,
	P8_7		,

	P9_0		,
	P9_1		,
	P9_2		,
	P9_3		,
	P9_4		,
	P9_5		,
	P9_6		,
	P9_7		,

	PA_0		,
	PA_1		,
	PA_2		,
	PA_3		,
	PA_4		,
	PA_5		,
	PA_6		,
	PA_7		,

	PB_0		,
	PB_1		,
	PB_2		,
	PB_3		,
	PB_4		,
	PB_5		,
	PB_6		,
	PB_7		,

	PC_0		,
	PC_1		,
	PC_2		,
	PC_3		,
	PC_4		,
	PC_5		,
	PC_6		,
	PC_7		,

	PD_0		,
	PD_1		,
	PD_2		,
	PD_3		,
	PD_4		,
	PD_5		,
	PD_6		,
	PD_7		,

	PE_0		,
	PE_1		,
	PE_2		,
	PE_3		,
	PE_4		,
	PE_5		,
	PE_6		,
	PE_7		,

	PF_0		,
	PF_1		,
	PF_2		,
	PF_3		,
	PF_4		,
	PF_5		,
	PF_6		,
	PF_7		,

	PG_0		,
	PG_1		,
	PG_2		,
	PG_3		,
	PG_4		,
	PG_5		,
	PG_6		,
	PG_7		,


};

//	SET PORT DIRECTION
void	pinMode(int	pin, int mode);

//	OUTPUT PORT
void	digitalWrite(int pin, int value);

//	INPUT  PORT
int		digitalRead(int pin);

//	INPUT  PORT
int		digitalInvert(int pin);

typedef	struct st_port1 GPIO_TypeDef;

static	inline	GPIO_TypeDef *port_p(int pin)
{
	char *PortBase = (char *) ( &PORT0 );
	int   off = (pin >> 3) & 0x1f;	// PORT0`PORTG‚Ü‚Å. 8bit x 17port
	return (GPIO_TypeDef *) (PortBase+off);
}

/********************************************************************
 *	GPIO‚Ì§Œä.(ƒCƒ“ƒ‰ƒCƒ“ŠÖ”)
 ********************************************************************
 */
//=================================================================

#define	JTAG_PORT_ACCEL_ON					\
	GPIO_TypeDef *jport_out = port_p(TDI);			\
	GPIO_TypeDef *jport_in  = port_p(TDO);


#define	Set_bit(pin_no_,bit_)	\
	if(bit_) jport_out->DR.BYTE |=  (1<<(pin_no_ & 0x07));	\
	else	 jport_out->DR.BYTE &= ~(1<<(pin_no_ & 0x07));

#define	Set_bitTCK(pin_no_,bit_)	\
	if(bit_) jport_in->DR.BYTE |=  (1<<(pin_no_ & 0x07));	\
	else	 jport_in->DR.BYTE &= ~(1<<(pin_no_ & 0x07));


#define	Get_TDO() 				\
	( jport_in->PORT.BYTE & (1<< (TDO & 0x07) ) )




#endif
