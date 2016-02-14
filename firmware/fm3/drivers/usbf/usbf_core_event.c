/**
* @file          usbf_core_event.c
* @brief         USB function event process module
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

#include "usbf.h"
#include "usbf_hal_io.h"
#include "usbf_core_event.h"
#include "usbf_core_control_transfer.h"
#include "usbf_core_stdrequest.h"

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

#define FIRST_SETUP_UNRECEIVED        (0x00)  /* first setup unreceive   */
#define FIRST_SETUP_RECEIVED          (0x01)  /* first setup received    */

/*----------------------------------------------------------------------------*/
/* Static Variable Definition                                                 */
/*----------------------------------------------------------------------------*/

/* callback function */
static USBF_ST_CALLBACK_HANDLER s_callback_handler;

/* first setup receive flag */
static uint8_t s_first_setup_flag;

/* USB function driver last action state */
static uint8_t s_laststatus;

/* USB function driver action state */
static uint8_t s_status;

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/

/* get USB function driver last action state */
static uint8_t get_laststatus(void);

/* set USB function driver action state */
static void set_status(uint8_t status);

/* event notification */
static void event_notify(uint8_t event_type, void* event_info);

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

/**
* @brief            event process module initialize
*
* @param            callback_handler_ptr: pointer to callback handler
* @return           none
*
**/
void usbfunc_event_init(const USBF_ST_CALLBACK_HANDLER * callback_handler_ptr)
{
    /* set unreceive to first setup receive flag */
    s_first_setup_flag = FIRST_SETUP_UNRECEIVED;

    /* callback function registration */

#if (  (USBF_VENDOR_REQUEST            == USBF_ENABLE) \
    || (USBF_CLASS_REQUEST             == USBF_ENABLE) \
    || (USBF_STD_REQUEST_SETDESCRIPTOR == USBF_ENABLE) \
    || (USBF_STD_REQUEST_SYNCHFRAME    == USBF_ENABLE))

    s_callback_handler.RequestHandler =
      callback_handler_ptr->RequestHandler;           /* request receive notification */

#else /* when request function is unused,request set null to receiving notification callback function */

    s_callback_handler.RequestHandler = NULL;         /* request receive notification */

#endif

    s_callback_handler.TransCompleteHandler =
      callback_handler_ptr->TransCompleteHandler;    /* transfer complete notification */

    s_callback_handler.EventHandler =
      callback_handler_ptr->EventHandler;             /* event notification */

    s_callback_handler.ErrorHandler =
      callback_handler_ptr->ErrorHandler;             /* error notification */

    /* set initialized to USB function driver action state */
    set_status(USBF_STATUS_INIT);

    return;
}

/**
* @brief            USB resume process
*
* @param            none
* @return           none
*
**/
void usbfunc_event_resume(void)
{
    /* clear suspend interrupt */
    usbfunc_reg_clear_eventint(REG_SUSPEND_INT);

    /* set normal mode to USB clock */
    usbfunc_reg_set_ustp(FALSE);

    /* enable suspend interrupt */
    usbfunc_reg_enable_event(REG_SUSPEND_INT);

    /* set USB resume instruction */
    usbfunc_reg_set_resume(TRUE);

    /* clear USB resume instruction */
    usbfunc_reg_set_resume(FALSE);

    /* disable wakeup interrupt */
    usbfunc_reg_disable_event(REG_WAKEUP_INT);

    /* set last state to suspend */
    set_status(get_laststatus());

    return;
}

/**
* @brief            connection request process
*
* @param            none
* @return           none
*
**/
void usbfunc_event_turn_on(void)
{
    /* connect to host */
    usbfunc_reg_set_connect(FALSE);

    /* set connected to  USB function driver action */
    set_status(USBF_STATUS_CONNECT);

    return;
}

/**
* @brief            cutting request process
*
* @param            none
* @return           none
*
**/
void usbfunc_event_turn_off(void)
{
    /* stop USB function macro */
    usbfunc_reg_stop();

    /* cut the connection of host */
    usbfunc_reg_set_connect(TRUE);

    /* set uninitialize to USB function driver action */
    set_status(USBF_STATUS_UNINIT);

    return;
}

