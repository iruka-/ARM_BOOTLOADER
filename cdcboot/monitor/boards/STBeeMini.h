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
 * @file   maple_mini.h
 * @author Marti Bolivar <mbolivar@leaflabs.com>
 * @brief  Private include file for Maple Mini in boards.h
 *
 * See maple.h for more information on these definitions.
 */

#ifndef _BOARD_STBEEMINI_H_
#define _BOARD_STBEEMINI_H_

//----------------------------------------
// macro for port number definition
#define PA(n) (n)
#define PB(n) (16+(n))
#define PC(n) (32+(n))
//#define PC13  32
//#define PC14  33
//#define PC15  34
//-----------------------------------------


#define CYCLES_PER_MICROSECOND    72
#define SYSTICK_RELOAD_VAL     71999 /* takes a cycle to reload */

#define BOARD_BUTTON_PIN          PC13
#define BOARD_LED_PIN             PA(13) // we have two LEDs(OA13, PA15)

#define BOARD_NR_USARTS         3
#define BOARD_USART1_TX_PIN     PA(9)
#define BOARD_USART1_RX_PIN     PA(10)
#define BOARD_USART2_TX_PIN     PA(2)
#define BOARD_USART2_RX_PIN     PA(3)
#define BOARD_USART3_TX_PIN     PB(10)
#define BOARD_USART3_RX_PIN     PB(11)

#define BOARD_NR_SPI              2
#define BOARD_SPI1_NSS_PIN      PA(4)
#define BOARD_SPI1_MOSI_PIN     PA(7)
#define BOARD_SPI1_MISO_PIN     PA(6)
#define BOARD_SPI1_SCK_PIN      PA(5)
#define BOARD_SPI2_NSS_PIN      PB(12)
#define BOARD_SPI2_MOSI_PIN     PB(15)
#define BOARD_SPI2_MISO_PIN     PB(14)
#define BOARD_SPI2_SCK_PIN      PB(13)

#define BOARD_NR_GPIO_PINS        36
#define BOARD_NR_PWM_PINS         12
#define BOARD_NR_ADC_PINS         10
#define BOARD_NR_USED_PINS         4

#define BOARD_JTMS_SWDIO_PIN    PA(13)
#define BOARD_JTCK_SWCLK_PIN    PA(14)
#define BOARD_JTDI_PIN          PA(15)
#define BOARD_JTDO_PIN          PB(3)
#define BOARD_NJTRST_PIN        PB(4)


#endif
