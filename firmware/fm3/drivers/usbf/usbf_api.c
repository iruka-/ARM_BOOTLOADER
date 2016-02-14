/**
* @file          usbf_api.c
* @brief         API  module
* @author        huangshan
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "usbf.h"
#include "usbf_core_event.h"
#include "usbf_core_control_transfer.h"
#include "usbf_core_data_transfer.h"
#include "usbf_core_stdrequest.h"
#include "usbf_hal_io.h"
#include "usbf_hal_int.h"
#include "usbf_core_descriptor_analyze.h"

/*----------------------------------------------------------------------------*/
/* Global Variable Definition                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

#define USBF_NO_EP   ((uint8_t)0xFF)  /* no endpoint number */

enum EM_API_ID          /* caller ID definition                           */
{
    API_INIT,           /* initialize API function ID                     */
    API_GET_VBUSSTATUS, /* get VBUS state function ID                     */
    API_GET_FRAMENUM,   /* get frame number function ID                   */
    API_ATTACH,         /* attach request API function ID                 */
    API_UNATTACH,       /* unattach request API function ID               */
    API_SEND,           /* EP sending request API function ID             */
    API_RECEIVE,        /* EP receiving API function ID                   */
    API_ABORT_TRANS,    /* EP transfer stop API function ID               */
    API_SETSTALL,       /* set EP stall API function ID                   */
    API_GET_STATUS,     /* get action state API function ID               */
    API_GET_EPSTATUS,   /* get EP transfer state API function ID          */
    API_RESUME          /* set resume request API function ID             */
};

/*----------------------------------------------------------------------------*/
/* Static Variable Definition                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Type Definition                                                            */
/*----------------------------------------------------------------------------*/
/* endpoint information structure definition */
typedef struct _ST_EP_INFO
{
    uint8_t    ep_valid;        /* endpoint effictive flag                         */
    uint8_t    ep0_stage;       /* endpoint transfer stage (only endpoint0)        */
    uint8_t    epn_direction;   /* endpoint transfer direction(excluding endpoint0)*/
    uint8_t    status;          /* endpoint transferstate                          */
    uint16_t   pkt_size;        /* endpoint packet size                            */
} ST_EP_INFO;

/* structure definition for l USB function driver initialize parameter  check */
typedef struct _ST_INIT_PARAM
{
    USBF_ST_USB_DESCRIPTOR   * usb_descriptor;    /* USB descriptor information storage*/
    uint8_t                  * ep_buffer;         /* endpoint attribute buffer storage */
    uint16_t                   ep_buffer_size;    /* endpoint attribute buffer size    */
    USBF_ST_CALLBACK_HANDLER * callback_handler;  /* callback funtion structure address*/
} ST_INIT_PARAM;

/* structure definition for endpoint sending request parameter check of USB function driver*/
typedef struct _ST_SEND_PARAM
{
    uint8_t      ep_id;                          /* specified endpoint number         */
    uint8_t    * buffer;                         /* send data buffer storage          */
    uint16_t     size;                           /* send data size                    */
    ST_EP_INFO * ep_info;                        /* API function information pointer  */
} ST_SEND_PARAM;

/* structure definition for endpoint receiving request parameter check of USB function driver */
typedef struct _ST_RECEIVE_PARAM
{
    uint8_t       ep_id;                         /* specified endpoint number         */
    uint8_t     * buffer;                        /* receive data buffer storage       */
    uint16_t      size;                          /* receive data buffer size          */
    ST_EP_INFO  * ep_info;                      /* API function information pointer   */
} ST_RECEIVE_PARAM;

/* structure definition for endpoint transfer stop request parameter check of USB function driver */
typedef struct _ST_ABORT_TRANS_PARAM
{
    uint8_t       ep_id;                         /* specified endpoint number         */
    ST_EP_INFO  * ep_info;                       /* API function information pointer  */
} ST_ABORT_TRANS_PARAM;

/* structure definition for endpoint set stall request parameter check of USB function driver */
typedef struct _ST_SETSTALL_PARAM
{
    uint8_t       ep_id;                         /* specified endpoint number         */
    uint8_t       set_value;                     /*  set/clear stall state            */
    ST_EP_INFO  * ep_info;                       /* API function information pointer  */
} ST_SETSTALL_PARAM;

/* structure definition for get endpoint transfer state parameter check of USB function driver */
typedef struct _ST_GET_EPSTATUS_PARAM
{
    uint8_t       ep_id;                         /* specified endpoint number         */
    uint8_t     * ep_status;                     /* endpoint transfer state pointer   */
    ST_EP_INFO  * ep_info;                       /* API function information pointer  */
} ST_GET_EPSTATUS_PARAM;

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/
/* check USB function driver action state */
static uint8_t check_status(uint8_t ep_id, uint8_t api_id);
/* check API function parameter */
static uint8_t check_param(uint8_t api_id, void const * api_info);
/* check EP transfer state  */
static uint8_t check_epstatus(uint8_t api_id, uint8_t ep_id, const ST_EP_INFO * ep_info);
/* check specified endpoint number */
static uint8_t check_epid(uint8_t ep_id, ST_EP_INFO * ep_info);

