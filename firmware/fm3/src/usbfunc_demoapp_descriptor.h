/**
* @file          usbfunc_demoapp_descriptor.h
* @brief         descriptor defination module
* @author        huangshan
* @version       1.0
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __USBFUNC_DEMOAPP_DESCRIPTOR_INC__
#define __USBFUNC_DEMOAPP_DESCRIPTOR_INC__

/*----------------------------------------------------------------------------*/
/*  Headers                                                                   */
/*----------------------------------------------------------------------------*/
#include "usbf.h"

/*
 #include "usbf_msc_config.h"
 #include "../drivers/usbf/usbf.h"
 #include "../drivers/usbf/usbf_spec.h"
 #include "../drivers/usbf/usbf_config.h"
*/

/******************************************************************************/
/* 外部定数定義                                                               */
/******************************************************************************/

/* packet size of control endpoint setting */
#define  CTRL_EP_BUFFER_SIZE     (USBF_MSC_CTRL_EP_PKT_SIZE)

/* number of bulk in endpoint setting */
#define  BULK_IN_EP_NUM          (USBF_MSC_BULKIN_EP)

/* packet size of bulk in endpoint setting */
#define  BULK_IN_EP_BUFFER_SIZE  (USBF_MSC_BULKIN_EP_PKT_SIZE)

/* number of bulk out endpoint setting */
#define  BULK_OUT_EP_NUM         (USBF_MSC_BULKOUT_EP)

/* packet size of bulk out endpoint setting */
#define  BULK_OUT_EP_BUFFER_SIZE (USBF_MSC_BULKOUT_EP_PKT_SIZE)



/*----------------------------------------------------------------------------*/
/* Descriptor configuration                                                   */
/*----------------------------------------------------------------------------*/
#define CFG_ATTRIBUTES    (0xE0)
/*--------------------------------------------------------------------------*/
/*                       B'11100000                                         */
/*                         |||||||+-- D0:reserved                           */
/*                         ||||||+--- D1:reserved                           */
/*                         |||||+---- D2:reserved                           */
/*                         ||||+----- D3:reserved                           */
/*                         |||+------ D4:reserved                           */
/*                         ||+------- D5:remote wakeup                      */
/*                         |+-------- D6:self power                         */
/*                         +--------- D7:reserved                           */
/*--------------------------------------------------------------------------*/
#define STRING_DESC_NUM     (4)                             /* string descriptor number        */
#define INTERFACE_NUMBER    (2)	//(1)                             /* interface number                */
#define USED_EPNUM          (4)	//(3)                             /* using endpoint number           */
#define STR0_DESC_LENGTH    (4)                             /* string0 descriptor's length     */
#define STR1_DESC_LENGTH    (USB_MANUFACTURE_LENGTH + 2)    /* string1 descriptor's length     */
#define STR2_DESC_LENGTH    (USB_PRODUCT_LENGTH + 2)        /* string2 descriptor's length     */
#define STR3_DESC_LENGTH    (USB_SERIAL_LENGTH + 2)         /* string3 descriptor's length     */

/* configuration descriptor's total length */
#define CFG_TOTAL_LENGTH (USBF_CFG_DESC_LENGTH + (INTERFACE_NUMBER * USBF_INF_DESC_LENGTH) \
                          + ((USED_EPNUM - 1) * USBF_EPN_DESC_LENGTH))

#define	IS_CUSTOM_CLASS	(0)	// for TEST

#if		IS_CUSTOM_CLASS
#define		dvCLASS	0xFF
#undef	USB_VENDOR_ID
#define	USB_VENDOR_ID	0xFFFF
#else
#define		dvCLASS	0x02
#endif

#undef CFG_TOTAL_LENGTH
#define CFG_TOTAL_LENGTH 67


