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
 *       System Name : SH7264 USB FUCNTION Sample Program
 *       File Name   : usb_firm.h
 *       Abstract    : Define USB Module Value
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
#ifndef _USB_FIRM_H_
#define _USB_FIRM_H_

/********** type define (COMMON) ********************************************/
typedef unsigned char	USB_U8;
typedef unsigned short	USB_U16;
typedef unsigned long	USB_U32;

typedef char			USB_S8;
typedef short			USB_S16;
typedef long			USB_S32;

typedef struct {
	USB_U16				pipe_number;
	USB_U16				pipe_cfg;
	USB_U16				pipe_buf;
	USB_U16				pipe_max_pktsize;
	USB_U16				pipe_cycle;
	USB_U16				fifo_port;
} USB_CFG_PIPETBL;

/********** macro define (COMMON) ********************************************/
#define	USB_ON							(1)
#define	USB_OFF							(0)
#define	USB_YES							(1)
#define USB_NO							(0)
#define	USB_SUCCESS						 0
#define	USB_ERROR						(-1)

#include "cfg_usb_sh7264.h"
#include "c_def7264.h"

#if PRINT_MODE == PRINT_USE
#include "c_debugprint.h"
#endif

/*========== USB specification define ==========*/
/* Descriptor type	Define */
#define	DT_DEVICE						(USB_U8)0x01	/* Configuration Descriptor */
#define	DT_CONFIGURATION				(USB_U8)0x02	/* Configuration Descriptor */
#define	DT_STRING						(USB_U8)0x03	/* Configuration Descriptor */
#define	DT_INTERFACE					(USB_U8)0x04	/* Interface Descriptor */
#define	DT_ENDPOINT						(USB_U8)0x05	/* Endpoint Descriptor */
#define	DT_DEVICE_QUALIFIER				(USB_U8)0x06	/* Device Qualifier Descriptor */
#define	DT_OTHER_SPEED_CONFIGURATION	(USB_U8)0x07	/* Other Speed Configuration Descriptor */
#define	DT_INTERFACE_POWER				(USB_U8)0x08	/* Interface Power Descriptor */

/* Device class Define  */
#define	DEVCLSINTF						(USB_U8)0x00	/* Class information at interface */
#define	DEVCLSCOMM						(USB_U8)0x02	/* Communication Device */
#define	DEVCLSHUB						(USB_U8)0x90	/* HUB Device */
#define	DEVCLSDIAG						(USB_U8)0xDC	/* Diagnostic Device */
#define	DEVCLSWIRE						(USB_U8)0xE0	/* Wireless Controller */
#define	DEVCLSAPL						(USB_U8)0xFE	/* Application-Specific */
#define	DEVCLSVEN						(USB_U8)0xFF	/* Vendor-Specific */

/* Interface class Define */
#define	INTFCLSNOT						(USB_U8)0x00	/* Un corresponding Class */
#define	INTFCLSAUD						(USB_U8)0x01	/* Audio Class */
#define	INTFCLSCDCC						(USB_U8)0x02	/* CDC-Control Class */
#define	INTFCLSHID						(USB_U8)0x03	/* HID Class */
#define	INTFCLSPHY						(USB_U8)0x05	/* Physical Class */
#define	INTFCLSIMG						(USB_U8)0x06	/* Image Class */
#define	INTFCLSPRN						(USB_U8)0x07	/* Printer Class */
#define	INTFCLSMAS						(USB_U8)0x08	/* Mass Storage Class */
#define	INTFCLSHUB						(USB_U8)0x09	/* HUB Class */
#define	INTFCLSCDCD						(USB_U8)0x0A	/* CDC-Data Class */
#define	INTFCLSCHIP						(USB_U8)0x0B	/* Chip/Smart Card Class */
#define	INTFCLSCNT						(USB_U8)0x0C	/* Content-Sequrity Class */
#define	INTFCLSVID						(USB_U8)0x0D	/* Video Class */
#define	INTFCLSDIAG						(USB_U8)0xDC	/* Diagnostic Device */
#define	INTFCLSWIRE						(USB_U8)0xE0	/* Wireless Controller */
#define	INTFCLSAPL						(USB_U8)0xFE	/* Application-Specific */
#define	INTFCLSVEN						(USB_U8)0xFF	/* Vendor-Specific Class */

