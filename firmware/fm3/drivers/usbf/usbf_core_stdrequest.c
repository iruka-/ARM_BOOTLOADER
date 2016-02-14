/**
* @file          usbf_core_stdrequest.c
* @brief         standard request control module
* @author        lijiale
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/


/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "usbf.h"
#include "usbf_core_stdrequest.h"
#include "usbf_core_event.h"
#include "usbf_core_descriptor_analyze.h"
#include "usbf_core_control_transfer.h"

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

#define USBFUNC_STD_REQUEST_OTHERS   (3)  /* others request */

/*----------------------------------------------------------------------------*/
/* Static Variable Definition                                                 */
/*----------------------------------------------------------------------------*/

static uint8_t   s_std_proc_flag;   /* standard request process flag */

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/

/* Get_Descriptor request process */
static void stdrequest_getdescriptor(USBF_ST_SETUP_INFO* setup_pkt);

/* request error process */
static void stdrequest_request_error(void);

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

/**
* @brief            standard request process
*
* @param            setup_pkt    pointer to setup packet
* @return           none
*
**/
void usbfunc_stdrequest_process(USBF_ST_SETUP_INFO* setup_pkt)
{
    /* request code */
    switch (setup_pkt->bRequest)
    {
        /* Get_Descriptor request */
        case USBF_REQ_GET_DESCRIPTOR:

            /* Get_Descriptor request process */
            stdrequest_getdescriptor(setup_pkt);

            break;

/* when Set_Descriptor standard request in using */
#if (USBF_STD_REQUEST_SETDESCRIPTOR == USBF_ENABLE)

        /* Set_Descriptor request */
        case USBF_REQ_SET_DESCRIPTOR:

            /* request receiving notification */
            usbfunc_event_request(setup_pkt);

            break;

#endif /* (USBF_STD_REQUEST_SETDESCRIPTOR == USBF_ENABLE) */

/* when Sync_Frame standard request in using */
#if (USBF_STD_REQUEST_SYNCHFRAME == USBF_ENABLE)

        /* Sync_Frame request */
        case USBF_REQ_SYNC_FRAME:

            /* request receiving notification */
            usbfunc_event_request(setup_pkt);

            break;

#endif /* (USBF_STD_REQUEST_SYNCHFRAME == USBF_ENABLE) */

        /* request not supported */
        case USBF_REQ_GET_STATUS:
        case USBF_REQ_CLEAR_FEATURE:
        case USBF_REQ_SET_FEATURE:
        case USBF_REQ_SET_ADDRESS:
        case USBF_REQ_GET_CONFIGURATION:
        case USBF_REQ_SET_CONFIGURATION:
        case USBF_REQ_GET_INTERFACE:
        case USBF_REQ_SET_INTERFACE:

        /* excluding above */
        default:

            /* request error process */
            stdrequest_request_error();

            break;
    }

    return;
}

/**
* @brief            get standard request process flag
*
* @param            none
* @return           TRUE  : standard request is processing
* @return           FALSE : standard request not processed
*
**/
uint8_t usbfunc_stdrequest_is_processing(void)
{
    /* return standard request process flag */
    return(s_std_proc_flag);
}

/**
* @brief            standard request complete process
*
* @param            none
* @return           none
*
**/
void usbfunc_stdrequest_complete(void)
{
    /*  end standard request process */
    s_std_proc_flag = FALSE;

    return;
}

/**
* @brief            Get_Descriptor request process
*
* @param            setup_pkt    pointer to setup packet
* @return           none
*
**/
static void stdrequest_getdescriptor(USBF_ST_SETUP_INFO* setup_pkt)
{
    uint8_t*  descriptor = NULL;      /* pointer to descriptor  */

    uint16_t  size = 0;               /* size of descriptor     */

    uint8_t   desc_flag = TRUE;       /* descriptor process flag */

    /* descriptor type */
    switch (HIBYTE(HWORD_CPU_TO_LE(setup_pkt->wValue)))
    {
        /* device descriptor */
        case USBF_DEV_DESC_TYPE:

            /* get device descriptor */
            usbfunc_descriptor_get_dev_desc(&descriptor, &size);

            break;

        /* configuration descriptor */
        case USBF_CFG_DESC_TYPE:

            /* get configuration descriptor */
            usbfunc_descriptor_get_cfg_desc(&descriptor, &size);

            break;

        /* string descriptor */
        case USBF_STR_DESC_TYPE:

            /* get string descriptor */
            desc_flag = usbfunc_descriptor_get_str_desc(&descriptor, &size,
                                                        LOBYTE(HWORD_CPU_TO_LE(setup_pkt->wValue)));
            break;

        /* excluding above */
        default:

            /* request not treatable */
            desc_flag = USBFUNC_STD_REQUEST_OTHERS;

            break;
    }

    /* when descriptor process normality */
    if (desc_flag == TRUE)
    {
        /* when descriptor size is larger than the length in setup packet data */
        if (size > HWORD_CPU_TO_LE(setup_pkt -> wLength))
        {
            /* set the length in setup packet data to descriptor size */
            size = HWORD_CPU_TO_LE(setup_pkt -> wLength);
        }

        /* set that standard request is processing */
        s_std_proc_flag = TRUE;

        /* control transfer request */
        usbfunc_ctrltrans_send(descriptor, size);

    /* when descriptor process unnormality */
    }
    else if (desc_flag == FALSE)
    {
        /* request error process */
        stdrequest_request_error();
    }
    /* request not treatable */
    else if (desc_flag == USBFUNC_STD_REQUEST_OTHERS)
    {
        /*** report request (HID eg.) ***/
        /* request receiving notification */
        usbfunc_event_request(setup_pkt);
    }
    /* other */
    else
    {

    }

    return;
}

/**
* @brief            request error process
*
* @param            none
* @return           none
*
**/
static void stdrequest_request_error(void)
{
    /* set endpoint0 stall */
    usbfunc_ctrltrans_setstall();

    return;
}
