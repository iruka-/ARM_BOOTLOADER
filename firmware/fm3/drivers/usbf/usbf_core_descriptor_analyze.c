/**
* @file          usbf_core_descriptor_analyze.c
* @brief         USB descriptor analysis module
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

#include "usbf.h"
#include "usbf_core_data_transfer.h"
#include "usbf_core_descriptor_analyze.h"
#include "usbf_core_control_transfer.h"

#include "../../src/debug.h"
/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/
#define   DMAE_EPN_PKTSIZE_MASK     (0x0007)

/*----------------------------------------------------------------------------*/
/* Static Variable Definition                                                 */
/*----------------------------------------------------------------------------*/
/* endpoint buffer information */
typedef struct _ST_EP_BUF_INFO{
    /* endpoint attribute buffer storage */
    USBFUNC_ST_DATATRANS_EPINFO* ep_buf_addr;

    /* calculated endpoint numbers */
    uint8_t        computed_ep_num;

    /* in useing endpoint numbers */
    uint8_t        used_ep_num;

}ST_EP_BUF_INFO;

static uint8_t*       s_dev_desc;          /* pointer to device descriptor */
static uint8_t*       s_cfg_desc;          /* pointer to configration descriptor */
static USBF_ST_STRING_DESCRIPTOR*  s_str_desc;  /* pointer to string descriptor */
static uint8_t        s_str_desc_num;      /* the numbers of string descriptor which in using */
static uint8_t        s_used_itf_num;      /* used interface numbers */
static uint16_t       s_used_ep_id_info;   /* used endpoint number information */
static uint8_t        s_remotewakeup_flag; /* remote wakeup valid flag */

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                   */
/*----------------------------------------------------------------------------*/

/* check device descriptor information */
static uint8_t judge_dev_desc(uint8_t* dev_desc_addr, uint8_t* ep0_pkt_size_addr);

/* check configration descriptor information */
static uint8_t judge_cfg_desc(uint8_t* cfg_desc_addr, ST_EP_BUF_INFO* ep_buf_info_ptr);

/* check interface descriptor information */
static uint8_t judge_itf_desc(uint8_t* itf_desc_addr, uint16_t* itf_total_length_addr, ST_EP_BUF_INFO* ep_buf_info_ptr);

/* check endpoint descriptor information */
static uint8_t judge_ep_desc(const uint8_t* ep_desc_addr, USBFUNC_ST_DATATRANS_EPINFO* ep_buf_addr);

/* check string descriptor information */
static uint8_t judge_str_desc (USBF_ST_STRING_DESCRIPTOR* str_desc_ptr, uint8_t str_desc_num);

/* retrieval string descriptor information */
static uint8_t search_string_index(uint8_t index);

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

