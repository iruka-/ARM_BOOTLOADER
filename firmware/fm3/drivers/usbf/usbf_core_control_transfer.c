/**
* @file          usbf_core_control_transfer.h
* @brief         control transfer control module
* @author        lijiale
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "usbf.h"
#include "usbf_core_control_transfer.h"
#include "usbf_core_stdrequest.h"
#include "usbf_hal_io.h"
#include "usbf_core_event.h"

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

/* setup packet size   */
#define CTRLTRANS_SETUP_SIZE             (8)  /* setup packet size            */

/* transfer direction */
#define CTRLTRANS_DIRECTION_IN           (1)  /* direction: devicer->host     */

#define CTRLTRANS_DIRECTION_OUT          (0)  /* direction: host->devicer     */

/* request type */
#define CTRLTRANS_REQUEST_TYPE_STANTARD  (0)  /* standard request             */
#define CTRLTRANS_REQUEST_TYPE_CLASS     (1)  /* class request                */
#define CTRLTRANS_REQUEST_TYPE_VENDOR    (2)  /* vendor request               */

/*----------------------------------------------------------------------------*/
/* Static Variable Definition                                                  */
/*----------------------------------------------------------------------------*/

/* setup packet information */
static USBF_ST_SETUP_INFO        s_setup_info;

/* endpoint0 attribute  */
static USBFUNC_ST_CTRLTRANS_EPINFO  s_ep0_info;

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/

/* endpoint0 initialize process */
static void ctrltrans_ep0_init(void);

/* control transfer sending process */
static void ctrltrans_send_process(void);

/* control transfer receiving process */
static void ctrltrans_receive_process (void);

/* setup receiving process */
static void ctrltrans_setup_process(void);

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

/**
* @brief            register endpoint0 attribute
*
* @param            ep0_pkt_size    endpoint0 packet size
* @return           none
*
**/
void usbfunc_ctrltrans_entry_ep0info(uint8_t ep0_pkt_size)
{
    /* endpoint0 attribute initialize */
    (void)memset(&s_ep0_info, 0, sizeof(USBFUNC_ST_CTRLTRANS_EPINFO));

    /* set endpoint0 attribute packet size */
    s_ep0_info.pkt_size = ep0_pkt_size;

    return;
}

/**
* @brief            control transfer module initialize
*
* @param            none
* @return           none
*
**/
void usbfunc_ctrltrans_init(void)
{
    /* setup packet information initialize */
    (void)memset(&s_setup_info, 0, sizeof(USBF_ST_SETUP_INFO));

    /* endpoint0 initialize process */
    ctrltrans_ep0_init();

    /* set endpoint0 packet size */
    usbfunc_reg_set_ep0pktsize(s_ep0_info.pkt_size);

    return;
}

/**
* @brief            control transfer sending request
*
* @param            buffer    send data buffer
* @param            size      send data size
* @return           none
*
**/
void usbfunc_ctrltrans_send(uint8_t * buffer, uint16_t size)
{
    /* set transmitting to transfer state */
    s_ep0_info.trans_status = USBF_EPSTATUS_TRANS;

    /* set transfer data storage */
    s_ep0_info.trans_data_buffer = buffer;

    /* set transfer data size */
    s_ep0_info.trans_data_size = size;

    /* transmitted data numbers initialize */
    s_ep0_info.transferred_count = 0;

    /* when send data numbers is more than 1byte */
    if (size > 0)
    {
        /* set unsended to SPK sended flag */
        s_ep0_info.spk_sended_flag = FALSE;

        /* when send data numbers is less than 1byte */
        if (size < s_ep0_info.pkt_size)
        {
            /* set sended to SPK sended flag */
            s_ep0_info.spk_sended_flag = TRUE;
        }

        /* control transfer sending process */
        ctrltrans_send_process();
    }
    /* when send data numbers is 0 byte */
    else
    {
        /* set sended to SPK sended flag */
        s_ep0_info.spk_sended_flag = TRUE;

        /* clear endpoint0 sending interrupt */
        usbfunc_reg_clear_ep0int(REG_EP0_SEND_INT);
    }

    /* enable endpoint0 sending interrupt */
    usbfunc_reg_enable_ep0(REG_EP0_SEND_INT);

    return;
}

