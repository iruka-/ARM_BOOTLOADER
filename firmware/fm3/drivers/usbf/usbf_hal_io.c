/**
* @file          usbf_hal_io.c
* @brief         USB function control module
* @author        zhangjie
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "usbf.h"
/*#include "gpio.h"*/
#include "usbf_hal_io.h"
#include "usbf_core_data_transfer.h"
#if (USBF_USE_DMA == USBF_ENABLE)  /* use DMA */
#include "usbf_hal_dma.h"
#endif

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

#define CONDITION_PID           (0)         /* select alternating data toggle mode */
#define NO_CONDITION_PID        (1)         /* select data toggle mode */
#define BUS_POWER               (0)         /* bus-power */
#define SELF_POWER              (1)         /* self-power */
#define HARD_AUTO_CLEAR         (1)         /* STALL is cleared automatically by hardware */

#define ID_TO_INDEX(epid)       ((epid) - 1) /* creat  index of endpoint */

/*----------------------------------------------------------------------------*/
/* Static Variable Definition                                                 */
/*----------------------------------------------------------------------------*/
ST_EP_REGISTER_ADDR* reg_epreg;

/*----------------------------------------------------------------------------*/
/* Static Type Definition                                                     */
/*----------------------------------------------------------------------------*/

typedef volatile union _ST_EPN_BFDT
{
    uint16_t word;
    struct
    {
#if (CPU_ENDIAN == CPU_LITTLEENDIAN)
        uint8_t BFDTL;
        uint8_t BFDTH;
#elif (CPU_ENDIAN == CPU_BIGENDIAN)
        uint8_t BFDTH;
        uint8_t BFDTL;
#endif
    }byte;
}ST_EPN_BFDT;

static const ST_EP_REGISTER_ADDR s_epreg[USBF_MAX_EP_NUM] =
{
    /* EP1 register */
    {
        (volatile uint16_t*)&FM3_USB0->EP1C,            /* pointer of EP1 control register */
        (volatile uint16_t*)&FM3_USB0->EP1S,            /* pointer of EP1 status register  */
        (volatile uint16_t*)&FM3_USB0->EP1DT,           /* pointer of EP1 data register    */
    },

    /* EP2 register */
    {
        (volatile uint16_t*)&FM3_USB0->EP2C,            /* pointer of EP2 control register */
        (volatile uint16_t*)&FM3_USB0->EP2S,            /* pointer of EP2 status register  */
        (volatile uint16_t*)&FM3_USB0->EP2DT,           /* pointer of EP2 data register    */
    },

    /* EP3 register */
    {
        (volatile uint16_t*)&FM3_USB0->EP3C,            /* pointer of EP3 control register */
        (volatile uint16_t*)&FM3_USB0->EP3S,            /* pointer of EP3 status register  */
        (volatile uint16_t*)&FM3_USB0->EP3DT,           /* pointer of EP3 data register    */
    },

    /* EP4 register */
    {
        (volatile uint16_t*)&FM3_USB0->EP4C,            /* pointer of EP4 control register */
        (volatile uint16_t*)&FM3_USB0->EP4S,            /* pointer of EP4 status register  */
        (volatile uint16_t*)&FM3_USB0->EP4DT,           /* pointer of EP4 data register    */
    },

    /* EP5 register */
    {
        (volatile uint16_t*)&FM3_USB0->EP5C,            /* pointer of EP5 control register */
        (volatile uint16_t*)&FM3_USB0->EP5S,            /* pointer of EP5 status register  */
        (volatile uint16_t*)&FM3_USB0->EP5DT,           /* pointer of EP5 data register    */
    }
};

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

