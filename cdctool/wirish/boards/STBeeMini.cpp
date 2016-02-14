/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2011 LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file   maple_mini.cpp
 * @author Marti Bolivar <mbolivar@leaflabs.com>
 * @brief  Maple Mini PIN_MAP and boardInit().
 */

#include "STBeeMini.h"
#include "gpio.h"
#include "wirish_debug.h"

#ifdef BOARD_STBeeMini

/* Since we want the Serial Wire/JTAG pins as GPIOs, disable both SW
 * and JTAG debug support */
void boardInit(void) {
    disableDebugPorts();
}

extern const stm32_pin_info PIN_MAP[BOARD_NR_GPIO_PINS] = {

	{GPIOA, TIMER2, ADC1,  0, 1,    0}, /* D0/PA0 */
    {GPIOA, TIMER2, ADC1,  1, 2,    1}, /* D1/PA1 */
    {GPIOA, TIMER2, ADC1,  2, 3,    2}, /* D2/PA2 */
    {GPIOA, TIMER2, ADC1,  3, 4,    3}, /* D3/PA3 */
    {GPIOA,   NULL, ADC1,  4, 0,    4}, /* D4/PA4 */
    {GPIOA,   NULL, ADC1,  5, 0,    5}, /* D5/PA5 */
    {GPIOA, TIMER3, ADC1,  6, 1,    6}, /* D6/PA6 */
    {GPIOA, TIMER3, ADC1,  7, 2,    7}, /* D7/PA7 */
    {GPIOA, TIMER1, NULL,  8, 1, ADCx}, /* D8/PA8 */
    {GPIOA, TIMER1, NULL,  9, 2, ADCx}, /* D9/PA9 */
    {GPIOA, TIMER1, NULL, 10, 3, ADCx}, /* D10/PA10 */
	{GPIOA, TIMER1, NULL, 11, 4, ADCx}, /* D11/PA11(USB D-) */
	{GPIOA,   NULL, NULL, 12, 0, ADCx}, /* D12/PA12(USB D+) */
	{GPIOA,   NULL, NULL, 13, 0, ADCx}, /* D13/PA13(LED1) */
	{GPIOA,   NULL, NULL, 14, 0, ADCx}, /* D14/PA14(USB DISC) */
	{GPIOA,   NULL, NULL, 15, 0, ADCx}, /* D15/PA15(LED2) */

	{GPIOB, TIMER3, ADC1,  0, 3,    8}, /* D16/PB0 */
    {GPIOB, TIMER3, ADC1,  1, 4,    9}, /* D17/PB1 */
    {GPIOB,   NULL, NULL,  2, 0, ADCx}, /* D18/PB2 */
	{GPIOB,   NULL, NULL,  3, 0, ADCx}, /* D19/PB3 */
    {GPIOB,   NULL, NULL,  4, 0, ADCx}, /* D20/PB4 */
    {GPIOB,   NULL, NULL,  5, 0, ADCx}, /* D21/PB5 */
    {GPIOB, TIMER4, NULL,  6, 1, ADCx}, /* D22/PB6 */
    {GPIOB, TIMER4, NULL,  7, 2, ADCx}, /* D23/PB7 */
    {GPIOB, TIMER4, NULL,  8, 3, ADCx}, /* D24/PB8 */
    {GPIOB, TIMER4, NULL,  9, 4, ADCx}, /* D25/PB9 */
    {GPIOB,   NULL, NULL, 10, 0, ADCx}, /* D26/PB10 */
    {GPIOB,   NULL, NULL, 11, 0, ADCx}, /* D27/PB11 */
    {GPIOB,   NULL, NULL, 12, 0, ADCx}, /* D28/PB12 */
    {GPIOB,   NULL, NULL, 13, 0, ADCx}, /* D29/PB13 */
    {GPIOB,   NULL, NULL, 14, 0, ADCx}, /* D30/PB14 */
    {GPIOB,   NULL, NULL, 15, 0, ADCx}, /* D31/PB15 */

	{GPIOC,   NULL, NULL, 13, 0, ADCx}, /* D32/PC13(USR SW) */
    {GPIOC,   NULL, NULL, 14, 0, ADCx}, /* D33/PC14 */
    {GPIOC,   NULL, NULL, 15, 0, ADCx}  /* D34/PC15 */

};

extern const uint8 boardPWMPins[BOARD_NR_PWM_PINS] __FLASH__ = {
	PA(0), PA(1), PA(2), PA(3), PA(6), PA(7), PA(8),  PA(9), PA(10), PB(0), PB(6), PB(7)
};

extern const uint8 boardADCPins[BOARD_NR_ADC_PINS] __FLASH__ = {
	PA(0), PA(1), PA(2), PA(3), PA(4), PA(5), PA(6), PA(7), PB(0), PB(1)
};


#define USB_DP PA(11)
#define USB_DM PA(12)

extern const uint8 boardUsedPins[BOARD_NR_USED_PINS] __FLASH__ = {
    BOARD_LED_PIN, BOARD_BUTTON_PIN, USB_DP, USB_DM
};

#endif
