/**
* @file          usbf_msc_bot.c
* @brief         BOT process module
* @author        zhangjie
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "usbf_msc.h"
#include "usbf_msc_bot.h"
#include "usbf_msc_scsi.h"

/*----------------------------------------------------------------------------*/
/* Local Constant Definition                                                  */
/*----------------------------------------------------------------------------*/
#define BOT_MAX_16BIT_VALUE ((uint16_t)0xFFFFU) /* uint16_t maximum */

/* command process state */
enum
{
    CMD_STATE_NOT_PROCESS,              /* unprocess state                    */
    CMD_STATE_SCSI_PROCESS,             /* SCSI subclass process state        */
    CMD_STATE_USB_PROCESS,              /* USB transfer process state         */
    CMD_STATE_USBWAIT_PROCESS,          /* USB transmitting process state     */
    CMD_STATE_SENDDUMMY_PROCESS,        /* dummy send process state           */
    CMD_STATE_RCVDUMMY_PROCESS          /* dummy receive process state        */
};

/* process state        */
enum
{
    STATE_NOT_PROCESS,                  /* unprocess state                    */
    STATE_WAIT_PROCESS,                 /* processing state                   */
    STATE_NEXT_PROCESS                  /* next process state                 */
};

/*----------------------------------------------------------------------------*/
/* Local Type Definition                                                      */
/*----------------------------------------------------------------------------*/

/* data structure for BOT manage */
typedef struct _ST_BOT_DATA
{
    uint8_t       stage;                /* BOT transfer stage                 */
    uint8_t       state;                /* process state                      */
    uint16_t      wait_reset_recovery;  /* wait reset recovery                */
    uint16_t      cbw_rcv_size;         /* CBW receive data size              */
    BOT_ST_CBW    cbw;                  /* CBW data                           */
    BOT_ST_CSW    csw;                  /* CSW data                           */
} ST_BOT_DATA;

/* data structure for BOT transfer */
typedef volatile struct _ST_BOT_TRANSFERRED
{
    volatile uint16_t  size;            /* transfered data numbers            */
    volatile uint8_t   semaphore;       /* transfer semaphore                 */
    volatile uint8_t   status;          /* transfer complete state            */
} ST_BOT_TRANSFERRED;

/* data structure for command manage */
typedef struct _ST_CMD
{
    uint8_t  state;                     /* command process state              */
    uint8_t  process_valid;             /* command process effictive/invalidity */
    uint32_t host_size;                 /* host expect transfer data numbers  */
    uint32_t device_size;               /* device will process data numbers   */
    uint32_t transferred_size;          /* transfered data numbers            */
    uint32_t processed_size;            /* processed data numbers             */
    uint32_t processing_size;           /* procssing data numbers             */
} ST_CMD;

/*----------------------------------------------------------------------------*/
/* Local Variable Definition                                                  */
/*----------------------------------------------------------------------------*/

static ST_CMD             s_cmd;               /* data for command manage     */
static ST_BOT_DATA        s_bot_data;          /* data for BOT manage         */
static ST_BOT_TRANSFERRED s_transferred;       /* data for BOT transfer       */
static uint32_t           s_trans_buffer_size; /* data transfer buffer size   */
static uint8_t*           s_trans_buffer_ptr;  /* data transfer buffer storage*/

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/

/* BOT transfer control module reset process */
static void bot_reset(void);

/* BOT transfer ready stage process */
static void ready_stage_process(void);

/* BOT transfer command transfer stage (CBW) process */
static void cbw_stage_process(void);

/* BOT transfer no data transfer stage process */
static void nodata_stage_process(void);

/* BOT transfer data-in stage process */
static void datain_stage_process(void);

/* BOT transfer data-out stage process */
static void dataout_stage_process(void);

/* status transfer stage (CSW) process */
static void csw_stage_process(void);

/* process for checking whether CBW data is effective */
static uint8_t check_CBW_valid(void);

/* process for checking CBW content whether is meaningful */
static uint8_t check_CBW_meaningful(void);

/* command precede process */
static void cmd_pre_process(void);

/* BOT transfer error stage process */
static void error_stage_process(void);

/* get send data size process */
static uint16_t get_send_data_size(void);

/* get receive data size process */
static uint16_t get_rcv_data_size(void);

/* process for waitting reset recovery */
static void wait_resetrecovery(void);
/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

/**
* @brief            BOT transfer control module initialize process
*
* @param            data_buffer_ptr    data transfer buffer storage
* @param            data_buffer_size   data transfer buffer size
* @return           none
*
**/
void msc_bot_init(uint8_t * const data_buffer_ptr, uint32_t data_buffer_size)
{
    /* data transfer buffer storage and size initialize */
    s_trans_buffer_ptr = data_buffer_ptr;

    s_trans_buffer_size = data_buffer_size;

    /* BOT transfer control module reset process */
    bot_reset();

    return;
}

/**
* @brief            get BOT transfer stage process
*
* @param            none
* @return           BOT transfer stage
* @return            BOT_STAGE_UNINIT    -> uninitialize stage
* @return            BOT_STAGE_READY     -> ready stage
* @return            BOT_STAGE_CBW       -> command tansfer stage (CBW)
* @return            BOT_STAGE_NODATA    -> no data transfer
* @return            BOT_STAGE_DATAIN    -> data-in transfer
* @return            BOT_STAGE_DATAOUT   -> data-out transfer
* @return            BOT_STAGE_ERROR     -> error process
* @return            BOT_STAGE_CSW       -> status transfer stage (CSW)
*
**/
uint8_t msc_bot_get_stage(void)
{
    /* return BOT transfer stage */
    return(s_bot_data.stage);
}