/*----------------------------------------------------------------------------*/
/*  Functions                                                                 */
/*----------------------------------------------------------------------------*/
/**
* @brief            USB function driver initialize
*
* @param            pUsbDescriptor   : USB descriptor information storage
* @param            pEpBuffer        : endpoint attribute buffer storage
* @param            EpBufferSize     : endpoint attribute buffer size
* @param            pCallback        : callback funtion table
* @return           USBF_RET_OK           :successfully complete
* @return           USBF_RET_USBSTATUS_NG :USBF driver action state abnormal
* @return           USBF_RET_INVALID_PARAM:parameter invalid
* @return           USBF_RET_INVALID_DESC :USB descriptor invalid
* @return          set the return value as follows
* @return          B'0000XXXX
* @return             |||||||+--USBF driver action state abnormal
* @return             ||||||+---parameter invalid
* @return             |||||+----specified endpoint transfer state abnormal
* @return             ||||+-----USB descriptor invalid
* @return             ||+------reserve (definite value 0)
* @return             |+-------reserve (definite value 0)
* @return             +--------reserve (definite value 0)
*
**/
uint8_t USBF_Init(USBF_ST_USB_DESCRIPTOR * pUsbDescriptor,
                      uint8_t * pEpBuffer, uint16_t EpBufferSize,
                      USBF_ST_CALLBACK_HANDLER * pCallback)
{
    uint8_t         ret = USBF_RET_OK; /* return value */
    uint8_t         func_ret;          /* return value of call function*/
    ST_INIT_PARAM   api_info;          /* API info */

    /* API information initialize */
    api_info.usb_descriptor   = pUsbDescriptor;
    api_info.ep_buffer        = pEpBuffer;
    api_info.ep_buffer_size   = EpBufferSize;
    api_info.callback_handler = pCallback;

    /* check USB function driver action state,and if result is unnormality,set USB action state unnormality bit of return value  */
    func_ret = check_status(USBF_NO_EP, API_INIT);
    if (func_ret != TRUE)
    {
        ret |= USBF_RET_USBSTATUS_NG;
    }

    /* check parameter,and if result is incorrect,set parameter incorrect bit of return value */
    func_ret = check_param(API_INIT, (void*)&api_info);
    if (func_ret != TRUE)
    {
        ret |= USBF_RET_INVALID_PARAM;
    }

    /* check descriptor information,and if information is incorrect,set USB descriptor incorrect bit of return value */
    if (pUsbDescriptor == NULL)
    {
        ret |= USBF_RET_INVALID_DESC;
    }
    else
    {
        func_ret = usbfunc_descriptor_judge_desc(pUsbDescriptor, pEpBuffer, EpBufferSize);
        if (func_ret != TRUE)
        {
            ret |= USBF_RET_INVALID_DESC;
        }
    }

    /* when result is correct */
    if (ret == USBF_RET_OK)
    {
        /* each module initialize */
        usbfunc_reg_init();                     /* USB function marco control module initialize */
        usbfunc_interrupt_init();               /* USB function interrupt process module initialize */
        usbfunc_ctrltrans_init();               /* control transfer module initialize */
        usbfunc_datatrans_init();               /* data transfer control module initialize */
        usbfunc_event_init(pCallback);          /* event process module initialize */
        /* USB function macro start */
        usbfunc_reg_start();
    }

    return (ret);
}

/**
* @brief            get USB function driver VBUS state
*
* @param            none
* @return           USBF_VBUS_CONNECT     -> VBUS connectted state
* @return           USBF_VBUS_NOTCONNECT  -> VBUS unconnectted state
*
**/
#if (USBF_VBUS_CTRL == USBF_ENABLE)
uint8_t USBF_GetVbusStatus(void)
{
    uint8_t    ret;    /* return value */

     /* get USB function driver VBUS state */
    ret = usbfunc_reg_get_vbusstatus();

    return (ret);
}
#endif

/**
* @brief            get frame number
*
* @param            pFrameNum      : poiter to frame num
* @return           USBF_RET_OK           :successfully complete
* @return           USBF_RET_USBSTATUS_NG :USBF driver action state abnormal
* @return           set the return value as follows
* @return           B'0000XXXX
* @return             |||||||+--USBF driver action state abnormal
* @return             ||||||+---parameter invalid
* @return             |||||+----specified endpoint transfer state abnormal
* @return             ||||+-----USB descriptor invalid
* @return             |||+------reserve (definite value 0)
* @return             ||+-------reserve (definite value 0)
* @return             |+--------reserve (definite value 0)
* @return             +---------reserve (definite value 0)
*
**/
#if (USBF_SOF_ENABLE == USBF_ENABLE)
uint8_t USBF_GetFrameNum(uint16_t * pFrameNum)
{
    uint8_t    ret = USBF_RET_OK;   /* return value */
    uint8_t    func_ret;            /* return value of call function */

    /* check USB function driver action state */
    func_ret = check_status(USBF_NO_EP, API_GET_FRAMENUM);

    if (func_ret == TRUE)
    {
        /* get frame number */
        *pFrameNum = usbfunc_reg_get_sofnum();
    }
    else
    {
        ret |= USBF_RET_USBSTATUS_NG;
    }

    return (ret);
}
#endif

