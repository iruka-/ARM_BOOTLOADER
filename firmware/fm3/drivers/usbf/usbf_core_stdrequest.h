/**
* @file          usbf_core_stdrequest.h
* @brief         standard request control module
* @author        lijiale
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __USBF_STDREQUEST_INC__
#define __USBF_STDREQUEST_INC__

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
/* Type Definition                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/

/* standard request process */
void usbfunc_stdrequest_process(USBF_ST_SETUP_INFO* setup_pkt);

/* get standard request process flag */
uint8_t usbfunc_stdrequest_is_processing(void);

/* standard request complete process */
void usbfunc_stdrequest_complete(void);

#endif
