/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Technology Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Technology Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* TECHNOLOGY CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
********************************************************************************
* Copyright (C) 2009. Renesas Technology Corp., All Rights Reserved.           *
********************************************************************************/

/*""FILE COMMENT""*********** Technical reference data *************************
 *
 *       System Name : SH7264 USB FUNCTION Sample Program
 *       File Name   : main.c
 *       Abstract    : Sample Program Main
 *       Version     : 1.03.00
 *       Device      : SH7264/SH7262
 *       Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
 *                   : C/C++ compiler package for the SuperH RISC engine family
 *                   :                             (Ver.9.02 Release00).
 *       OS          : None
 *       H/W Platform: M3A-HS64G50(CPU board)
 *
 *       History   : May.13.2009 ver.1.00.00
 *                 : Jul.06.2009 ver.1.01.00 Changed common directory
 *                 : Jul.13.2009 ver.1.02.00 Tool-Chain 9.2.0.0
 *                 : Nov.10.2009 ver.1.03.00 Changed common directory
 *""FILE COMMENT END""**********************************************************/
#include "includes.h"

/* ==== prototype declaration ==== */
extern	void USB_loop(void);	// usbmain.c
extern	void DEBUG_init(void);
extern	void USBIOinit(void);
extern	void io_init_scif2();	/* SCIF2シリアル */

static	void USB_setup(void)
{
	USBIOinit();
	P_ModeInit();		/* USB関連初期化 */
	io_init_scif2();	/* SCIF2シリアル */
	DEBUG_init();		/* LED初期化等	 */
}

/*""FUNC COMMENT""*******************************************************
 * Outline     : main
 *-----------------------------------------------------------------------
 * Include     : 
 *-----------------------------------------------------------------------
 * Declaration : void main(void);
 *-----------------------------------------------------------------------
 * Description : 
 *-----------------------------------------------------------------------
 * Argument    : void
 *-----------------------------------------------------------------------
 * Return Value: void
 *-----------------------------------------------------------------------
 * Notice      : 
 *""FUNC COMMENT END""***************************************************/
int main(void)
{
	USB_setup();
	while(1) {
		USB_loop();
	}
	return 0;
}

/* End of File */