/**
* @brief            MSC class request process
*
* @param            class_request_ptr    class request storage
* @return           MSC class request process result
* @return            USBF_MSC_RET_OK             normally end
* @return            USBF_MSC_RET_INVALID_PARAM  parameter invalid
*
**/
uint8_t msc_bot_class_request(const USBF_ST_SETUP_INFO *class_request_ptr)
{
    uint8_t  class_request_valid = TRUE;      /* initialize that class request is effective */
    uint8_t  ret = USBF_MSC_RET_OK;           /* return value:initialize that normally end */
    uint8_t  max_lun = USBF_MSC_LUN_NUM - 1;  /* max LUN number */

    /* judge class request */
    /* Bulk_Only Mass Storage Reset class request */
    if (class_request_ptr->bRequest == BOT_BULK_ONLY_RESET)
    {
            /* check class request parameter */
            if ((class_request_ptr->bmRequestType.byte == BOT_BULK_ONLY_RESET_REQUEST_TYPE) &&
                (class_request_ptr->wValue == BOT_BULK_ONLY_RESET_REQUEST_VALUE) &&
                (HWORD_CPU_TO_LE(class_request_ptr->wIndex) == USBF_MSC_INTERFACE_ID) &&
                (class_request_ptr->wLength == BOT_BULK_ONLY_RESET_REQUEST_LENGTH))
            {
                /* stop bulk-in EP transfer */
                (void)USBF_AbortTrans(USBF_MSC_BULKIN_EP);
                /* clear bulk-in EP stall state */
                (void)USBF_SetStall(USBF_MSC_BULKIN_EP, USBF_CLEAR_STALL);

                /* stop bulk-out EP transfer */
                (void)USBF_AbortTrans(USBF_MSC_BULKOUT_EP);
                /* clear bulk-out EP stall state */
                (void)USBF_SetStall(USBF_MSC_BULKOUT_EP, USBF_CLEAR_STALL);

                /* BOT transfer control module reset process */
                bot_reset();

                /* send NULL packet */
                (void)USBF_Send(USBF_MSC_CTRL_EP, NULL, 0);
            }
            else
            {
                /* set invalidity to class request */
                class_request_valid = FALSE;
            }
    }
    /* Get Max LUN class request */
    else if (class_request_ptr->bRequest == BOT_GET_MAX_LUN_REQUEST)
    {
            /* check class request parameter */
            if ((class_request_ptr->bmRequestType.byte == BOT_GET_MAX_LUN_REQUEST_TYPE) &&
                (class_request_ptr->wValue == BOT_GET_MAX_LUN_REQUEST_VALUE) &&
                (HWORD_CPU_TO_LE(class_request_ptr->wIndex) == USBF_MSC_INTERFACE_ID) &&
                (HWORD_CPU_TO_LE(class_request_ptr->wLength) == BOT_GET_MAX_LUN_REQUEST_LENGTH))
            {
#if (USBF_MSC_SINGLE_LUN_STALL == USBF_MSC_DISABLE)
                (void)USBF_Send(USBF_MSC_CTRL_EP, &max_lun, BOT_GET_MAX_LUN_REQUEST_LENGTH);
#else
                /* set invalidity to class request */
                class_request_valid = FALSE;
#endif
            }
            else
            {
                /* set invalidity to class request */
                class_request_valid = FALSE;
            }
    }
    else
    {
            /* set invalidity to class request */
            class_request_valid = FALSE;
    }

    /* when class request is invalidity */
    if (class_request_valid == FALSE)
    {
        /* stall control endpoint */
        (void)USBF_SetStall(USBF_MSC_CTRL_EP, USBF_SET_STALL);
        /* set parameter invalid to return value */
        ret = USBF_MSC_RET_INVALID_PARAM;
    }

    return(ret);
}

/**
* @brief            BOT transfer main process
*
* @param            none
* @return           none
*
**/
void msc_bot_main(void)
{
    if (s_bot_data.wait_reset_recovery == FALSE)
    {
        /* BOT transfer ready stage process */
        if (s_bot_data.stage == BOT_STAGE_READY)
        {
            ready_stage_process();
        }
        /* BOT transfer command transfer stage (CBW) process */
        else if (s_bot_data.stage == BOT_STAGE_CBW)
        {
            cbw_stage_process();
        }
        /* BOT transfer no data transfer stage process */
        else if (s_bot_data.stage == BOT_STAGE_NODATA)
        {
                nodata_stage_process();
        }
        /* BOT transfer data-in stage process */
        else if (s_bot_data.stage == BOT_STAGE_DATAIN)
        {
                datain_stage_process();
        }
        /* BOT transfer data-out stage process */
        else if (s_bot_data.stage == BOT_STAGE_DATAOUT)
        {
                dataout_stage_process();
        }
        /* BOT transfer error stage process */
        else if (s_bot_data.stage == BOT_STAGE_ERROR)
        {
                error_stage_process();
        }
        /* status transfer stage (CSW) process */
        else if (s_bot_data.stage == BOT_STAGE_CSW)
        {
                csw_stage_process();
        }
        else
        {
        }
    }
    else
    {
        /* wait reset recovery */
    }

    return;
}