/**
* @brief            control transfer receiving request
*
* @param            buffer    send data buffer storage
* @param            size      send data size
* @return           none
*
**/
void usbfunc_ctrltrans_receive(uint8_t * buffer, uint16_t size)
{
    /* set transfer data storage */
    s_ep0_info.trans_data_buffer = buffer;

    /* set transfer data size */
    s_ep0_info.trans_data_size = size;

    /* transmitted data numbers initialize */
    s_ep0_info.transferred_count = 0;

    /* set transmitting to transfer state */
    s_ep0_info.trans_status = USBF_EPSTATUS_TRANS;

    /* clear endpoint0 sending interrupt */
    usbfunc_reg_clear_ep0int(REG_EP0_RCV_INT);

    return;
}

/**
* @brief            stop control transfer
*
* @param            none
* @return           none
*
**/
void usbfunc_ctrltrans_abort(void)
{
    uint16_t  transferred_cnt;  /* transferred count */

    /* when transfer state is transmitting */
    if (s_ep0_info.trans_status == USBF_EPSTATUS_TRANS)
    {
        /* retreat transmitted data numbers */
        transferred_cnt = s_ep0_info.transferred_count;

        /* endpoint0 initialize process */
        ctrltrans_ep0_init();

        /* endpoint0 sending FIFO buffer initialize */
        usbfunc_reg_init_ep0fifo(REG_SEND_BUFFER);

        /* endpoint0 receiving FIFO buffer initialize */
        usbfunc_reg_init_ep0fifo(REG_RCV_BUFFER);

        /* cancelled transfer notification */
        usbfunc_event_trans_complete(USBF_EP0, transferred_cnt, USBF_TRANS_CANCELED);
    }
    /* when transfer state is not in transmitting */
    else
    {
        /* endpoint0 initialize process */
        ctrltrans_ep0_init();

        /* clear endpoint0 sending interrupt */
        usbfunc_reg_clear_ep0int(REG_EP0_RCV_INT);
    }

    return;
}

