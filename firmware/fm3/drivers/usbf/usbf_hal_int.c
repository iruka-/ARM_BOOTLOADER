/**
* @file          usbf_hal_int.c
* @brief         interrupt control module
* @author        zhangjie
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

#include "usbf.h"
#include "usbf_hal_io.h"
#include "usbf_core_data_transfer.h"
#include "usbf_core_control_transfer.h"
#include "usbf_core_event.h"
#include "usbf_hal_int.h"

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Static Variable Definition                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/
#if (CPU_TYPE == CPU_TYPE_32)
/* control transfer interrupt process */
static void int_ep0(void);

/* data transfer interrupt process */
static void int_epn(void);

/* endpoint 0~3 SPK interrupt process */
static void int_spk(void);

/* each USB event interrupt process */
static void int_event(void);
#endif

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

/**
* @brief            USB function interrupt process module initialize
*
* @param            none
* @return           none
*
**/
void usbfunc_interrupt_init(void)
{
#if (CPU_TYPE == CPU_TYPE_16)
    /* USB function1 and USB function2 interrupt level setting */
    REG_W(CPU_MAP_IO_ICR00, USBF_INTERRUPT_LEVEL);

    /* USB function3 and USB function4 interrupt level setting */
    REG_W(CPU_MAP_IO_ICR01, USBF_INTERRUPT_LEVEL);
#elif (CPU_TYPE == CPU_TYPE_32)
    /* USB function(endpoint 1 ~ 5 DRQ)interrupt level setting */
    NVIC_SetPriority(USB0F_IRQn, USBF_INTERRUPT_LEVEL);
    NVIC_EnableIRQ(USB0F_IRQn);

    /* USB function(endpoint 0  DRQI, DRQO and each status)interrupt level setting */
    NVIC_SetPriority(USB0F_USB0H_IRQn, USBF_INTERRUPT_LEVEL);
    NVIC_EnableIRQ(USB0F_USB0H_IRQn);
#endif
    return;
}

/**
* @brief            control transfer interrupt process
*
* @param            none
* @return           none
*
**/
#if (CPU_TYPE == CPU_TYPE_16)
void USBF_UsbFunction1_IRQHandler(void)
#elif (CPU_TYPE == CPU_TYPE_32)
static void int_ep0(void)
#endif
{
    uint8_t trans_parameter;   /* transfer process parameter */

    /* interrupt flag that the endpoint0 receives data is setted? */
    if (usbfunc_reg_get_ep0int(REG_EP0_RCV_INT) == TRUE)
    {
        /* set receiving process to transfer process parameter  */
        trans_parameter = CTRLTRANS_DATA_RECEIVED;

        /* the setup stage detection flag is setted? */
        if (usbfunc_reg_get_setupflag() == TRUE)
        {
            /* setup detection event process */
            usbfunc_event_setup();

            /* clear setup stage detection flag */
            usbfunc_reg_clear_setupflag();

            /* set setup stage detection process to transfer parameter */
            trans_parameter = CTRLTRANS_SETUP_DETECTED;
        }

        /* call endpoint0 transfer process */
        usbfunc_ctrltrans_process(trans_parameter);

    }
    /* interrupt flag that the endpoint0 sends data is setted? */
    else if (usbfunc_reg_get_ep0int(REG_EP0_SEND_INT) == TRUE)
    {
        /* set sending process to transfer parameter */
        trans_parameter = CTRLTRANS_DATA_SENDED;

        /* call endpoint0 transfer process */
        usbfunc_ctrltrans_process(trans_parameter);
    }
    else
    {
        ;
    }

    return;
}

/**
* @brief            data transfer interrupt process
*
* @param            none
* @return           none
*
**/
#if (CPU_TYPE == CPU_TYPE_16)
void USBF_UsbFunction2_IRQHandler(void)
#elif (CPU_TYPE == CPU_TYPE_32)
static void int_epn(void)
#endif
{
    static uint8_t s_epindex; /* endpoint index */
    uint8_t loop_count;       /* loop count */
    uint8_t epid;             /* specified endpoint number */
    uint8_t used_ep_num;      /* number of endpoints used(excludes endpoint0) */

    /* get the number of endpoints used(excludes endpoint0) */
    used_ep_num = usbfunc_datatrans_get_epnum();

    /* packet transfer interrupt process */
    for (loop_count = 0; loop_count < used_ep_num; loop_count ++)
    {
        /* by the index of the endpoint,get the specified endpoint number */
        epid = usbfunc_datatrans_get_epid(s_epindex);

        s_epindex ++;

        /* when the end piont index is larger then the number of endpoints used,clear end piont index  */
        if (s_epindex >= used_ep_num)
        {
            s_epindex = 0;
        }

#if (USBF_USE_DMA == USBF_ENABLE)  /* use DMA */

        /* when the DMA interrupt flag of endpoint is setted or sending packet is 0 lengths*/
        /* passes the number of DMA channel and calls DMA sending and receiving process who belongs to data transfer*/
        if ((usbfunc_dma_get_completestatus(epid + 1) != FALSE) ||
            (usbfunc_reg_get_epntransint(epid) == TRUE))
        {
            usbfunc_datatrans_dma_process(epid);

            break;
        }

#else
        /* when the packet interrupt flag of the endpoint is setted */
        /* passes the number of the endpoint and calls data transfer process */
        if (usbfunc_reg_get_epntransint(epid) == TRUE)
        {
            usbfunc_datatrans_process(epid);

            break;
        }

#endif /* USBF_USE_DMA == USBF_ENABLE */

    }

    return;
}

