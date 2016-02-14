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
 *       File Name   : usb.h
 *       Abstract    : USB driver header file
 *       Version     : 1.01.00
 *       Device      : SH7264/SH7262
 *       Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
 *                   : C/C++ compiler package for the SuperH RISC engine family
 *                   :                             (Ver.9.02 Release00).
 *       OS          : None
 *       H/W Platform: M3A-HS64G50(CPU board)
 *
 *       History   : May.13.2009 ver.1.00.00
 *                 : Sep.16.2009 ver.1.01.00
 *""FILE COMMENT END""**********************************************************/
#ifndef __USB_H__
#define __USB_H__

#include "usb_firm.h"
#include "iodefine.h"

/* ********** Type Declaration for user definition function  ********** */
/* Type Declaration for DMA control user defined function argument 		*/
#define	USB_D0FIFO 			0
#define	USB_D1FIFO 			1
#define	USB_DMA_READY 		0
#define	USB_DMA_BUSY 		1

typedef struct{
	USB_U32 fifo;				/* FIFO 		 			*/
	USB_U32 buffer;				/* buffer address 			*/
	USB_U32 bytes;				/* data byte 				*/
	USB_U32 dir;				/* direction 				*/
	USB_U32 size;				/* size per 1 DMA transfer	*/
}USB_DMA;

#ifndef __GLOBAL_C__

/*========== USB specification define ==========*/

/********** macro define (HOST) **********************************************/
/* ----- UsbCmdStage State ----- */
#define	CMD_IDLE					0x0000
#define	CMD_DOING					0x0001
#define	CMD_DONE					0x0002
#define	CMD_NORES					0x0003
#define	CMD_STALL					0x0004
#define	CMD_FIELD					0x000F
#define	CHG_CMD_FIELD( r, v )		do { r &= ( ~CMD_FIELD );	\
										 r |= v;				} while(0)

#define	MODE_WRITE					0x0100
#define	MODE_READ					0x0200
#define	MODE_NO_DATA				0x0300
#define	MODE_FIELD					0x0F00
#define	CHG_MODE_FIELD( r, v )		do { r &= ( ~MODE_FIELD );	\
										 r |= v;				} while(0)

#define	STAGE_SETUP					0x0010
#define	STAGE_DATA					0x0020
#define	STAGE_STATUS				0x0030
#define	STAGE_FIELD					0x00F0
#define	CHG_STAGE_FIELD( r, v )		do { r &= ( ~STAGE_FIELD );	\
										 r |= v;				} while(0)

/********** macro define (PERIPHERAL) ****************************************/

/********** global valiable (COMMON) *****************************************/

extern USB_U32		USBDataCount[];			/* PIPEn Buffer counter */
extern USB_U32		USBRcvDataCount[];		/* PIPEn receive data counter */
extern USB_U8		*USBDataPointer[];		/* PIPEn Buffer pointer */

extern USB_U16		ConfigNum;				/* configuration Number */

extern const USB_U16		BitSet[];

extern USB_DMA			USBDmaInfo[];		/* DMA Setting Infomation */
extern USB_U16			USBDmaPipe[];		/* DMA Transfar Pipe */
extern USB_U16			USBDmaVal[];		/* VAL(FIFOCTR) set flag after dma end */
extern USB_U16			USBDmaStatus[];
/********** global valiable (PERIPHERAL) *************************************/

extern void			(*pbRequest0[])(USB_U16 Reqtype, USB_U16 Value, USB_U16 Index, USB_U16 Length);
extern void			(*pbRequest1[])(USB_U16 Reqtype, USB_U16 Value, USB_U16 Index, USB_U16 Length);
extern void			(*pbRequest2[])(USB_U16 Reqtype, USB_U16 Value, USB_U16 Index, USB_U16 Length);
extern void			(*pbRequest3[])(USB_U16 Reqtype, USB_U16 Value, USB_U16 Index, USB_U16 Length);
extern void			(*pbRequest4[])(USB_U16 Reqtype, USB_U16 Value, USB_U16 Index, USB_U16 Length);
extern void			(*pbRequest5[])(USB_U16 Reqtype, USB_U16 Value, USB_U16 Index, USB_U16 Length);

extern USB_U16		Alternate[];
extern USB_U16		RemoteWakeupFlag;				/* Remote Wakeup Enable Flag */
extern USB_U16		TestModeFlag;
extern USB_U16		TestModeSelectors;
extern USB_U16		EPTableIndex[];					/* Index of EP Information table */
extern USB_U16		ReqType;						/* request type */
extern USB_U16		ReqTypeType;					/* request type TYPE */
extern USB_U16		ReqTypeRecip;					/* request type RECIPIENT */
extern USB_U16		ReqRequest;						/* request */
extern USB_U16		ReqValue;						/* value */
extern USB_U16		ReqIndex;						/* index */
extern USB_U16		ReqLength;						/* length */

