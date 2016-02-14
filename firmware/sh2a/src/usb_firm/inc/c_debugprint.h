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
 *       System Name : SH7264 USB Sample Program
 *       File Name   : c_debugprint.h
 *       Abstract    : debugprint Header File
 *       Version     : 1.00.00
 *       Device      : SH7264/SH7262
 *       Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
 *                   : C/C++ compiler package for the SuperH RISC engine family
 *                   :                             (Ver.9.02 Release00).
 *       OS          : None
 *       H/W Platform: M3A-HS64G50(CPU board)
 *
 *       History   : May.13.2009 ver.1.00.00
 *""FILE COMMENT END""**********************************************************/
#ifndef _C_DEBUGPRINT_H_
#define _C_DEBUGPRINT_H_
#if PRINT_MODE == PRINT_USE
#include <stdio.h>

extern void 		print_put_dmp(USB_U8 *Ptr, USB_U8 Size, USB_U16 Spc, USB_U16 Wid);
extern void			print_usb_busreset_end(USB_U16 Buf);
extern void			print_getconf_device_end(USB_U16 Stat);
extern void			print_getconf_config_end(USB_U16 Stat);
extern void			print_setaddress_end(USB_U16 Stat, USB_U16 Adr);
extern void			print_setconfiguration_end(USB_U16 Stat, USB_U16 Num);

#endif

#endif /* _C_DEBUGPRINT_H_ */

/* End of file */