/* USB Standard request */
#define	GET_STATUS						0x0000u
#define	CLEAR_FEATURE					0x0100u
#define	ReqRESERVED						0x0200u
#define	SET_FEATURE						0x0300u
#define	ReqRESERVED1					0x0400u
#define	SET_ADDRESS						0x0500u
#define	GET_DESCRIPTOR					0x0600u
#define	SET_DESCRIPTOR					0x0700u
#define	GET_CONFIGURATION				0x0800u
#define	SET_CONFIGURATION				0x0900u
#define	GET_INTERFACE					0x0A00u
#define	SET_INTERFACE					0x0B00u
#define	SYNCH_FRAME						0x0C00u
#define	HOST_TO_DEVICE					0x0000u
#define	DEVICE_TO_HOST					0x0080u
#define	STANDARD						0x0000u
#define	CLASS							0x0020u
#define	VENDOR							0x0040u
#define	DEVICE							0x0000u
#define	INTERFACE						0x0001u
#define	ENDPOINT						0x0002u

/* GET_STATUS request information */
/* Standard Device status */
#define	GS_BUSPOWERD					0x0000u
#define	GS_SELFPOWERD					0x0001u
#define	GS_REMOTEWAKEUP					0x0002u

/* Endpoint status */
#define	GS_NOTHALT						0x0000u
#define	GS_HALT							0x0001u

/* CLEAR_FEATURE/GET_FEATURE/SET_FEATURE request information */
/* Standard Feature Selector */
#define	ENDPOINT_HALT					0x0000u
#define	DEVICE_REMOTE_WAKEUP			0x0001u
#define	TEST_MODE						0x0002u

/* GET_DESCRIPTOR/SET_DESCRIPTOR request information */
/* Standard Descriptor type */
#define	DEVICEDESCRIPTOR						0x0100u
#define	CONFIGURATIONDESCRIPTOR					0x0200u
#define	STRINGDESCRIPTOR						0x0300u
#define	INTERFACEDESCRIPTOR						0x0400u
#define	ENDPOINTDESCRIPTOR						0x0500u
#define	DEVICE_QUALIFIERDESCRIPTOR				0x0600u
#define	OTHER_SPEED_CONFIGURATIONDESCRIPTOR		0x0700u
#define	INTERFACE_POWERDESCRIPTOR				0x0800u

/* Pipe define */
#define	MAX_PIPE_NO			9u					/* PIPE0 ... PIPE9 */
#define	PIPE0				(USB_U16)0x0000		/* PIPE 0 */
#define	PIPE1				(USB_U16)0x0001		/* PIPE 1 */
#define	PIPE2				(USB_U16)0x0002		/* PIPE 2 */
#define	PIPE3				(USB_U16)0x0003		/* PIPE 3 */
#define	PIPE4				(USB_U16)0x0004		/* PIPE 4 */
#define	PIPE5				(USB_U16)0x0005		/* PIPE 5 */
#define	PIPE6				(USB_U16)0x0006		/* PIPE 6 */
#define	PIPE7				(USB_U16)0x0007		/* PIPE 7 */
#define	PIPE8				(USB_U16)0x0008		/* PIPE 8 */
#define	PIPE9				(USB_U16)0x0009		/* PIPE 9 */
#define	NOPIPE			 	(USB_U16)0x00FF		/* Undecided pipe number */
#define	USEPIPE			 	(USB_U16)0x00FE

/* Pipe configuration table define */
#define	EPL					5u					/* Pipe configuration table length */

#define USB_NONE			(0u)

