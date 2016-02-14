/**
* @file          usbf_core_data_transfer.c
* @brief         data transfer control module
* @author        lijiale
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/


/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

#include "usbf.h"
#include "usbf_core_data_transfer.h"
#include "usbf_hal_io.h"
#if (USBF_USE_DMA == USBF_ENABLE)  /* use DMA */
#include "usbf_hal_dma.h"
#endif
#include "usbf_core_event.h"
#include "../../src/debug.h"

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Static Variable Definition                                                 */
/*----------------------------------------------------------------------------*/
/* the endpoint numbers which in using */
static uint8_t  s_used_ep_num;

/* endpoint index_ array */
static uint8_t  s_ep_index[USBF_MAX_EP_NUM];

/* endpoint attribute storage */
static USBFUNC_ST_DATATRANS_EPINFO*  s_ep_info;

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/

/* specified endpoint initialize process */
static void datatrans_ep_init(uint8_t ep_id);

/* endpoint index_ array initialize */
static void datatrans_ep_index_init(void);

/* data transfer sending process */
static void datatrans_send_process(uint8_t ep_id);

/* data transfer receiving process */
static void datatrans_receive_process(uint8_t ep_id);

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

/**
* @brief            register endpoint attribute
*
* @param            ep_number    endpoint number
* @param            ep_info_ptr  pointer to endpoint attribute
* @return           none
*
**/

void usbfunc_datatrans_entry_epinfo(uint8_t ep_number, USBFUNC_ST_DATATRANS_EPINFO* ep_info_ptr)
{
    /* endpoint numbers which in using initialize */
    s_used_ep_num = ep_number;

    /* endpoint attribute storage initialize */
    s_ep_info = ep_info_ptr;

    return;
}

/**
* @brief            data transfer control module initialize
*
* @param            none
* @return           none
*
**/
void usbfunc_datatrans_init(void)
{
    uint8_t  loopcnt;    /* loop counter */

    /* endpoint index_ array initialize */
    datatrans_ep_index_init();

//	hex_print("datatrans_init",s_used_ep_num);

    /* all used endpoint attribute initialize */
    for (loopcnt = 0; loopcnt < s_used_ep_num; loopcnt++)
    {
//		hex_print("set_epnattr",loopcnt);

        /* transfer state of endpoint attribute initialize */
        s_ep_info[loopcnt].trans_status = USBF_EPSTATUS_IDLE;

        /* transfer stage of endpoint attribute initialize */
        s_ep_info[loopcnt].trans_stage = DATATRANS_STAGE_NONE;

        /* transfer data storage of endpoint attribute initialize */
        s_ep_info[loopcnt].trans_data_buffer = NULL;

        /* transfer data size of endpoint attribute initialize */
        s_ep_info[loopcnt].trans_data_size = 0;

        /* transmitted data numbers of endpoint attribute initialize */
        s_ep_info[loopcnt].transferred_count = 0;

        /* SPK sended flag of endpoint attribute initialize */
        s_ep_info[loopcnt].spk_sended_flag = FALSE;

        /* set endpoint attribute */
        usbfunc_reg_set_epnattr(&s_ep_info[loopcnt]);
    }

    return;
}