/**
* @brief            BOT data transfer complete process
*
* @param            count             transfered data numbers
* @param            complete_status   transfered end state
* @return           none
*
**/
void msc_bot_trans_complete(uint16_t count, uint8_t complete_status)
{
    /* when transfer complete,set transfer complete data */
    if ((complete_status != USBF_TRANS_CANCELED) &&
        (complete_status != USBF_TRANS_NG))
    {
        /* set transfer semaphore */
        s_transferred.semaphore++;

        /* set transfered end state */
        s_transferred.status = complete_status;

        /* set transfered data numbers */
        s_transferred.size = count;
    }

    return;
}

/**
* @brief            BOT transfer control module reset process
*
* @param            none
* @return           none
*
**/
static void bot_reset(void)
{
    /* initialize the data for BOT manage */
    (void)memset(&s_bot_data, 0x00, sizeof(s_bot_data));

    /* initialize the data for command manage */
    (void)memset(&s_cmd, 0x00, sizeof(s_cmd));

    /* initialize the data for BOT transfer */
    (void)memset((void*)&s_transferred, 0x00, sizeof(s_transferred));

    /* move to ready transfer stage */
    s_bot_data.stage = BOT_STAGE_READY;

    return;
}

/**
* @brief            BOT transfer control module reset process
*
* @param            none
* @return           none
*
**/
static void ready_stage_process(void)
{
    uint8_t    *cbw_ptr = (uint8_t *)(&s_bot_data.cbw);   /* CBW storage */
    uint8_t    ret;                                       /* return value */

    /* if it is unprocess state,do following process */
    if (s_bot_data.state == STATE_NOT_PROCESS)
    {
        /* CBW in the bulk-out endpoint receive request */
        ret = USBF_Receive(USBF_MSC_BULKOUT_EP, s_trans_buffer_ptr, USBF_MSC_BULKOUT_EP_PKT_SIZE);
        if (ret == USBF_RET_OK)
        {
            /* when CBW receive request success,move to processing state */
            s_bot_data.state = STATE_WAIT_PROCESS;
        }
    }

    /* if it is processing state,do following process */
    if (s_bot_data.state == STATE_WAIT_PROCESS)
    {
        if (s_transferred.semaphore != 0)
        {
            /* if transfered end stateis [transfer buffer full]or [transfer normally end],get CBW data */
            if ((s_transferred.status == USBF_BUFFER_FULL) ||
                (s_transferred.status == USBF_TRANS_COMPLETE))
            {
                /* the sum of CBW receive size and transferred size is not larger then CBW size */
                /* get get the data of transferred size */
                if ((s_bot_data.cbw_rcv_size + s_transferred.size) <= BOT_CBW_SIZE)
                {
                    (void)memcpy(&(cbw_ptr[s_bot_data.cbw_rcv_size]),
                                 s_trans_buffer_ptr, s_transferred.size);
                }
                /* the sum of CBW receive size and transferred size is larger then CBW size */
                /* get the different data between CBW size and transferred size */
                else
                {
                    (void)memcpy(&(cbw_ptr[s_bot_data.cbw_rcv_size]),
                                 s_trans_buffer_ptr, (BOT_CBW_SIZE - s_bot_data.cbw_rcv_size));
                }

                /* renew CBW received size */
                s_bot_data.cbw_rcv_size += s_transferred.size;
                /* move to next process state */
                s_bot_data.state = STATE_NEXT_PROCESS;
            }
            else
            {
                /* process for waitting reset recovery */
                wait_resetrecovery();
            }

            /* clear transfer semaphore */
            s_transferred.semaphore = 0;
        }
    }

    /*  if it is next process state,do the following process */
    if (s_bot_data.state == STATE_NEXT_PROCESS)
    {
        /* CBW receive size is larger then CBW size or
           transfered end state is[transfer normally end],move to command transfer stage (CBW) */
        if ((s_bot_data.cbw_rcv_size >= BOT_CBW_SIZE) ||
            (s_transferred.status == USBF_TRANS_COMPLETE))
        {
            s_bot_data.stage = BOT_STAGE_CBW;
        }
        /* in one packet size,CBW can not receive, do receive request again */
        else
        {
            /* move to unprocess state */
            s_bot_data.state = STATE_NOT_PROCESS;
        }
    }

    return;
}

/**
* @brief            BOT transfer command transfer stage (CBW) process
*
* @param            none
* @return           none
*
**/
static void cbw_stage_process(void)
{
    /* move to unprocess state */
    s_bot_data.state = STATE_NOT_PROCESS;

    /* process for checking whether CBW data is effective */
    if (check_CBW_valid() == TRUE)
    {
        /* process for checking CBW content whether is meaningful */
        if (check_CBW_meaningful() == TRUE)
        {
            /* set command unprocess stage to command stage */
            s_cmd.state = CMD_STATE_NOT_PROCESS;

            /* command precede process */
            cmd_pre_process();
        }
        else
        {
            /* move to error stage */
            s_bot_data.stage = BOT_STAGE_ERROR;
        }
    }
    /* when CBW is invalidity */
    else
    {
        /* clear reset recovery waitting */
        if (s_bot_data.wait_reset_recovery == FALSE)
        {
            /* process for waitting reset recovery */
            wait_resetrecovery();
        }
     }
    return;
}