/**
* @brief            USB function driver attach request
*
* @param            none
* @return           USBF_RET_OK           :successfully complete
* @return           USBF_RET_USBSTATUS_NG :USBF driver action state abnormal
* @return           set the return value as follows
* @return           B'0000XXXX
* @return             |||||||+--USBF driver action state abnormal
* @return             ||||||+---parameter invalid
* @return             |||||+----specified endpoint transfer state abnormal
* @return             ||||+-----USB descriptor invalid
* @return             |||+------reserve (definite value 0)
* @return             ||+-------reserve (definite value 0)
* @return             |+--------reserve (definite value 0)
* @return             +---------reserve (definite value 0)
*
**/
uint8_t USBF_Attach(void)
{
    uint8_t ret = USBF_RET_OK;  /* return value */
    uint8_t func_ret;           /* return value of call function */

    /* check USB function driver action state  */
    func_ret = check_status(USBF_NO_EP, API_ATTACH);
    if (func_ret == TRUE)
    {
        /* attach notification to host */
        usbfunc_event_turn_on();
    }
    else
    {
        ret |= USBF_RET_USBSTATUS_NG;
    }

    return (ret);
}

/**
* @brief            USB function driver unattach request
*
* @param            none
* @return           USBF_RET_OK           :successfully complete
* @return           USBF_RET_USBSTATUS_NG :USBF driver action state abnormal
* @return           set the return value as follows
* @return           B'0000XXXX
* @return             |||||||+--USBF driver action state abnormal
* @return             ||||||+---parameter invalid
* @return             |||||+----specified endpoint transfer state abnormal
* @return             ||||+-----USB descriptor invalid
* @return             |||+------reserve (definite value 0)
* @return             ||+-------reserve (definite value 0)
* @return             |+--------reserve (definite value 0)
* @return             +---------reserve (definite value 0)
*
**/
uint8_t USBF_UnAttach(void)
{
    uint8_t ret = USBF_RET_OK;  /* return value */
    uint8_t func_ret;           /* return value of call function */

    /* check USB function driver action state  */
    func_ret = check_status(USBF_NO_EP, API_UNATTACH);
    if (func_ret == TRUE)
    {
        /* unattach notification to host */
        usbfunc_event_turn_off();
    }
    else
    {
        ret |= USBF_RET_USBSTATUS_NG;
    }

    return (ret);
}

/**
* @brief            USB function driver endpoint sending request
*
* @param            EpId       : specified endpoint numbr
* @param            pBuffer    : send data buffer storag
* @param            Size       : send data size
* @return           USBF_RET_OK           :successfully complete
* @return           USBF_RET_USBSTATUS_NG :USBF driver action state abnormal
* @return           USBF_RET_INVALID_PARAM:parameter invalid
* @return           USBF_RET_EPSTATUS_NG  :specified endpoint transfer state abnormal
* @return           set the return value as follows
* @return           B'0000XXXX
* @return             |||||||+--USBF driver action state abnormal
* @return             ||||||+---parameter invalid
* @return             |||||+----specified endpoint transfer state abnormal
* @return             ||||+-----USB descriptor invalid
* @return             |||+------reserve (definite value 0)
* @return             ||+-------reserve (definite value 0)
* @return             |+--------reserve (definite value 0)
* @return             +---------reserve (definite value 0)
*
**/
uint8_t USBF_Send(uint8_t EpId, uint8_t * pBuffer,uint16_t Size)
{
    ST_EP_INFO      epinfo;              /* EP info */
    uint8_t         func_ret;            /* return value of call function */
    ST_SEND_PARAM   api_info;            /* API info */
    uint8_t         ret = USBF_RET_OK;   /* return value */

    /* API information initialize */
    api_info.ep_id   = EpId;
    api_info.buffer  = pBuffer;
    api_info.size    = Size;
    api_info.ep_info = &epinfo;

    /* check USB function driver action state  */
    func_ret = check_status(EpId, API_SEND);
    if (func_ret != TRUE)
    {
        ret |= USBF_RET_USBSTATUS_NG;
    }

    /* check parameter */
    func_ret = check_param(API_SEND, (void*)&api_info);
    if (func_ret != TRUE)
    {
        ret |= USBF_RET_INVALID_PARAM;
    }

    if (api_info.ep_info->ep_valid == TRUE)
    {
        /* check EP transfer state  */
        func_ret = check_epstatus(API_SEND, EpId, &epinfo);
        if (func_ret != TRUE)
        {
            ret |= USBF_RET_EPSTATUS_NG;
        }
    }
    else
    {
        ret |= USBF_RET_EPSTATUS_NG;
    }

    /* when result is normality */
    if (ret == USBF_RET_OK)
    {
        /* if it is EP0,control sendind request */
        if (EpId == USBF_EP0)
        {
            usbfunc_ctrltrans_send(pBuffer, Size);
        }
        /* if it is not EP0,data sendind request */
        else
        {
            usbfunc_datatrans_send(EpId, pBuffer, Size);
        }
    }

    return (ret);
}