/**
* @brief            data transfer sending request
*
* @param            ep_id    specified endpoint number(1~USBFUNC_MAX_EP_NUM)
* @param            buffer   send data buffer storage
* @param            size     send data size
* @return           none
*
**/
void usbfunc_datatrans_send(uint8_t ep_id, uint8_t * buffer, uint16_t size)
{
    /* retreat endpoint index_ */
    uint8_t  index_ = s_ep_index[ep_id - 1];

    /* set transmitting to transfer state */
    s_ep_info[index_].trans_status = USBF_EPSTATUS_TRANS;

    /* set data in stage to transfer stage */
    s_ep_info[index_].trans_stage = DATATRANS_STAGE_DATAIN;

    /* set transfer data storage */
    s_ep_info[index_].trans_data_buffer = buffer;

    /* set transfer data size */
    s_ep_info[index_].trans_data_size = size;

    /* transmitted data numbers initialize */
    s_ep_info[index_].transferred_count = 0;

    /* when send data numbers is more than 1byte  */
    if (size > 0)
    {
        /* set SPK sended flag */
        s_ep_info[index_].spk_sended_flag = FALSE;

        /* when send data numbers is less than 1byte */
        if (size < s_ep_info[index_].ep_pkt_size)
        {
            /* set sended to SPK sended flag */
            s_ep_info[index_].spk_sended_flag = TRUE;
        }

#if (USBF_USE_DMA == USBF_ENABLE)  /* use DMA */

        /* enable DMA automatic transfer */
        usbfunc_reg_enable_dma(ep_id);

        /* DMA transfer function start */
        usbfunc_dma_start(ep_id + 1, buffer, size);

#else   /* not use DMA */

        /* data transfer sending process */
        datatrans_send_process(ep_id);

#endif /* USBF_USE_DMA == USBF_ENABLE */

    }
    /* when send data is 0 byte  */
    else
    {
        /* set sended to SPK sended flag */
        s_ep_info[index_].spk_sended_flag = TRUE;

        /* clear specified endpoint packet transfer interrupt */
        usbfunc_reg_clear_epntransint(ep_id);
    }

    /* enable specified endpoint packet transfer interrupt */
    usbfunc_reg_enable_epntrans(ep_id);

    return;
}

/**
* @brief            data transfer receiving request
*
* @param            ep_id    specified endpoint number(1~USBFUNC_MAX_EP_NUM)
* @param            buffer   receive data buffer storage
* @param            size     receive data size
* @return           none
*
**/
void usbfunc_datatrans_receive(uint8_t ep_id, uint8_t * buffer, uint16_t size)
{
    /* retreat endpoint index_ */
    uint8_t  index_ = s_ep_index[ep_id - 1];

    /* set transfer data storage */
    s_ep_info[index_].trans_data_buffer = buffer;

    /* set transfer data size */
    s_ep_info[index_].trans_data_size = size;

    /* transmitted data numbers initialize */
    s_ep_info[index_].transferred_count = 0;

    /* set transmitting to transfer state */
    s_ep_info[index_].trans_status = USBF_EPSTATUS_TRANS;

    /* set data out stage to transfer stage */
    s_ep_info[index_].trans_stage = DATATRANS_STAGE_DATAOUT;

#if (USBF_USE_DMA == USBF_ENABLE)  /* use DMA */
    /* enable DMA automatic transfer */
    usbfunc_reg_enable_dma(ep_id);

    /* DMA transfer function start */
    usbfunc_dma_start(ep_id + 1, buffer, size);
#endif /* USBF_USE_DMA == USBF_ENABLE */

    /* enable specified endpoint packet transfer interrupt */
    usbfunc_reg_enable_epntrans(ep_id);

#if (USBF_USE_DMA == USBF_ENABLE)  /* use DMA */
    /* enable SPK interrupt  */
    usbfunc_reg_enable_epnspk(ep_id);
#endif /* USBF_USE_DMA == USBF_ENABLE */

    return;
}

