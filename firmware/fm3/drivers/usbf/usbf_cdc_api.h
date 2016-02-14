/**
* @file          usbf_msc_api.h
* @brief         API module
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __USBF_MSC_API_INC__
#define __USBF_MSC_API_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Constant Definition                                                 */
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
/* information about LUN of media drive getting callback funtion used in this drive */
extern uint8_t usbf_msc_api_media_getluninfo(uint8_t lun, USBF_MSC_ST_LUN_DATA *lun_data_ptr);

/* media drive reading control callback function used in this drive */
extern uint8_t usbf_msc_api_media_read(uint8_t lun, uint32_t lba, const uint8_t* data_buffer_ptr);

/* media drive writting control callback function used in this drive */
extern uint8_t usbf_msc_api_media_write(uint8_t lun, uint32_t lba, const uint8_t* data_buffer_ptr);

/* event of media drive notification callback funtion used in this drive */
extern uint8_t usbf_msc_api_media_event(uint8_t lun, uint8_t event_type, void* event_info);

#endif   /* __USBF_MSC_API_INC__ */
