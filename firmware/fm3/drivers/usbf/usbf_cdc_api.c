/**
* @file          usbf_msc_api.c
* @brief         API module
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "usbf_cdc.h"
#include "usbf_msc_bot.h"
#include "usbf_msc_scsi.h"
#include "usbf_cdc_api.h"

#include "../../src/debug.h"
/*----------------------------------------------------------------------------*/
/* Local Constant Definition                                                  */
/*----------------------------------------------------------------------------*/
enum EM_API_ID                 /* caller ID definition                        */
{
    API_INIT,                  /* initialize API number                       */
    API_CLASS_REQUEST,         /* class request process API number            */
    API_POLLING,               /* polling procsee API number                  */
    API_TRANS_COMPLETE         /* data transfer end notification API number   */
};

/*----------------------------------------------------------------------------*/
/* Local Variable Definition                                                  */
/*----------------------------------------------------------------------------*/
static uint32_t s_max_block_size;                      /* block size maximum  */
static USBF_MSC_ST_CALLBACK_HANDLER s_callback_handler;/* callback fuction    */

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/

/* class action stage check function */
static uint8_t check_stage(void);

/* low layer USB function drive action state check function */
static uint8_t check_usb_status(uint8_t api_id);



/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

/**
* @brief            USBF_MSC_Init
*
* @param            pDataBuffer        : data transfer buffer storage
* @param            DataBufferSize       : data transfer buffer size
* @param            pCallbackHandler       : callback function address
* @return           process result
* @return           USBF_MSC_RET_OK            -> normally end
* @return           USBF_MSC_RET_INVALID_PARAM -> parameter invalid
* @return           USBF_MSC_RET_LUNINFO_NG    -> LUN iniformation incorrect
* @return          set the return value as follows
* @return          B'0000X0X0
* @return            |||||||+-- reserve (definite value 0)
* @return            ||||||+--- parameter invalid
* @return            |||||+---- reserve (definite value 0)
* @return            ||||+----- LUN iniformation incorrect
* @return            |||+------ reserve (definite value 0)
* @return            ||+------- reserve (definite value 0)
* @return            |+-------- reserve (definite value 0)
* @return            +--------- reserve (definite value 0)
*
**/

uint8_t USBF_CDC_Init(uint8_t * const pDataBuffer,
                      uint32_t DataBufferSize,
                      const USBF_MSC_ST_CALLBACK_HANDLER *pCallbackHandler)
{
    uint8_t func_ret;                  /* return value of call function */
    uint8_t ret = USBF_MSC_RET_OK;     /* return value               */

    /* callback function initialize */
    (void)memset(&s_callback_handler, 0x00 , sizeof(USBF_MSC_ST_CALLBACK_HANDLER));

    /* cheeck callbacke function address,and if address is unnormality,set parameter invalid bit of return value */
    if ((pCallbackHandler == NULL) ||                            /* callback function address is null */
        (pCallbackHandler->MediaGetLunInfoHandler == NULL) ||
        (pCallbackHandler->MediaReadHandler == NULL) ||
        (pCallbackHandler->MediaWriteHandler == NULL) ||
        (pCallbackHandler->MediaEventHandler == NULL))
    {
        ret |= USBF_MSC_RET_INVALID_PARAM;
    }

    /* when result is correct */
    if (ret == USBF_MSC_RET_OK)
    {
        /* register callback function */
        s_callback_handler.MediaGetLunInfoHandler = pCallbackHandler->MediaGetLunInfoHandler;
        s_callback_handler.MediaReadHandler = pCallbackHandler->MediaReadHandler;
        s_callback_handler.MediaWriteHandler = pCallbackHandler->MediaWriteHandler;
        s_callback_handler.MediaEventHandler = pCallbackHandler->MediaEventHandler;

        /* initialize SCSI command module and setreturn value */
        func_ret = msc_scsi_init(&s_max_block_size);
        if (func_ret == FALSE)
        {
            /* clear registered callback function */
            (void)memset(&s_callback_handler, 0x00 , sizeof(USBF_MSC_ST_CALLBACK_HANDLER));

            /* set the LUN information incorrect bit of return value */
            ret |= USBF_MSC_RET_LUNINFO_NG;
        }
        else if(DataBufferSize != s_max_block_size)  /* data transfer buffer size is not 1 block size */
        {
            /* clear registered callback function */
            (void)memset(&s_callback_handler, 0x00 , sizeof(USBF_MSC_ST_CALLBACK_HANDLER));

            /* set parameter invalid   bit of return value */
            ret |= USBF_MSC_RET_INVALID_PARAM;
        }
        else
        {
            /* BOT transfer control module initialize */
            msc_bot_init(pDataBuffer, DataBufferSize);
        }
    }

    return (ret);
}

