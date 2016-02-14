/**
* @file          usbfunc_demoapp.c
* @brief         usbfunc sample application main module
* @author        huangshan
* @version       1.0
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

/*----------------------------------------------------------------------------*/
/*  Headers                                                                   */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "cpu_config.h"
#include "hw_support.h"
#include "usbf.h"
#include "usbf_cdc.h"
#include "timer.h"
#include "led.h"
#include "joystick.h"
#include "usbfunc_demoapp_config.h"
#include "usbfunc_demoapp_descriptor.h"
#include "dbg_print.h"
#include "callback.h"
#include "debug.h"
#include "monitor/usbio.h"
#include "monitor/gpio.h"

/*----------------------------------------------------------------------------*/
/*  Global Variable Definition                                                */
/*----------------------------------------------------------------------------*/
extern USBF_ST_USB_DESCRIPTOR g_UsbDescriptor;

/*----------------------------------------------------------------------------*/
/*  Constant Definition                                                       */
/*----------------------------------------------------------------------------*/


#define	LED_TEST				   (0)
#define	USE_PWM_LED				   (0)

/* max scanning times for usb vbus detect */
#define  VBUS_SCAN_MAX_TIMES       (5)

/* request type */
#define USB_REQUEST_TYPE_STANDARD  (0)  /* stantard request */
#define USB_REQUEST_TYPE_CLASS     (1)  /* class request    */
#define USB_REQUEST_TYPE_VENDOR    (2)  /* vendor request   */

#define JOYSTICK_BUTTON_NUM   (1U)    /* number of valid joystick button */
                                      /* center valid */
                                      /* up/down/left/right invalid         */

/* demo application status */
enum
{
    DEMOAPP_STATUS_INIT,     /* init status */
    DEMOAPP_STATUS_ACTIVE,   /* active status */
    DEMOAPP_STATUS_SUSPEND   /* suspend status */
};

/* size of transfer buffer for MSC driver setting */
#define MAX_BLOCK_SIZE           (512U)

/*----------------------------------------------------------------------------*/
/*  Static Variable Definition                                                */
/*----------------------------------------------------------------------------*/
/* address of end point attribute buffer */
static uint8_t s_EpBuffer[(USED_EPNUM) * (USBF_EPINFO_SIZE)];

/* VBUS status */
static uint8_t s_UsbVbusStatus;

/* demo application status */
volatile static uint8_t s_DemoappStatus;

/* each joystick button pressed status */
//static uint8_t s_IsJoystickButtonPressed[JOYSTICK_BUTTON_NUM];

/* status of each joystick button to indicator button is detected or not */
//static uint8_t s_IsJoystickButtonDetected[JOYSTICK_BUTTON_NUM];

/*----------------------------------------------------------------------------*/
/*  Local Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/
/* usb function sample application initialize */
static void usbfunc_demoapp_init(void);

/* usb function sample application reset */
static void usbfunc_demoapp_reset(void);

/* USB VBUS status detect process */
static void usbfunc_demoapp_usb_vbus_detect(void);

/* process to do detect each button press of joystick */
//static void usbfunc_demoapp_joystick_detect(void);

/* process when button of joystick has been detected */
//static void usbfunc_demoapp_joystick_process(void);

/* process for PSW2 button is detected */
//static void usbfunc_demoapp_joystick_psw2_process(void);

/* callback function for USB request */
static void usbfunc_demoapp_usb_request_callback(USBF_ST_SETUP_INFO *pSetup);

/* callback function for USB  transfer completion */
static void usbfunc_demoapp_usb_transcomplete_callback(uint8_t EpId, uint16_t Count, uint8_t CompleteStatus);

/* callback function for USB event notification */
static void usbfunc_demoapp_usb_event_callback(uint8_t EventType, void* pEventInfo);

/* callback function for USB error notification */
static void usbfunc_demoapp_usb_error_callback(uint8_t ErrorType, void* pErrorInfo);

/* USB function driver initialize */
static void usbfunc_demoapp_usb_init(void);

