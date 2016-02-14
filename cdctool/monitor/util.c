/********************************************************************
 *	ŠÈˆÕƒ‚ƒjƒ^
 ********************************************************************
 */
#include <string.h>
#include "libmaple.h"
#include "monit.h"

#include "hidcmd.h"
#include "monit.h"
#include "picwrt.h"
#include "../libmaple/gpio.h"

#include "util.h"

#define	LIBC_REPLACE_MEMCPY		(1)		//

typedef int boolean;
enum {
	false,
	true,
};

/********************************************************************
 *
 ********************************************************************
 */
static gpio_dev *PORTADR[]={
	&gpioa,
	&gpiob,
	&gpioc,
	&gpiod,
	&gpioe,
};

#define	BOARD_NR_GPIO_PINS 16*3

void PinMode(int pin,int mode)
{
    gpio_pin_mode outputMode;
    boolean pwm = false;

    if (pin >= BOARD_NR_GPIO_PINS) {
        return;
    }

    switch(mode) {
    case OUTPUT:
        outputMode = GPIO_OUTPUT_PP;
        break;
    case OUTPUT_OPEN_DRAIN:
        outputMode = GPIO_OUTPUT_OD;
        break;
    case INPUT:
    case INPUT_FLOATING:
        outputMode = GPIO_INPUT_FLOATING;
        break;
    case INPUT_ANALOG:
        outputMode = GPIO_INPUT_ANALOG;
        break;
    case INPUT_PULLUP:
        outputMode = GPIO_INPUT_PU;
        break;
    case INPUT_PULLDOWN:
        outputMode = GPIO_INPUT_PD;
        break;
    case PWM:
        outputMode = GPIO_AF_OUTPUT_PP;
        pwm = true;
        break;
    case PWM_OPEN_DRAIN:
        outputMode = GPIO_AF_OUTPUT_OD;
        pwm = true;
        break;
    default:
//        ASSERT(0);
        return;
    }
    gpio_set_mode(PORTADR[pin>>4], pin & 15 , outputMode);
}

int DigitalRead(int pin)
{
    return gpio_read_bit(PORTADR[pin>>4], pin & 15 );
}

void DigitalWrite(int pin, int val)
{
    gpio_write_bit(PORTADR[pin>>4], pin & 15, val);
}

void TogglePin(int pin) 
{
    gpio_toggle_bit(PORTADR[pin>>4], pin & 15);
}


/********************************************************************
 *	
 ********************************************************************
 */
void wait_ms(int ms)
{
	int i;
	for(i=0;i<ms;i++) {
		wait_us(1000);
	}
}

/********************************************************************
 *	
 ********************************************************************
 */
void wait_us(int us)
{
	wait_u8s(us*8);
}

/********************************************************************
 *	( us * 1/8 ) uS ‘Ò‚Â. clock=72MHz‚Æ‰¼’è.
 ********************************************************************
 8002186:	bf00      	nop
 8002188:	bf00      	nop
 800218a:	bf00      	nop
 800218c:	bf00      	nop
 800218e:	bf00      	nop
 8002190:	3301      	adds	r3, #1
 8002192:	4283      	cmp	r3, r0
 8002194:	dbf7      	blt.n	8002186 <wait_u8s+0x4>
 */
void wait_u8s(int us)
{
	int i;
	for(i=0;i<us;i++) {
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
	}
}


//
//	memcpy replace (for small foot print.)
//

#if	LIBC_REPLACE_MEMCPY

//#define	_PTR		void *
_PTR memcpy(_PTR dst, const _PTR src, size_t size)
{
	char *s=(char*)src;
	char *t=(char*)dst;
	while(size--) *t++ = *s++;
	return dst;
}

#endif


#ifdef	BOARD_STBeeMini
//static inline 
void disableDebugPorts(void) 
{
    afio_cfg_debug_ports(AFIO_DEBUG_NONE);
}
#endif