/* ----- Pipe Status ----- */
#define	PIPE_IDLE					0x00
#define	PIPE_WAIT					0x01
#define	PIPE_DONE					0x02
#define	PIPE_NORES					0x03
#define	PIPE_STALL					0x04
#define	PIPE_ERROR					0x05

#define	BUF_READY					0
#define	BUF_BUSY					1

#define	NON_SPEED					(0)
#define	LOW_SPEED					(1)
#define	FULL_SPEED					(2)
#define	HI_SPEED					(3)

#define	FIFO_USE					0x7000
#define	C_FIFO_USE					0x0000
#define	D0_FIFO_USE					0x1000
#define	D1_FIFO_USE					0x2000

#if	1
#define	D0_FIFO_DMA					0x5000
#define	D1_FIFO_DMA					0x6000
#endif

#define	USB_BUF2FIFO				0
#define	USB_FIFO2BUF				1

#define	BUF_SIZE(x)			(USB_U16)((USB_U16)(((x) / 64) - 1) << 10)
#define	BUF_NUMB(x)			(USB_U16)(x)

/* Device connect information */
#define	ATTACH				(USB_U16)0x01
#define	DETACH				(USB_U16)0x00

/* FIFO port & access define */
#define	CUSE				(USB_U16)0		/* CFIFO CPU transfer */
#define	D0USE				(USB_U16)1		/* D0FIFO CPU transfer */
//#define	D0DMA				(USB_U16)2		/* D0FIFO DMA transfer */
#define	D1USE				(USB_U16)3		/* D1FIFO CPU transfer */
//#define	D1DMA				(USB_U16)4		/* D1FIFO DMA transfer */

/********** macro define (HOST) **********************************************/

/* ----- DeviceState ----- */
#define	DVST_DETACHED				0x0000
#define	DVST_ATTACHED				0x0001
#define	DVST_GET_DESCRIPTOR_64		0x0002
#define	DVST_POWERED				0x0003
#define	DVST_DEFAULT				0x0004
#define	DVST_SET_ADDRESS			0x0005
#define	DVST_ADDRESS				0x0006
#define	DVST_GET_DESCRIPTOR_18		0x0007
#define	DVST_GET_DESCRIPTOR_9		0x0008
#define	DVST_GET_DESCRIPTOR_SIZE	0x0009
#define	DVST_SET_PIPE				0x000A
#define	DVST_SET_CONFIGURATION		0x000B
#define	DVST_CONFIGURED				0x000C
#define	DVST_DETACH_CHK				0x000D
#define	DVST_SUSPEND				0x1000
#define	DVST_NORES					0x0100
#define	DVST_STALL					0x0200

#define	DVST_POWERED_SUSPEND		(DVST_SUSPEND | DVST_POWERED)
#define	DVST_DEFAULT_SUSPEND		(DVST_SUSPEND | DVST_DEFAULT)
#define	DVST_ADDRESS_SUSPEND		(DVST_SUSPEND | DVST_ADDRESS)
#define	DVST_CONFIGURED_SUSPEND		(DVST_SUSPEND | DVST_CONFIGURED)

/********** macro define (PERIPHERAL) ****************************************/
/* Configuration Descriptor  DEFINE */
#define	CF_RESERVED					0x80			/* Reserved(set to 1) */
#define	CF_SELF						0x40			/* Self Powered */
#define	CF_RWUP						0x20			/* Remote Wakeup */

/* Endpoint Descriptor	DEFINE */
#define	EP_DIR_MASK					0x80			/* Endpoint Direction	*/
#define	EP_OUT						0x00			/* Out Endpoint */
#define	EP_IN						0x80			/* In  Endpoint */
#define	EP_CNTRL					0x00			/* Control	   Transfer */
#define	EP_ISO						0x01			/* Isochronous Transfer */
#define	EP_BULK						0x02			/* Bulk		   Transfer */
#define	EP_INT						0x03			/* Interrupt   Transfer */
#define	EP_TYPE						0x03			/* Transfer    Type		*/
#define	EP_NUM_MASK					0x0F			/* Endpoint Direction	*/

