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
 *       File Name   : p_classvendor.c
 *       Abstract    : Peripheral Class/Vendor Request
 *       Version     : 1.00.00
 *       Device      : SH7264/SH7262
 *       Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
 *                   : C/C++ compiler package for the SuperH RISC engine family
 *                   :                             (Ver.9.02 Release00).
 *       OS          : None
 *       H/W Platform: M3A-HS64G50(CPU board)
 *
 *       History     : May.13.2009 ver.1.00.00
 *""FILE COMMENT END""**********************************************************/
#include "usb.h"

/* ==== �}�N����` ==== */
#define SEND_ENCAPSULATED_COMMAND	0x00
#define GET_ENCAPSULATED_RESPONSE	0x01
#define SET_COMM_FEATURE			0x02
#define GET_COMM_FEATURE			0x03
#define CLEAR_COMM_FEATURE			0x04
#define SET_LINE_CODING				0x20
#define GET_LINE_CODING				0x21
#define SET_CONTROL_LINE_STATE		0x22
#define SEND_BREAK					0x23

/* ==== �ϐ���` ==== */
/*	Line Coding Structure	*/
unsigned char		LineCodingItem[] = {
	0x00,0xC2,0x01,0x00,	/*	0-3	dwDTERate 	Bit Rate(bps)	Default : 0x1C200 (115200)	*/
	0x00,					/*	 4	bCharFormat Stop BIt		Default : 0 (1 Stop bit)	*/
	0x00,					/*	 5	bCharFormat Parity			Default : 0 (None )			*/
	0x08,					/*	 6	bCharFormat Data Bit		Default : 8					*/
	0x00					/*	 7	bCharFormat Flow Control	Default : 0 (SW or None)	*/
};

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Class request (idle or setup stage).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Class0(USB_U16 type, USB_U16 req, USB_U16 value, 
*             :                               USB_U16 index, USB_U16 length).
*----------------------------------------------------------------------------- 
* Description : USB�N���X���N�G�X�g�̃Z�b�g�A�b�v�X�e�[�W���������s���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type         ; bmRequestType
*             : USB_U16 req          ; bRequest
*             : USB_U16 value        ; wValue
*             : USB_U16 index        ; wIndex
*             : USB_U16 length       ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : Communication Device Class�̃��N�G�X�g�ɑΉ����Ă��܂��B
*""FUNC COMMENT END""*********************************************************/
void P_Class0(USB_U16 type, USB_U16 req, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_req, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_req		= req;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;
	/* User program */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Class request (control read data stage).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Class1(USB_U16 type, USB_U16 req, USB_U16 value, 
*             :                                  USB_U16 index, USB_U16 length)
*----------------------------------------------------------------------------- 
* Description : USB�N���X���N�G�X�g�̃��[�h�f�[�^�X�e�[�W���������s���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type         ; bmRequestType
*             : USB_U16 req          ; bRequest
*             : USB_U16 value        ; wValue
*             : USB_U16 index        ; wIndex
*             : USB_U16 length       ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : Communication Device Class�̃��N�G�X�g�ɑΉ����Ă��܂��B
*""FUNC COMMENT END""*********************************************************/
void P_Class1(USB_U16 type, USB_U16 req, USB_U16 value, USB_U16 index, USB_U16 length)
{
	switch( req ){
	case GET_LINE_CODING:
		P_CtrlReadStart((USB_U32)length, LineCodingItem);
		break;
		
	default:
		usb_set_pid_stall(PIPE0);
		break;
	}
}

void USB_set_linecoding_callback( int bitrate );
/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Class request (control write data stage).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Class2(USB_U16 type, USB_U16 req, USB_U16 value, 
*             :                                    USB_U16 index, USB_U16 length)
*----------------------------------------------------------------------------- 
* Description : USB�N���X���N�G�X�g�̃��C�g�f�[�^�X�e�[�W���������s���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type         ; bmRequestType
*             : USB_U16 req          ; bRequest
*             : USB_U16 value        ; wValue
*             : USB_U16 index        ; wIndex
*             : USB_U16 length       ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : Communication Device Class�̃��N�G�X�g�ɑΉ����Ă��܂��B
*""FUNC COMMENT END""*********************************************************/
void P_Class2(USB_U16 type, USB_U16 req, USB_U16 value, USB_U16 index, USB_U16 length)
{
	switch( req ){
	case SET_LINE_CODING:
		P_CtrlWriteStart((USB_U32)length, LineCodingItem);
		USB_set_linecoding_callback( 0 );	// unknown baudrate.
		break;
	case GET_LINE_CODING:
	case SET_CONTROL_LINE_STATE:
	case SEND_ENCAPSULATED_COMMAND:
	case GET_ENCAPSULATED_RESPONSE:
	case SET_COMM_FEATURE:
	case GET_COMM_FEATURE:
	case CLEAR_COMM_FEATURE:
	case SEND_BREAK:
	default:
		usb_set_pid_stall(PIPE0);
		break;
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Class request (control write nodata status stage).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Class3(USB_U16 type, USB_U16 req, USB_U16 value, 
*             :                                     USB_U16 index, USB_U16 length)
*----------------------------------------------------------------------------- 
* Description : USB�N���X���N�G�X�g�̃m�[�f�[�^�p�X�e�[�^�X�X�e�[�W���������s���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type         ; bmRequestType
*             : USB_U16 req          ; bRequest
*             : USB_U16 value        ; wValue
*             : USB_U16 index        ; wIndex
*             : USB_U16 length       ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : Communication Device Class�̃��N�G�X�g�ɑΉ����Ă��܂��B
*""FUNC COMMENT END""*********************************************************/
void P_Class3(USB_U16 type, USB_U16 req, USB_U16 value, USB_U16 index, USB_U16 length)
{
	switch( req ){
	case SET_CONTROL_LINE_STATE:
		usb_set_pid_buf(PIPE0);
		break;
	default:
		usb_set_pid_stall(PIPE0);
		break;
	}
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Class request (control read status stage).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Class4(USB_U16 type, USB_U16 req, USB_U16 value, 
*             :                                    USB_U16 index, USB_U16 length)
*----------------------------------------------------------------------------- 
* Description : USB�N���X���N�G�X�g�̃��[�h�f�[�^�p�X�e�[�^�X�X�e�[�W���������s���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type         ; bmRequestType
*             : USB_U16 req          ; bRequest
*             : USB_U16 value        ; wValue
*             : USB_U16 index        ; wIndex
*             : USB_U16 length       ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : Communication Device Class�̃��N�G�X�g�ɑΉ����Ă��܂��B
*""FUNC COMMENT END""*********************************************************/
void P_Class4(USB_U16 type, USB_U16 req, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_req, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_req		= req;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;
	/* User program */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Class request (control write status stage).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Class5(USB_U16 type, USB_U16 req, USB_U16 value, 
*             :                                   USB_U16 index, USB_U16 length)
*----------------------------------------------------------------------------- 
* Description : USB�N���X���N�G�X�g�̃��C�g�f�[�^�p�X�e�[�^�X�X�e�[�W���������s���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type         ; bmRequestType
*             : USB_U16 req          ; bRequest
*             : USB_U16 value        ; wValue
*             : USB_U16 index        ; wIndex
*             : USB_U16 length       ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : Communication Device Class�̃��N�G�X�g�ɑΉ����Ă��܂��B
*""FUNC COMMENT END""*********************************************************/
void P_Class5(USB_U16 type, USB_U16 req, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_req, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_req		= req;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;
	/* User program */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Vendor request (idle or setup stage).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Vendor0(USB_U16 type, USB_U16 req, USB_U16 value, 
*             :                                  USB_U16 index, USB_U16 length)
*----------------------------------------------------------------------------- 
* Description : USB�x���_���N�G�X�g�̃Z�b�g�A�b�v�X�e�[�W���������s���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type         ; bmRequestType
*             : USB_U16 req          ; bRequest
*             : USB_U16 value        ; wValue
*             : USB_U16 index        ; wIndex
*             : USB_U16 length       ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USB�T���v���h���C�o�ł�USB�x���_�������T�|�[�g���Ă��Ȃ��̂ŁA
*             : �������܂���B
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_Vendor0(USB_U16 type, USB_U16 req, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_req, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_req		= req;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;
	/* User program */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Vendor request (control read data stage).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Vendor1(USB_U16 type, USB_U16 req, USB_U16 value, 
*             :                                   USB_U16 index, USB_U16 length)
*----------------------------------------------------------------------------- 
* Description : USB�x���_���N�G�X�g�̃��[�h�f�[�^�X�e�[�W���������s���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type         ; bmRequestType
*             : USB_U16 req          ; bRequest
*             : USB_U16 value        ; wValue
*             : USB_U16 index        ; wIndex
*             : USB_U16 length       ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USB�T���v���h���C�o�ł�USB�x���_�������T�|�[�g���Ă��Ȃ��̂ŁA
*             : �������܂���B
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_Vendor1(USB_U16 type, USB_U16 req, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_req, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_req		= req;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;
	/* User program */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Vendor request (control write data stage).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Vendor2(USB_U16 type, USB_U16 req, USB_U16 value, 
*             :                                   USB_U16 index, USB_U16 length)
*----------------------------------------------------------------------------- 
* Description : USB�x���_���N�G�X�g�̃��C�g�f�[�^�X�e�[�W���������s���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type         ; bmRequestType
*             : USB_U16 req          ; bRequest
*             : USB_U16 value        ; wValue
*             : USB_U16 index        ; wIndex
*             : USB_U16 length       ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USB�T���v���h���C�o�ł�USB�x���_�������T�|�[�g���Ă��Ȃ��̂ŁA
*             : �������܂���B
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_Vendor2(USB_U16 type, USB_U16 req, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_req, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_req		= req;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;
	/* User program */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Vendor request (control write nodata status stage).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Vendor3(USB_U16 type, USB_U16 req, USB_U16 value, 
*             :                                     USB_U16 index, USB_U16 length)
*----------------------------------------------------------------------------- 
* Description : USB�x���_���N�G�X�g�̃m�[�f�[�^�p�X�e�[�^�X�X�e�[�W���������s���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type         ; bmRequestType
*             : USB_U16 req          ; bRequest
*             : USB_U16 value        ; wValue
*             : USB_U16 index        ; wIndex
*             : USB_U16 length       ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USB�T���v���h���C�o�ł�USB�x���_�������T�|�[�g���Ă��Ȃ��̂ŁA
*             : �������܂���B
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_Vendor3(USB_U16 type, USB_U16 req, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_req, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_req		= req;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;
	/* User program */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Vendor request (control read status stage).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Vendor4(USB_U16 type, USB_U16 req, USB_U16 value, 
*             :                                     USB_U16 index, USB_U16 length)
*----------------------------------------------------------------------------- 
* Description : USB�x���_���N�G�X�g�̃��[�h�f�[�^�p�X�e�[�^�X�X�e�[�W���������s���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type         ; bmRequestType
*             : USB_U16 req          ; bRequest
*             : USB_U16 value        ; wValue
*             : USB_U16 index        ; wIndex
*             : USB_U16 length       ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USB�T���v���h���C�o�ł�USB�x���_�������T�|�[�g���Ă��Ȃ��̂ŁA
*             : �������܂���B
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_Vendor4(USB_U16 type, USB_U16 req, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_req, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_req		= req;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;
	/* User program */
}

/*""FUNC COMMENT""************************************************************ 
* ID          : 
* Outline     : Vendor request (control write status stage).
*----------------------------------------------------------------------------- 
* Include     : 
*----------------------------------------------------------------------------- 
* Declaration : void P_Vendor5(USB_U16 type, USB_U16 req, USB_U16 value, 
*             :                                    USB_U16 index, USB_U16 length)
*----------------------------------------------------------------------------- 
* Description : USB�x���_���N�G�X�g�̃��C�g�f�[�^�p�X�e�[�^�X�X�e�[�W���������s���܂��B
*             : 
*----------------------------------------------------------------------------- 
* Argument    : USB_U16 type         ; bmRequestType
*             : USB_U16 req          ; bRequest
*             : USB_U16 value        ; wValue
*             : USB_U16 index        ; wIndex
*             : USB_U16 length       ; wLength
*----------------------------------------------------------------------------- 
* Return Value: void
*----------------------------------------------------------------------------- 
* Notice      : USB�T���v���h���C�o�ł�USB�x���_�������T�|�[�g���Ă��Ȃ��̂ŁA
*             : �������܂���B
*             : 
*""FUNC COMMENT END""*********************************************************/
void P_Vendor5(USB_U16 type, USB_U16 req, USB_U16 value, USB_U16 index, USB_U16 length)
{
	USB_U16 dummy_type, dummy_req, dummy_value, dummy_index, dummy_length;
	dummy_type		= type;
	dummy_req		= req;
	dummy_value		= value;
	dummy_index		= index;
	dummy_length	= length;
	/* User program */
}

/*===========================================================================*/
/* End of File */
