/**
* @file          MB9BF500.h
* @brief         CMSIS Cortex-M3 Core Peripheral Access Layer Header File for
 *               FUJITSU MB9BFxxx Series
* @author        lijiale
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __MB9B500_H__
#define __MB9B500_H__

/**
 * @brief Interrupt Number Type Definition
 */
typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
    NonMaskableInt_IRQn           = -14,  /* 2 Non Maskable Interrupt                             */
    MemoryManagement_IRQn         = -12,  /* 4 Cortex-M3 Memory Management Interrupt              */
    BusFault_IRQn                 = -11,  /* 5 Cortex-M3 Bus Fault Interrupt                      */
    UsageFault_IRQn               = -10,  /* 6 Cortex-M3 Usage Fault Interrupt                    */
    SVCall_IRQn                   = -5,   /* 11 Cortex-M3 SV Call Interrupt                       */
    DebugMonitor_IRQn             = -4,   /* 12 Cortex-M3 Debug Monitor Interrupt                 */
    PendSV_IRQn                   = -2,   /* 14 Cortex-M3 Pend SV Interrupt                       */
    SysTick_IRQn                  = -1,   /* 15 Cortex-M3 System Tick Interrupt                   */

/******  MB9BFxxx Specific Interrupt Numbers ******************************************************/
    FCS_IRQn                      = 0,    /* FCS Interupt                                         */
    WDT_IRQn                      = 1,    /* Watchdog Interupt                                    */
    LVD_IRQn                      = 2,    /* LVD Interupt                                         */
    MFT_IRQn                      = 3,    /* MFT Interupt                                         */
    EXT0_7_IRQn                   = 4,    /* EXT Interupt 0-7                                    */
    EXT8_15_IRQn                  = 5,    /* EXT Interupt 8-15                                   */
    QPRC_IRQn                     = 6,    /* QPRC Interupt Handler                                */
    MSI0_RX_IRQn                  = 7,    /* Multi-function Serial Interface receive Interupt 0   */
    MSI0_TX_IRQn                  = 8,    /* Multi-function Serial Interface send Interupt 0      */
    MSI1_RX_IRQn                  = 9,    /* Multi-function Serial Interface receive Interupt 1   */
    MSI1_TX_IRQn                  = 10,   /* Multi-function Serial Interface send Interupt 1      */
    MSI2_RX_IRQn                  = 11,   /* Multi-function Serial Interface receive Interupt 2   */
    MSI2_TX_IRQn                  = 12,   /* Multi-function Serial Interface send Interupt 2      */
    MSI3_RX_IRQn                  = 13,   /* Multi-function Serial Interface receive Interupt 3   */
    MSI3_TX_IRQn                  = 14,   /* Multi-function Serial Interface send Interupt 3      */
    MSI4_RX_IRQn                  = 15,   /* Multi-function Serial Interface receive Interupt 4   */
    MSI4_TX_IRQn                  = 16,   /* Multi-function Serial Interface send Interupt 4      */
    MSI5_RX_IRQn                  = 17,   /* Multi-function Serial Interface receive Interupt 5   */
    MSI5_TX_IRQn                  = 18,   /* Multi-function Serial Interface send Interupt 5      */
    MSI6_RX_IRQn                  = 19,   /* Multi-function Serial Interface receive Interupt 6   */
    MSI6_TX_IRQn                  = 20,   /* Multi-function Serial Interface send Interupt 6      */
    MSI7_RX_IRQn                  = 21,   /* Multi-function Serial Interface receive Interupt 7   */
    MSI7_TX_IRQn                  = 22,   /* Multi-function Serial Interface send Interupt 7      */
    PPG_IRQn                      = 23,   /* PPG Interupt                                         */
    OSC_IRQn                      = 24,   /* OSC Interupt                                         */
    AD0_IRQn                      = 25,   /* AD0 Interupt                                         */
    AD1_IRQn                      = 26,   /* AD1 Interupt                                         */
    AD2_IRQn                      = 27,   /* AD2 Interupt                                         */
    MFT_FRT_IRQn                  = 28,   /* MFT Free-Run Timer Interupt                          */
    MFT_IC_IRQn                   = 29,   /* MFT Input Capture Interupt                           */
    MFT_OC_IRQn                   = 30,   /* MFT Output Compare Interupt                          */
    BT_IRQn                       = 31,   /* Base Timer Interupt                                  */
    CAN0_IRQn                     = 32,   /* CAN0 Interupt                                        */
    CAN1_IRQn                     = 33,   /* CAN1 Interupt                                        */
    USB1_IRQn                     = 34,   /* USB function(DRQ of End Point 1 to 5) Interupt       */
    USB2_IRQn                     = 35,   /* USB function(DRQI, DRQO of End Point 0 and each status) and USB HOST (Each status) Interupt */
    DMA0_IRQn                     = 38,   /* DMA0 Interupt                                        */
    DMA1_IRQn                     = 39,   /* DMA1 Interupt                                        */
    DMA2_IRQn                     = 40,   /* DMA2 Interupt                                        */
    DMA3_IRQn                     = 41,   /* DMA3 Interupt                                        */
    DMA4_IRQn                     = 42,   /* DMA4 Interupt                                        */
    DMA5_IRQn                     = 43,   /* DMA5 Interupt                                        */
    DMA6_IRQn                     = 44,   /* DMA6 Interupt                                        */
    DMA7_IRQn                     = 45    /* DMA7 Interupt                                        */
} IRQn_Type;


/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/
/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __MPU_PRESENT             1         /* MPU present or not                               */
#define __NVIC_PRIO_BITS          4         /* Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /* Set to 1 if different SysTick Config is used     */

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "core_cm3.h"                       /* Cortex-M3 processor and core peripherals         */

/*----------------------------------------------------------------------------*/
/*  Struct Definition                                                         */
/*----------------------------------------------------------------------------*/
/* struct of clock generation register */
typedef union {
    uint8_t  byte;
    struct {
                 uint8_t  RESV2:1;
            __IO uint8_t  MOSCE:1;
                 uint8_t  RESV1:1;
            __IO uint8_t  SOSCE:1;
            __IO uint8_t  PLLE :1;
            __IO uint8_t  RCS  :3;
    } bit;
} SCM_CTL;

typedef union {
    uint8_t  byte;
    struct {
                 uint8_t  RESV2:1;
             __I uint8_t  MORDY:1;
                 uint8_t  RESV1:1;
             __I uint8_t  SORDY:1;
             __I uint8_t  PLRDY:1;
             __I uint8_t  RCM  :3;
    } bit;
} SCM_STR;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  BSR  :3;
                 uint8_t  RESV1:5;
    } bit;
} BSC_PSR;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  APBC0:2;
                 uint8_t  RESV1:6;
    } bit;
} APBC0_PSR;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  APBC1   :2;
                 uint8_t  RESV2   :2;
            __IO uint8_t  APBC1RST:1;
                 uint8_t  RESV1   :2;
            __IO uint8_t  APBC1EN :1;
    } bit;
} APBC1_PSR;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  APBC2   :2;
                 uint8_t  RESV2   :2;
            __IO uint8_t  APBC2RST:1;
                 uint8_t  RESV1   :2;
            __IO uint8_t  APBC2EN :1;
    } bit;
} APBC2_PSR;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  SWDS  :2;
                 uint8_t  RESV1 :5;
            __IO uint8_t  TESTB :1;
    } bit;
} SWC_PSR;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  TTC  :1;
                 uint8_t  RESV1:7;
    } bit;
} TTC_PSR;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  MOWT :4;
            __IO uint8_t  SOWT :3;
                 uint8_t  RESV1:1;
    } bit;
} CSW_TMR;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  PWOT :3;
                 uint8_t  RESV2:1;
            __IO uint8_t  PINC :1;
                 uint8_t  RESV1:3;
    } bit;
} PSW_TMR;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  PLLM:4;
            __IO uint8_t  PLLK:4;
    } bit;
} PLL_CTL1;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  PLLN:5;
                 uint8_t  RESV1:3;
    } bit;
} PLL_CTL2;

typedef union {
    uint8_t  byte;
    struct {
                 uint8_t  RESV2 :5;
            __IO uint8_t  DPSWBE:1;
                 uint8_t  RESV1 :1;
            __IO uint8_t  DPHWBE:1;
    } bit;
} DBWDT_CTL;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  MCSE :1;
            __IO uint8_t  SCSE :1;
            __IO uint8_t  PCSE :1;
                 uint8_t  RESV2:2;
            __IO uint8_t  FCSE :1;
                 uint8_t  RESV1:2;
    } bit;
} INT_ENR;

typedef union {
    uint8_t  byte;
    struct {
             __I uint8_t  MCSI :1;
             __I uint8_t  SCSI :1;
             __I uint8_t  PCSI :1;
                 uint8_t  RESV2:2;
             __I uint8_t  FCSI :1;
                 uint8_t  RESV1:2;
    } bit;
} INT_STR;