/**
* @brief            USB function macro control module initialize
*
* @param            none
* @return           none
*
**/
void usbfunc_reg_init(void)
{
#if (USBF_VBUS_CTRL == USBF_ENABLE)

    /* I/O port connects to VBUS initialize */
    REG_W(USBF_VBUS_IO_DDR, 0);

#endif

#if (USBF_PULLUP_CTRL == USBF_ENABLE)

    /* I/O port connects to pull-up control initialize */
    REG_W(USBF_PULLUP_IO_DDR, 1);
    /* set cutting to pull-up control */
    REG_W(USBF_PULLUP_IO_PDR, USBF_PULLUP_NOTCONNECT);

#endif

    /* USB function reset */
    REG_W(bFM3_USB0_UDCC_RST, 1);

    /* wait for 3 cycle */
    __NOP();
    __NOP();
    __NOP();

    /* set normal mode to USB clock */
    usbfunc_reg_set_ustp(FALSE);

    /* set data toggle mode */
    REG_W(bFM3_USB0_UDCC_RFBK, NO_CONDITION_PID);

    /* set self-power */
    REG_W(bFM3_USB0_UDCC_PWC, SELF_POWER);

#if (USBF_AUTO_CLEAR_STALL == USBF_ENABLE)
    /* enable that hardware automatically cleare STALL */
    REG_W(bFM3_USB0_UDCC_STALCLREN, HARD_AUTO_CLEAR);
#else
    /* disable that hardware automatically cleare STALL */
    REG_W(bFM3_USB0_UDCC_STALCLREN, ~HARD_AUTO_CLEAR);
#endif
    /* clear USB resume instruction  */
    usbfunc_reg_set_resume(FALSE);

    return;
}

/**
* @brief            USB function macro start
*
* @param            none
* @return           none
*
**/
void usbfunc_reg_start(void)
{
    /* clear USB function macro reset */
    REG_W(bFM3_USB0_UDCC_RST, 0);

    /* clear endpoint0 sending and receiving buffer initialize */
    REG_W(bFM3_USB0_EP0IS_BFINI, 0);
    REG_W(bFM3_USB0_EP0OS_BFINI, 0);

    /* clear endpoint1~3 sending and receiving buffer initialize */
    *s_epreg[ID_TO_INDEX(USBF_EP1)].status_reg &= ~EPN_FIFO_INIT_MASK;
    *s_epreg[ID_TO_INDEX(USBF_EP2)].status_reg &= ~EPN_FIFO_INIT_MASK;
    *s_epreg[ID_TO_INDEX(USBF_EP3)].status_reg &= ~EPN_FIFO_INIT_MASK;
    *s_epreg[ID_TO_INDEX(USBF_EP4)].status_reg &= ~EPN_FIFO_INIT_MASK;
    *s_epreg[ID_TO_INDEX(USBF_EP5)].status_reg &= ~EPN_FIFO_INIT_MASK;

#if (USBF_SOF_ENABLE == USBF_ENABLE)
    /* enable SOF receiving  */
    usbfunc_reg_enable_event(REG_SOF_INT);
#endif

    /* enable busreset  */
    usbfunc_reg_enable_event(REG_BUSRST_INT);

    /* enable configuration */
    usbfunc_reg_enable_event(REG_CONFIG_INT);

    /* enable suspend */
    usbfunc_reg_enable_event(REG_SUSPEND_INT);

    /* enable endpoint0 receive data interrupt */
    usbfunc_reg_enable_ep0(REG_EP0_RCV_INT);

    reg_epreg = (ST_EP_REGISTER_ADDR*)&s_epreg[0];

    return;
}

