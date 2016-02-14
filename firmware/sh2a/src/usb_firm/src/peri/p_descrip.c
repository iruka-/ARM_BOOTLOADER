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
 *       File Name   : p_descrip.c
 *       Abstract    : Define Peripheral Descriptor
 *       Version     : 1.00.00
 *       Device      : SH7264/SH7262
 *       Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
 *                   : C/C++ compiler package for the SuperH RISC engine family
 *                   :                             (Ver.9.02 Release00).
 *       OS          : None
 *       H/W Platform: M3A-HS64G50(CPU board)
 *
 *       History   : Dec.15.2009 ver.1.00.00
 *""FILE COMMENT END""**********************************************************/
#include "usb.h"

#define	BcdUsb					0x0200				/* bcdUSB */
//#define	BcdUsb					0x0110				/* bcdUSB */
#define	ReleaseNo				0x0100				/* Release Number */

/*---------- Communication Device Class define ----------*/
/* Interface Subclass Define */
#define	INTFSUBCLS_ABSCTR				(USB_U8)0x02	/* Abstract Control Model */
/* Interface Protocol Define */
#define	INTFPRTCL_ATV250				(USB_U8)0x01	/* AT Commands:V.250 etc */

/************************
 *	Device Descriptor	*
 ************************/
/* Standard Device Descriptor */
USB_U8	DeviceDescriptor[18] = {
	18,												/*  0:bLength */
	DT_DEVICE,										/*  1:bDescriptorType */
	(USB_U8)( BcdUsb			& 0xff),			/*  2:bcdUSB_lo */
	(USB_U8)((BcdUsb >> 8)		& 0xff),			/*  3:bcdUSB_hi */
	DEVCLSCOMM,										/*  4:bDeviceClass */
	0x00,											/*  5:bDeviceSubClass */
	0x00,											/*  6:bDeviceProtocol */
	64,												/*  7:bMaxPacketSize(for DCP) */
//	8,												/*  7:bMaxPacketSize(for DCP) */
	(USB_U8)( VENDOR_ID			& 0xff),			/*  8:idVendor_lo */
	(USB_U8)((VENDOR_ID >> 8)	& 0xff),			/*  9:idVendor_hi */
	(USB_U8)( PRODUCT_ID		& 0xff),			/* 10:idProduct_lo */
	(USB_U8)((PRODUCT_ID >> 8)	& 0xff),			/* 11:idProduct_hi */
	(USB_U8)( ReleaseNo			& 0xff),			/* 12:bcdDevice_lo */
	(USB_U8)((ReleaseNo >> 8)	& 0xff),			/* 13:bcdDevice_hi */
	0,												/* 14:iManufacturer */
	0,												/* 15:iProduct */
	3,												/* 16:iSerialNumber */
	1												/* 17:bNumConfigurations */
};

/* Device Qualifier Descriptor */
USB_U8	QualifierDescriptor[10] = {
	10,												/*  0:bLength */
	DT_DEVICE_QUALIFIER,							/*  1:bDescriptorType */
	(USB_U8)( BcdUsb			& 0xff),			/*  2:bcdUSB_lo */
	(USB_U8)((BcdUsb >> 8)		& 0xff),			/*  3:bcdUSB_hi */
	DEVCLSCOMM,										/*  4:bDeviceClass */
	0x00,											/*  5:bDeviceSubClass */
	0x00,											/*  6:bDeviceProtocol */
	64,												/*  7:bMaxPacketSize(for DCP) */
//	8,												/*  7:bMaxPacketSize(for DCP) */
	1,												/*  8:bNumConfigurations */
	0												/*  9:bReserved */
};

/************************************************************
 *	Configuration Or Other_Speed_Configuration Descriptor	*
 ************************************************************/
