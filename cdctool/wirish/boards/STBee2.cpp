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


#ifdef BOARD_STBee2

#include "STBee2.h"

#if defined(INIT_SRAM)

#include "fsmc.h"
#include "gpio.h"
#include "rcc.h"
#include "timer.h"
#include "wirish_types.h"

void initSRAMChip(void);

void boardInit(void) {
    initSRAMChip();
}

#else


#include "gpio.h"
#include "wirish_debug.h"

void boardInit(void) {
}

#endif

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
    {GPIOA, TIMER1, NULL, 11, 4, ADCx}, /* D11/PA11(USB D-) */
    {GPIOA, TIMER1, NULL, 12, 5, ADCx}, /* D12/PA12(USB D+) */
    {GPIOA,   NULL, NULL, 13, 0, ADCx}, /* D13/PA13 */
    {GPIOA,   NULL, NULL, 14, 0, ADCx}, /* D14/PA14 */
    {GPIOA,   NULL, NULL, 15, 0, ADCx}, /* D15/PA15 */

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

    {GPIOC,   NULL, ADC1,  0, 0,   10}, /* D32/PC0 */
    {GPIOC,   NULL, ADC1,  1, 0,   11}, /* D33/PC1 */
    {GPIOC,   NULL, ADC1,  2, 0,   12}, /* D34/PC2 */
    {GPIOC,   NULL, ADC1,  3, 0,   13}, /* D35/PC3 */
    {GPIOC,   NULL, ADC1,  4, 0,   14}, /* D36/PC4 */
    {GPIOC,   NULL, ADC1,  5, 0,   15}, /* D37/PC5 */
    {GPIOC,   NULL, NULL,  6, 0, ADCx}, /* D38/PC6 */
    {GPIOC,   NULL, NULL,  7, 0, ADCx}, /* D39/PC7 */
    {GPIOC,   NULL, NULL,  8, 0, ADCx}, /* D40/PC8 */
    {GPIOC,   NULL, NULL,  9, 0, ADCx}, /* D41/PC9 */
    {GPIOC,   NULL, NULL, 10, 0, ADCx}, /* D42/PC10 */
    {GPIOC,   NULL, NULL, 11, 0, ADCx}, /* D43/PC11 */
    {GPIOC,   NULL, NULL, 12, 0, ADCx}, /* D44/PC12 */
    {GPIOC,   NULL, NULL, 13, 0, ADCx}, /* D45/PC13 */
    {GPIOC,   NULL, NULL, 14, 0, ADCx}, /* D46/PC14 */
    {GPIOC,   NULL, NULL, 15, 0, ADCx}, /* D47/PC15 */

    {GPIOD,   NULL, NULL,  0, 0, ADCx}, /* D48/PD0 */
    {GPIOD,   NULL, NULL,  1, 0, ADCx}, /* D49/PD1 */
    {GPIOD,   NULL, NULL,  2, 0, ADCx}, /* D50/PD2 */
    {GPIOD,   NULL, NULL,  3, 0, ADCx}, /* D51/PD3(USB DISC) */
    {GPIOD,   NULL, NULL,  4, 0, ADCx}, /* D52/PD4(LED) */
	{GPIOD,   NULL, NULL,  5, 0, ADCx}, /* D53/PD5 */
    {GPIOD,   NULL, NULL,  6, 0, ADCx}, /* D54/PD6 */
    {GPIOD,   NULL, NULL,  7, 0, ADCx}, /* D55/PD7 */
    {GPIOD,   NULL, NULL,  8, 0, ADCx}, /* D56/PD8 */
    {GPIOD,   NULL, NULL,  9, 0, ADCx}, /* D57/PD9 */
    {GPIOD,   NULL, NULL, 10, 0, ADCx}, /* D58/PD10 */
    {GPIOD,   NULL, NULL, 11, 0, ADCx}, /* D59/PD11 */
    {GPIOD,   NULL, NULL, 12, 0, ADCx}, /* D60/PD12 */
    {GPIOD,   NULL, NULL, 13, 0, ADCx}, /* D61/PD13 */
    {GPIOD,   NULL, NULL, 14, 0, ADCx}, /* D62/PD14 */
    {GPIOD,   NULL, NULL, 15, 0, ADCx}, /* D63/PD15 */

    {GPIOE,   NULL, NULL,  0, 0, ADCx}, /* D64/PE0 */
    {GPIOE,   NULL, NULL,  1, 0, ADCx}, /* D65/PE1 */
    {GPIOE,   NULL, NULL,  2, 0, ADCx}, /* D66/PE2 */
    {GPIOE,   NULL, NULL,  3, 0, ADCx}, /* D67/PE3 */
    {GPIOE,   NULL, NULL,  4, 0, ADCx}, /* D68/PE4 */
    {GPIOE,   NULL, NULL,  5, 0, ADCx}, /* D69/PE5 */
    {GPIOE,   NULL, NULL,  6, 0, ADCx}, /* D70/PE6 */
    {GPIOE,   NULL, NULL,  7, 0, ADCx}, /* D71/PE7 */
    {GPIOE,   NULL, NULL,  8, 0, ADCx}, /* D72/PE8 */
    {GPIOE,   NULL, NULL,  9, 0, ADCx}, /* D73/PE9 */
    {GPIOE,   NULL, NULL, 10, 0, ADCx}, /* D74/PE10 */
    {GPIOE,   NULL, NULL, 11, 0, ADCx}, /* D75/PE11 */
    {GPIOE,   NULL, NULL, 12, 0, ADCx}, /* D76/PE12 */
    {GPIOE,   NULL, NULL, 13, 0, ADCx}, /* D77/PE13 */
    {GPIOE,   NULL, NULL, 14, 0, ADCx}, /* D78/PE14 */
    {GPIOE,   NULL, NULL, 15, 0, ADCx}, /* D79/PE15 */

