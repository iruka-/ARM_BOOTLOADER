/**
* @file          usbf_core_event.h
* @brief         low layer USB function control module
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __USBF_EVENT_INC__
#define __USBF_EVENT_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variable Definition                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/

/* event process module initialize */
extern void usbfunc_event_init(const USBF_ST_CALLBACK_HANDLER * callback_handler_ptr);

/* connection request process */
extern void usbfunc_event_turn_on(void);

/* cutting request process */
extern void usbfunc_event_turn_off(void);

/* bus reset detection processing */
extern void usbfunc_event_busreset(void);

#if (USBF_SOF_ENABLE == USBF_ENABLE)
/* SOF receiving detection process */
extern void usbfunc_event_sof(void);
#endif

/* configuration detection process */
extern void usbfunc_event_config(void);

/* setup detection event process */
extern void usbfunc_event_setup(void);

/* suspend detection process */
extern void usbfunc_event_suspend(void);

/* wakeup detection process */
extern void usbfunc_event_wakeup(void);

/* get USB function driver action state */
extern uint8_t usbfunc_event_get_status(void);

/* USB resume process */
extern void usbfunc_event_resume(void);

/* request receiving notification */
extern void usbfunc_event_request (USBF_ST_SETUP_INFO * setup);

/* transfer complete notification */
extern void usbfunc_event_trans_complete (uint8_t ep_id, uint16_t count, uint8_t complete_status);

/* error notification */
extern void usbfunc_event_error (uint8_t error_type, void * error_info);

#endif