/* For Full-Speed */
USB_U8	Configuration_Full_1[67] = {
	9,												/*  0:bLength */
	SOFTWARE_CHANGE,								/*  1:bDescriptorType */
	67,												/*  2:wTotalLength(L) */
	0,												/*  3:wTotalLength(H) */
	2,												/*  4:bNumInterfaces */
	1,												/*  5:bConfigurationValue */
	0,												/*  6:iConfiguration */
	(USB_U8)(CF_RESERVED | CF_SELF | RWUP_MODE),	/*  7:bmAttributes */
	(USB_U8)(32 / 2),								/*  8:bMaxPower (2mA unit) */

		/* Interface Descriptor(1-0-0 [Communication Class]) */
		9,											/*  0:bLength */
		DT_INTERFACE,								/*  1:bDescriptor */
		0,											/*  2:bInterfaceNumber */
		0,											/*  3:bAlternateSetting */
		1,											/*  4:bNumEndpoints */ 	/* support INT IN (= EP3) */
		INTFCLSCDCC,								/*  5:bInterfaceClass */
		INTFSUBCLS_ABSCTR,							/*  6:bInterfaceSubClass */
		INTFPRTCL_ATV250,							/*  7:bInterfaceProtocol */
		0,											/*  8:iInterface */
		/* Class-Specific Configuration Descriptors */
		/* Header Functional Descriptor */
		0x05,	/*  0:length of this desc. */
		0x24,	/*  1:CS_INTERFACE */
		0x00,	/*  2:Header Functional Descr. */
		0x10,	/*  3:bcdCDC Rel. 1.10 */
		0x01,	/*  4:bcdCDC Rel. 1.10 */
		/* Abstract Control Management Functional Descriptor */
		0x04,	/*  0:length of this desc. */
		0x24,	/*  1:CS_INTERFACE */
		0x02,	/*  2:Abstr.Contr.Managm. F.D. */
		0x06,	/*  3:bmCapabilities */	/* support Send Break, Set Line Coding, Set Control Line State, Get Line Coding */
		/* Union Functional Descriptor */
		0x05,	/*  0:length of this desc. */
		0x24,	/*  1:CS_INTERFACE */
		0x06,	/*  2:Union Functional Descr. */
		0x00,	/*  3:bMasterInterface */
		0x01,	/*  4:bSlaveInterface0 */
		/* Call Management Functional Descriptor */
		0x05,	/*  0:length of this desc. */
		0x24,	/*  1:CS_INTERFACE */
		0x01,	/*  2:Call Managment F.D. */
		0x03,	/*  3:bmCapabilities */
		0x01,	/*  4:bDataInterface */
	/* Endpoint Descriptor 1-0-0-0 */
		 	0x07,	/*  0:bLength */
			0x05,	/*  1:bDescriptorType */
			0x86,	/*  2:bEndpointAddress */	/* IN, number = 6 */
			0x03, 	/*  3:bmAttribute */
			0x08, 	/*  4:wMAXPacketSize_lo */
		 	0x00, 	/*  5:wMAXPacketSize_hi */
			0x10, 	/*  6:bInterval */
		/* Interface Descriptor 1-1-0 [Data Class Interface] */
		9,											/*  0:bLength */
		DT_INTERFACE,								/*  1:bDescriptor */
		1,											/*  2:bInterfaceNumber */
		0,											/*  3:bAlternateSetting */
		2,											/*  4:bNumEndpoints */ 	/* support Bulk OUT (= EP1) and Bulk IN (= EP2) */
		INTFCLSCDCD,								/*  5:bInterfaceClass */
		0,											/*  6:bInterfaceSubClass */
		0,											/*  7:bInterfaceProtocol */
		0,											/*  8:iInterface */
			/* Endpoint Descriptor 1-1-0-0 */
			0x07, 	/*  0:bLength */
			0x05,	/*  1:bDescriptorType */
			0x01,	/*  2:bEndpointAddress */
			0x02,	/*  3:bmAttribute */
			0x40, 	/*  4:wMAXPacketSize_lo */
			0x00, 	/*  5:wMAXPacketSize_hi */
			0x00,	/*  6:bInterval */
			/* Endpoint Descriptor 1-1-0-1 */
			0x07,	/*  0:bLength */
			0x05,	/*  1:bDescriptorType */
			0x82,	/*  2:bEndpointAddress */
			0x02,	/*  3:bmAttribute */
			0x40, 	/*  4:wMAXPacketSize_lo */
			0x00, 	/*  5:wMAXPacketSize_hi */
			0x00 	/*  6:bInterval */
};