/**
* @brief            BOT transfer command transfer stage (CBW) process
*
* @param            none
* @return           none
*
**/
static void nodata_stage_process(void)
{
    uint8_t    cmd_ret;       /* return value of SCSI command process function */

    /* when command process is effective,do the following process */
    if (s_cmd.process_valid == TRUE)
    {
        /* call SCSI command process */
        cmd_ret = msc_scsi_cmd_process(((SCSI_ST_CDB*)(&s_bot_data.cbw.bCBWCBLength)),
                                        NULL, 0);
        /* move to status transfer stage(CSW) */
        s_bot_data.stage = BOT_STAGE_CSW;
        /* when SCSI command process is fail,set fail to command CSW stage */
        if (cmd_ret == FALSE)
        {
            s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_FAIL;
        }
    }
    /* when command process is invailidity,move to status transfer stage(CSW) */
    else
    {
        s_bot_data.stage = BOT_STAGE_CSW;
    }

    return;
}

/**
* @brief            BOT transfer data-in stage process
*
* @param            none
* @return           none
*
**/
static void datain_stage_process(void)
{
    uint8_t  ret;              /* return value of call function */
    uint8_t  ep_status;        /* status of endpoint */
    uint32_t send_size = 0;    /* sending size */

    /* if it is unprocess stage,do the following process */
    if (s_cmd.state == CMD_STATE_NOT_PROCESS)
    {
        /* when SCSI command is effective,move to SCSI subclass process state */
        if (s_cmd.process_valid == TRUE)
        {
            s_cmd.state = CMD_STATE_SCSI_PROCESS;
        }
        /* when SCSI command is invalidity,move to dummy send sate */
        else
        {
            s_cmd.state = CMD_STATE_SENDDUMMY_PROCESS;
        }
    }

    /* when it is SCSI subclass process state ,get data from SCSI sunclass */
    if (s_cmd.state == CMD_STATE_SCSI_PROCESS)
    {
        /* set send data size process */
        send_size = get_send_data_size();

        /* clear buffer by 0x00,and prepared to send data */
        (void)memset(s_trans_buffer_ptr, 0x00, (uint16_t)send_size);

        /* set procesing data numburns */
        s_cmd.processing_size = (send_size <= (s_cmd.device_size - s_cmd.processed_size)) ?
                                 send_size :  (s_cmd.device_size - s_cmd.processed_size);

        /* call SCSI command process */
        ret = msc_scsi_cmd_process(((SCSI_ST_CDB*)(&s_bot_data.cbw.bCBWCBLength)),
                                   s_trans_buffer_ptr, s_cmd.processing_size);

        /* move to USB transfer state */
        s_cmd.state = CMD_STATE_USB_PROCESS;
        /* when SCSI command failled,set invalidity to command process */
        if (ret == FALSE)
        {
            /* set CSW sate,if pfase error is setted,do not set CSW state */
            if (s_bot_data.csw.bCSWStatus < BOT_CSW_STATUS_PHASE_ERROR)
            {
                s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_FAIL;
            }
            s_cmd.process_valid = FALSE;

        }
    }

    /* when dummy data send sate,fill dummy data in buffer,and prepared to send data */
    if (s_cmd.state == CMD_STATE_SENDDUMMY_PROCESS)
    {
        /* set send data size process */
        send_size = get_send_data_size();

        /* clear buffer by 0x00,and prepared to send data */
        (void)memset(s_trans_buffer_ptr, 0x00, (uint16_t)send_size);

        /* renew processing data numbers */
        s_cmd.processing_size = 0;

        /* move to USB transfer state */
        s_cmd.state = CMD_STATE_USB_PROCESS;
    }

    /* when it is USB transfer process state,use low layer drive to do data-in transfer process */
    if (s_cmd.state == CMD_STATE_USB_PROCESS)
    {
        /* get endpoint status */
        ret = USBF_GetEpStatus(USBF_MSC_BULKIN_EP, &ep_status);
        if ((ep_status != USBF_EPSTATUS_TRANS)&&(ret==USBF_RET_OK))
        {
            /* demand command data send request by bulk-in endpoint */
            ret = USBF_Send(USBF_MSC_BULKIN_EP, s_trans_buffer_ptr, (uint16_t)send_size);
            if (ret == USBF_RET_OK)
            {
                /* move to USB transferring process state */
                s_cmd.state = CMD_STATE_USBWAIT_PROCESS;
            }
        }
    }

    /* when it is USB transferring process state,do the following process */
    if (s_cmd.state == CMD_STATE_USBWAIT_PROCESS)
    {
        /* when data-in han been transferred */
        if (s_transferred.semaphore >= 1)
        {
            /* transfer normally end */
            if (s_transferred.status == USBF_TRANS_COMPLETE)
            {
                /* renew transfered data numbers */
                s_cmd.transferred_size += s_transferred.size;

                /* renew processed data numbers */
                s_cmd.processed_size += s_cmd.processing_size;

                /* move to status transfer  stage(CSW) */
                s_bot_data.stage = BOT_STAGE_CSW;
            }

            /* when transfer buffer is empty */
            else if (s_transferred.status == USBF_BUFFER_EMPTY)
            {
                /* renew transferred data numbers */
                s_cmd.transferred_size += s_transferred.size;

                /* when transfer data numbers is normality,renew processed data numbers and move to status transfer stage(CSW) */
                if (s_cmd.transferred_size == s_cmd.host_size)
                {
                    s_bot_data.stage = BOT_STAGE_CSW;
                    s_cmd.processed_size += s_cmd.processing_size;
                }

                /* the residue size is more then 0,renew processed data numbers and move to unprocess state */
                if (s_cmd.transferred_size < s_cmd.host_size)
                {
                    s_cmd.state = CMD_STATE_NOT_PROCESS;
                    s_cmd.processed_size += s_cmd.processing_size;
                }
            }
            /* excluding above */
            else
            {
                /* set phase error to CSW state and move to status transfer stage(CSW) */
                s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_PHASE_ERROR;
                s_bot_data.stage = BOT_STAGE_CSW;
            }

            /* clear transfer semaphore */
            s_transferred.semaphore = 0;
        }
    }

    return;
}