/**
* @brief            control transfer receiving and sending process
*
* @param            type    endpoint0 interrupt type
* @return           none
*
**/
void usbfunc_ctrltrans_process(uint8_t type)
{
    uint16_t  transferred_cnt;  /* transferred count */

    /* retrieval endpoint0 interrupt type */
    switch (type)
    {
        /* setup stage detection */
        case CTRLTRANS_SETUP_DETECTED:

            /* when transfer state is transmitting */
            if (s_ep0_info.trans_status == USBF_EPSTATUS_TRANS)
            {
                /* retreat transmitted data numbers */
                transferred_cnt = s_ep0_info.transferred_count;

                /* endpoint0 initialize process */
                ctrltrans_ep0_init();

                /* endpoint0 sending FIFO buffer initialize */
                usbfunc_reg_init_ep0fifo(REG_SEND_BUFFER);

                /* abandoned transfer notification */
                usbfunc_event_trans_complete(USBF_EP0, transferred_cnt, USBF_TRANS_STOPPED);
            }
            /* when transfer state is stall */
            else if (s_ep0_info.trans_status == USBF_EPSTATUS_STALL)
            {
                /* endpoint0 initialize process */
                ctrltrans_ep0_init();
            }
            else
            {
            	;
            }

            /* setup receiving process */
            ctrltrans_setup_process();

            /* clear endpoint0 sending interrupt */
            usbfunc_reg_clear_ep0int(REG_EP0_RCV_INT);

            break;

        /* data receiving interrupt */
        case CTRLTRANS_DATA_RECEIVED:

            /* when transfer state is transmitting */
            if (s_ep0_info.trans_status == USBF_EPSTATUS_TRANS)
            {
                /* when data stage is data out stage */
                if (s_ep0_info.trans_stage == CTRLTRANS_STAGE_DATAOUT)
                {
                    /* control transfer receiving process */
                    ctrltrans_receive_process();
                }
                /* when transfer stage is status out or data in(if had null packet receiving,end current transfer)*/
                else if ((s_ep0_info.trans_stage == CTRLTRANS_STAGE_STATUSOUT)
                      || (s_ep0_info.trans_stage == CTRLTRANS_STAGE_DATAIN))
                {
                    /* when receive data size is 0 byte */
                    if (usbfunc_reg_get_ep0outpktsize() == (uint16_t)0)
                    {
                        /* transfer normalitilly end notification */
                        usbfunc_event_trans_complete(USBF_EP0, s_ep0_info.transferred_count, USBF_TRANS_COMPLETE);

                        /* in the data stage,if received null packet, abandon prepared sending data */
                        if (s_ep0_info.trans_stage == CTRLTRANS_STAGE_DATAIN)
                        {
                            /* endpoint0 sending FIFO buffer initialize */
                            usbfunc_reg_init_ep0fifo(REG_SEND_BUFFER);
                        }

                        /* endpoint0 initialize process */
                        ctrltrans_ep0_init();
                    }
                    /* when receive data size is more than 1 byte */
                    else
                    {
                        /* set endpoint0 stall */
                        usbfunc_ctrltrans_setstall();

                        /* transfer unnormalitilly end notification */
                        usbfunc_event_trans_complete(USBF_EP0, s_ep0_info.transferred_count, USBF_TRANS_NG);
                    }
                }
                /* when transfer stage is incorrect */
                else
                {
                    /* set endpoint0 stall */
                    usbfunc_ctrltrans_setstall();

                    /* transfer unnormalitilly end notification */
                    usbfunc_event_trans_complete(USBF_EP0, s_ep0_info.transferred_count, USBF_TRANS_NG);
                }
            }
            /* when transfer state is incorrect */
            else
            {
                /* set endpoint0 stall */
                usbfunc_ctrltrans_setstall();

                /* transfer unnormalitilly end notification */
                usbfunc_event_trans_complete(USBF_EP0, s_ep0_info.transferred_count, USBF_TRANS_NG);
            }

            /* clear endpoint0 sending interrupt */
            usbfunc_reg_clear_ep0int(REG_EP0_RCV_INT);

            break;

        /* data sending interrupt */
        case CTRLTRANS_DATA_SENDED:

                /* when transfer stage is data in stage  */
                if (s_ep0_info.trans_stage == CTRLTRANS_STAGE_DATAIN)
                {
                    /* control transfer sending process */
                    ctrltrans_send_process();
                }
                /* when transfer stage is status in or data out(if had null packet sending,end current transfer) */
                else if ((s_ep0_info.trans_stage == CTRLTRANS_STAGE_STATUSIN)
                      || (s_ep0_info.trans_stage == CTRLTRANS_STAGE_DATAOUT))
                {
                    /* transfer normalitilly end notification */
                    usbfunc_event_trans_complete(USBF_EP0, s_ep0_info.transferred_count, USBF_TRANS_COMPLETE);

                    /* endpoint0 initialize process */
                    ctrltrans_ep0_init();

                    /* clear endpoint0 sending interrupt */
                    usbfunc_reg_clear_ep0int(REG_EP0_RCV_INT);
                }
                /* when transfer stage is incorrect */
                else
                {
                    /* set endpoint0 stall */
                    usbfunc_ctrltrans_setstall();

                    /* transfer unnormalitilly end notification */
                    usbfunc_event_trans_complete(USBF_EP0, s_ep0_info.transferred_count, USBF_TRANS_NG);
                }

            break;

        /* excluding above */
        default:

            break;
    }

    return;
}

/**
* @brief            set endpoint0 stall
*
* @param            none
* @return           none
*
**/
void usbfunc_ctrltrans_setstall(void)
{
    /* endpoint0 sending FIFO buffer initialize */
    usbfunc_reg_init_ep0fifo(REG_SEND_BUFFER);

    /* endpoint0 receiving FIFO buffer initialize */
    usbfunc_reg_init_ep0fifo(REG_RCV_BUFFER);

    /* set endpoint0 to stall */
    usbfunc_reg_set_ep0stall(TRUE);

    /* when transfer state is transmitting */
    if (s_ep0_info.trans_status == USBF_EPSTATUS_TRANS)
    {
        /* set stall to transfer state */
        s_ep0_info.trans_status = USBF_EPSTATUS_STALL;

        /* cancelled transfer notification */
        usbfunc_event_trans_complete(USBF_EP0, s_ep0_info.transferred_count, USBF_TRANS_CANCELED);
    }
    /* when transfer state is not in transmitting */
    else
    {
        /* set stall to transfer state */
        s_ep0_info.trans_status = USBF_EPSTATUS_STALL;
    }

    return;
}

/**
* @brief            clear endpoint0 stall
*
* @param            none
* @return           none
*
**/
void usbfunc_ctrltrans_clearstall(void)
{
    /* clear endpoint0 from stall */
    usbfunc_reg_set_ep0stall(FALSE);

    /* setup packet information initialize */
    (void)memset(&s_setup_info, 0, sizeof(USBF_ST_SETUP_INFO));

    /* endpoint0 initialize process */
    ctrltrans_ep0_init();

    return;
}

