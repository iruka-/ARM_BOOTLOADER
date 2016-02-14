/**
* @file          usbf_core_data_transfer.h
* @brief         data transfer control module
* @author        lijiale
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __USBF_DATATRANS_INC__
#define __USBF_DATATRANS_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

#define DATATRANS_UNUSED_EPID     (0xFF)  /* unused endpoint                  */
#define DATATRANS_INVALID_INDEX   (0xFF)  /* invalid endpoint index           */

/* data transfer stage */
#define DATATRANS_STAGE_NONE      (0)     /* no stage                         */
#define DATATRANS_STAGE_DATAIN    (1)     /* data in stage                    */
#define DATATRANS_STAGE_DATAOUT   (2)     /* data out stage                   */

/*----------------------------------------------------------------------------*/
/* Global Variable Definition                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Type Definition                                                            */
/*----------------------------------------------------------------------------*/

/* endpoint attribute */
typedef struct _USBFUNC_ST_DATATRANS_EPINFO{
    uint8_t    ep_id;              /* endpoint number                */
    uint8_t    ep_type;            /* endpoint transfer type         */
    uint16_t   ep_pkt_size;        /* endpoint packet size           */
    uint8_t    ep_direction;       /* endpoint transfer direction    */
    uint8_t    trans_status;       /* transfer state                 */
    uint8_t    trans_stage;        /* transfer stage                 */
    uint8_t    spk_sended_flag;    /* SPK sended flag                */
    uint8_t*   trans_data_buffer;  /* transfer data storage          */
    uint16_t   trans_data_size;    /* transfer data size             */
    uint16_t   transferred_count;  /* transmitted data numbers       */
}USBFUNC_ST_DATATRANS_EPINFO;

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/

/* register endpoint attribute  */
void usbfunc_datatrans_entry_epinfo(uint8_t ep_number, USBFUNC_ST_DATATRANS_EPINFO* ep_info_ptr);

/* data transfer control module initialize */
void usbfunc_datatrans_init(void);

/* data transfer sending request */
void usbfunc_datatrans_send(uint8_t ep_id, uint8_t * buffer, uint16_t size);

/* data transfer receiving request */
void usbfunc_datatrans_receive(uint8_t ep_id, uint8_t * buffer, uint16_t size);

/* stop data transfer */
void usbfunc_datatrans_abort(uint8_t ep_id);

/* data transfer receiving and sending process */
void usbfunc_datatrans_process(uint8_t ep_id);

/* set endpoint stall */
void usbfunc_datatrans_setstall(uint8_t ep_id);

/* clear endpoint stall */
void usbfunc_datatrans_clearstall(uint8_t ep_id);

/* get data transfer state */
uint8_t usbfunc_datatrans_getstatus(uint8_t ep_id);

/* get endpoint attribute */
uint8_t usbfunc_datatrans_get_epinfo(uint8_t ep_id, USBFUNC_ST_DATATRANS_EPINFO* ep_info_ptr);

/* get endpoint number */
uint8_t usbfunc_datatrans_get_epid(uint8_t ep_index);

/* get the endpoint numbers which in using */
uint8_t usbfunc_datatrans_get_epnum(void);

/* SPK receiving process */
void usbfunc_datatrans_spk_process(uint8_t ep_id);

#if (USBF_USE_DMA == USBF_ENABLE)  /* use DMA */

/* data transfer DMA receiving and sending process */
void usbfunc_datatrans_dma_process(uint8_t ep_id);

#endif /* USBF_USE_DMA == USBF_ENABLE */

#endif