/**
* @brief            check descriptor information
*
* @param            usb_descriptor     descriptor storage
* @param            ep_buffer          endpoint attribute buffer
* @param            ep_buffer_size     endpoint attribute buffer size
* @return           TRUE       normality end
* @return           FALSE      unnormality end
*
**/
uint8_t usbfunc_descriptor_judge_desc(const USBF_ST_USB_DESCRIPTOR* usb_descriptor,
                                   uint8_t* ep_buffer,
                                   uint16_t ep_buffer_size)
{
    ST_EP_BUF_INFO  ep_buf_info;   /* endpoint buffer information */
    uint8_t     ep0_pkt_size;      /* endpoint0 packet size */
    uint8_t     func_str_ret;      /* return the value that check string descriptor information */
    uint8_t     func_dev_ret;      /* return the value that check device descriptor information */
    uint8_t     func_cfg_ret;      /* return the value that check configration descriptor information */
    uint8_t     ret = TRUE;        /* return value */

    /* local Variable initialize */
    s_dev_desc = NULL;           /* device descriptor storage initialize */
    s_cfg_desc = NULL;           /* configration descriptor storage initialize */
    s_str_desc = NULL;           /* string descriptor information array pointer initialize */
    s_str_desc_num = 0;          /* the numbers of string descriptor which in using initialize */
    s_used_itf_num = 0;          /* used interface numbers initialize */
    s_remotewakeup_flag = FALSE; /* remote wakeup valid flag initialize */
    s_used_ep_id_info = 0x0000;  /* used endpoint number information initialize */

    /* clear endpoint attribute buffer */
    (void)memset(ep_buffer, 0x00, ep_buffer_size);

    /* endpoint numbers calculate */
    ep_buf_info.computed_ep_num = (uint8_t)(ep_buffer_size / sizeof(USBFUNC_ST_DATATRANS_EPINFO));

//	hex_print("computed_ep_num",ep_buf_info.computed_ep_num);

    /* the numbers of endpoint which in using initialize */
    ep_buf_info.used_ep_num = 0;

    /* get endpoint attribute buffer storage */
    ep_buf_info.ep_buf_addr = (USBFUNC_ST_DATATRANS_EPINFO*)ep_buffer;

    /* check string descriptor information */
    func_str_ret = judge_str_desc(usb_descriptor->StringDescriptor, usb_descriptor->StringDescriptorNum);

    /* check device descriptor information */
	ep0_pkt_size=0;
    func_dev_ret = judge_dev_desc(usb_descriptor->DeviceDescriptor, &ep0_pkt_size);

    /* check configration descriptor information */
    func_cfg_ret = judge_cfg_desc(usb_descriptor->ConfigurationDescriptor, &ep_buf_info);

    /* when all result which check  descriptor information is normality  */
    if ((func_str_ret == TRUE) &&
        (func_dev_ret == TRUE) &&
        (func_cfg_ret == TRUE))
    {
        /* register endpoint0 attribute and set endpoint0 packet size */
        usbfunc_ctrltrans_entry_ep0info(ep0_pkt_size);

        /* endpoint attribute registeration */
        usbfunc_datatrans_entry_epinfo(ep_buf_info.used_ep_num, (USBFUNC_ST_DATATRANS_EPINFO*)(ep_buffer));
//        hex_print("datatrans_entry_epinfo",ep_buf_info.used_ep_num);
//        hex_print("buf",(int) ep_buffer);
    }
    else/* when result is unnormality */
    {
        /* clear the setted endpoint attribute buffer */
        (void)memset(ep_buffer, 0x00, ep_buffer_size);

        /* return false */
        ret = FALSE;

        /* local Variable initialize */
        s_dev_desc = NULL;           /* device descriptor storage initialize */
        s_cfg_desc = NULL;           /* configration descriptor storage initialize */
        s_str_desc = NULL;           /* string descriptor information array pointer initialize */
        s_str_desc_num = 0;          /* the numbers of string descriptor which in using initialize */
        s_used_itf_num = 0;          /* used interface numbers initialize */
        s_remotewakeup_flag = FALSE; /* remote wakeup valid flag initialize */
        s_used_ep_id_info = 0x0000;  /* used endpoint number information initialize */
    }

    return(ret);
}

/**
* @brief            get descriptor information
*
* @param            dev_desc_ptr   pointer to device descriptor
* @param            dev_desc_size  pointer to device descriptor size
* @return           none
*
**/
void usbfunc_descriptor_get_dev_desc(uint8_t** dev_desc_ptr, uint16_t* dev_desc_size)
{
    /* set device descriptor */
    *dev_desc_ptr = s_dev_desc;

    /* set device descriptor size */
    *dev_desc_size = (uint16_t)s_dev_desc[USBF_DESC_LENGTH];

    return;
}

/**
* @brief            get configurationdescriptor information
*
* @param            cfg_desc_ptr   pointer to configration descriptor
* @param            cfg_desc_size  pointer to configration descriptor size
* @return           none
*
**/
void usbfunc_descriptor_get_cfg_desc(uint8_t** cfg_desc_ptr, uint16_t* cfg_desc_size)
{
    /* set configration descriptor */
    *cfg_desc_ptr = s_cfg_desc;

    /* set configration descriptor size */
    *cfg_desc_size = MAKEHWORD(s_cfg_desc[USBF_CFG_TOTALLENGTH + 1], s_cfg_desc[USBF_CFG_TOTALLENGTH]);

    return;
}

/**
* @brief            get specified string descriptor information
*
* @param            str_desc_ptr      pointer to string descriptor
* @param            str_desc_size     pointer to string descriptor size
* @param            str_desc_index    string descripto index
* @return           TRUE      normalitily end
* @return           FALSE     unnormalitily end
*
**/
uint8_t usbfunc_descriptor_get_str_desc(uint8_t** str_desc_ptr,
                                     uint16_t* str_desc_size,
                                     uint8_t str_desc_index)
{
    uint8_t   str_loop_cnt;             /* loop counter */
    uint8_t   ret = FALSE;              /* return value */
    USBF_ST_STRING_DESCRIPTOR* str_desc = s_str_desc;  /* pointer to string descriptor */

    /* retrieve specified string descriptor index */
    for (str_loop_cnt = 0; str_loop_cnt < s_str_desc_num; str_loop_cnt++)
    {
        /* when specified string descriptor index is existence */
        if (str_desc->StringDescriptorIndex == str_desc_index)
        {
            /* set specified string descriptor pointer */
            *str_desc_ptr = str_desc->StringDescriptorInfo;

            /* set specified string descriptor size */
            *str_desc_size = (uint16_t)(str_desc->StringDescriptorInfo)[USBF_DESC_LENGTH];

            /* normalitily end */
            ret = TRUE;

            /* skip from the for loop */
            break;
        }
        /* get next string descriptor information pointer */
        str_desc++;
    }

    return(ret);
}

