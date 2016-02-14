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
 * @file   maple.cpp
 * @author Marti Bolivar <mbolivar@leaflabs.com>
 * @brief  Maple PIN_MAP and boardInit().
 */

#include "gpio.h"
#include "timer.h"

#include "STM32VLD.h"

#ifdef BOARD_STM32VLD

void boardInit(void) {
}

extern const stm32_pin_info PIN_MAP[BOARD_NR_GPIO_PINS] = {

    {GPIOA, TIMER2, ADC1,  0, 1,    0}, /* D0/PA0(USR SW) */
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
    {GPIOA,   NULL, NULL, 11, 0, ADCx}, /* D11/PA11 */
    {GPIOA,   NULL, NULL, 12, 0, ADCx}, /* D12/PA12 */
    {GPIOA,   NULL, NULL, 13, 0, ADCx}, /* D13/PA13 */
    {GPIOA,   NULL, NULL, 14, 0, ADCx}, /* D14/PA14 */
    {GPIOA,   NULL, NULL, 15, 0, ADCx}, /* D15/PA15 */

    {GPIOB, TIMER3, ADC1,  0, 3,    8}, /* D16/PB0 */
    {GPIOB, TIMER3, ADC1,  1, 4,    9}, /* D17/PB1 */
    {GPIOB,   NULL, NULL,  2, 0, ADCx}, /* D18/PB2 */
    {GPIOB,   NULL, NULL,  3, 0, ADCx}, /* D19/PB3  */
    {GPIOB,   NULL, NULL,  4, 0, ADCx}, /* D20/PB4  */
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

    {GPIOC,   NULL, ADC1,  0, 0,   10}, /* D32/PC0 */
    {GPIOC,   NULL, ADC1,  1, 0,   11}, /* D33/PC1 */
    {GPIOC,   NULL, ADC1,  2, 0,   12}, /* D34/PC2 */
    {GPIOC,   NULL, ADC1,  3, 0,   13}, /* D35/PC3 */
    {GPIOC,   NULL, ADC1,  4, 0,   14}, /* D36/PC4 */
    {GPIOC,   NULL, ADC1,  5, 0,   15}, /* D37/PC5 */
    {GPIOC,   NULL, NULL,  6, 0, ADCx}, /* D38/PC6 */
    {GPIOC,   NULL, NULL,  7, 0, ADCx}, /* D39/PC7 */
    {GPIOC,   NULL, NULL,  8, 0, ADCx}, /* D40/PC8(LED/Blue) */
    {GPIOC,   NULL, NULL,  9, 0, ADCx}, /* D41/PC9(LED/Green) */
    {GPIOC,   NULL, NULL, 10, 0, ADCx}, /* D42/PC10 */
    {GPIOC,   NULL, NULL, 11, 0, ADCx}, /* D43/PC11 */
    {GPIOC,   NULL, NULL, 12, 0, ADCx}, /* D44/PC12 */
    {GPIOC,   NULL, NULL, 13, 0, ADCx}, /* D45/PC13 */
    {GPIOC,   NULL, NULL, 14, 0, ADCx}, /* D46/PC14 */
    {GPIOC,   NULL, NULL, 15, 0, ADCx}, /* D47/PC15 */

    {GPIOD,   NULL, NULL,  0, 0, ADCx}, /* D48/PD0 */
    {GPIOD,   NULL, NULL,  1, 0, ADCx}, /* D49/PD1 */
    {GPIOD,   NULL, NULL,  2, 0, ADCx}, /* D50/PD2 */

};


extern const uint8 boardPWMPins[] __FLASH__ = {
	PA(0), PA(1), PA(2), PA(3), PA(6), PA(7), PA(8), PA(9), PA(10), PB(0), PB(1), PB(6), PB(7), PB(8), PB(9)
	
};

extern const uint8 boardADCPins[] __FLASH__ = {
	PA(0), PA(1), PA(2), PA(3), PA(4), PA(5), PA(6), PA(7), PB(0), PB(1), PC(0), PC(1), PC(2), PC(3), PC(4), PC(5)
};


extern const uint8 boardUsedPins[] __FLASH__ = {
    BOARD_LED_PIN, BOARD_BUTTON_PIN, BOARD_JTMS_SWDIO_PIN,
    BOARD_JTCK_SWCLK_PIN, BOARD_JTDI_PIN, BOARD_JTDO_PIN, BOARD_NJTRST_PIN
};

#endif