/**
* @brief            USB function driver endpoint receiving request
*
* @param            EpId      : specified endpoint number
* @param            pBuffer   : receive data buffer storage
* @param            Size      : receive data size
* @return           USBF_RET_OK           :successfully complete
* @return           USBF_RET_USBSTATUS_NG :USBF driver action state abnormal
* @return           USBF_RET_INVALID_PARAM:parameter invalid
* @return           USBF_RET_EPSTATUS_NG  :specified endpoint transfer state abnormal
* @return           set the return value as follows
* @return           B'0000XXXX
* @return             |||||||+--USBF driver action state abnormal
* @return             ||||||+---parameter invalid
* @return             |||||+----specified endpoint transfer state abnormal
* @return             ||||+-----USB descriptor invalid
* @return             |||+------reserve (definite value 0)
* @return             ||+-------reserve (definite value 0)
* @return             |+--------reserve (definite value 0)
* @return             +---------reserve (definite value 0)
*
**/
uint8_t USBF_Receive(uint8_t EpId, uint8_t * pBuffer, uint16_t Size)
{
    ST_EP_INFO          epinfo;              /* EP info */
    uint8_t             func_ret;            /* return value of call function */
    ST_RECEIVE_PARAM    api_info;            /* API info */
    uint8_t             ret = USBF_RET_OK;   /* return value */

    /* API information initialize */
    api_info.ep_id   = EpId;
    api_info.buffer  = pBuffer;
    api_info.size    = Size;
    api_info.ep_info = &epinfo;

    /* check USB function driver action state  */
    func_ret = check_status(EpId, API_RECEIVE);
    if (func_ret != TRUE)
    {
        ret |= USBF_RET_USBSTATUS_NG;
    }

    /* check parameter */
    func_ret = check_param(API_RECEIVE, (void*)&api_info);
    if (func_ret != TRUE)
    {
        ret |= USBF_RET_INVALID_PARAM;
    }

    if (api_info.ep_info->ep_valid == TRUE)
    {
        /* check EP transfer state  */
        func_ret = check_epstatus(API_RECEIVE, EpId, &epinfo);
        if (func_ret != TRUE)
        {
            ret |= USBF_RET_EPSTATUS_NG;
        }
    }
    else
    {
        ret |= USBF_RET_EPSTATUS_NG;
    }

    /* when result is normality */
    if (ret == USBF_RET_OK)
    {
        /* if it is EP0,control receiving request */
        if (EpId == USBF_EP0)
        {
            usbfunc_ctrltrans_receive(pBuffer, Size);
        }
        /* if it is not EP0,control receiving request */
        else
        {
            usbfunc_datatrans_receive(EpId, pBuffer, Size);
        }
    }

    return (ret);
}

/**
* @brief            USB function driver endpoint transfer abort request
*
* @param            EpId     : specified endpoint number
* @return            USBF_RET_OK           :successfully complete
* @return            USBF_RET_USBSTATUS_NG :USBF driver action state abnormal
* @return            USBF_RET_INVALID_PARAM:parameter invalid
* @return            USBF_RET_EPSTATUS_NG  :specified endpoint transfer state abnormal
* @return           set the return value as follows
* @return           B'0000XXXX
* @return             |||||||+--USBF driver action state abnormal
* @return             ||||||+---parameter invalid
* @return             |||||+----specified endpoint transfer state abnormal
* @return             ||||+-----USB descriptor invalid
* @return             |||+------reserve (definite value 0)
* @return             ||+-------reserve (definite value 0)
* @return             |+--------reserve (definite value 0)
* @return             +---------reserve (definite value 0)
*
**/
uint8_t USBF_AbortTrans(uint8_t EpId)
{
    ST_EP_INFO              epinfo;              /* EP info */
    uint8_t                 func_ret;            /* return value of call function */
    ST_ABORT_TRANS_PARAM    api_info;            /* API info */
    uint8_t                 ret = USBF_RET_OK;   /* return value */

    /* API information initialize */
    api_info.ep_id   = EpId;
    api_info.ep_info = &epinfo;

    /* check USB function driver action state  */
    func_ret = check_status(EpId, API_ABORT_TRANS);
    if (func_ret != TRUE)
    {
        ret |= USBF_RET_USBSTATUS_NG;
    }

    /* check parameter */
    func_ret = check_param(API_ABORT_TRANS, (void*)&api_info);
    if (func_ret != TRUE)
    {
        ret |= USBF_RET_INVALID_PARAM;
    }

    if (api_info.ep_info->ep_valid == TRUE)
    {
        /* check EP transfer state  */
        func_ret = check_epstatus(API_ABORT_TRANS, EpId, &epinfo);
        if (func_ret != TRUE)
        {
            ret |= USBF_RET_EPSTATUS_NG;
        }
    }
    else
    {
        ret |= USBF_RET_EPSTATUS_NG;
    }

    /* when result is normality */
    if (ret == USBF_RET_OK)
    {
        /* if it is EP0,control transfer stop */
        if (EpId == USBF_EP0)
        {
            usbfunc_ctrltrans_abort();
        }
        /* if it is not EP0,data transfer stop */
        else
        {
            usbfunc_datatrans_abort(EpId);
        }
    }

    return (ret);
}