/**
* @brief            bus reset detection processing
*
* @param            none
* @return           none
*
**/
void usbfunc_event_busreset(void)
{
    uint8_t status;                         /* USB function driver action status */
    USBF_ST_STATUSERROR_INFO error_info;    /* error information */

    /* claer bus reset interrupt */
    usbfunc_reg_clear_eventint(REG_BUSRST_INT);

    /* get USB function driver action state */
    status = usbfunc_event_get_status();

    /* judge USB function driver action state */
    switch(status)
    {
        /* enumeration */
        case USBF_STATUS_ENUM:

        /* configuration */
        case USBF_STATUS_CONFIG:

        /* suspend */
        case USBF_STATUS_SUSPEND:

            /* stop USB function macro */
            usbfunc_reg_stop();

            /* control transfer module initialize */
            usbfunc_ctrltrans_init();

            /* data transfer control module initialize */
            usbfunc_datatrans_init();

            /* USB function macro start */
            usbfunc_reg_start();

            /* set unreceive to first setup receive flag */
            s_first_setup_flag = FIRST_SETUP_UNRECEIVED;

            /* set default to USB function driver action state */
            set_status(USBF_STATUS_DEFAULT);

            /* bus reset notification */
            event_notify(USBF_EVENT_BUSRESET, NULL);

            break;

        /* connection */
        case USBF_STATUS_CONNECT:

            /* set unreceive to first setup receive flag */
            s_first_setup_flag = FIRST_SETUP_UNRECEIVED;

            /* set default to USB function driver action state */
            set_status(USBF_STATUS_DEFAULT);

            /* bus reset notification */
            event_notify(USBF_EVENT_BUSRESET, NULL);

            break;

        /* initialized */
        case USBF_STATUS_INIT:

            /* set error information */
            error_info.UsbStatus = status;
            error_info.UsbEvent = USBF_EVENT_BUSRESET;

            /* error notification */
            usbfunc_event_error(USBF_ERROR_STATUS, (void*)(&error_info));

            break;

        /* uninitialize */
        case USBF_STATUS_UNINIT:

        /* default */
        case USBF_STATUS_DEFAULT:

        /* error */
        case USBF_STATUS_ERROR:

        default:

            break;
    }

    return;
}

/**
* @brief            SOF receiving detection process
*
* @param            none
* @return           none
*
**/
#if (USBF_SOF_ENABLE == USBF_ENABLE)
void usbfunc_event_sof(void)
{
    uint16_t sof_num;             /* SOF pakect receive frame number */

    /* clear SOF receiving interrupt */
    usbfunc_reg_clear_eventint(REG_SOF_INT);

    /* get SOF packet receiving frame number */
    sof_num = usbfunc_reg_get_sofnum();

    /* SOF receiving detection notification */
    event_notify(USBF_EVENT_SOF, (void*)(&sof_num));

    return;
}
#endif

/**
* @brief            configuration detection process
*
* @param            none
* @return           none
*
**/
void usbfunc_event_config(void)
{
    uint8_t status;                         /* USB function driver action status */
    uint8_t conf_num;                       /* configuration number */
    USBF_ST_STATUSERROR_INFO error_info;    /* error info */

    /* clear configuration interrupt  */
    usbfunc_reg_clear_eventint(REG_CONFIG_INT);

    /* get USB function driver action state */
    status = usbfunc_event_get_status();

    /* judge USB function driver action state */
    switch(status)
    {
        /* enumeration */
        case USBF_STATUS_ENUM:

            /* get configuration number */
            conf_num = usbfunc_reg_get_confnum();

            if (conf_num != REG_CONF_NUM_0)
            {
                /* set USB function driver action state to configuration */
                set_status(USBF_STATUS_CONFIG);

                /* configuration interrupt notification */
                event_notify(USBF_EVENT_SETCONFIG, (void*)(&conf_num));
            }
            break;

        /* configuration */
        case USBF_STATUS_CONFIG:
            /* get configuration number */
            conf_num = usbfunc_reg_get_confnum();

            if (conf_num != REG_CONF_NUM_0)
            {
                /* set error information */
                error_info.UsbStatus = status;
                error_info.UsbEvent = USBF_EVENT_SETCONFIG;

                /* error notification */
                usbfunc_event_error(USBF_ERROR_STATUS, (void*)(&error_info));
            }
            else
            {
                /* control transfer module initialize */
                usbfunc_ctrltrans_init();

                /* data transfer control module initialize */
                usbfunc_datatrans_init();

                /* set USB function driver action state to enumeration */
                set_status(USBF_STATUS_ENUM);

                /* configuration interrupt notification */
                event_notify(USBF_EVENT_SETCONFIG, (void*)(&conf_num));
            }
            break;

        /* initialized */
        case USBF_STATUS_INIT:

        /* conection */
        case USBF_STATUS_CONNECT:

        /* default */
        case USBF_STATUS_DEFAULT:

            /* set error information */
            error_info.UsbStatus = status;
            error_info.UsbEvent = USBF_EVENT_SETCONFIG;

            /* error notification */
            usbfunc_event_error(USBF_ERROR_STATUS, (void*)(&error_info));
            break;

        /* unitialize */
        case USBF_STATUS_UNINIT:

        /* suspenf */
        case USBF_STATUS_SUSPEND:

        /* error */
        case USBF_STATUS_ERROR:

        default:
            break;
        }

    return;

}