/**
* @brief            stop data transfer
*
* @param            ep_id    specified endpoint number(1~USBFUNC_MAX_EP_NUM)
* @return           none
*
**/
void usbfunc_datatrans_abort(uint8_t ep_id)
{
    uint16_t  transferred_cnt;  /* transfered data count */

    /* retreat endpoint index_ */
    uint8_t  index_ = s_ep_index[ep_id - 1];

    /* when transfer state is transmitting */
    if (s_ep_info[index_].trans_status == USBF_EPSTATUS_TRANS)
    {

#if (USBF_USE_DMA == USBF_ENABLE)  /* use DMA */

        /* DMA transfer end process */
        usbfunc_dma_complete(ep_id + 1);

        /* disable DMA automatic transfer */
        usbfunc_reg_disable_dma(ep_id);

        /* disable SPK interrupt */
        usbfunc_reg_disable_epnspk(ep_id);

        /* is sending to host (by word unit) */
        if (s_ep_info[index_].trans_stage == DATATRANS_STAGE_DATAIN)
        {
            /* calculate transmitted data numbers */
            s_ep_info[index_].transferred_count = s_ep_info[index_].trans_data_size - (uint16_t)(USBF_REG_R(IO_DMAC.DDCT) * 2);
        }
        /* is receiving from host(by byte unit) */
        else if (s_ep_info[index_].trans_stage == DATATRANS_STAGE_DATAOUT)
        {
          /* calculate transmitted data numbers */
            s_ep_info[index_].transferred_count = s_ep_info[index_].trans_data_size - USBF_REG_R(IO_DMAC.DDCT);
        }
        else
        {
          /* no process */
        }

#endif /* USBF_USE_DMA == USBF_ENABLE */

        /* save transmitted data numbers */
        transferred_cnt = s_ep_info[index_].transferred_count;

        /* specified endpoint initialize process */
        datatrans_ep_init(ep_id);

        /* specified endpoint FIFO buffer initialize */
        usbfunc_reg_init_epnfifo(ep_id);

        /*  cancelled transfer notification */
        usbfunc_event_trans_complete(ep_id, transferred_cnt, USBF_TRANS_CANCELED);
    }
    /* when transfer state is not in transmitting  */
    else
    {
        /* specified endpoint initialize process */
        datatrans_ep_init(ep_id);
    }

    return;
}

/**
* @brief            data transfer receiving and sending process
*
* @param            ep_id    specified endpoint number(1~USBFUNC_MAX_EP_NUM)
* @return           none
*
**/
void usbfunc_datatrans_process(uint8_t ep_id)
{
    /* retreat endpoint index_ */
    uint8_t  index_ = s_ep_index[ep_id - 1];

    /* when transfer state is transmitting */
    if (s_ep_info[index_].trans_status == USBF_EPSTATUS_TRANS)
    {
        /* when transfer stage is data in stage  */
        if (s_ep_info[index_].trans_stage == DATATRANS_STAGE_DATAIN)
        {
            /* data transfer sending process */
            datatrans_send_process(ep_id);
        }
        /* when transfer stage is data out stage */
        else if (s_ep_info[index_].trans_stage == DATATRANS_STAGE_DATAOUT)
        {
            /* data transfer receiving process */
            datatrans_receive_process(ep_id);
        }
        /* excluding above */
        else
        {
            /* specified endpoint initialize process */
            datatrans_ep_init(ep_id);

            /* specified endpoint FIFO buffer initialize */
            usbfunc_reg_init_epnfifo(ep_id);
        }
    }
    /* when transfer state is not in transmitting */
    else
    {
        /* specified endpoint initialize process */
        datatrans_ep_init(ep_id);

        /* specified endpoint FIFO buffer initialize */
        usbfunc_reg_init_epnfifo(ep_id);
    }

    return;
}

/**
* @brief            set endpoint stall
*
* @param            ep_id    specified endpoint number(1~USBFUNC_MAX_EP_NUM)
* @return           none
*
**/
void usbfunc_datatrans_setstall(uint8_t ep_id)
{
    /* retreat endpoint index_ */
    uint8_t  index_ = s_ep_index[ep_id - 1];

    /* set specified endpoint to stall */
    usbfunc_reg_set_epnstall(ep_id, TRUE);

    /* when transfer state is transmitting */
    if (s_ep_info[index_].trans_status == USBF_EPSTATUS_TRANS)
    {
        /* set stall to transfer state  */
        s_ep_info[index_].trans_status = USBF_EPSTATUS_STALL;

        /* cancelled transfer notification */
        usbfunc_event_trans_complete(ep_id, s_ep_info[index_].transferred_count, USBF_TRANS_CANCELED);
    }
    /* when transfer state is not in transmitting */
    else
    {
        /* set stall to transfer state */
        s_ep_info[index_].trans_status = USBF_EPSTATUS_STALL;
    }

    return;
}

