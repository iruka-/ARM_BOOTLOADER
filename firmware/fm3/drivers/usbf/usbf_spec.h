/**
* @file          usbf_spec.h
* @brief         USB protocol control module
* @author        huangshan
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __USBF_SPEC_INC__
#define __USBF_SPEC_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

/* descriptor type offset */
#define USBF_DEV_DESC_TYPE           ((uint8_t)0x01)  /* device */
#define USBF_CFG_DESC_TYPE           ((uint8_t)0x02)  /* configuration */
#define USBF_STR_DESC_TYPE           ((uint8_t)0x03)  /* string */
#define USBF_INF_DESC_TYPE           ((uint8_t)0x04)  /* interface */
#define USBF_EPN_DESC_TYPE           ((uint8_t)0x05)  /* endpoint */

/* descriptor size offset */
#define USBF_DEV_DESC_LENGTH         ((uint8_t)0x12)  /* device descriptor size */
#define USBF_CFG_DESC_LENGTH         ((uint8_t)0x09)  /* configuration descripter size */
#define USBF_INF_DESC_LENGTH         ((uint8_t)0x09)  /* interface descripter size */
#define USBF_EPN_DESC_LENGTH         ((uint8_t)0x07)  /* endpoint descripter size */

#define USBF_DESC_LENGTH             ((uint8_t)0x00)  /* descriptor size offset */
#define USBF_DESC_TYPE               ((uint8_t)0x01)  /* descriptor type offset */

/* offset only for device descriptor */
#define USBF_DEV_BCDUSB              ((uint8_t)0x02)  /* offset for USB version */
#define USBF_DEV_DEVICECLASS         ((uint8_t)0x04)  /* offset for class code offset */
#define USBF_DEV_DEVICESUBCLASS      ((uint8_t)0x05)  /* offset for subclass code offset */
#define USBF_DEV_DEVICEPROTOCOL      ((uint8_t)0x06)  /* offset for protocol code offset */
#define USBF_DEV_MAXPACKETSIZE0      ((uint8_t)0x07)  /* offset for endpoint0 max packet size */
#define USBF_DEV_IDVENDOR            ((uint8_t)0x08)  /* offset for header ID */
#define USBF_DEV_IDPRODUCT           ((uint8_t)0x0A)  /* offset for product ID */
#define USBF_DEV_BCDDEVICE           ((uint8_t)0x0C)  /* offset for device version */
#define USBF_DEV_MANUFACTURER        ((uint8_t)0x0E)  /* offset for manufacturer index */
#define USBF_DEV_PRODUCT             ((uint8_t)0x0F)  /* offset for device name index */
#define USBF_DEV_SERIALNUMBER        ((uint8_t)0x10)  /* offset for device index */
#define USBF_DEV_NUMCONFIGURATIONS   ((uint8_t)0x11)  /* offset for configuration numbers */

/* offset only for configuration */
#define USBF_CFG_TOTALLENGTH         ((uint8_t)0x02)  /* offset for all descriptor length */
#define USBF_CFG_NUMINTERFACES       ((uint8_t)0x04)  /* offset for interface numbers */
#define USBF_CFG_CONFIGURATIONVALUE  ((uint8_t)0x05)  /* offset for parameter value for selectting descriptor by Set_Configuration */
#define USBF_CFG_CONFIGURATION       ((uint8_t)0x06)  /* offset for string descriptor index */
#define USBF_CFG_ATTRIBUTES          ((uint8_t)0x07)  /* offset for device power */
#define USBF_CFG_MAXPOWER            ((uint8_t)0x08)  /* offset for max power */