#endif /* __C_GLOBAL_C__ */

/* ==== global function (COMMON) ==== */
/* ---- c_dataio.c ---- */
USB_U16 	usb_start_send_transfer(USB_U16 pipe, USB_U32 size, USB_U8 *data);
USB_U16 	usb_write_buffer(USB_U16 pipe);
USB_U16 	usb_write_buffer_c(USB_U16 pipe);
USB_U16 	usb_write_buffer_d0(USB_U16 pipe);
USB_U16 	usb_write_buffer_d1(USB_U16 pipe);
void 		usb_start_recieve_transfer(USB_U16 pipe, USB_U32 size, USB_U8 *data);
USB_U16 	usb_read_buffer(USB_U16 pipe);
USB_U16 	usb_read_buffer_c(USB_U16 pipe);
USB_U16 	usb_read_buffer_d0(USB_U16 pipe);
USB_U16 	usb_read_buffer_d1(USB_U16 pipe);
USB_U16 	usb_change_fifo_port(USB_U16 pipe, USB_U16 fifosel, 
								USB_U16 isel, USB_U16 mbw);
void 		usb_set_curpipe(USB_U16 pipe, USB_U16 fifosel, USB_U16 isel, USB_U16 mbw);
USB_U16		usb_get_mbw(USB_U32 trncount,USB_U32 dtptr);
USB_U16 	usb_read_dma(USB_U16 pipe);
void 		usb_stop_transfer( USB_U16 pipe );

/* ---- c_global.c ---- */
void 	usb_init_mem(void);

/* ---- intrn.c ---- */
void	usb_brdy_int(USB_U16 status, USB_U16 int_enb);
void	usb_nrdy_int(USB_U16 status, USB_U16 int_enb);
void	usb_bemp_int(USB_U16 status, USB_U16 int_enb);

/* ---- c_lib7264.c ---- */
void	usb_set_int_level(USB_U8 level);
void	usb_reset_module(void);
USB_U16 usb_get_buf_size(USB_U16 pipe);
USB_U16 usb_get_mxps(USB_U16 pipe);

/* ---- c_libint.c ---- */
void 		usb_enable_brdy_int(USB_U16 pipe);
void 		usb_disable_brdy_int(USB_U16 pipe);
void 		usb_enable_bemp_int(USB_U16 pipe);
void 		usb_disable_bemp_int(USB_U16 pipe);
void 		usb_enable_nrdy_int(USB_U16 pipe);
void 		usb_disable_nrdy_int(USB_U16 pipe);
USB_U16 	usb_is_hispeed( void);
USB_U16 	usb_is_hispeed_enable( void );
void 		usb_set_pid_buf(USB_U16 pipe);
void 		usb_set_pid_nak(USB_U16 pipe);
void 		usb_set_pid_stall(USB_U16 pipe);
void 		usb_clear_pid_stall(USB_U16 pipe);
USB_U16 	usb_get_pid(USB_U16 pipe);
void 		usb_set_sqclr(USB_U16 pipe);
void 		usb_set_sqset(USB_U16 pipe);
void 		usb_set_csclr(USB_U16 pipe);
void 		usb_aclrm(USB_U16 pipe);
void 		usb_set_aclrm(USB_U16 pipe);
void 		usb_clr_aclrm(USB_U16 pipe);
USB_U16 	usb_get_sqmon( USB_U16 pipe );
USB_U16 	usb_get_inbuf( USB_U16 pipe );

/* ---- c_usbint.c ---- */
void	usb_interrupt(void);

/* ==== global function (peri) ==== */
/* ---- c_usbint.c ---- */
void	P_ChangeConfigCB(USB_U16 Con_Num);
void	P_ChangeInterfaceCB(USB_U16 Con_Num, USB_U16 Int_Num, USB_U16 Alt_Num);

