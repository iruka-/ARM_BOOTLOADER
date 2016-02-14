/**
* @file          usbf_hal_io.h
* @brief         USB function control module
* @author        zhangjie
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __USBF_IO_INC__
#define __USBF_IO_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "usbf_core_data_transfer.h"

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/
#define REG_CONF_NUM_0      (0)   /* configuration number 0 */
#define REG_CONF_NUM_1      (1)   /* configuration number 1 */

#define REG_SOF_INT         (1)   /* SOF receiving interrupt */
#define REG_BUSRST_INT      (2)   /* bus reset interrupt */
#define REG_CONFIG_INT      (3)   /* configuration interrupt */
#define REG_SUSPEND_INT     (4)   /* suspend interrupt */
#define REG_WAKEUP_INT      (5)   /* wakeup interrupt */

#define REG_EP0_SEND_INT    (4)   /* endpoint0 sends data interrupt */
#define REG_EP0_RCV_INT     (5)   /* endpoint0 receives data interrupt */
#define REG_EP0_SPK_INT     (6)   /* endpoint0 SPK interrupt */

#define REG_SEND_BUFFER     (0)   /* endpoint0 sending buff */
#define REG_RCV_BUFFER      (1)   /* endpoint0 receiving buff */

/*----------------------------------------------------------------------------*/
/* Local Constant Definition                                                 */
/*----------------------------------------------------------------------------*/
#define TMSP_MASK               (0x07FFU)   /* SOF packet receives data frame number mask */
#define EP0_PKTSIZE_MASK        (0x7FU)     /* endpoint0 packet size mask */
#define EP0_OUT_PKTSIZE_MASK    (0x7FU)     /* endpoint0 OUT packet size mask */

#define EPN_SPKIE_MASK          (0x2000U)   /* endpointN SPK transfer interrupt enable mask */
#define EPN_SPK_MASK            (0x0200U)   /* endpointN SPK transfer interrupt getting mask */

#define EPN_DRQIE_MASK          (0x4000U)   /* endpointN packet transfer interrupt enable mask */
#define EPN_DRQ_MASK            (0x0400U)   /* endpointN packet transfer interrupt getting mask */

#define EPN_STALL_MASK          (0x0200U)   /* endpointN STALL status set mask */

#define EP1_OUT_PKTSIZE_MASK    (0x01FFU)   /* endpoint1 OUT packet size mask */
#define EPN_OUT_PKTSIZE_MASK    (0x007FU)   /* endpointN OUT packet size mask */

#if (USBF_TRANS_NULL == USBF_ENABLE)
#define EPN_DMA_MASK            (0x0C00U)   /* endpointN DMA automatic transfer enable mask */
#else
#define EPN_DMA_MASK            (0x0800U)   /* endpointN DMA automatic transfer enable mask */
#endif/* (USBF_TRANS_NULL == USBF_ENABLE) */

#define EPN_ATTR_MASK           (0x0E00U)   /* endpoint attribute mask */
#define EPN_ENABLE              (0x8000U)   /* endpoint effective */
#define EPN_TYPE_SHIFT          (13)        /* endpoint transfer type from left shifts */
#define EPN_DIR_SHIFT           (12)        /* endpoint transfer direction from left shifts */

#define EPN_FIFO_INIT_MASK      (0x8000U)   /* endpointN buffer initialize mask */
#define EPN_BUSY_MASK           (0x0800U)   /* endpointN busy status getting mask */

/*----------------------------------------------------------------------------*/
/* Global Variable Definition                                                 */
/*----------------------------------------------------------------------------*/
typedef struct _ST_EP_REGISTER_ADDR{
  volatile uint16_t*     ctrl_reg;
  volatile uint16_t*     status_reg;
  volatile uint16_t*     data_reg;
}ST_EP_REGISTER_ADDR;

extern ST_EP_REGISTER_ADDR* reg_epreg;