/* offset only for interface */
#define USBF_ITF_INTERFACENUMBER     ((uint8_t)0x02)  /* offset for index number */
#define USBF_ITF_ALTERNATESETTING    ((uint8_t)0x03)  /* offset for parameter value for selectting descriptor by Set_Interface */
#define USBF_ITF_NUMENDPOINTS        ((uint8_t)0x04)  /* offset for device has endpoint numbers */
#define USBF_ITF_INTERFACECLASS      ((uint8_t)0x05)  /* offset for class code */
#define USBF_ITF_INTERFACESUBCLASS   ((uint8_t)0x06)  /* offset for subclass code */
#define USBF_ITF_INTERFACEPROTOCOL   ((uint8_t)0x07)  /* offset for protocol code */
#define USBF_ITF_INTERFACE           ((uint8_t)0x08)  /* offset for string descripter index */

/* offset only for endpoint descriptor */
#define USBF_EPN_ENDPOINTADDRESS     ((uint8_t)0x02)  /* offset for endpoint address */
#define USBF_EPN_ATTRIBUTES          ((uint8_t)0x03)  /* offset for endpoint transfer method */
#define USBF_EPN_MAXPACKETSIZE       ((uint8_t)0x04)  /* offset for max packet size */
#define USBF_EPN_INTERVAL            ((uint8_t)0x06)  /* offset for polling interval */

/* offset only for string descriptor */
#define USBF_STR_STRING              ((uint8_t)0x02)  /* offset for string */

/* definition for standard request */
#define USBF_REQ_GET_STATUS          ((uint8_t)0x00)  /* status get */
#define USBF_REQ_CLEAR_FEATURE       ((uint8_t)0x01)  /* CLEAR_FEATURE */
#define USBF_REQ_RESERVED1           ((uint8_t)0x02)  /* reserve */
#define USBF_REQ_SET_FEATURE         ((uint8_t)0x03)  /* SET_FEATURE */
#define USBF_REQ_RESERVED2           ((uint8_t)0x04)  /* reserve */
#define USBF_REQ_SET_ADDRESS         ((uint8_t)0x05)  /* address set */
#define USBF_REQ_GET_DESCRIPTOR      ((uint8_t)0x06)  /* descriptor get */
#define USBF_REQ_SET_DESCRIPTOR      ((uint8_t)0x07)  /* descriptor set */
#define USBF_REQ_GET_CONFIGURATION   ((uint8_t)0x08)  /* configuration number get */
#define USBF_REQ_SET_CONFIGURATION   ((uint8_t)0x09)  /* configuration select */
#define USBF_REQ_GET_INTERFACE       ((uint8_t)0x0A)  /* interface number get */
#define USBF_REQ_SET_INTERFACE       ((uint8_t)0x0B)  /* interface select */
#define USBF_REQ_SYNC_FRAME          ((uint8_t)0x0C)  /* synchronization frame get */

/* definition for power */
#define USBF_BUS_POWERED             ((uint8_t)0x00)  /* bus-power  */
#define USBF_SELF_POWERED            ((uint8_t)0x01)  /* self-power */

/* definition for transfer direction */
#define USBF_DIR_OUT                 ((uint8_t)0x00)  /* transfer direction(OUT) */
#define USBF_DIR_IN                  ((uint8_t)0x01)  /* transfer direction(IN)  */

/* definition for transfer type */
#define USBF_TRANS_CONTROL           ((uint8_t)0x00)  /* transfer type(control) */
#define USBF_TRANS_ISOCHRONOUS       ((uint8_t)0x01)  /* transfer type(isochronous) */
#define USBF_TRANS_BULK              ((uint8_t)0x02)  /* transfer type(bulk) */
#define USBF_TRANS_INTERRUPT         ((uint8_t)0x03)  /* transfer type(interrupt) */


/*----------------------------------------------------------------------------*/
/* Global Variable Definition                                                 */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/