typedef union {
    uint8_t  byte;
    struct {
             __O uint8_t  MCSC :1;
             __O uint8_t  SCSC :1;
             __O uint8_t  PCSC :1;
                 uint8_t  RESV2:2;
             __O uint8_t  FCSC :1;
                 uint8_t  RESV1:2;
    } bit;
} INT_CLR;

/* struct of USB clock generation register */
typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  UCEN :1;
            __IO uint8_t  UCSEL:1;
                 uint8_t  RESV1:6;
    } bit;
} UCCR;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  UPLLEN:1;
            __IO uint8_t  UPINC :1;
                 uint8_t  RESV1 :6;
    } bit;
} UPCR1;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  UPOWT:3;
                 uint8_t  RESV1:5;
    } bit;
} UPCR2;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  UPLLK:5;
                 uint8_t  RESV1:3;
    } bit;
} UPCR3;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  UPLLN:5;
                 uint8_t  RESV1:3;
    } bit;
} UPCR4;

typedef union {
    uint8_t  byte;
    struct {
             __I uint8_t  UPRDY:1;
                 uint8_t  RESV1:7;
    } bit;
} UP_STR;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  UPCSE:1;
                 uint8_t  RESV1:7;
    } bit;
} UPINT_ENR;

typedef union {
    uint8_t  byte;
    struct {
             __I uint8_t  UPCSI:1;
                 uint8_t  RESV1:7;
    } bit;
} UPINT_STR;

typedef union {
    uint8_t  byte;
    struct {
             __O uint8_t  UPCSC:1;
                 uint8_t  RESV1:7;
    } bit;
} UPINT_CLR;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  USBEN:1;
                 uint8_t  RESV1:7;
    } bit;
} USBEN;


/* USB function */
typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  PWC:1;
            __IO uint8_t  RFBK:1;
                 uint8_t  RESV2:1;
            __IO uint8_t  STALCLREN:1;
            __IO uint8_t  USTP:1;
            __IO uint8_t  HCONX:1;
            __IO uint8_t  RESUM:1;
            __IO uint8_t  RST:1;
                 uint8_t  RESV1:8;
    } bit;
} UDCC;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  PKS0:7;
                 uint8_t  RESV8:1;
                 uint8_t  RESV7:1;
            __IO uint8_t  STAL:1;
                 uint8_t  RESV5:1;
                 uint8_t  RESV4:1;
                 uint8_t  RESV3:1;
                 uint8_t  RESV2:1;
                 uint8_t  RESV1:1;
                 uint8_t  RESV0:1;
    } bit;
} EP0C;

typedef union {
    uint16_t hword;
    struct {
            __IO uint16_t PKS1:9;
            __IO uint8_t  STAL:1;
            __IO uint8_t  NULE:1;
            __IO uint8_t  DMAE:1;
            __IO uint8_t  DIR:1;
            __IO uint8_t  TYPE:2;
            __IO uint8_t  EPEN:1;
    } bit;
} EP1C;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  PKS2:7;
                 uint8_t  RESV7:1;
                 uint8_t  RESV6:1;
            __IO uint8_t  STAL:1;
            __IO uint8_t  NULE:1;
            __IO uint8_t  DMAE:1;
            __IO uint8_t  DIR:1;
            __IO uint8_t  TYPE:2;
            __IO uint8_t  EPEN:1;
    } bit;
} EP2C,EP3C,EP4C,EP5C;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  CONF:1;
            __IO uint8_t  SETP:1;
            __IO uint8_t  WKUP:1;
            __IO uint8_t  BRST:1;
            __IO uint8_t  SOF:1;
            __IO uint8_t  SUSP:1;
                 uint8_t  RESV1:1;
                 uint8_t  RESV0:1;
    } bit;
} UDCS;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  CONFIE:1;
            __I  uint8_t  CONFN:1;
            __IO uint8_t  WKUPIE:1;
            __IO uint8_t  BRSTIE:1;
            __IO uint8_t  SOFIE:1;
            __IO uint8_t  SUSPIE:1;
                 uint8_t  RESV1:1;
                 uint8_t  RESV0:1;
    } bit;
} UDCIE;

typedef union {
    uint16_t hword;
    struct {
                 uint8_t  RESV15:1;
                 uint8_t  RESV14:1;
                 uint8_t  RESV13:1;
                 uint8_t  RESV12:1;
                 uint8_t  RESV11:1;
                 uint8_t  RESV10:1;
                 uint8_t  RESV9:1;
                 uint8_t  RESV8:1;
                 uint8_t  RESV7:1;
                 uint8_t  RESV6:1;
            __IO uint8_t  DRQI:1;
                 uint8_t  RESV4:1;
                 uint8_t  RESV3:1;
                 uint8_t  RESV2:1;
            __IO uint8_t  DRQIIE:1;
            __IO uint8_t  BFINI:1;
    } bit;
} EP0IS;

typedef union {
    uint16_t hword;
    struct {
            __I  uint8_t  SIZE:7;
                 uint8_t  RESV8:1;
                 uint8_t  RESV7:1;
            __IO uint8_t  SPK:1;
            __IO uint8_t  DRQO:1;
                 uint8_t  RESV4:1;
                 uint8_t  RESV3:1;
            __IO uint8_t  SPKIE:1;
            __IO uint8_t  DRQOIE:1;
            __IO uint8_t  BFINI:1;
    } bit;
} EP0OS;

typedef union {
    uint16_t hword;
    struct {
            __I  uint16_t SIZE:9;
            __IO uint8_t  SPK:1;
            __IO uint8_t  DRQ:1;
            __I  uint8_t  BUSY:1;
                 uint8_t  RESV3:1;
            __IO uint8_t  SPKIE:1;
            __IO uint8_t  DRQIE:1;
            __IO uint8_t  BFINI:1;
    } bit;
} EP1S;

typedef union {
    uint16_t hword;
    struct {
            __I  uint8_t  SIZE:7;
                 uint8_t  RESV8:1;
                 uint8_t  RESV7:1;
            __IO uint8_t  SPK:1;
            __IO uint8_t  DRQ:1;
            __I  uint8_t  BUSY:1;
                 uint8_t  RESV3:1;
            __IO uint8_t  SPKIE:1;
            __IO uint8_t  DRQIE:1;
            __IO uint8_t  BFINI:1;
    } bit;
} EP2S,EP3S,EP4S,EP5S;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  EP0DTL;
            __IO uint8_t  EP0DTH;
    } byte;
} EP0DT;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  EP1DTL;
            __IO uint8_t  EP1DTH;
    } byte;
} EP1DT;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  EP2DTL;
            __IO uint8_t  EP2DTH;
    } byte;
} EP2DT;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  EP3DTL;
            __IO uint8_t  EP3DTH;
    } byte;
} EP3DT;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  EP4DTL;
            __IO uint8_t  EP4DTH;
    } byte;
} EP4DT;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  EP5DTL;
            __IO uint8_t  EP5DTH;
    } byte;
} EP5DT;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  HOST:1;
            __IO uint8_t  URST:1;
            __IO uint8_t  SOFIRE:1;
            __IO uint8_t  DIRE:1;
            __IO uint8_t  CNNIRE:1;
            __IO uint8_t  CMPIRE:1;
            __IO uint8_t  URIRE:1;
            __IO uint8_t  RWKIRE:1;
            __IO uint8_t  RETRY:1;
            __IO uint8_t  CANCEL:1;
            __IO uint8_t  SOFSTEP:1;
            __IO uint8_t  RESV4:1;
            __IO uint8_t  RESV3:1;
            __IO uint8_t  RESV2:1;
            __IO uint8_t  RESV1:1;
            __IO uint8_t  RESV0:1;
    } bit;
} HCNT;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  SOFIRQ:1;
            __IO uint8_t  DIRQ:1;
            __IO uint8_t  CNNIRQ:1;
            __IO uint8_t  CMPIRQ:1;
            __IO uint8_t  URIRQ:1;
            __IO uint8_t  RWKIRQ:1;
            __IO uint8_t  RESV1:1;
            __IO uint8_t  TCAN:1;
    } bit;
} HIRQ;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  HS:2;
            __IO uint8_t  STUFF:1;
            __IO uint8_t  TGERR:1;
            __IO uint8_t  CRC:1;
            __IO uint8_t  TOUT:1;
            __IO uint8_t  RERR:1;
            __IO uint8_t  LSTSOF:1;
    } bit;
} HERR;

typedef union {
    uint8_t  byte;
    struct {
            __I  uint8_t  CSTAT:1;
            __I  uint8_t  TMODE:1;
            __IO uint8_t  SUSP:1;
            __IO uint8_t  SOFBUSY:1;
            __IO uint8_t  CLKSEL:1;
            __IO uint8_t  ALIVE:1;
                 uint8_t  RESV1:1;
                 uint8_t  RESV0:1;
    } bit;
} HSTATE;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  EOF0:8;
            __IO uint8_t  EOF1:6;
                 uint8_t  RESV1:1;
                 uint8_t  RESV0:1;
    } bit;
} HEOF;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  FRAME0:8;
            __IO uint8_t  FRAME1:3;
                 uint8_t  RESV4:1;
                 uint8_t  RESV3:1;
                 uint8_t  RESV2:1;
                 uint8_t  RESV1:1;
                 uint8_t  RESV0:1;
    } bit;
} HFRAME;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  ENDPT:4;
            __IO uint8_t  TKNEN:3;
            __IO uint8_t  TGGL:1;
    } bit;
} HTOKEN;