/******************************************************************************/
/* Function Name  :  usbfunc_reg_enable_event                                 */
/* Parameters     :  type : the specified macro interrupt type                */
/*                         REG_SOF_INT    : SOF receiving interrupt           */
/*                         REG_BUS_RSTINT : bus reset interrupt               */
/*                         REG_CONFIG_INT : configuration interrupt           */
/*                         REG_SUSPEND_INT: suspend interrupt                 */
/*                         REG_WAKEUP_INT : wakeup interrupt                  */
/* Return Value   :  none                                                     */
/* Description    :  set enable to the specified USB function macro interrupt */
/******************************************************************************/
#define usbfunc_reg_enable_event(type) usbfunc_reg_enable_event##_##type

/* set enable to SOF receiving interrupt  */
#define usbfunc_reg_enable_event_REG_SOF_INT        \
    REG_W(bFM3_USB0_UDCIE_SOFIE, 1)

/* set enable to bus reset interrupt */
#define usbfunc_reg_enable_event_REG_BUSRST_INT     \
    REG_W(bFM3_USB0_UDCIE_BRSTIE, 1)

/* set enable to configuration interrupt */
#define usbfunc_reg_enable_event_REG_CONFIG_INT     \
    REG_W(bFM3_USB0_UDCIE_CONFIE, 1)

/* set enable to suspend interrupt */
#define usbfunc_reg_enable_event_REG_SUSPEND_INT    \
    REG_W(bFM3_USB0_UDCIE_SUSPIE, 1)

/* set enable to wakeup interrupt */
#define usbfunc_reg_enable_event_REG_WAKEUP_INT     \
    REG_W(bFM3_USB0_UDCIE_WKUPIE, 1)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_disable_event                                */
/* Parameters     :  type : the specified macro interrupt type                */
/*                         REG_SOF_INT    : SOF receiving interrupt           */
/*                         REG_BUS_RSTINT : bus reset interrupt               */
/*                         REG_CONFIG_INT : configuration interrupt           */
/*                         REG_SUSPEND_INT: suspend interrupt                 */
/*                         REG_WAKEUP_INT : wakeup interrupt                  */
/* Return Value   :  none                                                     */
/* Description    :  set disable to the specified USB function macro interrupt*/
/******************************************************************************/
#define usbfunc_reg_disable_event(type) usbfunc_reg_disable_event##_##type

/* set disable to SOF receiving interrupt */
#define usbfunc_reg_disable_event_REG_SOF_INT        \
     REG_W(bFM3_USB0_UDCIE_SOFIE, 0)

/* set disable to bus reset interrupt */
#define usbfunc_reg_disable_event_REG_BUSRST_INT     \
     REG_W(bFM3_USB0_UDCIE_BRSTIE, 0)

/* set disable to configuration interrupt */
#define usbfunc_reg_disable_event_REG_CONFIG_INT     \
     REG_W(bFM3_USB0_UDCIE_CONFIE, 0)

/* set disable to suspend interrupt */
#define usbfunc_reg_disable_event_REG_SUSPEND_INT    \
     REG_W(bFM3_USB0_UDCIE_SUSPIE, 0)

/* set disable to wakeup interrup */
#define usbfunc_reg_disable_event_REG_WAKEUP_INT     \
     REG_W(bFM3_USB0_UDCIE_WKUPIE, 0)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_get_eventint                                 */
/* Parameters     :  type : the specified macro interrupt type                */
/*                         REG_SOF_INT    : SOF receiving interrupt           */
/*                         REG_BUS_RSTINT : bus reset interrupt               */
/*                         REG_CONFIG_INT : configuration interrupt           */
/*                         REG_SUSPEND_INT: suspend interrupt                 */
/*                         REG_WAKEUP_INT : wakeup interrupt                  */
/* Return Value   : the specified USB function macro interrupt factor         */
/*                        USBF_TRUE  : interrupt generated                    */
/*                        USBF_FALSE : interrupt did not generate             */
/* Description    :  get the specified USB function macro interrupt factor    */
/******************************************************************************/
#define usbfunc_reg_get_eventint(type) usbfunc_reg_get_eventint##_##type

/* get SOF receiving interrupt factor*/
#define usbfunc_reg_get_eventint_REG_SOF_INT                                \
                            (REG_R(bFM3_USB0_UDCS_SOF) &&  REG_R(bFM3_USB0_UDCIE_SOFIE))