/* definition for bit access */
typedef union
{
    uint8_t byte;
    struct
    {
#if (CPU_BITORDER == CPU_LSBFIRST)
        uint8_t bit0 : 1;
        uint8_t bit1 : 1;
        uint8_t bit2 : 1;
        uint8_t bit3 : 1;
        uint8_t bit4 : 1;
        uint8_t bit5 : 1;
        uint8_t bit6 : 1;
        uint8_t bit7 : 1;
#elif (CPU_BITORDER == CPU_MSBFIRST)
        uint8_t bit7 : 1;
        uint8_t bit6 : 1;
        uint8_t bit5 : 1;
        uint8_t bit4 : 1;
        uint8_t bit3 : 1;
        uint8_t bit2 : 1;
        uint8_t bit1 : 1;
        uint8_t bit0 : 1;
#endif
    }bit;
}USBF_UN_BIT_8;

/* setup packet */
typedef struct _USBF_ST_SETUP_INFO
{
    union
    {
        uint8_t byte;
        struct
        {
#if (CPU_BITORDER == CPU_LSBFIRST)
            uint8_t rcver : 5; /* receiver */
            uint8_t type  : 2; /* type   */
            uint8_t dir   : 1; /* direction   */
#elif (CPU_BITORDER == CPU_MSBFIRST)
            uint8_t dir   : 1; /* direction   */
            uint8_t type  : 2; /* type   */
            uint8_t rcver : 5; /* receiver */
#endif
        }bits;
    }bmRequestType;            /* request type */

    uint8_t  bRequest;         /* request type */

    uint16_t wValue;           /* wValue */

    uint16_t wIndex;           /* wIndex */

    uint16_t wLength;          /* transfer data size */

}USBF_ST_SETUP_INFO;

/* configuration descriptor power attibute field */
typedef union
{
    uint8_t byte;
    struct
    {
#if (CPU_BITORDER == CPU_LSBFIRST)
        uint8_t reserved0      : 5;    /* reserve(0) */
        uint8_t remote_wakeup  : 1;    /* remote wakeup */
        uint8_t powered_type   : 1;    /* power type */
        uint8_t reserved1      : 1;    /* reserve(1) */
#elif (CPU_BITORDER == CPU_MSBFIRST)
        uint8_t reserved1      : 1;    /* reserve(1) */
        uint8_t powered_type   : 1;    /* power type */
        uint8_t remote_wakeup  : 1;    /* remote wakeup */
        uint8_t reserved0      : 5;    /* reserve(0) */
#endif
    }bits;
}USBF_UN_CFG_bmAttributes;

/* endpoint descriptor address field */
typedef union
{
    uint8_t byte;
    struct
    {
#if (CPU_BITORDER == CPU_LSBFIRST)
        uint8_t ep_num         : 4;    /* endpoint number */
        uint8_t reserved       : 3;    /* reserve(0) */
        uint8_t direction      : 1;    /* transfer direction */
#elif (CPU_BITORDER == CPU_MSBFIRST)
        uint8_t direction      : 1;    /* transfer direction */
        uint8_t reserved       : 3;    /* reserve(0) */
        uint8_t ep_num         : 4;    /* endpoint number */
#endif
    }bits;
}USBF_UN_EPN_bEndpointAddress;

/* endpoint descriptor attribute field */
typedef union
{
    uint8_t byte;
    struct
    {
#if (CPU_BITORDER == CPU_LSBFIRST)
        uint8_t transfer_type  : 2;    /* transfer type */
        uint8_t reserved       : 6;    /* reserve */
#elif (CPU_BITORDER == CPU_MSBFIRST)
        uint8_t reserved       : 6;    /* reserve */
        uint8_t transfer_type  : 2;    /* transfer type */
#endif
    }bits;
}USBF_UN_EPN_bmAttributes;

/* endpoint descriptor max packet size attribute field */
typedef union
{
    uint16_t word;
    struct
    {
#if (CPU_BITORDER == CPU_LSBFIRST)
        uint16_t pkt_size  : 11;       /* max packet size */
        uint16_t reserved  : 5;        /* reserve */
#elif (CPU_BITORDER == CPU_MSBFIRST)
        uint16_t reserved  : 5;        /* reserve */
        uint16_t pkt_size  : 11;       /* max packet size */
#endif
    }bits;
}USBF_UN_EPN_wMaxPacketSize;
#endif