/* For Hi-Speed */
USB_U8	Configuration_Hi_1[67] = {
	9,												/*  0:bLength */
	SOFTWARE_CHANGE,								/*  1:bDescriptorType */
	67,												/*  2:wTotalLength(L) */
	0,												/*  3:wTotalLength(H) */
	2,												/*  4:bNumInterfaces */
	1,												/*  5:bConfigurationValue */
	0,												/*  6:iConfiguration */
	(USB_U8)(CF_RESERVED | CF_SELF | RWUP_MODE),	/*  7:bmAttributes */
	(USB_U8)(32 / 2),								/*  8:bMaxPower (2mA unit) */

		/* Interface Descriptor(1-0-0 [Communication Class]) */
		9,											/*  0:bLength */
		DT_INTERFACE,								/*  1:bDescriptor */
		0,											/*  2:bInterfaceNumber */
		0,											/*  3:bAlternateSetting */
		1,											/*  4:bNumEndpoints */ 	/* support INT IN (= EP6) */
		INTFCLSCDCC,								/*  5:bInterfaceClass */
		INTFSUBCLS_ABSCTR,							/*  6:bInterfaceSubClass */
		INTFPRTCL_ATV250,							/*  7:bInterfaceProtocol */
		0,											/*  8:iInterface */
		/* Class-Specific Configuration Descriptors */
		/* Header Functional Descriptor */
		0x05,	/*  0:length of this desc. */
		0x24,	/*  1:CS_INTERFACE */
		0x00,	/*  2:Header Functional Descr. */
		0x10,	/*  3:bcdCDC Rel. 1.10 */
		0x01,	/*  4:bcdCDC Rel. 1.10 */
		/* Abstract Control Management Functional Descriptor */
		0x04,	/*  0:length of this desc. */
		0x24,	/*  1:CS_INTERFACE */
		0x02,	/*  2:Abstr.Contr.Managm. F.D. */
		0x06,	/*  3:bmCapabilities */	/* support Send Break, Set Line Coding, Set Control Line State, Get Line Coding */
		/* Union Functional Descriptor */
		0x05,	/*  0:length of this desc. */
		0x24,	/*  1:CS_INTERFACE */
		0x06,	/*  2:Union Functional Descr. */
		0x00,	/*  3:bMasterInterface */
		0x01,	/*  4:bSlaveInterface0 */
		/* Call Management Functional Descriptor */
		0x05,	/*  0:length of this desc. */
		0x24,	/*  1:CS_INTERFACE */
		0x01,	/*  2:Call Managment F.D. */
		0x03,	/*  3:bmCapabilities */
		0x01,	/*  4:bDataInterface */
			/* Endpoint Descriptor 1-0-0-0 */
		 	0x07,	/*  0:bLength */
			0x05,	/*  1:bDescriptorType */
			0x86,	/*  2:bEndpointAddress */	/* IN, number = 6 */
			0x03, 	/*  3:bmAttribute */
			0x08, 	/*  4:wMAXPacketSize_lo */
		 	0x00, 	/*  5:wMAXPacketSize_hi */
			0x8, 	/*  6:bInterval */
		/* Interface Descriptor 1-1-0 [Data Class Interface] */
		9,											/*  0:bLength */
		DT_INTERFACE,								/*  1:bDescriptor */
		1,											/*  2:bInterfaceNumber */
		0,											/*  3:bAlternateSetting */
		2,											/*  4:bNumEndpoints */ 	/* support Bulk OUT (= EP1) and Bulk IN (= EP2) */
		INTFCLSCDCD,								/*  5:bInterfaceClass */
		0,											/*  6:bInterfaceSubClass */
		0,											/*  7:bInterfaceProtocol */
		0,											/*  8:iInterface */
			/* Endpoint Descriptor 1-1-0-0 */
			0x07, 	/*  0:bLength */
			0x05,	/*  1:bDescriptorType */
			0x01,	/*  2:bEndpointAddress */
			0x02,	/*  3:bmAttribute */
			0x00, 	/*  4:wMAXPacketSize_lo */
			0x02, 	/*  5:wMAXPacketSize_hi */
			0x00,	/*  6:bInterval */
			/* Endpoint Descriptor 1-1-0-1 */
			0x07,	/*  0:bLength */
			0x05,	/*  1:bDescriptorType */
			0x82,	/*  2:bEndpointAddress */
			0x02,	/*  3:bmAttribute */
			0x00, 	/*  4:wMAXPacketSize_lo */
			0x02, 	/*  5:wMAXPacketSize_hi */
			0x00 	/*  6:bInterval */
};