/* device descriptor */
const char DeviceDescriptor[USBF_DEV_DESC_LENGTH] =
{
    USBF_DEV_DESC_LENGTH,                       /* bLength           */
    USBF_DEV_DESC_TYPE,                         /* bDecriptor        */
    0x00,	//0x10,       /* bcdUSB : USB Spec Release Number in BCD */
    0x02,	//0x01,
    dvCLASS,//CDC=0x02               /* bDeviceClass :Class Code     */
    0x00,                                       /* bDeviceSubClass   */
    0x00,                                       /* bDeviceProtocol   */
    CTRL_EP_BUFFER_SIZE,                        /* bMaxPacketSize    */
    LOBYTE(USB_VENDOR_ID),                      /* idVendor          */
    HIBYTE(USB_VENDOR_ID),
    LOBYTE(USB_PRODUCT_ID),                     /* idProduct         */
    HIBYTE(USB_PRODUCT_ID),
    LOBYTE(USB_BCD_DEVICE),/* bcdDevice : Device release number in BCD*/
    HIBYTE(USB_BCD_DEVICE),
    0x01,                                       /* iManufacture      */
    0x02,                                       /* iProduct          */
    0x03,                                       /* iSerialNumber     */
    0x01,                                       /* bNumConfiguration */
};

/* configuration descriptor */
const uint8_t ConfigurationDescriptor[CFG_TOTAL_LENGTH] =
{
    /* ====Configuration Derscriptor================================== */
    USBF_CFG_DESC_LENGTH,                    /* bLength             */
    USBF_CFG_DESC_TYPE,                      /* bDescriptorType     */
    LOBYTE(CFG_TOTAL_LENGTH),                /* wTotalLength        */
    HIBYTE(CFG_TOTAL_LENGTH),
    0x02,	//0x01,                                    /* bNumInterfaces      */
    0x01,                                    /* bConfigurationValue */
    0x00,                                    /* iConfiguration      */
    0xc0,	//CFG_ATTRIBUTES,                          /* bmAttributes        */
    0x32,                                    /* bMaxPower           */

    /* ====Interface1 Derscriptor===================================== */
    USBF_INF_DESC_LENGTH,                    /* bLength             */
    USBF_INF_DESC_TYPE,                      /* bDescriptorType     */
    0x00,                                    /* bInterfaceNumber    */
    0x00,                                    /* bAlternateSetting   */
    0x01,	//     0x02,                                    /* bNumEndpoint        */
    0x02,	//CDC  0x08,                                    /* bInterfaceClass     */
    0x02,	//     0x06,                                    /* bInterfaceSubClass  */
    0x01,	//AT   0x50,                                    /* bInterfaceProtocol  */
    0x00,                                    /* iInterface          */

	/*Header Functional Descriptor*/
	0x05,   /* bLength: Endpoint Descriptor size */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x00,   /* bDescriptorSubtype: Header Func Desc */
	0x10,   /* bcdCDC: spec release number */
	0x01,
	/*Call Managment Functional Descriptor*/
	0x05,   /* bFunctionLength */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x01,   /* bDescriptorSubtype: Call Management Func Desc */
	0x00,   /* bmCapabilities: D0+D1 */
	0x01,   /* bDataInterface: 1 */
	/*ACM Functional Descriptor*/
	0x04,   /* bFunctionLength */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
	0x02,   /* bmCapabilities */
	/*Union Functional Descriptor*/
	0x05,   /* bFunctionLength */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x06,   /* bDescriptorSubtype: Union func desc */
	0x00,   /* bMasterInterface: Communication class interface */
	0x01,   /* bSlaveInterface0: Data Class Interface */
//EP2_IN(82):
//インタラプト転送.
    /* ====EndPoint(Bulk-In) Derscriptor============================= */
    USBF_EPN_DESC_LENGTH,                    /* bLength            */
    USBF_EPN_DESC_TYPE,                      /* bDescriptorType    */
    0x82,	//(USBF_DIR_IN << 7) | BULK_IN_EP_NUM,     /* bEndpointAddress   */
    USBF_TRANS_INTERRUPT,	//USBF_TRANS_BULK,                         /* bmAttributes       */
    0x08,	//LOBYTE(BULK_IN_EP_BUFFER_SIZE),          /* wMaxPaketSize      */
    0x00,	//HIBYTE(BULK_IN_EP_BUFFER_SIZE),
    0xff,	//0x00,                                    /* bInterval          */

	/*Data class interface descriptor*/
	0x09,   /* bLength: Endpoint Descriptor size */
	USBF_INF_DESC_TYPE,  /* bDescriptorType: */
	0x01,   /* bInterfaceNumber: Number of Interface */
	0x00,   /* bAlternateSetting: Alternate setting */
	0x02,   /* bNumEndpoints: Two endpoints used */
	0x0A,   /* bInterfaceClass: CDC */
	0x00,   /* bInterfaceSubClass: */
	0x00,   /* bInterfaceProtocol: */
	0x00,   /* iInterface: */
//EP3_OUT(03):	
//	PCからDevice側へデータを送信する.
	/*Endpoint 3 Descriptor*/
	0x07,   /* bLength: Endpoint Descriptor size */
	USBF_EPN_DESC_TYPE,   /* bDescriptorType: Endpoint */
	0x03,   /* bEndpointAddress: (OUT3) */
	0x02,   /* bmAttributes: Bulk */
	0x40,             /* wMaxPacketSize: */
	0x00,
	0x00,   /* bInterval: ignore for Bulk transfer */
//EP1_IN(81):
//	Device側からPCへデータを返送する.
    /* ====EndPoint(Bulk-Out) Derscriptor============================ */
    USBF_EPN_DESC_LENGTH,                    /* bLength            */
    USBF_EPN_DESC_TYPE,                      /* bDescriptorType    */
	0x81,	// (USBF_DIR_OUT << 7) | BULK_OUT_EP_NUM,   /* bEndpointAddress   */
    USBF_TRANS_BULK,                         /* bmAttributes       */
    LOBYTE(BULK_OUT_EP_BUFFER_SIZE),         /* wMaxPaketSize      */
    HIBYTE(BULK_OUT_EP_BUFFER_SIZE),
    0x00,                                     /* bInterval          */
};

