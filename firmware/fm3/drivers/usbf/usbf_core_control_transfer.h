/**
* @file          usbf_core_control_transfer.h
* @brief         control transfer control module
* @author        lijiale
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __USBF_CTRLTRANS_INC__
#define __USBF_CTRLTRANS_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

/* endpoint0 interrupt type */
#define CTRLTRANS_SETUP_DETECTED   (0)   /* setup stage detection    */
#define CTRLTRANS_DATA_RECEIVED    (1)   /* data receiving           */
#define CTRLTRANS_DATA_SENDED      (2)   /* data sending             */

/* control transfer stage */
#define CTRLTRANS_STAGE_NONE       (0)   /* no stage                 */
#define CTRLTRANS_STAGE_SETUP      (1)   /* setup stage              */
#define CTRLTRANS_STAGE_DATAIN     (2)   /* data in stage            */
#define CTRLTRANS_STAGE_DATAOUT    (3)   /* data out stage           */
#define CTRLTRANS_STAGE_STATUSIN   (4)   /* status in stage          */
#define CTRLTRANS_STAGE_STATUSOUT  (5)   /* status out stage         */

/*----------------------------------------------------------------------------*/
/* Global Variable Definition                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Type Definition                                                            */
/*----------------------------------------------------------------------------*/

/* endpoint0 attribute */
typedef struct _USBFUNC_ST_CTRLTRANS_EPINFO{
    uint8_t    pkt_size;           /* packet size                        */
    uint8_t    trans_status;       /* transfer state                     */
    uint8_t    trans_stage;        /* transfer stage                     */
    uint8_t    spk_sended_flag;    /* SPK sended flag                    */
    uint8_t*   trans_data_buffer;  /* transfer data storage              */
    uint16_t   trans_data_size;    /* transfer data size                 */
    uint16_t   transferred_count;  /* transmitted data numbers           */
}USBFUNC_ST_CTRLTRANS_EPINFO;

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/

/* register endpoint0 attribute */
void usbfunc_ctrltrans_entry_ep0info(uint8_t ep0_pkt_size);

/* control transfer module initialize */
void usbfunc_ctrltrans_init(void);

/* control transfer sending request */
void usbfunc_ctrltrans_send(uint8_t * buffer, uint16_t size);

/* control transfer receiving request */
void usbfunc_ctrltrans_receive(uint8_t * buffer, uint16_t size);

/* stop control transfer */
void usbfunc_ctrltrans_abort(void);

/* control transfer receiving and sending process */
void usbfunc_ctrltrans_process(uint8_t type);

/* set endpoint0 stall */
void usbfunc_ctrltrans_setstall(void);

/* clear endpoint0 stall */
void usbfunc_ctrltrans_clearstall(void);

/* get control transfer state  */
uint8_t usbfunc_ctrltrans_getstatus(void);

/* get endpoint0 attribute */
void usbfunc_ctrltrans_get_ep0info(USBFUNC_ST_CTRLTRANS_EPINFO* ep0_info_ptr);

/* SPK receiving process */
void usbfunc_ctrltrans_spk_process(void);

#endif