/**
* @brief           set/clear USB function driver endpoint stall state
*
* @param           EpId     : specified endpoint number
* @param           SetValue : set/clear tall state
* @return            USBF_RET_OK           :successfully complete
* @return            USBF_RET_USBSTATUS_NG :USBF driver action state abnormal
* @return            USBF_RET_INVALID_PARAM:parameter invalid
* @return           set the return value as follows
* @return           B'0000XXXX
* @return             |||||||+--USBF driver action state abnormal
* @return             ||||||+---parameter invalid
* @return             |||||+----specified endpoint transfer state abnormal
* @return             ||||+-----USB descriptor invalid
* @return             |||+------reserve (definite value 0)
* @return             ||+-------reserve (definite value 0)
* @return             |+--------reserve (definite value 0)
* @return             +---------reserve (definite value 0)
*
**/
uint8_t USBF_SetStall(uint8_t EpId, uint8_t SetValue)
{
    ST_EP_INFO          epinfo;              /* EP info */
    uint8_t             func_ret;            /* return value of call function */
    ST_SETSTALL_PARAM   api_info;            /* API info */
    uint8_t             ret = USBF_RET_OK;   /* return value */

    /* API information initialize */
    api_info.ep_id     = EpId;
    api_info.set_value = SetValue;
    api_info.ep_info   = &epinfo;

    /* check USB function driver action state  */
    func_ret = check_status(EpId, API_SETSTALL);
    if (func_ret != TRUE)
    {
        ret |= USBF_RET_USBSTATUS_NG;
    }

    /* check parameter */
    func_ret = check_param(API_SETSTALL, (void*)&api_info);
    if (func_ret != TRUE)
    {
        ret |= USBF_RET_INVALID_PARAM;
    }

    /* when result is normality */
    if (ret == USBF_RET_OK)
    {
        /* when it is EP0 */
        if (EpId == USBF_EP0)
        {
            /* if it is stall set,set stall */
            if (SetValue == USBF_SET_STALL)
            {
                usbfunc_ctrltrans_setstall();
            }
            /* if it is stall clear,clear stall  */
            else
            {
                usbfunc_ctrltrans_clearstall();
            }
        }
        /* when it is not EP0 */
        else
        {
            /* if it is stall set,accroding to endpoint number ,set stall */
            if (SetValue == USBF_SET_STALL)
            {
                usbfunc_datatrans_setstall(EpId);
            }
            /* if it is stall clear,accroding to endpoint number ,clear stall */
            else
            {
                usbfunc_datatrans_clearstall(EpId);
            }
        }
    }

    return (ret);
}

/**
* @brief            get USB function driver action state
*
* @param            none
* @return          USBF_STATUS_UNINIT  -> uninitialize
* @return          USBF_STATUS_INIT    -> initialized
* @return          USBF_STATUS_CONNECT -> connect
* @return          USBF_STATUS_DEFAULT -> default
* @return          USBF_STATUS_ENUM    -> enumeration
* @return          USBF_STATUS_CONFIG  -> configuration
* @return          USBF_STATUS_SUSPEND -> suspend
* @return          USBF_STATUS_ERROR   -> error
*
**/
uint8_t USBF_GetStatus(void)
{
    uint8_t    ret;    /* return value  */

    /* get USB function driver action state */
    ret = usbfunc_event_get_status();

    return (ret);
}

/**
* @brief            get USB function driver endpoint transfer state
*
* @param            EpId     : specified endpoint number
* @param            pEpStatus : endpoint transfer staet pointer
* @return           USBF_RET_OK           :successfully complete
* @return           USBF_RET_INVALID_PARAM:parameter invalid
* @return           USBF_RET_EPSTATUS_NG  :specified endpoint transfer state abnormal
* @return           set the return value as follows
* @return           B'0000XXXX
* @return             |||||||+--USBF driver action state abnormal
* @return             ||||||+---parameter invalid
* @return             |||||+----specified endpoint transfer state abnormal
* @return             ||||+-----USB descriptor invalid
* @return             |||+------reserve (definite value 0)
* @return             ||+-------reserve (definite value 0)
* @return             |+--------reserve (definite value 0)
* @return             +---------reserve (definite value 0)
*
**/
uint8_t USBF_GetEpStatus(uint8_t EpId, uint8_t * pEpStatus)
{
    ST_EP_INFO              epinfo;              /* EP info */
    uint8_t                 func_ret;            /* return value of call function */
    ST_GET_EPSTATUS_PARAM   api_info;            /* API info */
    uint8_t                 ret = USBF_RET_OK;   /* return value */

    /* API information initialize */
    api_info.ep_id     = EpId;
    api_info.ep_status = pEpStatus;
    api_info.ep_info   = &epinfo;

    /* check USB function driver action state  */
    func_ret = check_status(EpId, API_GET_EPSTATUS);
    if (func_ret != TRUE)
    {
        ret |= USBF_RET_USBSTATUS_NG;
    }

    /* check parameter */
    func_ret = check_param(API_GET_EPSTATUS, (void*)&api_info);
    if (func_ret != TRUE)
    {
        ret |= USBF_RET_INVALID_PARAM;
    }

    /* when result is normality */
    if (ret == USBF_RET_OK)
    {
        /* when it is EP0,get control transfer state */
        if (EpId == USBF_EP0)
        {
            *pEpStatus = usbfunc_ctrltrans_getstatus();
        }
        /* when it is not EP0,get data transfer state */
        else
        {
            *pEpStatus = usbfunc_datatrans_getstatus(EpId);
        }
    }

    return (ret);
}

