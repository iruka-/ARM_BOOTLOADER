/******************************************************************************
*   DISCLAIMER
*
*   This software is supplied by Renesas Technology Corp. and is only 
*   intended for use with Renesas products. No other uses are authorized.
*
*   This software is owned by Renesas Technology Corp. and is protected under 
*   all applicable laws, including copyright laws.
*
*   THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
*   REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
*   INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
*   PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
*   DISCLAIMED.
*
*   TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
*   TECHNOLOGY CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
*   FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
*   FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
*   AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
*   Renesas reserves the right, without notice, to make changes to this 
*   software and to discontinue the availability of this software.
*   By using this software, you agree to the additional terms and 
*   conditions found by accessing the following link: 
*   http://www.renesas.com/disclaimer
********************************************************************************
*   Copyright (C) 2008(2009). Renesas Technology Corp., All Rights Reserved.
*""FILE COMMENT""*********** Technical reference data **************************
*   System Name : SH7264 Sample Program
*   File Name   : m3a_hs64.h
*   Abstract    : SH7264 CPU board hardware define
*   Version     : 1.01.00
*   Device      : SH7264
*   Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
*               : C/C++ compiler package for the SuperH RISC engine family
*               :                             (Ver.9.02 Release00).
*   OS          : None
*   H/W Platform: M3A-HS64G50(CPU board)
*   Description : 
********************************************************************************
*   History     : Oct.02,2008 Ver.1.00.00
*               : Jul.01,2009 Ver.1.01.00 Changed FILE FORMAT
*""FILE COMMENT END""**********************************************************/
#ifndef _M3A_HS64_H_
#define _M3A_HS64_H_

/* ==== macros ==== */
#define BOARD_NAME	"SH7264 CPU BOARD"


/* ---- LED ID ---- */
#define ID_LED1		1
#define ID_LED2		2
#define ID_LED_ALL	(ID_LED1 | ID_LED2)

#ifndef NULL
#define NULL ((void*)0)
#endif

/* ==== prototype ==== */

/* ---- /src/common/io_init_port.c ---- */
void io_init_port(void);

#if	0
/* ---- /src/common/led.c ---- */
void led_init(void);
void led_on(unsigned short ledno);
void led_off(unsigned short ledno);
#endif

#endif /* _M3A_HS64_H_ */

/* End of File */