/**
* @brief            get control transfer state
*
* @param            none
* @return           USBF_EPSTATUS_IDLE     idle
* @return           USBF_EPSTATUS_TRANS    transmitting
* @return           USBF_EPSTATUS_STALL    stall
*
**/
uint8_t usbfunc_ctrltrans_getstatus(void)
{
    /* return transfer state */
    return (s_ep0_info.trans_status);
}

/**
* @brief            get endpoint0 attribute
*
* @param            ep0_info_ptr  pointer to endpoint0 attribute
* @return           none
*
**/
void usbfunc_ctrltrans_get_ep0info(USBFUNC_ST_CTRLTRANS_EPINFO* ep0_info_ptr)
{
    /* set endpoint0 attribute */
    (void)memcpy(ep0_info_ptr, &s_ep0_info, sizeof(USBFUNC_ST_CTRLTRANS_EPINFO));

    return;
}

/**
* @brief            SPK receiving process
*
* @param            none
* @return           none
*
**/
void usbfunc_ctrltrans_spk_process(void)
{

    /* clear endpoint0 SPK interrupt */
    usbfunc_reg_clear_ep0int(REG_EP0_SPK_INT);

    return;
}

/**
* @brief            control transfer sending process
*
* @param            none
* @return           none
*
**/
static void ctrltrans_ep0_init(void)
{
    /* disable endpoint0 sending interrupt */
    usbfunc_reg_disable_ep0(REG_EP0_SEND_INT);

    /* transfer state initialize */
    s_ep0_info.trans_status = USBF_EPSTATUS_IDLE;

    /* transfer stage initialize */
    s_ep0_info.trans_stage = CTRLTRANS_STAGE_NONE;

    /* transfer data storage initialize */
    s_ep0_info.trans_data_buffer = NULL;

    /* transfer data size initialize */
    s_ep0_info.trans_data_size = 0;

    /* transmitted data numbers initialize */
    s_ep0_info.transferred_count = 0;

    /* SPK sended flag initialize */
    s_ep0_info.spk_sended_flag = FALSE;

    /* enable endpoint0 receiving interrupt */
    usbfunc_reg_enable_ep0(REG_EP0_RCV_INT);

    return;
}

