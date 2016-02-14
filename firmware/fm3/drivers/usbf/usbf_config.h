/**
* @file          usbf_config.h
* @brief         user customs module configuration
* @author        huangshan
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/
#ifndef __USBF_CONFIG_INC__
#define __USBF_CONFIG_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Configuration  (variable)                                                  */
/*----------------------------------------------------------------------------*/

/* definition for user customs field  */
#define USBF_ENABLE                  (0x00)   /* use function   */
#define USBF_DISABLE                 (0x01)   /* don't use function */

#define USBF_USBCLK_PLL              (0x00)   /* PLL macro oscilliation clock */
#define USBF_USBCLK_MAIN             (0x01)   /* main clock        */

#define USBF_IO_LO_LEVEL             (0x00)   /* low level out */
#define USBF_IO_HI_LEVEL             (0x01)   /* high level out */

#define USBF_SEND_SPK_DRIVER_CONTROL (0x00)   /* SPK sending control,high-ranking layer don't control */
#define USBF_SEND_SPK_APP_CONTROL    (0x01)   /* SPK sending don't control,high-ranking layer control */

#define USBF_SEND_ZLP_DRIVER_CONTROL (0x00)   /* ZLP sending control,high-ranking layer don't control */
#define USBF_SEND_ZLP_APP_CONTROL    (0x01)   /* ZLP sending don't control,high-ranking layer control */

/******************************************************************************/
/* interrupt level                                                            */
/* possible value:  0~7(16LX family)                                          */
/*               :  16~31(FR family)                                          */
/*               :  0~15(FM family)                                          */
/******************************************************************************/
#define USBF_INTERRUPT_LEVEL       (12)

/******************************************************************************/
/* pull-up resistor control using set                                         */
/* possible value:                                                            */
/*                  USBF_ENABLE  -> pull-up resistor control                  */
/*                  USBF_DISABLE -> pull-up resistor don't control            */
/******************************************************************************/
#define USBF_PULLUP_CTRL   (USBF_ENABLE)

/******************************************************************************/
/* IO for pull-up resistor control                                            */
/* possible value:  GPIO_DDR00~GPIO_DDR0F                                     */
/*                  GPIO_DDR10~GPIO_DDR1F                                     */
/*                  GPIO_DDR20~GPIO_DDR2F                                     */
/*                  GPIO_DDR30~GPIO_DDR3F                                     */
/*                  GPIO_DDR40~GPIO_DDR4F                                     */
/*                  GPIO_DDR50~GPIO_DDR5F                                     */
/*                  GPIO_DDR60~GPIO_DDR6F                                     */
/*                  GPIO_DDR70~GPIO_DDR7F                                     */
/******************************************************************************/
#define USBF_PULLUP_IO_DDR         (bFM3_GPIO_DDR6_P1)     /* direction register of pull-up IO port */
#define USBF_PULLUP_IO_PDR         (bFM3_GPIO_PDOR6_P1)    /* data register of pull-up IO port      */

/******************************************************************************/
/* pull-up resistor connect set                                               */
/* possible value: (set depend on external circuit)                           */
/*                  USBF_IO_LO_LEVEL -> low level out                         */
/*                  USBF_IO_HI_LEVEL -> high level out                        */
/******************************************************************************/
#define USBF_PULLUP_CONNECT       (USBF_IO_LO_LEVEL)

/******************************************************************************/
/* VBUS terminal control using set                                            */
/* possible value:                                                            */
/*                  USBF_ENABLE  -> VBUS terminal control                     */
/*                  USBF_DISABLE -> VBUS terminal don't control               */
/******************************************************************************/
#define USBF_VBUS_CTRL   (USBF_ENABLE)

/******************************************************************************/
/* IO for VBUS status detect                                                  */
/* possible value:  GPIO_DDR00~GPIO_DDR0F                                     */
/*                  GPIO_DDR10~GPIO_DDR1F                                     */
/*                  GPIO_DDR20~GPIO_DDR2F                                     */
/*                  GPIO_DDR30~GPIO_DDR3F                                     */
/*                  GPIO_DDR40~GPIO_DDR4F                                     */
/*                  GPIO_DDR50~GPIO_DDR5F                                     */
/*                  GPIO_DDR60~GPIO_DDR6F                                     */
/*                  GPIO_DDR70~GPIO_DDR7F                                     */
/******************************************************************************/
#define USBF_VBUS_IO_DDR          (bFM3_GPIO_DDR6_P0)   /* direction register of VBUS IO port */
#define USBF_VBUS_IO_PDR          (bFM3_GPIO_PDIR6_P0)  /* data register of VBUS IO port      */