#if defined(STM32_HIGH_DENSITY)
    {GPIOF,   NULL, NULL,  0, 0, ADCx}, /* D80/PF0 */
    {GPIOF,   NULL, NULL,  1, 0, ADCx}, /* D81/PF1 */
    {GPIOF,   NULL, NULL,  2, 0, ADCx}, /* D82/PF2 */
    {GPIOF,   NULL, NULL,  3, 0, ADCx}, /* D83/PF3 */
    {GPIOF,   NULL, NULL,  4, 0, ADCx}, /* D84/PF4 */
    {GPIOF,   NULL, NULL,  5, 0, ADCx}, /* D85/PF5 */
    {GPIOF,   NULL, NULL,  6, 0, ADCx}, /* D86/PF6 */
    {GPIOF,   NULL, NULL,  7, 0, ADCx}, /* D87/PF7 */
    {GPIOF,   NULL, NULL,  8, 0, ADCx}, /* D88/PF8 */
    {GPIOF,   NULL, NULL,  9, 0, ADCx}, /* D89/PF9 */
    {GPIOF,   NULL, NULL, 10, 0, ADCx}, /* D90/PF10 */
    {GPIOF,   NULL, NULL, 11, 0, ADCx}, /* D91/PF11 */
    {GPIOF,   NULL, NULL, 12, 0, ADCx}, /* D92/PF12 */
    {GPIOF,   NULL, NULL, 13, 0, ADCx}, /* D92/PF13 */
    {GPIOF,   NULL, NULL, 14, 0, ADCx}, /* D94/PF14 */
    {GPIOF,   NULL, NULL, 15, 0, ADCx}, /* D95/PF15 */

    {GPIOG,   NULL, NULL,  0, 0, ADCx}, /* D96/PG0 */
    {GPIOG,   NULL, NULL,  1, 0, ADCx}, /* D97/PG1 */
    {GPIOG,   NULL, NULL,  2, 0, ADCx}, /* D98/PG2 */
    {GPIOG,   NULL, NULL,  3, 0, ADCx}, /* D99/PG3 */
    {GPIOG,   NULL, NULL,  4, 0, ADCx}, /* D100/PG4 */
    {GPIOG,   NULL, NULL,  5, 0, ADCx}, /* D101/PG5 */
    {GPIOG,   NULL, NULL,  6, 0, ADCx}, /* D102/PG6 */
    {GPIOG,   NULL, NULL,  7, 0, ADCx}, /* D103/PG7 */
    {GPIOG,   NULL, NULL,  8, 0, ADCx}, /* D104/PG8 */
    {GPIOG,   NULL, NULL,  9, 0, ADCx}, /* D105/PG9 */
    {GPIOG,   NULL, NULL, 10, 0, ADCx}, /* D106/PG10 */
    {GPIOG,   NULL, NULL, 11, 0, ADCx}, /* D107/PG11 */
    {GPIOG,   NULL, NULL, 12, 0, ADCx}, /* D108/PG12 */
    {GPIOG,   NULL, NULL, 13, 0, ADCx}, /* D109/PG13 */
    {GPIOG,   NULL, NULL, 14, 0, ADCx}, /* D110/PG14 */
    {GPIOG,   NULL, NULL, 15, 0, ADCx}, /* D111/PG15 */
#endif

};


extern const uint8 boardPWMPins[BOARD_NR_PWM_PINS] __FLASH__ = {
	PA(0), PA(1), PA(2), PA(3), PA(6), PA(7), PA(8), PA(9), PA(10), PB(0), PB(1), PB(6), PB(7), PB(8), PB(9)
};

extern const uint8 boardADCPins[BOARD_NR_ADC_PINS] __FLASH__ = {
	PA(0), PA(1), PA(2), PA(3), PA(4), PA(5), PA(6), PA(7), PB(0), PB(1), PC(0), PC(1), PC(2), PC(3), PC(4), PC(5)
};


#if defined(INIT_SRAM)

/* FIXME [0.0.12] see comment by BOARD_NR_USED_PINS in maple_native.h */
extern const uint8 boardUsedPins[BOARD_NR_USED_PINS] __FLASH__ = {
    BOARD_LED_PIN, BOARD_BUTTON_PIN, BOARD_JTMS_SWDIO_PIN,
    BOARD_JTCK_SWCLK_PIN, BOARD_JTDI_PIN, BOARD_JTDO_PIN, BOARD_NJTRST_PIN
};

void initSRAMChip(void) {
    fsmc_nor_psram_reg_map *regs = FSMC_NOR_PSRAM1_BASE;

    fsmc_sram_init_gpios();
    rcc_clk_enable(RCC_FSMC);

    regs->BCR = FSMC_BCR_WREN | FSMC_BCR_MWID_16BITS | FSMC_BCR_MBKEN;
    fsmc_nor_psram_set_addset(regs, 0);
    fsmc_nor_psram_set_datast(regs, 3);
}

#else

#define USB_DP PA(11)
#define USB_DM PA(12)

extern const uint8 boardUsedPins[BOARD_NR_USED_PINS] __FLASH__ = {
    BOARD_LED_PIN, BOARD_BUTTON_PIN, USB_DP, USB_DM
};
#endif


#endif