/**
* @brief            dataout_stage_process
*
* @param            none
* @return           none
*
**/
static void dataout_stage_process(void)
{
    uint8_t  ret;                  /* return value of call function */
    uint32_t rcv_size;             /* receiving size */

    /* when it is unprocess state,move to USB transfer state */
    if (s_cmd.state == CMD_STATE_NOT_PROCESS)
    {
        s_cmd.state = CMD_STATE_USB_PROCESS;
    }

    /* when USB transfer process state,do data-out transfer process by low layer USB function drive */
    if (s_cmd.state == CMD_STATE_USB_PROCESS)
    {
        /* get receive data size process */
        rcv_size = get_rcv_data_size();

        /* demand command data receive request by bulk-out endpoint */
        ret = USBF_Receive(USBF_MSC_BULKOUT_EP, s_trans_buffer_ptr, (uint16_t)rcv_size);
        if (ret == USBF_RET_OK)
        {
            /* move to USB transferring process state */
            s_cmd.state = CMD_STATE_USBWAIT_PROCESS;
        }
    }

    /* when it is USB processing sate,do the following process */
    if (s_cmd.state == CMD_STATE_USBWAIT_PROCESS)
    {
        /* when data-in has been transferred */
        if (s_transferred.semaphore >= 1)
        {
            /* transfer normally end or transfer buffer is full */
            /* renew transferred data numbers and move to SCSI subclass prosess sate */
            if ((s_transferred.status == USBF_TRANS_COMPLETE) ||
                (s_transferred.status == USBF_BUFFER_FULL))
            {
                s_cmd.transferred_size += s_transferred.size;

                /* when SCSI command is effective, move to SCSI subclass process state */
                if (s_cmd.process_valid == TRUE)
                {
                    s_cmd.state = CMD_STATE_SCSI_PROCESS;
                }
                /* when SCSI command is invalidity,move to dummy data receive state */
                else
                {
                    s_cmd.state = CMD_STATE_RCVDUMMY_PROCESS;
                }
            }
            /* excluding above */
            else
            {
                /* move to error stage */
                s_bot_data.stage = BOT_STAGE_ERROR;
            }

            /* clear transfer semaphore */
            s_transferred.semaphore = 0;
        }
    }

    /* when dummy data receive state,do the following process */
    if (s_cmd.state == CMD_STATE_RCVDUMMY_PROCESS)
    {
        /* when transfer data numbers is normality,move to SCSI subclear process state */
        if (s_cmd.transferred_size == s_cmd.host_size)
        {
            /* move to status transfer  stage(CSW) */
            s_bot_data.stage = BOT_STAGE_CSW;
        }
        /* when transfer data numbers is not enough,do the following process */
        else if (s_cmd.transferred_size < s_cmd.host_size)
        {
            /* when transfer buffer is full,move to unprocess state */
            if (s_transferred.status == USBF_BUFFER_FULL)
            {
                s_cmd.state = CMD_STATE_NOT_PROCESS;
            }
            /* when transfer noumally end,do the following process */
            if (s_transferred.status == USBF_TRANS_COMPLETE)
            {
                /* move to error stage */
                s_bot_data.stage = BOT_STAGE_ERROR;
            }
        }
        /* when transfer data numbers is unnormality,do the following process */
        else
        {
            /* move to error stage */
            s_bot_data.stage = BOT_STAGE_ERROR;
        }
    }

    /* when it is SCSI subclass process state,get data from SCSI subclass */
    if (s_cmd.state == CMD_STATE_SCSI_PROCESS)
    {
        /* when transfer data numbers is normality,move to SCSI subclass process state */
        if (s_cmd.transferred_size == s_cmd.host_size)
        {
            s_cmd.processing_size = (s_transferred.size <= (s_cmd.device_size - s_cmd.processed_size)) ?
                                     s_transferred.size  : (s_cmd.device_size - s_cmd.processed_size);

            /* call SCSI command process */
            ret = msc_scsi_cmd_process(((SCSI_ST_CDB*)(&s_bot_data.cbw.bCBWCBLength)),
                                       s_trans_buffer_ptr, s_cmd.processing_size);

            /* move to status transfer  stage(CSW) */
            s_bot_data.stage = BOT_STAGE_CSW;

            /* when SCSI command process failed */
            if (ret == FALSE)
            {
                /* set CSW sate,if pfase error is setted,do not set CSW state */
                if (s_bot_data.csw.bCSWStatus < BOT_CSW_STATUS_PHASE_ERROR)
                {
                    s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_FAIL;
                }
            }

            /* when SCSI command process sucessed,renew processed data numbers and move to status transfer stage(CSW) */
            else
            {
                s_cmd.processed_size += s_cmd.processing_size;
            }
        }
        /* when transfer data numbers is not enough,do the following process */
        else if (s_cmd.transferred_size < s_cmd.host_size)
        {
            /* transfer buffer is full,do the following process */
            if (s_transferred.status == USBF_BUFFER_FULL)
            {
                s_cmd.processing_size = (s_transferred.size <= (s_cmd.device_size - s_cmd.processed_size)) ?
                                         s_transferred.size  : (s_cmd.device_size - s_cmd.processed_size);

                /* call SCSI command process */
                ret = msc_scsi_cmd_process(((SCSI_ST_CDB*)(&s_bot_data.cbw.bCBWCBLength)),
                                           s_trans_buffer_ptr, s_cmd.processing_size);

                /* move to unprocess state */
                s_cmd.state = CMD_STATE_NOT_PROCESS;
                /* when SCSI command process failled */
                if (ret == FALSE)
                {
                    /* set CSW sate,if pfase error is setted,do not set CSW state */
                    if (s_bot_data.csw.bCSWStatus < BOT_CSW_STATUS_PHASE_ERROR)
                    {
                        s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_FAIL;
                    }
                    /* when command process is invalidity */
                    s_cmd.process_valid = FALSE;
                }

                /* when SCSI command process sucessed,renew processed data numbers */
                else if (ret == TRUE)
                {
                    s_cmd.processed_size += s_cmd.processing_size;
                }
                else
                {
                }

            }
            /* when transfer normally end,do the following process */
            if (s_transferred.status == USBF_TRANS_COMPLETE)
            {
                /* move to error stage */
                s_bot_data.stage = BOT_STAGE_ERROR;
            }
        }
        /* when transfer data numbers is innormality,do the following process */
        else
        {
            /* move to error stage */
            s_bot_data.stage = BOT_STAGE_ERROR;
        }
    }

    return;
}