/**
* @brief            control transfer sending process
*
* @param            none
* @return           none
*
**/
static void ctrltrans_send_process(void)
{
    uint16_t  remain_cnt;   /* remain data count */
    uint16_t  send_length;  /* send data length */

    /* calculate remain data numbers */
    remain_cnt = s_ep0_info.trans_data_size - s_ep0_info.transferred_count;

    /* when remain data is more than 1byte */
    if (remain_cnt > 0)
    {
        /* set endpoint0 packet size to send data length */
        send_length = s_ep0_info.pkt_size;

        /* when remain data numbers is smaller than send data length */
        if (remain_cnt < send_length)
        {
            /* when descriptor is processing */
            /* (because of descriptor transfer is automatically executes by USB function,transfer end does not need to notofy )*/
            if (usbfunc_stdrequest_is_processing() == TRUE)
            {
                /* set remain data numbers to send data length */
                send_length = remain_cnt;

                /* set sended to SPK sended flag */
                s_ep0_info.spk_sended_flag = TRUE;
            }
            /* when descriptor is not in processing */
            else
            {
                /* when SPK sended */
                if (s_ep0_info.spk_sended_flag == TRUE)
                {
                    /* set remain data numbers to send data length */
                    send_length = remain_cnt;

                    /* set sended to SPK sended flag */
                    s_ep0_info.spk_sended_flag = TRUE;
                }
                /* excluding above */
                else
                {
/* SPK sending is controlled by application layer */
#if (USBF_SEND_SPK_CONTROL == USBF_SEND_SPK_APP_CONTROL)

                    /* set idle to transfer state */
                    s_ep0_info.trans_status = USBF_EPSTATUS_IDLE;

                    /* disable endpoint0 sending interrupt */
                    usbfunc_reg_disable_ep0(REG_EP0_SEND_INT);

                    /* SPK untransmitted notification */
                    usbfunc_event_trans_complete(USBF_EP0, s_ep0_info.transferred_count, USBF_TRANS_SPK_NOTSEND);

                    return;

/* when SPK sending is controlled by driver */
#else
                    /* set remain data numbers to send data length */
                    send_length = remain_cnt;

                    /* set sended to SPK sended flag */
                    s_ep0_info.spk_sended_flag = TRUE;

#endif /* (USBF_SEND_SPK__CONTROL == USBF_SEND_SPK_APP_CONTROL) */
                }
            }
        }

        /* endpoint0 sending FIFO buffer writting */
        usbfunc_reg_write_ep0fifo(s_ep0_info.trans_data_buffer, send_length);

        /* renew transmitted data numbers */
        s_ep0_info.transferred_count += send_length;

        /* renew transfer data storage */
        s_ep0_info.trans_data_buffer += send_length;

        /* clear endpoint0 transfer interrupt */
        usbfunc_reg_clear_ep0int(REG_EP0_SEND_INT);
    }
    /* when remain data numbers is 0byte and SPK has been sended */
    else if (s_ep0_info.spk_sended_flag == TRUE)
    {
        /* transfer stage moves to status out stage */
        s_ep0_info.trans_stage = CTRLTRANS_STAGE_STATUSOUT;

        /* disable endpoint0 sending interrupt */
        usbfunc_reg_disable_ep0(REG_EP0_SEND_INT);
    }
    /* when remain data numbers is 0byte and SPK is not sended */
    else
    {
/* 0 length sending is contrlled by application layer */
#if (USBF_SEND_ZLP_CONTROL == USBF_SEND_ZLP_APP_CONTROL)

        /* when descriptor is processing */
        /* (because of descriptor transfer is automatically executes by USB function,transfer end does not need to notofy)*/
        if (usbfunc_stdrequest_is_processing() == FALSE)
        {
            /* set idle to transfer state */
            s_ep0_info.trans_status = USBF_EPSTATUS_IDLE;

            /* disable endpoint0 sending interrupt */
            usbfunc_reg_disable_ep0(REG_EP0_SEND_INT);

            /* buffer empty notification */
            usbfunc_event_trans_complete(USBF_EP0, s_ep0_info.transferred_count, USBF_BUFFER_EMPTY);
        }
        /* when descriptor is not in processing */
        else
        {
            /* set sended to SPK sended flag */
            s_ep0_info.spk_sended_flag = TRUE;

            /* clear endpoint sending interrupt */
            usbfunc_reg_clear_ep0int(REG_EP0_SEND_INT);
        }

/* 0 length sending is contrlled by driver */
#else

        /* set sended to SPK sended flag */
        s_ep0_info.spk_sended_flag = TRUE;

        /* clear endpoint sending interrupt */
        usbfunc_reg_clear_ep0int(REG_EP0_SEND_INT);

#endif /* (USBF_SEND_ZLP_CONTROL == USBF_SEND_ZLP_APP_CONTROL) */
    }

    return;
}

/**
* @brief            control transfer receiving process
*
* @param            none
* @return           none
*
**/
static void ctrltrans_receive_process (void)
{
    uint16_t  remain_size;     /* remain size of transfer data buffer */
    uint16_t  rcv_length;      /* receive data length  */

    /* get receive data length */
    rcv_length = usbfunc_reg_get_ep0outpktsize();

    /* when receive data length is more than 1 byte */
    if (rcv_length > 0)
    {
        /* read receive data from endpoint0 receiving FIFO buffer */
        usbfunc_reg_read_ep0fifo(s_ep0_info.trans_data_buffer, rcv_length);

        /* renew transmitted data numbers  */
        s_ep0_info.transferred_count += rcv_length;

        /* renew transfer data storage */
        s_ep0_info.trans_data_buffer += rcv_length;

        /* when receive data length is endpoint0 packet size */
        if (rcv_length == s_ep0_info.pkt_size)
        {
            /* calculate transfer data buffer remain size */
            remain_size = s_ep0_info.trans_data_size - s_ep0_info.transferred_count;

            /* remain size is smaller than endpoint0 packet size */
            if (remain_size < s_ep0_info.pkt_size)
            {
                /* set idle to transfer state */
                s_ep0_info.trans_status = USBF_EPSTATUS_IDLE;

                /* buffer full notification */
                usbfunc_event_trans_complete(USBF_EP0, s_ep0_info.transferred_count, USBF_BUFFER_FULL);
            }

        }
        /* when SPK has been received */
        else
        {
            /* transfer stage moves to status in stage */
            s_ep0_info.trans_stage = CTRLTRANS_STAGE_STATUSIN;
        }
    }
    /* when receive data size is 0 byte */
    else
    {
        /* transfer stage moves to status in stage */
        s_ep0_info.trans_stage = CTRLTRANS_STAGE_STATUSIN;

        /* clear endpoint sending interrupt */
        usbfunc_reg_clear_ep0int(REG_EP0_SEND_INT);

        /* enable endpoint0 sending interrupt */
        usbfunc_reg_enable_ep0(REG_EP0_SEND_INT);
    }

    return;
}