typedef struct {
    unsigned int   dwDTERate;
    unsigned char  bCharFormat;
    unsigned char  bParityType;
    unsigned char  bDataBits;
} LINE_CODING;

static 	LINE_CODING CDC_LineCoding={9600,8,0,0};	// 8bit , 1stop-bit , parity-none
#define SET_LINE_CODING 0x20
#define GET_LINE_CODING 0x21

void USB_set_linecoding_callback( int bitrate );

/**
* @brief            CDC class request process
*
* @param            class_request_ptr    class request storage
* @return           MSC class request process result
* @return            USBF_MSC_RET_OK             normally end
* @return            USBF_MSC_RET_INVALID_PARAM  parameter invalid
*
**/
uint8_t cdc_class_request(const USBF_ST_SETUP_INFO *class_request_ptr)
{
	uint8_t  ret = USBF_MSC_RET_OK;       /* return value:initialize that normally end */
	uint8_t *buf = (uint8_t *)&CDC_LineCoding;
	int      size= class_request_ptr->wLength;

//	hex_print("class_request",class_request_ptr->bRequest);
//	hex_print("size",size);

	if(	size > sizeof(CDC_LineCoding)) {
		size = sizeof(CDC_LineCoding);
	}
	switch (class_request_ptr->bRequest ) {
	case SET_LINE_CODING:
		if ((class_request_ptr->wValue == 0) &&
		        (class_request_ptr->wIndex == 0)) {
			(void)USBF_Receive(USBF_CDC_CTRL_EP, buf,size );
			USB_set_linecoding_callback( CDC_LineCoding.dwDTERate );
			return ret;
		}
		break;
	case GET_LINE_CODING:
		if ((class_request_ptr->wValue == 0) &&
		        (class_request_ptr->wIndex == 0)) {
			(void)USBF_Send(USBF_CDC_CTRL_EP, buf,size );
			return ret;
		}
	default:
		break;
	}
	// error
	ret = USBF_MSC_RET_INVALID_PARAM;
	return(ret);
}

/**
* @brief            USBF_MSC_Request
*
* @param            pSetupPkt    setup packet data storage
* @return           process result
* @return           USBF_MSC_RET_OK           ->normally end
* @return           USBF_MSC_RET_STAGE_NG     ->class action stage abnormal
* @return           USBF_MSC_RET_INVALID_PARAM->parameter incorrect
* @return           USBF_MSC_RET_USBFUNC_NG   -> USB function driver action abnormal
* @return          set the return value as follows
* @return          B'0000X0X0
* @return            |||||||+-- reserve (definite value 0)
* @return            ||||||+--- parameter invalid
* @return            |||||+---- reserve (definite value 0)
* @return            ||||+----- LUN iniformation incorrect
* @return            |||+------ reserve (definite value 0)
* @return            ||+------- reserve (definite value 0)
* @return            |+-------- reserve (definite value 0)
* @return            +--------- reserve (definite value 0)
*
**/
uint8_t USBF_CDC_Request(const USBF_ST_SETUP_INFO * pSetupPkt)
{
    uint8_t ret = USBF_MSC_RET_OK;     /* return value               */

    /* when transfer data size is not 0,do following process */
    if (pSetupPkt->wLength != 0)
    {
        /* check usb function driver action state and if state is abnormal, */
        /* set the low lyaer USB function drive action unnorlmality bit of return value */
        if (check_usb_status(API_CLASS_REQUEST) != TRUE)
        {
            ret |= USBF_MSC_RET_USBFUNC_NG;
        }

        /* check class action stage and stage is abnormal,set the class action stage abnormal bit of rerurn value */
        if (check_stage() != TRUE)
        {
            ret |= USBF_MSC_RET_STAGE_NG;
        }
    }

    /* when result is correct,call class request process and set return value */
    if (ret == USBF_MSC_RET_OK)
    {
//      ret |= msc_bot_class_request(pSetupPkt);
        ret |= cdc_class_request(pSetupPkt);
    }

    return (ret);
}

