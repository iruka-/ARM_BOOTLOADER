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
 *       File Name   : p_changeep.c
 *       Abstract    : Peripheral Set Configration/Interface
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
#include "usb.h"

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Change configuration
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_ChangeConfigCB(USB_U16 cfg_num);
*----------------------------------------------------------------------------- 
* Description : USBコンフィギュレーションを変更します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 cfg_num      ; Configuration Number
*----------------------------------------------------------------------------- 
* Retrn Value: void
*----------------------------------------------------------------------------- 
* Notice      : USBサンプルドライバではコンフィギュレーション変更をサポートして
*             : いないため、何もしません。
*""FUNC COMMENT END""*********************************************************/
void P_ChangeConfigCB(USB_U16 cfg_num)
{
	USB_U16	dummy;
	dummy	= cfg_num;
	/* User program */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Change interface
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_ChangeInterfaceCB(USB_U16 cfg_num,
*             :                          USB_U16 itf_num,
*             :                          USB_U16 alt_num);
*----------------------------------------------------------------------------- 
* Description : USBインタフェースを変更します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 cfg_num      ; Configuration Number
*             : USB_U16 itf_num      ; Interface Number
*             : USB_U16 alt_num      ; Alternate Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USBサンプルドライバではインタフェース変更をサポートして
*             : いないため、何もしません。
*""FUNC COMMENT END""*********************************************************/
void P_ChangeInterfaceCB(USB_U16 cfg_num, USB_U16 itf_num, USB_U16 alt_num)
{
	USB_U16	dummy_cfg,dummy_itf, dummy_alt;
	dummy_cfg	= cfg_num;
	dummy_itf	= itf_num;
	dummy_alt	= alt_num;
	/* User program */
}

/*===========================================================================*/
/* End of File */