/**
* @brief            USB function macro stop
*
* @param            none
* @return           none
*
**/
void usbfunc_reg_stop(void)
{
#if (USBF_SOF_ENABLE == USBF_ENABLE)
    /* disable SOF receiving */
    usbfunc_reg_disable_event(REG_SOF_INT);
#endif

    /* disable bus reset */
    usbfunc_reg_disable_event(REG_BUSRST_INT);

    /* disable configuration */
    usbfunc_reg_disable_event(REG_CONFIG_INT);

    /* disable suspend */
    usbfunc_reg_disable_event(REG_SUSPEND_INT);

    /* disable wakeup */
    usbfunc_reg_disable_event(REG_WAKEUP_INT);

    /* disable endpoint0 send data interrupt */
    usbfunc_reg_disable_ep0(REG_EP0_SEND_INT);

    /* disable endpoint0 receive data interrupt */
    usbfunc_reg_disable_ep0(REG_EP0_RCV_INT);

    /* disable endpoint1 packet transfer interrupt */
    usbfunc_reg_disable_epntrans(USBF_EP1);

    /* disable endpoint2 packet transfer interrupt */
    usbfunc_reg_disable_epntrans(USBF_EP2);

    /* disable endpoint3 packet transfer interrupt */
    usbfunc_reg_disable_epntrans(USBF_EP3);

    /* disable endpoint4 packet transfer interrupt */
    usbfunc_reg_disable_epntrans(USBF_EP4);

    /* disable endpoint5 packet transfer interrupt */
    usbfunc_reg_disable_epntrans(USBF_EP5);

    /* disable endpoint0 short packet transfer interrupt */
    usbfunc_reg_disable_ep0(REG_EP0_SPK_INT);

    /* disable endpoint1 short packet transfer interrupt */
    usbfunc_reg_disable_epnspk(USBF_EP1);

    /* disable endpoint2 short packet transfer interrupt */
    usbfunc_reg_disable_epnspk(USBF_EP2);

    /* disable endpoint3 short packet transfer interrupt */
    usbfunc_reg_disable_epnspk(USBF_EP3);

    /* disable endpoint4 short packet transfer interrupt */
    usbfunc_reg_disable_epnspk(USBF_EP4);

    /* disable endpoint5 short packet transfer interrupt */
    usbfunc_reg_disable_epnspk(USBF_EP5);

    /* USB function reset */
    REG_W(bFM3_USB0_UDCC_RST, 1);

    return;
}

/**
* @brief            set packet size of endpoint0
*
* @param            ep0_pktsize : endpoint0 packet size
* @return           none
*
**/
void usbfunc_reg_set_ep0pktsize(uint8_t ep0_pktsize)
{
    uint16_t i;
    i = REG_R(FM3_USB0->EP0C);
    i = i & 0x0200;
    i = i | ((uint16_t)(ep0_pktsize & EP0_PKTSIZE_MASK) & 0x007F);
    REG_W(FM3_USB0->EP0C, i);

    return;
}

/**
* @brief            set each endpoint attribute
*
* @param            epn_info_ptr : pointer to endpoint attribute
* @return           none
*
**/
void usbfunc_reg_set_epnattr(const USBFUNC_ST_DATATRANS_EPINFO* epn_info_ptr)
{
    /* endpoint attribute */
    uint16_t epn_attr = (*s_epreg[ID_TO_INDEX(epn_info_ptr->ep_id)].ctrl_reg) & EPN_ATTR_MASK;

    /* set effective to endpoint */
    epn_attr |= EPN_ENABLE;

    /* get endpoint transfer type */
    epn_attr |= ((uint16_t)epn_info_ptr->ep_type << EPN_TYPE_SHIFT);

    /* get endpoint transfer direction */
    epn_attr |= ((uint16_t)epn_info_ptr->ep_direction << EPN_DIR_SHIFT);

    /* get endpoint packet size */
    epn_attr |= epn_info_ptr->ep_pkt_size;

    /* set endpoint attribute */
    *s_epreg[ID_TO_INDEX(epn_info_ptr->ep_id)].ctrl_reg = epn_attr;

#if (USBF_USE_DMA == USBF_ENABLE)  /* using DMA  */

    /* DMA function initialize*/
    usbfunc_dma_init(epn_info_ptr->ep_id + 1, epn_info_ptr->ep_direction,
                     s_epreg[ID_TO_INDEX(epn_info_ptr->ep_id)].data_reg);

#endif /* USBF_USE_DMA == USBF_ENABLE */

    return;
}

