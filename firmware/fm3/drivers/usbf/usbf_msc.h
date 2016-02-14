/**
* @file          usbf_msc.h
* @brief         MSC driver API control module
* @author        maxiaodog
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __USBF_MSC_INC__
#define __USBF_MSC_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

#include "usbf.h"
#include "usbf_msc_config.h"

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

/* return value of API function */
#define USBF_MSC_RET_OK            (0x00)               /* normally end */
#define USBF_MSC_RET_STAGE_NG      (0x01)               /* class action stage unnormal */
#define USBF_MSC_RET_INVALID_PARAM (0x02)               /* parameter incorrect */
#define USBF_MSC_RET_USBFUNC_NG    (0x04)               /* USB function driver action abnormal */
#define USBF_MSC_RET_LUNINFO_NG    (0x08)               /* LUN information incorrect */
#define USBF_MSC_RET_NG            (0x10)               /* unnormally end */

/* definition for user customizing file */
#define USBF_MSC_ENABLE            (0x00)               /* use function */
#define USBF_MSC_DISABLE           (0x01)               /* do not use function */

/* event type */
enum
{
    USBF_MSC_MEDIUM_EVENT_SET_REMOVAL,               /* medium removal set event */
    USBF_MSC_MEDIUM_EVENT_SET_POWER_CONDITION,       /* power mode set event */
    USBF_MSC_MEDIUM_EVENT_VERIFY,                    /* verify event */
    USBF_MSC_MEDIUM_EVENT_SET_CACHE_INFO             /* cache information set event */
};

/*----------------------------------------------------------------------------*/
/* Global Variable Definition                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/

/* structure definition for information of LUN */
typedef struct _USBF_MSC_ST_LUN_DATA
{
    uint32_t   BlockNum;      /* block numbers */
    uint32_t   BlockSize;     /* block size */
    uint8_t    MediaStatus;   /* media status */
}USBF_MSC_ST_LUN_DATA;

/* callback function */
/* media driver LUN information get function */
typedef uint8_t USBF_MSC_CALLBACK_MEDIA_GETLUNINFO(uint8_t Lun, USBF_MSC_ST_LUN_DATA* pLunData);

/* media driver read control function */
typedef uint8_t USBF_MSC_CALLBACK_MEDIA_READ(uint8_t Lun, uint32_t Lba, const uint8_t* pDataBuffer);

/* media driver write control function */
typedef uint8_t USBF_MSC_CALLBACK_MEDIA_WRITE(uint8_t Lun, uint32_t Lba, const uint8_t* pDataBuffer);

/* media driver event notification function */
typedef uint8_t USBF_MSC_CALLBACK_MEDIA_EVENT(uint8_t Lun, uint16_t EventType, void* pEventInfo);

/* structure definition for MSC class driver callback function */
typedef struct _USBF_MSC_ST_CALLBACK_HANDLER
{
    USBF_MSC_CALLBACK_MEDIA_GETLUNINFO  *MediaGetLunInfoHandler;       /* media driver LUN information get function address */
    USBF_MSC_CALLBACK_MEDIA_READ        *MediaReadHandler;             /* media driver read control function address */
    USBF_MSC_CALLBACK_MEDIA_WRITE       *MediaWriteHandler;            /* media driver write control function address */
    USBF_MSC_CALLBACK_MEDIA_EVENT       *MediaEventHandler;            /* media driver event notification function address */
} USBF_MSC_ST_CALLBACK_HANDLER;

/* API function */
/* MSC driver initialize process */
extern uint8_t USBF_MSC_Init(uint8_t * const pDataBuffer,
                             uint32_t DataBufferSize,
                             const USBF_MSC_ST_CALLBACK_HANDLER *pCallbackHandler);

/* MSC driver class request process */
extern uint8_t USBF_MSC_Request(const USBF_ST_SETUP_INFO * pSetupPkt);

/* MSC driver polling process */
extern uint8_t USBF_MSC_Polling(void);

/* MSC driver data transfer complete notification */
extern uint8_t USBF_MSC_TransComplete(uint8_t EpId, uint16_t Count, uint8_t CompleteStatus);

/* structure definition for power mode information */
typedef struct _USBF_MSC_ST_POWER_CONDITION
{
    uint8_t  PowerCondition;                  /* POWER CONDITION */
    uint8_t  LoadEject;                       /* LOAD EJECT */
    uint8_t  Start;                           /* START */
}USBF_MSC_ST_POWER_CONDITION;

/* structure definition for medium removal information */
typedef struct _USBF_MSC_ST_SET_MEDIUM_REMOVAL
{
    uint8_t  RemovalType;                     /* REMOVAL TYPE */
}USBF_MSC_ST_SET_MEDIUM_REMOVAL;

/* structure definition for verify information */
typedef struct _USBF_MSC_ST_VERIFY
{
    uint32_t  StartLba;                       /* START LBA */
    uint16_t  VerifyLength;                   /* VERIFY LENGTH */
    uint8_t   VrprotectCode;                  /* VRPROTECT CODE */
}USBF_MSC_ST_VERIFY;

/* structure definition for cache information */
typedef struct _USBF_MSC_ST_CACHE_INFO
{
    uint8_t OperationCode;                   /* OPERATION CODE */
    uint8_t DisablePageOut;                  /* DISABLE PAGE OUT */
    uint8_t ForceUnitAccess;                 /* FORCE UNIT ACCESS */
}USBF_MSC_ST_CACHE_INFO;
#endif   /* __USBF_MSC_INC__ */