/* struct of DMA control register */
typedef union {
    uint32_t  word;
    struct {
            __IO uint32_t  RESV2:24;
            __IO uint8_t  DH:4;
            __IO uint8_t  PR:1;
            __IO uint8_t  RESV1:1;
            __IO uint8_t  DS:1;
            __IO uint8_t  DE:1;
    } bit;
} DMACR;

typedef union {
    uint32_t  word;
    struct {
            __IO uint16_t TC:16;
            __IO uint8_t  BC:4;
            __IO uint8_t  RESV1:3;
            __IO uint8_t  IS2:1;
            __IO uint8_t  IS1:5;
            __IO uint8_t  ST:1;
            __IO uint8_t  PB:1;
            __IO uint8_t  EB:1;
    } bit;
} DMACA0,DMACA1;

typedef union {
    uint32_t  word;
    struct {
            __IO uint8_t  EM:1;
            __IO uint8_t  RESV2:7;
            __IO uint8_t  DP:4;
            __IO uint8_t  SP:4;
            __IO uint8_t  SS:3;
            __IO uint8_t  CI:1;
            __IO uint8_t  EI:1;
            __IO uint8_t  RD:1;
            __IO uint8_t  RS:1;
            __IO uint8_t  RC:1;
            __IO uint8_t  FD:1;
            __IO uint8_t  FS:1;
            __IO uint8_t  TW:2;
            __IO uint8_t  MS:2;
            __IO uint8_t  RESV1:2;
    } bit;
} DMACB0,DMACB1;


/* struct of I/O port control register */
typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  P00:1;
            __IO uint8_t  P01:1;
            __IO uint8_t  P02:1;
            __IO uint8_t  P03:1;
            __IO uint8_t  P04:1;
            __IO uint8_t  P05:1;
            __IO uint8_t  P06:1;
            __IO uint8_t  P07:1;
            __IO uint8_t  P08:1;
            __IO uint8_t  P09:1;
            __IO uint8_t  P0A:1;
            __IO uint8_t  P0B:1;
            __IO uint8_t  P0C:1;
            __IO uint8_t  P0D:1;
            __IO uint8_t  P0E:1;
            __IO uint8_t  P0F:1;
    } bit;
} PFR0,PCR0,DDR0,PDIR0,PDOR0;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  P10:1;
            __IO uint8_t  P11:1;
            __IO uint8_t  P12:1;
            __IO uint8_t  P13:1;
            __IO uint8_t  P14:1;
            __IO uint8_t  P15:1;
            __IO uint8_t  P16:1;
            __IO uint8_t  P17:1;
            __IO uint8_t  P18:1;
            __IO uint8_t  P19:1;
            __IO uint8_t  P1A:1;
            __IO uint8_t  P1B:1;
            __IO uint8_t  P1C:1;
            __IO uint8_t  P1D:1;
            __IO uint8_t  P1E:1;
            __IO uint8_t  P1F:1;
    } bit;
} PFR1,PCR1,DDR1,PDIR1,PDOR1,ADE;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  P20:1;
            __IO uint8_t  P21:1;
            __IO uint8_t  P22:1;
            __IO uint8_t  P23:1;
            __IO uint8_t  P24:1;
            __IO uint8_t  P25:1;
            __IO uint8_t  P26:1;
            __IO uint8_t  P27:1;
            __IO uint8_t  P28:1;
            __IO uint8_t  P29:1;
            __IO uint8_t  P2A:1;
            __IO uint8_t  P2B:1;
            __IO uint8_t  P2C:1;
            __IO uint8_t  P2D:1;
            __IO uint8_t  P2E:1;
            __IO uint8_t  P2F:1;
    } bit;
} PFR2,PCR2,DDR2,PDIR2,PDOR2;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  P30:1;
            __IO uint8_t  P31:1;
            __IO uint8_t  P32:1;
            __IO uint8_t  P33:1;
            __IO uint8_t  P34:1;
            __IO uint8_t  P35:1;
            __IO uint8_t  P36:1;
            __IO uint8_t  P37:1;
            __IO uint8_t  P38:1;
            __IO uint8_t  P39:1;
            __IO uint8_t  P3A:1;
            __IO uint8_t  P3B:1;
            __IO uint8_t  P3C:1;
            __IO uint8_t  P3D:1;
            __IO uint8_t  P3E:1;
            __IO uint8_t  P3F:1;
    } bit;
} PFR3,PCR3,DDR3,PDIR3,PDOR3;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  P40:1;
            __IO uint8_t  P41:1;
            __IO uint8_t  P42:1;
            __IO uint8_t  P43:1;
            __IO uint8_t  P44:1;
            __IO uint8_t  P45:1;
            __IO uint8_t  P46:1;
            __IO uint8_t  P47:1;
            __IO uint8_t  P48:1;
            __IO uint8_t  P49:1;
            __IO uint8_t  P4A:1;
            __IO uint8_t  P4B:1;
            __IO uint8_t  P4C:1;
            __IO uint8_t  P4D:1;
            __IO uint8_t  P4E:1;
            __IO uint8_t  P4F:1;
    } bit;
} PFR4,PCR4,DDR4,PDIR4,PDOR4;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  P50:1;
            __IO uint8_t  P51:1;
            __IO uint8_t  P52:1;
            __IO uint8_t  P53:1;
            __IO uint8_t  P54:1;
            __IO uint8_t  P55:1;
            __IO uint8_t  P56:1;
            __IO uint8_t  P57:1;
            __IO uint8_t  P58:1;
            __IO uint8_t  P59:1;
            __IO uint8_t  P5A:1;
            __IO uint8_t  P5B:1;
            __IO uint8_t  P5C:1;
            __IO uint8_t  P5D:1;
            __IO uint8_t  P5E:1;
            __IO uint8_t  P5F:1;
    } bit;
} PFR5,PCR5,DDR5,PDIR5,PDOR5;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  P60:1;
            __IO uint8_t  P61:1;
            __IO uint8_t  P62:1;
            __IO uint8_t  P63:1;
            __IO uint8_t  P64:1;
            __IO uint8_t  P65:1;
            __IO uint8_t  P66:1;
            __IO uint8_t  P67:1;
            __IO uint8_t  P68:1;
            __IO uint8_t  P69:1;
            __IO uint8_t  P6A:1;
            __IO uint8_t  P6B:1;
            __IO uint8_t  P6C:1;
            __IO uint8_t  P6D:1;
            __IO uint8_t  P6E:1;
            __IO uint8_t  P6F:1;
    } bit;
} PFR6,PCR6,DDR6,PDIR6,PDOR6;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  P70:1;
            __IO uint8_t  P71:1;
            __IO uint8_t  P72:1;
            __IO uint8_t  P73:1;
            __IO uint8_t  P74:1;
            __IO uint8_t  P75:1;
            __IO uint8_t  P76:1;
            __IO uint8_t  P77:1;
            __IO uint8_t  P78:1;
            __IO uint8_t  P79:1;
            __IO uint8_t  P7A:1;
            __IO uint8_t  P7B:1;
            __IO uint8_t  P7C:1;
            __IO uint8_t  P7D:1;
            __IO uint8_t  P7E:1;
            __IO uint8_t  P7F:1;
    } bit;
} PFR7,PCR7,DDR7,PDIR7,PDOR7;

typedef union {
    uint32_t  word;
    struct {
            __IO uint8_t  NMIS:1;
            __IO uint8_t  CROUTE:1;
                 uint8_t  RESV5:6;
                 uint8_t  RESV4:1;
            __IO uint8_t  USB0PE:1;
                 uint8_t  RESV3:6;
            __IO uint8_t  JTAGEN0B:1;
            __IO uint8_t  JTAGEN1S:1;
                 uint8_t  RESV2:6;
            __IO uint8_t  TRC0E:1;
            __IO uint8_t  TRC1E:1;
                 uint8_t  RESV1:6;
    } bit;
} EPFR00;

typedef union {
    uint32_t  word;
    struct {
            __IO uint8_t  RTO00E:2;
            __IO uint8_t  RTO01E:2;
            __IO uint8_t  RTO02E:2;
            __IO uint8_t  RTO03E:2;
            __IO uint8_t  RTO04E:2;
            __IO uint8_t  RTO05E:2;
            __IO uint8_t  DTTI0C:1;
                 uint8_t  RESV1:3;
            __IO uint8_t  DTTI0S:2;
            __IO uint8_t  FRCK0S:2;
            __IO uint8_t  IC00S:3;
            __IO uint8_t  IC01S:3;
            __IO uint8_t  IC02S:3;
            __IO uint8_t  IC03S:3;
    } bit;
} EPFR01;