/**
* @brief            clear endpoint stall
*
* @param            ep_id    specified endpoint number(1~USBFUNC_MAX_EP_NUM)
* @return           none
*
**/
void usbfunc_datatrans_clearstall(uint8_t ep_id)
{
    /* specified endpoint initialize process */
    datatrans_ep_init(ep_id);

    /* specified endpoint FIFO buffer initialize */
    usbfunc_reg_init_epnfifo(ep_id);

    /* clear specified endpoint from stall */
    usbfunc_reg_set_epnstall(ep_id, FALSE);

    return;
}

/**
* @brief            get data transfer state
*
* @param            ep_id    specified endpoint number(1~USBFUNC_MAX_EP_NUM)
* @return           USBFUNC_EPSTATUS_IDLE     idle
* @return           USBFUNC_EPSTATUS_TRANS    transmitting
* @return           USBFUNC_EPSTATUS_STALL    stall
*
**/
uint8_t usbfunc_datatrans_getstatus(uint8_t ep_id)
{
    /* retreat endpoint index_ */
    uint8_t  index_ = s_ep_index[ep_id - 1];

    /* return transfer state */
    return (s_ep_info[index_].trans_status);
}

/**
* @brief            get endpoint attribute
*
* @param            ep_id    specified endpoint number(1~USBFUNC_MAX_EP_NUM)
* @param            ep_info_ptr  endpoint attribute pointer
* @return           0~(USBFUNC_MAX_EP_NUM-1) used endpoint index_
* @return           DATATRANS_UNUSED_EPID     unused endpoint index_
*
**/
uint8_t usbfunc_datatrans_get_epinfo(uint8_t ep_id, USBFUNC_ST_DATATRANS_EPINFO* ep_info_ptr)
{
    /* retreat endpoint index_ */
    uint8_t  index_ = s_ep_index[ep_id - 1];

    /* set endpoint index_ to return value */
    uint8_t  ret = index_;

    /* when endpoint is not idle */
    if (ret != DATATRANS_UNUSED_EPID)
    {
        /* set endpoint attribute */
        (void)memcpy(ep_info_ptr, &(s_ep_info[index_]), sizeof(USBFUNC_ST_DATATRANS_EPINFO));
    }

    return (ret);
}

/**
* @brief            get endpoint number
*
* @param            ep_index    endpoint index_
* @return           1~(USBFUNC_MAX_EP_NUM)    used endpoint number
* @return           DATATRANS_INVALID_INDEX    invalid endpoint index_
*
**/
uint8_t usbfunc_datatrans_get_epid(uint8_t ep_index)
{
    uint8_t  ret;  /* return value */

    /* when endpoint index_ is effective */
    if (ep_index < s_used_ep_num)
    {
        /* set endpoint number to return value */
        ret = s_ep_info[ep_index].ep_id;
    }
    /* when endpoint index_ is invalid */
    else
    {
        /* set invalid endpoint index_ to return value */
        ret = DATATRANS_INVALID_INDEX;
    }

    return (ret);
}

/**
* @brief            get the endpoint numbers which in using
*
* @param            none
* @return           0~(USBFUNC_MAX_EP_NUM)   endpoint numbers which in using
*
**/
uint8_t usbfunc_datatrans_get_epnum(void)
{
    /* return endpoint numbers which in using */
    return (s_used_ep_num);
}

/**
* @brief            SPK receiving process
*
* @param            ep_id    specified endpoint number(1~USBFUNC_MAX_EP_NUM)
* @return           none
*
**/
void usbfunc_datatrans_spk_process(uint8_t ep_id)
{
    /* clear specified endpoint SPK receive interrupt */
    usbfunc_reg_clear_epnspkint(ep_id);

#if (USBF_USE_DMA == USBF_ENABLE)  /* use DMA */

    /* when DMA transfer has started */
    if (usbfunc_dma_get_startflag(ep_id + 1) == TRUE)
    {
        /* data transfer DMA receiving and sending process */
        usbfunc_datatrans_dma_process(ep_id);
    }

#endif /* USBF_USE_DMA == USBF_ENABLE */

    return;
}