/**/
#define	DEV_MAX_PKT_SIZE			7				/* index of bMAXPacketSize */
#define	DEV_NUM_CONFIG				17				/* index of bNumConfigurations */

#define	ALT_NO						255

#define	EP_ERROR					0xFF
#define	SOFTWARE_CHANGE				0

/* FIFO read / write result */
#define	FIFOERROR	(USB_U16)USB_ERROR				/* FIFO not ready */
#define	WRITEEND	(USB_U16)0x00		/* end of write ( but packet may not be outputting ) */
#define	WRITESHRT	(USB_U16)0x01		/* end of write ( send short packet ) */
#define	WRITING		(USB_U16)0x02		/* write continues */
#define	WRITEDMA	(USB_U16)0x03		/* DMA write  */
#define	READEND		(USB_U16)0x00		/* end of read */
#define	READSHRT	(USB_U16)0x01		/* insufficient ( receive short packet ) */
#define	READING		(USB_U16)0x02		/* read continues */
#define	READOVER	(USB_U16)0x03		/* buffer size over */

/********** global valiable (COMMON) *****************************************/
/******* declarations external reference variable **********************/
extern USB_U8 P1_Buff[];
extern USB_U8 P2_Buff[];
extern USB_U8 P3_Buff[];
extern USB_U8 P4_Buff[];
extern USB_U8 P5_Buff[];
extern USB_U8 P6_Buff[];
extern USB_U8 P7_Buff[];
extern USB_U8 P8_Buff[];
extern USB_U8 P9_Buff[];

extern USB_U16 PipeIgnore[];					/* Ignore count */
extern USB_U16 PipeTbl[];
extern USB_U16 PipeFlag[];
extern USB_U32 PipeDataSize[];

extern USB_U8 DeviceDescriptor[];
extern USB_U8 QualifierDescriptor[];
extern USB_U8 Configuration_Full_1[];
extern USB_U8 Configuration_Full_2[];
extern USB_U8 Configuration_Full_3[];
extern USB_U8 Configuration_Full_4[];
extern USB_U8 Configuration_Full_5[];
extern USB_U8 Configuration_Hi_1[];
extern USB_U8 Configuration_Hi_2[];
extern USB_U8 Configuration_Hi_3[];
extern USB_U8 Configuration_Hi_4[];
extern USB_U8 Configuration_Hi_5[];

extern USB_U8 *ConPtr[];
extern USB_U8 *ConPtr_Other[];

extern USB_U8 StringDescriptor0[];
extern USB_U8 StringDescriptor1[];
extern USB_U8 StringDescriptor2[];
extern USB_U8 *StrPtr[];

/* ==== global function (COMMON) ==== */
void	usb_interrupt(void);
USB_U16 usb_get_bus_speed(void);
void	usb_start_recieve_transfer(USB_U16 Pipe, USB_U32 Bsize, USB_U8 *Tbl);
USB_U16 usb_start_send_transfer(USB_U16 Pipe, USB_U32 Bsize, USB_U8 *Tbl);
USB_U16 usb_get_pid(USB_U16 pipe);

/* ==== global function (PERIPHERAL) ==== */
void	P_ModeInit(void);
USB_U16 P_GetDeviceState(void);
USB_U16 P_IsConfigured(void);

void	P_UserApplication(void);
void	P_DataIN(USB_U16 Pipe, USB_U32 Size, USB_U8 *Tbl);
void	P_DataOUT(USB_U16 Pipe, USB_U32 Size, USB_U8 *Tbl);

USB_U16 P_CtrlReadStart(USB_U32 Bsize, USB_U8 *Table);	/* start Control Read */
void 	P_CtrlWriteStart(USB_U32 Bsize, USB_U8 *Table);	/* start Control Write */

#endif /* _USB_FIRM_H_ */

/* End of File */
