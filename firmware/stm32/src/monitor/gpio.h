#ifndef	_gpio_h_
#define	_gpio_h_

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
};

enum {
	PB0	=16	,
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
};

enum {
	PC0	=32	,
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
};

enum {
	PD0	=48	,
	PD1		,
	PD2		,
	PD3		,
	PD4		,
	PD5		,
	PD6		,
	PD7		,

	PD8		,
	PD9		,
	PD10	,
	PD11	,
	PD12	,
	PD13	,
	PD14	,
	PD15	,
};

enum {
	PE0	=64	,
	PE1		,
	PE2		,
	PE3		,
	PE4		,
	PE5		,
	PE6		,
	PE7		,

	PE8		,
	PE9		,
	PE10	,
	PE11	,
	PE12	,
	PE13	,
	PE14	,
	PE15	,
};

//	SET PORT DIRECTION
void	pinMode(int	pin, int mode);

//	OUTPUT PORT
void	digitalWrite(int pin, int value);

//	INPUT  PORT
int		digitalRead(int pin);


static	inline	GPIO_TypeDef *port_p(int pin)
{
	static	GPIO_TypeDef *port_tab[]={
		GPIOA,
		GPIOB,
		GPIOC,
		GPIOD,
		GPIOE,
		GPIOF,
		GPIOG,
		GPIOG,
	};
	return port_tab[(pin >> 4) & 7];
}

/********************************************************************
 *	GPIO‚Ì§Œä.(ƒCƒ“ƒ‰ƒCƒ“ŠÖ”)
 ********************************************************************
 */
static inline void dgWrite(GPIO_TypeDef *p,int mask,int value)
{
//	int mask = 1 << (pin & 15);
	if(value) {
		p->BSRR = mask;		// bit set pattern
	}else{
		p->BRR  = mask;		// bit reset pattern
	}
}

static inline int  dgRead(GPIO_TypeDef *p,int mask) 
{
//	int mask = 1 << (pin & 15);
  	if(p->IDR & mask) {
		return 1;
   	}else{
		return 0;
   	}
}

//=================================================================

#define	JTAG_PORT_ACCEL_ON					\
	GPIO_TypeDef *jport_out = port_p(TDI);			\
	GPIO_TypeDef *jport_in  = port_p(TDO);


#define	Set_bit(pin_no_,bit_)	\
	if(bit_) jport_out->BSRR = (1<<(pin_no_ & 0x0f));	\
	else	 jport_out->BRR  = (1<<(pin_no_ & 0x0f));


#define	Get_TDO() 				\
	( jport_in->IDR & (1<< (TDO & 0x0f) ) )

#endif