/**
* @brief            setup detection event process
*
* @param            none
* @return           none
*
**/
void usbfunc_event_setup (void)
{
    uint8_t status;                         /* USB function driver action status */
    USBF_ST_STATUSERROR_INFO error_info;    /* error info */

    /* get USB function driver action state */
    status = usbfunc_event_get_status();

    /* judge USB function driver action state */
    switch(status)
    {
        /* default */
        case USBF_STATUS_DEFAULT:

            /* judge first time setup receive flag */
            if (s_first_setup_flag == FIRST_SETUP_UNRECEIVED)
            {
                /* set USB function driver action state to enumeration */
                set_status(USBF_STATUS_ENUM);

                /* set received to first setup receive flag */
                s_first_setup_flag = FIRST_SETUP_RECEIVED;
            }

            break;

        /* initialized */
        case USBF_STATUS_INIT:

        /* conection */
        case USBF_STATUS_CONNECT:

            /* set error information */
            error_info.UsbStatus = status;
            error_info.UsbEvent = USBF_EVENT_SETUP;

            /* error notification */
            usbfunc_event_error(USBF_ERROR_STATUS, (void*)(&error_info));

            break;

        /* unitialize */
        case USBF_STATUS_UNINIT:

        /* configuration */
        case USBF_STATUS_CONFIG:

        /* enumeration */
        case USBF_STATUS_ENUM:

        /* suspend */
        case USBF_STATUS_SUSPEND:

        /* error */
        case USBF_STATUS_ERROR:

        default:

            break;
        }

    return;
}

/**
* @brief            suspend detection process
*
* @param            none
* @return           none
*
**/
void usbfunc_event_suspend(void)
{
    uint8_t status;                         /* USB function driver action status */

    USBF_ST_STATUSERROR_INFO error_info;    /* error info */

    /* clear suspend interrupt */
    usbfunc_reg_clear_eventint(REG_SUSPEND_INT);

    /* get USB function driver action state */
    status = usbfunc_event_get_status();

    /* judge USB function driver action state */
    switch(status)
    {
        /* default */
        case USBF_STATUS_DEFAULT:

        /* configuration */
        case USBF_STATUS_CONFIG:

        /* enumeration */
        case USBF_STATUS_ENUM:

        /* connection */
        case USBF_STATUS_CONNECT:

            /* set USB function driver action state to suspend */
            set_status(USBF_STATUS_SUSPEND);

            /* enable wakeup interrupt*/
            usbfunc_reg_enable_event(REG_WAKEUP_INT);

            /* disable suspend interrupt */
            usbfunc_reg_disable_event(REG_SUSPEND_INT);

            /* set stop to USB clock */
            usbfunc_reg_set_ustp(TRUE);

            /* suspend detection notification */
            event_notify(USBF_EVENT_SUSPEND, NULL);

            break;

        /* initialized */
        case USBF_STATUS_INIT:

        /* suspend */
        case USBF_STATUS_SUSPEND:

            /* set error information */
            error_info.UsbStatus = status;
            error_info.UsbEvent = USBF_EVENT_SUSPEND;

            /* error notification */
            usbfunc_event_error(USBF_ERROR_STATUS, (void*)(&error_info));

            break;

        /* unitialize */
        case USBF_STATUS_UNINIT:

        /* error */
        case USBF_STATUS_ERROR:

        default:

            break;
        }

    return;
}

