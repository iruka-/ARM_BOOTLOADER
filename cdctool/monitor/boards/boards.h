/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Bryan Newbold.
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
 * @file boards.h
 * @author Bryan Newbold <bnewbold@leaflabs.com>,
 *         Marti Bolivar <mbolivar@leaflabs.com>
 * @brief Board-specific pin information.
 *
 * To add a new board type, add a new pair of files to
 * /wirish/boards/, update the section below with a new "BOARD" type,
 * and update /wirish/rules.mk to include your boards/your_board.cpp
 * file in the top-level Makefile build.
 */

#ifndef _BOARDS_H_
#define _BOARDS_H_

#include "libmaple.h"

/* Include the appropriate private header from boards/: */

/* FIXME HACK put boards/ before these paths once IDE uses make. */

#ifdef BOARD_maple
#include "maple.h"
#elif defined(BOARD_maple_native)
#include "maple_native.h"
#elif defined(BOARD_maple_mini)
#include "maple_mini.h"
#elif defined(BOARD_maple_RET6)
/*
 * **NOT** MAPLE REV6.  This the **Maple RET6 EDITION**, which is a
 * Maple with an STM32F103RET6 (...RET6) instead of an STM32F103RBT6
 * (...RBT6) on it.  Maple Rev6 (as of March 2011) DOES NOT EXIST.
 */
#include "maple_RET6.h"
#elif defined(BOARD_STBee)
#include "STBee.h"
#elif defined(BOARD_STBee2)
#include "STBee2.h"
#elif defined(BOARD_STBeeMini)
#include "STBeeMini.h"
#elif defined(BOARD_STM32VLD)
#include "STM32VLD.h"
#elif defined(BOARD_CQSTARM)
#include "CQSTARM.h"
#elif defined(BOARD_DISCOVERY)
#include "DISCOVERY.h"
#else
/*
 * TODO turn this into a warning so people can:
 *
 * #include "my_board_config.h"
 * #include "wirish.h"
 *
 * This will enable third-party board support without requiring that
 * anybody hack around in libmaple itself.
 */
#error "Board type has not been selected correctly."
#endif

/* Set derived definitions */

#define CLOCK_SPEED_MHZ                 CYCLES_PER_MICROSECOND
#define CLOCK_SPEED_HZ                  (CLOCK_SPEED_MHZ * 1000000UL)

#endif