/**
* @brief            specified endpoint initialize process
*
* @param            ep_id    specified endpoint number(1~USBFUNC_MAX_EP_NUM)
* @return           none
*
**/
static void datatrans_ep_init(uint8_t ep_id)
{
    /* retreat endpoint index_ */
    uint8_t  index_ = s_ep_index[ep_id - 1];

//	hex_print("datatrans_ep_init",ep_id);

    /* disable specified endpoint packet transfer interrupt */
    usbfunc_reg_disable_epntrans(ep_id);

    /* transfer state initialize */
    s_ep_info[index_].trans_status = USBF_EPSTATUS_IDLE;

    /* transfer stage initialize */
    s_ep_info[index_].trans_stage = DATATRANS_STAGE_NONE;

    /* transfer data storage initialize */
    s_ep_info[index_].trans_data_buffer = NULL;

    /* transfer data size initialize */
    s_ep_info[index_].trans_data_size = 0;

    /* transmitted data numbers initialize */
    s_ep_info[index_].transferred_count = 0;

    /* SPK sended flag initialize */
    s_ep_info[index_].spk_sended_flag = FALSE;

    return;
}

/**
* @brief            endpoint index_ array initialize
*
* @param            none
* @return           none
*
**/
static void datatrans_ep_index_init(void)
{
    uint8_t  loopcnt;  /* loop counter */

    /* endpoint index_ array initialize */
    (void)memset(s_ep_index, DATATRANS_UNUSED_EPID, sizeof(s_ep_index));

    /* used endpoint index_ initialize */
    for (loopcnt = 0; loopcnt < s_used_ep_num; loopcnt++)
    {
        /* the loop count is setted to the element of the endpoint index_ array by the loop count */
        s_ep_index[s_ep_info[loopcnt].ep_id - 1] = loopcnt;
    }

    return;
}

/**
* @brief            data transfer sending process
*
* @param            ep_id    specified endpoint number(1~USBFUNC_MAX_EP_NUM)
* @return           none
*
**/
static void datatrans_send_process(uint8_t ep_id)
{
    uint16_t  remain_cnt;   /* remain data count */
    uint16_t  send_length;  /* send data length  */

    /* retreat endpoint index_ */
    uint8_t   index_ = s_ep_index[ep_id - 1];

    /* retreat transmitted data numbers  */
    uint16_t  transferred_cnt = s_ep_info[index_].transferred_count;

    /* calculate remain data numbers */
    remain_cnt = s_ep_info[index_].trans_data_size - s_ep_info[index_].transferred_count;

    /* when remain data is more than 1byte */
    if (remain_cnt > 0)
    {
        /* set endpoint packet size to send data length */
        send_length = s_ep_info[index_].ep_pkt_size;

        /* when remain data numbers is smaller than send data length */
        if (remain_cnt < send_length)
        {
            /* has SPK been sended? */
            if (s_ep_info[index_].spk_sended_flag == TRUE)
            {
                /* set remain data numbers to send data length */
                send_length = remain_cnt;
            }
            /* SPK is not  sended */
            else
            {
/* when SPK sending is controlled by application layer */
#if (USBF_SEND_SPK_CONTROL == USBF_SEND_SPK_APP_CONTROL)

                /* set idle to transfer state */
                s_ep_info[index_].trans_status = USBF_EPSTATUS_IDLE;

                /* disable specified endpoint packet transfer interrupt */
                usbfunc_reg_disable_epntrans(ep_id);

                /* SPK untransmitted notification */
                usbfunc_event_trans_complete(ep_id, transferred_cnt, USBF_TRANS_SPK_NOTSEND);

                return;

/* when SPK sending is controlled by driver */
#else

                /* set remain data numbers to send data length */
                send_length = remain_cnt;

                /* set sended to SPK sended flag */
                s_ep_info[index_].spk_sended_flag = TRUE;
#endif
            }
        }

        /* endpoint FIFO buffer writting */
        usbfunc_reg_write_epnfifo(ep_id, s_ep_info[index_].trans_data_buffer, send_length);

        /* renew transmitted data numbers */
        s_ep_info[index_].transferred_count = transferred_cnt + send_length;

        /* renew transfer data storage */
        s_ep_info[index_].trans_data_buffer += send_length;

        /* clear specified endpoint transfer interrupt */
        usbfunc_reg_clear_epntransint(ep_id);
    }
    /* when remain data numbers is 0byte and SPK has been sended */
    else if (s_ep_info[index_].spk_sended_flag == TRUE)
    {
        /* specified endpoint initialize process */
        datatrans_ep_init(ep_id);

        /* transfer normalitilly end notification */
        usbfunc_event_trans_complete(ep_id, transferred_cnt, USBF_TRANS_COMPLETE);
    }
    /* when remain data numbers is 0byte and SPK is not sended */
    else
    {
/* 0 length sending is contrlled by application layer */
#if (USBF_SEND_ZLP_CONTROL == USBF_SEND_ZLP_APP_CONTROL)

        /* set idle to transfer state */
        s_ep_info[index_].trans_status = USBF_EPSTATUS_IDLE;

        /* disable specified endpoint packet transfer interrupt */
        usbfunc_reg_disable_epntrans(ep_id);

        /* buffer empty notification */
        usbfunc_event_trans_complete(ep_id, transferred_cnt, USBF_BUFFER_EMPTY);

/* 0 length sending is contrlled by driver */
#else

        /* set sended to SPK sended flag */
        s_ep_info[index_].spk_sended_flag = TRUE;

        /* clear specified endpoint transfer interrupt */
        usbfunc_reg_clear_epntransint(ep_id);

#endif /* (USBF_SEND_ZLP_CONTROL == USBF_SEND_ZLP_APP_CONTROL) */
    }

    return;
}

