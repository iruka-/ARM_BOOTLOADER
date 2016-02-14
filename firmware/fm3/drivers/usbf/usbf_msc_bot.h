/**
* @file          usbf_msc_bot.h
* @brief         BOT process module
* @author        zhangjie
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __USBF_MSC_BOT_INC__
#define __USBF_MSC_BOT_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Constant Definition                                                 */
/*----------------------------------------------------------------------------*/
/* class action stage */
enum
{
    BOT_STAGE_UNINIT,                                       /* uninitialize stage                */
    BOT_STAGE_READY,                                        /* Ready stage                       */
    BOT_STAGE_CBW,                                          /* command tansfer stage (CBW)       */
    BOT_STAGE_NODATA,                                       /* no data transfer                  */
    BOT_STAGE_DATAIN,                                       /* data-in transfer                  */
    BOT_STAGE_DATAOUT,                                      /* data-out transfer                 */
    BOT_STAGE_ERROR,                                        /* error process                     */
    BOT_STAGE_CSW                                           /* status transfer stage (CSW)       */
};

/* transfer type */
enum
{
    TRANSFER_TYPE_NO_TRANSFER,                              /* transfer type(no transfer)        */
    TRANSFER_TYPE_DEVICE_TO_HOST,                           /* transfer type(from device)        */
    TRANSFER_TYPE_HOST_TO_DEVICE                            /* transfer type(from host)          */
};

/* definition for class request  */
/* Bulk-Only Mass Storage Reset */
#define BOT_BULK_ONLY_RESET                 (0xFF)          /* bRequest                          */
#define BOT_BULK_ONLY_RESET_REQUEST_TYPE    (0x21)          /* bmRequestType                     */
#define BOT_BULK_ONLY_RESET_REQUEST_VALUE   (0x0000)        /* wValue                            */
#define BOT_BULK_ONLY_RESET_REQUEST_LENGTH  (0x0000)        /* wLength                           */

/* Get Max LUN */
#define BOT_GET_MAX_LUN_REQUEST             (0xFE)          /* bRequest                          */
#define BOT_GET_MAX_LUN_REQUEST_TYPE        (0xA1)          /* bmRequestType                     */
#define BOT_GET_MAX_LUN_REQUEST_VALUE       (0x0000)        /* wValue                            */
#define BOT_GET_MAX_LUN_REQUEST_LENGTH      (0x0001)        /* wLength                           */

/* definition for CBW */
#define BOT_CBW_SIZE                        (31)            /* CBW size                          */
#define BOT_CBW_SIGNATURE                   (0x43425355U)   /* CBW signature                     */
#define BOT_CBWFLAGS_DEVICE_TO_HOST         (0x80)          /* CBW flag: device to host          */
#define BOT_CBWFLAGS_HOST_TO_DEVICE         (0x00)          /* CBW flag: host to device          */
#define BOT_CBWCBLENGTH_MIN                 (0x01)          /* min value of CBWCB command length */
#define BOT_CBWCBLENGTH_MAX                 (0x10)          /* max value of CBWCB command length */

/* definition for CSW */
#define BOT_CSW_SIZE                        (13)            /* CSW size                          */
#define BOT_CSW_SIGNATURE                   (0x53425355U)   /* CSW signature                     */

/* status of csw status */
#define BOT_CSW_STATUS_GOOD                 (0U)            /* good status                       */
#define BOT_CSW_STATUS_FAIL                 (1U)            /* fail status                       */
#define BOT_CSW_STATUS_PHASE_ERROR          (2U)            /* phase error                       */


/*----------------------------------------------------------------------------*/
/* Global Variable Definition                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Type Definition                                                            */
/*----------------------------------------------------------------------------*/
/* CBW format */
typedef struct _BOT_ST_CBW
{
    uint32_t  dCBWSignature;               /*  0: contains 'USBC'                    */
    uint32_t  dCBWTag;                     /*  4: unique per command id              */
    uint32_t  dCBWDataTransferLength;      /*  8: size of the data                   */
    uint8_t   bmCBWFlags;                  /* 12: direction in bit 7                 */
    uint8_t   bCBWLUN;                     /* 13: LUN (normally 0)                   */
    uint8_t   bCBWCBLength;                /* 14: length of CDB, <= MAX_COMMAND_SIZE */
    uint8_t   CBWCB[16];                   /* 15: command data block                 */
} BOT_ST_CBW;

/* CSW format*/
typedef struct _BOT_ST_CSW
{
    uint32_t  dCSWSignature;               /*  0: signature, should be 'USBS'                              */
    uint32_t  dCSWTag;                     /*  4: tag, same as original command                            */
    uint32_t  dCSWDataResidue;             /*  8: the difference between the amount of data                */
                                           /*     expected (as stated in cbw->DataTransferLength)          */
                                           /*     and the actual amount of data processed by the device    */
    uint8_t   bCSWStatus;                  /* 12: status 0:GOOD 1:FAILED 2:Phase Error(repeat the command) */
} BOT_ST_CSW;

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/
/* BOT transfer control module initialize process */
extern void msc_bot_init(uint8_t * const data_buffer_ptr, uint32_t data_buffer_size);

/* get BOT transfer stage process */
extern uint8_t msc_bot_get_stage(void);

/* MSC class request process */
extern uint8_t msc_bot_class_request(const USBF_ST_SETUP_INFO *class_request_ptr);

/* BOT transfer main process */
extern void msc_bot_main(void);

/* BOT data transfer complete process */
extern void msc_bot_trans_complete(uint16_t count, uint8_t complete_status);

#endif   /* __USBF_MSC_BOT_INC__ */