/**
* @brief            setup receiving process
*
* @param            none
* @return           none
*
**/
static void ctrltrans_setup_process(void)
{
    uint16_t  rcv_length;      /* receive data length */

    /* get receive data length */
    rcv_length = usbfunc_reg_get_ep0outpktsize();

    /* when receive data size is the same to setup packet size */
    if (rcv_length == CTRLTRANS_SETUP_SIZE)
    {
        /* read receive data from endpoint0 receiving FIFO buffer */
        usbfunc_reg_read_ep0fifo((uint8_t *)&s_setup_info, rcv_length);

        /* it not has the next transfer data(no data stage) */
        if (HWORD_CPU_TO_LE(s_setup_info.wLength) == 0)
        {
            /* set transmitting to transfer state */
            s_ep0_info.trans_status = USBF_EPSTATUS_TRANS;

            /* set status in stage to transfer stage */
            s_ep0_info.trans_stage = CTRLTRANS_STAGE_STATUSIN;

        }
        /* when it has the next transfer data */
        else
        {   /* when the next transfer data stage transfer direction is IN transfer */
            if (s_setup_info.bmRequestType.bits.dir == CTRLTRANS_DIRECTION_IN)
            {
                /* move to data in stage */
                s_ep0_info.trans_stage = CTRLTRANS_STAGE_DATAIN;
            }
            /* when the next transfer data stage transfer direction is OUT transfer */
            else
            {
                /* move to data out stage */
                s_ep0_info.trans_stage = CTRLTRANS_STAGE_DATAOUT;

                /*** before end data out stage,because it has that host gets null packet ***/
                /* clear endpoint sending interrupt */
                usbfunc_reg_clear_ep0int(REG_EP0_SEND_INT);

                /* enable endpoint0 sending interrupt */
                usbfunc_reg_enable_ep0(REG_EP0_SEND_INT);
            }
        }

        /* request type */
        switch (s_setup_info.bmRequestType.bits.type)
        {
            /* standard request */
            case CTRLTRANS_REQUEST_TYPE_STANTARD:

                /* standard request process */
                usbfunc_stdrequest_process(&s_setup_info);

                break;

#if (USBF_CLASS_REQUEST == USBF_ENABLE)

            /* class request */
            case CTRLTRANS_REQUEST_TYPE_CLASS:

                /* request receiving notification */
                usbfunc_event_request(&s_setup_info);

                break;

#endif /* (USBF_CLASS_REQUEST == USBF_ENABLE)) */


#if (USBF_VENDOR_REQUEST == USBF_ENABLE)

            /* vendor request */
            case CTRLTRANS_REQUEST_TYPE_VENDOR:

                /* request receiving notification */
                usbfunc_event_request(&s_setup_info);

                break;

#endif /* (USBF_VENDOR_REQUEST == USBF_ENABLE) */

            default:

                /* set endpoint0 stall */
                usbfunc_ctrltrans_setstall();

                /* error notification that nalyzes setup packet */
                usbfunc_event_error(USBF_ERROR_SETUP, &s_setup_info);

                break;
        }

        /* when no the next transfer data and endpoint0 state is not in stall state(no data stage) */
        /* for avoiding the sending interrupt that after enabled immediately can be responded */
        /* the process after judge request type process */
        if ((HWORD_CPU_TO_LE(s_setup_info.wLength) == 0) && (s_ep0_info.trans_status != USBF_EPSTATUS_STALL))
        {
            /* clear endpoint sending interrupt factor */
            usbfunc_reg_clear_ep0int(REG_EP0_SEND_INT);

            /* enable endpoint0 sending interrupt */
            usbfunc_reg_enable_ep0(REG_EP0_SEND_INT);
        }
    }
    /* when receive data size is incorrect */
    else
    {
        /* set endpoint0 stall */
        usbfunc_ctrltrans_setstall();

        /* error notification that analyzes setup packet */
        usbfunc_event_error(USBF_ERROR_SETUP, &s_setup_info);
    }

    return;
}
