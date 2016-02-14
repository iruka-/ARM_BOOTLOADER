/**
* @file          usbf_msc_config.h
* @brief         user customs module configuration
* @author        huangshan
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/
#ifndef __USBF_MSC_CONFIG_INC__
#define __USBF_MSC_CONFIG_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Configuration  (variable)                                                  */
/*----------------------------------------------------------------------------*/

/******************************************************************************/
/* INQUIRY commmand header ID                                                 */
/* possible value:any one-byte character                                      */
/*    because of 8 character fixation,the shortfall stuffs with one-byte space*/
/******************************************************************************/
#define USBF_MSC_VENDOR_ID        'F','U','J','I','T','S','U',' '

/******************************************************************************/
/* INQUIRY command product ID                                                 */
/* possible value:any one-byte character                                      */
/*    because of16 character fixation,the shortfall stuffs with one-byte space*/
/******************************************************************************/
#define USBF_MSC_PRODUCT_ID        'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '

/******************************************************************************/
/* INQUIRY command product revision                                           */
/* possible value: any one-byte character                                     */
/*    because of 4 character fixation,the shortfall stuffs with one-byte space*/
/******************************************************************************/
#define USBF_MSC_PRODUCT_REVISION_LEVEL        '0','0','0','1'

/******************************************************************************/
/* used LUN max numbers                                                       */
/* possible value:  1~16                                                      */
/******************************************************************************/
#define USBF_MSC_LUN_NUM                         (1)

/******************************************************************************/
/* number of interface that acts MSC driver                                   */
/* possible value:  0~254                                                     */
/******************************************************************************/
#define USBF_MSC_INTERFACE_ID                    (0x00)

/******************************************************************************/
/* number of control endpoint that acts MSC driver                            */
/* possible value:  only USBF_EP0                                             */
/******************************************************************************/
#define USBF_MSC_CTRL_EP                         (USBF_EP0)

/******************************************************************************/
/* number of bulk-out endpoint that acts MSC driver                           */
/* possible value:  USBF_EP1 ~ USBF_EP3                                       */
/******************************************************************************/
#define USBF_MSC_BULKOUT_EP                      (USBF_EP1)

/******************************************************************************/
/* number of bulk-in endpoint that acts MSC driver                            */
/* possible value:  USBF_EP1 ~ USBF_EP3                                       */
/******************************************************************************/
#define USBF_MSC_BULKIN_EP                       (USBF_EP2)

/******************************************************************************/
/* control endpoint packet size                                               */
/* possible value:  8/16/32/64                                                */
/******************************************************************************/
#define USBF_MSC_CTRL_EP_PKT_SIZE                (64)

/******************************************************************************/
/* bulk-out endpoint packet size                                              */
/* possible value:  8/16/32/64                                                */
/******************************************************************************/
#define USBF_MSC_BULKOUT_EP_PKT_SIZE             (64)

/******************************************************************************/
/* bulk-in endpoint packet size                                               */
/* possible value:  8/16/32/64                                                */
/******************************************************************************/
#define USBF_MSC_BULKIN_EP_PKT_SIZE              (64)

/******************************************************************************/
/* when not two or more LUN,response to request of[Get Max LUN]               */
/* possible value:  USBF_MSC_ENABLE    stall control endpoint                 */
/*                  USBF_MSC_DISABLE   response by 0                          */
/******************************************************************************/
#define USBF_MSC_SINGLE_LUN_STALL                (USBF_MSC_DISABLE)

#endif   /* __USBF_MSC_CONFIG_INC__ */