/* get bus reset interrupt factor*/
#define usbfunc_reg_get_eventint_REG_BUSRST_INT                             \
                            (REG_R(bFM3_USB0_UDCS_BRST) && REG_R(bFM3_USB0_UDCIE_BRSTIE))

/* get configuration interrupt factor*/
#define usbfunc_reg_get_eventint_REG_CONFIG_INT                             \
                            (REG_R(bFM3_USB0_UDCS_CONF) && REG_R(bFM3_USB0_UDCIE_CONFIE))

/* get suspend interrupt factor*/
#define usbfunc_reg_get_eventint_REG_SUSPEND_INT                            \
                            (REG_R(bFM3_USB0_UDCS_SUSP) && REG_R(bFM3_USB0_UDCIE_SUSPIE))

/* get wakeup interrupt factor*/
#define usbfunc_reg_get_eventint_REG_WAKEUP_INT                             \
                            (REG_R(bFM3_USB0_UDCS_WKUP) && REG_R(bFM3_USB0_UDCIE_WKUPIE))

/******************************************************************************/
/* Function Name  :  usbfunc_reg_clear_eventint                               */
/* Parameters     :  type : the specified macro interrupt type                */
/*                         REG_SOF_INT    : SOF receiving interrupt           */
/*                         REG_BUS_RSTINT : bus reset interrupt               */
/*                         REG_CONFIG_INT : configuration interrupt           */
/*                         REG_SUSPEND_INT: suspend interrupt                 */
/*                         REG_WAKEUP_INT : wakeup interrupt                  */
/* Return Value   :  none                                                     */
/* Description    :  clear the specified USB function macro interrupt         */
/******************************************************************************/
#define usbfunc_reg_clear_eventint(type) usbfunc_reg_clear_eventint##_##type

/* clear SOF receiving interrupt */
#define usbfunc_reg_clear_eventint_REG_SOF_INT                   \
            REG_W(bFM3_USB0_UDCS_SOF, 0)

/* clear bus reset interrupt */
#define usbfunc_reg_clear_eventint_REG_BUSRST_INT                \
            REG_W(bFM3_USB0_UDCS_BRST, 0)

/* clear configuration interruptt */
#define usbfunc_reg_clear_eventint_REG_CONFIG_INT                \
            REG_W(bFM3_USB0_UDCS_CONF, 0)

/* clear suspend interrupt */
#define usbfunc_reg_clear_eventint_REG_SUSPEND_INT               \
            REG_W(bFM3_USB0_UDCS_SUSP, 0)

/* clear wakeup interrupt */
#define usbfunc_reg_clear_eventint_REG_WAKEUP_INT                \
            REG_W(bFM3_USB0_UDCS_WKUP, 0)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_enable_ep0                                   */
/*                   in specified USB function macro                          */
/* Parameters     :  type :                                                   */
/*                   endpoint0 interrupt type in specified USB function macro */
/*                         REG_EP0_RCV_INT  : receives data interrupt         */
/*                         REG_EP0_SEND_INT : sends data interrupt            */
/*                         REG_EP0_SPK_INT  : SPK interrupt                   */
/* Return Value   :  none                                                     */
/* Description    :  set enable to endpoint0 interrupt                        */
/******************************************************************************/
#define usbfunc_reg_enable_ep0(type) usbfunc_reg_enable_ep0##_##type

/* set enable to the receives data interrupt */
#define usbfunc_reg_enable_ep0_REG_EP0_RCV_INT                 \
    REG_W(bFM3_USB0_EP0OS_DRQOIE, 1)

/* set enable to the sends data interrupt */
#define usbfunc_reg_enable_ep0_REG_EP0_SEND_INT                \
    REG_W(bFM3_USB0_EP0IS_DRQIIE, 1)

/* set enable to the SPK interrupt */
#define usbfunc_reg_enable_ep0_REG_EP0_SPK_INT                 \
    REG_W(bFM3_USB0_EP0OS_SPKIE, 1)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_disable_ep0                                  */
/* Parameters     :  type :                                                   */
/*                   endpoint0 interrupt type in specified USB function macro */
/*                         REG_EP0_RCV_INT  : receives data interrupt         */
/*                         REG_EP0_SEND_INT : sends data interrupt            */
/*                         REG_EP0_SPK_INT  : SPK interrupt                   */
/* Return Value   :  none                                                     */
/* Description    :  set disable to endpoint0 interrupt                       */
/*                   in specified USB function macro                          */
/******************************************************************************/
#define usbfunc_reg_disable_ep0(type) usbfunc_reg_disable_ep0##_##type