/**
* @brief            BOT transfer error stage process
*
* @param            none
* @return           none
*
**/
static void error_stage_process(void)
{
    /* stall bulk-out endpoint */
    (void)USBF_SetStall(USBF_MSC_BULKOUT_EP, USBF_SET_STALL);

    /* set pfase error to CSW state */
    s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_PHASE_ERROR;

    /* move to BOT transfer status transfer(CSW)stage */
    s_bot_data.stage = BOT_STAGE_CSW;
}

/**
* @brief            status transfer stage (CSW) process
*
* @param            none
* @return           none
*
**/
static void csw_stage_process(void)
{
    uint8_t  ret;                         /* return value */

    /* when it is unprocess state,do the following process */
    if (s_bot_data.state == STATE_NOT_PROCESS)
    {
        /* prepared for CSW data */
        /* set CSW signature */
        s_bot_data.csw.dCSWSignature = WORD_CPU_TO_LE(BOT_CSW_SIGNATURE);

        /* set CSW tag */
        s_bot_data.csw.dCSWTag = s_bot_data.cbw.dCBWTag;

        /* set difference between host except transfer data numbres and device processed numbers to dCSWDataResidue field */
        s_bot_data.csw.dCSWDataResidue = WORD_CPU_TO_LE(s_cmd.host_size - s_cmd.processed_size);

        /* demand CSW send request by bulk-in endpoint */
        ret = USBF_Send(USBF_MSC_BULKIN_EP, (uint8_t *)&s_bot_data.csw, BOT_CSW_SIZE);
        if (ret == USBF_RET_OK)
        {
            /* when send request is sucessed,move to processing state */
            s_bot_data.state = STATE_WAIT_PROCESS;
        }
    }

    /* when it is processing state,do the following process */
    if (s_bot_data.state == STATE_WAIT_PROCESS)
    {
        if (s_transferred.semaphore != 0)
        {
            /* when transfer norally end or transfer is empty,and transfer size is CSW size */
            if (((s_transferred.status == USBF_TRANS_COMPLETE) ||
                 (s_transferred.status == USBF_BUFFER_EMPTY)) &&
                 (s_transferred.size == BOT_CSW_SIZE))
            {
                /* BOT transfer control module reset process */
                bot_reset();
            }
            else
            {
                /* clear reset recovery waitting */
                if (s_bot_data.wait_reset_recovery == FALSE)
                {
                    /* process for waitting reset recovery*/
                    wait_resetrecovery();
                }
            }
        }
    }

    return;
}

/**
* @brief            process for checking whether CBW data is effective
*
* @param            none
* @return           check result
*                    FALSE  CBW data is invalidity
*                    TRUE   CBW data is effective
*
**/
static uint8_t check_CBW_valid(void)
{
    uint8_t ret = FALSE;   /* set that CBW data is invalidity to return value */

    /* whether CBW data is effective,if the following factors are true,we think that CBW data is effective */
    /* 1.CBW receiving size is 31 bytes */
    /* 2.the value of dCBWSignature is 43425355H */
    if ((s_bot_data.cbw_rcv_size == BOT_CBW_SIZE) &&
        (s_bot_data.cbw.dCBWSignature == WORD_CPU_TO_LE(BOT_CBW_SIGNATURE)))
    {
        ret = TRUE;
    }

    return(ret);
}

/**
* @brief            process for checking CBW content whether is meaningful
*
* @param            none
* @return           check result
*                   FALSE     CBW content is not meaningful
*                   TRUE      CBW content is meaningful
*
**/
static uint8_t check_CBW_meaningful(void)
{
    uint8_t  ret = FALSE;   /* set that CBW content is not meaningful to return value */

    /* judge CBW content whether is meaningful,if the following factors are true, we think that CBW content is not meaningful  */
    /* 1.reserve bit is not setted */
    /* 2.bCBWLUN  contains the value of LUN */
    /* 3.bCBWCBLength content is correct to subclass */
    if ((s_bot_data.cbw.bCBWLUN <= (USBF_MSC_LUN_NUM - 1)) &&
        (s_bot_data.cbw.bCBWCBLength <= BOT_CBWCBLENGTH_MAX) &&
        (s_bot_data.cbw.bCBWCBLength >= BOT_CBWCBLENGTH_MIN) &&
        ((s_bot_data.cbw.bmCBWFlags == BOT_CBWFLAGS_HOST_TO_DEVICE) ||
         (s_bot_data.cbw.bmCBWFlags == BOT_CBWFLAGS_DEVICE_TO_HOST)))
    {
        ret = TRUE;
    }

    return(ret);
}

