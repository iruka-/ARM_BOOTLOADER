/**
* @file          usbfunc_demoapp_config.h
* @brief         configuration of sample application for usb function
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/
#ifndef __USBFUNC_DEMOAPP_CONFIG__
#define __USBFUNC_DEMOAPP_CONFIG__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Configuration  (variable)                                                  */
/*----------------------------------------------------------------------------*/
/* debug infomation output setting */
/* #define DEBUG_PRINT */         /* define   : debug infomation output enable  */
                            /* undeifne : debug infomation output disable   */

#ifdef DEBUG_MONITOR
    #undef DEBUG_PRINT
#endif /* DEBUG_MONITOR */

/*------------ USB descriptor configuration-----------------------------------*/
#if	0
#define USB_VENDOR_ID    (0x04C5)   /* Vendor ID(fujitsu) */
#define USB_PRODUCT_ID   (0x4242)   /* Product ID */
#else
#define USB_VENDOR_ID    (0x26FF)   /* Vendor ID(CQ) */
#define USB_PRODUCT_ID   (0x4912)   /* Product ID(CDC) */
#endif

#define USB_BCD_DEVICE   (0x0100)   /* USB Specification Release Number in Binary-Coded Decimal */

/* manufacturer string descriptor */
#define USB_STRING_MANUFACTURE        'F', 0, 'u', 0, 'j', 0, 'i', 0, 't', 0, 's', 0, 'u', 0
#define USB_MANUFACTURE_LENGTH  (14)  /* length of manufacturer string descriptorc */

/* product string descriptor */
#define USB_STRING_PRODUCT            'S', 0, 'e', 0, 'r', 0, 'i', 0,'a', 0, 'l', 0, '-', 0, 'P', 0,'o', 0, 'r', 0, 't', 0
#define USB_PRODUCT_LENGTH      (22)  /* length of product string descriptor */

/* product number string descriptor */
#define USB_STRING_SERIAL             '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '1', 0
#define USB_SERIAL_LENGTH       (24)   /* length of product number string descriptor */


#endif /* __USBFUNC_DEMOAPP_CONFIG__ */