/* set disable to the receive data interrupt */
#define usbfunc_reg_disable_ep0_REG_EP0_RCV_INT               \
    REG_W(bFM3_USB0_EP0OS_DRQOIE, 0)

/* set disable to the sends data interrupt */
#define usbfunc_reg_disable_ep0_REG_EP0_SEND_INT              \
    REG_W(bFM3_USB0_EP0IS_DRQIIE, 0)

/* set disable to the SPK interrupt */
#define usbfunc_reg_disable_ep0_REG_EP0_SPK_INT               \
    REG_W(bFM3_USB0_EP0OS_SPKIE, 0)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_get_ep0int                                   */
/* Parameters     :  type :                                                   */
/*                   endpoint0 interrupt type in specified USB function macro */
/*                         REG_EP0_RCV_INT  : receives data interrupt         */
/*                         REG_EP0_SEND_INT : sends data interrupt            */
/*                         REG_EP0_SPK_INT  : SPK interrupt                   */
/* Return Value   : endpoint0 interrupt factor                                */
/*                        USBF_TRUE  : interrupt generated                    */
/*                        USBF_FALSE : interrupt did not generate             */
/* Description    :  get endpoint0 interrupt factor                           */
/******************************************************************************/
#define usbfunc_reg_get_ep0int(type) usbfunc_reg_get_ep0int##_##type


/* get receives data interrupt factor */
#define usbfunc_reg_get_ep0int_REG_EP0_RCV_INT                              \
                            (REG_R(bFM3_USB0_EP0OS_DRQO) && REG_R(bFM3_USB0_EP0OS_DRQOIE))

/* get sends data interrupt factor */
#define usbfunc_reg_get_ep0int_REG_EP0_SEND_INT                             \
                            (REG_R(bFM3_USB0_EP0IS_DRQI) &&  REG_R(bFM3_USB0_EP0IS_DRQIIE))

/* get SPK interrupt factor */
#define usbfunc_reg_get_ep0int_REG_EP0_SPK_INT                              \
                            (REG_R(bFM3_USB0_EP0OS_SPK) && REG_R(bFM3_USB0_EP0OS_SPKIE))

/******************************************************************************/
/* Function Name  :  usbfunc_reg_clear_ep0int                                 */
/* Parameters     :  type : endpoint0 interrupt type                          */
/*                         REG_EP0_RCV_INT  : receives data interrupt         */
/*                         REG_EP0_SEND_INT : sends data interrupt            */
/*                         REG_EP0_SPK_INT  : SPK interrupt                   */
/* Return Value   :  none                                                     */
/* Description    :  clear the endpoint0 interrupt                            */
/******************************************************************************/
#define usbfunc_reg_clear_ep0int(type) usbfunc_reg_clear_ep0int##_##type

/* clear receives data interrupt */
#define usbfunc_reg_clear_ep0int_REG_EP0_RCV_INT                \
    REG_W(bFM3_USB0_EP0OS_DRQO, 0)

/* clear sends data interrupt  */
#define usbfunc_reg_clear_ep0int_REG_EP0_SEND_INT               \
    REG_W(bFM3_USB0_EP0IS_DRQI, 0)

/* clear SPK interrupt  */
#define usbfunc_reg_clear_ep0int_REG_EP0_SPK_INT                \
    REG_W(bFM3_USB0_EP0OS_SPK, 0)

/******************************************************************************/
/* Function Name  : usbfunc_reg_enable_epntrans                               */
/* Parameters     : epid : the specified endpoint number                      */
/* Return Value   : none                                                      */
/* Description    : set enable to packet transfer in the specified endpoint   */
/******************************************************************************/
#define usbfunc_reg_enable_epntrans(epid)                                   \
    ((*reg_epreg[(epid) - 1].status_reg) |= EPN_DRQIE_MASK)