void cdc_trans_complete(int epid,uint16_t count, uint8_t complete_status);

/**
* @brief            USBF_MSC_TransComplete
*
* @param            EpId                  : endpoint number
* @param            Count                 : transmitted data numbers
* @param            CompleteStatus        : transfer end state
* @return           process result
* @return          USBF_MSC_RET_OK           ->normally end
* @return          USBF_MSC_RET_STAGE_NG     ->class action stage abnormal
* @return          USBF_MSC_RET_INVALID_PARAM->parameter incorrect
* @return          set the return value as follows
* @return          B'0000X0X0
* @return            |||||||+-- reserve (definite value 0)
* @return            ||||||+--- parameter invalid
* @return            |||||+---- reserve (definite value 0)
* @return            ||||+----- LUN iniformation incorrect
* @return            |||+------ reserve (definite value 0)
* @return            ||+------- reserve (definite value 0)
* @return            |+-------- reserve (definite value 0)
* @return            +--------- reserve (definite value 0)
*
**/
uint8_t USBF_CDC_TransComplete(uint8_t EpId, uint16_t Count, uint8_t CompleteStatus)
{
    uint8_t ret = USBF_MSC_RET_OK;     /* return value               */

    if (EpId != USBF_CDC_CTRL_EP)
    {
        /* check class action stage and stage is abnormal,set the class action stage abnormal bit of rerurn value */
        if (check_stage() != TRUE)
        {
            ret |= USBF_MSC_RET_STAGE_NG;
        }

        /* check parameter and parameter is abnormal,set parameter invalid   bit of return value */
        if (((EpId != USBF_CDC_BULKIN_EP) && (EpId != USBF_CDC_BULKOUT_EP)) ||  /* check endpoint numbers */
            (Count > 64 /*s_max_block_size*/ ) ||                             /* check transmitted data numbers */
            (CompleteStatus > USBF_TRANS_NG))                      /* check transfer end state */
        {
            ret |= USBF_MSC_RET_INVALID_PARAM;

//			hex_print("epid",EpId);
//			hex_print("Count",Count);
//			hex_print("CompleteStatus",CompleteStatus);
        }

        /* when result is correct,call BOT transfer end process */
        if (ret == USBF_MSC_RET_OK)
        {
	      	cdc_trans_complete(EpId,Count, CompleteStatus);
        }
    }

    return (ret);
}

/**
* @brief            check_stage
*
* @param            none
* @return           stage state
*                   USBF_MSC_TRUE   -> stage normal
*                   USBF_MSC_FALSE  -> stage abnormal
*
**/
static uint8_t check_stage(void)
{
    uint8_t ret = TRUE;      /* return value             */
    return (ret);
}
/**
* @brief            check_usb_status
*
* @param            api_id : caller ID
* @return           USB function driver action state
*                   TRUE   -> status normal
*                   FALSE  -> status abnormal
*
**/
static uint8_t check_usb_status(uint8_t api_id)
{
    uint8_t status;                   /* USB function driver action state                 */
    uint8_t ep_status;                /* transfer state of endpoint of USB function driver */
    uint8_t ret = TRUE;               /* return value                                   */

    /* get USB function driver action state */
    status = USBF_GetStatus();

    /* judge caller ID  */
    if (api_id == API_CLASS_REQUEST)  /* class request process */
    {
        /* when it is not enumeration or configuration,set state abnormal to return value */
        if ((status == USBF_STATUS_ENUM) || (status == USBF_STATUS_CONFIG))
        {
            /* get transfer state of endpoint of USB function driver */
            (void)USBF_GetEpStatus(USBF_EP0, &ep_status);

            /* when endpoint0 transfer state is not in idle,set state abnormal to return value */
            if (ep_status != USBF_EPSTATUS_IDLE)
            {
                ret = FALSE;
            }
        }
        else
        {
            ret = FALSE;
        }
    }
    else if (api_id == API_POLLING) /* polling process */
    {
        /* when it is not configuration state,set stage abnormal to return value */
        if (status != USBF_STATUS_CONFIG)
        {
            ret = FALSE;
        }
    }
    else
    {

    }

    return (ret);
}