/*----------------------------------------------------------------------------*/
/*  Functions                                                                 */
/*----------------------------------------------------------------------------*/
void JTAG_disable(void)
{
	int pin;
	for(pin=P00;pin<P05;pin++) {
		pinModeEx(pin, 0);	// 0=GPIO , 1=Function
		pinMode(pin, 0);	// 0=input, 1=output
	}

#if	1
	bFM3_GPIO_EPFR00_JTAGEN0B=0;	//!< TCK/TMS/TDOを使用しない.(->gpio)
	bFM3_GPIO_EPFR00_JTAGEN1S=0;	//!< TRSTX/TDIを使用しない.  (->gpio)
//	bFM3_GPIO_EPFR00_TRC1E=0;		//!< TRCED2/TRCED3を使用しない. (->gpio)
//	bFM3_GPIO_EPFR00_TRC0E=0;		//!< TRCED0/TRCED1/TRACECLKを使用しない. (->gpio)
#endif
}

volatile unsigned int *GPIO_PFR_BASE  ;
volatile unsigned int *GPIO_PCR_BASE  ;
volatile unsigned int *GPIO_DDR_BASE  ;
volatile unsigned int *GPIO_PDIR_BASE ;
volatile unsigned int *GPIO_PDOR_BASE ;

/**
* @brief             main process
* @param             none
* @return            none
*
**/
void usbfunc_demoapp_main(void)
{
#if	_ROMADRS	// 4000番地のファーム限定.
#if	LED_TEST

//	どうしても動かないときはここで試す。

//GPIO_PFR_BASE  = &bFM3_GPIO_PFR0_P0 ;//機能設定(GPIO=0,その他=1)
//GPIO_PCR_BASE  = &bFM3_GPIO_PCR0_P0 ;//PullUP (1=プルアップ)
//GPIO_DDR_BASE  = &bFM3_GPIO_DDR0_P0 ;//方向(入力=0 , 出力=1)
//GPIO_PDIR_BASE = &bFM3_GPIO_PDIR0_P0;//端子入力データ.
//GPIO_PDOR_BASE = &bFM3_GPIO_PDOR0_P0;//端子出力データ.

    led_init();
	while(1) {
		led_on();
		wait_ms(200);
		led_off();
		wait_ms(200);
	}
#endif
#endif

    /* mb9b500 startkit system boot */
	mb9b500_startkit_boot();

    /* timer module initialize */
    TIMER_Init();

    /* LED module initialize */
    led_init();

	USBIOinit();		//!< FIFOバッファの初期化.

#if	USE_PWM_LED			// LED出力をPWM駆動する.(矩形波源として)
	PWM_init();
#endif

    /* usb function sample application initialize */
    usbfunc_demoapp_init();

    /* USB function driver initialize */
    bFM3_GPIO_SPSR_USB0C = 1;

    usbfunc_demoapp_usb_init();

    /* USB function MSC driver initialize */
    //usbfunc_demoapp_msc_init();

    /* start ch.0 reload timer */
    if (TIMER_Start(TIMER_CH0) != TIMER_RET_OK)
    {
        DbgPrintMsg("start ch.0 reload timer error\n");
    }

    /* interrupt enable */
    __enable_irq();

	int led1=0;
    /* infinite loop */
	while (1) {
		/* USB VBUS status deteted process */
		usbfunc_demoapp_usb_vbus_detect();
        if(s_DemoappStatus == DEMOAPP_STATUS_ACTIVE) {
			/* active status */
			if (USBF_GetStatus() == USBF_STATUS_CONFIG) {
				led_on();
				USB_loop();
			}else{
				led_blink(15);
			}
        }else{
			led_blink(14);
        }
    }
}

/**
* @brief            usb function sample application initialize
*
* @param            none
* @return           none
*
**/
static void usbfunc_demoapp_init(void)
{
    /* demo application status initialize */
    s_DemoappStatus = DEMOAPP_STATUS_INIT;

    /* VBUS status initialize */
    s_UsbVbusStatus = USBF_VBUS_NOTCONNECT;

    return;
}

/**
* @brief             usb function sample application reset
* @param             none
* @return            none
*
**/
static void usbfunc_demoapp_reset(void)
{
    /* disconnect USB function driver */
    if (USBF_UnAttach() != USBF_RET_OK)
    {
        DbgPrintMsg("USBF_UnAttach error\n");
    }

    /* usb function sample application initialize */
    usbfunc_demoapp_init();

    /* USB function driver initialize */
    usbfunc_demoapp_usb_init();

    /* USB function MSC driver initialize */
    //usbfunc_demoapp_msc_init();

    return;
}


/**
* @brief             USB SOF EVENT process
* @param             none
* @return            none
*
**/
static void usbfunc_demoapp_sof(void)
{
	if (USBF_GetStatus() == USBF_STATUS_CONFIG) {
//		led_flip(0x01,10);	// 1mS --> 1S
		UsbReadKick();
	}
}