typedef union {
    uint32_t  word;
    struct {
            __IO uint8_t  RTO10E:2;
            __IO uint8_t  RTO11E:2;
            __IO uint8_t  RTO12E:2;
            __IO uint8_t  RTO13E:2;
            __IO uint8_t  RTO14E:2;
            __IO uint8_t  RTO15E:2;
            __IO uint8_t  DTTI1C:1;
                 uint8_t  RESV1:3;
            __IO uint8_t  DTTI1S:2;
            __IO uint8_t  FRCK1S:2;
            __IO uint8_t  IC10S:3;
            __IO uint8_t  IC11S:3;
            __IO uint8_t  IC12S:3;
            __IO uint8_t  IC13S:3;
    } bit;
} EPFR02;

typedef union {
    uint32_t  word;
    struct {
                 uint8_t  RESV6:2;
            __IO uint8_t  TIOA0E:2;
            __IO uint8_t  TIOB0S:2;
                 uint8_t  RESV5:2;
            __IO uint8_t  TIOA1S:2;
            __IO uint8_t  TIOA1E:2;
            __IO uint8_t  TIOB1S:2;
                 uint8_t  RESV4:2;
                 uint8_t  RESV3:2;
            __IO uint8_t  TIOA2E:2;
            __IO uint8_t  TIOB2S:2;
                 uint8_t  RESV2:2;
            __IO uint8_t  TIOA3S:2;
            __IO uint8_t  TIOA3E:2;
            __IO uint8_t  TIOB3S:2;
                 uint8_t  RESV1:2;
    } bit;
} EPFR04;

typedef union {
    uint32_t  word;
    struct {
                 uint8_t  RESV6:2;
            __IO uint8_t  TIOA4E:2;
            __IO uint8_t  TIOB4S:2;
                 uint8_t  RESV5:2;
            __IO uint8_t  TIOA5S:2;
            __IO uint8_t  TIOA5E:2;
            __IO uint8_t  TIOB5S:2;
                 uint8_t  RESV4:2;
                 uint8_t  RESV3:2;
            __IO uint8_t  TIOA6E:2;
            __IO uint8_t  TIOB6S:2;
                 uint8_t  RESV2:2;
            __IO uint8_t  TIOA7S:2;
            __IO uint8_t  TIOA7E:2;
            __IO uint8_t  TIOB7S:2;
                 uint8_t  RESV1:2;
    } bit;
} EPFR05;

typedef union {
    uint32_t  word;
    struct {
            __IO uint8_t  EINT00S:2;
            __IO uint8_t  EINT01S:2;
            __IO uint8_t  EINT02S:2;
            __IO uint8_t  EINT03S:2;
            __IO uint8_t  EINT04S:2;
            __IO uint8_t  EINT05S:2;
            __IO uint8_t  EINT06S:2;
            __IO uint8_t  EINT07S:2;
            __IO uint8_t  EINT08S:2;
            __IO uint8_t  EINT09S:2;
            __IO uint8_t  EINT10S:2;
            __IO uint8_t  EINT11S:2;
            __IO uint8_t  EINT12S:2;
            __IO uint8_t  EINT13S:2;
            __IO uint8_t  EINT14S:2;
            __IO uint8_t  EINT15S:2;
    } bit;
} EPFR06;

typedef union {
    uint32_t  word;
    struct {
                 uint8_t  RESV2:4;
            __IO uint8_t  SIN0S:2;
            __IO uint8_t  SOT0B:2;
            __IO uint8_t  SCK0B:2;
            __IO uint8_t  SIN1S:2;
            __IO uint8_t  SOT1B:2;
            __IO uint8_t  SCK1B:2;
            __IO uint8_t  SIN2S:2;
            __IO uint8_t  SOT2B:2;
            __IO uint8_t  SCK2B:2;
            __IO uint8_t  SIN3S:2;
            __IO uint8_t  SOT3B:2;
            __IO uint8_t  SCK3B:2;
                 uint8_t  RESV1:4;
    } bit;
} EPFR07;

typedef union {
    uint32_t  word;
    struct {
            __IO uint8_t  RTS4E:2;
            __IO uint8_t  CTS4S:2;
            __IO uint8_t  SIN4S:2;
            __IO uint8_t  SOT4B:2;
            __IO uint8_t  SCK4B:2;
            __IO uint8_t  SIN5S:2;
            __IO uint8_t  SOT5B:2;
            __IO uint8_t  SCK5B:2;
            __IO uint8_t  SIN6S:2;
            __IO uint8_t  SOT6B:2;
            __IO uint8_t  SCK6B:2;
            __IO uint8_t  SIN7S:2;
            __IO uint8_t  SOT7B:2;
            __IO uint8_t  SCK7B:2;
                 uint8_t  RESV1:4;
    } bit;
} EPFR08;

typedef union {
    uint32_t  word;
    struct {
            __IO uint8_t  QAIN0S:2;
            __IO uint8_t  QBIN0S:2;
            __IO uint8_t  QZIN0S:2;
            __IO uint8_t  QAIN1S:2;
            __IO uint8_t  QBIN1S:2;
            __IO uint8_t  QZIN1S:2;
            __IO uint8_t  ADTRG0S:4;
            __IO uint8_t  ADTRG1S:4;
            __IO uint8_t  ADTRG2S:4;
            __IO uint8_t  CRX0S:2;
            __IO uint8_t  CTX0E:2;
            __IO uint8_t  CRX1S:2;
            __IO uint8_t  CTX1E:2;
    } bit;
} EPFR09;

typedef union {
    uint32_t  word;
    struct {
            __IO uint8_t  UEDEFB:1;
            __IO uint8_t  UEDTHB:1;
            __IO uint8_t  TESTB:1;
            __IO uint8_t  UEWEXE:1;
            __IO uint8_t  UEDQME:1;
            __IO uint8_t  UEOEXE:1;
            __IO uint8_t  UEFLSE:1;
            __IO uint8_t  UECS1E:1;
            __IO uint8_t  UECS2E:1;
            __IO uint8_t  UECS3E:1;
            __IO uint8_t  UECS4E:1;
            __IO uint8_t  UECS5E:1;
            __IO uint8_t  UECS6E:1;
            __IO uint8_t  UECS7E:1;
            __IO uint8_t  UEAOOE:1;
            __IO uint8_t  UEA08E:1;
            __IO uint8_t  UEA09E:1;
            __IO uint8_t  UEA10E:1;
            __IO uint8_t  UEA11E:1;
            __IO uint8_t  UEA12E:1;
            __IO uint8_t  UEA13E:1;
            __IO uint8_t  UEA14E:1;
            __IO uint8_t  UEA15E:1;
            __IO uint8_t  UEA16E:1;
            __IO uint8_t  UEA17E:1;
            __IO uint8_t  UEA18E:1;
            __IO uint8_t  UEA19E:1;
            __IO uint8_t  UEA20E:1;
            __IO uint8_t  UEA21E:1;
            __IO uint8_t  UEA22E:1;
            __IO uint8_t  UEA23E:1;
            __IO uint8_t  UEA24E:1;
    } bit;
} EPFR10;


/* struct of base timer register */
typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  SEL01:4;
            __IO uint8_t  SEL23:4;
    } bit;
} BTSEL0123;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  SEL45:4;
            __IO uint8_t  SEL67:4;
    } bit;
} BTSEL4567;

typedef union {
    uint16_t hword;
    struct {
            __O  uint8_t  SSSR0:1;
            __O  uint8_t  SSSR1:1;
            __O  uint8_t  SSSR2:1;
            __O  uint8_t  SSSR3:1;
            __O  uint8_t  SSSR4:1;
            __O  uint8_t  SSSR5:1;
            __O  uint8_t  SSSR6:1;
            __O  uint8_t  SSSR7:1;
            __O  uint8_t  SSSR8:1;
            __O  uint8_t  SSSR9:1;
            __O  uint8_t  SSSR10:1;
            __O  uint8_t  SSSR11:1;
            __O  uint8_t  SSSR12:1;
            __O  uint8_t  SSSR13:1;
            __O  uint8_t  SSSR14:1;
            __O  uint8_t  SSSR15:1;
    } bit;
} BTSSSR;