/******************************************************************************/
/* Function Name : usbfunc_reg_disable_epntrans                               */
/* Parameters    : epid : the specified endpoint number                       */
/* Return Value  : none                                                       */
/* Description   : set disable to packet transfer in the specified endpoint   */
/******************************************************************************/
#define usbfunc_reg_disable_epntrans(epid)                                  \
    ((*reg_epreg[(epid) - 1].status_reg) &= ~EPN_DRQIE_MASK)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_get_epntransint                              */
/* Parameters     :  epid : the specified endpoint number                     */
/* Return Value   :  factor of the packet interrupt in specified endpoint     */
/*                        USBF_TRUE  : interrupt generated                    */
/*                        USBF_FALSE : interrupt did not generate             */
/* Description    :  get the factor of the packet interrupt                   */
/*                   in specified endpoint                                    */
/******************************************************************************/
#define usbfunc_reg_get_epntransint(epid)                                   \
                        (((*reg_epreg[(epid) - 1].status_reg) &             \
                           (EPN_DRQ_MASK | EPN_DRQIE_MASK))                 \
                          == (EPN_DRQ_MASK | EPN_DRQIE_MASK))

/******************************************************************************/
/* Function Name  :  usbfunc_reg_clear_epntransint                            */
/* Parameters     :  epid : the specified endpoint number                     */
/* Return Value   :  none                                                     */
/* Description    :  clear the packet interrupt in specified endpoint         */
/******************************************************************************/
#define usbfunc_reg_clear_epntransint(epid)                                \
    ((*reg_epreg[(epid) - 1].status_reg) &= ~EPN_DRQ_MASK)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_enable_epnspk                                */
/* Parameters     :  epid : the specified endpoint number                     */
/* Return Value   :  none                                                     */
/* Description    :  set enable to the SPK transfer in the specified endpoint */
/******************************************************************************/
#define usbfunc_reg_enable_epnspk(epid)                                    \
    ((*reg_epreg[(epid) - 1].status_reg) |= EPN_SPKIE_MASK)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_disable_epnspk                               */
/* Parameters     :  epid : the specified endpoint number                     */
/* Return Value   :  none                                                     */
/* Description    :  set disable to the SPK transfer in the specified endpoint*/
/******************************************************************************/
#define usbfunc_reg_disable_epnspk(epid)                                    \
    ((*reg_epreg[(epid) - 1].status_reg) &= ~EPN_SPKIE_MASK)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_get_epnspkint                                */
/* Parameters     :  epid : the specified endpoint number                     */
/* Return Value   :  the factor of the SPK interrupt in specified endpoint    */
/*                        USBF_TRUE  : interrupt generated                    */
/*                        USBF_FALSE : interrupt did not generate             */
/* Description    :  get the factor of the SPK interrupt in                   */
/*                   the specified endpoint                                   */
/******************************************************************************/
#define usbfunc_reg_get_epnspkint(epid)                                     \
                        (((*reg_epreg[(epid) - 1].status_reg) &             \
                           (EPN_SPK_MASK | EPN_SPKIE_MASK))                 \
                          == (EPN_SPK_MASK | EPN_SPKIE_MASK))

/******************************************************************************/
/* Function Name  :  usbfunc_reg_clear_epnspkint                              */
/* Parameters     :  epid : the specified endpoint number                     */
/* Return Value   :  none                                                     */
/* Description    :  clear the SPK transfer in the specified endpoint         */
/******************************************************************************/
#define usbfunc_reg_clear_epnspkint(epid)                                  \
    ((*reg_epreg[(epid) - 1].status_reg) &= ~EPN_SPK_MASK)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_enable_dma                                   */
/* Parameters     :  epid : the specified endpoint number                     */
/* Return Value   :  none                                                     */
/* Description    :  set enable to DMA automatic transfer                     */
/*                   in specified endpoint number                             */
/******************************************************************************/
#define usbfunc_reg_enable_dma(epid)                            \
    ((*reg_epreg[(epid) - 1].ctrl_reg) |= EPN_DMA_MASK)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_disable_dma                                  */