/**
* @brief            get remote wakeup valid flag
*
* @param            none
* @return           TRUE      remote wakeup function valid
* @return           FALSE     remote wakeup function invalid
*
**/
uint8_t usbfunc_descriptor_get_remotewakeup(void)
{
    return (s_remotewakeup_flag);
}

/**
* @brief            check device descriptor information
*
* @param            dev_desc_addr           pointer to device descriptor
* @param            ep0_pkt_size_addr       pointer to endpoint attribute
* @return           TRUE            normalitily end
* @return           FALSE           unnormalitily end
*
**/
static uint8_t judge_dev_desc(uint8_t* dev_desc_addr, uint8_t* ep0_pkt_size_addr)
{
    uint8_t ret = FALSE;         /* return value */

    /* when device descriptor pointer is not null */
    if (dev_desc_addr != NULL)
    {
        /* save pointer to device descriptor */
        s_dev_desc = dev_desc_addr;

        /* judge device descriptor size and type */
        if ((s_dev_desc[USBF_DESC_LENGTH] == USBF_DEV_DESC_LENGTH) &&
            (s_dev_desc[USBF_DESC_TYPE] == USBF_DEV_DESC_TYPE))
        {
            /* judge endpoint0 packet size */
            if (((s_dev_desc[USBF_DEV_MAXPACKETSIZE0]) == 0x08) ||
                ((s_dev_desc[USBF_DEV_MAXPACKETSIZE0]) == 0x10) ||
                ((s_dev_desc[USBF_DEV_MAXPACKETSIZE0]) == 0x20) ||
                ((s_dev_desc[USBF_DEV_MAXPACKETSIZE0]) == 0x40))
            {
                /* retrieve manufacturer string descriptor index */
                if (search_string_index(s_dev_desc[USBF_DEV_MANUFACTURER]) == TRUE)
                {
                    /* retrieve product string descriptor index */
                    if (search_string_index(s_dev_desc[USBF_DEV_PRODUCT]) == TRUE)
                    {
                        /* retrieve device name manufacturing number string descriptor index */
                        if (search_string_index(s_dev_desc[USBF_DEV_SERIALNUMBER]) == TRUE)
                        {
                            /* judge the number that possible to configuration */
                            if (s_dev_desc[USBF_DEV_NUMCONFIGURATIONS] == 1)
                            {
                                /* set return value */
                                ret = TRUE;
                                /* set endpoint0 packet size */
                                *ep0_pkt_size_addr = s_dev_desc[USBF_DEV_MAXPACKETSIZE0];
                            }
                        }
                    }
                }
            }
        }
    }

    return (ret);
}