/* language ID */
const uint8_t String0Derscriptor[STR0_DESC_LENGTH] =
{
    STR0_DESC_LENGTH,       /* bLength         */
    0x03,                   /* bDescriptorType */
    0x09,
    0x04
};

/* Manufacture */
const uint8_t String1Derscriptor[STR1_DESC_LENGTH] =
{
    STR1_DESC_LENGTH,       /* bLength         */
    0x03,                   /* bDescriptorType */
    USB_STRING_MANUFACTURE  /* bString         */
};

/* Product */
const uint8_t String2Derscriptor[STR2_DESC_LENGTH] =
{
    STR2_DESC_LENGTH,       /* bLength         */
    0x03,                   /* bDescriptorType */
    USB_STRING_PRODUCT      /* bString         */
};

/* SerialNumber */
const uint8_t String3Derscriptor[STR3_DESC_LENGTH] =
{
    STR3_DESC_LENGTH,       /* bLength         */
    0x03,                   /* bDescriptorType */
    USB_STRING_SERIAL       /* bString         */
};

/* string descriptor */
USBF_ST_STRING_DESCRIPTOR StringDescriptor[STRING_DESC_NUM] =
{
    {
        0,                          /* index   */
        (uint8_t*)&String0Derscriptor  /* String0 */
    },

    {
        1,                          /* index   */
        (uint8_t*)&String1Derscriptor  /* String1 */
    },

    {
        2,                          /* index   */
        (uint8_t*)&String2Derscriptor  /* String2 */
    },

    {
        3,                          /* index   */
        (uint8_t*)&String3Derscriptor  /* String3 */
    }
};

/* USB descriptor information */
USBF_ST_USB_DESCRIPTOR g_UsbDescriptor =
{
    (uint8_t*)&DeviceDescriptor,
    (uint8_t*)&ConfigurationDescriptor,
    (USBF_ST_STRING_DESCRIPTOR*)&StringDescriptor,
    STRING_DESC_NUM
};

#endif/* __APP_DESCRIPTOR_INC__ */