typedef union {
    uint16_t hword;
    struct {
            __IO uint8_t  STRG:1;
            __IO uint8_t  CTEN:1;
            __IO uint8_t  MDSE:1;
            __IO uint8_t  OSEL:1;
            __IO uint8_t  FMD:3;
            __IO uint8_t  RESV2:1;
            __IO uint8_t  EGS:2;
            __IO uint8_t  PMSK:1;
            __IO uint8_t  RTGEN:1;
            __IO uint8_t  CKS210:3;
            __IO uint8_t  RESV1:1;
    } bit;
} TMCR0,TMCR1,TMCR2,TMCR3,TMCR4,TMCR5,TMCR6,TMCR7;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  CKS3:1;
            __IO uint8_t  RESV7:1;
            __IO uint8_t  RESV6:1;
            __IO uint8_t  RESV5:1;
            __IO uint8_t  RESV4:1;
            __IO uint8_t  RESV3:1;
            __IO uint8_t  RESV2:1;
            __IO uint8_t  RESV1:1;
    } bit;
} TMCR20,TMCR21,TMCR22,TMCR23,TMCR24,TMCR25,TMCR26,TMCR27;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  UDIR:1;
            __IO uint8_t  DTIR:1;
            __IO uint8_t  TGIR:1;
            __IO uint8_t  RESV2:1;
            __IO uint8_t  UDIE:1;
            __IO uint8_t  DTIE:1;
            __IO uint8_t  TGIE:1;
            __IO uint8_t  RESV1:1;
    } bit;
} STC0,STC1,STC2,STC3,STC4,STC5,STC6,STC7;

/* struct of CSIO control register */
typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  TXE:1;
            __IO uint8_t  RXE:1;
            __IO uint8_t  TBIE:1;
            __IO uint8_t  TIE:1;
            __IO uint8_t  RIE:1;
            __IO uint8_t  SPI:1;
            __IO uint8_t  MS:1;
            __IO uint8_t  UPCL:1;
    } bit;
} SCR0,SCR1,SCR2,SCR3,SCR4,SCR5,SCR6,SCR7;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  SOE:1;
            __IO uint8_t  SCKE:1;
            __IO uint8_t  BDS:1;
            __IO uint8_t  SCINV:1;
            __IO uint8_t  WUCR:1;
            __IO uint8_t  MD:3;
    } bit;
} SMR0,SMR1,SMR2,SMR3,SMR4,SMR5,SMR6,SMR7;

typedef union {
    uint8_t  byte;
    struct {
            __I  uint8_t  TBI:1;
            __I  uint8_t  TDRE:1;
            __I  uint8_t  RDRF:1;
            __I  uint8_t  ORE:1;
                 uint8_t  RESV3:1;
                 uint8_t  RESV2:1;
                 uint8_t  RESV1:1;
            __IO uint8_t  REC:1;
    } bit;
} SSR0,SSR1,SSR2,SSR3,SSR4,SSR5,SSR6,SSR7;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  L:3;
            __IO uint8_t  WT:2;
                 uint8_t  RESV2:1;
                 uint8_t  RESV1:1;
            __IO uint8_t  SOP:1;
    } bit;
} ESCR0,ESCR1,ESCR2,ESCR3,ESCR4,ESCR5,ESCR6,ESCR7;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  SM0:1;
            __IO uint8_t  SM1:1;
            __IO uint8_t  SM2:1;
            __IO uint8_t  SM3:1;
            __IO uint8_t  SM4:1;
            __IO uint8_t  SM5:1;
            __IO uint8_t  SM6:1;
            __IO uint8_t  EN:1;
    } bit;
} ISMK0,ISMK1,ISMK2,ISMK3,ISMK4,ISMK5,ISMK6,ISMK7;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  SA0:1;
            __IO uint8_t  SA1:1;
            __IO uint8_t  SA2:1;
            __IO uint8_t  SA3:1;
            __IO uint8_t  SA4:1;
            __IO uint8_t  SA5:1;
            __IO uint8_t  SA6:1;
            __IO uint8_t  SAEN:1;
    } bit;
} ISBA0,ISBA1,ISBA2,ISBA3,ISBA4,ISBA5,ISBA6,ISBA7;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  FSEL:1;
            __IO uint8_t  FTIE:1;
            __IO uint8_t  FDRQ:1;
            __IO uint8_t  FRIIE:1;
            __IO uint8_t  FLSTE:1;
                 uint8_t  RESV1:1;
            __IO uint8_t  FTST:2;
    } bit;
} FCR14,FCR15,FCR16,FCR17;

typedef union {
    uint8_t  byte;
    struct {
            __IO uint8_t  FE:2;
            __IO uint8_t  FCL:2;
            __IO uint8_t  FSET:1;
            __IO uint8_t  FLD:1;
            __I  uint8_t  FLST:1;
                 uint8_t  RESV1:1;
    } bit;
} FCR04,FCR05,FCR06,FCR07;

/* base address */
#define BASEADDR_CLK        (0x40010000)
#define BASEADDR_BT0        (0x40025000)
#define BASEADDR_BT1        (0x40025040)
#define BASEADDR_BT2        (0x40025080)
#define BASEADDR_BT3        (0x400250C0)
#define BASEADDR_BT4        (0x40025200)
#define BASEADDR_BT5        (0x40025240)
#define BASEADDR_BT6        (0x40025280)
#define BASEADDR_BT7        (0x400252C0)
#define BASEADDR_GPIO       (0x40033000)
#define BASEADDR_USBCLK     (0x40036000)
#define BASEADDR_USB        (0x40040000)
#define BASEADDR_DMAC       (0x40060000)
#define BASEADDR_MFS0       (0x40038000)
#define BASEADDR_MFS1       (0x40038100)
#define BASEADDR_MFS2       (0x40038200)
#define BASEADDR_MFS3       (0x40038300)
#define BASEADDR_MFS4       (0x40038400)
#define BASEADDR_MFS5       (0x40038500)
#define BASEADDR_MFS6       (0x40038600)
#define BASEADDR_MFS7       (0x40038700)
#define BASEADDR_HWWDT      (0x40011000)
#define BASEADDR_INTREQ     (0x40031000)


/* CSIO control register */
#define IO_SMR0           (*(SMR0*)(BASEADDR_MFS0 + 0x0000))
#define IO_SMR1           (*(SMR1*)(BASEADDR_MFS1 + 0x0000))
#define IO_SMR2           (*(SMR2*)(BASEADDR_MFS2 + 0x0000))
#define IO_SMR3           (*(SMR3*)(BASEADDR_MFS3 + 0x0000))
#define IO_SMR4           (*(SMR4*)(BASEADDR_MFS4 + 0x0000))
#define IO_SMR5           (*(SMR5*)(BASEADDR_MFS5 + 0x0000))
#define IO_SMR6           (*(SMR6*)(BASEADDR_MFS6 + 0x0000))
#define IO_SMR7           (*(SMR7*)(BASEADDR_MFS7 + 0x0000))

#define IO_SCR0           (*(SCR0*)(BASEADDR_MFS0 + 0x0001))
#define IO_SCR1           (*(SCR1*)(BASEADDR_MFS1 + 0x0001))
#define IO_SCR2           (*(SCR2*)(BASEADDR_MFS2 + 0x0001))
#define IO_SCR3           (*(SCR3*)(BASEADDR_MFS3 + 0x0001))
#define IO_SCR4           (*(SCR4*)(BASEADDR_MFS4 + 0x0001))
#define IO_SCR5           (*(SCR5*)(BASEADDR_MFS5 + 0x0001))
#define IO_SCR6           (*(SCR6*)(BASEADDR_MFS6 + 0x0001))
#define IO_SCR7           (*(SCR7*)(BASEADDR_MFS7 + 0x0001))

#define IO_ESCR0          (*(ESCR0*)(BASEADDR_MFS0 + 0x0004))
#define IO_ESCR1          (*(ESCR1*)(BASEADDR_MFS1 + 0x0004))
#define IO_ESCR2          (*(ESCR2*)(BASEADDR_MFS2 + 0x0004))
#define IO_ESCR3          (*(ESCR3*)(BASEADDR_MFS3 + 0x0004))
#define IO_ESCR4          (*(ESCR4*)(BASEADDR_MFS4 + 0x0004))
#define IO_ESCR5          (*(ESCR5*)(BASEADDR_MFS5 + 0x0004))
#define IO_ESCR6          (*(ESCR6*)(BASEADDR_MFS6 + 0x0004))
#define IO_ESCR7          (*(ESCR7*)(BASEADDR_MFS7 + 0x0004))

#define IO_SSR0           (*(SSR0*)(BASEADDR_MFS0 + 0x0005))
#define IO_SSR1           (*(SSR1*)(BASEADDR_MFS1 + 0x0005))
#define IO_SSR2           (*(SSR2*)(BASEADDR_MFS2 + 0x0005))
#define IO_SSR3           (*(SSR3*)(BASEADDR_MFS3 + 0x0005))
#define IO_SSR4           (*(SSR4*)(BASEADDR_MFS4 + 0x0005))
#define IO_SSR5           (*(SSR5*)(BASEADDR_MFS5 + 0x0005))
#define IO_SSR6           (*(SSR6*)(BASEADDR_MFS6 + 0x0005))
#define IO_SSR7           (*(SSR7*)(BASEADDR_MFS7 + 0x0005))