/**
* @brief            check configration descriptor information
*
* @param            cfg_desc_addr      pointer to configration descriptor
* @param            ep_buf_info_ptr    pointer to endpoint buffer information
* @return           TRUE       normalitily end
* @return           FALSE      unnormalitily end
*
**/
static uint8_t judge_cfg_desc(uint8_t* cfg_desc_addr, ST_EP_BUF_INFO* ep_buf_info_ptr)
{
    USBF_UN_CFG_bmAttributes     attributes;       /* configuration attribute */
    uint8_t   itf_num;                             /* interface numbers in configuration */
    uint8_t   itf_loop_cnt;                        /* interface loop count */
    uint8_t*  itf_desc_ptr;                        /* interface descriptor pointer */
    uint16_t  itf_total_length;                    /* interface total length */
    uint8_t   itf_ret;                             /* interface descriptor check return value */
    uint16_t  cfg_total_length = 0;                /* configuration total length */
    uint16_t  actual_cfg_total_length = 0;         /* actually configuration length */
    uint8_t   ret = FALSE;                         /* return value */

    /* when pointer to configration descriptor is not null */
    if (cfg_desc_addr != NULL)
    {
        /* save configration descriptor */
        s_cfg_desc = cfg_desc_addr;

        /* judge configration descriptor size and type */
        if ((s_cfg_desc[USBF_DESC_LENGTH] == USBF_CFG_DESC_LENGTH) &&
            (s_cfg_desc[USBF_DESC_TYPE] == USBF_CFG_DESC_TYPE))
        {
            /* retrieve configuration string descriptor index */
            if (search_string_index(s_cfg_desc[USBF_CFG_CONFIGURATION])
                                                              == TRUE)
            {
                /* get configuration attribute */
                attributes.byte = s_cfg_desc[USBF_CFG_ATTRIBUTES];

                /* is device power self-power?*/
                if (attributes.bits.powered_type == USBF_SELF_POWERED)
                {
                    /* set remote wakeup valid flag */
                    s_remotewakeup_flag = attributes.bits.remote_wakeup;

                    /* get configuration total length from configration descriptor */
                    cfg_total_length = MAKEHWORD(s_cfg_desc[USBF_CFG_TOTALLENGTH + 1], s_cfg_desc[USBF_CFG_TOTALLENGTH]);

                    /* set actually configuration length */
                    actual_cfg_total_length = USBF_CFG_DESC_LENGTH;

                    /* get interface numbers in configuration */
                    itf_num = s_cfg_desc[USBF_CFG_NUMINTERFACES];

                    /* set interface descriptor pointer */
                    itf_desc_ptr = s_cfg_desc + USBF_CFG_DESC_LENGTH;

                    /* normalitily end and return true */
                    ret = TRUE;

                    /* check each interface descriptor in configuration  */
                    for (itf_loop_cnt = 0; itf_loop_cnt < itf_num; itf_loop_cnt++)
                    {
                        /* check interface descriptor information */
                        itf_ret = judge_itf_desc(itf_desc_ptr, &itf_total_length, ep_buf_info_ptr);
                        /* when check result is error,skip from the for loop */
                        if (itf_ret == FALSE)
                        {
                            /* unnormalitily end and return false */
                            ret = FALSE;

                            /* skip from the for loop */
                            break;
                        }
                        /* get the next interface descriptor storage  */
                        itf_desc_ptr += itf_total_length;

                        /* set actually configuration length */
                        actual_cfg_total_length += itf_total_length;

                        /* the used interface numbers increment */
                        s_used_itf_num++;
                    }
                }
            }
        }
    }

    /* when normalitily end,compares actually configuration length and configuration length */
    if (ret == TRUE)
    {
        if (actual_cfg_total_length != cfg_total_length)
        {
            /* when length is not identical, unnormalitily end and return false */
            ret = FALSE;
        }
    }

    return (ret);
}