/**
* @brief            command precede process
*
* @param            none
* @return           command block content is(CBWCB)whether effective to subclass
*                   FALSE     command block is invalidity
*                   TRUE      command block is effective
*
**/
static void cmd_pre_process(void)
{
    uint32_t device_length;          /* treansfer data numbers */
    uint8_t  device_trans_type;      /* device transfer type */
    uint8_t  cmd_valid;              /* command effective/invalidity */
    uint8_t  host_trans_type;        /* host transfer type */

    /* get host expect */
    /* save transfer data numbers */
    s_cmd.host_size = WORD_CPU_TO_LE(s_bot_data.cbw.dCBWDataTransferLength);

    /* when transfer data numbers is 0,set transfer type [no transfer] to transfer type */
    if (s_cmd.host_size == 0)
    {
        host_trans_type = TRANSFER_TYPE_NO_TRANSFER;
    }
    /* when transfer direction is [out],set [BOT_CBWFLAGS_HOST_TO_DEVICE] to transfer type */
    else if (s_bot_data.cbw.bmCBWFlags == BOT_CBWFLAGS_HOST_TO_DEVICE)
    {
        host_trans_type = TRANSFER_TYPE_HOST_TO_DEVICE;
    }
    /* when transfer direction is [in],set [TRANSFER_TYPE_DEVICE_TO_HOST] to transfer type */
    else
    {
        host_trans_type = TRANSFER_TYPE_DEVICE_TO_HOST;
    }

    /* get the intend of device */
    cmd_valid = msc_scsi_cmd_check(s_bot_data.cbw.bCBWLUN,
                                   ((SCSI_ST_CDB*)(&s_bot_data.cbw.bCBWCBLength)),
                                   &device_trans_type,
                                   &device_length);

    /* when command block is valid */
    if (cmd_valid == TRUE)
    {
        /* set reserve process data numbres of host */
        s_cmd.device_size = device_length;

        /* set intend transfer type of device */
        /* s_cmd.device_type = device_trans_type; */

        /* when host expect no transfer */
        if (host_trans_type == TRANSFER_TYPE_NO_TRANSFER)
        {
            /* move to no data transfer stage */
            s_bot_data.stage = BOT_STAGE_NODATA;

            /* when device intend no data transfer */
            if (device_trans_type == TRANSFER_TYPE_NO_TRANSFER)
            {
                /* case1  (Hn = Dn) */
                s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_GOOD;
            }
            /* when device intend to send data to host */
            else if (device_trans_type == TRANSFER_TYPE_DEVICE_TO_HOST)
            {
                /* case2  (Hn < Di) */
                s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_PHASE_ERROR;
            }
            /* when device intend to receive data from host */
            else
            {
                /* case3  (Hn < Do) */
                s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_PHASE_ERROR;
            }
        }
        /* when host expect receive data from device */
        else if (host_trans_type == TRANSFER_TYPE_DEVICE_TO_HOST)
        {
            /* move to data-in transfer stage */
            s_bot_data.stage = BOT_STAGE_DATAIN;

            /* when device expect no transfer */
            if (device_trans_type == TRANSFER_TYPE_NO_TRANSFER)
            {
                /* case4  (Hi > Dn) */
                s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_GOOD;
            }
            /* when device send data to host */
            else if (device_trans_type == TRANSFER_TYPE_DEVICE_TO_HOST)
            {
                /* when host expectted receive data size is more then device intend to send data size */
                if (s_cmd.host_size > s_cmd.device_size)
                {
                    /* case5  (Hi > Di) */
                    s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_GOOD;
                }
                /*when host expectted receive data size is the same to device intend send data size */
                else if (s_cmd.host_size == s_cmd.device_size)
                {
                    /* case6  (Hi = Di) */
                    s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_GOOD;
                }
                /* when host expectted receive data lenth is less then device intend to send data size */
                else
                {
                    /* case7  (Hi < Di) */
                    s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_PHASE_ERROR;
                }
            }
            /* when device intend to receive data from host */
            else
            {
                /* case8  (Hi <> Do) */
                s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_PHASE_ERROR;
            }
        }
        /* when host expect to send data to device */
        else
        {
            /* move to data-out transfer stage */
            s_bot_data.stage = BOT_STAGE_DATAOUT;

            /* device intend to transfer no data */
            if (device_trans_type == TRANSFER_TYPE_NO_TRANSFER)
            {
                /* case9  (Ho > Dn) */
                s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_GOOD;
            }
            /* device intend to send data to host */
            else if (device_trans_type == TRANSFER_TYPE_DEVICE_TO_HOST)
            {
                /* case10 (Ho <> Di) */
                s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_PHASE_ERROR;
            }
            /* device intend to receive data from host */
            else
            {
                /* when host expectted data length is more then device intend to receive data length */
                if (s_cmd.host_size > s_cmd.device_size)
                {
                    /* case11 (Ho > Do) */
                    s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_GOOD;
                }
                /* when host expectted data length is the same to device intend to receive data length */
                else if (s_cmd.host_size == s_cmd.device_size)
                {
                    /* case12 (Ho = Do) */
                    s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_GOOD;
                }
                /* when host expectted data length is less then device intend to receive data length */
                else
                {
                    /* case13 (Ho < Do) */
                    s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_PHASE_ERROR;
                }
            }
        }

        /* judge whether host expect transfer type and device intended is identical */
        if (host_trans_type == device_trans_type)
        {
            /* set valid to command process */
            s_cmd.process_valid = TRUE;
        }
        else
        {
            /* set invalid to command process */
            s_cmd.process_valid = FALSE;
        }
    }
    /* when command is invalid,do the following process */
    else
    {
        /* when host expect transfer no data */
        if (host_trans_type == TRANSFER_TYPE_NO_TRANSFER)
        {
            /* move to transfer no data stage */
            s_bot_data.stage = BOT_STAGE_NODATA;
        }
        /* when host expect to receive data from device */
        else if (host_trans_type == TRANSFER_TYPE_DEVICE_TO_HOST)
        {
            /* move to data-in transfer stage */
            s_bot_data.stage = BOT_STAGE_DATAIN;
        }
        /* when host expect to send data to device */
        else
        {
            /* move to data-out transfer stage */
            s_bot_data.stage = BOT_STAGE_DATAOUT;
        }

        /* set command process is invalid */
        s_cmd.process_valid = FALSE;

        /* set command fail to CSW state */
        s_bot_data.csw.bCSWStatus = BOT_CSW_STATUS_FAIL;
    }

    return;
}

