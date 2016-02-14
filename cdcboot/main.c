// Sample main.cpp file. Blinks the built-in LED, sends a message out
// USART2, and turns on PWM on pin 2.

//#include "s:monitor/wirish.h"
#define true 0x1
#define false 0x0


#define	VCOM_ECHOBACK_TEST	(0)		// 1にすると エコーバックテスト.


void init();
void LEDtest();

//#define PWM_PIN  2



void setup() {
    /* Set up the LED to blink  */
//    pinMode(BOARD_LED_PIN, OUTPUT);

    /* Turn on PWM on pin PWM_PIN */
//    pinMode(PWM_PIN, PWM);
//    pwmWrite(PWM_PIN, 0x8000);

    /* Send a message out the usb virtual serial port  */
//    SerialUSB.println("Hello!");
}

#if	VCOM_ECHOBACK_TEST
void loop()
{
//	LEDtest();
    while (1) {
		int ch = USBgetch();
		USBputch(ch);
	}
}
#else
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
#endif


int main(void)
{
	init();
    setup();

    while (true) {
        loop();
    }

    return 0;
}