/**
* @brief            check interface descriptor information
*
* @param            itf_desc_addr         pointer to interface descriptor
* @param            itf_total_length_addr pointer to interface total length
* @param            ep_buf_info_ptr       pointer to endpoint buffer
* @return           TRUE          normalitily end
* @return           FALSE         unnormalitily end
*
**/
static uint8_t judge_itf_desc(uint8_t* itf_desc_addr,
                    uint16_t* itf_total_length_addr,
                    ST_EP_BUF_INFO* ep_buf_info_ptr)
{
    uint8_t   itf_ep_num;            /* the endpoint numbers in interface  */
    uint8_t   ep_loop_cnt;           /* endpoint loop count */
    uint8_t*  ep_desc_ptr;           /* pointer to endpoint descriptor */
    uint8_t   ret = FALSE;           /* return value */

    /* judge descriptor size and type and interface index in configuration */

    if ((itf_desc_addr[USBF_DESC_LENGTH] == USBF_INF_DESC_LENGTH) &&
        (itf_desc_addr[USBF_DESC_TYPE] == USBF_INF_DESC_TYPE) &&
        (itf_desc_addr[USBF_ITF_INTERFACENUMBER] == s_used_itf_num))
    {
        /* get the endpoint numbers in interface */
        itf_ep_num = itf_desc_addr[USBF_ITF_NUMENDPOINTS];

        /* retrieve interface string descriptor index */
        if (search_string_index(itf_desc_addr[USBF_ITF_INTERFACE])
                                                    == TRUE)
        {
            /* set interface length */
            *itf_total_length_addr = (uint16_t)USBF_INF_DESC_LENGTH;

            /* FDDL 2010/01/08 add */
            if (itf_ep_num > 0)
            {
                /* set pointer to endpoint descriptor */
                ep_desc_ptr = itf_desc_addr + USBF_INF_DESC_LENGTH;

                /* search until found endpoint descriptor from interface descriptor */
                while (*(ep_desc_ptr + 1) != USBF_EPN_DESC_TYPE)
                {
                    ep_desc_ptr += *ep_desc_ptr;
                }
                /* update length of interface total length */
                *itf_total_length_addr = (uint16_t)(ep_desc_ptr - itf_desc_addr);
            }
            /* FDDL 2010/01/08 add end */

            /* return true */
            ret = TRUE;

            /*check each endpoint descriptor in interface  */
            for (ep_loop_cnt = 0; ep_loop_cnt < itf_ep_num; ep_loop_cnt++)
            {
                /* when the numbers which in using is not larger then the calculated numbers */
                if (ep_buf_info_ptr->used_ep_num < ep_buf_info_ptr->computed_ep_num)
                {
                    /* check endpoint descriptor information */
                    if (judge_ep_desc(ep_desc_ptr, ep_buf_info_ptr->ep_buf_addr) == TRUE)
                    {
                        /* set interface length */
                        *itf_total_length_addr += USBF_EPN_DESC_LENGTH;

                        /* get the next endpoint attribute buffer storage */
                        ep_buf_info_ptr->ep_buf_addr++;

                        /* set the next endpoint attribute buffer storage */
                        ep_desc_ptr += USBF_EPN_DESC_LENGTH;

                        /* the endpoint numbers which in using increment */
                        ep_buf_info_ptr->used_ep_num++;
                    }
                    else/* when check result is error,skip from the for loop */
                    {
                        /* unnormalitily end and return false */
                        ret = FALSE;

                        /* skip from the for loop */
                        break;
                    }
                }
                else
                {
                    /* unnormalitily end and return false */
                    ret = FALSE;

                    /* skip from the for loop */
                    break;
                }
            }
        }
    }

    return (ret);
}

/**
* @brief            check endpoint descriptor information
*
* @param            ep_desc_addr    endpoint descriptor storage
* @param            ep_buf_addr     endpoint attribute buffer storage
* @return           TRUE    normalitily end
* @return           FALSE   unnormalitily end
*
**/
static uint8_t judge_ep_desc(const uint8_t* ep_desc_addr, USBFUNC_ST_DATATRANS_EPINFO* ep_buf_addr)
{
    USBF_UN_EPN_bEndpointAddress  ep_address;    /* endpoint descriptor address */
    USBF_UN_EPN_bmAttributes      ep_attributes; /* endpoint descriptor attribute */
    USBF_UN_EPN_wMaxPacketSize    ep_maxpktsize; /* endpoint descriptor max packet size */
    uint8_t                       ret = FALSE;   /* return value */

    /* judge descriptor size and type */
    if ((ep_desc_addr[USBF_DESC_LENGTH] == USBF_EPN_DESC_LENGTH) &&
        (ep_desc_addr[USBF_DESC_TYPE] == USBF_EPN_DESC_TYPE))
    {
        /* get endpoint descriptor attribute */
        ep_attributes.byte = ep_desc_addr[USBF_EPN_ATTRIBUTES];

#if (CPU_TYPE == CPU_TYPE_16)
        /* when endpoint transfer type is not control transfer */
        if (ep_attributes.bits.transfer_type != 0)
#elif (CPU_TYPE == CPU_TYPE_32)
        /* when endpoint transfer type is not control transfer and isochronous transfer */
        if ((ep_attributes.bits.transfer_type != 0) && (ep_attributes.bits.transfer_type != 1))
#endif
        {
            /* get endpoint descriptor address */
            ep_address.byte = ep_desc_addr[USBF_EPN_ENDPOINTADDRESS];

            /* when endpoint number is in this USB fuction macro support range(0~USBFUNC_MAX_EP_NUM) */
            if ((0 < ep_address.bits.ep_num) && (ep_address.bits.ep_num <= USBF_MAX_EP_NUM))
            {
                /* is endpoint used? */
                if (s_used_ep_id_info != (s_used_ep_id_info | (0x0001U << (ep_address.bits.ep_num - 1))))
                {
                    /* get endpoint descriptor packet size maximum */
                    ep_maxpktsize.word = MAKEHWORD(ep_desc_addr[USBF_EPN_MAXPACKETSIZE + 1], ep_desc_addr[USBF_EPN_MAXPACKETSIZE]);
#if (USBF_USE_DMA == USBF_ENABLE)  /* use DMA */
                    /* when the automatic buffer transfer mode is used,is PKT in the endpoint bit0~2 setted? */
                    if ((ep_maxpktsize.bits.pkt_size & DMAE_EPN_PKTSIZE_MASK) == 0)
                    {
#endif /* (USBF_USE_DMA == USBF_ENABLE) */
                        /* judge endpoint descriptor packet size */
                        if (((ep_address.bits.ep_num == USBF_EP1) &&
                             (ep_maxpktsize.bits.pkt_size >= 0x0001) &&
                             (ep_maxpktsize.bits.pkt_size <= 0x0100)) ||
                            ((ep_address.bits.ep_num >= USBF_EP2) &&
                             (ep_maxpktsize.bits.pkt_size >= 0x0001) &&
                             (ep_maxpktsize.bits.pkt_size <= 0x0040)))
                        {
                            /* set endpoint number to endpoint attribute buffer */
                            ep_buf_addr->ep_id = ep_address.bits.ep_num;

                            /* set endpoint transfer derection to endpoint attribute buffer */
                            ep_buf_addr->ep_direction = ep_address.bits.direction;

                            /* set endpoint transfer type to endpoint attribute buffer */
                            ep_buf_addr->ep_type = ep_attributes.bits.transfer_type;

                            /* set endpoint packet size to endpoint attribute buffer */
                            ep_buf_addr->ep_pkt_size = ep_maxpktsize.bits.pkt_size;

                            /* add this endpoint to used endpoint number information */
                            s_used_ep_id_info = (s_used_ep_id_info | (0x0001U << (ep_address.bits.ep_num - 1)));

                            /* normalitily end and return true */
                            ret = TRUE;
                        }
#if (USBF_USE_DMA == USBF_ENABLE)  /* use DMA */
                    }
#endif /* (USBF_USE_DMA == USBF_ENABLE) */
                }
            }
        }
    }

    return (ret);
}