/* Parameters     :  epid : the specified endpoint number                     */
/* Return Value   :  none                                                     */
/* Description    :  set disable to DMA automatic transfer                    */
/*                   in specified endpoint number                             */
/******************************************************************************/
#define usbfunc_reg_disable_dma(epid)                      \
    ((*reg_epreg[(epid) - 1].ctrl_reg) &= ~EPN_DMA_MASK)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_get_setupflag                                */
/* Parameters     :  none                                                     */
/* Return Value   :  setup stage detection flag                               */
/*                        USBF_TRUE  : setup stage detected                   */
/*                        USBF_FALSE : setup stage not detected               */
/* Description    :  get setup stage detection flag                           */
/******************************************************************************/
#define usbfunc_reg_get_setupflag()                                         \
                            (REG_R(bFM3_USB0_UDCS_SETP))

/******************************************************************************/
/* Function Name  :  usbfunc_reg_clear_setupflag                              */
/* Parameters     :  none                                                     */
/* Return Value   :  none                                                     */
/* Description    :  clear setup stage detection flag                         */
/******************************************************************************/
#define usbfunc_reg_clear_setupflag()                                       \
    REG_W(bFM3_USB0_UDCS_SETP, 0)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_get_sofnum                                   */
/* Parameters     :  none                                                     */
/* Return Value   :  the SOF packet receiving frame number                    */
/* Description    :  get the SOF packet receiving frame number                */
/******************************************************************************/
#define usbfunc_reg_get_sofnum()                                            \
    ((uint16_t)REG_R(FM3_USB0->TMSP) & TMSP_MASK)

//    ((uint16_t)REG_R(USB0->TMSP) & TMSP_MASK)		???

/******************************************************************************/
/* Function Name  :  usbfunc_reg_get_confnum                                  */
/* Parameters     :  none                                                     */
/* Return Value   :  CONF_NUM_0 : configuration number0                       */
/*                   CONF_NUM_1 : configuration number1                       */
/* Description    :  get the configuration number                             */
/******************************************************************************/
#define usbfunc_reg_get_confnum()                                           \
    ((uint8_t)REG_R(bFM3_USB0_UDCIE_CONFN))

/******************************************************************************/
/* Function Name  :  usbfunc_reg_get_busy                                     */
/* Parameters     :  epid : the specified endpoint number                     */
/* Return Value   :  USBF_TRUE  : busy                                        */
/*                   USBF_FALSE : free                                        */
/* Description    :  by the specified endpoint number,                        */
/*                   get the busy factor(excludes endpoint0)                  */
/******************************************************************************/
#define usbfunc_reg_get_busy(epid)                                              \
                        (((*reg_epreg[(epid) - 1].status_reg) & EPN_BUSY_MASK)  \
                          == EPN_BUSY_MASK)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_get_ep0outpktsize                            */
/* Parameters     :  none                                                     */
/* Return Value   :  the OUT packet size of the endpoint 0                    */
/* Description    :  get the OUT packet size of the endpoint 0                */
/******************************************************************************/
#define usbfunc_reg_get_ep0outpktsize()                                     \
    ((uint8_t)REG_R(FM3_USB0->EP0OS) & EP0_OUT_PKTSIZE_MASK)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_get_epnoutpktsize                            */
/* Parameters     :  epid : the specified endpoint number                     */
/* Return Value   :  the OUT packet size of the specified endpoint            */
/* Description    :  by the specified endpoint number,                        */
/*                   get the OUT packet size                                  */
/******************************************************************************/
#define usbfunc_reg_get_epnoutpktsize(epid)                                 \
               ((*reg_epreg[(epid) - 1].status_reg) &                       \
                (((epid) == USBF_EP1)  ?                                    \
                 EP1_OUT_PKTSIZE_MASK :                                     \
                 EPN_OUT_PKTSIZE_MASK))

/******************************************************************************/
/* Function Name  :  usbfunc_reg_set_resume                                   */
/* Parameters     :  resume_flag : USB resume instruction flag                */
/*                   USBF_TRUE    set USB resume instruction                  */
/*                   USBF_FALSE   clear USB resume instruction                */
/* Return Value   :  none                                                     */
/* Description    :  USB resume instruction setting                           */
/******************************************************************************/
#define usbfunc_reg_set_resume(resume_flag)                                 \
    REG_W(bFM3_USB0_UDCC_RESUM, resume_flag)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_set_ep0stall                                 */