/**
* @brief            USB function driver resume
*
* @param            none
* @return           USBF_F_RET_OK         :successfully complete
* @return           USBF_RET_USBSTATUS_NG :USBF driver action state abnormal
* @return           set the return value as follows
* @return           B'0000XXXX
* @return             |||||||+--USBF driver action state abnormal
* @return             ||||||+---parameter invalid
* @return             |||||+----specified endpoint transfer state abnormal
* @return             ||||+-----USB descriptor invalid
* @return             |||+------reserve (definite value 0)
* @return             ||+-------reserve (definite value 0)
* @return             |+--------reserve (definite value 0)
* @return             +---------reserve (definite value 0)
*
**/
uint8_t USBF_Resume(void)
{
    uint8_t    ret = USBF_RET_OK;   /* return value */
    uint8_t    func_ret;            /* return value of call function */

    /* check USB function driver action state  */
    func_ret = check_status(USBF_NO_EP, API_RESUME);
    if (func_ret == TRUE)
    {
        /* USB resume process */
        usbfunc_event_resume();
    }
    else
    {
        ret |= USBF_RET_USBSTATUS_NG;
    }

    return (ret);
}

/**
* @brief            check USB function driver action state
*
* @param            ep_id  : specified endpoint number
* @param            api_id : caller ID
* @return          TRUE   -> normal
* @return          FALSE  -> abnormal
*
**/
static uint8_t check_status(uint8_t ep_id, uint8_t api_id)
{
    uint8_t usbf_status;        /* action state */
    uint8_t ret = TRUE;         /* return value */

    /* get USB function driver action state */
    usbf_status = usbfunc_event_get_status();
    switch (api_id)
    {
        case API_INIT:           /* initialize */

            /* if it is not uninitialize,set abnormal to return value */
            if (usbf_status != USBF_STATUS_UNINIT)
            {
                ret = FALSE;
            }

            break;

        case API_ATTACH:         /* connect request */

            /* if it is not initialized,set abnormal to return value */
            if (usbf_status != USBF_STATUS_INIT)
            {
                ret = FALSE;
            }

            break;

        case API_UNATTACH:       /* cut request */
        case API_GET_EPSTATUS:   /* get EP transfer state  */
        case API_GET_FRAMENUM:   /* get frame number */

            /* if it is uninitialize,set abnormal to return value */
            if (usbf_status == USBF_STATUS_UNINIT)
            {
                ret = FALSE;
            }

            break;

        case API_SEND:           /* EP sending request */
        case API_RECEIVE:        /* EP receiving request */
        case API_ABORT_TRANS:    /* EP transfer request */
        case API_SETSTALL:       /* EP stall set */
            /* when it is enumeration */
            if (usbf_status == USBF_STATUS_ENUM)
            {
                /* when USBFUNC_EP0 is not specified,set abnormal to return value */
                if (ep_id != USBF_EP0)
                {
                    ret = FALSE;
                }
            }
            /* when it is not enumeration */
            else
            {
                /* when it is not configuration,set abnormal to return value */
                if (usbf_status != USBF_STATUS_CONFIG)
                {
                    ret = FALSE;
                }
            }

            break;

        case API_RESUME:         /* resume set request */
            /* when it is not suspend,set abnormal to return value */
            if (usbf_status != USBF_STATUS_SUSPEND)
            {
                ret = FALSE;
            }
            /* when remote wakeup is invalidity,set abnormal to return value */
            else if(!usbfunc_descriptor_get_remotewakeup())
            {
                ret = FALSE;
            }
            else
            {
            }
            break;

        case API_GET_VBUSSTATUS: /* get VBUS state,do not check */
        case API_GET_STATUS:     /* get action state,do not check */
            break;

        default :                /* invalidity call origin ID ,do not check */
            break;

    }

    return (ret);
}