/**
* @brief            set send data size process
*
* @param            none
* @return           send data size
*
**/
static uint16_t get_send_data_size(void)
{
    uint16_t   send_size;          /* send data size */
    uint32_t   residues_size;      /* residues data size */
    uint32_t   temp_size;          /* size */

    /* when send data is lefted,do the following process */
    /* set the smallest one among residues data size,transfer buffer size and  */
    /* the max size of one time send data to send data size */
    if (s_cmd.host_size > s_cmd.transferred_size)
    {
        residues_size = s_cmd.host_size - s_cmd.transferred_size;

        temp_size = ((residues_size < s_trans_buffer_size) ? residues_size : s_trans_buffer_size);

        send_size = (uint16_t)(((uint32_t)BOT_MAX_16BIT_VALUE < temp_size) ? (uint16_t)BOT_MAX_16BIT_VALUE : (uint16_t)temp_size);
    }
    /* when send end,set 0 to send data size */
    else
    {
        send_size = 0;
    }

    return (send_size);
}

/**
* @brief            get receive data size process
*
* @param            none
* @return           receive data size
*
**/
static uint16_t get_rcv_data_size(void)
{
    uint16_t   rcv_size;                  /* receive data size */
    uint32_t   residues_size;             /* residues data size */
    uint32_t   temp_size;                 /* size */
    uint16_t   valid_max_size;            /* max size of effective receive data */
    uint16_t   valid_rcv_max_size;        /* max size of effective one time receive data */
    uint32_t   valid_buffer_size;         /* max size of effective buffer */

    /* when receive data is lefted,do the following process */
    /* set the smallest one among residues data size,transfer buffer size and */
    /* the max size of one time send data to receive data size */
    if (s_cmd.host_size > s_cmd.transferred_size)
    {
        /* acrossing to low layer USB function drive,receive data size has to be setted by multipe of endpoint packet size */
        /* calculate max size of effective one time receive data */
        valid_rcv_max_size = (BOT_MAX_16BIT_VALUE / USBF_MSC_BULKOUT_EP_PKT_SIZE) * USBF_MSC_BULKOUT_EP_PKT_SIZE;

        /* calculate max size of effective buffer */
        valid_buffer_size = (s_trans_buffer_size / (uint32_t)USBF_MSC_BULKOUT_EP_PKT_SIZE) * (uint32_t)USBF_MSC_BULKOUT_EP_PKT_SIZE;

        /* calculate max size of effective receive data */
        valid_max_size = (uint16_t)(((uint32_t)valid_rcv_max_size < valid_buffer_size) ? (uint16_t)valid_rcv_max_size : (uint16_t)valid_buffer_size);

        /* calculate residues data size */
        residues_size = s_cmd.host_size - s_cmd.transferred_size;

        /* when residues data size is not mulpite of endpoint packet size  */
        /* ajest residues data size to mulpite of endpoint packet size */
        if ((residues_size % USBF_MSC_BULKOUT_EP_PKT_SIZE) == 0)
        {
            temp_size = residues_size;
        }
        else
        {
            temp_size = ((residues_size / USBF_MSC_BULKOUT_EP_PKT_SIZE) + 1) * USBF_MSC_BULKOUT_EP_PKT_SIZE;
        }

        /* calculate receive data size */
        rcv_size = (uint16_t)(((uint32_t)valid_max_size < temp_size) ? (uint16_t)valid_max_size : (uint16_t)temp_size);
    }
    /* when receive end,set 0 to receive data size */
    else
    {
        rcv_size = 0;
    }

    return(rcv_size);
}

/**
* @brief            process for waitting reset recovery
*
* @param            none
* @return           none
*
**/
static void wait_resetrecovery(void)
{
    /* stall bulk-in and bulk-out endpoint */
    (void)USBF_SetStall(USBF_MSC_BULKIN_EP,  USBF_SET_STALL);
    (void)USBF_SetStall(USBF_MSC_BULKOUT_EP, USBF_SET_STALL);

    /* set reset recovery wait */
    s_bot_data.wait_reset_recovery = TRUE;
}
