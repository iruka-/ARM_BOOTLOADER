/**
* @file          usbf.h
* @brief         low layer USB function API control module
* @author        huangshan
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __USBF_INC__
#define __USBF_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "usbf_config.h"
#include "cpu_config.h"
#include "usbf_spec.h"

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/
/* endpoint number */
#define USBF_EP0                     (0x00)      /* endpoint number0 */
#define USBF_EP1                     (0x01)      /* endpoint number1 */
#define USBF_EP2                     (0x02)      /* endpoint number2 */
#define USBF_EP3                     (0x03)      /* endpoint number3 */
#define USBF_EP4                     (0x04)      /* endpoint number4 */
#define USBF_EP5                     (0x05)      /* endpoint number5 */

/* max endpoint numbers */
#define USBF_MAX_EP_NUM              ((uint8_t)0x05)  /* MB9B500 max endpoint numbers (excluding EP0) */

/* error type */
#define USBF_ERROR_SETUP             ((uint8_t)0x00)  /* setup packet analysis error */
#define USBF_ERROR_STATUS            ((uint8_t)0x02)  /* USB function driver action status transition error */

/* event type */
#define USBF_EVENT_BUSRESET          ((uint8_t)0x00)  /* bus reset */
#define USBF_EVENT_SETUP             ((uint8_t)0x01)  /* setup packet */
#define USBF_EVENT_SOF               ((uint8_t)0x02)  /* SOF */
#define USBF_EVENT_SETCONFIG         ((uint8_t)0x03)  /* configuration */
#define USBF_EVENT_SUSPEND           ((uint8_t)0x04)  /* susupend */
#define USBF_EVENT_HOSTWAKEUP        ((uint8_t)0x05)  /* wakeup from host */

/* transfer complete */
#define USBF_BUFFER_EMPTY            ((uint8_t)0x00)  /* transfer buffer empty */
#define USBF_BUFFER_FULL             ((uint8_t)0x01)  /* transfer buffer full */
#define USBF_TRANS_SPK_NOTSEND       ((uint8_t)0x02)  /* no short packet send */
#define USBF_TRANS_COMPLETE          ((uint8_t)0x03)  /* transfer normally complete */
#define USBF_TRANS_CANCELED          ((uint8_t)0x04)  /* transfer canceled */
#define USBF_TRANS_STOPPED           ((uint8_t)0x05)  /* transfer abandoned */
#define USBF_TRANS_NG                ((uint8_t)0x06)  /* transfer unnormally complete */

/* endpoint attribute storage buffer size */
#if (CPU_TYPE == CPU_TYPE_16)
#define USBF_EPINFO_SIZE             ((uint8_t)14u)
#elif (CPU_TYPE == CPU_TYPE_32)
#define USBF_EPINFO_SIZE             ((uint8_t)16u)
#else
#endif

/* set/clear of stall status */
#define USBF_SET_STALL               ((uint8_t)0x00)  /* stall status set */
#define USBF_CLEAR_STALL             ((uint8_t)0x01)  /* stall status clear */

/* return value of API function */
#define USBF_RET_OK                  ((uint8_t)0x00)  /* successfully complete */
#define USBF_RET_USBSTATUS_NG        ((uint8_t)0x01)  /* USBF driver action state abnormal */
#define USBF_RET_INVALID_PARAM       ((uint8_t)0x02)  /* parameter invalid */
#define USBF_RET_EPSTATUS_NG         ((uint8_t)0x04)  /* specified endpoinit transfer status abnormal */
#define USBF_RET_INVALID_DESC        ((uint8_t)0x08)  /* USB descriptor invalid */

/* low layer USB function driver action status */
#define USBF_STATUS_UNINIT           ((uint8_t)0x00)  /* uninitialize*/
#define USBF_STATUS_INIT             ((uint8_t)0x01)  /* initialized */
#define USBF_STATUS_CONNECT          ((uint8_t)0x02)  /* connect */
#define USBF_STATUS_DEFAULT          ((uint8_t)0x03)  /* default */
#define USBF_STATUS_ENUM             ((uint8_t)0x04)  /* enumeration */
#define USBF_STATUS_CONFIG           ((uint8_t)0x05)  /* configuration */
#define USBF_STATUS_SUSPEND          ((uint8_t)0x06)  /* suspend */
#define USBF_STATUS_ERROR            ((uint8_t)0x07)  /* error */

/* endpoint transfer status */
#define USBF_EPSTATUS_IDLE           ((uint8_t)0x07)  /* idle */
#define USBF_EPSTATUS_TRANS          ((uint8_t)0x08)  /* transfering */
#define USBF_EPSTATUS_STALL          ((uint8_t)0x09)  /* stall */

/* VBUS connect status */
#define USBF_VBUS_CONNECT            ((uint8_t)0x01)  /* VBUS connect status */
#define USBF_VBUS_NOTCONNECT         ((uint8_t)0x00)  /* VBUS disconnect status */