#define IO_RDR0           (*(__I uint16_t*)(BASEADDR_MFS0 + 0x0008))
#define IO_RDR1           (*(__I uint16_t*)(BASEADDR_MFS1 + 0x0008))
#define IO_RDR2           (*(__I uint16_t*)(BASEADDR_MFS2 + 0x0008))
#define IO_RDR3           (*(__I uint16_t*)(BASEADDR_MFS3 + 0x0008))
#define IO_RDR4           (*(__I uint16_t*)(BASEADDR_MFS4 + 0x0008))
#define IO_RDR5           (*(__I uint16_t*)(BASEADDR_MFS5 + 0x0008))
#define IO_RDR6           (*(__I uint16_t*)(BASEADDR_MFS6 + 0x0008))
#define IO_RDR7           (*(__I uint16_t*)(BASEADDR_MFS7 + 0x0008))

#define IO_TDR0           (*(__O uint16_t*)(BASEADDR_MFS0 + 0x0008))
#define IO_TDR1           (*(__O uint16_t*)(BASEADDR_MFS1 + 0x0008))
#define IO_TDR2           (*(__O uint16_t*)(BASEADDR_MFS2 + 0x0008))
#define IO_TDR3           (*(__O uint16_t*)(BASEADDR_MFS3 + 0x0008))
#define IO_TDR4           (*(__O uint16_t*)(BASEADDR_MFS4 + 0x0008))
#define IO_TDR5           (*(__O uint16_t*)(BASEADDR_MFS5 + 0x0008))
#define IO_TDR6           (*(__O uint16_t*)(BASEADDR_MFS6 + 0x0008))
#define IO_TDR7           (*(__O uint16_t*)(BASEADDR_MFS7 + 0x0008))

#define IO_BGR00          (*(__IO uint8_t*)(BASEADDR_MFS0 + 0x000C))
#define IO_BGR01          (*(__IO uint8_t*)(BASEADDR_MFS1 + 0x000C))
#define IO_BGR02          (*(__IO uint8_t*)(BASEADDR_MFS2 + 0x000C))
#define IO_BGR03          (*(__IO uint8_t*)(BASEADDR_MFS3 + 0x000C))
#define IO_BGR04          (*(__IO uint8_t*)(BASEADDR_MFS4 + 0x000C))
#define IO_BGR05          (*(__IO uint8_t*)(BASEADDR_MFS5 + 0x000C))
#define IO_BGR06          (*(__IO uint8_t*)(BASEADDR_MFS6 + 0x000C))
#define IO_BGR07          (*(__IO uint8_t*)(BASEADDR_MFS7 + 0x000C))

#define IO_BGR10          (*(__IO uint8_t*)(BASEADDR_MFS0 + 0x000D))
#define IO_BGR11          (*(__IO uint8_t*)(BASEADDR_MFS1 + 0x000D))
#define IO_BGR12          (*(__IO uint8_t*)(BASEADDR_MFS2 + 0x000D))
#define IO_BGR13          (*(__IO uint8_t*)(BASEADDR_MFS3 + 0x000D))
#define IO_BGR14          (*(__IO uint8_t*)(BASEADDR_MFS4 + 0x000D))
#define IO_BGR15          (*(__IO uint8_t*)(BASEADDR_MFS5 + 0x000D))
#define IO_BGR16          (*(__IO uint8_t*)(BASEADDR_MFS6 + 0x000D))
#define IO_BGR17          (*(__IO uint8_t*)(BASEADDR_MFS7 + 0x000D))

#define IO_ISBA0           (*(ISBA0*)(BASEADDR_MFS0 + 0x0010))
#define IO_ISBA1           (*(ISBA1*)(BASEADDR_MFS1 + 0x0010))
#define IO_ISBA2           (*(ISBA2*)(BASEADDR_MFS2 + 0x0010))
#define IO_ISBA3           (*(ISBA3*)(BASEADDR_MFS3 + 0x0010))
#define IO_ISBA4           (*(ISBA4*)(BASEADDR_MFS4 + 0x0010))
#define IO_ISBA5           (*(ISBA5*)(BASEADDR_MFS5 + 0x0010))
#define IO_ISBA6           (*(ISBA6*)(BASEADDR_MFS6 + 0x0010))
#define IO_ISBA7           (*(ISBA7*)(BASEADDR_MFS7 + 0x0010))

#define IO_ISMK0           (*(ISMK0*)(BASEADDR_MFS0 + 0x0011))
#define IO_ISMK1           (*(ISMK1*)(BASEADDR_MFS1 + 0x0011))
#define IO_ISMK2           (*(ISMK2*)(BASEADDR_MFS2 + 0x0011))
#define IO_ISMK3           (*(ISMK3*)(BASEADDR_MFS3 + 0x0011))
#define IO_ISMK4           (*(ISMK4*)(BASEADDR_MFS4 + 0x0011))
#define IO_ISMK5           (*(ISMK5*)(BASEADDR_MFS5 + 0x0011))
#define IO_ISMK6           (*(ISMK6*)(BASEADDR_MFS6 + 0x0011))
#define IO_ISMK7           (*(ISMK7*)(BASEADDR_MFS7 + 0x0011))

#define IO_FCR04           (*(FCR04*)(BASEADDR_MFS4 + 0x0014))
#define IO_FCR05           (*(FCR05*)(BASEADDR_MFS5 + 0x0014))
#define IO_FCR06           (*(FCR06*)(BASEADDR_MFS6 + 0x0014))
#define IO_FCR07           (*(FCR07*)(BASEADDR_MFS7 + 0x0014))

#define IO_FCR14           (*(FCR14*)(BASEADDR_MFS4 + 0x0015))
#define IO_FCR15           (*(FCR15*)(BASEADDR_MFS5 + 0x0015))
#define IO_FCR16           (*(FCR16*)(BASEADDR_MFS6 + 0x0015))
#define IO_FCR17           (*(FCR17*)(BASEADDR_MFS7 + 0x0015))

#define IO_FBYTE14         (*(__IO uint8_t*)(BASEADDR_MFS4 + 0x0018))
#define IO_FBYTE15         (*(__IO uint8_t*)(BASEADDR_MFS5 + 0x0018))
#define IO_FBYTE16         (*(__IO uint8_t*)(BASEADDR_MFS6 + 0x0018))
#define IO_FBYTE17         (*(__IO uint8_t*)(BASEADDR_MFS7 + 0x0018))

#define IO_FBYTE24         (*(__IO uint8_t*)(BASEADDR_MFS4 + 0x0019))
#define IO_FBYTE25         (*(__IO uint8_t*)(BASEADDR_MFS5 + 0x0019))
#define IO_FBYTE26         (*(__IO uint8_t*)(BASEADDR_MFS6 + 0x0019))
#define IO_FBYTE27         (*(__IO uint8_t*)(BASEADDR_MFS7 + 0x0019))

/* base timer register */
#define IO_BTSEL0123      (*(BTSEL0123*)(0x40025101))
#define IO_BTSEL4567      (*(BTSEL4567*)(0x40025301))
#define IO_BTSSSR         (*(BTSSSR*)(0x40025FFC))

#define IO_PCSR0          (*(__IO uint16_t*)(BASEADDR_BT0 + 0x0000))
#define IO_PCSR1          (*(__IO uint16_t*)(BASEADDR_BT1 + 0x0000))
#define IO_PCSR2          (*(__IO uint16_t*)(BASEADDR_BT2 + 0x0000))
#define IO_PCSR3          (*(__IO uint16_t*)(BASEADDR_BT3 + 0x0000))
#define IO_PCSR4          (*(__IO uint16_t*)(BASEADDR_BT4 + 0x0000))
#define IO_PCSR5          (*(__IO uint16_t*)(BASEADDR_BT5 + 0x0000))
#define IO_PCSR6          (*(__IO uint16_t*)(BASEADDR_BT6 + 0x0000))
#define IO_PCSR7          (*(__IO uint16_t*)(BASEADDR_BT7 + 0x0000))

#define IO_PRLL0          (*(__IO uint16_t*)(BASEADDR_BT0 + 0x0000))
#define IO_PRLL1          (*(__IO uint16_t*)(BASEADDR_BT1 + 0x0000))
#define IO_PRLL2          (*(__IO uint16_t*)(BASEADDR_BT2 + 0x0000))
#define IO_PRLL3          (*(__IO uint16_t*)(BASEADDR_BT3 + 0x0000))
#define IO_PRLL4          (*(__IO uint16_t*)(BASEADDR_BT4 + 0x0000))
#define IO_PRLL5          (*(__IO uint16_t*)(BASEADDR_BT5 + 0x0000))
#define IO_PRLL6          (*(__IO uint16_t*)(BASEADDR_BT6 + 0x0000))
#define IO_PRLL7          (*(__IO uint16_t*)(BASEADDR_BT7 + 0x0000))

#define IO_PDUT0          (*(__IO uint16_t*)(BASEADDR_BT0 + 0x0004))
#define IO_PDUT1          (*(__IO uint16_t*)(BASEADDR_BT1 + 0x0004))
#define IO_PDUT2          (*(__IO uint16_t*)(BASEADDR_BT2 + 0x0004))
#define IO_PDUT3          (*(__IO uint16_t*)(BASEADDR_BT3 + 0x0004))
#define IO_PDUT4          (*(__IO uint16_t*)(BASEADDR_BT4 + 0x0004))
#define IO_PDUT5          (*(__IO uint16_t*)(BASEADDR_BT5 + 0x0004))
#define IO_PDUT6          (*(__IO uint16_t*)(BASEADDR_BT6 + 0x0004))
#define IO_PDUT7          (*(__IO uint16_t*)(BASEADDR_BT7 + 0x0004))