/**
* @brief            data transfer receiving process
*
* @param            ep_id    specified endpoint number(1~USBFUNC_MAX_EP_NUM)
* @return           none
*
**/
static void datatrans_receive_process(uint8_t ep_id)
{
    uint16_t  remain_size;      /* transfer data buffer remain size */
    uint16_t  rcv_length;       /* receive data length */
    uint16_t  transferred_cnt;  /* transferred count*/

    /* retreat endpoint index_ */
    uint8_t   index_ = s_ep_index[ep_id - 1];

    /* get receive data length */
    rcv_length = usbfunc_reg_get_epnoutpktsize(ep_id);

    /* retreat transmitted data numbers */
    transferred_cnt = s_ep_info[index_].transferred_count;

    /* when receive data length is more than 1byte */
    if (rcv_length > 0)
    {
        /* endpoint FIFO buffer reading */
        usbfunc_reg_read_epnfifo(ep_id, s_ep_info[index_].trans_data_buffer, rcv_length);

        /* renew transmitted data numbers */
        s_ep_info[index_].transferred_count += rcv_length;

        /* renew transfer data storage */
        s_ep_info[index_].trans_data_buffer += rcv_length;

        /* retreat transmitted data numbers */
        transferred_cnt = s_ep_info[index_].transferred_count;

        /* when receive data length is endpoint packet size */
        if (rcv_length == s_ep_info[index_].ep_pkt_size)
        {
            /* calculate transfer data buffer remain size */
            remain_size = s_ep_info[index_].trans_data_size - transferred_cnt;

            /* remain size is smaller than endpoint packet size */
            if (remain_size < s_ep_info[index_].ep_pkt_size)
            {
                /* set idle to transfer state */
                s_ep_info[index_].trans_status = USBF_EPSTATUS_IDLE;

                /* disable specified endpoint packet transfer interrupt */
                usbfunc_reg_disable_epntrans(ep_id);

                /* clear specified endpoint transfer interrupt */
                usbfunc_reg_clear_epntransint(ep_id);

                /* buffer full notification */
                usbfunc_event_trans_complete(ep_id, transferred_cnt, USBF_BUFFER_FULL);
            }
            /* remain size is larger than endpoint packet size */
            else
            {
                /* clear specified endpoint transfer interrupt */
                usbfunc_reg_clear_epntransint(ep_id);
            }
        }
        /* when SPK has been received  */
        else
        {
            /* clear specified endpoint transfer interrupt */
            usbfunc_reg_clear_epntransint(ep_id);

            /* specified endpoint initialize process */
            datatrans_ep_init(ep_id);

            /* transfer normalitilly end notification */
            usbfunc_event_trans_complete(ep_id, transferred_cnt, USBF_TRANS_COMPLETE);
        }

    }
    /* receive data size is 0byte */
    else
    {
        /* clear specified endpoint transfer interrupt */
        usbfunc_reg_clear_epntransint(ep_id);

        /* specified endpoint initialize process */
        datatrans_ep_init(ep_id);

        /* transfer normalitilly end notification */
        usbfunc_event_trans_complete(ep_id, transferred_cnt, USBF_TRANS_COMPLETE);
    }

    return;
}