/**
* @brief            check API function parameter
*
* @param            api_id   : caller ID
* @param            api_info : API fuction information pointer
* @return          TRUE   -> normal
* @return          FALSE  -> abnormal
*
**/
static uint8_t check_param(uint8_t api_id, void const * api_info)
{
    uint8_t ret = TRUE;  /* return value */

    switch (api_id)
    {
        case API_INIT:           /* intialize */
            /* when descriptor storage is null,set parameter invalid to return value */
            if (((const ST_INIT_PARAM*)api_info)->usb_descriptor == NULL)
            {
                ret = FALSE;
            }

            /* when endpoint attribute buffer size is invalidity,set parameter invalid to return value */
            if ((((const ST_INIT_PARAM*)api_info)->ep_buffer_size != 0x00) &&
                (((const ST_INIT_PARAM*)api_info)->ep_buffer == NULL))
            {
                ret = FALSE;
            }

            /* when callback function structure address is null,set parameter invalid to return value */
            if (((const ST_INIT_PARAM*)api_info)->callback_handler == NULL)
            {
                ret = FALSE;
            }
            else
            {

#if (  (USBF_VENDOR_REQUEST            == USBF_ENABLE) \
    || (USBF_CLASS_REQUEST             == USBF_ENABLE) \
    || (USBF_STD_REQUEST_SETDESCRIPTOR == USBF_ENABLE) \
    || (USBF_STD_REQUEST_SYNCHFRAME    == USBF_ENABLE))

                /* when request receiving notification address,set parameter invalid to return value */
                if (((const ST_INIT_PARAM*)api_info)->callback_handler->RequestHandler == NULL)
                {
                    ret = FALSE;
                }
#endif
                /* event notify function,transfer end notify and error notify function address is null */
                /* set parameter invalid to return value */
                if ((((const ST_INIT_PARAM*)api_info)->callback_handler->EventHandler == NULL) ||
                    (((const ST_INIT_PARAM*)api_info)->callback_handler->TransCompleteHandler == NULL) ||
                    (((const ST_INIT_PARAM*)api_info)->callback_handler->ErrorHandler == NULL))
                {
                    ret = FALSE;
                }
            }
            break;

        case API_SEND:           /* EP sending request */
            /* check specified endpoint number */
            ret = check_epid(((const ST_SEND_PARAM*)api_info)->ep_id,
                             ((const ST_SEND_PARAM*)api_info)->ep_info);

            /* when number is normality,check EPn direction */
            if (ret == TRUE)
            {
                /* when EPn direction is out,set parameter invalid to return value */
                if ((((const ST_SEND_PARAM*)api_info)->ep_id != USBF_EP0) &&
                    (((const ST_SEND_PARAM*)api_info)->ep_info->epn_direction == USBF_DIR_OUT))
                {
                    ret = FALSE;
                }
            }

            /* send data buffer is null,set parameter invalid to return value */
            if ((((const ST_SEND_PARAM*)api_info)->size != 0x00) &&
                (((const ST_SEND_PARAM*)api_info)->buffer == NULL))
            {
                ret = FALSE;
            }
            break;

        case API_RECEIVE:        /* EP receiving request */
            /* check specified endpoint number */
            ret = check_epid(((const ST_RECEIVE_PARAM*)api_info)->ep_id,
                             ((const ST_RECEIVE_PARAM*)api_info)->ep_info);

            /* when number is normality,check receive data size */
            if (ret == TRUE)
            {
                /* when EPn direction is IN,set parameter invalid to return value */
                if ((((const ST_RECEIVE_PARAM*)api_info)->ep_id != USBF_EP0) &&
                    (((const ST_RECEIVE_PARAM*)api_info)->ep_info->epn_direction == USBF_DIR_IN))
                {
                    ret = FALSE;
                }

                /* when size is not satisfied whih request,set parameter invalid to return value */
                if ((((const ST_RECEIVE_PARAM*)api_info)->size == 0x00) ||
                    ((((const ST_RECEIVE_PARAM*)api_info)->size %
                     ((const ST_RECEIVE_PARAM*)api_info)->ep_info->pkt_size) != 0x00))
                {
                    ret = FALSE;
                }
            }

            /* when receive data buffer is null,set parameter invalid to return value */
            if (((const ST_RECEIVE_PARAM*)api_info)->buffer == NULL)
            {
                ret = FALSE;
            }
            break;

        case API_ABORT_TRANS:    /* EP transfer stop request */
            /* check specified endpoint number */
            ret = check_epid(((const ST_ABORT_TRANS_PARAM*)api_info)->ep_id,
                             ((const ST_ABORT_TRANS_PARAM*)api_info)->ep_info);
            break;

        case API_SETSTALL:       /* EP stall set */
            /* check specified endpoint number */
            ret = check_epid(((const ST_SETSTALL_PARAM*)api_info)->ep_id,
                             ((const ST_SETSTALL_PARAM*)api_info)->ep_info);

            /* when parameter set_value is incorrect,set parameter invalid to return value */
            if (((((const ST_SETSTALL_PARAM*)api_info)->set_value) != USBF_SET_STALL) &&
                ((((const ST_SETSTALL_PARAM*)api_info)->set_value) != USBF_CLEAR_STALL))
            {
                ret = FALSE;
            }

            break;

        case API_GET_EPSTATUS:   /* get EP transfer state */
            /* check specified endpoint number */
            ret = check_epid(((const ST_GET_EPSTATUS_PARAM*)api_info)->ep_id,
                             ((const ST_GET_EPSTATUS_PARAM*)api_info)->ep_info);
            /* when parameter ep_status is incorrect,set parameter invalid to return value */
            if (((const ST_GET_EPSTATUS_PARAM*)api_info)->ep_status == NULL)
            {
                ret = FALSE;
            }
            break;

        case API_GET_VBUSSTATUS: /* when get VBUS state,do not check  */
        case API_GET_FRAMENUM:   /* when get frame number,do not check */
        case API_GET_STATUS:     /* when get action state,do not check */
        case API_ATTACH:         /* when connect request,do not check */
        case API_UNATTACH:       /* when cut request,do not check */
        case API_RESUME:         /* when resume set request,do not check */
            break;

        default :                /* invalidity call origin ID,do not check */
            break;

    }

    return (ret);
}