/* ---- p_classvendor.c ---- */
void	P_Class0(USB_U16 Reqtype, USB_U16 Request, USB_U16 Value, USB_U16 Index, USB_U16 Length);
void	P_Class1(USB_U16 Reqtype, USB_U16 Request, USB_U16 Value, USB_U16 Index, USB_U16 Length);
void	P_Class2(USB_U16 Reqtype, USB_U16 Request, USB_U16 Value, USB_U16 Index, USB_U16 Length);
void	P_Class3(USB_U16 Reqtype, USB_U16 Request, USB_U16 Value, USB_U16 Index, USB_U16 Length);
void	P_Class4(USB_U16 Reqtype, USB_U16 Request, USB_U16 Value, USB_U16 Index, USB_U16 Length);
void	P_Class5(USB_U16 Reqtype, USB_U16 Request, USB_U16 Value, USB_U16 Index, USB_U16 Length);
void	P_Vendor0(USB_U16 Reqtype, USB_U16 Request, USB_U16 Value, USB_U16 Index, USB_U16 Length);
void	P_Vendor1(USB_U16 Reqtype, USB_U16 Request, USB_U16 Value, USB_U16 Index, USB_U16 Length);
void	P_Vendor2(USB_U16 Reqtype, USB_U16 Request, USB_U16 Value, USB_U16 Index, USB_U16 Length);
void	P_Vendor3(USB_U16 Reqtype, USB_U16 Request, USB_U16 Value, USB_U16 Index, USB_U16 Length);
void	P_Vendor4(USB_U16 Reqtype, USB_U16 Request, USB_U16 Value, USB_U16 Index, USB_U16 Length);
void	P_Vendor5(USB_U16 Reqtype, USB_U16 Request, USB_U16 Value, USB_U16 Index, USB_U16 Length);

/* ---- p_controlrw.c ---- */
USB_U16 P_CtrlReadStart(USB_U32 size, USB_U8 *data);
void	P_CtrlWriteStart(USB_U32 size, USB_U8 *data);

/* ---- p_intrn.c ---- */
void	P_BRDYInterrupt(USB_U16 status, USB_U16 intenb);
void	P_NRDYInterrupt(USB_U16 status, USB_U16 intenb);
void	P_BEMPInterrupt(USB_U16 status, USB_U16 intenb);

/* ---- p_lib7264.c ---- */
void	P_ResetDCP(void);
void	P_ResetEP(USB_U16 num);
void	P_ResetDeviceDescriptor(USB_U16 mode);

/* ---- p_libint.c ---- */
void	P_EnableINTModule(void);
USB_U16 P_IsConfigured(void);
USB_U16 P_GetDeviceState(void);
USB_U16 P_EpToPipe(USB_U16 ep);

/* ---- p_status.c ---- */
USB_U16 P_GetConfigNum(void);
USB_U16 P_GetInterfaceNum(USB_U16 num);
USB_U16 P_GetAltNum(USB_U16 Con_Num, USB_U16 Int_Num);
void	P_InitEPTable(USB_U16 Con_Num, USB_U16 Int_Num, USB_U16 Alt_Num);
USB_U16 P_CheckRemoteWakeup(void);

/* ---- p_stdreqget.c ---- */
void	P_GetStatus(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);
void	P_GetDescriptor(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);
void	P_GetConfiguration(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);
void	P_GetInterface(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);
USB_U8	P_GetCurrentPower(USB_U16 ConfigNum);
void	P_Resrv_0(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);
void	P_Resrv_123(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);
void	P_Resrv_4(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);
void	P_Resrv_5(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);

/* ---- p_stdrqset.c ---- */
void	P_ClearFeature(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);
void	P_SetFeature(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);
void	P_SetAddress(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);
void	P_SetDescriptor(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);
void	P_SetConfiguration(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);
void	P_SetInterface(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);
void	P_SynchFrame(USB_U16 type, USB_U16 value, USB_U16 index, USB_U16 length);
void	usb_clear_alt(void);
void	P_ChangeConfigCB(USB_U16 Con_Num);
void	P_ChangeInterfaceCB(USB_U16 Con_Num, USB_U16 Int_Num, USB_U16 Alt_Num);

/* ---- p_usbint.c ---- */
void	P_USBInterrupt(void);
void	P_SaveRequest(void);

/* ---- p_usbsig.c ---- */
void	P_ModeInit(void);
void	P_InitModule(void);
USB_U16 P_CheckVBUStaus(void);
void	P_USBAttachCB(void);
void	P_USBDetachCB(void);
void	P_USBBusResetCB(void);
void	P_USBResumeCB(void);
void	P_USBSuspendCB(void);
void	P_USBTestModeCB(void);

/* ---- c_usb_dly.c ---- */
void	usb_delay_1ms(void);
void	usb_delay_xms(USB_U32 msec);
void	usb_delay_10us(USB_U32 usec);

/* ---- c_usb_dma.c ---- */
void	usb_userdef_clear_dmaint(void);
void	usb_userdef_start_dma(USB_DMA *dma);
USB_U32	usb_userdef_stop_dma0(void);
USB_U32	usb_userdef_stop_dma1(void);
void	usb_dma_stop_d0( USB_U16 pipe, USB_U32 remain );
void	usb_dma_stop_d1( USB_U16 pipe, USB_U32 remain );
void 	usb_dma_interrupt0(void);
void 	usb_dma_interrupt1(void);

#endif /* __USB_H__ */

/* End of File */