/**
* @brief            check string descriptor information
*
* @param            str_desc_ptr    pointer to string descriptor array
* @param            str_desc_num    string descriptor numbers which in using
* @return           TRUE    normalitily end
* @return           FALSE   unnormalitily end
*
**/
static uint8_t judge_str_desc (USBF_ST_STRING_DESCRIPTOR* str_desc_ptr, uint8_t str_desc_num)
{
    uint8_t   str_loop_cnt;              /* descriptor type */
    uint8_t   ret = TRUE;                /* return value */

    /* set string descriptor numbers which in using */
    s_str_desc_num = str_desc_num;

    /* when string descriptor numbers is not 0 */
    if (str_desc_num != 0)
    {
        if (str_desc_ptr != NULL)
        {
            /* save string descriptor array storage */
            s_str_desc = str_desc_ptr;

            /* check each string descriptior */
            for (str_loop_cnt = 0; str_loop_cnt < str_desc_num; str_loop_cnt++)
            {
                if ((str_desc_ptr->StringDescriptorInfo)[USBF_DESC_TYPE]
                                                      != USBF_STR_DESC_TYPE)
                {
                    /* unnormalitily end and return false */
                    ret = FALSE;

                    /* skip from the for loop */
                    break;
                }
                /* get the next string descriptor information pointer */
                str_desc_ptr++;
            }
        }
        else
        {
            /* unnormalitily end and return false */
            ret = FALSE;
        }
    }

    return (ret);
}

/**
* @brief            retrieval string descriptor information
*
* @param            index           string index
* @return           TRUE    normalitily end
* @return           FALSE   unnormalitily end
*
**/
static uint8_t search_string_index(uint8_t index_)
{
    USBF_ST_STRING_DESCRIPTOR *  str_desc_ptr;  /* pointer to string descriptor */
    uint8_t   str_loop_cnt;                     /* descriptor type */
    uint8_t   ret = FALSE;                      /* return value */

    /* set string descriptor information pointer */
    str_desc_ptr = s_str_desc;

    /* retrieve each string descriptor */
    for (str_loop_cnt = 0; str_loop_cnt < s_str_desc_num; str_loop_cnt++)
    {
        /* when passed index is existence */
        if ((str_desc_ptr->StringDescriptorIndex) == index_)
        {
            /* normalitily end and return true */
            ret = TRUE;

            /* skip from the for loop */
            break;
        }
        /* get the next string descriptor information pointer */
        str_desc_ptr++;
    }

    return (ret);
}
