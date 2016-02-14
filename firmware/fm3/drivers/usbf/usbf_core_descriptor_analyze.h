/**
* @file          usbf_core_descriptor_analyze.h
* @brief         USB descriptor analysis module
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __USBF_DESCRIPTOR_ANAYZE_INC__
#define __USBF_DESCRIPTOR_ANAYZE_INC__


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

/* check descriptor information */
extern uint8_t usbfunc_descriptor_judge_desc(const USBF_ST_USB_DESCRIPTOR * usb_descriptor,
                                             uint8_t * ep_buffer,
                                             uint16_t ep_buffer_size);

/* get descriptor information */
extern void usbfunc_descriptor_get_dev_desc(uint8_t** dev_desc_ptr, uint16_t* dev_desc_size);

/* get configuration descriptor information */
extern void usbfunc_descriptor_get_cfg_desc(uint8_t** cfg_desc_ptr, uint16_t* cfg_desc_size);

/* get specified string descriptor information */
extern uint8_t usbfunc_descriptor_get_str_desc(uint8_t** str_desc_ptr,
                                               uint16_t* str_desc_size,
                                               uint8_t str_desc_index);

/* get remote wakeup valid flag */
extern uint8_t usbfunc_descriptor_get_remotewakeup(void);

#endif