/**
* @brief            endpoint0 FIFO buffer initialize
*
* @param            fifo_type : endpoint0 FIFO buffer type
*                             REG_SEND_BUFFER    : sending FIFO buffer
*                             REG_RECEIVE_BUFFER : receiving FIFO buffer
* @return           none
*
**/
void usbfunc_reg_init_ep0fifo(uint8_t fifo_type)
{
    /* endpoint0 sending FIFO buffer initialize  */
    if (fifo_type == REG_SEND_BUFFER)
    {
        REG_W(bFM3_USB0_EP0IS_BFINI, 1);
        REG_W(bFM3_USB0_EP0IS_BFINI, 0);
    }
    /* endpoint0 receiving FIFO buffer initialize */
    else
    {
        REG_W(bFM3_USB0_EP0OS_BFINI, 1);
        REG_W(bFM3_USB0_EP0OS_BFINI, 0);
    }

    return;
}
/**
* @brief            each endpoint FIFO buffer initialize
*
* @param            epid : specified endpoint number
* @return           none
*
**/
void usbfunc_reg_init_epnfifo(uint8_t epid)
{
    *s_epreg[ID_TO_INDEX(epid)].status_reg |= EPN_FIFO_INIT_MASK;
    *s_epreg[ID_TO_INDEX(epid)].status_reg &= ~EPN_FIFO_INIT_MASK;

    return;
}

/**
* @brief            endpoint0 FIFO buffer writting
*
* @param            addr : transfer data buffer
* @param            size : transfer data size
* @return           none
*
**/
void usbfunc_reg_write_ep0fifo(const uint8_t* addr, uint16_t size)
{
    uint16_t  loopcount;     /* loop counter */
#if (CPU_TYPE == CPU_TYPE_16)
    /* write in FIFO buffer by unit of word */
    for (loopcount = size ; loopcount >= 2 ; loopcount -= 2)
    {
        REG_W(FM3_USB0->EP0DT, USBF_MAKEHWORD(*(addr + 1), *addr));
        addr += 2;
    }
    /* when odd number process is necessary,call odd number process */
    if (loopcount == 1)
    {
         REG_W(FM3_USB0->EP0DTL, *addr);
    }
#elif (CPU_TYPE == CPU_TYPE_32)
    /* write in FIFO buffer by unit of byte */
    for (loopcount = size ; loopcount >= 2 ; loopcount -= 2)
    {
        REG_W(FM3_USB0->EP0DTL, *addr);
        addr++;
        REG_W(FM3_USB0->EP0DTH, *addr);
        addr++;
    }
    /* when odd number process is necessary,call odd number process  */
    if (loopcount == 1)
    {
        REG_W(FM3_USB0->EP0DTL, *addr);
    }
#endif

    return;
}
/**
* @brief            each endpoint FIFO buffer writting
*
* @param            epid : specified endpoint number
* @param            addr : transfer data buffer
* @param            size : transfer data size
* @return           none
*
**/
void usbfunc_reg_write_epnfifo(uint8_t epid, const uint8_t* addr, uint16_t size)
{

    uint16_t           loopcount;      /* loop counter */
    ST_EPN_BFDT        *epn_dt;        /* data register */

    epn_dt = (ST_EPN_BFDT*)s_epreg[ID_TO_INDEX(epid)].data_reg;

#if (CPU_TYPE == CPU_TYPE_16)
    /* write in FIFO buffer by unit of word */
    for (loopcount = size ; loopcount >= 2 ; loopcount -= 2)
    {
        REG_W(epn_dt->word, USBF_MAKEHWORD(*(addr + 1), *addr));
        addr += 2;
    }
#elif (CPU_TYPE == CPU_TYPE_32)
    /* write in FIFO buffer by unit of byte*/
    for (loopcount = size ; loopcount >= 2 ; loopcount -= 2)
    {
        REG_W(epn_dt->byte.BFDTL, *addr);
        addr++;
        REG_W(epn_dt->byte.BFDTH, *addr);
        addr++;
    }
#endif

    /* when odd number process is necessary,call odd number process */
    if (loopcount == 1)
    {
         REG_W(epn_dt->byte.BFDTL, *addr);
    }

    return;
}
/**
* @brief            each endpoint FIFO buffer writting
*
* @param            addr : transfer data buffer
* @param            size : transfer data size
* @return           none
*
**/
void usbfunc_reg_read_ep0fifo(uint8_t* addr,uint16_t size)
{
    uint16_t       loopcount;                  /* loop counter */
#if (CPU_TYPE == CPU_TYPE_16)
    uint16_t       word_data;                  /* transfer data */

    /* read from FIFO buffer by unit of word */
    for (loopcount = size ; loopcount >= 2 ; loopcount -= 2)
    {
        word_data = (uint16_t)REG_R(FM3_USB0->EP0DT);
        *addr = USBF_LOBYTE(word_data);
        addr++;
        *addr = USBF_HIBYTE(word_data);
        addr++;
    }

    /* when odd number process is necessary,call odd number process */
    if (loopcount == 1)
    {
        *addr = (uint8_t)REG_R(FM3_USB0->EP0DTL);
    }
#elif (CPU_TYPE == CPU_TYPE_32)
    /* read from FIFO buffer by unit of byte */
    for (loopcount = size ; loopcount >= 2 ; loopcount -= 2)
    {
        *addr = REG_R(FM3_USB0->EP0DTL);
        addr++;
        *addr = REG_R(FM3_USB0->EP0DTH);
        addr++;
    }

    /* when odd number process is necessary,call odd number process */
    if (loopcount == 1)
    {
        *addr = (uint8_t)REG_R(FM3_USB0->EP0DTL);
    }
#endif

    return;
}
/**
* @brief            each endpoint FIFO buffer reading
*
* @param            epid : specified endpoint number
* @param            addr : transfer data buffer
* @param            size : transfer data size
* @return           none
*
**/
void usbfunc_reg_read_epnfifo(uint8_t epid, uint8_t* addr,uint16_t size)
{
    uint16_t       loopcount;                  /* loop counter  */
    ST_EPN_BFDT    *epn_dt;                    /* data register */
#if (CPU_TYPE == CPU_TYPE_16)
    uint16_t       word_data;                  /* transfer data  */

    epn_dt = (ST_EPN_BFDT*)s_epreg[ID_TO_INDEX(epid)].data_reg;

    /* read from FIFO buffer by unit of word */
    for (loopcount = size ; loopcount >= 2 ; loopcount -= 2)
    {
        word_data = (uint16_t)REG_R(epn_dt->word);
        *addr = USBF_LOBYTE(word_data);
        addr++;
        *addr = USBF_HIBYTE(word_data);
        addr++;
    }
#elif (CPU_TYPE == CPU_TYPE_32)
    epn_dt = (ST_EPN_BFDT*)s_epreg[ID_TO_INDEX(epid)].data_reg;

    /* read from FIFO buffer by unit of byte */
    for (loopcount = size ; loopcount >= 2 ; loopcount -= 2)
    {
        *addr = REG_R(epn_dt->byte.BFDTL);
        addr++;
        *addr = REG_R(epn_dt->byte.BFDTH);
        addr++;
    }
#endif

    /* when odd number process is necessary,call odd number process */
    if (loopcount == 1)
    {
        *addr = (uint8_t)REG_R(epn_dt->byte.BFDTL);
    }

    return;
}