#define IO_PRLH0          (*(__IO uint16_t*)(BASEADDR_BT0 + 0x0004))
#define IO_PRLH1          (*(__IO uint16_t*)(BASEADDR_BT1 + 0x0004))
#define IO_PRLH2          (*(__IO uint16_t*)(BASEADDR_BT2 + 0x0004))
#define IO_PRLH3          (*(__IO uint16_t*)(BASEADDR_BT3 + 0x0004))
#define IO_PRLH4          (*(__IO uint16_t*)(BASEADDR_BT4 + 0x0004))
#define IO_PRLH5          (*(__IO uint16_t*)(BASEADDR_BT5 + 0x0004))
#define IO_PRLH6          (*(__IO uint16_t*)(BASEADDR_BT6 + 0x0004))
#define IO_PRLH7          (*(__IO uint16_t*)(BASEADDR_BT7 + 0x0004))

#define IO_DTBF0          (*(__I  uint16_t*)(BASEADDR_BT0 + 0x0004))
#define IO_DTBF1          (*(__I  uint16_t*)(BASEADDR_BT1 + 0x0004))
#define IO_DTBF2          (*(__I  uint16_t*)(BASEADDR_BT2 + 0x0004))
#define IO_DTBF3          (*(__I  uint16_t*)(BASEADDR_BT3 + 0x0004))
#define IO_DTBF4          (*(__I  uint16_t*)(BASEADDR_BT4 + 0x0004))
#define IO_DTBF5          (*(__I  uint16_t*)(BASEADDR_BT5 + 0x0004))
#define IO_DTBF6          (*(__I  uint16_t*)(BASEADDR_BT6 + 0x0004))
#define IO_DTBF7          (*(__I  uint16_t*)(BASEADDR_BT7 + 0x0004))

#define IO_TMR0           (*(__I uint16_t*)(BASEADDR_BT0 + 0x0008))
#define IO_TMR1           (*(__I uint16_t*)(BASEADDR_BT1 + 0x0008))
#define IO_TMR2           (*(__I uint16_t*)(BASEADDR_BT2 + 0x0008))
#define IO_TMR3           (*(__I uint16_t*)(BASEADDR_BT3 + 0x0008))
#define IO_TMR4           (*(__I uint16_t*)(BASEADDR_BT4 + 0x0008))
#define IO_TMR5           (*(__I uint16_t*)(BASEADDR_BT5 + 0x0008))
#define IO_TMR6           (*(__I uint16_t*)(BASEADDR_BT6 + 0x0008))
#define IO_TMR7           (*(__I uint16_t*)(BASEADDR_BT7 + 0x0008))

#define IO_TMCR0          (*(TMCR0*)(BASEADDR_BT0 + 0x000C))
#define IO_TMCR1          (*(TMCR1*)(BASEADDR_BT1 + 0x000C))
#define IO_TMCR2          (*(TMCR2*)(BASEADDR_BT2 + 0x000C))
#define IO_TMCR3          (*(TMCR3*)(BASEADDR_BT3 + 0x000C))
#define IO_TMCR4          (*(TMCR4*)(BASEADDR_BT4 + 0x000C))
#define IO_TMCR5          (*(TMCR5*)(BASEADDR_BT5 + 0x000C))
#define IO_TMCR6          (*(TMCR6*)(BASEADDR_BT6 + 0x000C))
#define IO_TMCR7          (*(TMCR7*)(BASEADDR_BT7 + 0x000C))

#define IO_STC0           (*(STC0*)(BASEADDR_BT0 + 0x0010))
#define IO_STC1           (*(STC1*)(BASEADDR_BT1 + 0x0010))
#define IO_STC2           (*(STC2*)(BASEADDR_BT2 + 0x0010))
#define IO_STC3           (*(STC3*)(BASEADDR_BT3 + 0x0010))
#define IO_STC4           (*(STC4*)(BASEADDR_BT4 + 0x0010))
#define IO_STC5           (*(STC5*)(BASEADDR_BT5 + 0x0010))
#define IO_STC6           (*(STC6*)(BASEADDR_BT6 + 0x0010))
#define IO_STC7           (*(STC7*)(BASEADDR_BT7 + 0x0010))

#define IO_TMCR20         (*(TMCR20 *)(BASEADDR_BT0 + 0x0011))
#define IO_TMCR21         (*(TMCR21 *)(BASEADDR_BT1 + 0x0011))
#define IO_TMCR22         (*(TMCR22 *)(BASEADDR_BT2 + 0x0011))
#define IO_TMCR23         (*(TMCR23 *)(BASEADDR_BT3 + 0x0011))
#define IO_TMCR24         (*(TMCR24 *)(BASEADDR_BT4 + 0x0011))
#define IO_TMCR25         (*(TMCR25 *)(BASEADDR_BT5 + 0x0011))
#define IO_TMCR26         (*(TMCR26 *)(BASEADDR_BT6 + 0x0011))
#define IO_TMCR27         (*(TMCR27 *)(BASEADDR_BT7 + 0x0011))

/* clock generation register */
#define IO_SCM_CTL      (*(SCM_CTL*)(BASEADDR_CLK + 0x0000))
#define IO_SCM_STR      (*(SCM_STR*)(BASEADDR_CLK + 0x0004))
#define IO_BSC_PSR      (*(BSC_PSR*)(BASEADDR_CLK + 0x0010))
#define IO_APBC0_PSR    (*(APBC0_PSR*)(BASEADDR_CLK + 0x0014))
#define IO_APBC1_PSR    (*(APBC1_PSR*)(BASEADDR_CLK + 0x0018))
#define IO_APBC2_PSR    (*(APBC2_PSR*)(BASEADDR_CLK + 0x001C))
#define IO_SWC_PSR      (*(SWC_PSR*)(BASEADDR_CLK + 0x0020))
#define IO_TTC_PSR      (*(TTC_PSR*)(BASEADDR_CLK + 0x0028))
#define IO_CSW_TMR      (*(CSW_TMR*)(BASEADDR_CLK + 0x0030))
#define IO_PSW_TMR      (*(PSW_TMR*)(BASEADDR_CLK + 0x0034))
#define IO_PLL_CTL1     (*(PLL_CTL1*)(BASEADDR_CLK + 0x0038))
#define IO_PLL_CTL2     (*(PLL_CTL2*)(BASEADDR_CLK + 0x003C))
#define IO_DBWDT_CTL    (*(DBWDT_CTL*)(BASEADDR_CLK + 0x0054))
#define IO_INT_ENR      (*(INT_ENR*)(BASEADDR_CLK + 0x0060))
#define IO_INT_STR      (*(INT_STR*)(BASEADDR_CLK + 0x0064))
#define IO_INT_CLR      (*(INT_CLR*)(BASEADDR_CLK + 0x0068))

/* I/O port control register */
#define IO_PFR0        (*(PFR0*)(BASEADDR_GPIO + 0x0000))
#define IO_PFR1        (*(PFR1*)(BASEADDR_GPIO + 0x0004))
#define IO_PFR2        (*(PFR2*)(BASEADDR_GPIO + 0x0008))
#define IO_PFR3        (*(PFR3*)(BASEADDR_GPIO + 0x000C))
#define IO_PFR4        (*(PFR4*)(BASEADDR_GPIO + 0x0010))
#define IO_PFR5        (*(PFR5*)(BASEADDR_GPIO + 0x0014))
#define IO_PFR6        (*(PFR6*)(BASEADDR_GPIO + 0x0018))
#define IO_PFR7        (*(PFR7*)(BASEADDR_GPIO + 0x001C))

#define IO_PCR0        (*(PCR0*)(BASEADDR_GPIO + 0x0100))
#define IO_PCR1        (*(PCR1*)(BASEADDR_GPIO + 0x0104))
#define IO_PCR2        (*(PCR2*)(BASEADDR_GPIO + 0x0108))
#define IO_PCR3        (*(PCR3*)(BASEADDR_GPIO + 0x010C))
#define IO_PCR4        (*(PCR4*)(BASEADDR_GPIO + 0x0110))
#define IO_PCR5        (*(PCR5*)(BASEADDR_GPIO + 0x0114))
#define IO_PCR6        (*(PCR6*)(BASEADDR_GPIO + 0x0118))
#define IO_PCR7        (*(PCR7*)(BASEADDR_GPIO + 0x011C))