/**
* @brief             USB VBUS status detect process
* @param             none
* @return            none
*
**/
static void usbfunc_demoapp_usb_vbus_detect(void)
{
    uint8_t vbus_new_status;           /* VBUS new status */
    static uint8_t scan_times = 0;     /* scanning times */

    /* get VBUS status */
    vbus_new_status = USBF_GetVbusStatus ();

    /* if under connect status */
    if (s_UsbVbusStatus == USBF_VBUS_CONNECT)
    {
        /* disconnect event occured */
        if (vbus_new_status == USBF_VBUS_NOTCONNECT)
        {
            scan_times++;
        }
        /* connect event occured */
        else
        {
            scan_times = 0;
        }

        /* reached max scan times */
        if (scan_times >= VBUS_SCAN_MAX_TIMES)
        {
            /* clear scanning times */
            scan_times = 0;
            /* update VBUS status */
            s_UsbVbusStatus = USBF_VBUS_NOTCONNECT;

            /* usb function sample application reset */
            usbfunc_demoapp_reset();
        }
    }
    /* if under disconnect status */
    else
    {
        /* connect event occured */
        if (vbus_new_status == USBF_VBUS_CONNECT)
        {
            scan_times++;
        }
        /* disconnect event occured */
        else
        {
            scan_times = 0;
        }

        /* reached max scan times */
        if (scan_times >= VBUS_SCAN_MAX_TIMES)
        {
            /* clear scanning times */
            scan_times = 0;
            /* update VBUS status */
            s_UsbVbusStatus = USBF_VBUS_CONNECT;
        }
    }

    return;
}

#if	0
/**
* @brief            process to do detect each button press of joystick
*
* @param            none
* @return           none
*
**/
static void usbfunc_demoapp_joystick_detect(void)
{
    uint8_t joystick_status;     /* joystick status */
    uint8_t i;                   /* loop count      */

    /* to do detect each button press of joystick */
    for (i = 0; i < JOYSTICK_BUTTON_NUM; i++)
    {
        /* get status of button from joytick */
        (void)JOYSTICK_GetStatus(i, &joystick_status);

        /* first step: if a button is not pressed and push status is getted, */
        /*             then set the button pressed */
        if ((s_IsJoystickButtonPressed[i] == FALSE) && (joystick_status == JOYSTICK_PUSH))
        {
            s_IsJoystickButtonPressed[i] = TRUE;
        }

        /* second step: if a button is pressed and not push status is getted, */
        /*              then set the button not pressed and the button is detected */
        else if ((s_IsJoystickButtonPressed[i] == TRUE) && (joystick_status == JOYSTICK_NO_PUSH))
        {
            s_IsJoystickButtonPressed[i] = FALSE;

            s_IsJoystickButtonDetected[i] = TRUE;
        }
        else
        {
            ;
        }
    }

    return;
}

/**
* @brief            process when button of joystick has been detected
*
* @param            none
* @return           none
*
**/
static void usbfunc_demoapp_joystick_process(void)
{
    /* if center button of joystick has been detected, then do process for center button */
    if (s_IsJoystickButtonDetected[JOYSTICK_BUTTON_PSW2] == TRUE)
    {
        usbfunc_demoapp_joystick_psw2_process();
        /* clear detected flag for psw2 button */
        s_IsJoystickButtonDetected[JOYSTICK_BUTTON_PSW2] = FALSE;
    }

    return;
}

/**
* @brief            process for PSW2 button is detected
*
* @param            none
* @return           none
*
**/
static void usbfunc_demoapp_joystick_psw2_process(void)
{
    /* remote wakeup */
    if (USBF_Resume() != USBF_RET_OK)
    {
        DbgPrintMsg("remote wakeup error\n");
    }
}
#endif

/**
* @brief             callback function for USB request
* @param             pSetup : pointer to information of setup packet
* @return            none
*
**/
static void usbfunc_demoapp_usb_request_callback(USBF_ST_SETUP_INFO *pSetup)
{
    /* judge request type of setup information */
    switch (pSetup->bmRequestType.bits.type)
    {
        /* class request */
        case USB_REQUEST_TYPE_CLASS:
            /* process CDC class request */
            (void)USBF_CDC_Request(pSetup);
            break;

        case USB_REQUEST_TYPE_STANDARD:
            (void)USBF_SetStall(0, USBF_SET_STALL);
            break;

        case USB_REQUEST_TYPE_VENDOR:
        default:
            break;
    }

    return;
}