/**
* @brief            check EP transfer state
*
* @param            api_id  : caller ID
* @param            ep_id   : specified endpoint number
* @param            ep_info : specified endpoint information pointer
* @return          TRUE   -> normal
* @return          FALSE  -> abnormal
*
**/
static uint8_t check_epstatus(uint8_t api_id, uint8_t ep_id, const ST_EP_INFO * ep_info)
{
    uint8_t ret = TRUE;             /* return value */

    switch (api_id)
    {
        case API_SEND:           /* EP sending request */
            /* when EP transfer state is not in idle,set EP transfer state abnormal to return value */
            if ((ep_info->status != USBF_EPSTATUS_IDLE)
#if (USBF_AUTO_CLEAR_STALL == USBF_ENABLE)
               && (ep_info->status != USBF_EPSTATUS_STALL))
#else
               )
#endif
            {
                ret = FALSE;
            }

            /* when EP0 transfer stage is not IN,set EP transfer state abnormal to return value */
            if ((ep_id == USBF_EP0) && (ep_info->ep0_stage != CTRLTRANS_STAGE_DATAIN))
            {
                ret = FALSE;
            }

            break;

        case API_RECEIVE:        /* EP receiving request */
            /* when EP transfer state is not in idle,set EP transfer state abnormal to return value */
            if ((ep_info->status != USBF_EPSTATUS_IDLE)
#if (USBF_AUTO_CLEAR_STALL == USBF_ENABLE)
               && (ep_info->status != USBF_EPSTATUS_STALL))
#else
               )
#endif
            {
                ret = FALSE;
            }

            /* when EP0 transfer stage is not OUT,set EP transfer state abnormal to return value */
            if ((ep_id == USBF_EP0) && (ep_info->ep0_stage != CTRLTRANS_STAGE_DATAOUT))
            {
                ret = FALSE;
            }

            break;

        case API_ABORT_TRANS:    /* EP transfer stop request */
#if (USBF_AUTO_CLEAR_STALL != USBF_ENABLE)
            /* when EP transfer state is in idle,set EP transfer state abnormal to return value */
            if (ep_info->status == USBF_EPSTATUS_STALL)
            {
                ret = FALSE;
            }
#endif
            break;

        case API_INIT:           /* when initialize,do not check */
        case API_GET_VBUSSTATUS: /* when get VBUS state,do not check */
        case API_GET_FRAMENUM:   /* when get frame number,do not check */
        case API_ATTACH:         /* when connect request,do not check */
        case API_UNATTACH:       /* when cut request,do not check */
        case API_SETSTALL:       /* when EP stall set,do not check */
        case API_GET_STATUS:     /* when get action state,do not check */
        case API_GET_EPSTATUS:   /* when get EP transfer state,do not check */
        case API_RESUME:         /* when resume set request,do not check */
            break;

        default :                /* invalidity call origin ID,do not check */
            break;
    }

    return (ret);
}

/**
* @brief            check specified endpoint number
*
* @param            ep_id   : specified endpoint number
* @param            ep_info : pointer to specified endpoint information
* @return          TRUE   -> number correct
* @return          FALSE  -> number uncorrect
*
**/
static uint8_t check_epid(uint8_t ep_id, ST_EP_INFO * ep_info)
{
    USBFUNC_ST_CTRLTRANS_EPINFO ep0_info;     /* endpoint0 attribute */
    USBFUNC_ST_DATATRANS_EPINFO epn_info;     /* endpointN attribute */
    uint8_t datatrans_ret;                    /* return value of call function */
    uint8_t ret;                              /* return value */

    /* when this USB function macro is in support range */
    if (ep_id <= USBF_MAX_EP_NUM)
    {
        /* when it is EP0 */
        if (ep_id == USBF_EP0)
        {
            /* get endpoint0 attribute */
            usbfunc_ctrltrans_get_ep0info(&ep0_info);

            /* set attribute */
            ep_info->ep0_stage = ep0_info.trans_stage; /* set transfer stage */
            ep_info->status = ep0_info.trans_status;   /* set transfer state */
            ep_info->pkt_size = ep0_info.pkt_size;     /* set packet size */
            ep_info->ep_valid = TRUE;                  /* set valid flag of endpoint to valid */
            ret = TRUE;
        }
        /* when it is not EP0 */
        else
        {
            /* get endpoint attribute */
            datatrans_ret = usbfunc_datatrans_get_epinfo(ep_id, &epn_info);

            /* when specified endpoint is invalidity,set number incorrect to return value */
            if (datatrans_ret == DATATRANS_UNUSED_EPID)
            {
                ep_info->ep_valid = FALSE;               /* set valid flag of endpoint to invalid */
                ret = FALSE;
            }
            /* when specified endpoint is effictive,set attribute */
            else
            {
                ep_info->epn_direction = epn_info.ep_direction; /* set transfer direction */
                ep_info->status = epn_info.trans_status;        /* set transfer state */
                ep_info->pkt_size = epn_info.ep_pkt_size;       /* set packet size */
                ep_info->ep_valid = TRUE;                       /* set valid flag of endpoint to invalid */
                ret = TRUE;
            }
        }
    }
    /* when this USB function macro is out support range,set number incorrect to return value */
    else
    {
        ep_info->ep_valid = FALSE;               /* set valid flag of endpoint to invalid */
        ret = FALSE;
    }

    return (ret);
}