/**
* @brief            wakeup detection process
*
* @param            none
* @return           none
*
**/
void usbfunc_event_wakeup(void)
{
    uint8_t last_status;                     /* USB function driver last action status */

    /* get USB function driver last action state */
    last_status = get_laststatus();

    /* set last state to USB function driver action state */
    set_status(last_status);

    /* clear suspend interrupt */
    usbfunc_reg_clear_eventint(REG_SUSPEND_INT);

    /* set normal mode to USB clock */
    usbfunc_reg_set_ustp(FALSE);

    /* enable suspend interrupt */
    usbfunc_reg_enable_event(REG_SUSPEND_INT);

    /* disable wakeup interrupt */
    usbfunc_reg_disable_event(REG_WAKEUP_INT);

    /* clear wakeup interrupt */
    usbfunc_reg_clear_eventint(REG_WAKEUP_INT);

    /* wakeup detection notification */
    event_notify(USBF_EVENT_HOSTWAKEUP, NULL);

    return;
}

/**
* @brief            request receiving notification
*
* @param            setup :  setup packet
* @return           none
*
**/
void usbfunc_event_request(USBF_ST_SETUP_INFO * setup)
{
    /* check function address */
    if (s_callback_handler.RequestHandler != NULL)
    {
        /* request receiving notification callback function */
        s_callback_handler.RequestHandler(setup);
    }

    return;
}

/**
* @brief            transfer complete callback function
*
* @param            ep_id           : specified endpoint number
* @param            count           : transmitted data numbers
* @param            complete_status : transfer complete state
* @return           none
*
**/
void usbfunc_event_trans_complete(uint8_t ep_id, uint16_t count, uint8_t complete_status)
{
    /* when descriptor process is in doing */
    if (usbfunc_stdrequest_is_processing() == TRUE)
    {
      /* descriptor complete process */
        usbfunc_stdrequest_complete();
    }
    /* when descriptor process is not in doing */
    else
    {
        /* check function address */
        if (s_callback_handler.TransCompleteHandler != NULL)
        {
            /* transfer end callback function */
            s_callback_handler.TransCompleteHandler(ep_id, count, complete_status);
        }
    }

    return;
}

/**
* @brief            transfer complete callback function
*
* @param            error_type :  error type
* @param            error_info :  error information
* @return           none
*
**/
void usbfunc_event_error(uint8_t error_type, void* error_info)
{
    /* check function address */
    if (s_callback_handler.ErrorHandler != NULL)
    {
        /* judge error type */
        switch(error_type)
        {
            /* setup paket analysis error */
            case USBF_ERROR_SETUP:

            /* USB function drive action state transmit error */
            case USBF_ERROR_STATUS:

                /* set error to USB function driver action state*/
                set_status(USBF_STATUS_ERROR);

                /* call error notify function */
                s_callback_handler.ErrorHandler(error_type, error_info);
                break;

            default:

                break;
        }
    }

    return;
}

/**
* @brief            get USB function driver action state
*
* @param            none
* @return           USB function driver action state
*
**/
uint8_t usbfunc_event_get_status(void)
{
    /* return USB function driver action state */
    return (s_status);
}

/**
* @brief            set USB function driver action state
*
* @param            status     USB function driver action state
* @return           none
*
**/
static void set_status(uint8_t status)
{
    /* update last USB function driver action state */
    s_laststatus = s_status;

    /* set specified to USB function driver action state */
    s_status = status;

    return;
}

/**
* @brief            get last USB function driver action state
*
* @param            none
* @return           last USB function driver action state
*
**/
static uint8_t get_laststatus(void)
{
    /* return last USB function driver action state */
    return (s_laststatus);
}

/**
* @brief            event notification callback function
*
* @param            event_type :  event type
* @param            event_info :  event detail info
* @return           none
*
**/
static void event_notify(uint8_t event_type, void* event_info)
{
    /* check function address */
    if (s_callback_handler.EventHandler != NULL)
    {
        /* call USB event receiving notification callback function */
        s_callback_handler.EventHandler(event_type, event_info);
    }

    return;
}