/******************************************************************************/
/* USB cleck select                                                           */
/* possible value:                                                            */
/*                  USBF_USBCLK_MAIN -> main clock                            */
/*                  (main clock must be 48MHz)                                */
/*                  USBF_USBCLK_PLL -> PLL clock/2                            */
/*                  (PLL macro oscilliation clock must be 96MHz)              */
/* only for FR family,can use possible value                                  */
/* for 16LX family,set value is invalid                                       */
/******************************************************************************/
#define USBF_USBCLK              (USBF_USBCLK_PLL)

/******************************************************************************/
/* vebter request function using set                                          */
/* possible value:                                                            */
/*                  USBF_ENABLE  -> use vebter request function               */
/*                  USBF_DISABLE -> don't use vebter request function         */
/******************************************************************************/
#define USBF_VENDOR_REQUEST                        (USBF_ENABLE)

/******************************************************************************/
/* calss request function using set                                           */
/* possible value:                                                            */
/*                  USBF_ENABLE  -> use calss request function                */
/*                  USBF_DISABLE -> don't calss request function              */
/******************************************************************************/
#define USBF_CLASS_REQUEST                         (USBF_ENABLE)

/******************************************************************************/
/* Set_Descriptorstandard request function using set                          */
/* possible value:                                                            */
/*                   USBF_ENABLE                                              */
/*                     -> use[Set_Descriptor]standard request function        */
/*                   USBF_DISABLE                                             */
/*                     -> don't use[Set_Descriptor]standard request function  */
/******************************************************************************/
#define USBF_STD_REQUEST_SETDESCRIPTOR             (USBF_ENABLE)

/******************************************************************************/
/* SynchFrame standard request function using set                             */
/* possible value:                                                            */
/*               USBF_ENABLE  -> use[SYNCHFRAME]function                      */
/*               USBF_DISABLE -> don't use[SYNCHFRAME]function                */
/******************************************************************************/
#define USBF_STD_REQUEST_SYNCHFRAME                (USBF_ENABLE)

/******************************************************************************/
/* DMA transfer function set                                                  */
/* possible value:                                                            */
/*                  USBF_ENABLE                                               */
/*                   -> use DMA transfer function                             */
/*                  USBF_DISABLE                                              */
/*                   -> don't use DMA transfer function                       */
/******************************************************************************/
#define USBF_USE_DMA                               (USBF_DISABLE)

/******************************************************************************/
/* when DMA automatic transfer is used,NULL packet automatic transfer set     */
/* possible value:                                                            */
/*                  USBF_ENABLE                                               */
/*                   -> set NULL packet automatic transfer                    */
/*                  USBF_DISABLE                                              */
/*                   -> don't set NULL packet automatic transfer              */
/******************************************************************************/
#define USBF_TRANS_NULL                            (USBF_DISABLE)

/******************************************************************************/
/* SOF detect notification functioni set                                      */
/* possible value:                                                            */
/*                  USBF_ENABLE                                               */
/*                   -> use SOF detect notification function                  */
/*                  USBF_DISABLE                                              */
/*                   -> don't use SOF detect notification function            */
/******************************************************************************/
//#define USBF_SOF_ENABLE                            (USBF_DISABLE)
#define USBF_SOF_ENABLE                            (USBF_ENABLE)

/******************************************************************************/
/* short packet data sending control set                                      */
/* possible value:                                                            */
/*                  USBF_SEND_SPK_DRIVER_CONTROL                              */
/*                   -> SPK sending control,high-ranking layer don't control  */
/*                  USBF_SEND_SPK_APP_CONTROL                                 */
/*                   -> SPK sending don't control,high-ranking layer control  */
/******************************************************************************/
#define USBF_SEND_SPK_CONTROL    (USBF_SEND_SPK_DRIVER_CONTROL)

/******************************************************************************/
/* 0 length data sending control set                                          */
/* possible value:                                                            */
/*               USBF_SEND_ZLP_DRIVER_CONTROL                                 */
/*                   -> ZLP sending control,high-ranking layer don't control  */
/*               USBF_SEND_ZLP_APP_CONTROL                                    */
/*                   -> ZLP sending don't control,high-ranking layer control  */
/******************************************************************************/
#define USBF_SEND_ZLP_CONTROL    (USBF_SEND_ZLP_APP_CONTROL)

/******************************************************************************/
/* EPn STALL auto clear set                                                   */
/* possible value:                                                            */
/*                  USBF_ENABLE                                               */
/*                   -> EPn STALL auto clear                                  */
/*                  USBF_DISABLE                                              */
/*                   -> EPn STALL isn't auto clear                            */
/******************************************************************************/
#define USBF_AUTO_CLEAR_STALL    (USBF_DISABLE)

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/

#endif
