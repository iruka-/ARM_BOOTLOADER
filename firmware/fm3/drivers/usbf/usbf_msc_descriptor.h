/**
* @file          usbf_msc_descriptor.h
* @brief         MSC descriptor control module
* @author        huangshan
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __USBF_MSC_DESCRIPTOR_INC__
#define __USBF_MSC_DESCRIPTOR_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

#define USBF_MSC_CLASS_CODE              (0x00)          /* specify by interface */
#define USBF_MSC_SUBCLASS_CODE           (0x00)          /* specify by interface */
#define USBF_MSC_PROTOCOL_CODE           (0x00)          /* specify by interface */
#define USBF_MSC_IF_CLASS_CODE           (0x08)          /* mass storage class   */
#define USBF_MSC_IF_SUBCLASS_CODE        (0x06)          /* SCSI media           */
#define USBF_MSC_IF_PROTOCOL_CODE        (0x50)          /* BOTÅionly bulk Åj    */

#endif   /* __USBF_MSC_DESCRIPTOR_INC__ */