/**
* @brief            endpoint 0~3 SPK interrupt process
*
* @param            none
* @return           none
*
**/
#if (CPU_TYPE == CPU_TYPE_16)
void USBF_UsbFunction4_IRQHandler(void)
#elif (CPU_TYPE == CPU_TYPE_32)
static void int_spk(void)
#endif
{
    static uint8_t s_epindex;                       /* endpoint index */
    static uint8_t s_ctrl_process_flag = TRUE;      /* control transfer process flag*/
    uint8_t loop_count;                             /* loop count */
    uint8_t epid;                                   /* specified endpoint number */
    uint8_t used_ep_num;                            /* number of endpoints used(excludes endpoint0) */

    /* control transfer process flag is setted? */
    if (s_ctrl_process_flag == TRUE)
    {
        /* if endpoint SPK interrupt flag is setted,call the SPK process of the control transfer */
        if (usbfunc_reg_get_ep0int(REG_EP0_SPK_INT) == TRUE)
        {
            usbfunc_ctrltrans_spk_process();
        }

        /* clear Control transfer process flag */
        s_ctrl_process_flag = FALSE;
   }
   else
   {
        /* get the number of endpoints used */
        used_ep_num = usbfunc_datatrans_get_epnum();

        for (loop_count = 0; loop_count < used_ep_num; loop_count ++)
        {
            /* by the index of the endpoint,get the specified endpoint number */
            epid = usbfunc_datatrans_get_epid(s_epindex);

            s_epindex ++;

            /* when the end piont index is larger then the number of endpoints used */
            if (s_epindex >= used_ep_num)
            {
                /* clear endpoint index */
                s_epindex = 0;

                /* set control transfer process flag */
                s_ctrl_process_flag = TRUE;
            }

            /* if the specified endpoint SPK interrupt flag is setted,call SPK interrupt process */
            if (usbfunc_reg_get_epnspkint(epid) == TRUE)
            {
                usbfunc_datatrans_spk_process(epid);

                break;
            }
       }
    }

    return;
}

/**
* @brief            each USB event interrupt process each USB event interrupt process suspend wakeup
*
* @param            none
* @return           none
*
**/
#if (CPU_TYPE == CPU_TYPE_16)
void USBF_UsbFunction3_IRQHandler(void)
#elif (CPU_TYPE == CPU_TYPE_32)
static void int_event(void)
#endif
{
    /* if buss reset detection flag is setted,call bus reset detection process */
    if (usbfunc_reg_get_eventint(REG_BUSRST_INT) == TRUE)
    {
        usbfunc_event_busreset();
    }

#if (USBF_SOF_ENABLE == USBF_ENABLE)
    /* if SOF detection flag is setted,call SOF receiving detection process */
    else if (usbfunc_reg_get_eventint(REG_SOF_INT) == TRUE)
    {
        usbfunc_event_sof();
    }
#endif

    /* if configuration detection flag is setted,call configuration detection process */
    else if (usbfunc_reg_get_eventint(REG_CONFIG_INT) == TRUE)
    {
        usbfunc_event_config();
    }
    /* if suspend detection flag is setted,call suspend detection process */
    else if (usbfunc_reg_get_eventint(REG_SUSPEND_INT) == TRUE)
    {
        usbfunc_event_suspend();
    }
    /* if wakeup detection flag is setted,call wakeup detection process */
    else if (usbfunc_reg_get_eventint(REG_WAKEUP_INT) == TRUE)
    {
        usbfunc_event_wakeup();
    }
    else
    {
        ;
    }

    return;
}

#if (CPU_TYPE == CPU_TYPE_32)
/**
* @brief            USB function(endpoint1~3 DRQ)interrupt process
*
* @param            none
* @return           none
*
**/
void USBF_UsbFunction1_IRQHandler(void)
{
    /* data transfer interrupt process */
    int_epn();

    return;
}

/**
* @brief            USB function(endpoint0  DRQI, DRQO and each status) interrupt process
*
* @param            none
* @return           none
*
**/
void USBF_UsbFunction2_IRQHandler(void)
{
    /* when it has the control transfer interrupt  */
    if ((usbfunc_reg_get_ep0int(REG_EP0_RCV_INT) == TRUE)   ||
        (usbfunc_reg_get_ep0int(REG_EP0_SEND_INT) == TRUE))
    {
        /* control transfer interrupt process  */
        int_ep0();
    }
    /*  when it has the event intrrupt */
    else if ((usbfunc_reg_get_eventint(REG_BUSRST_INT) == TRUE) || /* bus reset */
#if (USBF_SOF_ENABLE == USBF_ENABLE)
             (usbfunc_reg_get_eventint(REG_SOF_INT) == TRUE)    || /* SOF */
#endif
             (usbfunc_reg_get_eventint(REG_CONFIG_INT) == TRUE) || /* set configuration */
             (usbfunc_reg_get_eventint(REG_SUSPEND_INT) == TRUE)|| /* suspend */
             (usbfunc_reg_get_eventint(REG_WAKEUP_INT) == TRUE))   /* wakeup */
    {
        /* event interrupt process */
        int_event();
    }
    else
    {
        /* SPK interrupt process */
        int_spk();
    }

    return;
}

#endif