/* pull-up resistor disconnect set */
#define USBF_PULLUP_NOTCONNECT       (!USBF_PULLUP_CONNECT)

/*----------------------------------------------------------------------------*/
/* Global Variable Definition                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/

/* string descriptor */
typedef struct _USBF_ST_STRING_DESCRIPTOR
{
    uint8_t        StringDescriptorIndex;  /* index of string descriptor */
    uint8_t *      StringDescriptorInfo;   /* string descriptor array storage */
}USBF_ST_STRING_DESCRIPTOR;

/* error status information */
typedef struct _USBF_ST_STATUSERROR_INFO
{
    uint8_t        UsbStatus;              /* current USB function driver action status */
    uint8_t        UsbEvent;               /* event occur */
}USBF_ST_STATUSERROR_INFO;

/* descriptor */
typedef struct _USBF_ST_USB_DESCRIPTOR
{
    /* device descriptor storage */
    uint8_t *  DeviceDescriptor;

    /* configuration descriptor array storage */
    uint8_t *  ConfigurationDescriptor;

    /* string descriptor array storage */
    USBF_ST_STRING_DESCRIPTOR *    StringDescriptor;

    /* string descriptor numbers */
    uint8_t    StringDescriptorNum;

}USBF_ST_USB_DESCRIPTOR;

/* request receive notification function */
typedef void USBF_CALLBACK_REQUEST(USBF_ST_SETUP_INFO * Setup);

/* transfer complete notification function */
typedef void USBF_CALLBACK_TRANS_COMPLETE(uint8_t EpId, uint16_t Count, uint8_t CompleteStatus);

/* USB receive event notification function */
typedef void USBF_CALLBACK_EVENT(uint8_t EventType, void* EventInfo);

/* error notification function */
typedef void USBF_CALLBACK_ERROR (uint8_t ErrorType, void * ErrorInfo);

/* callback function */
typedef struct _USBF_ST_CALLBACK_HANDLER
{
    USBF_CALLBACK_REQUEST         *   RequestHandler;            /* request receive notification */
    USBF_CALLBACK_TRANS_COMPLETE  *   TransCompleteHandler;      /* transfer complete notification */
    USBF_CALLBACK_EVENT           *   EventHandler;              /* event notification */
    USBF_CALLBACK_ERROR           *   ErrorHandler;              /* error notification */
}USBF_ST_CALLBACK_HANDLER;

/* low layer USB function driver initialize */
extern uint8_t USBF_Init(USBF_ST_USB_DESCRIPTOR * pUsbDescriptor,
                         uint8_t * pEpBuffer, uint16_t EpBufferSize,
                         USBF_ST_CALLBACK_HANDLER * pCallback);

/* low layer USB function driver connect request */
extern uint8_t USBF_Attach(void);

/* low layer USB function driver disconnect request */
extern uint8_t USBF_UnAttach(void);

#if (USBF_SOF_ENABLE == USBF_ENABLE)
/* get frame number request */
extern uint8_t USBF_GetFrameNum(uint16_t * pFrameNum);
#endif

#if (USBF_VBUS_CTRL == USBF_ENABLE)
/* get USB function driver VBUS status */
extern uint8_t USBF_GetVbusStatus(void);
#endif

/* send request of USB function driver endpoint */
extern uint8_t USBF_Send(uint8_t EpId, uint8_t * pBuffer, uint16_t Size);

/* receive request of USB function driver endpoint */
extern uint8_t USBF_Receive(uint8_t EpId, uint8_t * pBuffer, uint16_t Size);

/* transfer stop request of SB function driver endpoint */
extern uint8_t USBF_AbortTrans(uint8_t EpId);

/* stall set of USB function driver endpoint */
extern uint8_t USBF_SetStall(uint8_t EpId, uint8_t SetValue);

/* get USB function driver action status */
extern uint8_t  USBF_GetStatus(void);

/* get USB function driver transfer status */
extern uint8_t USBF_GetEpStatus(uint8_t EpId, uint8_t * pEpStatus);

/* resume set request of USB function driver */
extern uint8_t USBF_Resume(void);

#if (CPU_TYPE == CPU_TYPE_16)
/* control transfer interrupt process */
void USBF_UsbFunction1_IRQHandler(void);

/* data transfer interrupt process */
void USBF_UsbFunction2_IRQHandler(void);

/* each USB event interrput process */
void USBF_UsbFunction3_IRQHandler(void);

/* endpointN SPK interrupt process */
void USBF_UsbFunction4_IRQHandler (void);
#elif (CPU_TYPE == CPU_TYPE_32)
/* USB function(endpointN DRQ)interrupt process */
void USBF_UsbFunction1_IRQHandler(void);

/* USB function(DRQI,DRQO and each status)interrupt process */
void USBF_UsbFunction2_IRQHandler(void);
#endif

#endif
