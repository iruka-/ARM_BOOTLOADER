#ifndef	util_h_
#define	util_h_

typedef enum {
    OUTPUT, 
    OUTPUT_OPEN_DRAIN,
    INPUT,
    INPUT_ANALOG,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    INPUT_FLOATING,
    PWM,
    PWM_OPEN_DRAIN,
} _PinMode;

//	LOW = 0 Volt HIGH=3.3Volt
enum {
	LOW  = 0,
	HIGH = 1,
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
#if	0
	PC13	,
	PC14	,
	PC15	,
#endif
};

//	SET PORT DIRECTION
void	PinMode(int	pin, int mode);

//	OUTPUT PORT
void	DigitalWrite(int pin, int value);

//	INPUT  PORT
int		DigitalRead(int pin);

//	TOGGLE PORT
void	TogglePin(int pin);


void wait_us(int us);
void wait_ms(int ms);
void wait_u8s(int u8s);

#endif
