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
 *       File Name   : c_libint.c
 *       Abstract    : USB interrupt register routine
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
* Outline     : Enable Buffer Ready Interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_enable_brdy_int(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプのBRDY割り込みを許可します。
*             : BRDYを許可する前に、一度すべてのパイプのBEMP/NRDY/BRDY
*             : 割り込みを禁止し、禁止状態で引数にて指定したパイプの
*             : BRDY割り込みを許可します。BRDY許可後、すべてのパイプの
*             : BEMP/NRDY/BRDY禁止/許可状態を元に戻します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe           ; pipe Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_enable_brdy_int(USB_U16 pipe)
{
	USB_U16 tmp;
	USB_U16 buf;

	tmp = USB.INTENB0.WORD;						/* Value Save */
	USB.INTENB0.WORD = (USB_U16)(tmp & (USB_U16)~(BITBEMPE| BITNRDYE|BITBRDYE));
	buf = USB.BRDYENB.WORD;
	buf |= BitSet[pipe];
	USB.BRDYENB.WORD = buf;
	USB.INTENB0.WORD = tmp;						/* Interrupt Enable */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Disable Buffer Ready Interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_disable_brdy_int(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプのBRDY割り込みを禁止します。
*             : BRDYを禁止する前に、一度すべてのパイプのBEMP/NRDY/BRDY
*             : 割り込みを禁止し、禁止状態で引数にて指定したパイプの
*             : BRDY割り込みを禁止します。BRDY禁止後、すべてのパイプの
*             : BEMP/NRDY/BRDY禁止/許可状態を元に戻します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe        ; pipe Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_disable_brdy_int(USB_U16 pipe)
{
	USB_U16  tmp;
	USB_U16	buf;

	tmp = USB.INTENB0.WORD;						/* Value Save */
	USB.INTENB0.WORD = (USB_U16)(tmp & (USB_U16)~(BITBEMPE| BITNRDYE|BITBRDYE));
	buf = USB.BRDYENB.WORD;
	buf &= (USB_U16)~(BitSet[pipe]);
	USB.BRDYENB.WORD = buf;
	USB.INTENB0.WORD = tmp;						/* Interrupt Enable */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Enable Buffer Empty Interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_enable_bemp_int(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプのBEMP割り込みを許可します。
*             : BEMPを許可する前に、一度すべてのパイプのBEMP/NRDY/BRDY
*             : 割り込みを禁止し、禁止状態で引数にて指定したパイプの
*             : BEMP割り込みを許可します。BEMP許可後、すべてのパイプの
*             : BEMP/NRDY/BRDY禁止/許可状態を元に戻します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe           ; pipe Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_enable_bemp_int(USB_U16 pipe)
{
	USB_U16 tmp;
	USB_U16 buf;

	tmp = USB.INTENB0.WORD;						/* Value Save */
	USB.INTENB0.WORD = (USB_U16)(tmp	& (USB_U16)~(BITBEMPE| BITNRDYE|BITBRDYE));
	buf = USB.BEMPENB.WORD;
	buf |= BitSet[pipe];
	USB.BEMPENB.WORD = buf;
	USB.INTENB0.WORD = tmp;						/* Interrupt Enable */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Disable Buffer Empty Interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_disable_bemp_int(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプのBEMP割り込みを禁止します。
*             : BEMPを禁止する前に、一度すべてのパイプのBEMP/NRDY/BRDY
*             : 割り込みを禁止し、禁止状態で引数にて指定したパイプの
*             : BEMP割り込みを禁止します。BEMP禁止後、すべてのパイプの
*             : BEMP/NRDY/BRDY禁止/許可状態を元に戻します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe           ; pipe Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_disable_bemp_int(USB_U16 pipe)
{
	USB_U16  tmp;
	USB_U16 buf;

	tmp = USB.INTENB0.WORD;						/* Value Save */
	USB.INTENB0.WORD = (USB_U16)(tmp	& (USB_U16)~(BITBEMPE| BITNRDYE|BITBRDYE));
	buf = USB.BEMPENB.WORD;
	buf &= (USB_U16)~(BitSet[pipe]);
	USB.BEMPENB.WORD = buf;
	USB.INTENB0.WORD = tmp;						/* Interrupt Enable */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Enable Buffer Not Ready Interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_enable_nrdy_int(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプのNRDY割り込みを許可します。
*             : NRDYを許可する前に、一度すべてのパイプのBEMP/NRDY/BRDY
*             : 割り込みを禁止し、禁止状態で引数にて指定したパイプの
*             : NRDY割り込みを許可します。NRDY許可後、すべてのパイプの
*             : BEMP/NRDY/BRDY禁止/許可状態を元に戻します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe             ; pipe Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_enable_nrdy_int(USB_U16 pipe)
{
	USB_U16  tmp;
	USB_U16	buf;

	tmp = USB.INTENB0.WORD;						/* Value Save */
	USB.INTENB0.WORD = (USB_U16)(tmp	& (USB_U16)~(BITBEMPE| BITNRDYE|BITBRDYE));
	buf = USB.NRDYENB.WORD;
	buf |= BitSet[pipe];
	USB.NRDYENB.WORD = buf;
	USB.INTENB0.WORD = tmp;						/* Interrupt Enable */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Disable Buffer Not Ready Interrupt
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_disable_nrdy_int(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプのNRDY割り込みを禁止します。
*             : NRDYを禁止する前に、一度すべてのパイプのBEMP/NRDY/BRDY
*             : 割り込みを禁止し、禁止状態で引数にて指定したパイプの
*             : NRDY割り込みを禁止します。NRDY禁止後、すべてのパイプの
*             : BEMP/NRDY/BRDY禁止/許可状態を元に戻します。
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe            ; pipe Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_disable_nrdy_int(USB_U16 pipe)
{
	USB_U16 tmp;
	USB_U16 buf;

	tmp = USB.INTENB0.WORD;						/* Value Save */
	USB.INTENB0.WORD = (USB_U16)(tmp	& (USB_U16)~(BITBEMPE| BITNRDYE|BITBRDYE));
	buf = USB.NRDYENB.WORD;
	buf &= (USB_U16)~(BitSet[pipe]);
	USB.NRDYENB.WORD = buf;
	USB.INTENB0.WORD = tmp;						/* Interrupt Enable */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Check Current Speed
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_is_hispeed( void );
*----------------------------------------------------------------------------- 
* Description : USBリセットハンドシェイクの結果(RHST)を戻り値として返します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: HI_SPEED    ; Hi-Speed
*             : FULL_SPEED  ; Full-Speed
*             : LOW_SPEED   ; Low-Speed
*             : NON_SPEED   ; error
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_is_hispeed( void )
{
	USB_U16 rhst,speed;

	rhst = USB.DVSTCTR.BIT.RHST;
	if( rhst == HSMODE ) {
		speed = HI_SPEED;									/* Hi-Speed mode */
	}
	else if( rhst == FSMODE ) {
		speed = FULL_SPEED;									/* Full-Speed mode */
	}
	else if( rhst == LSMODE ) {
		speed = LOW_SPEED;									/* Low-Speed mode */
	}
	else{
		speed = NON_SPEED;									/* error */
	}
	return speed;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Get Current Speed
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_get_bus_speed(void);
*----------------------------------------------------------------------------- 
* Description : USBリセットハンドシェイクの結果(RHST)を戻り値として返します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: RHST bit value(DVSTCTR).
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_get_bus_speed(void)
{
	return (USB_U16)USB.DVSTCTR.BIT.RHST;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Check Hi-Speed enable bit
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_is_hispeed_enable(void);
*----------------------------------------------------------------------------- 
* Description : USB High-Speed接続許可状態を戻り値として返します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : void
*----------------------------------------------------------------------------- 
* Return Value: USB_YES : Hi-Speed Enable
*             : USB_NO  : Hi-Speed Disable
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_is_hispeed_enable(void)
{
	USB_U16 ret = USB_NO;
	
	if( USB.SYSCFG.BIT.HSE == 1 ) {
		ret = USB_YES;									/* Hi-Speed Enable */
	}
	return ret;
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Set pipe PID_BUF
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_set_pid_buf(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプの通信可能状態(BUF)にします。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe             ; pipe Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_set_pid_buf(USB_U16 pipe)
{
	USB_U16		pid;

	pid = usb_get_pid( pipe );
	if( pid == PID_STALL2 ){
		/* When it makes the BUF state from the PID＝STALL(11) state, */
		/* write 01 after writing 00 once.	*/
		usb_set_pid_nak( pipe );
	}

	switch( pipe ) {
	case PIPE0:
		USB.DCPCTR.BIT.PID = PID_BUF;
		break;
	case PIPE1:
		USB.PIPE1CTR.BIT.PID = PID_BUF;
		break;
	case PIPE2:
		USB.PIPE2CTR.BIT.PID = PID_BUF;
		break;
	case PIPE3:
		USB.PIPE3CTR.BIT.PID = PID_BUF;
		break;
	case PIPE4:
		USB.PIPE4CTR.BIT.PID = PID_BUF;
		break;
	case PIPE5:
		USB.PIPE5CTR.BIT.PID = PID_BUF;
		break;
	case PIPE6:
		USB.PIPE6CTR.BIT.PID = PID_BUF;
		break;
	case PIPE7:
		USB.PIPE7CTR.BIT.PID = PID_BUF;
		break;
	case PIPE8:
		USB.PIPE8CTR.BIT.PID = PID_BUF;
		break;
	case PIPE9:
		USB.PIPE9CTR.BIT.PID = PID_BUF;
		break;
	default:
		break;
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Set pipe PID_NAK
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_set_pid_nak(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプの通信禁止状態(NAK)にします。
*             : 本関数実行前のパイプ状態が通信可能状態(BUF)である場合は、
*             : 通信禁止設定後、パイプがレディーとなるまでソフトウェアにて待ちます。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe            ; pipe Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_set_pid_nak(USB_U16 pipe)
{
	USB_U16		pid, pbusy;

	pid = usb_get_pid( pipe );
	if( pid == PID_STALL2 ){
		/* When it makes the NAK state from the PID＝STALL(11) state, */
		/* write 00 after writing 10 once.	*/
		usb_set_pid_stall( pipe );
	}

	switch( pipe ) {
	case PIPE0:
		USB.DCPCTR.BIT.PID = PID_NAK;
		break;
	case PIPE1:
		USB.PIPE1CTR.BIT.PID = PID_NAK;
		break;
	case PIPE2:
		USB.PIPE2CTR.BIT.PID = PID_NAK;
		break;
	case PIPE3:
		USB.PIPE3CTR.BIT.PID = PID_NAK;
		break;
	case PIPE4:
		USB.PIPE4CTR.BIT.PID = PID_NAK;
		break;
	case PIPE5:
		USB.PIPE5CTR.BIT.PID = PID_NAK;
		break;
	case PIPE6:
		USB.PIPE6CTR.BIT.PID = PID_NAK;
		break;
	case PIPE7:
		USB.PIPE7CTR.BIT.PID = PID_NAK;
		break;
	case PIPE8:
		USB.PIPE8CTR.BIT.PID = PID_NAK;
		break;
	case PIPE9:
		USB.PIPE9CTR.BIT.PID = PID_NAK;
		break;
	default:
		break;
	}

	if( pid == PID_BUF ){
		do {
			switch( pipe ) {
			case PIPE0:
				pbusy = USB.DCPCTR.BIT.PBUSY;
				break;
			case PIPE1:
				pbusy = USB.PIPE1CTR.BIT.PBUSY;
				break;
			case PIPE2:
				pbusy = USB.PIPE2CTR.BIT.PBUSY;
				break;
			case PIPE3:
				pbusy = USB.PIPE3CTR.BIT.PBUSY;
				break;
			case PIPE4:
				pbusy = USB.PIPE4CTR.BIT.PBUSY;
				break;
			case PIPE5:
				pbusy = USB.PIPE5CTR.BIT.PBUSY;
				break;
			case PIPE6:
				pbusy = USB.PIPE6CTR.BIT.PBUSY;
				break;
			case PIPE7:
				pbusy = USB.PIPE7CTR.BIT.PBUSY;
				break;
			case PIPE8:
				pbusy = USB.PIPE8CTR.BIT.PBUSY;
				break;
			case PIPE9:
				pbusy = USB.PIPE9CTR.BIT.PBUSY;
				break;
			default:
				pbusy	= 1;
				break;
			}
		} while( pbusy == 1 );
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Set pipe PID_TALL
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void SetTALL(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプの通信禁止状態(STALL)にします。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe            ; pipe Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_set_pid_stall(USB_U16 pipe)
{
	USB_U16		pid;

	pid = usb_get_pid( pipe );
	if( pid == PID_BUF ){
		/* When it makes the STALL state from the PID＝STALL(01) state, */
		/* write 11.	*/

		switch( pipe ) {
		case PIPE0:
			USB.DCPCTR.BIT.PID = PID_STALL2;
			break;
		case PIPE1:
			USB.PIPE1CTR.BIT.PID = PID_STALL2;
			break;
		case PIPE2:
			USB.PIPE2CTR.BIT.PID = PID_STALL2;
			break;
		case PIPE3:
			USB.PIPE3CTR.BIT.PID = PID_STALL2;
			break;
		case PIPE4:
			USB.PIPE4CTR.BIT.PID = PID_STALL2;
			break;
		case PIPE5:
			USB.PIPE5CTR.BIT.PID = PID_STALL2;
			break;
		case PIPE6:
			USB.PIPE6CTR.BIT.PID = PID_STALL2;
			break;
		case PIPE7:
			USB.PIPE7CTR.BIT.PID = PID_STALL2;
			break;
		case PIPE8:
			USB.PIPE8CTR.BIT.PID = PID_STALL2;
			break;
		case PIPE9:
			USB.PIPE9CTR.BIT.PID = PID_STALL2;
			break;
		default:
			break;
		}
	}
	else{
		switch( pipe ) {
		case PIPE0:
			USB.DCPCTR.BIT.PID = PID_STALL;
			break;
		case PIPE1:
			USB.PIPE1CTR.BIT.PID = PID_STALL;
			break;
		case PIPE2:
			USB.PIPE2CTR.BIT.PID = PID_STALL;
			break;
		case PIPE3:
			USB.PIPE3CTR.BIT.PID = PID_STALL;
			break;
		case PIPE4:
			USB.PIPE4CTR.BIT.PID = PID_STALL;
			break;
		case PIPE5:
			USB.PIPE5CTR.BIT.PID = PID_STALL;
			break;
		case PIPE6:
			USB.PIPE6CTR.BIT.PID = PID_STALL;
			break;
		case PIPE7:
			USB.PIPE7CTR.BIT.PID = PID_STALL;
			break;
		case PIPE8:
			USB.PIPE8CTR.BIT.PID = PID_STALL;
			break;
		case PIPE9:
			USB.PIPE9CTR.BIT.PID = PID_STALL;
			break;
		default:
			break;
		}
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Clear pipe PID_STALL
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void ClearSTALL(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプの通信禁止状態(NAK)にします。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe            ; pipe Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_clear_pid_stall(USB_U16 pipe)
{
	usb_set_pid_nak( pipe );
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Get pipe PID
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_get_pid(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプの状態を戻り値として返します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe          ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: PID
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_get_pid(USB_U16 pipe)
{
	USB_U16 pid;

	switch( pipe ) {
	case PIPE0:
		pid = USB.DCPCTR.BIT.PID;
		break;
	case PIPE1:
		pid = USB.PIPE1CTR.BIT.PID;
		break;
	case PIPE2:
		pid = USB.PIPE2CTR.BIT.PID;
		break;
	case PIPE3:
		pid = USB.PIPE3CTR.BIT.PID;
		break;
	case PIPE4:
		pid = USB.PIPE4CTR.BIT.PID;
		break;
	case PIPE5:
		pid = USB.PIPE5CTR.BIT.PID;
		break;
	case PIPE6:
		pid = USB.PIPE6CTR.BIT.PID;
		break;
	case PIPE7:
		pid = USB.PIPE7CTR.BIT.PID;
		break;
	case PIPE8:
		pid = USB.PIPE8CTR.BIT.PID;
		break;
	case PIPE9:
		pid = USB.PIPE9CTR.BIT.PID;
		break;
	default:
		pid = 0;
		break;
	}
	
	return pid;
}

/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     : CSPLIT status clear setting of sprit transaction 
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : void usb_set_csclr( USB_U16 pipe );
*-----------------------------------------------------------------------------
* Description : CSPLIT status clear setting of sprit transaction in specified 
*             : pipe is performed.
*-----------------------------------------------------------------------------
* Argument    : USB_U16 pipe     ; Pipe number 
*-----------------------------------------------------------------------------
* Return Value: void
*-----------------------------------------------------------------------------
* Note        : When SQSET bit or SQCLR bit, and SQSET bit or SQCLR bit 
*             : in DCPCTR register are continuously changed (when the sequence
*             : toggle bit of data PID is continuously changed over two or more pipes), 
*             : the access cycle with 120 ns and more than 5 cycle bus clock is necessary.
*             : Do not set both SQCLR bit and SQSET bit to 1 at the same time. 
*             : In addition, both bits should be operated after PID is set to NAK. 
*             : However, when it is set to the isochronous transfer as the transfer type
*             : (TYPE=11), writing in SQSET bit is disabled.
*""FUNC COMMENT END""********************************************************/
void	usb_set_csclr( USB_U16 pipe )
{
	switch( pipe ) {
	case PIPE0:
		USB.DCPCTR.BIT.CSCLR = 1;
		break;
	case PIPE1:
		USB.PIPE1CTR.BIT.CSCLR = 1;
		break;
	case PIPE2:
		USB.PIPE2CTR.BIT.CSCLR = 1;
		break;
	case PIPE3:
		USB.PIPE3CTR.BIT.CSCLR = 1;
		break;
	case PIPE4:
		USB.PIPE4CTR.BIT.CSCLR = 1;
		break;
	case PIPE5:
		USB.PIPE5CTR.BIT.CSCLR = 1;
		break;
	case PIPE6:
		USB.PIPE6CTR.BIT.CSCLR = 1;
		break;
	case PIPE7:
		USB.PIPE7CTR.BIT.CSCLR = 1;
		break;
	case PIPE8:
		USB.PIPE8CTR.BIT.CSCLR = 1;
		break;
	case PIPE9:
		USB.PIPE9CTR.BIT.CSCLR = 1;
		break;
	default:
		break;
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Do SQCLR
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void usb_set_sqclr(USB_U16 pipe);
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプのシーケンスビットをDATA0に設定します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe              ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
void usb_set_sqclr(USB_U16 pipe)
{
	switch( pipe ) {
	case PIPE0:
		USB.DCPCTR.BIT.SQCLR = 1;
		break;
	case PIPE1:
		USB.PIPE1CTR.BIT.SQCLR = 1;
		break;
	case PIPE2:
		USB.PIPE2CTR.BIT.SQCLR = 1;
		break;
	case PIPE3:
		USB.PIPE3CTR.BIT.SQCLR = 1;
		break;
	case PIPE4:
		USB.PIPE4CTR.BIT.SQCLR = 1;
		break;
	case PIPE5:
		USB.PIPE5CTR.BIT.SQCLR = 1;
		break;
	case PIPE6:
		USB.PIPE6CTR.BIT.SQCLR = 1;
		break;
	case PIPE7:
		USB.PIPE7CTR.BIT.SQCLR = 1;
		break;
	case PIPE8:
		USB.PIPE8CTR.BIT.SQCLR = 1;
		break;
	case PIPE9:
		USB.PIPE9CTR.BIT.SQCLR = 1;
		break;
	default:
		break;
	}
}

/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     : Pipe toggle bit set setting 
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : void usb_set_sqset( USB_U16 pipe );
*-----------------------------------------------------------------------------
* Description : 引数にて指定したパイプのシーケンスビットをDATA1に設定します。
*-----------------------------------------------------------------------------
* Argument    : USB_U16 pipe   ; Pipe number 
*             : 
*-----------------------------------------------------------------------------
* Return Value: void
*-----------------------------------------------------------------------------
* Note        : 
*""FUNC COMMENT END""********************************************************/
void usb_set_sqset( USB_U16 pipe )
{
	switch( pipe ) {
	case PIPE0:
		USB.DCPCTR.BIT.SQSET = 1;
		break;
	case PIPE1:
		USB.PIPE1CTR.BIT.SQSET = 1;
		break;
	case PIPE2:
		USB.PIPE2CTR.BIT.SQSET = 1;
		break;
	case PIPE3:
		USB.PIPE3CTR.BIT.SQSET = 1;
		break;
	case PIPE4:
		USB.PIPE4CTR.BIT.SQSET = 1;
		break;
	case PIPE5:
		USB.PIPE5CTR.BIT.SQSET = 1;
		break;
	case PIPE6:
		USB.PIPE6CTR.BIT.SQSET = 1;
		break;
	case PIPE7:
		USB.PIPE7CTR.BIT.SQSET = 1;
		break;
	case PIPE8:
		USB.PIPE8CTR.BIT.SQSET = 1;
		break;
	case PIPE9:
		USB.PIPE9CTR.BIT.SQSET = 1;
		break;
	default:
		break;
	}
}

/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     : Obtaining pipe toggle bit 
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : USB_U16 usb_get_sqmon( USB_U16 pipe );
*-----------------------------------------------------------------------------
* Description : Toggle bit of specified pipe is obtained 
*-----------------------------------------------------------------------------
* Argument    : USB_U16 pipe   ; Pipe number 
*             : 
*-----------------------------------------------------------------------------
* Return Value: void
*-----------------------------------------------------------------------------
* Note        : 
*""FUNC COMMENT END""********************************************************/
USB_U16 usb_get_sqmon( USB_U16 pipe )
{
	USB_U16 sqmon;
	
	switch( pipe ) {
	case PIPE0:
		sqmon = USB.DCPCTR.BIT.SQMON;
		break;
	case PIPE1:
		sqmon = USB.PIPE1CTR.BIT.SQMON;
		break;
	case PIPE2:
		sqmon = USB.PIPE2CTR.BIT.SQMON;
		break;
	case PIPE3:
		sqmon = USB.PIPE3CTR.BIT.SQMON;
		break;
	case PIPE4:
		sqmon = USB.PIPE4CTR.BIT.SQMON;
		break;
	case PIPE5:
		sqmon = USB.PIPE5CTR.BIT.SQMON;
		break;
	case PIPE6:
		sqmon = USB.PIPE6CTR.BIT.SQMON;
		break;
	case PIPE7:
		sqmon = USB.PIPE7CTR.BIT.SQMON;
		break;
	case PIPE8:
		sqmon = USB.PIPE8CTR.BIT.SQMON;
		break;
	case PIPE9:
		sqmon = USB.PIPE9CTR.BIT.SQMON;
		break;
	default:
		sqmon = 0;
		break;
	}
	return sqmon;
}

/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     : FIFO buffer initialization 
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : void usb_aclrm( USB_U16 pipe );
*-----------------------------------------------------------------------------
* Description : The buffer of specified pipe is initialized 
*             : 
*-----------------------------------------------------------------------------
* Argument    : USB_U16 pipe    : Pipe
*-----------------------------------------------------------------------------
* Return Value: void
*-----------------------------------------------------------------------------
* Note        : 
*""FUNC COMMENT END""********************************************************/
void	usb_aclrm( USB_U16 pipe )
{
	usb_set_aclrm( pipe );
	usb_clr_aclrm( pipe );
}

/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     : Enabling auto buffer clear mode 
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : void usb_set_aclrm( USB_U16 pipe );
*-----------------------------------------------------------------------------
* Description : The auto buffer clear mode of specified pipe is enabled
*             : 
*-----------------------------------------------------------------------------
* Argument    : USB_U16 pipe    : Pipe
*-----------------------------------------------------------------------------
* Return Value: void
*-----------------------------------------------------------------------------
* Note        : 
*""FUNC COMMENT END""********************************************************/
void	usb_set_aclrm( USB_U16 pipe )
{
	switch( pipe ) {
		case PIPE0: 
			break;
		case PIPE1: 
			USB.PIPE1CTR.BIT.ACLRM = 1;	
			break;
		case PIPE2: 
			USB.PIPE2CTR.BIT.ACLRM = 1;	
			break;
		case PIPE3: 
			USB.PIPE3CTR.BIT.ACLRM = 1;	
			break;
		case PIPE4: 
			USB.PIPE4CTR.BIT.ACLRM = 1;	
			break;
		case PIPE5: 
			USB.PIPE5CTR.BIT.ACLRM = 1;	
			break;
		case PIPE6: 
			USB.PIPE6CTR.BIT.ACLRM = 1;	
			break;
		case PIPE7: 
			USB.PIPE7CTR.BIT.ACLRM = 1;	
			break;
		case PIPE8: 
			USB.PIPE8CTR.BIT.ACLRM = 1;	
			break;
		case PIPE9: 
			USB.PIPE9CTR.BIT.ACLRM = 1;	
			break;
		default:
			break;
	}
}

/*""FUNC COMMENT""*************************************************************
* ID          : 
* Outline     : Auto buffer clear mode is disabled 
*-----------------------------------------------------------------------------
* Include     : 
*-----------------------------------------------------------------------------
* Declaration : void usb_clr_aclrm( USB_U16 pipe );
*-----------------------------------------------------------------------------
* Description : The auto buffer clear mode of specified pipe is enabled 
*             : 
*-----------------------------------------------------------------------------
* Argument    : USB_U16 pipe    : Pipe
*-----------------------------------------------------------------------------
* Return Value: void
*-----------------------------------------------------------------------------
* Note        : 
*""FUNC COMMENT END""********************************************************/
void	usb_clr_aclrm( USB_U16 pipe )
{
	switch( pipe ) {
		case PIPE0: 
			break;
		case PIPE1: 
			USB.PIPE1CTR.BIT.ACLRM = 0;	
			break;
		case PIPE2: 
			USB.PIPE2CTR.BIT.ACLRM = 0;	
			break;
		case PIPE3: 
			USB.PIPE3CTR.BIT.ACLRM = 0;	
			break;
		case PIPE4: 
			USB.PIPE4CTR.BIT.ACLRM = 0;	
			break;
		case PIPE5: 
			USB.PIPE5CTR.BIT.ACLRM = 0;	
			break;
		case PIPE6: 
			USB.PIPE6CTR.BIT.ACLRM = 0;	
			break;
		case PIPE7: 
			USB.PIPE7CTR.BIT.ACLRM = 0;	
			break;
		case PIPE8: 
			USB.PIPE8CTR.BIT.ACLRM = 0;	
			break;
		case PIPE9: 
			USB.PIPE9CTR.BIT.ACLRM = 0;	
			break;
		default:
			break;
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Get INBUFM
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : USB_U16 usb_get_inbuf( USB_U16 pipe );
*----------------------------------------------------------------------------- 
* Description : 引数にて指定したパイプのINBUFMを戻り値として返します。
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 pipe             ; Pipe Number
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : 
*             : 
*""FUNC COMMENT END""*********************************************************/
USB_U16 usb_get_inbuf( USB_U16 pipe )
{
	USB_U16	inbuf;

	switch( pipe ) {
	case PIPE0:
		inbuf = 0;
		break;
	case PIPE1:
		inbuf = USB.PIPE1CTR.BIT.INBUFM;
		break;
	case PIPE2:
		inbuf = USB.PIPE2CTR.BIT.INBUFM;
		break;
	case PIPE3:
		inbuf = USB.PIPE3CTR.BIT.INBUFM;
		break;
	case PIPE4:
		inbuf = USB.PIPE4CTR.BIT.INBUFM;
		break;
	case PIPE5:
		inbuf = USB.PIPE5CTR.BIT.INBUFM;
		break;
	case PIPE6:
		inbuf = 0;
		break;
	case PIPE7:
		inbuf = 0;
		break;
	case PIPE8:
		inbuf = 0;
		break;
	case PIPE9:
		inbuf = 0;
		break;
	default:
		inbuf = 0;
		break;
	}
	return inbuf;
}

/* End of File */
