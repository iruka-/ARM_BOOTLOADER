// Sample main.cpp file. Blinks the built-in LED, sends a message out
// USART2, and turns on PWM on pin 2.

#include "wirish.h"
#include "monitor/monit.h"

void setup() {
    /* Set up the LED to blink  */
    pinMode(BOARD_LED_PIN, OUTPUT);

    /* Turn on PWM on pin PWM_PIN */
//    pinMode(PWM_PIN, PWM);
//    pwmWrite(PWM_PIN, 0x8000);

    /* Send a message out the usb virtual serial port  */
//    SerialUSB.println("Hello!");
}


//-----------------------------------------------

void loop()
{
    while (1) {
		int ch = USBgetch();
		if(	ch == 0xaa) {
			ch = USBgetch();
			if(	ch == 0x55) {
				USBmonit();
			}
		}
	}
}

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need libmaple may fail.
__attribute__((constructor)) void premain()
{
    init();			// call wirish/board.cpp 
}

int main(void) {
    setup();

    while (true) {
        loop();
    }

    return 0;
}