/* Configuration */
USB_U8	Configuration_Full_2[2] = { 0, SOFTWARE_CHANGE };
USB_U8	Configuration_Full_3[2] = { 0, SOFTWARE_CHANGE };
USB_U8	Configuration_Full_4[2] = { 0, SOFTWARE_CHANGE };
USB_U8	Configuration_Full_5[2] = { 0, SOFTWARE_CHANGE };

/* Other_Speed_Configuration */
USB_U8	Configuration_Hi_2[2] = { 0, SOFTWARE_CHANGE };
USB_U8	Configuration_Hi_3[2] = { 0, SOFTWARE_CHANGE };
USB_U8	Configuration_Hi_4[2] = { 0, SOFTWARE_CHANGE };
USB_U8	Configuration_Hi_5[2] = { 0, SOFTWARE_CHANGE };

USB_U8 *ConPtr[] = {
	(USB_U8 *)SOFTWARE_CHANGE,
	(USB_U8 *)SOFTWARE_CHANGE,
	(USB_U8 *)SOFTWARE_CHANGE,
	(USB_U8 *)SOFTWARE_CHANGE,
	(USB_U8 *)SOFTWARE_CHANGE
};

USB_U8 *ConPtr_Other[] = {
	(USB_U8 *)SOFTWARE_CHANGE,
	(USB_U8 *)SOFTWARE_CHANGE,
	(USB_U8 *)SOFTWARE_CHANGE,
	(USB_U8 *)SOFTWARE_CHANGE,
	(USB_U8 *)SOFTWARE_CHANGE
};

/*************************************
 *	  String Descriptor				 *
 *************************************/
USB_U8	StringDescriptor0[4] = {			/* UNICODE 0x0409 English (United States) */
	4,												/*  0:bLength */
	DT_STRING,										/*  1:bDescriptorType */
	0x09, 0x04										/*  2:wLANGID[0] */
};
	
USB_U8	StringDescriptor1[] = {
	16,
	DT_STRING,										/*  1:bDescriptorType */
	0x52,0x00,0x65,0x00,0x6E,0x00,
	0x65,0x00,0x73,0x00,0x61,0x00,0x73,0x00
};	/*	Manufacturer STRING Descriptor(Sample)	*/

USB_U8	StringDescriptor2[] = {						/* iProduct */
	2,												/*  0:bLength */
	DT_STRING,										/*  1:bDescriptorType */
	0x00
};
		
USB_U8	StringDescriptor3[] = {					/* iSerialNumber */
	26,												/*  0:bLength */
	0x03,										/*  1:bDescriptorType */
	0x30,0x00,0x30,0x00,0x30,0x00,
	0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,
	0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,
	0x30,0x00,0x00,0x00
};	/*	SerialNumber STRING Descriptor(Sample)	*/

USB_U8	StringDescriptor4[] = {						/* Configration STRING Descriptor */
	0,												/*  0:bLength */
	DT_STRING,										/*  1:bDescriptorType */
	0x00
};
USB_U8	StringDescriptor5[] = {						/* Interface STRING Descriptor */
	0,												/*  0:bLength */
	DT_STRING,										/*  1:bDescriptorType */
	0x00
};

USB_U8 *StrPtr[] = {
	StringDescriptor0,
	StringDescriptor1,
	StringDescriptor2,
	StringDescriptor3,
	StringDescriptor4,
	StringDescriptor5
};

/*===========================================================================*/
/* End of File */