/**
* @brief            get state of I/O port which connects to VBUS
*
* @param            none
* @return           none
*
**/
#if (USBF_VBUS_CTRL == USBF_ENABLE)
uint8_t usbfunc_reg_get_vbusstatus(void)
{
    uint8_t ret;          /* return value */

    if (REG_R(USBF_VBUS_IO_PDR) == 1)
    {
        ret = USBF_VBUS_CONNECT;
    }
    else
    {
        ret = USBF_VBUS_NOTCONNECT;
    }

    return (ret);
}
#endif

/**
* @brief            set USB connect
*
* @param            connect_type : connection  cutting
* @return           none
*
**/
void usbfunc_reg_set_connect(uint8_t connect_type)
{
    REG_W(bFM3_USB0_UDCC_HCONX, connect_type);	// LO=USB D+ PullUp

#if (USBF_PULLUP_CTRL == USBF_ENABLE)

    /* when has connection request */
    if (connect_type == FALSE)
    {
        /* connect to pull-up control I/O port */
        REG_W(USBF_PULLUP_IO_PDR, USBF_PULLUP_CONNECT);
    }
    /* when has cutting request */
    else
    {
        /* cut connection pull-up control I/O port */
        REG_W(USBF_PULLUP_IO_PDR, USBF_PULLUP_NOTCONNECT);
    }

#endif

    return;
}