#if (USBF_USE_DMA == USBF_ENABLE)  /* use DMA */
/**
* @brief            data transfer DMA receiving and sending process
*
* @param            ep_id    specified endpoint number(1~USBFUNC_MAX_EP_NUM)
* @return           none
*
**/
void usbfunc_datatrans_dma_process(uint8_t ep_id)
{
    uint16_t  transferred_cnt;  /* transferred count */

    /* retreat endpoint index_ */
    uint8_t   index_ = s_ep_index[ep_id - 1];

    /* when receive from host */
    if (s_ep_info[index_].trans_stage == DATATRANS_STAGE_DATAOUT)
    {
        /* retreat transmitted data numbers */
        transferred_cnt = s_ep_info[index_].trans_data_size - usbfunc_dma_get_ddct(ep_id + 1);
    }
    /* when send to host */
    else
    {
      /* retreat endpoint index_ */
        transferred_cnt = s_ep_info[index_].trans_data_size;
    }

    /* DMA transfer end process */
    usbfunc_dma_complete(ep_id + 1);

    /* disable DMA automatic transfer */
    usbfunc_reg_disable_dma(ep_id);

    /* disable SPK interrupt */
    usbfunc_reg_disable_epnspk(ep_id);

    /* when receive from host */
    if (s_ep_info[index_].trans_stage == DATATRANS_STAGE_DATAOUT)
    {
        /* when buffer is full */
        if (transferred_cnt == s_ep_info[index_].trans_data_size)
        {
            /* set idle to transfer state */
            s_ep_info[index_].trans_status = USBF_EPSTATUS_IDLE;

            /* disable specified endpoint packet transfer interrupt */
            usbfunc_reg_disable_epntrans(ep_id);

            /* buffer full notification */
            usbfunc_event_trans_complete(ep_id, transferred_cnt, USBF_BUFFER_FULL);
        }
        else
        {
            /* specified endpoint initialize process */
            datatrans_ep_init(ep_id);

            /* transfer normalitilly end notification */
            usbfunc_event_trans_complete(ep_id, transferred_cnt, USBF_TRANS_COMPLETE);
        }
    }
    /* when send to host */
    else
    {
        /* when integral multiples endpoint packet size data is sended and transmitted data number is not 0 */
        if (((transferred_cnt % s_ep_info[index_].ep_pkt_size) == 0) && (transferred_cnt != 0))
        {
#if (USBF_TRANS_NULL == USBF_DISABLE)

            /* set idle to transfer state */
            s_ep_info[index_].trans_status = USBF_EPSTATUS_IDLE;

            /* disable specified endpoint packet transfer interrupt */
            usbfunc_reg_disable_epntrans(ep_id);

            /* buffer empty notification */
            usbfunc_event_trans_complete(ep_id, transferred_cnt, USBF_BUFFER_EMPTY);
#else

            /* specified endpoint initialize process */
            datatrans_ep_init(ep_id);

            /* transfer normalitilly end notification */
            usbfunc_event_trans_complete(ep_id, transferred_cnt, USBF_TRANS_COMPLETE);
#endif
        }
        else
        {
            /* specified endpoint initialize process */
            datatrans_ep_init(ep_id);

            /* transfer normalitilly end notification */
            usbfunc_event_trans_complete(ep_id, transferred_cnt, USBF_TRANS_COMPLETE);
        }
    }

    return;
}
#endif /* USBF_USE_DMA == USBF_ENABLE */