/**
* @brief             callback function for USB  transfer completion
* @param             EpId           : specific endpoint number
* @param             Count          : number of transfered data
* @param             CompleteStatus : status of transfer completion
* @return            none
*
**/
static void usbfunc_demoapp_usb_transcomplete_callback(uint8_t EpId, uint16_t Count, uint8_t CompleteStatus)
{
	int rc;
    if ( (rc=USBF_CDC_TransComplete(EpId, Count, CompleteStatus)) != USBF_MSC_RET_OK)
    {
        DbgPrintMsg("CDC driver data transfer completion notification error\n");
//		hex_print("rc",rc);
    }
    return;
}

/**
* @brief             callback function for USB event notification
* @param             EventType  : event type
* @param             pEventInfo : pointer to event information
* @return            none
*
**/
static void usbfunc_demoapp_usb_event_callback(uint8_t EventType, void* pEventInfo)
{
    uint8_t  config_number;   /* configuration number            */

    switch (EventType)
    {
        /* is event type suspend? */
        case USBF_EVENT_SUSPEND:
            /* set demo application status to config status */
            s_DemoappStatus = DEMOAPP_STATUS_SUSPEND;
            break;

        /* is event type host wakeup? */
        case USBF_EVENT_HOSTWAKEUP:
            /* set demo application status to active status */
            s_DemoappStatus = DEMOAPP_STATUS_ACTIVE;
            break;

        /* is event type set config? */
        case USBF_EVENT_SETCONFIG:
            /* get configuration number */
            config_number = *(uint8_t *)pEventInfo;

            /* is configuraion number cleared? */
            if (config_number  == 0)
            {
                /* set demo application status to initial status */
                s_DemoappStatus = DEMOAPP_STATUS_INIT;

                /* USB function MSC driver initialize */
                //usbfunc_demoapp_msc_init();
            }

            /* is configuraion number set? */
            if (config_number  == 1)
            {
                /* set demo application status to active status */
                s_DemoappStatus = DEMOAPP_STATUS_ACTIVE;
            }
            break;

        /* is event type bus reset? */
        case USBF_EVENT_BUSRESET:
            /* USB function MSC driver initialize */
            //usbfunc_demoapp_msc_init();
            break;
        case USBF_EVENT_SOF:
            usbfunc_demoapp_sof();
			break;
        default:
            break;
    }

    return;
}

#if	0
void recStatus(int r)
{
	static int old=-1;
	if(	old != r) {
		old = r;
//		hex_print("Stat",r);
	}
}
#endif



/**
* @brief             callback function for USB error notification
* @param             ErrorType : error type
* @param             pErrorInfo : pointer to error information
* @return            none
*
**/
static void usbfunc_demoapp_usb_error_callback(uint8_t ErrorType, void* pErrorInfo)
{
    DbgPrintValue("usb function error callback: %d\n", ErrorType);

    return;
}

/**
* @brief            USB function driver initialize
*
* @param            none
* @return           none
*
**/
static void usbfunc_demoapp_usb_init(void)
{
    /* callback function for USB function driver  */
    USBF_ST_CALLBACK_HANDLER     callback_handler;

    /* size of end point attribute buffer */
    uint16_t ep_buffer_size;

    /* set size of end point attribute buffer */
    ep_buffer_size = USBF_EPINFO_SIZE * (USED_EPNUM);

    /* set USB function driver callback */
    callback_handler.RequestHandler = usbfunc_demoapp_usb_request_callback;
    callback_handler.TransCompleteHandler = usbfunc_demoapp_usb_transcomplete_callback;
    callback_handler.EventHandler = usbfunc_demoapp_usb_event_callback;
    callback_handler.ErrorHandler = usbfunc_demoapp_usb_error_callback;

    /* initialize USB function driver */
    if (USBF_Init(&g_UsbDescriptor, s_EpBuffer, ep_buffer_size, &callback_handler) != USBF_RET_OK)
    {
        DbgPrintMsg("USBF_Init error\n");
    }

    /* wait until vbus is connected */
    while (s_UsbVbusStatus == USBF_VBUS_NOTCONNECT)
    {
        usbfunc_demoapp_usb_vbus_detect();
    }

    /* connect USB function driver */
    if (USBF_Attach() != USBF_RET_OK)
    {
        DbgPrintMsg("USBF_Attach error\n");
    }

    return ;
}