/* Parameters     :  stall_flag : The stall factorg                           */
/*                           USBF_TRUE : set stall                            */
/*                           USBF_FALSE: clear stall                          */
/* Return Value   :  none                                                     */
/* Description    :  stall factorg of the endpoint0 setting                   */
/******************************************************************************/
#define usbfunc_reg_set_ep0stall(stall_flag)                                \
    REG_W(bFM3_USB0_EP0C_STAL, stall_flag)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_get_ep0stall                                 */
/* Parameters     :  none                                                     */
/* Return Value   :  STALL factor:  USBF_TRUE :  set stall                    */
/*                                  USBF_FALSE:  clear stall                  */
/* Description    :  get the stall factor                                     */
/******************************************************************************/
#define usbfunc_reg_get_ep0stall()                                          \
    REG_R(bFM3_USB0_EP0C_STAL)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_set_epnstall                                 */
/* Parameters     :  epid : the specified endpoint number                     */
/*                   STALL factor:  USBF_TRUE :  set stall                    */
/*                                  USBF_FALSE:  clear stall                  */
/* Return Value   :  none                                                     */
/* Description    :  by the specified endpoint number,                        */
/*                   set the stall factor                                     */
/******************************************************************************/
#define usbfunc_reg_set_epnstall(epid, stall_flag)                          \
                usbfunc_reg_set_epnstall##_##stall_flag(epid)

//                usbfunc_reg_set_epnstall##_##stall_flag##(epid)	?

/* set the stall  */
#define usbfunc_reg_set_epnstall_TRUE(epid)                         \
                ((*reg_epreg[(epid) - 1].ctrl_reg) |= EPN_STALL_MASK)

/* clear the stall */
#define usbfunc_reg_set_epnstall_FALSE(epid)                        \
                ((*reg_epreg[(epid) - 1].ctrl_reg) &= ~EPN_STALL_MASK)

/******************************************************************************/
/* Function Name  :  usbfunc_reg_set_ustp                                     */
/* Parameters     :  val :                                                    */
/*                    USBF_TRUE : the USB operation block stop                */
/*                    USBF_FALSE: normal mode                                 */
/* Return Value   :  none                                                     */
/* Description    :  set normal mode to the USB operation block               */
/******************************************************************************/
#define usbfunc_reg_set_ustp(val) \
            REG_W(bFM3_USB0_UDCC_USTP, val)

/******************************************************************************/
/* Global Function Prototypes                                                 */
/******************************************************************************/
/* USB function macro control module initialize */
extern void usbfunc_reg_init(void);

/* USB function macro start */
extern void usbfunc_reg_start(void);

/* USB function macro stop */
extern void usbfunc_reg_stop(void);

/* set the packet size of the endpoint0 */
extern void usbfunc_reg_set_ep0pktsize(uint8_t ep0_pktsize);

/* set each endpoint attribute */
extern void usbfunc_reg_set_epnattr(const USBFUNC_ST_DATATRANS_EPINFO* epn_info_ptr);

/* endpoint0 FIFO buffer initialize */
extern void usbfunc_reg_init_ep0fifo(uint8_t fifo_type);

/* each endpoint FIFO buffer initialize */
extern void usbfunc_reg_init_epnfifo(uint8_t epid);

/* endpoint0 FIFO buffer writting */
extern void usbfunc_reg_write_ep0fifo(const uint8_t* addr,uint16_t size);

/* each endpoint FIFO buffer writting */
extern void usbfunc_reg_write_epnfifo(uint8_t epid, const uint8_t* addr,uint16_t size);

/* endpoint0 FIFO buffer reading */
extern void usbfunc_reg_read_ep0fifo(uint8_t* addr,uint16_t size);

/* each endpoint FIFO buffer reading */
extern void usbfunc_reg_read_epnfifo(uint8_t epid, uint8_t* addr,uint16_t size);

#if (USBF_VBUS_CTRL == USBF_ENABLE)
/* get state of I/O port which connects to VBUS */
extern uint8_t usbfunc_reg_get_vbusstatus(void);
#endif

/* set USB connect */
extern void usbfunc_reg_set_connect(uint8_t connect_type);

#endif