#define IO_DDR0        (*(DDR0*)(BASEADDR_GPIO + 0x0200))
#define IO_DDR1        (*(DDR1*)(BASEADDR_GPIO + 0x0204))
#define IO_DDR2        (*(DDR2*)(BASEADDR_GPIO + 0x0208))
#define IO_DDR3        (*(DDR3*)(BASEADDR_GPIO + 0x020C))
#define IO_DDR4        (*(DDR4*)(BASEADDR_GPIO + 0x0210))
#define IO_DDR5        (*(DDR5*)(BASEADDR_GPIO + 0x0214))
#define IO_DDR6        (*(DDR6*)(BASEADDR_GPIO + 0x0218))
#define IO_DDR7        (*(DDR7*)(BASEADDR_GPIO + 0x021C))

#define IO_PDIR0       (*(PDIR0*)(BASEADDR_GPIO + 0x0300))
#define IO_PDIR1       (*(PDIR1*)(BASEADDR_GPIO + 0x0304))
#define IO_PDIR2       (*(PDIR2*)(BASEADDR_GPIO + 0x0308))
#define IO_PDIR3       (*(PDIR3*)(BASEADDR_GPIO + 0x030C))
#define IO_PDIR4       (*(PDIR4*)(BASEADDR_GPIO + 0x0310))
#define IO_PDIR5       (*(PDIR5*)(BASEADDR_GPIO + 0x0314))
#define IO_PDIR6       (*(PDIR6*)(BASEADDR_GPIO + 0x0318))
#define IO_PDIR7       (*(PDIR7*)(BASEADDR_GPIO + 0x031C))

#define IO_PDOR0       (*(PDOR0*)(BASEADDR_GPIO + 0x0400))
#define IO_PDOR1       (*(PDOR1*)(BASEADDR_GPIO + 0x0404))
#define IO_PDOR2       (*(PDOR2*)(BASEADDR_GPIO + 0x0408))
#define IO_PDOR3       (*(PDOR3*)(BASEADDR_GPIO + 0x040C))
#define IO_PDOR4       (*(PDOR4*)(BASEADDR_GPIO + 0x0410))
#define IO_PDOR5       (*(PDOR5*)(BASEADDR_GPIO + 0x0414))
#define IO_PDOR6       (*(PDOR6*)(BASEADDR_GPIO + 0x0418))
#define IO_PDOR7       (*(PDOR7*)(BASEADDR_GPIO + 0x041C))

#define IO_ADE         (*(ADE*)(BASEADDR_GPIO + 0x0500))

#define IO_EPFR00      (*(EPFR00*)(BASEADDR_GPIO + 0x0600))
#define IO_EPFR01      (*(EPFR01*)(BASEADDR_GPIO + 0x0604))
#define IO_EPFR02      (*(EPFR02*)(BASEADDR_GPIO + 0x0608))
#define IO_EPFR04      (*(EPFR04*)(BASEADDR_GPIO + 0x0610))
#define IO_EPFR05      (*(EPFR05*)(BASEADDR_GPIO + 0x0614))
#define IO_EPFR06      (*(EPFR06*)(BASEADDR_GPIO + 0x0618))
#define IO_EPFR07      (*(EPFR07*)(BASEADDR_GPIO + 0x061C))
#define IO_EPFR08      (*(EPFR08*)(BASEADDR_GPIO + 0x0620))
#define IO_EPFR09      (*(EPFR09*)(BASEADDR_GPIO + 0x0624))
#define IO_EPFR10      (*(EPFR10*)(BASEADDR_GPIO + 0x0628))

/* USB clock generation register */
#define IO_UCCR         (*(UCCR*)(BASEADDR_USBCLK + 0x0000))
#define IO_UPCR1        (*(UPCR1*)(BASEADDR_USBCLK + 0x0004))
#define IO_UPCR2        (*(UPCR2*)(BASEADDR_USBCLK + 0x0008))
#define IO_UPCR3        (*(UPCR3*)(BASEADDR_USBCLK + 0x000C))
#define IO_UPCR4        (*(UPCR4*)(BASEADDR_USBCLK + 0x0010))
#define IO_UP_STR       (*(UP_STR*)(BASEADDR_USBCLK + 0x0014))
#define IO_UPINT_ENR    (*(UPINT_ENR*)(BASEADDR_USBCLK + 0x0018))
#define IO_UPINT_CLR    (*(UPINT_CLR*)(BASEADDR_USBCLK + 0x001C))
#define IO_UPIN_STR     (*(UPIN_STR*)(BASEADDR_USBCLK + 0x0020))
#define IO_USBEN        (*(USBEN*)(BASEADDR_USBCLK + 0x0030))

/* USB control register */
#define IO_HCNT         (*(HCNT*)(BASEADDR_USB + 0x2100))
#define IO_HIRQ         (*(HIRQ*)(BASEADDR_USB + 0x2104))
#define IO_HERR         (*(HERR*)(BASEADDR_USB + 0x2105))
#define IO_HSTATE       (*(HSTATE*)(BASEADDR_USB + 0x2108))
#define IO_HFCOMP       (*(__IO uint8_t*)(BASEADDR_USB + 0x2109))
#define IO_HRTIMER10    (*(__IO uint16_t*)(BASEADDR_USB + 0x210C))
#define IO_HRTIMER2     (*(__IO uint8_t*)(BASEADDR_USB + 0x2110))
#define IO_HADR         (*(__IO uint8_t*)(BASEADDR_USB + 0x2111))
#define IO_HEOF         (*(HEOF*)(BASEADDR_USB + 0x2114))
#define IO_HFRAME       (*(HFRAME*)(BASEADDR_USB + 0x2118))
#define IO_HTOKEN       (*(HTOKEN*)(BASEADDR_USB + 0x211C))
#define IO_UDCC         (*(UDCC*)(BASEADDR_USB + 0x2120))
#define IO_EP0C         (*(EP0C*)(BASEADDR_USB + 0x2124))
#define IO_EP1C         (*(EP1C*)(BASEADDR_USB + 0x2128))
#define IO_EP2C         (*(EP2C*)(BASEADDR_USB + 0x212C))
#define IO_EP3C         (*(EP3C*)(BASEADDR_USB + 0x2130))
#define IO_EP4C         (*(EP4C*)(BASEADDR_USB + 0x2134))
#define IO_EP5C         (*(EP5C*)(BASEADDR_USB + 0x2138))
#define IO_TMSP         (*(__I uint16_t*)(BASEADDR_USB + 0x213C))
#define IO_UDCS         (*(UDCS*)(BASEADDR_USB + 0x2140))
#define IO_UDCIE        (*(UDCIE*)(BASEADDR_USB + 0x2141))
#define IO_EP0IS        (*(EP0IS*)(BASEADDR_USB + 0x2144))
#define IO_EP0OS        (*(EP0OS*)(BASEADDR_USB + 0x2148))
#define IO_EP1S         (*(EP1S*)(BASEADDR_USB + 0x214C))
#define IO_EP2S         (*(EP2S*)(BASEADDR_USB + 0x2150))
#define IO_EP3S         (*(EP3S*)(BASEADDR_USB + 0x2154))
#define IO_EP4S         (*(EP4S*)(BASEADDR_USB + 0x2158))
#define IO_EP5S         (*(EP5S*)(BASEADDR_USB + 0x215C))
#define IO_EP0DT        (*(EP0DT*)(BASEADDR_USB + 0x2160))
#define IO_EP1DT        (*(EP1DT*)(BASEADDR_USB + 0x2164))
#define IO_EP2DT        (*(EP2DT*)(BASEADDR_USB + 0x2168))
#define IO_EP3DT        (*(EP3DT*)(BASEADDR_USB + 0x216C))
#define IO_EP4DT        (*(EP4DT*)(BASEADDR_USB + 0x2170))
#define IO_EP5DT        (*(EP5DT*)(BASEADDR_USB + 0x2174))

/* DMA control register */
#define IO_DMACR        (*(DMACR*)(BASEADDR_DMAC + 0x0000))
#define IO_DMACA0       (*(DMACA0*)(BASEADDR_DMAC + 0x0010))
#define IO_DMACB0       (*(DMACB0*)(BASEADDR_DMAC + 0x0014))
#define IO_DMACSA0      (*(__IO uint32_t*)(BASEADDR_DMAC + 0x0018))
#define IO_DMACDA0      (*(__IO uint32_t*)(BASEADDR_DMAC + 0x001C))
#define IO_DMACA1       (*(DMACA1*)(BASEADDR_DMAC + 0x0020))
#define IO_DMACB1       (*(DMACB1*)(BASEADDR_DMAC + 0x0024))
#define IO_DMACSA1      (*(__IO uint32_t*)(BASEADDR_DMAC + 0x0028))
#define IO_DMACDA1      (*(__IO uint32_t*)(BASEADDR_DMAC + 0x002C))

/* HW_WDT */
#define	IO_WDG_CTL      (*(__IO uint8_t*)(BASEADDR_HWWDT + 0x0008))
#define	IO_WDG_LCK	    (*(__IO uint32_t*)(BASEADDR_HWWDT + 0x0C00))

/* interrupt request register */
#define	IO_DRQSEL       (*(__IO uint32_t*)(BASEADDR_INTREQ + 0x0000))

#endif

