/******************************************************************************
*   DISCLAIMER
*
*   This software is supplied by Renesas Technology Corp. and is only 
*   intended for use with Renesas products. No other uses are authorized.
*
*   This software is owned by Renesas Technology Corp. and is protected under 
*   all applicable laws, including copyright laws.
*
*   THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
*   REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
*   INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
*   PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
*   DISCLAIMED.
*
*   TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
*   TECHNOLOGY CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
*   FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
*   FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
*   AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
*   Renesas reserves the right, without notice, to make changes to this 
*   software and to discontinue the availability of this software.
*   By using this software, you agree to the additional terms and 
*   conditions found by accessing the following link: 
*   http://www.renesas.com/disclaimer
********************************************************************************
*   Copyright (C) 2008(2009). Renesas Technology Corp., All Rights Reserved.
*""FILE COMMENT""*********** Technical reference data **************************
*   System Name : SH7264 Sample Program
*   File Name   : iodefine.h
*   Abstract    : SH7264 IO define file
*   Version     : 1.03.00
*   Device      : SH7264
*               : (R5S72640W144FPU,R5S72641W144FPU,R5S72642W144FPU,R5S72643W144FPU)
*               : (R5S72640P144FPU,R5S72641P144FPU,R5S72642P144FPU,R5S72643P144FPU)
*   Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
*               : C/C++ compiler package for the SuperH RISC engine family
*               :                             (Ver.9.02 Release00).
*   OS          : None
*   H/W Platform: M3A-HS64G50(CPU board)
*   Description : 
********************************************************************************
*       History   : Apr.13,2009 ver.1.00.00
*                 : Apr.22,2009 ver.1.01.00
*                 :    Changed bit name
*                 :     VDC3.VIDEO_IMAGE_CNT.CLKP_C  ->  VDC3.VIDEO_IMAGE_CNT.CLIP_C
*                 :     VDC3.SYNCNT.M_DISP_TM  ->  VDC3.SYNCNT.M_DISP_TIM
*                 : Jun.29,2009 Ver.1.02.00  Changed FILE FORMAT
*                 : Sep.29,2009 Ver.1.03.00
*                 :    Changed bit name
*                 :     USB.USBACSWR1.PREENA -> USB.USBACSWR1.UACS25
*""FILE COMMENT END""**********************************************************/
#ifndef _IODEFINE_H_
#define _IODEFINE_H_

#include "typedefine.h"

struct st_cpg {                                 /* struct CPG   */
       union {                                  /* FRQCR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD CKOEN2:1;            /*   CKOEN2     */
                    _UWORD CKOEN:2;             /*   CKOEN      */
                    _UWORD :3;                  /*              */
                    _UWORD STC:1;               /*   STC        */
                    _UWORD :2;                  /*              */
                    _UWORD IFC:2;               /*   IFC        */
                    _UWORD :1;                  /*              */
                    _UWORD PFC:3;               /*   PFC        */
                    } BIT;                      /*              */
             } FRQCR;                           /*              */
       _UBYTE wk0[2];                           /*              */
       union {                                  /* STBCR1       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE STBY:1;              /*   STBY       */
                    _UBYTE DEEP:1;              /*   DEEP       */
                    _UBYTE :6;                  /*              */
                    } BIT;                      /*              */
             } STBCR1;                          /*              */
       _UBYTE wk1[3];                           /*              */
       union {                                  /* STBCR2       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE MSTP10:1;            /*   MSTP10     */
                    _UBYTE :1;                  /*              */
                    _UBYTE MSTP8:1;             /*   MSTP8      */
                    _UBYTE MSTP7:1;             /*   MSTP7      */
                    _UBYTE :4;                  /*              */
                    } BIT;                      /*              */
             } STBCR2;                          /*              */
       _UBYTE wk2[999];                         /*              */
       union {                                  /* SYSCR1       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :4;                  /*              */
                    _UBYTE RAME3:1;             /*   RAME3      */
                    _UBYTE RAME2:1;             /*   RAME2      */
                    _UBYTE RAME1:1;             /*   RAME1      */
                    _UBYTE RAME0:1;             /*   RAME0      */
                    } BIT;                      /*              */
             } SYSCR1;                          /*              */
       _UBYTE wk3[3];                           /*              */
       union {                                  /* SYSCR2       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :4;                  /*              */
                    _UBYTE RAMWE3:1;            /*   RAMWE3     */
                    _UBYTE RAMWE2:1;            /*   RAMWE2     */
                    _UBYTE RAMWE1:1;            /*   RAMWE1     */
                    _UBYTE RAMWE0:1;            /*   RAMWE0     */
                    } BIT;                      /*              */
             } SYSCR2;                          /*              */
       _UBYTE wk4[3];                           /*              */
       union {                                  /* STBCR3       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE HIZ:1;               /*   HIZ        */
                    _UBYTE MSTP36:1;            /*   MSTP36     */
                    _UBYTE MSTP35:1;            /*   MSTP35     */
                    _UBYTE MSTP34:1;            /*   MSTP34     */
                    _UBYTE MSTP33:1;            /*   MSTP33     */
                    _UBYTE MSTP32:1;            /*   MSTP32     */
                    _UBYTE :1;                  /*              */
                    _UBYTE MSTP30:1;            /*   MSTP30     */
                    } BIT;                      /*              */
             } STBCR3;                          /*              */
       _UBYTE wk5[3];                           /*              */
       union {                                  /* STBCR4       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE MSTP47:1;            /*   MSTP47     */
                    _UBYTE MSTP46:1;            /*   MSTP46     */
                    _UBYTE MSTP45:1;            /*   MSTP45     */
                    _UBYTE MSTP44:1;            /*   MSTP44     */
                    _UBYTE MSTP43:1;            /*   MSTP43     */
                    _UBYTE MSTP42:1;            /*   MSTP42     */
                    _UBYTE MSTP41:1;            /*   MSTP41     */
                    _UBYTE MSTP40:1;            /*   MSTP40     */
                    } BIT;                      /*              */
             } STBCR4;                          /*              */
       _UBYTE wk6[3];                           /*              */
       union {                                  /* STBCR5       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE MSTP57:1;            /*   MSTP57     */
                    _UBYTE MSTP56:1;            /*   MSTP56     */
                    _UBYTE MSTP55:1;            /*   MSTP55     */
                    _UBYTE :1;                  /*              */
                    _UBYTE MSTP53:1;            /*   MSTP53     */
                    _UBYTE MSTP52:1;            /*   MSTP52     */
                    _UBYTE MSTP51:1;            /*   MSTP51     */
                    _UBYTE MSTP50:1;            /*   MSTP50     */
                    } BIT;                      /*              */
             } STBCR5;                          /*              */
       _UBYTE wk7[3];                           /*              */
       union {                                  /* STBCR6       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE MSTP67:1;            /*   MSTP67     */
                    _UBYTE MSTP66:1;            /*   MSTP66     */
                    _UBYTE MSTP65:1;            /*   MSTP65     */
                    _UBYTE MSTP64:1;            /*   MSTP64     */
                    _UBYTE MSTP63:1;            /*   MSTP63     */
                    _UBYTE MSTP62:1;            /*   MSTP62     */
                    _UBYTE MSTP61:1;            /*   MSTP61     */
                    _UBYTE MSTP60:1;            /*   MSTP60     */
                    } BIT;                      /*              */
             } STBCR6;                          /*              */
       _UBYTE wk8[3];                           /*              */
       union {                                  /* STBCR7       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE MSTP77:1;            /*   MSTP77     */
                    _UBYTE MSTP76:1;            /*   MSTP76     */
                    _UBYTE :1;                  /*              */
                    _UBYTE MSTP74:1;            /*   MSTP74     */
                    _UBYTE :1;                  /*              */
                    _UBYTE MSTP72:1;            /*   MSTP72     */
                    _UBYTE :1;                  /*              */
                    _UBYTE MSTP70:1;            /*   MSTP70     */
                    } BIT;                      /*              */
             } STBCR7;                          /*              */
       _UBYTE wk9[3];                           /*              */
       union {                                  /* STBCR8       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE MSTP87:1;            /*   MSTP87     */
                    _UBYTE :6;                  /*              */
                    _UBYTE MSTP80:1;            /*   MSTP80     */
                    } BIT;                      /*              */
             } STBCR8;                          /*              */
       _UBYTE wk10[3];                          /*              */
       union {                                  /* SYSCR3       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE VRAME5:1;            /*   VRAME5     */
                    _UBYTE VRAME4:1;            /*   VRAME4     */
                    _UBYTE VRAME3:1;            /*   VRAME3     */
                    _UBYTE VRAME2:1;            /*   VRAME2     */
                    _UBYTE VRAME1:1;            /*   VRAME1     */
                    _UBYTE VRAME0:1;            /*   VRAME0     */
                    } BIT;                      /*              */
             } SYSCR3;                          /*              */
       _UBYTE wk11[3];                          /*              */
       union {                                  /* SYSCR4       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE VRAMWE5:1;           /*   VRAMWE5    */
                    _UBYTE VRAMWE4:1;           /*   VRAMWE4    */
                    _UBYTE VRAMWE3:1;           /*   VRAMWE3    */
                    _UBYTE VRAMWE2:1;           /*   VRAMWE2    */
                    _UBYTE VRAMWE1:1;           /*   VRAMWE1    */
                    _UBYTE VRAMWE0:1;           /*   VRAMWE0    */
                    } BIT;                      /*              */
             } SYSCR4;                          /*              */
       _UBYTE wk12[11];                         /*              */
       union {                                  /* SWRSTCR      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE AXTALE:1;            /*   AXTALE     */
                    _UBYTE :2;                  /*              */
                    _UBYTE IEBSRST:1;           /*   IEBSRST    */
                    _UBYTE SSIF3SRST:1;         /*   SSIF3SRST  */
                    _UBYTE SSIF2SRST:1;         /*   SSIF2SRST  */
                    _UBYTE SSIF1SRST:1;         /*   SSIF1SRST  */
                    _UBYTE SSIF0SRST:1;         /*   SSIF0SRST  */
                    } BIT;                      /*              */
             } SWRSTCR;                         /*              */
       _UBYTE wk13[25551];                      /*              */
       union {                                  /* RRAMKP       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :6;                  /*              */
                    _UBYTE RRAMKP1:1;           /*   RRAMKP1    */
                    _UBYTE RRAMKP0:1;           /*   RRAMKP0    */
                    } BIT;                      /*              */
             } RRAMKP;                          /*              */
       _UBYTE wk14[1];                          /*              */
       union {                                  /* DSCTR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE EBUSKEEPE:1;         /*   EBUSKEEPE  */
                    _UBYTE RAMBOOT:1;           /*   RAMBOOT    */
                    _UBYTE :6;                  /*              */
                    } BIT;                      /*              */
             } DSCTR;                           /*              */
       _UBYTE wk15[1];                          /*              */
       union {                                  /* DSSSR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :9;                  /*              */
                    _UWORD RTCAR:1;             /*   RTCAR      */
                    _UWORD PC8:1;               /*   PC8        */
                    _UWORD PC7:1;               /*   PC7        */
                    _UWORD PC6:1;               /*   PC6        */
                    _UWORD PC5:1;               /*   PC5        */
                    _UWORD PJ3:1;               /*   PJ3        */
                    _UWORD PJ1:1;               /*   PJ1        */
                    } BIT;                      /*              */
             } DSSSR;                           /*              */
       union {                                  /* DSESR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :7;                  /*              */
                    _UWORD NMIE:1;              /*   NMIE       */
                    _UWORD :2;                  /*              */
                    _UWORD PC8E:1;              /*   PC8E       */
                    _UWORD PC7E:1;              /*   PC7E       */
                    _UWORD PC6E:1;              /*   PC6E       */
                    _UWORD PC5E:1;              /*   PC5E       */
                    _UWORD PJ3E:1;              /*   PJ3E       */
                    _UWORD PJ1E:1;              /*   PJ1E       */
                    } BIT;                      /*              */
             } DSESR;                           /*              */
       union {                                  /* DSFR         */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD IOKEEP:1;            /*   IOKEEP     */
                    _UWORD :6;                  /*              */
                    _UWORD NMIF:1;              /*   NMIF       */
                    _UWORD :1;                  /*              */
                    _UWORD RTCARF:1;            /*   RTCARF     */
                    _UWORD PC8F:1;              /*   PC8F       */
                    _UWORD PC7F:1;              /*   PC7F       */
                    _UWORD PC6F:1;              /*   PC6F       */
                    _UWORD PC5F:1;              /*   PC5F       */
                    _UWORD PJ3F:1;              /*   PJ3F       */
                    _UWORD PJ1F:1;              /*   PJ1F       */
                    } BIT;                      /*              */
             } DSFR;                            /*              */
       _UBYTE wk16[6];                          /*              */
       union {                                  /* XTALCTR      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :7;                  /*              */
                    _UBYTE GAIN:1;              /*   GAIN       */
                    } BIT;                      /*              */
             } XTALCTR;                         /*              */
};                                              /*              */
struct st_intc {                                /* struct INTC  */
       union {                                  /* ICR0         */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD NMIL:1;              /*   NMIL       */
                    _UWORD :6;                  /*              */
                    _UWORD NMIE:1;              /*   NMIE       */
                    _UWORD :8;                  /*              */
                    } BIT;                      /*              */
             } ICR0;                            /*              */
       union {                                  /* ICR1         */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD IRQ71S:1;            /*   IRQ71S     */
                    _UWORD IRQ70S:1;            /*   IRQ70S     */
                    _UWORD IRQ61S:1;            /*   IRQ61S     */
                    _UWORD IRQ60S:1;            /*   IRQ60S     */
                    _UWORD IRQ51S:1;            /*   IRQ51S     */
                    _UWORD IRQ50S:1;            /*   IRQ50S     */
                    _UWORD IRQ41S:1;            /*   IRQ41S     */
                    _UWORD IRQ40S:1;            /*   IRQ40S     */
                    _UWORD IRQ31S:1;            /*   IRQ31S     */
                    _UWORD IRQ30S:1;            /*   IRQ30S     */
                    _UWORD IRQ21S:1;            /*   IRQ21S     */
                    _UWORD IRQ20S:1;            /*   IRQ20S     */
                    _UWORD IRQ11S:1;            /*   IRQ11S     */
                    _UWORD IRQ10S:1;            /*   IRQ10S     */
                    _UWORD IRQ01S:1;            /*   IRQ01S     */
                    _UWORD IRQ00S:1;            /*   IRQ00S     */
                    } BIT;                      /*              */
             } ICR1;                            /*              */
       union {                                  /* ICR2         */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD PINT7S:1;            /*   PINT7S     */
                    _UWORD PINT6S:1;            /*   PINT6S     */
                    _UWORD PINT5S:1;            /*   PINT5S     */
                    _UWORD PINT4S:1;            /*   PINT4S     */
                    _UWORD PINT3S:1;            /*   PINT3S     */
                    _UWORD PINT2S:1;            /*   PINT2S     */
                    _UWORD PINT1S:1;            /*   PINT1S     */
                    _UWORD PINT0S:1;            /*   PINT0S     */
                    } BIT;                      /*              */
             } ICR2;                            /*              */
       union {                                  /* IRQRR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD IRQ7F:1;             /*   IRQ7F      */
                    _UWORD IRQ6F:1;             /*   IRQ6F      */
                    _UWORD IRQ5F:1;             /*   IRQ5F      */
                    _UWORD IRQ4F:1;             /*   IRQ4F      */
                    _UWORD IRQ3F:1;             /*   IRQ3F      */
                    _UWORD IRQ2F:1;             /*   IRQ2F      */
                    _UWORD IRQ1F:1;             /*   IRQ1F      */
                    _UWORD IRQ0F:1;             /*   IRQ0F      */
                    } BIT;                      /*              */
             } IRQRR;                           /*              */
       union {                                  /* PINTER       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD PINT7E:1;            /*   PINT7E     */
                    _UWORD PINT6E:1;            /*   PINT6E     */
                    _UWORD PINT5E:1;            /*   PINT5E     */
                    _UWORD PINT4E:1;            /*   PINT4E     */
                    _UWORD PINT3E:1;            /*   PINT3E     */
                    _UWORD PINT2E:1;            /*   PINT2E     */
                    _UWORD PINT1E:1;            /*   PINT1E     */
                    _UWORD PINT0E:1;            /*   PINT0E     */
                    } BIT;                      /*              */
             } PINTER;                          /*              */
       union {                                  /* PIRR         */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD PINT7R:1;            /*   PINT7R     */
                    _UWORD PINT6R:1;            /*   PINT6R     */
                    _UWORD PINT5R:1;            /*   PINT5R     */
                    _UWORD PINT4R:1;            /*   PINT4R     */
                    _UWORD PINT3R:1;            /*   PINT3R     */
                    _UWORD PINT2R:1;            /*   PINT2R     */
                    _UWORD PINT1R:1;            /*   PINT1R     */
                    _UWORD PINT0R:1;            /*   PINT0R     */
                    } BIT;                      /*              */
             } PIRR;                            /*              */
       union {                                  /* IBCR         */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD E15:1;               /*   E15        */
                    _UWORD E14:1;               /*   E14        */
                    _UWORD E13:1;               /*   E13        */
                    _UWORD E12:1;               /*   E12        */
                    _UWORD E11:1;               /*   E11        */
                    _UWORD E10:1;               /*   E10        */
                    _UWORD E9:1;                /*   E9         */
                    _UWORD E8:1;                /*   E8         */
                    _UWORD E7:1;                /*   E7         */
                    _UWORD E6:1;                /*   E6         */
                    _UWORD E5:1;                /*   E5         */
                    _UWORD E4:1;                /*   E4         */
                    _UWORD E3:1;                /*   E3         */
                    _UWORD E2:1;                /*   E2         */
                    _UWORD E1:1;                /*   E1         */
                    _UWORD :1;                  /*              */
                    } BIT;                      /*              */
             } IBCR;                            /*              */
       union {                                  /* IBNR         */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BE:2;                /*   BE         */
                    _UWORD BOVE:1;              /*   BOVE       */
                    _UWORD :9;                  /*              */
                    _UWORD BN:4;                /*   BN         */
                    } BIT;                      /*              */
             } IBNR;                            /*              */
       _UBYTE wk0[8];                           /*              */
       union {                                  /* IPR01        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _IRQ0:4;             /*   _IRQ0      */
                    _UWORD _IRQ1:4;             /*   _IRQ1      */
                    _UWORD _IRQ2:4;             /*   _IRQ2      */
                    _UWORD _IRQ3:4;             /*   _IRQ3      */
                    } BIT;                      /*              */
             } IPR01;                           /*              */
       union {                                  /* IPR02        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _IRQ4:4;             /*   _IRQ4      */
                    _UWORD _IRQ5:4;             /*   _IRQ5      */
                    _UWORD _IRQ6:4;             /*   _IRQ6      */
                    _UWORD _IRQ7:4;             /*   _IRQ7      */
                    } BIT;                      /*              */
             } IPR02;                           /*              */
       _UBYTE wk1[4];                           /*              */
       union {                                  /* IPR05        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _PINT:4;             /*   _IRQ4      */
                    _UWORD :12;                 /*              */
                    } BIT;                      /*              */
             } IPR05;                           /*              */
       _UBYTE wk2[990];                         /*              */
       union {                                  /* IPR06        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _DMAC0:4;            /*   _DMAC0     */
                    _UWORD _DMAC1:4;            /*   _DMAC1     */
                    _UWORD _DMAC2:4;            /*   _DMAC2     */
                    _UWORD _DMAC3:4;            /*   _DMAC3     */
                    } BIT;                      /*              */
             } IPR06;                           /*              */
       union {                                  /* IPR07        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _DMAC4:4;            /*   _DMAC4     */
                    _UWORD _DMAC5:4;            /*   _DMAC5     */
                    _UWORD _DMAC6:4;            /*   _DMAC6     */
                    _UWORD _DMAC7:4;            /*   _DMAC7     */
                    } BIT;                      /*              */
             } IPR07;                           /*              */
       union {                                  /* IPR08        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _DMAC8:4;            /*   _DMAC8     */
                    _UWORD _DMAC9:4;            /*   _DMAC9     */
                    _UWORD _DMAC10:4;           /*   _DMAC10    */
                    _UWORD _DMAC11:4;           /*   _DMAC11    */
                    } BIT;                      /*              */
             } IPR08;                           /*              */
       union {                                  /* IPR09        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _DMAC12:4;           /*   _DMAC12    */
                    _UWORD _DMAC13:4;           /*   _DMAC13    */
                    _UWORD _DMAC14:4;           /*   _DMAC14    */
                    _UWORD _DMAC15:4;           /*   _DMAC15    */
                    } BIT;                      /*              */
             } IPR09;                           /*              */
       union {                                  /* IPR10        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _USB:4;              /*   _USB       */
                    _UWORD _VDC3:4;             /*   _VDC3      */
                    _UWORD _CMT0:4;             /*   _CMT0      */
                    _UWORD _CMT1:4;             /*   _CMT1      */
                    } BIT;                      /*              */
             } IPR10;                           /*              */
       union {                                  /* IPR11        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _BSC:4;              /*   _BSC       */
                    _UWORD _WDT:4;              /*   _WDT       */
                    _UWORD _MTU00:4;            /*   _MTU00     */
                    _UWORD _MTU01:4;            /*   _MTU01     */
                    } BIT;                      /*              */
             } IPR11;                           /*              */
       union {                                  /* IPR12        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _MTU10:4;            /*   _MTU10     */
                    _UWORD _MTU11:4;            /*   _MTU11     */
                    _UWORD _MTU20:4;            /*   _MTU20     */
                    _UWORD _MTU21:4;            /*   _MTU21     */
                    } BIT;                      /*              */
             } IPR12;                           /*              */
       union {                                  /* IPR13        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _MTU30:4;            /*   _MTU30     */
                    _UWORD _MTU31:4;            /*   _MTU31     */
                    _UWORD _MTU40:4;            /*   _MTU40     */
                    _UWORD _MTU41:4;            /*   _MTU41     */
                    } BIT;                      /*              */
             } IPR13;                           /*              */
       union {                                  /* IPR14        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _PWM1:4;             /*   _PWM1      */
                    _UWORD _PWM2:4;             /*   _PWM2      */
                    _UWORD _ADC:4;              /*   _ADC       */
                    _UWORD :4;                  /*              */
                    } BIT;                      /*              */
             } IPR14;                           /*              */
       union {                                  /* IPR15        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _SSI0:4;             /*   _SSI0      */
                    _UWORD _SSI1:4;             /*   _SSI1      */
                    _UWORD _SSI2:4;             /*   _SSI2      */
                    _UWORD _SSI3:4;             /*   _SSI3      */
                    } BIT;                      /*              */
             } IPR15;                           /*              */
       union {                                  /* IPR16        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _SPDIF:4;            /*   _SPDIF     */
                    _UWORD _IIC30:4;            /*   _IIC30     */
                    _UWORD _IIC31:4;            /*   _IIC31     */
                    _UWORD _IIC32:4;            /*   _IIC32     */
                    } BIT;                      /*              */
             } IPR16;                           /*              */
       union {                                  /* IPR17        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _SCIF0:4;            /*   _SCIF0     */
                    _UWORD _SCIF1:4;            /*   _SCIF1     */
                    _UWORD _SCIF2:4;            /*   _SCIF2     */
                    _UWORD _SCIF3:4;            /*   _SCIF3     */
                    } BIT;                      /*              */
             } IPR17;                           /*              */
       union {                                  /* IPR18        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _SCIF4:4;            /*   _SCIF4     */
                    _UWORD _SCIF5:4;            /*   _SCIF5     */
                    _UWORD _SCIF6:4;            /*   _SCIF6     */
                    _UWORD _SCIF7:4;            /*   _SCIF7     */
                    } BIT;                      /*              */
             } IPR18;                           /*              */
       union {                                  /* IPR19        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _SIOF:4;             /*   _SIOF      */
                    _UWORD :4;                  /*              */
                    _UWORD _RSPI0:4;            /*   _RSPI0     */
                    _UWORD _RSPI1:4;            /*   _RSPI1     */
                    } BIT;                      /*              */
             } IPR19;                           /*              */
       union {                                  /* IPR20        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _RCAN0:4;            /*   _RCAN0     */
                    _UWORD _RCAN1:4;            /*   _RCAN1     */
                    _UWORD _IEB:4;              /*   _IEB       */
                    _UWORD _ROMDEC:4;           /*   _ROMDEC    */
                    } BIT;                      /*              */
             } IPR20;                           /*              */
       union {                                  /* IPR21        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _FLCTL:4;            /*   _FLCTL     */
                    _UWORD _SDHI:4;             /*   _SDHI      */
                    _UWORD _RTC:4;              /*   _RTC       */
                    _UWORD :4;                  /*              */
                    } BIT;                      /*              */
             } IPR21;                           /*              */
       union {                                  /* IPR22        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD _SRC0:4;             /*   _SRC0      */
                    _UWORD _SRC1:4;             /*   _SRC1      */
                    _UWORD :4;                  /*              */
                    _UWORD _DCU:4;              /*   _DCU       */
                    } BIT;                      /*              */
             } IPR22;                           /*              */
};                                              /*              */
struct st_ccnt {                                /* struct CCNT  */
       union {                                  /* CCR1         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :20;                /*              */
                    _UDWORD ICF:1;              /*   ICF        */
                    _UDWORD :2;                 /*              */
                    _UDWORD ICE:1;              /*   ICE        */
                    _UDWORD :4;                 /*              */
                    _UDWORD OCF:1;              /*   OCF        */
                    _UDWORD :1;                 /*              */
                    _UDWORD WT:1;               /*   WT         */
                    _UDWORD OCE:1;              /*   OCE        */
                    } BIT;                      /*              */
             } CCR1;                            /*              */
       union {                                  /* CCR2         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :15;                /*              */
                    _UDWORD LE:1;               /*   LE         */
                    _UDWORD :6;                 /*              */
                    _UDWORD W3LOAD:1;           /*   W3LOAD     */
                    _UDWORD W3LOCK:1;           /*   W3LOCK     */
                    _UDWORD :6;                 /*              */
                    _UDWORD W2LOAD:1;           /*   W2LOAD     */
                    _UDWORD W2LOCK:1;           /*   W2LOCK     */
                    } BIT;                      /*              */
             } CCR2;                            /*              */
};
union CSnBCR{                                   /* CSnBCR       */
      _UDWORD LONG;                             /*  Long Access */
      struct {                                  /*  Bit Access  */
             _UDWORD :1;                        /*              */
             _UDWORD IWW:3;                     /*   IWW        */
             _UDWORD IWRWD:3;                   /*   IWRWD      */
             _UDWORD IWRWS:3;                   /*   IWRWS      */
             _UDWORD IWRRD:3;                   /*   IWRRD      */
             _UDWORD IWRRS:3;                   /*   IWRRS      */
             _UDWORD :1;                        /*              */
             _UDWORD TYPE:3;                    /*   TYPE       */
             _UDWORD ENDIAN:1;                  /*   ENDIAN     */
             _UDWORD BSZ:2;                     /*   BSZ        */
             _UDWORD :9;                        /*              */
             } BIT;                             /*              */
};                                              /*              */
struct st_bsc {                                 /* struct BSC   */
       union {                                  /* CMNCR        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :20;                /*              */
                    _UDWORD BLOCK:1;            /*   BLOCK      */
                    _UDWORD DPRTY:2;            /*   DPRTY      */
                    _UDWORD DMAIW:3;            /*   DMAIW      */
                    _UDWORD DMAIWA:1;           /*   DMAIWA     */
                    _UDWORD :3;                 /*              */
                    _UDWORD HIZMEM:1;           /*   HIZMEM     */
                    _UDWORD HIZCNT:1;           /*   HIZCNT     */
                    } BIT;                      /*              */
             } CMNCR;                           /*              */
       union CSnBCR CS0BCR;                     /* CS0BCR       */
       union CSnBCR CS1BCR;                     /* CS1BCR       */
       union CSnBCR CS2BCR;                     /* CS2BCR       */
       union CSnBCR CS3BCR;                     /* CS3BCR       */
       union CSnBCR CS4BCR;                     /* CS4BCR       */
       union CSnBCR CS5BCR;                     /* CS5BCR       */
       union CSnBCR CS6BCR;                     /* CS6BCR       */
       
       _UBYTE wk0[8];                           /*              */
       union {                                  /* CS0WCR       */
             union {                            /* CS0WCR(NORMAL) */
                    _UDWORD LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           _UDWORD :11;         /*              */
                           _UDWORD BAS:1;       /*   BAS        */
                           _UDWORD :7;          /*              */
                           _UDWORD SW:2;        /*   SW         */
                           _UDWORD WR:4;        /*   WR         */
                           _UDWORD WM:1;        /*   WM         */
                           _UDWORD :4;          /*              */
                           _UDWORD HW:2;        /*   HW         */
                           } BIT;               /*              */
                    } NORMAL;                   /*              */
             union {                            /* CS0WCR(BROM_ASY) */
                    _UDWORD LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           _UDWORD :10;         /*              */
                           _UDWORD BST:2;       /*   BST        */
                           _UDWORD :2;          /*              */
                           _UDWORD BW:2;        /*   BW         */
                           _UDWORD :5;          /*              */
                           _UDWORD W:4;         /*   W          */
                           _UDWORD WM:1;        /*   WM         */
                           _UDWORD :6;          /*              */
                           } BIT;               /*              */
                    } BROM_ASY;                 /*              */
             union {                            /* CS0WCR(BROM_SY) */
                    _UDWORD LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           _UDWORD :14;         /*              */
                           _UDWORD BW:2;        /*   BW         */
                           _UDWORD :5;          /*              */
                           _UDWORD W:4;         /*   W          */
                           _UDWORD WM:1;        /*   WM         */
                           _UDWORD :6;          /*              */
                           } BIT;               /*              */
                    } BROM_SY;                  /*              */
             } CS0WCR;
       union {                                  /* CS1WCR       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :11;                /*              */
                    _UDWORD BAS:1;              /*   BAS        */
                    _UDWORD :1;                 /*              */
                    _UDWORD WW:3;               /*   WW         */
                    _UDWORD :3;                 /*              */
                    _UDWORD SW:2;               /*   SW         */
                    _UDWORD WR:4;               /*   WR         */
                    _UDWORD WM:1;               /*   WM         */
                    _UDWORD :4;                 /*              */
                    _UDWORD HW:2;               /*   HW         */
                    } BIT;                      /*              */
             } CS1WCR;                          /*              */
       union {                                  /* CS2WCR       */
             union {                            /* CS2WCR(NORMAL) */
                    _UDWORD LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           _UDWORD :11;         /*              */
                           _UDWORD BAS:1;       /*   BAS        */
                           _UDWORD :9;          /*              */
                           _UDWORD WR:4;        /*   WR         */
                           _UDWORD WM:1;        /*   WM         */
                           _UDWORD :6;          /*              */
                           } BIT;               /*              */
                    } NORMAL;                   /*              */
             union {                            /* CS2WCR(SDRAM) */
                    _UDWORD LONG;               /*  Long Access */
                    struct {                          /*  Bit Access  */
                           _UDWORD :23;         /*              */
                           _UDWORD A2CL:2;      /*   A2CL       */
                           _UDWORD :7;          /*              */
                           } BIT;               /*              */
                    } SDRAM;                    /*              */
             } CS2WCR;                          /*              */
       union {                                  /* CS3WCR       */
             union {                            /* CS3WCR(NORMAL) */
                    _UDWORD LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           _UDWORD :11;         /*              */
                           _UDWORD BAS:1;       /*   BAS        */
                           _UDWORD :9;          /*              */
                           _UDWORD WR:4;        /*   WR         */
                           _UDWORD WM:1;        /*   WM         */
                           _UDWORD :6;          /*              */
                           } BIT;               /*              */
                    } NORMAL;                   /*              */
             union {                            /* CS3WCR(SDRAM) */
                    _UDWORD LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           _UDWORD :17;         /*              */
                           _UDWORD WTRP:2;      /*   WTRP       */
                           _UDWORD :1;          /*              */
                           _UDWORD WTRCD:2;     /*   WTRCD      */
                           _UDWORD :1;          /*              */
                           _UDWORD A3CL:2;      /*   A3CL       */
                           _UDWORD :2;          /*              */
                           _UDWORD TRWL:2;      /*   TRWL       */
                           _UDWORD :1;          /*              */
                           _UDWORD WTRC:2;      /*   WTRC       */
                           } BIT;               /*              */
                    } SDRAM;                    /*              */
             } CS3WCR;                          /*              */
       union {                                  /* CS4WCR       */
              union {                           /* CS4WCR(NORMAL) */
                    _UDWORD LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           _UDWORD :11;         /*              */
                           _UDWORD BAS:1;       /*   BAS        */
                           _UDWORD :1;          /*              */
                           _UDWORD WW:3;        /*   WW         */
                           _UDWORD :3;          /*              */
                           _UDWORD SW:2;        /*   SW         */
                           _UDWORD WR:4;        /*   WR         */
                           _UDWORD WM:1;        /*   WM         */
                           _UDWORD :4;          /*              */
                           _UDWORD HW:2;        /*   HW         */
                           } BIT;               /*              */
                    } NORMAL;                   /*              */
              union {                           /* CS4WCR(BROM_ASY) */
                    _UDWORD LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           _UDWORD :10;         /*              */
                           _UDWORD BST:2;       /*   BST        */
                           _UDWORD :2;          /*              */
                           _UDWORD BW:2;        /*   BW         */
                           _UDWORD :3;          /*              */
                           _UDWORD SW:2;        /*   SW         */
                           _UDWORD W:4;         /*   W          */
                           _UDWORD WM:1;        /*   WM         */
                           _UDWORD :4;          /*              */
                           _UDWORD HW:2;        /*   HW         */
                           } BIT;               /*              */
                    } BROM_ASY;                 /*              */
             } CS4WCR;                          /*              */
       union {                                  /* CS5WCR       */
              union {                           /* CS5WCR(NORMAL) */
                    _UDWORD LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           _UDWORD :10;         /*              */
                           _UDWORD SZSEL:1;     /*   SZSEL      */
                           _UDWORD MPXWBAS:1;   /*   MPXW/BAS   */
                           _UDWORD :1;          /*              */
                           _UDWORD WW:3;        /*   WW         */
                           _UDWORD :3;          /*              */
                           _UDWORD SW:2;        /*   SW         */
                           _UDWORD WR:4;        /*   WR         */
                           _UDWORD WM:1;        /*   WM         */
                           _UDWORD :4;          /*              */
                           _UDWORD HW:2;        /*   HW         */
                           } BIT;               /*              */
                    } NORMAL;                   /*              */
              union {                           /* CS5WCR(PCMCIA) */
                    _UDWORD LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           _UDWORD :10;         /*              */
                           _UDWORD SA:2;        /*   SA         */
                           _UDWORD :5;          /*              */
                           _UDWORD TED:4;       /*   TED        */
                           _UDWORD PCW:4;       /*   PCW        */
                           _UDWORD WM:1;        /*   WM         */
                           _UDWORD :2;          /*              */
                           _UDWORD TEH:4;       /*   TEH        */
                           } BIT;               /*              */
                    } PCMCIA;                   /*              */
             } CS5WCR;                          /*              */
       union {                                  /* CS6WCR       */
              union {                           /* CS6WCR(NORMAL) */
                    _UDWORD LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           _UDWORD :11;         /*              */
                           _UDWORD BAS:1;       /*   BAS        */
                           _UDWORD :7;          /*              */
                           _UDWORD SW:2;        /*   SW         */
                           _UDWORD WR:4;        /*   WR         */
                           _UDWORD WM:1;        /*   WM         */
                           _UDWORD :4;          /*              */
                           _UDWORD HW:2;        /*   HW         */
                           } BIT;               /*              */
                    } NORMAL;                   /*              */
              union {                           /* CS6WCR(PCMCIA) */
                    _UDWORD LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           _UDWORD :10;         /*              */
                           _UDWORD SA:2;        /*   SA         */
                           _UDWORD :5;          /*              */
                           _UDWORD TED:4;       /*   TED        */
                           _UDWORD PCW:4;       /*   PCW        */
                           _UDWORD WM:1;        /*   WM         */
                           _UDWORD :2;          /*              */
                           _UDWORD TEH:4;       /*   TEH        */
                           } BIT;               /*              */
                    } PCMCIA;                   /*              */
             } CS6WCR;                          /*              */
       _UBYTE wk1[8];                           /*              */
       union {                                  /* SDCR         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :11;                /*              */
                    _UDWORD A2ROW:2;            /*   A2ROW      */
                    _UDWORD :1;                 /*              */
                    _UDWORD A2COL:2;            /*   A2COL      */
                    _UDWORD :2;                 /*              */
                    _UDWORD DEEP:1;             /*   DEEP       */
                    _UDWORD SLOW:1;             /*   SLOW       */
                    _UDWORD RFSH:1;             /*   RFSH       */
                    _UDWORD RMODE:1;            /*   RMODE      */
                    _UDWORD PDOWN:1;            /*   PDOWN      */
                    _UDWORD BACTV:1;            /*   BACTV      */
                    _UDWORD :3;                 /*              */
                    _UDWORD A3ROW:2;            /*   A3ROW      */
                    _UDWORD :1;                 /*              */
                    _UDWORD A3COL:2;            /*   A3COL      */
                    } BIT;                      /*              */
             } SDCR;                            /*              */
       union {                                  /* RTCSR        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :24;                /*              */
                    _UDWORD CMF:1;              /*   CMF        */
                    _UDWORD CMIE:1;             /*   CMIE       */
                    _UDWORD CKS:3;              /*   CKS        */
                    _UDWORD RRC:3;              /*   RRC        */
                    } BIT;                      /*              */
             } RTCSR;                           /*              */
       union {                                  /* RTCNT        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RTCNT;                           /*              */
       union {                                  /* RTCOR        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RTCOR;                           /*              */
       _UBYTE wk2[6064];                        /*              */
       union {                                  /* ACSWR        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :28;                /*              */
                    _UDWORD ACOSW:4;            /*   ACOSW      */
             } BIT;                             /*              */
       } ACSWR;                                 /*              */
       _UBYTE wk3[1004];                        /*              */
       _UBYTE ACKEYR;                           /* ACKEYR       */
};                                              /*              */
struct st_dmac {                                /* struct DMAC  */
       union {                                  /* SAR0         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR0;                            /*              */
       union {                                  /* DAR0         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR0;                            /*              */
       union {                                  /* DMATCR0      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR0;                         /*              */
       union {                                  /* CHCR0        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE DO:1;                /*   DO         */
                    _UBYTE TL:1;                /*   TL         */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE AM:1;                /*   AM         */
                    _UBYTE AL:1;                /*   AL         */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE DL:1;                /*   DL         */
                    _UBYTE DS:1;                /*   DS         */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR0;                           /*              */
       union {                                  /* SAR1         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR1;                            /*              */
       union {                                  /* DAR1         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR1;                            /*              */
       union {                                  /* DMATCR1      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR1;                         /*              */
       union {                                  /* CHCR1        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE DO:1;                /*   DO         */
                    _UBYTE TL:1;                /*   TL         */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE AM:1;                /*   AM         */
                    _UBYTE AL:1;                /*   AL         */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE DL:1;                /*   DL         */
                    _UBYTE DS:1;                /*   DS         */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR1;                           /*              */
       union {                                  /* SAR2         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR2;                            /*              */
       union {                                  /* DAR2         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR2;                            /*              */
       union {                                  /* DMATCR2      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR2;                         /*              */
       union {                                  /* CHCR2        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR2;                           /*              */
       union {                                  /* SAR3         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR3;                            /*              */
       union {                                  /* DAR3         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR3;                            /*              */
       union {                                  /* DMATCR3      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR3;                         /*              */
       union {                                  /* CHCR3        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR3;                           /*              */
       union {                                  /* SAR4         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR4;                            /*              */
       union {                                  /* DAR4         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR4;                            /*              */
       union {                                  /* DMATCR4      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR4;                         /*              */
       union {                                  /* CHCR4        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR4;                           /*              */
       union {                                  /* SAR5         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR5;                            /*              */
       union {                                  /* DAR5         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR5;                            /*              */
       union {                                  /* DMATCR5      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR5;                         /*              */
       union {                                  /* CHCR5        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR5;                           /*              */
       union {                                  /* SAR6         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR6;                            /*              */
       union {                                  /* DAR6         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR6;                            /*              */
       union {                                  /* DMATCR6      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR6;                         /*              */
       union {                                  /* CHCR6        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR6;                           /*              */
       union {                                  /* SAR7         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR7;                            /*              */
       union {                                  /* DAR7         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR7;                            /*              */
       union {                                  /* DMATCR7      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR7;                         /*              */
       union {                                  /* CHCR7        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR7;                           /*              */
       union {                                  /* SAR8         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR8;                            /*              */
       union {                                  /* DAR8         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR8;                            /*              */
       union {                                  /* DMATCR8      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR8;                         /*              */
       union {                                  /* CHCR8        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR8;                           /*              */
       union {                                  /* SAR9         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR9;                            /*              */
       union {                                  /* DAR9         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR9;                            /*              */
       union {                                  /* DMATCR9      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR9;                         /*              */
       union {                                  /* CHCR9        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR9;                           /*              */
       union {                                  /* SAR10        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR10;                           /*              */
       union {                                  /* DAR10        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR10;                           /*              */
       union {                                  /* DMATCR10     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR10;                        /*              */
       union {                                  /* CHCR10       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR10;                          /*              */
       union {                                  /* SAR11        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR11;                           /*              */
       union {                                  /* DAR11        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR11;                           /*              */
       union {                                  /* DMATCR11     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR11;                        /*              */
       union {                                  /* CHCR11       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR11;                          /*              */
       union {                                  /* SAR12        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR12;                           /*              */
       union {                                  /* DAR12        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR12;                           /*              */
       union {                                  /* DMATCR12     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR12;                        /*              */
       union {                                  /* CHCR12       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR12;                          /*              */
       union {                                  /* SAR13        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR13;                           /*              */
       union {                                  /* DAR13        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR13;                           /*              */
       union {                                  /* DMATCR13     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR13;                        /*              */
       union {                                  /* CHCR13       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR13;                          /*              */
       union {                                  /* SAR14        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR14;                           /*              */
       union {                                  /* DAR14        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR14;                           /*              */
       union {                                  /* DMATCR14     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR14;                        /*              */
       union {                                  /* CHCR14       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR14;                          /*              */
       union {                                  /* SAR15        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } SAR15;                           /*              */
       union {                                  /* DAR15        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DAR15;                           /*              */
       union {                                  /* DMATCR15     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } DMATCR15;                        /*              */
       union {                                  /* CHCR15       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE TC:1;                /*   TC         */
                    _UBYTE :1;                  /*              */
                    _UBYTE RLDSAR:1;            /*   RLDSAR     */
                    _UBYTE RLDDAR:1;            /*   RLDDAR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE DAF:1;               /*   DAF        */
                    _UBYTE SAF:1;               /*   SAF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TEMASK:1;            /*   TEMASK     */
                    _UBYTE HE:1;                /*   HE         */
                    _UBYTE HIE:1;               /*   HIE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE DM:2;                /*   DM         */
                    _UBYTE SM:2;                /*   SM         */
                    _UBYTE RS:4;                /*   RS         */
                    _UBYTE :1;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE TB:1;                /*   TB         */
                    _UBYTE TS:2;                /*   TS         */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE TE:1;                /*   TE         */
                    _UBYTE DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR15;                          /*              */
       union {                                  /* RSAR0        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR0;                           /*              */
       union {                                  /* RDAR0        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR0;                           /*              */
       union {                                  /* RDMATCR0     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR0;                        /*              */
       _UBYTE wk0[4];                           /*              */
       union {                                  /* RSAR1        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR1;                           /*              */
       union {                                  /* RDAR1        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR1;                           /*              */
       union {                                  /* RDMATCR1     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR1;                        /*              */
       _UBYTE wk1[4];                           /*              */
       union {                                  /* RSAR2        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR2;                           /*              */
       union {                                  /* RDAR2        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR2;                           /*              */
       union {                                  /* RDMATCR2     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR2;                        /*              */
       _UBYTE wk2[4];                           /*              */
       union {                                  /* RSAR3        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR3;                           /*              */
       union {                                  /* RDAR3        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR3;                           /*              */
       union {                                  /* RDMATCR3     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR3;                        /*              */
       _UBYTE wk3[4];                           /*              */
       union {                                  /* RSAR4        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR4;                           /*              */
       union {                                  /* RDAR4        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR4;                           /*              */
       union {                                  /* RDMATCR4     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR4;                        /*              */
       _UBYTE wk4[4];                           /*              */
       union {                                  /* RSAR5        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR5;                           /*              */
       union {                                  /* RDAR5        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR5;                           /*              */
       union {                                  /* RDMATCR5     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR5;                        /*              */
       _UBYTE wk5[4];                           /*              */
       union {                                  /* RSAR6        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR6;                           /*              */
       union {                                  /* RDAR6        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR6;                           /*              */
       union {                                  /* RDMATCR6     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR6;                        /*              */
       _UBYTE wk6[4];                           /*              */
       union {                                  /* RSAR7        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR7;                           /*              */
       union {                                  /* RDAR7        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR7;                           /*              */
       union {                                  /* RDMATCR7     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR7;                        /*              */
       _UBYTE wk7[4];                           /*              */
       union {                                  /* RSAR8        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR8;                           /*              */
       union {                                  /* RDAR8        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR8;                           /*              */
       union {                                  /* RDMATCR8     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR8;                        /*              */
       _UBYTE wk8[4];                           /*              */
       union {                                  /* RSAR9        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR9;                           /*              */
       union {                                  /* RDAR9        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR9;                           /*              */
       union {                                  /* RDMATCR9     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR9;                        /*              */
       _UBYTE wk9[4];                           /*              */
       union {                                  /* RSAR10       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR10;                          /*              */
       union {                                  /* RDAR10       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR10;                          /*              */
       union {                                  /* RDMATCR10    */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR10;                       /*              */
       _UBYTE wk10[4];                          /*              */
       union {                                  /* RSAR11       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR11;                          /*              */
       union {                                  /* RDAR11       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR11;                          /*              */
       union {                                  /* RDMATCR11    */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR11;                       /*              */
       _UBYTE wk11[4];                          /*              */
       union {                                  /* RSAR12       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR12;                          /*              */
       union {                                  /* RDAR12       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR12;                          /*              */
       union {                                  /* RDMATCR12    */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR12;                       /*              */
       _UBYTE wk12[4];                          /*              */
       union {                                  /* RSAR13       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR13;                          /*              */
       union {                                  /* RDAR13       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR13;                          /*              */
       union {                                  /* RDMATCR13    */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR13;                       /*              */
       _UBYTE wk13[4];                          /*              */
       union {                                  /* RSAR14       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR14;                          /*              */
       union {                                  /* RDAR14       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR14;                          /*              */
       union {                                  /* RDMATCR14    */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR14;                       /*              */
       _UBYTE wk14[4];                          /*              */
       union {                                  /* RSAR15       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RSAR15;                          /*              */
       union {                                  /* RDAR15       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDAR15;                          /*              */
       union {                                  /* RDMATCR15    */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD D:32;               /*   D          */
                    } BIT;                      /*              */
             } RDMATCR15;                       /*              */
       _UBYTE wk15[4];                          /*              */
       union {                                  /* DMAOR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UWORD :2;                  /*              */
                    _UWORD CMS:2;               /*   CMS        */
                    _UWORD :2;                  /*              */
                    _UWORD PR:2;                /*   PR         */
                    _UWORD :5;                  /*              */
                    _UWORD AE:1;                /*   AE         */
                    _UWORD NMIF:1;              /*   NMIF       */
                    _UWORD DME:1;               /*   DME        */
                    } BIT;                      /*              */
             } DMAOR;                           /*              */
       _UBYTE wk16[254];                        /*              */
       union {                                  /* DMARS0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UWORD CH1:8;               /*   CH1        */
                    _UWORD CH0:8;               /*   CH0        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UWORD CH1MID:6;            /*   CH1MID     */
                    _UWORD CH1RID:2;            /*   CH1RID     */
                    _UWORD CH0MID:6;            /*   CH0MID     */
                    _UWORD CH0RID:2;            /*   CH0RID     */
                    } BIT;                      /*              */
             } DMARS0;                          /*              */
       _UBYTE wk17[2];                          /*              */
       union {                                  /* DMARS1       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UWORD CH3:8;               /*   CH3        */
                    _UWORD CH2:8;               /*   CH2        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UWORD CH3MID:6;            /*   CH3ID     */
                    _UWORD CH3RID:2;            /*   CH3RID     */
                    _UWORD CH2MID:6;            /*   CH2MID     */
                    _UWORD CH2RID:2;            /*   CH2RID     */
                    } BIT;                      /*              */
             } DMARS1;                          /*              */
       _UBYTE wk18[2];                          /*              */
       union {                                  /* DMARS2       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UWORD CH5:8;               /*   CH5        */
                    _UWORD CH4:8;               /*   CH4        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UWORD CH5MID:6;            /*   CH5MID     */
                    _UWORD CH5RID:2;            /*   CH5RID     */
                    _UWORD CH4MID:6;            /*   CH4MID     */
                    _UWORD CH4RID:2;            /*   CH4RID     */
                    } BIT;                      /*              */
             } DMARS2;                          /*              */
       _UBYTE wk19[2];                          /*              */
       union {                                  /* DMARS3       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UWORD CH7:8;               /*   CH7        */
                    _UWORD CH6:8;               /*   CH6        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UWORD CH7MID:6;            /*   CH7MID     */
                    _UWORD CH7RID:2;            /*   CH7RID     */
                    _UWORD CH6MID:6;            /*   CH6MID     */
                    _UWORD CH6RID:2;            /*   CH6RID     */
                    } BIT;                      /*              */
             } DMARS3;                          /*              */
       _UBYTE wk20[2];                          /*              */
       union {                                  /* DMARS4       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UWORD CH9:8;               /*   CH9        */
                    _UWORD CH8:8;               /*   CH8        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UWORD CH9MID:6;            /*   CH9MID     */
                    _UWORD CH9RID:2;            /*   CH9RID     */
                    _UWORD CH8MID:6;            /*   CH8MID     */
                    _UWORD CH8RID:2;            /*   CH8RID     */
                    } BIT;                      /*              */
             } DMARS4;                          /*              */
       _UBYTE wk21[2];                          /*              */
       union {                                  /* DMARS5       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UWORD CH11:8;              /*   CH11       */
                    _UWORD CH10:8;              /*   CH10       */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UWORD CH11MID:6;           /*   CH11MID    */
                    _UWORD CH11RID:2;           /*   CH11RID    */
                    _UWORD CH10MID:6;           /*   CH10MID    */
                    _UWORD CH10RID:2;           /*   CH10RID    */
                    } BIT;                      /*              */
             } DMARS5;                          /*              */
       _UBYTE wk22[2];                          /*              */
       union {                                  /* DMARS6       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UWORD CH13:8;              /*   CH13       */
                    _UWORD CH12:8;              /*   CH12       */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UWORD CH13MID:6;           /*   CH13MID    */
                    _UWORD CH13RID:2;           /*   CH13RID    */
                    _UWORD CH12MID:6;           /*   CH12MID    */
                    _UWORD CH12RID:2;           /*   CH12RID    */
                    } BIT;                      /*              */
             } DMARS6;                          /*              */
       _UBYTE wk23[2];                          /*              */
       union {                                  /* DMARS7       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UWORD CH15:8;              /*   CH15       */
                    _UWORD CH14:8;              /*   CH14       */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UWORD CH15MID:6;           /*   CH15MID    */
                    _UWORD CH15RID:2;           /*   CH15RID    */
                    _UWORD CH14MID:6;           /*   CH14MID    */
                    _UWORD CH14RID:2;           /*   CH14RID    */
                    } BIT;                      /*              */
             } DMARS7;                          /*              */
};                                              /*              */
struct st_mtu2 {                                /* struct MTU2  */
       union {                                  /* TCR_2        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE CCLR:2;              /*   CCLR       */
                    _UBYTE CKEG:2;              /*   CKEG       */
                    _UBYTE TPSC:3;              /*   TPSC       */
                    } BIT;                      /*              */
             } TCR_2;                           /*              */
       union {                                  /* TMDR_2       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :4;                  /*              */
                    _UBYTE MD:4;                /*   MD         */
                    } BIT;                      /*              */
             } TMDR_2;                          /*              */
       union {                                  /* TIOR_2       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE IOB:4;               /*   IOB        */
                    _UBYTE IOA:4;               /*   IOA        */
                    } BIT;                      /*              */
             } TIOR_2;                          /*              */
       _UBYTE wk0[1];                           /*              */
       union {                                  /* TIER_2       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TTGE:1;              /*   TTGE       */
                    _UBYTE :1;                  /*              */
                    _UBYTE TCIEU:1;             /*   TCIEU      */
                    _UBYTE TCIEV:1;             /*   TCIEV      */
                    _UBYTE :2;                  /*              */
                    _UBYTE TGIEB:1;             /*   TGIEB      */
                    _UBYTE TGIEA:1;             /*   TGIEA      */
                    } BIT;                      /*              */
             } TIER_2;                          /*              */
       union {                                  /* TSR_2        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TCFD:1;              /*   TCFD       */
                    _UBYTE :1;                  /*              */
                    _UBYTE TCFU:1;              /*   TCFU       */
                    _UBYTE TCFV:1;              /*   TCFV       */
                    _UBYTE TGFD:1;              /*   TGFD       */
                    _UBYTE TGFC:1;              /*   TGFC       */
                    _UBYTE TGFB:1;              /*   TGFB       */
                    _UBYTE TGFA:1;              /*   TGFA       */
                    } BIT;                      /*              */
             } TSR_2;                           /*              */
       union {                                  /* TCNT_2       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TCNT_2;                          /*              */
       union {                                  /* TGRA_2       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRA_2;                          /*              */
       union {                                  /* TGRB_2       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRB_2;                          /*              */
       _UBYTE wk1[500];                         /*              */
       union {                                  /* TCR_3        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE CCLR:3;              /*   CCLR       */
                    _UBYTE CKEG:2;              /*   CKEG       */
                    _UBYTE TPSC:3;              /*   TPSC       */
                    } BIT;                      /*              */
             } TCR_3;                           /*              */
       union {                                  /* TCR_4        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE CCLR:3;              /*   CCLR       */
                    _UBYTE CKEG:2;              /*   CKEG       */
                    _UBYTE TPSC:3;              /*   TPSC       */
                    } BIT;                      /*              */
             } TCR_4;                           /*              */
       union {                                  /* TMDR_3       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE BFB:1;               /*   BFB        */
                    _UBYTE BFA:1;               /*   BFA        */
                    _UBYTE MD:4;                /*   MD         */
                    } BIT;                      /*              */
             } TMDR_3;                          /*              */
       union {                                  /* TMDR_4       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE BFB:1;               /*   BFB        */
                    _UBYTE BFA:1;               /*   BFA        */
                    _UBYTE MD:4;                /*   MD         */
                    } BIT;                      /*              */
             } TMDR_4;                          /*              */
       union {                                  /* TIORH_3      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE IOB:4;               /*   IOB        */
                    _UBYTE IOA:4;               /*   IOA        */
                    } BIT;                      /*              */
             } TIORH_3;                         /*              */
       union {                                  /* TIORL_3      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE IOD:4;               /*   IOD        */
                    _UBYTE IOC:4;               /*   IOC        */
                    } BIT;                      /*              */
             } TIORL_3;                         /*              */
       union {                                  /* TIORH_4      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE IOB:4;               /*   IOB        */
                    _UBYTE IOA:4;               /*   IOA        */
                    } BIT;                      /*              */
             } TIORH_4;                         /*              */
       union {                                  /* TIORL_4      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE IOD:4;               /*   IOD        */
                    _UBYTE IOC:4;               /*   IOC        */
                    } BIT;                      /*              */
             } TIORL_4;                         /*              */
       union {                                  /* TIER_3       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TTGE:1;              /*   TTGE       */
                    _UBYTE :2;                  /*              */
                    _UBYTE TCIEV:1;             /*   TCIEV      */
                    _UBYTE TGIED:1;             /*   TGIED      */
                    _UBYTE TGIEC:1;             /*   TGIEC      */
                    _UBYTE TGIEB:1;             /*   TGIEB      */
                    _UBYTE TGIEA:1;             /*   TGIEA      */
                    } BIT;                      /*              */
             } TIER_3;                          /*              */
       union {                                  /* TIER_4       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TTGE:1;              /*   TTGE       */
                    _UBYTE TTGE2:1;             /*   TTGE2      */
                    _UBYTE :1;                  /*              */
                    _UBYTE TCIEV:1;             /*   TCIEV      */
                    _UBYTE TGIED:1;             /*   TGIED      */
                    _UBYTE TGIEC:1;             /*   TGIEC      */
                    _UBYTE TGIEB:1;             /*   TGIEB      */
                    _UBYTE TGIEA:1;             /*   TGIEA      */
                    } BIT;                      /*              */
             } TIER_4;                          /*              */
       union {                                  /* TOER         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE OE4D:1;              /*   OE4D       */
                    _UBYTE OE4C:1;              /*   OE4C       */
                    _UBYTE OE3D:1;              /*   OE3D       */
                    _UBYTE OE4B:1;              /*   OE4B       */
                    _UBYTE OE4A:1;              /*   OE4A       */
                    _UBYTE OE3B:1;              /*   OE3B       */
                    } BIT;                      /*              */
             } TOER;                            /*              */
       _UBYTE wk2[2];                           /*              */
       union {                                  /* TGCR         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE BDC:1;               /*   BDC        */
                    _UBYTE N:1;                 /*   N          */
                    _UBYTE P:1;                 /*   P          */
                    _UBYTE FB:1;                /*   FB         */
                    _UBYTE WF:1;                /*   WF         */
                    _UBYTE VF:1;                /*   VF         */
                    _UBYTE UF:1;                /*   UF         */
                    } BIT;                      /*              */
             } TGCR;                            /*              */
       union {                                  /* TOCR1        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE PSYE:1;              /*   PSYE       */
                    _UBYTE :2;                  /*              */
                    _UBYTE TOCL:1;              /*   TOCL       */
                    _UBYTE TOCS:1;              /*   TOCS       */
                    _UBYTE OLSN:1;              /*   OLSN       */
                    _UBYTE OLSP:1;              /*   OLSP       */
                    } BIT;                      /*              */
             } TOCR1;                           /*              */
       union {                                  /* TOCR2        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE BF:2;                /*   BF         */
                    _UBYTE OLS3N:1;             /*   OLS3N      */
                    _UBYTE OLS3P:1;             /*   OLS3P      */
                    _UBYTE OLS2N:1;             /*   OLS2N      */
                    _UBYTE OLS2P:1;             /*   OLS2P      */
                    _UBYTE OLS1N:1;             /*   OLS1N      */
                    _UBYTE OLS1P:1;             /*   OLS1P      */
                    } BIT;                      /*              */
             } TOCR2;                           /*              */
       union {                                  /* TCNT_3       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TCNT_3;                          /*              */
       union {                                  /* TCNT_4       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TCNT_4;                          /*              */
       union {                                  /* TCDR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TCDR;                            /*              */
       union {                                  /* TDDR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TDDR;                            /*              */
       union {                                  /* TGRA_3       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRA_3;                          /*              */
       union {                                  /* TGRB_3       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRB_3;                          /*              */
       union {                                  /* TGRA_4       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRA_4;                          /*              */
       union {                                  /* TGRB_4       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRB_4;                          /*              */
       union {                                  /* TCNTS       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TCNTS;                           /*              */
       union {                                  /* TCBR         */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TCBR;                            /*              */
       union {                                  /* TGRC_3       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRC_3;                          /*              */
       union {                                  /* TGRD_3       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRD_3;                          /*              */
       union {                                  /* TGRC_4       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRC_4;                          /*              */
       union {                                  /* TGRD_4       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRD_4;                          /*              */
       union {                                  /* TSR_3        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TCFD:1;              /*   TCFD       */
                    _UBYTE :2;                  /*              */
                    _UBYTE TCFV:1;              /*   TCFV       */
                    _UBYTE TGFD:1;              /*   TGFD       */
                    _UBYTE TGFC:1;              /*   TGFC       */
                    _UBYTE TGFB:1;              /*   TGFB       */
                    _UBYTE TGFA:1;              /*   TGFA       */
                    } BIT;                      /*              */
             } TSR_3;                           /*              */
       union {                                  /* TSR_4        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TCFD:1;              /*   TCFD       */
                    _UBYTE :2;                  /*              */
                    _UBYTE TCFV:1;              /*   TCFV       */
                    _UBYTE TGFD:1;              /*   TGFD       */
                    _UBYTE TGFC:1;              /*   TGFC       */
                    _UBYTE TGFB:1;              /*   TGFB       */
                    _UBYTE TGFA:1;              /*   TGFA       */
                    } BIT;                      /*              */
             } TSR_4;                           /*              */
       _UBYTE wk3[2];                           /*              */
       union {                                  /* TITCR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE T3AEN:1;             /*   T3AEN      */
                    _UBYTE _3ACOR:3;            /*   _3ACOR     */
                    _UBYTE T4VEN:1;             /*   T4VEN      */
                    _UBYTE _4VCOR:3;            /*   _4VCOR     */
                    } BIT;                      /*              */
             } TITCR;                           /*              */
       union {                                  /* TITCNT       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE _3ACNT:3;            /*   _3ACNT     */
                    _UBYTE :1;                  /*              */
                    _UBYTE _4VCNT:3;            /*   _4VCNT     */
                    } BIT;                      /*              */
             } TITCNT;                          /*              */
       union {                                  /* TBTER        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :6;                  /*              */
                    _UBYTE BTE:2;               /*   BTE        */
                    } BIT;                      /*              */
             } TBTER;                           /*              */
       _UBYTE wk4[1];                           /*              */
       union {                                  /* TDER         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :7;                  /*              */
                    _UBYTE TDER:1;              /*   TDER       */
                    } BIT;                      /*              */
             } TDER;                            /*              */
       _UBYTE wk5[1];                           /*              */
       union {                                  /* TOLBR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE OLS3N:1;             /*   OLS3N      */
                    _UBYTE OLS3P:1;             /*   OLS3P      */
                    _UBYTE OLS2N:1;             /*   OLS2N      */
                    _UBYTE OLS2P:1;             /*   OLS2P      */
                    _UBYTE OLS1N:1;             /*   OLS1N      */
                    _UBYTE OLS1P:1;             /*   OLS1P      */
                    } BIT;                      /*              */
             } TOLBR;                           /*              */
       _UBYTE wk6[1];                           /*              */
       union {                                  /* TBTM_3       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :6;                  /*              */
                    _UBYTE TTSB:1;              /*   TTSB       */
                    _UBYTE TTSA:1;              /*   TTSA       */
                    } BIT;                      /*              */
             } TBTM_3;                          /*              */
       union {                                  /* TBTM_4       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :6;                  /*              */
                    _UBYTE TTSB:1;              /*   TTSB       */
                    _UBYTE TTSA:1;              /*   TTSA       */
                    } BIT;                      /*              */
             } TBTM_4;                          /*              */
       _UBYTE wk7[6];                           /*              */
       union {                                  /* TADCR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BF:2;                /*   BF         */
                    _UWORD :6;                  /*              */
                    _UWORD UT4AE:1;             /*   UT4AE      */
                    _UWORD DT4AE:1;             /*   DT4AE      */
                    _UWORD UT4BE:1;             /*   UT4BE      */
                    _UWORD DT4BE:1;             /*   DT4BE      */
                    _UWORD ITA3AE:1;            /*   ITA3AE     */
                    _UWORD ITA4VE:1;            /*   ITA4VE     */
                    _UWORD ITB3AE:1;            /*   ITB3AE     */
                    _UWORD ITB4VE:1;            /*   ITB4VE     */
                    } BIT;                      /*              */
             } TADCR;                           /*              */
       _UBYTE wk8[2];                           /*              */
       union {                                  /* TADCORA_4    */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TADCORA_4;                       /*              */
       union {                                  /* TADCORB_4    */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TADCORB_4;                       /*              */
       union {                                  /* TADCOBRA_4   */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TADCOBRA_4;                      /*              */
       union {                                  /* TADCOBRB_4   */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TADCOBRB_4;                      /*              */
       _UBYTE wk9[20];                          /*              */
       union {                                  /* TWCR         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE CCE:1;               /*   CCE        */
                    _UBYTE :6;                  /*              */
                    _UBYTE WRE:1;               /*   WRE        */
                    } BIT;                      /*              */
             } TWCR;                            /*              */
       _UBYTE wk10[31];                         /*              */
       union {                                  /* TSTR         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE CST4:1;              /*   CST4       */
                    _UBYTE CST3:1;              /*   CST3       */
                    _UBYTE :3;                  /*              */
                    _UBYTE CST2:1;              /*   CST2       */
                    _UBYTE CST1:1;              /*   CST1       */
                    _UBYTE CST0:1;              /*   CST0       */
                    } BIT;                      /*              */
             } TSTR;                            /*              */
       union {                                  /* TSYR         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE SYNC4:1;             /*   SYNC4      */
                    _UBYTE SYNC3:1;             /*   SYNC3      */
                    _UBYTE :3;                  /*              */
                    _UBYTE SYNC2:1;             /*   SYNC2      */
                    _UBYTE SYNC1:1;             /*   SYNC1      */
                    _UBYTE SYNC0:1;             /*   SYNC0      */
                    } BIT;                      /*              */
             } TSYR;                            /*              */
       _UBYTE wk11[2];                          /*              */
       union {                                  /* TRWER        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :7;                  /*              */
                    _UBYTE RWE:1;               /*   RWE        */
                    } BIT;                      /*              */
             } TRWER;                           /*              */
       _UBYTE wk12[123];                        /*              */
       union {                                  /* TCR_0        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE CCLR:3;              /*   CCLR       */
                    _UBYTE CKEG:2;              /*   CKEG       */
                    _UBYTE TPSC:3;              /*   TPSC       */
                    } BIT;                      /*              */
             } TCR_0;                           /*              */
       union {                                  /* TMDR_0       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE BFE:1;               /*   BFE        */
                    _UBYTE BFB:1;               /*   BFB        */
                    _UBYTE BFA:1;               /*   BFA        */
                    _UBYTE MD:4;                /*   MD         */
                    } BIT;                      /*              */
             } TMDR_0;                          /*              */
       union {                                  /* TIORH_0      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE IOB:4;               /*   IOB        */
                    _UBYTE IOA:4;               /*   IOA        */
                    } BIT;                      /*              */
             } TIORH_0;                         /*              */
       union {                                  /* TIORL_0      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE IOD:4;               /*   IOD        */
                    _UBYTE IOC:4;               /*   IOC        */
                    } BIT;                      /*              */
             } TIORL_0;                         /*              */
       union {                                  /* TIER_0       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TTGE:1;              /*   TTGE       */
                    _UBYTE :2;                  /*              */
                    _UBYTE TCIEV:1;             /*   TCIEV      */
                    _UBYTE TGIED:1;             /*   TGIED      */
                    _UBYTE TGIEC:1;             /*   TGIEC      */
                    _UBYTE TGIEB:1;             /*   TGIEB      */
                    _UBYTE TGIEA:1;             /*   TGIEA      */
                    } BIT;                      /*              */
             } TIER_0;                          /*              */
       union {                                  /* TSR_0        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TCFD:1;              /*   TCFD       */
                    _UBYTE :2;                  /*              */
                    _UBYTE TCFV:1;              /*   TCFV       */
                    _UBYTE TGFD:1;              /*   TGFD       */
                    _UBYTE TGFC:1;              /*   TGFC       */
                    _UBYTE TGFB:1;              /*   TGFB       */
                    _UBYTE TGFA:1;              /*   TGFA       */
                    } BIT;                      /*              */
             } TSR_0;                           /*              */
       union {                                  /* TCNT_0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TCNT_0;                          /*              */
       union {                                  /* TGRA_0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRA_0;                          /*              */
       union {                                  /* TGRB_0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRB_0;                          /*              */
       union {                                  /* TGRC_0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRC_0;                          /*              */
       union {                                  /* TGRD_0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRD_0;                          /*              */
       _UBYTE wk13[16];                         /*              */
       union {                                  /* TGRE_0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRE_0;                          /*              */
       union {                                  /* TGRF_0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRF_0;                          /*              */
       union {                                  /* TIER2_0      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TTGE2:1;             /*   TTGE2      */
                    _UBYTE :5;                  /*              */
                    _UBYTE TGIEF:1;             /*   TGIEF      */
                    _UBYTE TGIEE:1;             /*   TGIEE      */
                    } BIT;                      /*              */
             } TIER2_0;                         /*              */
       union {                                  /* TSR2_0       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :6;                  /*              */
                    _UBYTE TGFF:1;              /*   TGFF       */
                    _UBYTE TGFE:1;              /*   TGFE       */
                    } BIT;                      /*              */
             } TSR2_0;                          /*              */
       union {                                  /* TBTM_0       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :5;                  /*              */
                    _UBYTE TTSE:1;              /*   TTSE       */
                    _UBYTE TTSB:1;              /*   TTSB       */
                    _UBYTE TTSA:1;              /*   TTSA       */
                    } BIT;                      /*              */
             } TBTM_0;                          /*              */
       _UBYTE wk14[89];                         /*              */
       union {                                  /* TCR_1        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE CCLR:2;              /*   CCLR       */
                    _UBYTE CKEG:2;              /*   CKEG       */
                    _UBYTE TPSC:3;              /*   TPSC       */
                    } BIT;                      /*              */
             } TCR_1;                           /*              */
       union {                                  /* TMDR_1       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :4;                  /*              */
                    _UBYTE MD:4;                /*   MD         */
                    } BIT;                      /*              */
             } TMDR_1;                          /*              */
       union {                                  /* TIOR_1       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE IOB:4;               /*   IOB        */
                    _UBYTE IOA:4;               /*   IOA        */
                    } BIT;                      /*              */
             } TIOR_1;                          /*              */
       _UBYTE wk15[1];                          /*              */
       union {                                  /* TIER_1       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TTGE:1;              /*   TTGE       */
                    _UBYTE :1;                  /*              */
                    _UBYTE TCIEU:1;             /*   TCIEU      */
                    _UBYTE TCIEV:1;             /*   TCIEV      */
                    _UBYTE :2;                  /*              */
                    _UBYTE TGIEB:1;             /*   TGIEB      */
                    _UBYTE TGIEA:1;             /*   TGIEA      */
                    } BIT;                      /*              */
             } TIER_1;                          /*              */
       union {                                  /* TSR_1        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TCFD:1;              /*   TCFD       */
                    _UBYTE :1;                  /*              */
                    _UBYTE TCFU:1;              /*   TCFU       */
                    _UBYTE TCFV:1;              /*   TCFV       */
                    _UBYTE TGFD:1;              /*   TGFD       */
                    _UBYTE TGFC:1;              /*   TGFC       */
                    _UBYTE TGFB:1;              /*   TGFB       */
                    _UBYTE TGFA:1;              /*   TGFA       */
                    } BIT;                      /*              */
             } TSR_1;                           /*              */
       union {                                  /* TCNT_1       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TCNT_1;                          /*              */
       union {                                  /* TGRA_1       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRA_1;                          /*              */
       union {                                  /* TGRB_1       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } TGRB_1;                          /*              */
       _UBYTE wk16[4];                          /*              */
       union {                                  /* TICCR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :4;                  /*              */
                    _UBYTE I2BE:1;              /*   I2BE       */
                    _UBYTE I2AE:1;              /*   I2AE       */
                    _UBYTE I1BE:1;              /*   I1BE       */
                    _UBYTE I1AE:1;              /*   I1AE       */
                    } BIT;                      /*              */
             } TICCR;                           /*              */
};                                              /*              */
struct st_cmt {                                 /* struct CMT   */
       union {                                  /* CMSTR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :14;                 /*              */
                    _UWORD STR1:1;              /*   STR1       */
                    _UWORD STR0:1;              /*   STR0       */
                    } BIT;                      /*              */
             } CMSTR;                           /*              */
       union {                                  /* CMCSR0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD CMF:1;               /*   CMF        */
                    _UWORD CMIE:1;              /*   CMIE       */
                    _UWORD :4;                  /*              */
                    _UWORD CKS:2;               /*   CKS        */
                    } BIT;                      /*              */
             } CMCSR0;                          /*              */
       union {                                  /* CMCNT0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } CMCNT0;                          /*              */
       union {                                  /* CMCOR0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } CMCOR0;                          /*              */
       union {                                  /* CMCSR1       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD CMF:1;               /*   CMF        */
                    _UWORD CMIE:1;              /*   CMIE       */
                    _UWORD :4;                  /*              */
                    _UWORD CKS:2;               /*   CKS        */
                    } BIT;                      /*              */
             } CMCSR1;                          /*              */
       union {                                  /* CMCNT1       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } CMCNT1;                          /*              */
       union {                                  /* CMCOR1       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } CMCOR1;                          /*              */
};                                              /*              */
struct st_wdt {                                 /* struct WDT   */
       union {                                  /* WTCSR        */
             _UWORD WORD;                       /*  word Access */
             struct {                           /*  Bit Access  */
                    _UBYTE IOVF:1;              /*   IOVF       */
                    _UBYTE WTIT:1;              /*   WT/IT      */
                    _UBYTE TME:1;               /*   TME        */
                    _UBYTE :2;                  /*              */
                    _UBYTE CKS:3;               /*   CKS        */
                    } BIT;                      /*              */
             } WTCSR;                           /*              */
       union {                                  /* WTCNT        */
             _UWORD WORD;                       /*  word Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TCNT:8;              /*   CKS        */
                    } BIT;                      /*              */
             } WTCNT;                           /*              */
       union {                                  /* WRCSR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UBYTE WOVF:1;              /*   WOVF       */
                    _UBYTE RSTE:1;              /*   RSTE       */
                    _UBYTE RSTS:1;              /*   RSTS       */
                    _UBYTE :5;                  /*              */
                    } BIT;                      /*              */
             } WRCSR;                           /*              */
};                                              /*              */
struct st_rtc {                                 /* struct RTC   */
       union {                                  /* R64CNT       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE _1Hz:1;              /*   _1Hz       */
                    _UBYTE _2Hz:1;              /*   _2Hz       */
                    _UBYTE _4Hz:1;              /*   _4Hz       */
                    _UBYTE _8Hz:1;              /*   _8Hz       */
                    _UBYTE _16Hz:1;             /*   _16Hz      */
                    _UBYTE _32Hz:1;             /*   _32Hz      */
                    _UBYTE _64Hz:1;             /*   _64Hz      */
                    } BIT;                      /*              */
             } R64CNT;                          /*              */
       _UBYTE wk0[1];                           /*              */
       _UBYTE RSECCNT;                          /* RSECCNT      */
       _UBYTE wk1[1];                           /*              */
       _UBYTE RMINCNT;                          /* RMINCNT      */
       _UBYTE wk2[1];                           /*              */
       _UBYTE RHRCNT;                           /* RHRCNT       */
       _UBYTE wk3[1];                           /*              */
       _UBYTE RWKCNT;                           /* RWKCNT       */
       _UBYTE wk4[1];                           /*              */
       _UBYTE RDAYCNT;                          /* RDAYCNT      */
       _UBYTE wk5[1];                           /*              */
       _UBYTE RMONCNT;                          /* RMONCNT      */
       _UBYTE wk6[1];                           /*              */
       _UWORD RYRCNT;                           /* RYRCNT       */
       union {                                  /* RSECAR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ENB:1;               /*   ENB        */
                    _UBYTE :7;                  /*              */
                    } BIT;                      /*              */
             } RSECAR;                          /*              */
       _UBYTE wk7[1];                           /*              */
       union {                                  /* RMINAR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ENB:1;               /*   ENB        */
                    _UBYTE :7;                  /*              */
                    } BIT;                      /*              */
             } RMINAR;                          /*              */
       _UBYTE wk8[1];                           /*              */
       union {                                  /* RHRAR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ENB:1;               /*   ENB        */
                    _UBYTE :7;                  /*              */
                    } BIT;                      /*              */
             } RHRAR;                           /*              */
       _UBYTE wk9[1];                           /*              */
       union {                                  /* RWKAR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ENB:1;               /*   ENB        */
                    _UBYTE :7;                  /*              */
                    } BIT;                      /*              */
             } RWKAR;                           /*              */
       _UBYTE wk10[1];                          /*              */
       union {                                  /* RDAYAR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ENB:1;               /*   ENB        */
                    _UBYTE :7;                  /*              */
                    } BIT;                      /*              */
             } RDAYAR;                          /*              */
       _UBYTE wk11[1];                          /*              */
       union {                                  /* RMONAR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ENB:1;               /*   ENB        */
                    _UBYTE :7;                  /*              */
                    } BIT;                      /*              */
             } RMONAR;                          /*              */
       _UBYTE wk12[1];                          /*              */
       union {                                  /* RCR1         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE CF:1;                /*   CF         */
                    _UBYTE :2;                  /*              */
                    _UBYTE CIE:1;               /*   CIE        */
                    _UBYTE AIE:1;               /*   AIE        */
                    _UBYTE :2;                  /*              */
                    _UBYTE AF:1;                /*   AF         */
                    } BIT;                      /*              */
             } RCR1;                            /*              */
       _UBYTE wk13[1];                          /*              */
       union {                                  /* RCR2         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE PEF:1;               /*   PEF        */
                    _UBYTE PES:3;               /*   PES        */
                    _UBYTE RTCEN:1;             /*   RTCEN      */
                    _UBYTE ADJ:1;               /*   ADJ        */
                    _UBYTE RESET:1;             /*   RESET      */
                    _UBYTE START:1;             /*   START      */
                    } BIT;                      /*              */
             } RCR2;                            /*              */
       _UBYTE wk14[1];                          /*              */
       _UWORD RYRAR;                            /* RYRAR        */
       _UBYTE wk15[2];                          /*              */
       union {                                  /* RCR3         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ENB:1;               /*   ENB        */
                    _UBYTE :7;                  /*              */
                    } BIT;                      /*              */
             } RCR3;                            /*              */
       _UBYTE wk16[1];                          /*              */
       union {                                  /* RCR5         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :7;                  /*              */
                    _UBYTE RCKSEL:1;            /*   RCKSEL     */
                    } BIT;                      /*              */
             } RCR5;                            /*              */
       _UBYTE wk17[2];                          /*              */
       _UBYTE wk18[1];                          /*              */
       union {                                  /* RFRH         */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :15;                 /*              */
                    _UWORD RFC16:1;             /*   RFC[16]    */
                    } BIT;                      /*              */
             } RFRH;                            /*              */
       union {                                  /* RFRL         */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD RFC15:1;             /*   RFC[15]    */
                    _UWORD RFC14:1;             /*   RFC[14]    */
                    _UWORD RFC13:1;             /*   RFC[13]    */
                    _UWORD RFC12:1;             /*   RFC[12]    */
                    _UWORD RFC11:1;             /*   RFC[11]    */
                    _UWORD RFC10:1;             /*   RFC[10]    */
                    _UWORD RFC9:1;              /*   RFC[9]     */
                    _UWORD RFC8:1;              /*   RFC[8]     */
                    _UWORD RFC7:1;              /*   RFC[7]     */
                    _UWORD RFC6:1;              /*   RFC[6]     */
                    _UWORD RFC5:1;              /*   RFC[5]     */
                    _UWORD RFC4:1;              /*   RFC[4]     */
                    _UWORD RFC3:1;              /*   RFC[3]     */
                    _UWORD RFC2:1;              /*   RFC[2]     */
                    _UWORD RFC1:1;              /*   RFC[1]     */
                    _UWORD RFC0:1;              /*   RFC[0]     */
                    } BIT;                      /*              */
             } RFRL;                            /*              */
};                                              /*              */
struct st_scif02 {                              /* struct SCIF  */
       union {                                  /* SCSMR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD CA:1;                /*   C/A        */
                    _UWORD CHR:1;               /*   CHR        */
                    _UWORD PE:1;                /*   PE         */
                    _UWORD OE:1;                /*   O/E        */
                    _UWORD STOP:1;              /*   STOP       */
                    _UWORD :1;                  /*              */
                    _UWORD CKS:2;               /*   CKS        */
                    } BIT;                      /*              */
             } SCSMR;                           /*              */
       _UBYTE wk0[2];                           /*              */
       union {                                  /* SCBRR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCBRR;                           /*              */
       _UBYTE wk1[3];                           /*              */
       union {                                  /* SCSCR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD TIE:1;               /*   TIE        */
                    _UWORD RIE:1;               /*   RIE        */
                    _UWORD TE:1;                /*   TE         */
                    _UWORD RE:1;                /*   RE         */
                    _UWORD REIE:1;              /*   REIE       */
                    _UWORD :1;                  /*              */
                    _UWORD CKE:2;               /*   CKE        */
                    } BIT;                      /*              */
             } SCSCR;                           /*              */
       _UBYTE wk2[2];                           /*              */
       union {                                  /* SCFTDR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCFTDR;                          /*              */
       _UBYTE wk3[3];                           /*              */
       union {                                  /* SCFSR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD PERN:4;              /*   PERN       */
                    _UWORD FERN:4;              /*   FERN       */
                    _UWORD ER:1;                /*   ER         */
                    _UWORD TEND:1;              /*   TEND       */
                    _UWORD TDFE:1;              /*   TDFE       */
                    _UWORD BRK:1;               /*   BRK        */
                    _UWORD FER:1;               /*   FER        */
                    _UWORD PER:1;               /*   PER        */
                    _UWORD RDF:1;               /*   RDF        */
                    _UWORD DR:1;                /*   DR         */
                    } BIT;                      /*              */
             } SCFSR;                           /*              */
       _UBYTE wk4[2];                           /*              */
       union {                                  /* SCFRDR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCFRDR;                          /*              */
       _UBYTE wk5[3];                           /*              */
       union {                                  /* SCFCR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :5;                  /*              */
                    _UWORD RSTRG:3;             /*   RSTRG      */
                    _UWORD RTRG:2;              /*   RTRG       */
                    _UWORD TTRG:2;              /*   TTRG       */
                    _UWORD MCE:1;               /*   MCE        */
                    _UWORD TFRST:1;             /*   TFRST      */
                    _UWORD RFRST:1;             /*   RFRST      */
                    _UWORD LOOP:1;              /*   LOOP       */
                    } BIT;                      /*              */
             } SCFCR;                           /*              */
       _UBYTE wk6[2];                           /*              */
       union {                                  /* SCFDR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :3;                  /*              */
                    _UWORD T:5;                 /*   T          */
                    _UWORD :3;                  /*              */
                    _UWORD R:5;                 /*   R          */
                    } BIT;                      /*              */
             } SCFDR;                           /*              */
       _UBYTE wk7[2];                           /*              */
       union {                                  /* SCSPTR       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :12;                 /*              */
                    _UWORD SCKIO:1;             /*   SCKIO      */
                    _UWORD SCKDT:1;             /*   SCKDT      */
                    _UWORD SPB2IO:1;            /*   SPB2IO     */
                    _UWORD SPB2DT:1;            /*   SPB2DT     */
                    } BIT;                      /*              */
             } SCSPTR;                          /*              */
       _UBYTE wk8[2];                           /*              */
       union {                                  /* SCLSR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :15;                 /*              */
                    _UWORD ORER:1;              /*   ORER       */
                    } BIT;                      /*              */
             } SCLSR;                           /*              */
       _UBYTE wk9[2];                           /*              */
       union {                                  /* SCEMR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD BGDM:1;              /*   BGDM       */
                    _UWORD :6;                  /*              */
                    _UWORD ABCS:1;              /*   ABCS       */
                    } BIT;                      /*              */
             } SCEMR;                           /*              */
};                                              /*              */
struct st_scif4567 {                            /* struct SCIF  */
       union {                                  /* SCSMR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD CA:1;                /*   C/A        */
                    _UWORD CHR:1;               /*   CHR        */
                    _UWORD PE:1;                /*   PE         */
                    _UWORD OE:1;                /*   O/E        */
                    _UWORD STOP:1;              /*   STOP       */
                    _UWORD :1;                  /*              */
                    _UWORD CKS:2;               /*   CKS        */
                    } BIT;                      /*              */
             } SCSMR;                           /*              */
       _UBYTE wk0[2];                           /*              */
       union {                                  /* SCBRR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCBRR;                           /*              */
       _UBYTE wk1[3];                           /*              */
       union {                                  /* SCSCR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD TIE:1;               /*   TIE        */
                    _UWORD RIE:1;               /*   RIE        */
                    _UWORD TE:1;                /*   TE         */
                    _UWORD RE:1;                /*   RE         */
                    _UWORD REIE:1;              /*   REIE       */
                    _UWORD :1;                  /*              */
                    _UWORD CKE:2;               /*   CKE        */
                    } BIT;                      /*              */
             } SCSCR;                           /*              */
       _UBYTE wk2[2];                           /*              */
       union {                                  /* SCFTDR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCFTDR;                          /*              */
       _UBYTE wk3[3];                           /*              */
       union {                                  /* SCFSR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD PERN:4;              /*   PERN       */
                    _UWORD FERN:4;              /*   FERN       */
                    _UWORD ER:1;                /*   ER         */
                    _UWORD TEND:1;              /*   TEND       */
                    _UWORD TDFE:1;              /*   TDFE       */
                    _UWORD BRK:1;               /*   BRK        */
                    _UWORD FER:1;               /*   FER        */
                    _UWORD PER:1;               /*   PER        */
                    _UWORD RDF:1;               /*   RDF        */
                    _UWORD DR:1;                /*   DR         */
                    } BIT;                      /*              */
             } SCFSR;                           /*              */
       _UBYTE wk4[2];                           /*              */
       union {                                  /* SCFRDR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCFRDR;                          /*              */
       _UBYTE wk5[3];                           /*              */
       union {                                  /* SCFCR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :5;                  /*              */
                    _UWORD RSTRG:3;             /*   RSTRG      */
                    _UWORD RTRG:2;              /*   RTRG       */
                    _UWORD TTRG:2;              /*   TTRG       */
                    _UWORD MCE:1;               /*   MCE        */
                    _UWORD TFRST:1;             /*   TFRST      */
                    _UWORD RFRST:1;             /*   RFRST      */
                    _UWORD LOOP:1;              /*   LOOP       */
                    } BIT;                      /*              */
             } SCFCR;                           /*              */
       _UBYTE wk6[2];                           /*              */
       union {                                  /* SCFDR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :3;                  /*              */
                    _UWORD T:5;                 /*   T          */
                    _UWORD :3;                  /*              */
                    _UWORD R:5;                 /*   R          */
                    } BIT;                      /*              */
             } SCFDR;                           /*              */
       _UBYTE wk7[2];                           /*              */
       union {                                  /* SCSPTR       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :14;                 /*              */
                    _UWORD SPB2IO:1;            /*   SPB2IO     */
                    _UWORD SPB2DT:1;            /*   SPB2DT     */
                    } BIT;                      /*              */
             } SCSPTR;                          /*              */
       _UBYTE wk8[2];                           /*              */
       union {                                  /* SCLSR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :15;                 /*              */
                    _UWORD ORER:1;              /*   ORER       */
                    } BIT;                      /*              */
             } SCLSR;                           /*              */
       _UBYTE wk9[2];                           /*              */
       union {                                  /* SCEMR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD BGDM:1;              /*   BGDM       */
                    _UWORD :6;                  /*              */
                    _UWORD ABCS:1;              /*   ABCS       */
                    } BIT;                      /*              */
             } SCEMR;                           /*              */
};                                              /*              */
struct st_scif13 {                              /* struct SCIF  */
       union {                                  /* SCSMR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD CA:1;                /*   C/A        */
                    _UWORD CHR:1;               /*   CHR        */
                    _UWORD PE:1;                /*   PE         */
                    _UWORD OE:1;                /*   O/E        */
                    _UWORD STOP:1;              /*   STOP       */
                    _UWORD :1;                  /*              */
                    _UWORD CKS:2;               /*   CKS        */
                    } BIT;                      /*              */
             } SCSMR;                           /*              */
       _UBYTE wk0[2];                           /*              */
       union {                                  /* SCBRR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCBRR;                           /*              */
       _UBYTE wk1[3];                           /*              */
       union {                                  /* SCSCR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD TIE:1;               /*   TIE        */
                    _UWORD RIE:1;               /*   RIE        */
                    _UWORD TE:1;                /*   TE         */
                    _UWORD RE:1;                /*   RE         */
                    _UWORD REIE:1;              /*   REIE       */
                    _UWORD :1;                  /*              */
                    _UWORD CKE:2;               /*   CKE        */
                    } BIT;                      /*              */
             } SCSCR  ;                         /*              */
       _UBYTE wk2[2];                           /*              */
       union {                                  /* SCFTDR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCFTDR;                          /*              */
       _UBYTE wk3[3];                           /*              */
       union {                                  /* SCFSR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD PERN:4;              /*   PERN       */
                    _UWORD FERN:4;              /*   FERN       */
                    _UWORD ER:1;                /*   ER         */
                    _UWORD TEND:1;              /*   TEND       */
                    _UWORD TDFE:1;              /*   TDFE       */
                    _UWORD BRK:1;               /*   BRK        */
                    _UWORD FER:1;               /*   FER        */
                    _UWORD PER:1;               /*   PER        */
                    _UWORD RDF:1;               /*   RDF        */
                    _UWORD DR:1;                /*   DR         */
                    } BIT;                      /*              */
             } SCFSR  ;                         /*              */
       _UBYTE wk4[2];                           /*              */
       union {                                  /* SCFRDR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE D:8;                 /*   D          */
                    } BIT;                      /*              */
             } SCFRDR;                          /*              */
       _UBYTE wk5[3];                           /*              */
       union {                                  /* SCFCR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :5;                  /*              */
                    _UWORD RSTRG:3;             /*   RSTRG      */
                    _UWORD RTRG:2;              /*   RTRG       */
                    _UWORD TTRG:2;              /*   TTRG       */
                    _UWORD MCE:1;               /*   MCE        */
                    _UWORD TFRST:1;             /*   TFRST      */
                    _UWORD RFRST:1;             /*   RFRST      */
                    _UWORD LOOP:1;              /*   LOOP       */
                    } BIT;                      /*              */
             } SCFCR;                           /*              */
       _UBYTE wk6[2];                           /*              */
       union {                                  /* SCFDR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :3;                  /*              */
                    _UWORD T:5;                 /*   T          */
                    _UWORD :3;                  /*              */
                    _UWORD R:5;                 /*   R          */
                    } BIT;                      /*              */
             } SCFDR;                           /*              */
       _UBYTE wk7[2];                           /*              */
       union {                                  /* SCSPTR       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD RTSIO:1;             /*   RTSIO      */
                    _UWORD RTSDT:1;             /*   RTSDT      */
                    _UWORD CTSIO:1;             /*   CTSIO      */
                    _UWORD CTSDT:1;             /*   CTSDT      */
                    _UWORD SCKIO:1;             /*   SCKIO      */
                    _UWORD SCKDT:1;             /*   SCKDT      */
                    _UWORD SPB2IO:1;            /*   SPB2IO     */
                    _UWORD SPB2DT:1;            /*   SPB2DT     */
                    } BIT;                      /*              */
             } SCSPTR;                          /*              */
       _UBYTE wk8[2];                           /*              */
       union {                                  /* SCLSR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :15;                 /*              */
                    _UWORD ORER:1;              /*   ORER       */
                    } BIT;                      /*              */
             } SCLSR;                           /*              */
       _UBYTE wk9[2];                           /*              */
       union {                                  /* SCEMR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD BGDM:1;              /*   BGDM       */
                    _UWORD :6;                  /*              */
                    _UWORD ABCS:1;              /*   ABCS       */
                    } BIT;                      /*              */
             } SCEMR;                           /*              */
};                                              /*              */

struct st_rspi {                                /* struct RSPI  */
       union {                                  /* SPCR         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE SPRIE:1;             /*   SPRIE      */
                    _UBYTE SPE:1;               /*   SPE        */
                    _UBYTE SPTIE:1;             /*   SPTIE      */
                    _UBYTE SPEIE:1;             /*   SPEIE      */
                    _UBYTE MSTR:1;              /*   MSTR       */
                    _UBYTE MODFEN:1;            /*   MODFEN     */
                    _UBYTE :2;                  /*              */
                    } BIT;                      /*              */
             } SPCR;                            /*              */
       union {                                  /* SSLP         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :7;                  /*              */
                    _UBYTE SSL0P:1;             /*   SSL0P      */
                    } BIT;                      /*              */
             } SSLP;                            /*              */
       union {                                  /* SPPCR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE MOIFE:1;             /*   MOIFE      */
                    _UBYTE MOIFV:1;             /*   MOIFV      */
                    _UBYTE :3;                  /*              */
                    _UBYTE SPLP:1;              /*   SPLP       */
                    } BIT;                      /*              */
             } SPPCR;                           /*              */
       union {                                  /* SPSR         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE SPRF:1;              /*   SPRF       */
                    _UBYTE TEND:1;              /*   TEND       */
                    _UBYTE SPTEF:1;             /*   SPTEF      */
                    _UBYTE :2;                  /*              */
                    _UBYTE MODF:1;              /*   MODF       */
                    _UBYTE :1;                  /*              */
                    _UBYTE OVRF:1;              /*   OVRF       */
                    } BIT;                      /*              */
             } SPSR;                            /*              */
       union {                                  /* SPDR         */
             _UDWORD LONG;                      /*  Long Access */
             _UWORD WORD;                       /*  Word Access */
             _UBYTE BYTE;                       /*  Byte Access */
             } SPDR;                            /*              */
       union {                                  /* SPSCR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :6;                  /*              */
                    _UBYTE SPSLN:2;             /*   SPSLN      */
                    } BIT;                      /*              */
             } SPSCR;                           /*              */
       union {                                  /* SPSSR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :6;                  /*              */
                    _UBYTE SPCP:2;              /*   SPCP       */
                    } BIT;                      /*              */
             } SPSSR;                           /*              */
       union {                                  /* SPBR         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE SPR:8;               /*   SPR        */
                    } BIT;                      /*              */
             } SPBR;                            /*              */
       union {                                  /* SPDCR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TXDMY:1;             /*   TXDMY      */
                    _UBYTE SPLW:2;              /*   SPLW       */
                    _UBYTE :5;                  /*              */
                    } BIT;                      /*              */
             } SPDCR;                           /*              */
       union {                                  /* SPCKD        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :5;                  /*              */
                    _UBYTE SCKDL:3;             /*   SCKDL      */
                    } BIT;                      /*              */
             } SPCKD;                           /*              */
       union {                                  /* SSLND        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :5;                  /*              */
                    _UBYTE SLNDL:3;             /*   SLNDL      */
                    } BIT;                      /*              */
             } SSLND;                           /*              */
       union {                                  /* SPND         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :5;                  /*              */
                    _UBYTE SPNDL:3;             /*   SPNDL      */
                    } BIT;                      /*              */
             } SPND;                            /*              */
       _UBYTE wk0[1];                           /*              */
       union {                                  /* SPCMD0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD SCKDEN:1;            /*   SCKDEN     */
                    _UWORD SLNDEN:1;            /*   SLNDEN     */
                    _UWORD SPNDEN:1;            /*   SPNDEN     */
                    _UWORD LSBF:1;              /*   LSBF       */
                    _UWORD SPB:4;               /*   SPB        */
                    _UWORD SSLKP:1;             /*   SSLKP      */
                    _UWORD :3;                  /*              */
                    _UWORD BRDV:2;              /*   BRDV       */
                    _UWORD CPOL:1;              /*   CPOL       */
                    _UWORD CPHA:1;              /*   CPHA       */
                    } BIT;                      /*              */
             } SPCMD0;                          /*              */
       union {                                  /* SPCMD1       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD SCKDEN:1;            /*   SCKDEN     */
                    _UWORD SLNDEN:1;            /*   SLNDEN     */
                    _UWORD SPNDEN:1;            /*   SPNDEN     */
                    _UWORD LSBF:1;              /*   LSBF       */
                    _UWORD SPB:4;               /*   SPB        */
                    _UWORD SSLKP:1;             /*   SSLKP      */
                    _UWORD :3;                  /*              */
                    _UWORD BRDV:2;              /*   BRDV       */
                    _UWORD CPOL:1;              /*   CPOL       */
                    _UWORD CPHA:1;              /*   CPHA       */
                    } BIT;                      /*              */
             } SPCMD1 ;                         /*              */
       union {                                  /* SPCMD2       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD SCKDEN:1;            /*   SCKDEN     */
                    _UWORD SLNDEN:1;            /*   SLNDEN     */
                    _UWORD SPNDEN:1;            /*   SPNDEN     */
                    _UWORD LSBF:1;              /*   LSBF       */
                    _UWORD SPB:4;               /*   SPB        */
                    _UWORD SSLKP:1;             /*   SSLKP      */
                    _UWORD :3;                  /*              */
                    _UWORD BRDV:2;              /*   BRDV       */
                    _UWORD CPOL:1;              /*   CPOL       */
                    _UWORD CPHA:1;              /*   CPHA       */
                    } BIT;                      /*              */
             } SPCMD2 ;                         /*              */
       union {                                  /* SPCMD3       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD SCKDEN:1;            /*   SCKDEN     */
                    _UWORD SLNDEN:1;            /*   SLNDEN     */
                    _UWORD SPNDEN:1;            /*   SPNDEN     */
                    _UWORD LSBF:1;              /*   LSBF       */
                    _UWORD SPB:4;               /*   SPB        */
                    _UWORD SSLKP:1;             /*   SSLKP      */
                    _UWORD :3;                  /*              */
                    _UWORD BRDV:2;              /*   BRDV       */
                    _UWORD CPOL:1;              /*   CPOL       */
                    _UWORD CPHA:1;              /*   CPHA       */
                    } BIT;                      /*              */
             } SPCMD3 ;                         /*              */
       _UBYTE wk1[8];                           /*              */
       union {                                  /* SPBFCR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TXRST:1;             /*   TXRST      */
                    _UBYTE RXRST:1;             /*   RXRST      */
                    _UBYTE TXTRG:2;             /*   TXTRG      */
                    _UBYTE :1;                  /*              */
                    _UBYTE RXTRG:3;             /*   RXTRG      */
                    } BIT;                      /*              */
             } SPBFCR;                          /*              */
       _UBYTE wk2[1];                           /*              */
       union {                                  /* SPBFDR       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :4;                  /*              */
                    _UWORD T:4 ;                /*   T          */
                    _UWORD :2;                  /*              */
                    _UWORD R:6;                 /*   R          */
                    } BIT;                      /*              */
             } SPBFDR;                          /*              */
};                                              /*              */
struct st_iic3 {                                /* struct IIC3  */
       union {                                  /* ICCR1        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ICE:1;               /*   ICE        */
                    _UBYTE RCVD:1;              /*   RCVD       */
                    _UBYTE MST:1;               /*   MST        */
                    _UBYTE TRS:1;               /*   TRS        */
                    _UBYTE CKS:4;               /*   CKS        */
                    } BIT;                      /*              */
             } ICCR1;                           /*              */
       union {                                  /* ICCR2        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE BBSY:1;              /*   BBSY       */
                    _UBYTE SCP:1;               /*   SCP        */
                    _UBYTE SDAO:1;              /*   SDAO       */
                    _UBYTE SDAOP:1;             /*   SDAOP      */
                    _UBYTE SCLO:1;              /*   SCLO       */
                    _UBYTE :1;                  /*              */
                    _UBYTE IICRST:1;            /*   IICRST     */
                    _UBYTE :1;                  /*              */
                    } BIT;                      /*              */
             } ICCR2;                           /*              */
       union {                                  /* ICMR         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE MLS:1;               /*   MLS        */
                    _UBYTE :3;                  /*              */
                    _UBYTE BCWP:1;              /*   BCWP       */
                    _UBYTE BC:3;                /*   BC         */
                    } BIT;                      /*              */
             } ICMR;                            /*              */
       union {                                  /* ICIER        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TIE:1;               /*   TIE        */
                    _UBYTE TEIE:1;              /*   TEIE       */
                    _UBYTE RIE:1;               /*   RIE        */
                    _UBYTE NAKIE:1;             /*   NAKIE      */
                    _UBYTE STIE:1;              /*   STIE       */
                    _UBYTE ACKE:1;              /*   ACKE       */
                    _UBYTE ACKBR:1;             /*   ACKBR      */
                    _UBYTE ACKBT:1;             /*   ACKBT      */
                    } BIT;                      /*              */
             } ICIER;                           /*              */
       union {                                  /* ICSR         */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TDRE:1;              /*   TDRE       */
                    _UBYTE TEND:1;              /*   TEND       */
                    _UBYTE RDRF:1;              /*   RDRF       */
                    _UBYTE NACKF:1;             /*   NACKF      */
                    _UBYTE STOP:1;              /*   STOP       */
                    _UBYTE ALOVE:1;             /*   AL/OVE     */
                    _UBYTE AAS:1;               /*   AAS        */
                    _UBYTE ADZ:1;               /*   ADZ        */
                    } BIT;                      /*              */
             } ICSR;                            /*              */
       union {                                  /* SAR          */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE SVA:7;               /*   SVA        */
                    _UBYTE FS:1;                /*   FS         */
                    } BIT;                      /*              */
             } SAR;                             /*              */
       _UBYTE ICDRT;                            /* ICDRT        */
       _UBYTE ICDRR;                            /* ICDRR        */
       union {                                  /* NF2CYC       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :6;                  /*              */
                    _UBYTE PRS:1;               /*   PRS        */
                    _UBYTE NF2CYC:1;            /*   NF2CYC     */
                    } BIT;                      /*              */
             } NF2CYC;                          /*              */
};                                              /*              */
struct st_ssif {                                /* struct SSIF  */
       union {                                  /* SSICR        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :1;                 /*              */
                    _UDWORD CKS:1;              /*   CKS        */
                    _UDWORD TUIEN:1;            /*   TUIEN      */
                    _UDWORD TOIEN:1;            /*   TOIEN      */
                    _UDWORD RUIEN:1;            /*   RUIEN      */
                    _UDWORD ROIEN:1;            /*   ROIEN      */
                    _UDWORD IIEN:1;             /*   IIEN       */
                    _UDWORD :1;                 /*              */
                    _UDWORD CHNL:2;             /*   CHNL       */
                    _UDWORD DWL:3;              /*   DWL        */
                    _UDWORD SWL:3;              /*   SWL        */
                    _UDWORD SCKD:1;             /*   SCKD       */
                    _UDWORD SWSD:1;             /*   SWSD       */
                    _UDWORD SCKP:1;             /*   SCKP       */
                    _UDWORD SWSP:1;             /*   SWSP       */
                    _UDWORD SPDP:1;             /*   SPDP       */
                    _UDWORD SDTA:1;             /*   SDTA       */
                    _UDWORD PDTA:1;             /*   PDTA       */
                    _UDWORD DEL:1;              /*   DEL        */
                    _UDWORD CKDV:4;             /*   CKDV       */
                    _UDWORD MUEN:1;             /*   MUEN       */
                    _UDWORD :1;                 /*              */
                    _UDWORD TEN:1;              /*   TEN        */
                    _UDWORD REN:1;              /*   REN        */
                    } BIT;                      /*              */
             } SSICR;                           /*              */
       union {                                  /* SSISR        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :2;                 /*              */
                    _UDWORD TUIRQ:1;            /*   TUIRQ      */
                    _UDWORD TOIRQ:1;            /*   TOIRQ      */
                    _UDWORD RUIRQ:1;            /*   RUIRQ      */
                    _UDWORD ROIRQ:1;            /*   ROIRQ      */
                    _UDWORD IIRQ:1;             /*   IIRQ       */
                    _UDWORD :18;                /*              */
                    _UDWORD TCHNO:2;            /*   TCHNO      */
                    _UDWORD TSWNO:1;            /*   TSWNO      */
                    _UDWORD RCHNO:2;            /*   RCHNO      */
                    _UDWORD RSWNO:1;            /*   RSWNO      */
                    _UDWORD IDST:1;             /*   IDST       */
                    } BIT;                      /*              */
             } SSISR;                           /*              */
       _UBYTE wk0[8];                           /*              */
       union {                                  /* SSIFCR       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :24;                /*              */
                    _UDWORD TTRG:2;             /*   TTRG       */
                    _UDWORD RTRG:2;             /*   RTRG       */
                    _UDWORD TIE:1;              /*   TIE        */
                    _UDWORD RIE:1;              /*   RIE        */
                    _UDWORD TFRST:1;            /*   TFRST      */
                    _UDWORD RFRST:1;            /*   RFRST      */
                    } BIT;                      /*              */
             } SSIFCR;                          /*              */
       union {                                  /* SSIFSR       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :4;                 /*              */
                    _UDWORD TDC:4;              /*   TDC        */
                    _UDWORD :7;                 /*              */
                    _UDWORD TDE:1;              /*   TDE        */
                    _UDWORD :4;                 /*              */
                    _UDWORD RDC:4;              /*   RDC        */
                    _UDWORD :7;                 /*              */
                    _UDWORD RDF:1;              /*   RDF        */
                    } BIT;                      /*              */
             } SSIFSR;                          /*              */
       _UDWORD SSIFTDR;                         /* SSIFTDR      */
       _UDWORD SSIFRDR;                         /* SSIFRDR      */
};                                              /*              */
struct st_siof {                                /* struct SIOF  */
       union {                                  /* SIMDR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD TRMD:2;              /*   TRMD       */
                    _UWORD SYNCAT:1;            /*   SYNCAT     */
                    _UWORD REDG:1;              /*   REDG       */
                    _UWORD FL:4;                /*   FL         */
                    _UWORD TXDIZ:1;             /*   TXDIZ      */
                    _UWORD :1;                  /*              */
                    _UWORD SYNCAC:1;            /*   SYNCAC     */
                    _UWORD SYNCDL:1;            /*   SYNCDL     */
                    _UWORD :4;                  /*              */
                    } BIT;                      /*              */
             } SIMDR;                           /*              */
       union {                                  /* SISCR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD MSSEL:1;             /*   MSSEL      */
                    _UWORD :2;                  /*              */
                    _UWORD BRPS:5;              /*   BRPS       */
                    _UWORD :5;                  /*              */
                    _UWORD BRDV:3;              /*   BRDV       */
                    } BIT;                      /*              */
             } SISCR;                           /*              */
       union {                                  /* SITDAR       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD TDLE:1;              /*   TDLE       */
                    _UWORD :3;                  /*              */
                    _UWORD TDLA:4;              /*   TDLA       */
                    _UWORD TDRE:1;              /*   TDRE       */
                    _UWORD TLREP:1;             /*   TLREP      */
                    _UWORD :2;                  /*              */
                    _UWORD TDRA:4;              /*   TDRA       */
                    } BIT;                      /*              */
             } SITDAR;                          /*              */
       union {                                  /* SIRDAR       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD RDLE:1;              /*   RDLE       */
                    _UWORD :3;                  /*              */
                    _UWORD RDLA:4;              /*   RDLA       */
                    _UWORD RDRE:1;              /*   RDRE       */
                    _UWORD :3;                  /*              */
                    _UWORD RDRA:4;              /*   RDRA       */
                    } BIT;                      /*              */
             } SIRDAR;                          /*              */
       _UBYTE wk0[4];                           /*              */
       union {                                  /* SICTR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD SCKE:1;              /*   SCKE       */
                    _UWORD FSE:1;               /*   FSE        */
                    _UWORD :4;                  /*              */
                    _UWORD TXE:1;               /*   TXE        */
                    _UWORD RXE:1;               /*   RXE        */
                    _UWORD :6;                  /*              */
                    _UWORD TXRST:1;             /*   TXRST      */
                    _UWORD RXRST:1;             /*   RXRST      */
                    } BIT;                      /*              */
             } SICTR;                           /*              */
       _UBYTE wk1[2];                           /*              */
       union {                                  /* SIFCTR       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD TFWM:3;              /*   TFWM       */
                    _UWORD TFUA:5 ;             /*   TFUA       */
                    _UWORD RFWM:3;              /*   RFWM       */
                    _UWORD RFUA:5;              /*   RFUA       */
                    } BIT;                      /*              */
             } SIFCTR;                          /*              */
       _UBYTE wk2[2];                           /*              */
       union {                                  /* SISTR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :2;                  /*              */
                    _UWORD TFEMP:1;             /*   TFEMP      */
                    _UWORD TDREQ:1;             /*   TDREQ      */
                    _UWORD :2;                  /*              */
                    _UWORD RFFUL:1 ;            /*   RFFUL      */
                    _UWORD RDREQ:1;             /*   RDREQ      */
                    _UWORD :3;                  /*              */
                    _UWORD FSERRE:1;            /*   FSERRE     */
                    _UWORD TFOVFE:1;            /*   TFOVFE     */
                    _UWORD TFUDFE:1;            /*   TFUDFE     */
                    _UWORD RFUDFE:1;            /*   RFUDFE     */
                    _UWORD RFOVFE:1;            /*   RFOVFE     */
                    } BIT;                      /*              */
             } SISTR;                           /*              */
       union {                                  /* SIIER        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD TDMAE:1;             /*   TDMAE      */
                    _UWORD :1;                  /*              */
                    _UWORD TFEMPE:1;            /*   TFEMPE     */
                    _UWORD TDREQE:1;            /*   TDREQE     */
                    _UWORD RDMAE:1;             /*   RDMAE      */
                    _UWORD :1;                  /*              */
                    _UWORD RFFULE:1;            /*   RFFULE     */
                    _UWORD RDREQE:1;            /*   RDREQE     */
                    _UWORD :2;                  /*              */
                    _UWORD SAERRE:1;            /*   SASERRE    */
                    _UWORD FSERRE:1;            /*   FSERRE     */
                    _UWORD TFOVFE:1;            /*   TFOVFE     */
                    _UWORD TFUDFE:1;            /*   TFUDFE     */
                    _UWORD RFUDFE:1;            /*   RFUDFE     */
                    _UWORD RFOVFE:1;            /*   RFOVFE     */
                    } BIT;                      /*              */
             } SIIER;                           /*              */
       _UBYTE wk3[8];                           /*              */
       union {                                  /* SITDR        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UWORD SITDL:16;            /*   SITDL      */
                    _UWORD SITDR:16;            /*   SITDR      */
                    } BIT;                      /*              */
             } SITDR;                           /*              */
       union {                                  /* SIRDR        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    _UWORD SIRDL:16;            /*   SIRDL      */
                    _UWORD SIRDR:16;            /*   SIRDR      */
                    } BIT;                      /*              */
             } SIRDR;                           /*              */
};
union un_mb3116{                                /* MB31-MB16    */
      _UWORD WORD;                              /*  Word Access */
      struct {                                  /*  Bit Access  */
             _UWORD MB31:1;                     /*   MB31       */
             _UWORD MB30:1;                     /*   MB30       */
             _UWORD MB29:1;                     /*   MB29       */
             _UWORD MB28:1;                     /*   MB28       */
             _UWORD MB27:1;                     /*   MB27       */
             _UWORD MB26:1;                     /*   MB26       */
             _UWORD MB25:1;                     /*   MB25       */
             _UWORD MB24:1;                     /*   MB24       */
             _UWORD MB23:1;                     /*   MB23       */
             _UWORD MB22:1;                     /*   MB22       */
             _UWORD MB21:1;                     /*   MB21       */
             _UWORD MB20:1;                     /*   MB20       */
             _UWORD MB19:1;                     /*   MB19       */
             _UWORD MB18:1;                     /*   MB18       */
             _UWORD MB17:1;                     /*   MB17       */
             _UWORD MB16:1;                     /*   MB16       */
             } BIT;                             /*              */
};
union un_mb15_0{                                /* MB15-MB0     */
      _UWORD WORD;                              /*  Word Access */
      struct {                                  /*  Bit Access  */
             _UWORD MB15:1;                     /*   MB15       */
             _UWORD MB14:1;                     /*   MB14       */
             _UWORD MB13:1;                     /*   MB13       */
             _UWORD MB12:1;                     /*   MB12       */
             _UWORD MB11:1;                     /*   MB11       */
             _UWORD MB10:1;                     /*   MB10       */
             _UWORD MB9:1;                      /*   MB9        */
             _UWORD MB8:1;                      /*   MB8        */
             _UWORD MB7:1;                      /*   MB7        */
             _UWORD MB6:1;                      /*   MB6        */
             _UWORD MB5:1;                      /*   MB5        */
             _UWORD MB4:1;                      /*   MB4        */
             _UWORD MB3:1;                      /*   MB3        */
             _UWORD MB2:1;                      /*   MB2        */
             _UWORD MB1:1;                      /*   MB1        */
             _UWORD MB0:1;                      /*   MB0        */
             } BIT;                             /*              */
};
union un_mb15_1{                                /* MB15-MB1     */
      _UWORD WORD;                              /*  Word Access */
      struct {                                  /*  Bit Access  */
             _UWORD MB15:1;                     /*   MB15       */
             _UWORD MB14:1;                     /*   MB14       */
             _UWORD MB13:1;                     /*   MB13       */
             _UWORD MB12:1;                     /*   MB12       */
             _UWORD MB11:1;                     /*   MB11       */
             _UWORD MB10:1;                     /*   MB10       */
             _UWORD MB9:1;                      /*   MB9        */
             _UWORD MB8:1;                      /*   MB8        */
             _UWORD MB7:1;                      /*   MB7        */
             _UWORD MB6:1;                      /*   MB6        */
             _UWORD MB5:1;                      /*   MB5        */
             _UWORD MB4:1;                      /*   MB4        */
             _UWORD MB3:1;                      /*   MB3        */
             _UWORD MB2:1;                      /*   MB2        */
             _UWORD MB1:1;                      /*   MB1        */
             _UWORD :1;                         /*              */
             } BIT;                             /*              */
};
struct st_rcan {                                /* struct RCAN  */
       union {                                  /* MCR          */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD IDR  :1;             /*   IDR        */
                    _UWORD AHBO :1;             /*   AHBO       */
                    _UWORD      :3;             /*              */
                    _UWORD TST  :3;             /*   TST        */
                    _UWORD AWM  :1;             /*   AWM        */
                    _UWORD HTBO :1;             /*   HTBO       */
                    _UWORD SLPM :1;             /*   SLPM       */
                    _UWORD      :2;             /*              */
                    _UWORD MTP  :1;             /*   MTP        */
                    _UWORD HLTRQ:1;             /*   HLTRQ      */
                    _UWORD RSTRQ:1;             /*   RSTRQ      */
                    } BIT;                      /*              */
             } MCR;                             /*              */
       union {                                  /* GSR          */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :10;                 /*              */
                    _UWORD EPS:1;               /*   EPS        */
                    _UWORD HSS:1;               /*   HSS        */
                    _UWORD RS:1;                /*   RS         */
                    _UWORD MTPF:1;              /*   MTPF       */
                    _UWORD TRWF:1;              /*   TRWF       */
                    _UWORD BOF:1;               /*   BOF        */
                    } BIT;                      /*              */
             } GSR;                             /*              */
       union {                                  /* BCR1         */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD TSG1:4;              /*   TSG1       */
                    _UWORD :1;                  /*              */
                    _UWORD TSG2:3;              /*   TSG2       */
                    _UWORD :2;                  /*              */
                    _UWORD SJW:2;               /*   SJW        */
                    _UWORD :3;                  /*              */
                    _UWORD BSP:1;               /*   BSP        */
                    } BIT;                      /*              */
             } BCR1;                            /*              */
       union {                                  /* BCR0         */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :8;                  /*              */
                    _UWORD BRP:8;               /*   BRP        */
                    } BIT;                      /*              */
             } BCR0;                            /*              */
       union {                                  /* IRR          */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD TCMI1 :1;            /*   TCMI1      */
                    _UWORD TCMI0 :1;            /*   TCMI0      */
                    _UWORD TOI   :1;            /*   TOI        */
                    _UWORD BASMIF:1;            /*   BASMIF     */
                    _UWORD TCMI2 :1;            /*   TCMI2      */
                    _UWORD SNSMI :1;            /*   SNSMI      */
                    _UWORD MOOIF :1;            /*   MOOIF      */
                    _UWORD MBEIF :1;            /*   MBEIF      */
                    _UWORD OLF   :1;            /*   OLF        */
                    _UWORD BOFIF :1;            /*   BOFIF      */
                    _UWORD EPIF  :1;            /*   EPIF       */
                    _UWORD RECWIF:1;            /*   RECWIF     */
                    _UWORD TECWIF:1;            /*   TECWIF     */
                    _UWORD RFRIF :1;            /*   RFRIF      */
                    _UWORD DFRIF :1;            /*   DFRIF      */
                    _UWORD RSTIF :1;            /*   RSTIF      */
                    } BIT;                      /*              */
             } IRR;                             /*              */
       union {                                  /* IMR          */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD TCMI1M:1;            /*   TCMI1M     */
                    _UWORD TCMI0M:1;            /*   TCMI0M     */
                    _UWORD TOIM  :1;            /*   TOIM       */
                    _UWORD BASMIM:1;            /*   BASMIM     */
                    _UWORD TCMI2M:1;            /*   TCMI2M     */
                    _UWORD SNSMIM:1;            /*   SNSMIM     */
                    _UWORD MOOIM :1;            /*   MOOIM      */
                    _UWORD MBEIM :1;            /*   MBEIM      */
                    _UWORD OLFM  :1;            /*   OLFM       */
                    _UWORD BOFIM :1;            /*   BOFIM      */
                    _UWORD EPIM  :1;            /*   EPIM       */
                    _UWORD RECWIM:1;            /*   RECWIM     */
                    _UWORD TECWIM:1;            /*   TECWIM     */
                    _UWORD RFRIM :1;            /*   RFRIM      */
                    _UWORD DFRIM :1;            /*   DFRIM      */
                    _UWORD RSTIM :1;            /*   RSTIM      */
                    } BIT;                      /*              */
             } IMR;                             /*              */
       union {                                  /* TEC_REC      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD TEC:8;               /*   TEC        */
                    _UWORD REC:8;               /*   REC        */
                    } BIT;                      /*              */
             } TEC_REC  ;                       /*              */
       _UBYTE wk0[18];                          /*              */
       union{                                   /* TXPR0        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD MB31:1;             /*   MB31       */
                    _UDWORD MB30:1;             /*   MB30       */
                    _UDWORD MB29:1;             /*   MB29       */
                    _UDWORD MB28:1;             /*   MB28       */
                    _UDWORD MB27:1;             /*   MB27       */
                    _UDWORD MB26:1;             /*   MB26       */
                    _UDWORD MB25:1;             /*   MB25       */
                    _UDWORD MB24:1;             /*   MB24       */
                    _UDWORD MB23:1;             /*   MB23       */
                    _UDWORD MB22:1;             /*   MB22       */
                    _UDWORD MB21:1;             /*   MB21       */
                    _UDWORD MB20:1;             /*   MB20       */
                    _UDWORD MB19:1;             /*   MB19       */
                    _UDWORD MB18:1;             /*   MB18       */
                    _UDWORD MB17:1;             /*   MB17       */
                    _UDWORD MB16:1;             /*   MB16       */
                    _UDWORD MB15:1;             /*   MB15       */
                    _UDWORD MB14:1;             /*   MB14       */
                    _UDWORD MB13:1;             /*   MB13       */
                    _UDWORD MB12:1;             /*   MB12       */
                    _UDWORD MB11:1;             /*   MB11       */
                    _UDWORD MB10:1;             /*   MB10       */
                    _UDWORD MB9:1;              /*   MB9        */
                    _UDWORD MB8:1;              /*   MB8        */
                    _UDWORD MB7:1;              /*   MB7        */
                    _UDWORD MB6:1;              /*   MB6        */
                    _UDWORD MB5:1;              /*   MB5        */
                    _UDWORD MB4:1;              /*   MB4        */
                    _UDWORD MB3:1;              /*   MB3        */
                    _UDWORD MB2:1;              /*   MB2        */
                    _UDWORD MB1:1;              /*   MB1        */
                    } BIT;                      /*              */
             } TXPR0  ;                         /*              */
       _UBYTE wk1[4];                           /*              */
       union un_mb3116 TXCR1;                   /* TXCR1        */
       union un_mb15_1 TXCR0;                   /* TXCR0        */
       _UBYTE wk2[4];                           /*              */
       union un_mb3116 TXACK1;                  /* TXACK1       */
       union un_mb15_1 TXACK0;                  /* TXACK0       */
       _UBYTE wk3[4];                           /*              */
       union un_mb3116 ABACK1;                  /* ABACK1       */
       union un_mb15_1 ABACK0;                  /* ABACK0       */
       _UBYTE wk4[4];                           /*              */
       union un_mb3116 RXPR1;                   /* RXPR1        */
       union un_mb15_0 RXPR0;                   /* RXPR0        */
       _UBYTE wk5[4];                           /*              */
       union un_mb3116 RFPR1;                   /* RFPR1        */
       union un_mb15_0 RFPR0;                   /* RFPR0        */
       _UBYTE wk6[4];                           /*              */
       union un_mb3116 MBIMR1;                  /* MBIMR1       */
       union un_mb15_0 MBIMR0;                  /* MBIMR0       */
       _UBYTE wk7[4];                           /*              */
       union un_mb3116 UMSR1;                   /* UMSR1        */
       union un_mb15_0 UMSR0;                   /* UMSR0        */
       _UBYTE wk8[36];                          /*              */
       union {                                  /* TTCR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD TE:1;                /*   TE         */
                    _UWORD TS:1;                /*   TS         */
                    _UWORD CANC :1;             /*   CANC       */
                    _UWORD CME2:1;              /*   CME2       */
                    _UWORD CME1:1;              /*   CME1       */
                    _UWORD CME0:1;              /*   CME0       */
                    _UWORD :3;                  /*              */
                    _UWORD TCSC:1;              /*   TCSC       */
                    _UWORD TPSC :6;             /*   TPSC       */
                    } BIT;                      /*              */
             } TTCR0;                           /*              */
       _UBYTE wk9[2];                           /*              */
       union {                                  /* CMAX_TEW     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :5;                  /*              */
                    _UWORD CMAX:3;              /*   CMAX       */
                    _UWORD :4;                  /*              */
                    _UWORD TEW:4 ;              /*   TEW        */
                    } BIT;                      /*              */
             } CMAX_TEW;                        /*              */
       _UWORD RFTROFF;                          /* RFTROFF      */
       union {                                  /* TSR          */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :11;                 /*              */
                    _UWORD SNSM:1;              /*   SNSM       */
                    _UWORD TCMF2:1;             /*   TCMF2      */
                    _UWORD TCMF1:1;             /*   TCMF1      */
                    _UWORD TCMF0:1;             /*   TCMF0      */
                    _UWORD TO_NGR_ME:1;         /*   TO_NGR_ME  */
                    } BIT;                      /*              */
             } TSR;                             /*              */
       _UWORD CCR;                              /* CCR          */
       _UWORD TCNTR;                            /* TCNTR        */
       _UBYTE wk10[2];                          /*              */
       _UWORD CYCTR;                            /* CYCTR        */
       _UBYTE wk11[2];                          /*              */
       _UWORD RFMK;                             /* RFMK         */
       _UBYTE wk12[2];                          /*              */
       _UWORD TCMR0;                            /* TCMR0        */
       _UBYTE wk13[2];                          /*              */
       _UWORD TCMR1;                            /* TCMR1        */
       _UBYTE wk14[2];                          /*              */
       _UWORD TCMR2;                            /* TCMR2        */
       _UBYTE wk15[2];                          /*              */
       _UWORD TTTSEL;                           /* TTTSEL       */
       _UBYTE wk16[90];                         /*              */
       struct {
             union {                            /* CONTROL0     */
                   _UDWORD LONG;                /*  Long Access */
                   struct {                     /*  Word Access */
                          _UWORD H;             /*   High       */
                          _UWORD L;             /*   Low        */
                          } WORD;               /*              */
                   struct {                     /*  Bit Access  */
                          _UDWORD IDE:1;        /*   IDE        */
                          _UDWORD RTR:1;        /*   RTR        */
                          _UDWORD :1;           /*              */
                          _UDWORD STDID:11;     /*   STDID      */
                          _UDWORD EXTID:18;     /*   EXTID      */
                          } BIT;                /*              */
                   } CONTROL0;                  /*              */
             union {                            /* LAFM         */
                   _UDWORD LONG;                /*  Long Access */
                   struct {                     /*  Word Access */
                          _UWORD H;             /*   High       */
                          _UWORD L;             /*   Low        */
                          } WORD;               /*              */
                   struct {                     /*  Bit Access  */
                          _UDWORD IDE:1;        /*   IDE        */
                          _UDWORD :2;           /*              */
                          _UDWORD STDID_LAFM:11;/*   STDID_LAFM */
                          _UDWORD EXTID_LAFM:18;/*   EXTID_LAFM */
                          } BIT;                /*              */
                   } LAFM;                      /*              */
             _UBYTE MSG_DATA[8];                /* MSG_DATA     */
             union {                            /* CONTROL1     */
                   _UWORD WORD;                 /*  Word Access */
                   struct {                     /*  Byte Access */
                          _UBYTE H;             /*   High       */
                          _UBYTE L;             /*   Low        */
                          } BYTE;               /*              */
                   struct {                     /*  Bit Access  */
                          _UWORD :2;            /*              */
                          _UWORD NMC:1;         /*   NMC        */
                          _UWORD ATX:1;         /*   ATX        */
                          _UWORD DART:1;        /*   DART       */
                          _UWORD MBC:3;         /*   MBC        */
                          _UWORD :4;            /*              */
                          _UWORD DLC:4;         /*   DLC        */
                          } BIT;                /*              */
                   } CONTROL1;                  /*              */
             _UWORD TIMESTAMP;                  /* TIMESTAMP    */
             _UWORD TTT;                        /* TTT          */
             union {                            /* TTC          */
                   _UWORD WORD;                 /*  Word Access */
                   struct {                     /*  Bit Access  */
                          _UWORD TTW:2;         /*   TTW        */
                          _UWORD Offset:6;      /*   Offset     */
                          _UWORD :5;            /*              */
                          _UWORD rep_factor:3;  /*   rep_factor */
                          } BIT;                /*              */
                   } TTC;                       /*              */
             _UBYTE wk[8];                      /*              */
       } MB[32];                                /*              */
};
struct st_ieb {                                 /* struct IEB   */
       union {                                  /* IECTR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE IOL:1;               /*   IOL        */
                    _UBYTE DEE:1;               /*   DEE        */
                    _UBYTE :1;                  /*              */
                    _UBYTE RE:1;                /*   RE         */
                    _UBYTE :3;                  /*              */
                    } BIT;                      /*              */
             } IECTR;                           /*              */
       union {                                  /* IECMR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :5;                  /*              */
                    _UBYTE CMD:3;               /*   CMD        */
                    } BIT;                      /*              */
             } IECMR;                           /*              */
       union {                                  /* IEMCR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE SS:1;                /*   SS         */
                    _UBYTE RN:3;                /*   RN         */
                    _UBYTE CTL:4;               /*   CTL        */
                    } BIT;                      /*              */
             } IEMCR;                           /*              */
       union {                                  /* IEAR1        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE IARL4:4;             /*   IARL4      */
                    _UBYTE IMD:2;               /*   IMD        */
                    _UBYTE :1;                  /*              */
                    _UBYTE STE:1;               /*   STE        */
                    } BIT;                      /*              */
             } IEAR1;                           /*              */
       union {                                  /* IEAR2        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE IARU8:8;             /*   IARU8      */
                    } BIT;                      /*              */
             } IEAR2;                           /*              */
       union {                                  /* IESA1        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ISAL4:4;             /*   ISAL4      */
                    _UBYTE :4;                  /*              */
                    } BIT;                      /*              */
             } IESA1;                           /*              */
       union {                                  /* IESA2        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ISAU8:8;             /*   ISAU8      */
                    } BIT;                      /*              */
             } IESA2;                           /*              */
       _UBYTE IETBFL;                           /* IETBFL       */
       _UBYTE wk0[1];                           /*              */
       union {                                  /* IEMA1        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE IMAL4:4;             /*   IMAL4      */
                    _UBYTE :4;                  /*              */
                    } BIT;                      /*              */
             } IEMA1;                           /*              */
       union {                                  /* IEMA2        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE IMAU8:8;             /*   IMAU8      */
                    } BIT;                      /*              */
             } IEMA2;                           /*              */
       union {                                  /* IERCTL       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :4;                  /*              */
                    _UBYTE RCTL:4;              /*   RCTL       */
                    } BIT;                      /*              */
             } IERCTL;                          /*              */
       _UBYTE IERBFL;                           /* IERBFL       */
       _UBYTE wk1[1];                           /*              */
       union {                                  /* IELA1        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ILAL8:8;             /*   ILAL8      */
                    } BIT;                      /*              */
             } IELA1;                           /*              */
       union {                                  /* IELA2        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :4;                  /*              */
                    _UBYTE ILAU4:4;             /*   ILAU4      */
                    } BIT;                      /*              */
             } IELA2;                           /*              */
       union {                                  /* IEFLG        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE CMX:1;               /*   CMX        */
                    _UBYTE MRQ:1;               /*   MRQ        */
                    _UBYTE SRQ:1;               /*   SRQ        */
                    _UBYTE SRE:1;               /*   SRE        */
                    _UBYTE LCK:1;               /*   LCK        */
                    _UBYTE :1;                  /*              */
                    _UBYTE RSS:1;               /*   RSS        */
                    _UBYTE GG:1;                /*   GG         */
                    } BIT;                      /*              */
             } IEFLG;                           /*              */
       union {                                  /* IETSR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE TXS:1;               /*   TXS        */
                    _UBYTE TXF:1;               /*   TXF        */
                    _UBYTE :1;                  /*              */
                    _UBYTE TXEAL:1;             /*   TXEAL      */
                    _UBYTE TXETTME:1;           /*   TXETTME    */
                    _UBYTE TXERO:1;             /*   TXERO      */
                    _UBYTE TXEACK:1;            /*   TXEACK     */
                    } BIT;                      /*              */
             } IETSR;                           /*              */
       union {                                  /* IEIET        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE TXSE:1;              /*   TXSE       */
                    _UBYTE TXFE:1;              /*   TXFE       */
                    _UBYTE :1;                  /*              */
                    _UBYTE TXEALE:1;            /*   TXEALE     */
                    _UBYTE TXETTMEE:1;          /*   TXETTMEE   */
                    _UBYTE TXEROE:1;            /*   TXEROE     */
                    _UBYTE TXEACKE:1;           /*   TXEACKE    */
                    } BIT;                      /*              */
             } IEIET;                           /*              */
       _UBYTE wk2[1];                           /*              */
       union {                                  /* IERSR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE RXBSY:1;             /*   RXBSY      */
                    _UBYTE RXS:1;               /*   RXS        */
                    _UBYTE RXF:1;               /*   RXF        */
                    _UBYTE RXEDE:1;             /*   RXEDE      */
                    _UBYTE RXEOVE:1;            /*   RXEOVE     */
                    _UBYTE RXERTME:1;           /*   RXERTME    */
                    _UBYTE RXEDLE:1;            /*   RXEDLE     */
                    _UBYTE RXEPE:1;             /*   RXEPE      */
                    } BIT;                      /*              */
             } IERSR;                           /*              */
       union {                                  /* IEIER        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE RXBSYE:1;            /*   RXBSYE     */
                    _UBYTE RXSE:1;              /*   RXSE       */
                    _UBYTE RXFE:1;              /*   RXFE       */
                    _UBYTE RXEDEE:1;            /*   RXEDEE     */
                    _UBYTE RXEOVEE:1;           /*   RXEOVEE    */
                    _UBYTE RXERTMEE:1;          /*   RXERTMEE   */
                    _UBYTE RXEDLEE:1;           /*   RXEDLEE    */
                    _UBYTE RXEPEE:1;            /*   RXEPEE     */
                    } BIT;                      /*              */
             } IEIER;                           /*              */
       _UBYTE wk3[2];                           /*              */
       union {                                  /* IECKSR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :3;                  /*              */
                    _UBYTE CKS3:1;              /*   CKS3       */
                    _UBYTE :1;                  /*              */
                    _UBYTE CKS:3;               /*   CKS        */
                    } BIT;                      /*              */
             } IECKSR;                          /*              */
       _UBYTE wk4[231];                         /*              */
       union {                                  /* IETB001-128  */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE TB:8;                /*  TB          */
                    } BIT;                      /*              */
             } IETB[128];                       /*              */
       _UBYTE wk5[128];                         /*              */
       union {                                  /* IERB001-128  */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE RB:8;                /*  RB          */
                    } BIT;                      /*              */
             } IERB[128];                       /*              */
};                                              /*              */
struct st_spdif {                               /* struct SPDIF */
       _UDWORD TLCA;                            /* TLCA         */
       _UDWORD TRCA;                            /* TRCA         */
       union {                                  /* TLCS         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :2;                 /*              */
                    _UDWORD CLAC:2;             /*   CLAC       */
                    _UDWORD FS:4;               /*   FS         */
                    _UDWORD CHNO:4;             /*   CHNO       */
                    _UDWORD SRCNO:4;            /*   SRCNO      */
                    _UDWORD CATCD:8;            /*   CATCD      */
                    _UDWORD :2;                 /*              */
                    _UDWORD CTL:5;              /*   CTL        */
                    _UDWORD :1;                 /*              */
                    } BIT;                      /*              */
             } TLCS;                            /*              */
       union {                                  /* TRCS         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :2;                 /*              */
                    _UDWORD CLAC:2;             /*   CLAC       */
                    _UDWORD FS:4;               /*   FS         */
                    _UDWORD CHNO:4;             /*   CHNO       */
                    _UDWORD SRCNO:4;            /*   SRCNO      */
                    _UDWORD CATCD:8;            /*   CATCD      */
                    _UDWORD :2;                 /*              */
                    _UDWORD CTL:5;              /*   CTL        */
                    _UDWORD :1;                 /*              */
                    } BIT;                      /*              */
             } TRCS;                            /*              */
       _UDWORD TUI;                             /* TUI          */
       _UDWORD RLCA;                            /* RLCA         */
       _UDWORD RRCA;                            /* RRCA         */
       union {                                  /* RLCS         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :2;                 /*              */
                    _UDWORD CLAC:2;             /*   CLAC       */
                    _UDWORD FS:4;               /*   FS         */
                    _UDWORD CHNO:4;             /*   CHNO       */
                    _UDWORD SRCNO:4;            /*   SRCNO      */
                    _UDWORD CATCD:8;            /*   CATCD      */
                    _UDWORD :2;                 /*              */
                    _UDWORD CTL:5;              /*   CTL        */
                    _UDWORD :1;                 /*              */
                    } BIT;                      /*              */
             } RLCS;                            /*              */
       union {                                  /* RRCS         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :2;                 /*              */
                    _UDWORD CLAC:2;             /*   CLAC       */
                    _UDWORD FS:4;               /*   FS         */
                    _UDWORD CHNO:4;             /*   CHNO       */
                    _UDWORD SRCNO:4;            /*   SRCNO      */
                    _UDWORD CATCD:8;            /*   CATCD      */
                    _UDWORD :2;                 /*              */
                    _UDWORD CTL:5;              /*   CTL        */
                    _UDWORD :1;                 /*              */
                    } BIT;                      /*              */
             } RRCS;                            /*              */
       _UDWORD RUI;                             /* RUI          */
       union {                                  /* CTRL         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :3;                 /*              */
                    _UDWORD CKS:1;              /*   CKS        */
                    _UDWORD :1;                 /*              */
                    _UDWORD PB:1;               /*   PB         */
                    _UDWORD RASS:2;             /*   RASS       */
                    _UDWORD TASS:2;             /*   TASS       */
                    _UDWORD RDE:1;              /*   RDE        */
                    _UDWORD TDE:1;              /*   TDE        */
                    _UDWORD NCSI:1;             /*   NCSI       */
                    _UDWORD AOS:1;              /*   AOS        */
                    _UDWORD RME:1;              /*   RME        */
                    _UDWORD TME:1;              /*   TME        */
                    _UDWORD REIE:1;             /*   REIE       */
                    _UDWORD TEIE:1;             /*   TEIE       */
                    _UDWORD UBOI:1;             /*   UBOI       */
                    _UDWORD UBUI:1;             /*   UBUI       */
                    _UDWORD CREI:1;             /*   CREI       */
                    _UDWORD PAEI:1;             /*   PAEI       */
                    _UDWORD PREI:1;             /*   PREI       */
                    _UDWORD CSEI:1;             /*   CSEI       */
                    _UDWORD ABOI:1;             /*   ABOI       */
                    _UDWORD ABUI:1;             /*   ABUI       */
                    _UDWORD RUII:1;             /*   RUII       */
                    _UDWORD TUII:1;             /*   TUII       */
                    _UDWORD RCSI:1;             /*   RCSI       */
                    _UDWORD RCBI:1;             /*   RCBI       */
                    _UDWORD TCSI:1;             /*   TCSI       */
                    _UDWORD TCBI:1;             /*   TCBI       */
                    } BIT;                      /*              */
             } CTRL;                            /*              */
       union {                                  /* STAT         */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :15;                /*              */
                    _UDWORD CMD:1;              /*   CMD        */
                    _UDWORD RIS:1;              /*   RIS        */
                    _UDWORD TIS:1;              /*   TIS        */
                    _UDWORD UBO:1;              /*   UBO        */
                    _UDWORD UBU:1;              /*   UBU        */
                    _UDWORD CE:1;               /*   CE         */
                    _UDWORD PARE:1;             /*   PARE       */
                    _UDWORD PREE:1;             /*   PREE       */
                    _UDWORD CSE:1;              /*   CSE        */
                    _UDWORD ABO:1;              /*   ABO        */
                    _UDWORD ABU:1;              /*   ABU        */
                    _UDWORD RUIR:1;             /*   RUIR       */
                    _UDWORD TUIR:1;             /*   TUIR       */
                    _UDWORD CSRX:1;             /*   CSRX       */
                    _UDWORD CBRX:1;             /*   CBRX       */
                    _UDWORD CSTX:1;             /*   CSTX       */
                    _UDWORD CBTX:1;             /*   CBTX       */
                    } BIT;                      /*              */
             } STAT;                            /*              */
       _UDWORD TDAD;                            /* TDAD         */
       _UDWORD RDAD;                            /* RDAD         */
};                                              /*              */
struct st_romdec {                              /* struct ROMDEC */
       union {                                  /* CROMEN       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE SUBC_EN:1;           /*   SUBC_EN    */
                    _UBYTE CROM_EN:1;           /*   CROM_EN    */
                    _UBYTE CROM_STP:1;          /*   CROM_STP   */
                    _UBYTE :5;                  /*              */
                    } BIT;                      /*              */
             } CROMEN;                          /*              */
       union {                                  /* CROMSY0      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE SY_AUT:1;            /*   SY_AUT     */
                    _UBYTE SY_IEN:1;            /*   SY_IEN     */
                    _UBYTE SY_DEN:1;            /*   SY_DEN     */
                    _UBYTE :5;                  /*              */
                    } BIT;                      /*              */
             } CROMSY0;                         /*              */
       union {                                  /* CROMCTL0     */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE MD_DESC:1;           /*   MD_DESC    */
                    _UBYTE :1;                  /*              */
                    _UBYTE MD_AUTO:1;           /*   MD_AUTO    */
                    _UBYTE MD_AUTOS1:1;         /*   MD_AUTOS1  */
                    _UBYTE MD_AUTOS2:1;         /*   MD_AUTOS2  */
                    _UBYTE MD_SEC:3;            /*   MD_SEC     */
                    } BIT;                      /*              */
             } CROMCTL0;                        /*              */
       union {                                  /* CROMCTL1     */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE M2F2EDC:1;           /*   M2F2EDC    */
                    _UBYTE MD_DEC:3;            /*   MD_DEC     */
                    _UBYTE :2;                  /*              */
                    _UBYTE MD_PQREP:2;          /*   MD_PQREP   */
                    } BIT;                      /*              */
             } CROMCTL1;                        /*              */
       _UBYTE wk0[1];                           /*              */
       union {                                  /* CROMCTL3     */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE STP_ECC:1;           /*   STP_ECC    */
                    _UBYTE STP_EDC:1;           /*   STP_EDC    */
                    _UBYTE :1;                  /*              */
                    _UBYTE STP_MD:1;            /*   STP_MD     */
                    _UBYTE STP_MIN:1;           /*   STP_MIN    */
                    _UBYTE :3;                  /*              */
                    } BIT;                      /*              */
             } CROMCTL3;                        /*              */
       union {                                  /* CROMCTL4     */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE LINKOFF:1;           /*   LINKOFF    */
                    _UBYTE LINK2:1;             /*   LINK2      */
                    _UBYTE :1;                  /*              */
                    _UBYTE EROSEL:1;            /*   EROSEL     */
                    _UBYTE NO_ECC:1;            /*   NO_ECC     */
                    _UBYTE :3;                  /*              */
                    } BIT;                      /*              */
             } CROMCTL4;                        /*              */
       union {                                  /* CROMCTL5     */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :7;                  /*              */
                    _UBYTE MSF_LBA_SEL:1;       /*   MSF_LBA_SEL */
                    } BIT;                      /*              */
             } CROMCTL5;                        /*              */
       union {                                  /* CROMST0      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE ST_SYIL:1;           /*   ST_SYIL    */
                    _UBYTE ST_SYNO:1;           /*   ST_SYNO    */
                    _UBYTE ST_BLKS:1;           /*   ST_BLKS    */
                    _UBYTE ST_BLKL:1;           /*   ST_BLKL    */
                    _UBYTE ST_SECS:1;           /*   ST_SECS    */
                    _UBYTE ST_SECL:1;           /*   ST_SECL    */
                    } BIT;                      /*              */
             } CROMST0;                         /*              */
       union {                                  /* CROMST1      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :4;                  /*              */
                    _UBYTE ER2_HEAD0:1;         /*   ER2_HEAD0  */
                    _UBYTE ER2_HEAD1:1;         /*   ER2_HEAD1  */
                    _UBYTE ER2_HEAD2:1;         /*   ER2_HEAD2  */
                    _UBYTE ER2_HEAD3:1;         /*   ER2_HEAD3  */
                    } BIT;                      /*              */
             } CROMST1;                         /*              */
       _UBYTE wk1[1];                           /*              */
       union {                                  /* CROMST3      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ER2_SHEAD0:1;        /*   ER2_SHEAD0 */
                    _UBYTE ER2_SHEAD1:1;        /*   ER2_SHEAD1 */
                    _UBYTE ER2_SHEAD2:1;        /*   ER2_SHEAD2 */
                    _UBYTE ER2_SHEAD3:1;        /*   ER2_SHEAD3 */
                    _UBYTE ER2_SHEAD4:1;        /*   ER2_SHEAD4 */
                    _UBYTE ER2_SHEAD5:1;        /*   ER2_SHEAD5 */
                    _UBYTE ER2_SHEAD6:1;        /*   ER2_SHEAD6 */
                    _UBYTE ER2_SHEAD7:1;        /*   ER2_SHEAD7 */
                    } BIT;                      /*              */
             } CROMST3;                         /*              */
       union {                                  /* CROMST4      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE NG_MD:1;             /*   NG_MD      */
                    _UBYTE NG_MDCMP1:1;         /*   NG_MDCMP1  */
                    _UBYTE NG_MDCMP2:1;         /*   NG_MDCMP2  */
                    _UBYTE NG_MDCMP3:1;         /*   NG_MDCMP3  */
                    _UBYTE NG_MDCMP4:1;         /*   NG_MDCMP4  */
                    _UBYTE NG_MDDEF:1;          /*   NG_MDDEF   */
                    _UBYTE NG_MDTIM1:1;         /*   NG_MDTIM1  */
                    _UBYTE NG_MDTIM2:1;         /*   NG_MDTIM2  */
                    } BIT;                      /*              */
             } CROMST4;                         /*              */
       union {                                  /* CROMST5      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ST_AMD:3;            /*   ST_AMD     */
                    _UBYTE ST_MDX:1;            /*   ST_MDX     */
                    _UBYTE LINK_ON:1;           /*   LINK_ON    */
                    _UBYTE LINK_DET:1;          /*   LINK_DET   */
                    _UBYTE LINK_SDET:1;         /*   LINK_SDET  */
                    _UBYTE LINK_OUT1:1;         /*   LINK_OUT1  */
                    } BIT;                      /*              */
             } CROMST5;                         /*              */
       union {                                  /* CROMST6      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ST_ERR:1;            /*   ST_ERR     */
                    _UBYTE :1;                  /*              */
                    _UBYTE ST_ECCABT:1;         /*   ST_ECCABT  */
                    _UBYTE ST_ECCNG:1;          /*   ST_ECCNG   */
                    _UBYTE ST_ECCP:1;           /*   ST_ECCP    */
                    _UBYTE ST_ECCQ:1;           /*   ST_ECCQ    */
                    _UBYTE ST_EDC1:1;           /*   ST_EDC1    */
                    _UBYTE ST_EDC2:1;           /*   ST_EDC2    */
                    } BIT;                      /*              */
             } CROMST6;                         /*              */
       _UBYTE wk2[5];                           /*              */
       union {                                  /* CBUFST0      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE BUF_REF:1;           /*   BUF_REF    */
                    _UBYTE BUF_ACT:1;           /*   BUF_ACT    */
                    _UBYTE :6;                  /*              */
                    } BIT;                      /*              */
             } CBUFST0;                         /*              */
       union {                                  /* CBUFST1      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE BUF_ECC:1;           /*   BUF_ECC    */
                    _UBYTE BUF_EDC:1;           /*   BUF_EDC    */
                    _UBYTE :1;                  /*              */
                    _UBYTE BUF_MD:1;            /*   BUF_MD     */
                    _UBYTE BUF_MIN:1;           /*   BUF_MIN    */
                    _UBYTE :3;                  /*              */
                    } BIT;                      /*              */
             } CBUFST1;                         /*              */
       union {                                  /* CBUFST2      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE BUF_NG:1;            /*   BUF_NG     */
                    _UBYTE :7;                  /*              */
                    } BIT;                      /*              */
             } CBUFST2;                         /*              */
       _UBYTE wk3[1];                           /*              */
       _UBYTE HEAD00;                           /* HEAD00       */
       _UBYTE HEAD01;                           /* HEAD01       */
       _UBYTE HEAD02;                           /* HEAD02       */
       _UBYTE HEAD03;                           /* HEAD03       */
       _UBYTE SHEAD00;                          /* SHEAD00      */
       _UBYTE SHEAD01;                          /* SHEAD01      */
       _UBYTE SHEAD02;                          /* SHEAD02      */
       _UBYTE SHEAD03;                          /* SHEAD03      */
       _UBYTE SHEAD04;                          /* SHEAD04      */
       _UBYTE SHEAD05;                          /* SHEAD05      */
       _UBYTE SHEAD06;                          /* SHEAD06      */
       _UBYTE SHEAD07;                          /* SHEAD07      */
       _UBYTE HEAD20;                           /* HEAD20       */
       _UBYTE HEAD21;                           /* HEAD21       */
       _UBYTE HEAD22;                           /* HEAD22       */
       _UBYTE HEAD23;                           /* HEAD23       */
       _UBYTE SHEAD20;                          /* SHEAD20      */
       _UBYTE SHEAD21;                          /* SHEAD21      */
       _UBYTE SHEAD22;                          /* SHEAD22      */
       _UBYTE SHEAD23;                          /* SHEAD23      */
       _UBYTE SHEAD24;                          /* SHEAD24      */
       _UBYTE SHEAD25;                          /* SHEAD25      */
       _UBYTE SHEAD26;                          /* SHEAD26      */
       _UBYTE SHEAD27;                          /* SHEAD27      */       
       _UBYTE wk4[16];                          /*              */
       union {                                  /* CBUFCTL0     */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE CBUF_AUT:1;          /*   CBUF_AUT   */
                    _UBYTE CBUF_EN:1;           /*   CBUF_EN    */
                    _UBYTE CBUF_LINK:1;         /*   CBUF_LINK  */
                    _UBYTE CBUF_MD:2;           /*   CBUF_MD    */
                    _UBYTE CBUF_TS:1;           /*   CBUF_TS    */
                    _UBYTE CBUF_Q:1;            /*   CBUF_Q     */
                    _UBYTE :1;                  /*              */
                    } BIT;                      /*              */
             } CBUFCTL0;                        /*              */
       _UBYTE CBUFCTL1;                         /* CBUFCTL1     */
       _UBYTE CBUFCTL2;                         /* CBUFCTL2     */
       _UBYTE CBUFCTL3;                         /* CBUFCTL3     */
       _UBYTE wk5[1];                           /*              */
       union {                                  /* CROMST0M     */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE ST_SYILM:1;          /*   ST_SYILM   */
                    _UBYTE ST_SYNOM:1;          /*   ST_SYNOM   */
                    _UBYTE ST_BLKSM:1;          /*   ST_BLKSM   */
                    _UBYTE ST_BLKLM:1;          /*   ST_BLKLM   */
                    _UBYTE ST_SECSM:1;          /*   ST_SECSM   */
                    _UBYTE ST_SECLM:1;          /*   ST_SECLM   */
                    } BIT;                      /*              */
             } CROMST0M;                        /*              */
       _UBYTE wk6[186];                         /*              */
       union {                                  /* ROMDECRST    */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE LOGICRST:1;          /*   LOGICRST   */
                    _UBYTE RAMRST:1;            /*   RAMRST     */
                    _UBYTE :6;                  /*              */
                    } BIT;                      /*              */
             } ROMDECRST;                       /*              */
       union {                                  /* RSTSTAT      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE RAMCLRST:1;          /*   RAMCLRST   */
                    _UBYTE :7;                  /*              */
                    } BIT;                      /*              */
             } RSTSTAT;                         /*              */
       union {                                  /* SSI          */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE BYTEND:1;            /*   BYTEND     */
                    _UBYTE BITEND:1;            /*   BITEND     */
                    _UBYTE BUFEND0:2;           /*   BUFEND0    */
                    _UBYTE BUFEND1:2;           /*   BUFEND1    */
                    _UBYTE :2;                  /*              */
                    } BIT;                      /*              */
             } SSI;                             /*              */
       _UBYTE wk7[5];                           /*              */
       union {                                  /* INTHOLD      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE ISEC:1;              /*   ISEC       */
                    _UBYTE ITARG:1;             /*   ITARG      */
                    _UBYTE ISY:1;               /*   ISY        */
                    _UBYTE IERR:1;              /*   IERR       */
                    _UBYTE IBUF:1;              /*   IBUF       */
                    _UBYTE IREADY:1;            /*   IREADY     */
                    _UBYTE :2;                  /*              */
                    } BIT;                      /*              */
             } INTHOLD;                         /*              */
       union {                                  /* INHINT       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE INHISEC:1;           /*   INHISEC    */
                    _UBYTE INHITARG:1;          /*   INHITARG   */
                    _UBYTE INHISY:1;            /*   INHISY     */
                    _UBYTE INHIERR:1;           /*   INHIERR    */
                    _UBYTE INHIBUF:1;           /*   INHIBUF    */
                    _UBYTE INHIREADY:1;         /*   INHIREADY  */
                    _UBYTE PREINHREQDM:1;       /*   PREINHREQDM */
                    _UBYTE PREINHIREADY:1;      /*   PREINHIREADY */
                    } BIT;                      /*              */
             } INHINT;                          /*              */
       _UBYTE wk8[246];                         /*              */
       _UDWORD STRMDIN;                         /* STRMDIN      */
       _UWORD STRMDOUT;                         /* STRMDOUT     */
};                                              /*              */
struct st_adc {                                 /* struct ADC   */
       union {                                  /* ADDRA        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } ADDRA;                           /*              */
       union {                                  /* ADDRB        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } ADDRB;                           /*              */
       union {                                  /* ADDRC        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } ADDRC;                           /*              */
       union {                                  /* ADDRD        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } ADDRD;                           /*              */
       union {                                  /* ADDRE        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } ADDRE;                           /*              */
       union {                                  /* ADDRF        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } ADDRF;                           /*              */
       union {                                  /* ADDRG        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } ADDRG;                           /*              */
       union {                                  /* ADDRH        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD D:16;                /*   D          */
                    } BIT;                      /*              */
             } ADDRH;                           /*              */
       _UBYTE wk0[16];                          /*              */
       union {                                  /* ADCSR        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD ADF:1;               /*   ADF        */
                    _UWORD ADIE:1;              /*   ADIE       */
                    _UWORD ADST:1;              /*   ADST       */
                    _UWORD TRGS:4;              /*   TRGS       */
                    _UWORD CKS:3;               /*   CKS        */
                    _UWORD MDS:3;               /*   MDS        */
                    _UWORD CH:3;                /*   CH         */
                    } BIT;                      /*              */
             } ADCSR;                           /*              */
};                                              /*              */
struct st_flctl {                               /* struct FLCTL */
       union {                                  /* FLCMNCR      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD ECCPOS2:1;          /*   ECCPOS[2]  */
                    _UDWORD _4ECCCNTEN:1;       /*   _4ECCCNTEN */
                    _UDWORD _4ECCEN:1;          /*   _4ECCEN    */
                    _UDWORD _4ECCCORRECT:1;     /*   _4ECCCORRECT */
                    _UDWORD BUSYON:1;           /*   BUSYON     */
                    _UDWORD :2;                 /*              */
                    _UDWORD SNAND:1;            /*   SNAND      */
                    _UDWORD QTSEL:1;            /*   QTSEL      */
                    _UDWORD :3;                 /*              */
                    _UDWORD ECCPOS:2;           /*   ECCPOS     */
                    _UDWORD ACM:2;              /*   ACM        */
                    _UDWORD NANDWF:1;           /*   NANDWF     */
                    _UDWORD :5;                 /*              */
                    _UDWORD CE:1;               /*   CE         */
                    _UDWORD :2;                 /*              */
                    _UDWORD :1;                 /*              */
                    } BIT;                      /*              */
             } FLCMNCR;                         /*              */
       union {                                  /* FLCMDCR      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD ADRCNT2:1;          /*   ADRCNT2    */
                    _UDWORD SCTCNT_:4;          /*   SCTCNT     */
                    _UDWORD ADRMD:1;            /*   ADRMD      */
                    _UDWORD CDSRC:1;            /*   CDSRC      */
                    _UDWORD DOSR:1;             /*   DOSR       */
                    _UDWORD :2;                 /*              */
                    _UDWORD SELRW:1;            /*   SELRW      */
                    _UDWORD DOADR:1;            /*   DOADR      */
                    _UDWORD ADRCNT:2;           /*   ADRCNT     */
                    _UDWORD DOCMD2:1;           /*   DOCMD2     */
                    _UDWORD DOCMD1:1;           /*   DOCMD1     */
                    _UDWORD SCTCNT:16;          /*   SCTCNT     */
                    } BIT;                      /*              */
             } FLCMDCR;                         /*              */
       union {                                  /* FLCMCDR      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :16;                /*              */
                    _UDWORD CMD2:8;             /*   CMD2       */
                    _UDWORD CMD1:8;             /*   CMD1       */
                    } BIT;                      /*              */
             } FLCMCDR;                         /*              */
       _UDWORD  FLADR;                          /* FLADR        */
       _UDWORD  FLDATAR;                        /* FLDATAR      */
       union {                                  /* FLDTCNTR     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD ECFLW:8;            /*   ECFLW      */
                    _UDWORD DTFLW:8;            /*   DTFLW      */
                    _UDWORD :4;                 /*              */
                    _UDWORD DTCNT:12;           /*   DTCNT      */
                    } BIT;                      /*              */
             } FLDTCNTR;                        /*              */
       union {                                  /* FLINTDMACR   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD _4ECEINTE:1;        /*   _4ECEINTE  */
                    _UDWORD ECERINTE:1;         /*   ECERINTE   */
                    _UDWORD :2;                 /*              */
                    _UDWORD FIFOTRG:2;          /*   FIFOTRG    */
                    _UDWORD AC1CLR:1;           /*   AC1CLR     */
                    _UDWORD AC0CLR:1;           /*   AC0CLR     */
                    _UDWORD DREQ1EN:1;          /*   DREQ1EN    */
                    _UDWORD DREQ0EN:1;          /*   DREQ0EN    */
                    _UDWORD :6;                 /*              */
                    _UDWORD ECERB:1;            /*   ECERB      */
                    _UDWORD STERB:1;            /*   STERB      */
                    _UDWORD BTOERB:1;           /*   BTOERB     */
                    _UDWORD TRREQF1:1;          /*   TRREQF1    */
                    _UDWORD TRREQF0:1;          /*   TRREQF0    */
                    _UDWORD STERINTE:1;         /*   STERINTE   */
                    _UDWORD RBERINTE:1;         /*   RBERINTE   */
                    _UDWORD TEINTE:1;           /*   TEINTE     */
                    _UDWORD TRINTE1:1;          /*   TRINTE1    */
                    _UDWORD TRINTE0:1;          /*   TRINTE0    */
                    } BIT;                      /*              */
             } FLINTDMACR;                      /*              */
       union {                                  /* FLBSYTMR     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :12;                /*              */
                    _UDWORD RBTMOUT:20;         /*   RBTMOUT    */
                    } BIT;                      /*              */
             } FLBSYTMR;                        /*              */
       union {                                  /* FLBSYCNT     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD STAT:8;             /*   STAT       */
                    _UDWORD :4;                 /*              */
                    _UDWORD RBTIMCNT:20;        /*   RBTIMCNT   */
                    } BIT;                      /*              */
             } FLBSYCNT;                        /*              */
       _UBYTE wk0[8];                           /*              */
       union {                                  /* FLTRCR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :5;                  /*              */
                    _UBYTE TRSTAT:1;            /*   TRSTAT     */
                    _UBYTE TREND:1;             /*   TREND      */
                    _UBYTE TRSTRT:1;            /*   TRSTRT     */
                    } BIT;                      /*              */
             } FLTRCR;                          /*              */
       _UBYTE wk1[11];                          /*              */
       union {                                  /* FLHOLDCR     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :31;                /*              */
                    _UDWORD HOLDEN:1;           /*   HOLDEN     */
                    } BIT;                      /*              */
             } FLHOLDCR;                        /*              */
       _UDWORD FLADR2;                          /* FLADR2       */
       _UBYTE wk2[16];                          /*              */
       _UDWORD FLDTFIFO;                        /* FLDTFIFO     */
       _UBYTE wk3[12];                          /*              */
       _UDWORD FLECFIFO;                        /* FLECFIFO     */
       _UBYTE wk4[28];                          /*              */
       union {                                  /* FL4ECCRES1   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD LOC1:10;            /*   LOC1       */
                    _UDWORD :6;                 /*              */
                    _UDWORD PAT1:10;            /*   PAT1       */
                    } BIT;                      /*              */
             } FL4ECCRES1;                      /*              */
       union {                                  /* FL4ECCRES2   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD LOC2:10;            /*   LOC2       */
                    _UDWORD :6;                 /*              */
                    _UDWORD PAT2:10;            /*   PAT2       */
                    } BIT;                      /*              */
             } FL4ECCRES2;                      /*              */
       union {                                  /* FL4ECCRES3   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD LOC3:10;            /*   LOC3       */
                    _UDWORD :6;                 /*              */
                    _UDWORD PAT3:10;            /*   PAT3       */
                    } BIT;                      /*              */
             } FL4ECCRES3;                      /*              */
       union {                                  /* FL4ECCRES4   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD LOC4:10;            /*   LOC4       */
                    _UDWORD :6;                 /*              */
                    _UDWORD PAT4:10;            /*   PAT4       */
                    } BIT;                      /*              */
             } FL4ECCRES4;                      /*              */
       union {                                  /* FL4ECCCR     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :29;                /*              */
                    _UDWORD _4ECCFA:1;          /*   _4ECCFA    */
                    _UDWORD _4ECCEND:1;         /*   _4ECCEND   */
                    _UDWORD _4ECCEXST:1;        /*   _4ECCEXST  */
                    } BIT;                      /*              */
             } FL4ECCCR;                        /*              */
       union {                                  /* FL4ECCCNT    */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    _UDWORD :5;                 /*              */
                    _UDWORD ERRCNT:11;          /*   ERRCNT     */
                    _UDWORD :13;                /*              */
                    _UDWORD ERRMAX:3;           /*   ERRMAX     */
                    } BIT;                      /*              */
             } FL4ECCCNT;                       /*              */
};                                              /*              */
struct st_usb {                                 /* struct USB   */
       union {                                  /* SYSCFG       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :5;                  /*              */
                    _UWORD SCKE:1;              /*   SCKE       */
                    _UWORD :2;                  /*              */
                    _UWORD HSE:1;               /*   HSE        */
                    _UWORD DCFM:1;              /*   DCFM       */
                    _UWORD DRPD:1;              /*   DRPD       */
                    _UWORD DPRPU:1;             /*   DPRPU      */
                    _UWORD :3;                  /*              */
                    _UWORD USBE:1;              /*   USBE       */
                    } BIT;                      /*              */
             } SYSCFG;                          /*              */
       union {                                  /* BUSWAIT      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :12;                 /*              */
                    _UWORD BWAIT:4;             /*   BWAIT      */
                    } BIT;                      /*              */
             } BUSWAIT;                         /*              */
       union {                                  /* SYSSTS       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :14;                 /*              */
                    _UWORD LNST:2;              /*   LNST       */
                    } BIT;                      /*              */
             } SYSSTS;                          /*              */
       _UBYTE wk0[2];                           /*              */
       union {                                  /* DVSTCTR      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :7;                  /*              */
                    _UWORD WKUP:1;              /*   WKUP       */
                    _UWORD RWUPE:1;             /*   RWUPE      */
                    _UWORD USBRST:1;            /*   USBRST     */
                    _UWORD RESUME:1;            /*   RESUME     */
                    _UWORD UACT:1;              /*   UACT       */
                    _UWORD :1;                  /*              */
                    _UWORD RHST:3;              /*   RHST       */
                    } BIT;                      /*              */
             } DVSTCTR;                         /*              */
       _UBYTE wk1[2];                           /*              */
       union {                                  /* TESTMODE     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :12;                 /*              */
                    _UWORD UTST:4;              /*   UTST       */
                    } BIT;                      /*              */
             } TESTMODE;                        /*              */
       _UBYTE wk2[2];                           /*              */
       union {                                  /* D0FBCFG      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :11;                 /*              */
                    _UWORD TENDE:1;             /*   TENDE      */
                    _UWORD :4;                  /*              */
                    } BIT;                      /*              */
             } D0FBCFG;                         /*              */
       union {                                  /* D1FBCFG      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :11;                 /*              */
                    _UWORD TENDE:1;             /*   TENDE      */
                    _UWORD :4;                  /*              */
                    } BIT;                      /*              */
             } D1FBCFG;                         /*              */
       union {                                  /* CFIFO        */
             _UDWORD LONG;                      /*  Long Access */
             _UWORD WORD;                       /*  Word Access */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UDWORD FIFOPORT:32;        /*   FIFOPORT   */
                    } BIT;                      /*              */
             } CFIFO;                           /*              */
       union {                                  /* D0FIFO       */
             _UDWORD LONG;                      /*  Long Access */
             _UWORD WORD;                       /*  Word Access */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UDWORD FIFOPORT:32;        /*   FIFOPORT   */
                    } BIT;                      /*              */
             } D0FIFO;                          /*              */
       union {                                  /* D1FIFO       */
             _UDWORD LONG;                      /*  Long Access */
             _UWORD WORD;                       /*  Word Access */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UDWORD FIFOPORT:32;        /*   FIFOPORT   */
                    } BIT;                      /*              */
             } D1FIFO;                          /*              */
       union {                                  /* CFIFOSEL     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD RCNT:1;              /*   RCNT       */
                    _UWORD REW:1;               /*   REW        */
                    _UWORD :2;                  /*              */
                    _UWORD MBW:2;               /*   MBW        */
                    _UWORD :1;                  /*              */
                    _UWORD BIGEND:1;            /*   BIGEND     */
                    _UWORD :2;                  /*              */
                    _UWORD ISEL:1;              /*   ISEL       */
                    _UWORD :1;                  /*              */
                    _UWORD CURPIPE:4;           /*   CURPIPE    */
                    } BIT;                      /*              */
             } CFIFOSEL;                        /*              */
       union {                                  /* CFIFOCTR     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BVAL:1;              /*   BVAL       */
                    _UWORD BCLR:1;              /*   BCLR       */
                    _UWORD FRDY:1;              /*   FRDY       */
                    _UWORD :1;                  /*              */
                    _UWORD DTLN:12;             /*   DTLN       */
                    } BIT;                      /*              */
             } CFIFOCTR;                        /*              */
       _UBYTE wk3[4];                           /*              */
       union {                                  /* D0FIFOSEL    */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD RCNT:1;              /*   RCNT       */
                    _UWORD REW:1;               /*   REW        */
                    _UWORD DCLRM:1;             /*   DCLRM      */
                    _UWORD DREQE:1;             /*   DREQE      */
                    _UWORD MBW:2;               /*   MBW        */
                    _UWORD :1;                  /*              */
                    _UWORD BIGEND:1;            /*   BIGEND     */
                    _UWORD :4;                  /*              */
                    _UWORD CURPIPE:4;           /*   CURPIPE    */
                    } BIT;                      /*              */
             } D0FIFOSEL;                       /*              */
       union {                                  /* D0FIFOCTR    */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BVAL:1;              /*   BVAL       */
                    _UWORD BCLR:1;              /*   BCLR       */
                    _UWORD FRDY:1;              /*   FRDY       */
                    _UWORD :1;                  /*              */
                    _UWORD DTLN:12;             /*   DTLN       */
                    } BIT;                      /*              */
             } D0FIFOCTR;                       /*              */
       union {                                  /* D1FIFOSEL    */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD RCNT:1;              /*   RCNT       */
                    _UWORD REW:1;               /*   REW        */
                    _UWORD DCLRM:1;             /*   DCLRM      */
                    _UWORD DREQE:1;             /*   DREQE      */
                    _UWORD MBW:2;               /*   MBW        */
                    _UWORD :1;                  /*              */
                    _UWORD BIGEND:1;            /*   BIGEND     */
                    _UWORD :4;                  /*              */
                    _UWORD CURPIPE:4;           /*   CURPIPE    */
                    } BIT;                      /*              */
             } D1FIFOSEL;                       /*              */
       union {                                  /* D1FIFOCTR    */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BVAL:1;              /*   BVAL       */
                    _UWORD BCLR:1;              /*   BCLR       */
                    _UWORD FRDY:1;              /*   FRDY       */
                    _UWORD :1;                  /*              */
                    _UWORD DTLN:12;             /*   DTLN       */
                    } BIT;                      /*              */
             } D1FIFOCTR;                       /*              */
       union {                                  /* INTENB0      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD VBSE:1;              /*   VBSE       */
                    _UWORD RSME:1;              /*   RSME       */
                    _UWORD SOFE:1;              /*   SOFE       */
                    _UWORD DVSE:1;              /*   DVSE       */
                    _UWORD CTRE:1;              /*   CTRE       */
                    _UWORD BEMPE:1;             /*   BEMPE      */
                    _UWORD NRDYE:1;             /*   NRDYE      */
                    _UWORD BRDYE:1;             /*   BRDYE      */
                    _UWORD :8;                  /*              */
                    } BIT;                      /*              */
             } INTENB0;                         /*              */
       union {                                  /* INTENB1      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD BCHGE:1;             /*   BCHGE      */
                    _UWORD :1;                  /*              */
                    _UWORD DTCHE:1;             /*   DTCHE      */
                    _UWORD ATTCHE:1;            /*   ATTCHE     */
                    _UWORD :4;                  /*              */
                    _UWORD EOFERRE:1;           /*   EOFERRE    */
                    _UWORD SIGNE:1;             /*   SIGNE      */
                    _UWORD SACKE:1;             /*   SACKE      */
                    _UWORD :4;                  /*              */
                    } BIT;                      /*              */
             } INTENB1;                         /*              */
       _UBYTE wk4[2];                           /*              */
       union {                                  /* BRDYENB      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :6;                  /*              */
                    _UWORD PIPE9BRDYE:1;        /*   PIPE9BRDYE */
                    _UWORD PIPE8BRDYE:1;        /*   PIPE8BRDYE */
                    _UWORD PIPE7BRDYE:1;        /*   PIPE7BRDYE */
                    _UWORD PIPE6BRDYE:1;        /*   PIPE6BRDYE */
                    _UWORD PIPE5BRDYE:1;        /*   PIPE5BRDYE */
                    _UWORD PIPE4BRDYE:1;        /*   PIPE4BRDYE */
                    _UWORD PIPE3BRDYE:1;        /*   PIPE3BRDYE */
                    _UWORD PIPE2BRDYE:1;        /*   PIPE2BRDYE */
                    _UWORD PIPE1BRDYE:1;        /*   PIPE1BRDYE */
                    _UWORD PIPE0BRDYE:1;        /*   PIPE0BRDYE */
                    } BIT;                      /*              */
             } BRDYENB;                         /*              */
       union {                                  /* NRDYENB      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :6;                  /*              */
                    _UWORD PIPE9NRDYE:1;        /*   PIPE9NRDYE */
                    _UWORD PIPE8NRDYE:1;        /*   PIPE8NRDYE */
                    _UWORD PIPE7NRDYE:1;        /*   PIPE7NRDYE */
                    _UWORD PIPE6NRDYE:1;        /*   PIPE6NRDYE */
                    _UWORD PIPE5NRDYE:1;        /*   PIPE5NRDYE */
                    _UWORD PIPE4NRDYE:1;        /*   PIPE4NRDYE */
                    _UWORD PIPE3NRDYE:1;        /*   PIPE3NRDYE */
                    _UWORD PIPE2NRDYE:1;        /*   PIPE2NRDYE */
                    _UWORD PIPE1NRDYE:1;        /*   PIPE1NRDYE */
                    _UWORD PIPE0NRDYE:1;        /*   PIPE0NRDYE */
                    } BIT;                      /*              */
             } NRDYENB;                         /*              */
       union {                                  /* BEMPENB      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :6;                  /*              */
                    _UWORD PIPE9BEMPE:1;        /*   PIPE9BEMPE */
                    _UWORD PIPE8BEMPE:1;        /*   PIPE8BEMPE */
                    _UWORD PIPE7BEMPE:1;        /*   PIPE7BEMPE */
                    _UWORD PIPE6BEMPE:1;        /*   PIPE6BEMPE */
                    _UWORD PIPE5BEMPE:1;        /*   PIPE5BEMPE */
                    _UWORD PIPE4BEMPE:1;        /*   PIPE4BEMPE */
                    _UWORD PIPE3BEMPE:1;        /*   PIPE3BEMPE */
                    _UWORD PIPE2BEMPE:1;        /*   PIPE2BEMPE */
                    _UWORD PIPE1BEMPE:1;        /*   PIPE1BEMPE */
                    _UWORD PIPE0BEMPE:1;        /*   PIPE0BEMPE */
                    } BIT;                      /*              */
             } BEMPENB;                         /*              */
       union {                                  /* SOFCFG       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :7;                  /*              */
                    _UWORD TRNENSEL:1;          /*   TRNENSEL   */
                    _UWORD :1;                  /*              */
                    _UWORD BRDYM:1;             /*   BRDYM      */
                    _UWORD :6;                  /*              */
                    } BIT;                      /*              */
             } SOFCFG;                          /*              */
       _UBYTE wk5[2];                           /*              */
       union {                                  /* INTSTS0      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD VBINT:1;             /*   VBINT      */
                    _UWORD RESM:1;              /*   RESM       */
                    _UWORD SOFR:1;              /*   SOFR       */
                    _UWORD DVST:1;              /*   DVST       */
                    _UWORD CTRT:1;              /*   CTRT       */
                    _UWORD BEMP:1;              /*   BEMP       */
                    _UWORD NRDY:1;              /*   NRDY       */
                    _UWORD BRDY:1;              /*   BRDY       */
                    _UWORD VBSTS:1;             /*   VBSTS      */
                    _UWORD DVSQ:3;              /*   DVSQ       */
                    _UWORD VALID:1;             /*   VALID      */
                    _UWORD CTSQ:3;              /*   CTSQ       */
                    } BIT;                      /*              */
             } INTSTS0;                         /*              */
       union {                                  /* INTSTS1      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD BCHG:1;              /*   BCHG       */
                    _UWORD :1;                  /*              */
                    _UWORD DTCH:1;              /*   DTCH       */
                    _UWORD ATTCH:1;             /*   ATTCH      */
                    _UWORD :4;                  /*              */
                    _UWORD EOFERR:1;            /*   EOFERR     */
                    _UWORD SIGN:1;              /*   SIGN       */
                    _UWORD SACK:1;              /*   SACK       */
                    _UWORD :4;                  /*              */
                    } BIT;                      /*              */
             } INTSTS1;                         /*              */
       _UBYTE wk6[2];                           /*              */
       union {                                  /* BRDYSTS      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :6;                  /*              */
                    _UWORD PIPE9BRDY:1;         /*   PIPE9BRDY  */
                    _UWORD PIPE8BRDY:1;         /*   PIPE8BRDY  */
                    _UWORD PIPE7BRDY:1;         /*   PIPE7BRDY  */
                    _UWORD PIPE6BRDY:1;         /*   PIPE6BRDY  */
                    _UWORD PIPE5BRDY:1;         /*   PIPE5BRDY  */
                    _UWORD PIPE4BRDY:1;         /*   PIPE4BRDY  */
                    _UWORD PIPE3BRDY:1;         /*   PIPE3BRDY  */
                    _UWORD PIPE2BRDY:1;         /*   PIPE2BRDY  */
                    _UWORD PIPE1BRDY:1;         /*   PIPE1BRDY  */
                    _UWORD PIPE0BRDY:1;         /*   PIPE0BRDY  */
                    } BIT;                      /*              */
             } BRDYSTS;                         /*              */
       union {                                  /* NRDYSTS      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :6;                  /*              */
                    _UWORD PIPE9NRDY:1;         /*   PIPE9NRDY  */
                    _UWORD PIPE8NRDY:1;         /*   PIPE8NRDY  */
                    _UWORD PIPE7NRDY:1;         /*   PIPE7NRDY  */
                    _UWORD PIPE6NRDY:1;         /*   PIPE6NRDY  */
                    _UWORD PIPE5NRDY:1;         /*   PIPE5NRDY  */
                    _UWORD PIPE4NRDY:1;         /*   PIPE4NRDY  */
                    _UWORD PIPE3NRDY:1;         /*   PIPE3NRDY  */
                    _UWORD PIPE2NRDY:1;         /*   PIPE2NRDY  */
                    _UWORD PIPE1NRDY:1;         /*   PIPE1NRDY  */
                    _UWORD PIPE0NRDY:1;         /*   PIPE0NRDY  */
                    } BIT;                      /*              */
             } NRDYSTS;                         /*              */
       union {                                  /* BEMPSTS      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :6;                  /*              */
                    _UWORD PIPE9BEMP:1;         /*   PIPE9BEMP  */
                    _UWORD PIPE8BEMP:1;         /*   PIPE8BEMP  */
                    _UWORD PIPE7BEMP:1;         /*   PIPE7BEMP  */
                    _UWORD PIPE6BEMP:1;         /*   PIPE6BEMP  */
                    _UWORD PIPE5BEMP:1;         /*   PIPE5BEMP  */
                    _UWORD PIPE4BEMP:1;         /*   PIPE4BEMP  */
                    _UWORD PIPE3BEMP:1;         /*   PIPE3BEMP  */
                    _UWORD PIPE2BEMP:1;         /*   PIPE2BEMP  */
                    _UWORD PIPE1BEMP:1;         /*   PIPE1BEMP  */
                    _UWORD PIPE0BEMP:1;         /*   PIPE0BEMP  */
                    } BIT;                      /*              */
             } BEMPSTS;                         /*              */
       union {                                  /* FRMNUM       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD OVRN:1;              /*   OVRN       */
                    _UWORD CRCE:1;              /*   CRCE       */
                    _UWORD :3;                  /*              */
                    _UWORD FRNM:11;             /*   FRNM       */
                    } BIT;                      /*              */
             } FRMNUM;                          /*              */
       union {                                  /* UFRMNUM      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :13;                 /*              */
                    _UWORD UFRNM:3;             /*   UFRNM      */
                    } BIT;                      /*              */
             } UFRMNUM;                         /*              */
       union {                                  /* USBADDR      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :9;                  /*              */
                    _UWORD USBADDR:7;           /*   USBADDR    */
                    } BIT;                      /*              */
             } USBADDR;                         /*              */
       _UBYTE wk7[2];                           /*              */
       union {                                  /* USBREQ       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BREQUEST:8;          /*   BREQUEST   */
                    _UWORD BMREQUESTTYPE:8;     /*   BMREQUESTTYPE */
                    } BIT;                      /*              */
             } USBREQ;                          /*              */
       _UWORD USBVAL;                           /* USBVAL       */
       _UWORD USBINDX;                          /* USBINDX      */
       _UWORD USBLENG;                          /* USBLENG      */
       union {                                  /* DCPCFG       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :11;                 /*              */
                    _UWORD DIR:1;               /*   DIR        */
                    _UWORD :4;                  /*              */
                    } BIT;                      /*              */
             } DCPCFG;                          /*              */
       union {                                  /* DCPMAXP      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD DEVSEL:4;            /*   DEVSEL     */
                    _UWORD :5;                  /*              */
                    _UWORD MXPS:7;              /*   MXPS       */
                    } BIT;                      /*              */
             } DCPMAXP;                         /*              */
       union {                                  /* DCPCTR       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BSTS:1;              /*   BSTS       */
                    _UWORD SUREQ:1;             /*   SUREQ      */
                    _UWORD CSCLR:1;             /*   CSCLR      */
                    _UWORD CSSTS:1;             /*   CSSTS      */
                    _UWORD SUREQCLR:1;          /*   SUREQCLR   */
                    _UWORD :2;                  /*              */
                    _UWORD SQCLR:1;             /*   SQCLR      */
                    _UWORD SQSET:1;             /*   SQSET      */
                    _UWORD SQMON:1;             /*   SQMON      */
                    _UWORD PBUSY:1;             /*   PBUSY      */
                    _UWORD PINGE:1;             /*   PINGE      */
                    _UWORD :1;                  /*              */
                    _UWORD CCPL:1;              /*   CCPL       */
                    _UWORD PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } DCPCTR;                          /*              */
       _UBYTE wk8[2];                           /*              */
       union {                                  /* PIPESEL      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :12;                 /*              */
                    _UWORD PIPESEL:4;           /*   PIPESEL    */
                    } BIT;                      /*              */
             } PIPESEL;                         /*              */
       _UBYTE wk9[2];                           /*              */
       union {                                  /* PIPECFG      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD TYPE:2;              /*   TYPE       */
                    _UWORD :3;                  /*              */
                    _UWORD BFRE:1;              /*   BFRE       */
                    _UWORD DBLB:1;              /*   DBLB       */
                    _UWORD CNTMD:1;             /*   CNTMD      */
                    _UWORD SHTNAK:1;            /*   SHTNAK     */
                    _UWORD :2;                  /*              */
                    _UWORD DIR:1;               /*   DIR        */
                    _UWORD EPNUM:4;             /*   EPNUM      */
                    } BIT;                      /*              */
             } PIPECFG;                         /*              */
       union {                                  /* PIPEBUF      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD BUFSIZE:5;           /*   BUFSIZE    */
                    _UWORD :3;                  /*              */
                    _UWORD BUFNMB:7;            /*   BUFNMB     */
                    } BIT;                      /*              */
             } PIPEBUF;                         /*              */
       union {                                  /* PIPEMAXP     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD DEVSEL:4;            /*   DEVSEL     */
                    _UWORD :1;                  /*              */
                    _UWORD MXPS:11;             /*   MXPS       */
                    } BIT;                      /*              */
             } PIPEMAXP;                        /*              */
       union {                                  /* PIPEPERI     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :3;                  /*              */
                    _UWORD IFIS:1;              /*   IFIS       */
                    _UWORD :9;                  /*              */
                    _UWORD IITV:3;              /*   IITV       */
                    } BIT;                      /*              */
             } PIPEPERI;                        /*              */
       union {                                  /* PIPE1CTR     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BSTS:1;              /*   BSTS       */
                    _UWORD INBUFM:1;            /*   INBUFM     */
                    _UWORD CSCLR:1;             /*   CSCLR      */
                    _UWORD CSSTS:1;             /*   CSSTS      */
                    _UWORD :1;                  /*              */
                    _UWORD ATREPM:1;            /*   ATREPM     */
                    _UWORD ACLRM:1;             /*   ACLRM      */
                    _UWORD SQCLR:1;             /*   SQCLR      */
                    _UWORD SQSET:1;             /*   SQSET      */
                    _UWORD SQMON:1;             /*   SQMON      */
                    _UWORD PBUSY:1;             /*   PBUSY      */
                    _UWORD :3;                  /*              */
                    _UWORD PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE1CTR;                        /*              */
       union {                                  /* PIPE2CTR     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BSTS:1;              /*   BSTS       */
                    _UWORD INBUFM:1;            /*   INBUFM     */
                    _UWORD CSCLR:1;             /*   CSCLR      */
                    _UWORD CSSTS:1;             /*   CSSTS      */
                    _UWORD :1;                  /*              */
                    _UWORD ATREPM:1;            /*   ATREPM     */
                    _UWORD ACLRM:1;             /*   ACLRM      */
                    _UWORD SQCLR:1;             /*   SQCLR      */
                    _UWORD SQSET:1;             /*   SQSET      */
                    _UWORD SQMON:1;             /*   SQMON      */
                    _UWORD PBUSY:1;             /*   PBUSY      */
                    _UWORD :3;                  /*              */
                    _UWORD PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE2CTR;                        /*              */
       union {                                  /* PIPE3CTR     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BSTS:1;              /*   BSTS       */
                    _UWORD INBUFM:1;            /*   INBUFM     */
                    _UWORD CSCLR:1;             /*   CSCLR      */
                    _UWORD CSSTS:1;             /*   CSSTS      */
                    _UWORD :1;                  /*              */
                    _UWORD ATREPM:1;            /*   ATREPM     */
                    _UWORD ACLRM:1;             /*   ACLRM      */
                    _UWORD SQCLR:1;             /*   SQCLR      */
                    _UWORD SQSET:1;             /*   SQSET      */
                    _UWORD SQMON:1;             /*   SQMON      */
                    _UWORD PBUSY:1;             /*   PBUSY      */
                    _UWORD :3;                  /*              */
                    _UWORD PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE3CTR;                        /*              */
       union {                                  /* PIPE4CTR     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BSTS:1;              /*   BSTS       */
                    _UWORD INBUFM:1;            /*   INBUFM     */
                    _UWORD CSCLR:1;             /*   CSCLR      */
                    _UWORD CSSTS:1;             /*   CSSTS      */
                    _UWORD :1;                  /*              */
                    _UWORD ATREPM:1;            /*   ATREPM     */
                    _UWORD ACLRM:1;             /*   ACLRM      */
                    _UWORD SQCLR:1;             /*   SQCLR      */
                    _UWORD SQSET:1;             /*   SQSET      */
                    _UWORD SQMON:1;             /*   SQMON      */
                    _UWORD PBUSY:1;             /*   PBUSY      */
                    _UWORD :3;                  /*              */
                    _UWORD PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE4CTR;                        /*              */
       union {                                  /* PIPE5CTR     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BSTS:1;              /*   BSTS       */
                    _UWORD INBUFM:1;            /*   INBUFM     */
                    _UWORD CSCLR:1;             /*   CSCLR      */
                    _UWORD CSSTS:1;             /*   CSSTS      */
                    _UWORD :1;                  /*              */
                    _UWORD ATREPM:1;            /*   ATREPM     */
                    _UWORD ACLRM:1;             /*   ACLRM      */
                    _UWORD SQCLR:1;             /*   SQCLR      */
                    _UWORD SQSET:1;             /*   SQSET      */
                    _UWORD SQMON:1;             /*   SQMON      */
                    _UWORD PBUSY:1;             /*   PBUSY      */
                    _UWORD :3;                  /*              */
                    _UWORD PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE5CTR;                        /*              */
       union {                                  /* PIPE6CTR     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BSTS:1;              /*   BSTS       */
                    _UWORD :1;                  /*              */
                    _UWORD CSCLR:1;             /*   CSCLR      */
                    _UWORD CSSTS:1;             /*   CSSTS      */
                    _UWORD :2;                  /*              */
                    _UWORD ACLRM:1;             /*   ACLRM      */
                    _UWORD SQCLR:1;             /*   SQCLR      */
                    _UWORD SQSET:1;             /*   SQSET      */
                    _UWORD SQMON:1;             /*   SQMON      */
                    _UWORD PBUSY:1;             /*   PBUSY      */
                    _UWORD :3;                  /*              */
                    _UWORD PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE6CTR;                        /*              */
       union {                                  /* PIPE7CTR     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BSTS:1;              /*   BSTS       */
                    _UWORD :1;                  /*              */
                    _UWORD CSCLR:1;             /*   CSCLR      */
                    _UWORD CSSTS:1;             /*   CSSTS      */
                    _UWORD :2;                  /*              */
                    _UWORD ACLRM:1;             /*   ACLRM      */
                    _UWORD SQCLR:1;             /*   SQCLR      */
                    _UWORD SQSET:1;             /*   SQSET      */
                    _UWORD SQMON:1;             /*   SQMON      */
                    _UWORD PBUSY:1;             /*   PBUSY      */
                    _UWORD :3;                  /*              */
                    _UWORD PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE7CTR;                        /*              */
       union {                                  /* PIPE8CTR     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BSTS:1;              /*   BSTS       */
                    _UWORD :1;                  /*              */
                    _UWORD CSCLR:1;             /*   CSCLR      */
                    _UWORD CSSTS:1;             /*   CSSTS      */
                    _UWORD :2;                  /*              */
                    _UWORD ACLRM:1;             /*   ACLRM      */
                    _UWORD SQCLR:1;             /*   SQCLR      */
                    _UWORD SQSET:1;             /*   SQSET      */
                    _UWORD SQMON:1;             /*   SQMON      */
                    _UWORD PBUSY:1;             /*   PBUSY      */
                    _UWORD :3;                  /*              */
                    _UWORD PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE8CTR;                        /*              */
       union {                                  /* PIPE9CTR     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD BSTS:1;              /*   BSTS       */
                    _UWORD :1;                  /*              */
                    _UWORD CSCLR:1;             /*   CSCLR      */
                    _UWORD CSSTS:1;             /*   CSSTS      */
                    _UWORD :2;                  /*              */
                    _UWORD ACLRM:1;             /*   ACLRM      */
                    _UWORD SQCLR:1;             /*   SQCLR      */
                    _UWORD SQSET:1;             /*   SQSET      */
                    _UWORD SQMON:1;             /*   SQMON      */
                    _UWORD PBUSY:1;             /*   PBUSY      */
                    _UWORD :3;                  /*              */
                    _UWORD PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE9CTR;                        /*              */
       _UBYTE wk10[14];                         /*              */
       union {                                  /* PIPE1TRE     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :6;                  /*              */
                    _UWORD TRENB:1;             /*   TRENB      */
                    _UWORD TRCLR:1;             /*   TRCLR      */
                    _UWORD :8;                  /*              */
                    } BIT;                      /*              */
             } PIPE1TRE;                        /*              */
       _UWORD PIPE1TRN;                         /* PIPE1TRN     */
       union {                                  /* PIPE2TRE     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :6;                  /*              */
                    _UWORD TRENB:1;             /*   TRENB      */
                    _UWORD TRCLR:1;             /*   TRCLR      */
                    _UWORD :8;                  /*              */
                    } BIT;                      /*              */
             } PIPE2TRE;                        /*              */
       _UWORD PIPE2TRN;                         /* PIPE2TRN     */
       union {                                  /* PIPE3TRE     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :6;                  /*              */
                    _UWORD TRENB:1;             /*   TRENB      */
                    _UWORD TRCLR:1;             /*   TRCLR      */
                    _UWORD :8;                  /*              */
                    } BIT;                      /*              */
             } PIPE3TRE;                        /*              */
       _UWORD PIPE3TRN;                         /* PIPE3TRN     */
       union {                                  /* PIPE4TRE     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :6;                  /*              */
                    _UWORD TRENB:1;             /*   TRENB      */
                    _UWORD TRCLR:1;             /*   TRCLR      */
                    _UWORD :8;                  /*              */
                    } BIT;                      /*              */
             } PIPE4TRE;                        /*              */
       _UWORD PIPE4TRN;                         /* PIPE4TRN     */
       union {                                  /* PIPE5TRE     */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :6;                  /*              */
                    _UWORD TRENB:1;             /*   TRENB      */
                    _UWORD TRCLR:1;             /*   TRCLR      */
                    _UWORD :8;                  /*              */
                    } BIT;                      /*              */
             } PIPE5TRE;                        /*              */
       _UWORD PIPE5TRN;                         /* PIPE5TRN     */
       _UBYTE wk11[30];                         /*              */
        union {                                 /* USBACSWR1    */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit  Access */
                    _UWORD :6;                  /*              */
                    _UWORD UACS25 :1;           /*  UACS25      */
                    _UWORD :9;                  /*              */
                    } BIT;                      /*              */
             } USBACSWR1;                       /*              */
       _UBYTE wk12[12];                         /*              */
       union {                                  /* DEVADD0      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD UPPHUB:4;            /*   UPPHUB     */
                    _UWORD HUBPORT:3;           /*   HUBPORT    */
                    _UWORD USBSPD:2;            /*   USBSPD     */
                    _UWORD :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD0;                         /*              */
       union {                                  /* DEVADD1      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD UPPHUB:4;            /*   UPPHUB     */
                    _UWORD HUBPORT:3;           /*   HUBPORT    */
                    _UWORD USBSPD:2;            /*   USBSPD     */
                    _UWORD :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD1;                         /*              */
       union {                                  /* DEVADD2      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD UPPHUB:4;            /*   UPPHUB     */
                    _UWORD HUBPORT:3;           /*   HUBPORT    */
                    _UWORD USBSPD:2;            /*   USBSPD     */
                    _UWORD :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD2;                         /*              */
       union {                                  /* DEVADD3      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD UPPHUB:4;            /*   UPPHUB     */
                    _UWORD HUBPORT:3;           /*   HUBPORT    */
                    _UWORD USBSPD:2;            /*   USBSPD     */
                    _UWORD :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD3;                         /*              */
       union {                                  /* DEVADD4      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD UPPHUB:4;            /*   UPPHUB     */
                    _UWORD HUBPORT:3;           /*   HUBPORT    */
                    _UWORD USBSPD:2;            /*   USBSPD     */
                    _UWORD :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD4;                         /*              */
       union {                                  /* DEVADD5      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD UPPHUB:4;            /*   UPPHUB     */
                    _UWORD HUBPORT:3;           /*   HUBPORT    */
                    _UWORD USBSPD:2;            /*   USBSPD     */
                    _UWORD :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD5;                         /*              */
       union {                                  /* DEVADD6      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD UPPHUB:4;            /*   UPPHUB     */
                    _UWORD HUBPORT:3;           /*   HUBPORT    */
                    _UWORD USBSPD:2;            /*   USBSPD     */
                    _UWORD :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD6;                         /*              */
       union {                                  /* DEVADD7      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD UPPHUB:4;            /*   UPPHUB     */
                    _UWORD HUBPORT:3;           /*   HUBPORT    */
                    _UWORD USBSPD:2;            /*   USBSPD     */
                    _UWORD :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD7;                         /*              */
       union {                                  /* DEVADD8      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD UPPHUB:4;            /*   UPPHUB     */
                    _UWORD HUBPORT:3;           /*   HUBPORT    */
                    _UWORD USBSPD:2;            /*   USBSPD     */
                    _UWORD :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD8;                         /*              */
       union {                                  /* DEVADD9      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD UPPHUB:4;            /*   UPPHUB     */
                    _UWORD HUBPORT:3;           /*   HUBPORT    */
                    _UWORD USBSPD:2;            /*   USBSPD     */
                    _UWORD :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD9;                         /*              */
       union {                                  /* DEVADDA      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :1;                  /*              */
                    _UWORD UPPHUB:4;            /*   UPPHUB     */
                    _UWORD HUBPORT:3;           /*   HUBPORT    */
                    _UWORD USBSPD:2;            /*   USBSPD     */
                    _UWORD :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADDA;                         /*              */
};                                              /*              */
struct st_vdc3 {                                /* struct VDC3  */
       union {                                  /* VIDEO_MODE   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :2;                 /*              */
                    _UDWORD RGB565:1;           /*   RGB565     */
                    _UDWORD INV_CbCr:1;         /*   INV_CbCr   */
                    _UDWORD :6;                 /*              */
                    _UDWORD BURST_MODE_DISP:1;  /*   BURST_MODE_DISP */
                    _UDWORD BURST_MODE_MAIN:1;  /*   BURST_MODE_MAIN */
                    _UDWORD :2;                 /*              */
                    _UDWORD ENDIAN_DISP:1;      /*   ENDIAN_DISP*/
                    _UDWORD ENDIAN_MAIN:1;      /*   ENDIAN_MAIN*/
                    _UDWORD :1;                 /*              */
                    _UDWORD SEL_EXSYNC:1;       /*   SEL_EXSYNC */
                    _UDWORD SEL_656601:1;       /*   SEL_656601 */
                    _UDWORD SEL_525625:1;       /*   SEL_525625 */
                    _UDWORD :7;                 /*              */
                    _UDWORD VIDEO_MODE:1;       /*   VIDEO_MODE */
                    _UDWORD :2;                 /*              */
                    _UDWORD VIDEO_DISP_EXE:1;   /*   VIDEO_DISP_EXE */
                    _UDWORD VIDEO_MAIN_EXE:1;   /*   VIDEO_MAIN_EXE */
                    } BIT;                      /*              */
            } VIDEO_MODE;                       /*              */
       union {                                  /* VIDEO_INT_CNT */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :3;                 /*              */
                    _UDWORD INT_V_EN:1;         /*   INT_V_EN   */
                    _UDWORD :3;                 /*              */
                    _UDWORD INT_F_EN:1;         /*   INT_F_EN   */
                    _UDWORD :3;                 /*              */
                    _UDWORD INT_UF_EN:1;        /*   INT_UF_EN  */
                    _UDWORD :3;                 /*              */
                    _UDWORD INT_OF_EN:1;        /*   INT_OF_EN  */
                    _UDWORD :3;                 /*              */
                    _UDWORD V_PERIOD:1;         /*   V_PERIOD   */
                    _UDWORD :3;                 /*              */
                    _UDWORD F_END:1;            /*   F_END      */
                    _UDWORD :3;                 /*              */
                    _UDWORD UNDER_FLOW:1;       /*   UNDER_FLOW */
                    _UDWORD :3;                 /*              */
                    _UDWORD OVER_FLOW:1;        /*   OVER_FLOW  */
                    } BIT;                      /*              */
             } VIDEO_INT_CNT;                   /*              */
       union {                                  /* VIDEO_TIM_CNT */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD FIELD_SKEW:10;      /*   FIELD_SKEW */
                    _UDWORD :10;                /*              */
                    _UDWORD VSYNC_TYP:1;        /*   VSYNC_TYP  */
                    _UDWORD HSYNC_TYP:1;        /*   HSYNC_TYP  */
                    _UDWORD :1;                 /*              */
                    _UDWORD VSYNC_TIM:1;        /*   VSYNC_TIM  */
                    _UDWORD HSYNC_TIM:1;        /*   HSYNC_TIM  */
                    _UDWORD VIDEO_TIM:1;        /*   VIDEO_TIM  */
                    } BIT;                      /*              */
             } VIDEO_TIM_CNT;                   /*              */
       _UBYTE wk0[244];                         /*              */
       union {                                  /* VIDEO_SIZE   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :7;                 /*              */
                    _UDWORD VIDEO_HEIGHT:9;     /*   VIDEO_HEIGHT */
                    _UDWORD :6;                 /*              */
                    _UDWORD VIDEO_WIDTH:10;     /*   VIDEO_WIDTH */
                    } BIT;                      /*              */
             } VIDEO_SIZE;                      /*              */
       union {                                  /* VIDEO_VSTART */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :7;                 /*              */
                    _UDWORD VIDEO_VSTART_TOP:9; /*   VIDEO_VSTART_TOP */
                    _UDWORD :7;                 /*              */
                    _UDWORD VIDEO_VSTART_BTM:9; /*   VIDEO_VSTART_BTM */
                    } BIT;                      /*              */
             } VIDEO_VSTART;                    /*              */
       union {                                  /* VIDEO_HSTART */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :23;                /*              */
                    _UDWORD VIDEO_HSTART:9;     /*   VIDEO_HSTART */
                    } BIT;                      /*              */
             } VIDEO_HSTART;                    /*              */
       union {                                  /* VIDEO_VSYNC_TIM1 */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD VIDEO_VSYNC_START1_TOP:10;/*   VIDEO_VSYNC_START1_TOP */
                    _UDWORD :6;                 /*              */
                    _UDWORD VIDEO_VSYNC_START1_BTM:10;/*   VIDEO_VSYNC_START1_BTM */
                    } BIT;                      /*              */
             } VIDEO_VSYNC_TIM1;                /*              */
       union {                                  /* VIDEO_SAVE_NUM */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD FIELD_NUM:10;       /*   FIELD_NUM */
                    _UDWORD :6;                 /*              */
                    _UDWORD FIELD_SAVE_NUM:10;  /*   FIELD_SAVE_NUM */
                    } BIT;                      /*              */
             } VIDEO_SAVE_NUM;                  /*              */
       union {                                  /* VIDEO_IMAGE_CNT */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD Contrast:8;         /*   Contrast   */
                    _UDWORD Bright:8 ;          /*   Bright     */
                    _UDWORD :6;                 /*              */
                    _UDWORD CLIP_Y:1;           /*   CLIP_Y     */
                    _UDWORD CLIP_C:1;           /*   CLIP_C     */
                    _UDWORD :1;                 /*              */
                    _UDWORD SUB_SCALE_V:1;      /*   SUB_SCALE_V */
                    _UDWORD SCALE_V:2;          /*   SCALE_V    */
                    _UDWORD :1;                 /*              */
                    _UDWORD SUB_SCALE_H:1;      /*   SUB_SCALE_H */
                    _UDWORD SCALE_H:2;          /*   SCALE_H    */
                    } BIT;                      /*              */
             } VIDEO_IMAGE_CNT;                 /*              */
       union {                                  /* VIDEO_BASEADR */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             } VIDEO_BASEADR;                    /*              */
       union {                                  /* VIDEO_LINE_OFFSET */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             } VIDEO_LINE_OFFSET;               /*              */
       union {                                  /* VIDEO_FIELD_OFFSET */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             } VIDEO_FIELD_OFFSET;              /*              */
       union {                                  /* VIDEO_LINEBUFF_NUM */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :23;                /*              */
                    _UDWORD VIDEO_LINEBUFF_NUM:9;/*   VIDEO_LINEBUFF_NUM */
                    } BIT;                      /*              */
             } VIDEO_LINEBUFF_NUM;              /*              */
       union {                                  /* VIDEO_DISP_SIZE */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :7;                 /*              */
                    _UDWORD VIDEO_DISP_HEIGHT:9;/*   VIDEO_DISP_HEIGHT */
                    _UDWORD :6;                 /*              */
                    _UDWORD VIDEO_DISP_WIDTH:10;/*   VIDEO_DISP_WIDTH */
                    } BIT;                      /*              */
             } VIDEO_DISP_SIZE;                 /*              */
       union {                                  /* VIDEO_DISP_HSTART */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :22;                /*              */
                    _UDWORD VIDEO_DISP_HSTART:10;/*   VIDEO_DISP_HSTART */
                    } BIT;                      /*              */
             } VIDEO_DISP_HSTART;               /*              */
       _UBYTE wk1[1744];                        /*              */
       union {                                  /* GRCMEN1      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD WE:1;               /*   WE         */
                    _UDWORD :29;                /*              */
                    _UDWORD DEN:1;              /*   DEN        */
                    _UDWORD VEN:1;              /*   VEN        */
                    } BIT;                      /*              */
             } GRCMEN1;                         /*              */
       union {                                  /* GRCBUSCNT1   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :23;                /*              */
                    _UDWORD BURST_MODE:1;       /*   BURST_MODE */
                    _UDWORD :3;                 /*              */
                    _UDWORD BUS_FORMAT:1;       /*   BUS_FORMAT */
                    _UDWORD :3;                 /*              */
                    _UDWORD ENDIAN:1;           /*   ENDIAN     */
                    } BIT;                      /*              */
             } GRCBUSCNT1;                      /*              */
       union {                                  /* GRCINTCNT1   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :15;                /*              */
                    _UDWORD INT_UF_EN:1;        /*   INT_UF_EN  */
                    _UDWORD :15;                /*              */
                    _UDWORD UNDER_FLOW:1;       /*   UNDER_FLOW */
                    } BIT;                      /*              */
             } GRCINTCNT1;                      /*              */
       _UBYTE wk2[764];                         /*              */
       union {                                  /* GROPSADR1    */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :3;                 /*              */
                    _UDWORD GROPSADR:29;        /*   GROPSADR   */
                    } BIT;                      /*              */
             } GROPSADR1;                       /*              */
       union {                                  /* GROPSWH1     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPSH:10;          /*   GROPSH     */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPSW:10;          /*   GROPSW     */
                    } BIT;                      /*              */
             } GROPSWH1;                        /*              */
       union {                                  /* GROPSOFST1   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :3;                 /*              */
                    _UDWORD GROPSOFST:29;       /*   GROPSOFST  */
                    } BIT;                      /*              */
             } GROPSOFST1;                      /*              */
       union {                                  /* GROPDPHV1    */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPDPV:10;         /*   GROPDPV    */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPDPH:10;         /*   GROPDPH    */
                    } BIT;                      /*              */
             } GROPDPHV1;                       /*              */
       union {                                  /* GROPEWH1     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPEH:10;          /*   GROPEH     */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPEW:10;          /*   GROPEW     */
                    } BIT;                      /*              */
             } GROPEWH1;                        /*              */
       union {                                  /* GROPEDPHV1   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPEDPV:10;        /*   GROPEDPV   */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPEDPH:10;        /*   GROPEDPH   */
                    } BIT;                      /*              */
             } GROPEDPHV1;                      /*              */
       union {                                  /* GROPEDPA1    */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD DEFA:8;             /*   DEFA       */
                    _UDWORD ACOEF:8;            /*   ACOEF      */
                    _UDWORD ARATE:8;            /*   ARATE      */
                    _UDWORD WE:1;               /*   WE         */
                    _UDWORD :2;                 /*              */
                    _UDWORD AST:1;              /*   AST        */
                    _UDWORD :1;                 /*              */
                    _UDWORD AMOD:2;             /*   AMOD       */
                    _UDWORD AEN:1;              /*   AEN        */
                    } BIT;                      /*              */
             } GROPEDPA1;                       /*              */
       union {                                  /* GROPCRKY0_1  */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :15;                /*              */
                    _UDWORD CKEN:1;             /*   CKEN       */
                    _UDWORD CROMAKR:5;          /*   CROMAKR    */
                    _UDWORD CROMAKG:6;          /*   CROMAKG    */
                    _UDWORD CROMAKB:5;          /*   CROMAKB    */
                    } BIT;                      /*              */
             } GROPCRKY0_1;                     /*              */
       union {                                  /* GROPCRKY1_1  */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :8;                 /*              */
                    _UDWORD ALPHA:8;            /*   ALPHA      */
                    _UDWORD R:5;                /*   R          */
                    _UDWORD G:6;                /*   G          */
                    _UDWORD B:5;                /*   B          */
                    } BIT;                      /*              */
             } GROPCRKY1_1;                     /*              */
       union {                                  /* GROPBASERGB1 */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :16;                /*              */
                    _UDWORD BASE_R:5;           /*   BASE_R     */
                    _UDWORD BASE_G:6;           /*   BASE_G     */
                    _UDWORD BASE_B:5;           /*   BASE_B     */
                    } BIT;                      /*              */
             } GROPBASERGB1;                    /*              */
       _UBYTE wk3[1232];                        /*              */
       union {                                  /* GRCMEN2      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD WE:1;               /*   WE         */
                    _UDWORD :29;                /*              */
                    _UDWORD DEN:1;              /*   DEN        */
                    _UDWORD VEN:1;              /*   VEN        */
                    } BIT;                      /*              */
             } GRCMEN2;                         /*              */
       union {                                  /* GRCBUSCNT2   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :23;                /*              */
                    _UDWORD BURST_MODE:1;       /*   BURST_MODE */
                    _UDWORD :3;                 /*              */
                    _UDWORD BUS_FORMAT:1;       /*   BUS_FORMAT */
                    _UDWORD :3;                 /*              */
                    _UDWORD ENDIAN:1;           /*   ENDIAN     */
                    } BIT;                      /*              */
             } GRCBUSCNT2;                      /*              */
       union {                                  /* GRCINTCNT2   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :15;                /*              */
                    _UDWORD INT_UF_EN:1;        /*   INT_UF_EN  */
                    _UDWORD :15;                /*              */
                    _UDWORD UNDER_FLOW:1;       /*   UNDER_FLOW */
                    } BIT;                      /*              */
             } GRCINTCNT2;                      /*              */
       _UBYTE wk4[764];                         /*              */
       union {                                  /* GROPSADR2    */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :3;                 /*              */
                    _UDWORD GROPSADR:29;        /*   GROPSADR   */
                    } BIT;                      /*              */
             } GROPSADR2;                       /*              */
       union {                                  /* GROPSWH2     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPSH:10;          /*   GROPSH     */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPSW:10;          /*   GROPSW     */
                    } BIT;                      /*              */
             } GROPSWH2;                        /*              */
       union {                                  /* GROPSOFST2   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :3;                 /*              */
                    _UDWORD GROPSOFST:29;       /*   GROPSOFST  */
                    } BIT;                      /*              */
             } GROPSOFST2;                      /*              */
       union {                                  /* GROPDPHV2    */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPDPV:10;         /*   GROPDPV    */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPDPH:10;         /*   GROPDPH    */
                    } BIT;                      /*              */
             } GROPDPHV2;                       /*              */
       union {                                  /* GROPEWH2     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPEH:10;          /*   GROPEH     */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPEW:10;          /*   GROPEW     */
                    } BIT;                      /*              */
             } GROPEWH2;                        /*              */
       union {                                  /* GROPEDPHV2   */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPEDPV:10;        /*   GROPEDPV   */
                    _UDWORD :6;                 /*              */
                    _UDWORD GROPEDPH:10;        /*   GROPEDPH   */
                    } BIT;                      /*              */
             } GROPEDPHV2;                      /*              */
       union {                                  /* GROPEDPA2    */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD DEFA:8;             /*   DEFA       */
                    _UDWORD ACOEF:8;            /*   ACOEF      */
                    _UDWORD ARATE:8;            /*   ARATE      */
                    _UDWORD WE:1;               /*   WE         */
                    _UDWORD :2;                 /*              */
                    _UDWORD AST:1;              /*   AST        */
                    _UDWORD :1;                 /*              */
                    _UDWORD AMOD:2;             /*   AMOD       */
                    _UDWORD AEN:1;              /*   AEN        */
                    } BIT;                      /*              */
             } GROPEDPA2;                       /*              */
       union {                                  /* GROPCRKY0_2  */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :15;                /*              */
                    _UDWORD CKEN:1;             /*   CKEN       */
                    _UDWORD CROMAKR:5;          /*   CROMAKR    */
                    _UDWORD CROMAKG:6;          /*   CROMAKG    */
                    _UDWORD CROMAKB:5;          /*   CROMAKB    */
                    } BIT;                      /*              */
             } GROPCRKY0_2;                     /*              */
       union {                                  /* GROPCRKY1_2  */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :8;                 /*              */
                    _UDWORD ALPHA:8;            /*   ALPHA      */
                    _UDWORD R:5;                /*   R          */
                    _UDWORD G:6;                /*   G          */
                    _UDWORD B:5;                /*   B          */
                    } BIT;                      /*              */
             } GROPCRKY1_2;                     /*              */
       union {                                  /* GROPBASERGB2 */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :16;                /*              */
                    _UDWORD BASE_R:5;           /*   BASE_R     */
                    _UDWORD BASE_G:6;           /*   BASE_G     */
                    _UDWORD BASE_B:5;           /*   BASE_B     */
                    } BIT;                      /*              */
             } GROPBASERGB2;                    /*              */
       _UBYTE wk5[1232];                        /*              */
       union {                                  /* SGMODE       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :15;                /*              */
                    _UDWORD EX_SYNC_MODE:1;     /*   EX_SYNC_MODE */
                    _UDWORD :15;                /*              */
                    _UDWORD RGB565:1;           /*   RGB565     */
                    } BIT;                      /*              */
             } SGMODE;                          /*              */
       union {                                  /* SGINTCNT     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :2;                 /*              */
                    _UDWORD INT_LINE_NUM:10;    /*   INT_LINE_NUM */
                    _UDWORD :3;                 /*              */
                    _UDWORD INT_LINE_EN:1;      /*   INT_LINE_EN */
                    _UDWORD :15;                /*              */
                    _UDWORD LINE_STATUS:1;      /*   LINE_STATUS */
                    } BIT;                      /*              */
             } SGINTCNT;                        /*              */
       union {                                  /* SYNCNT       */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :7;                 /*              */
                    _UDWORD RGB_TIM:1;          /*   RGB_TIM    */
                    _UDWORD :4;                 /*              */
                    _UDWORD VSYNC_TIM:1;        /*   VSYNC_TIM  */
                    _UDWORD HSYNC_TIM:1;        /*   HSYNC_TIM  */
                    _UDWORD DE_TIM:1;           /*   DE_TIM     */
                    _UDWORD M_DISP_TIM:1;       /*   M_DISP_TIM */
                    _UDWORD :12;                /*              */
                    _UDWORD VSYNC_TYPE:1;       /*   VSYNC_TYPE */
                    _UDWORD HSYNC_TYPE:1;       /*   HSYNC_TYPE */
                    _UDWORD DE_TYPE:1;          /*   DE_TYPE    */
                    _UDWORD M_DISP_TYPE:1;      /*   M_DISP_TYPE */
                    } BIT;                      /*              */
             } SYNCNT;                          /*              */
       union {                                  /* PANEL_CLKSEL */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :18;                /*              */
                    _UDWORD ICKSEL:1;           /*   ICKSEL     */
                    _UDWORD ICKEN:1;            /*   ICKEN      */
                    _UDWORD :6;                 /*              */
                    _UDWORD DCDR:6;             /*   DCDR       */
                    } BIT;                      /*              */
             } PANEL_CLKSEL;                    /*              */
       _UBYTE wk6[240];                         /*              */
       union {                                  /* SYN_SIZE     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD SYN_HEIGHT:10;      /*   SYN_HEIGHT */
                    _UDWORD :5;                 /*              */
                    _UDWORD SYN_WIDTH:11;       /*   SYN_WIDTH  */
                    } BIT;                      /*              */
             } SYN_SIZE;                        /*              */
       union {                                  /* PANEL_VSYNC_TIM */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD VSYNC_START:10;     /*   VSYNC_START */
                    _UDWORD :6;                 /*              */
                    _UDWORD VSYNC_END:10;       /*   VSYNC_END  */
                    } BIT;                      /*              */
             } PANEL_VSYNC_TIM;                 /*              */
       union {                                  /* PANEL_HSYNC_TIM */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :5;                 /*              */
                    _UDWORD HSYNC_START:11;     /*   HSYNC_START */
                    _UDWORD :5;                 /*              */
                    _UDWORD HSYNC_END:11;       /*   HSYNC_END  */
                    } BIT;                      /*              */
             } PANEL_HSYNC_TIM;                 /*              */
       union {                                  /* VIDEO_VSYNC_TIM2 */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :22;                /*              */
                    _UDWORD VIDEO_VSYNC_START2:10;/*   VIDEO_VSYNC_START2 */
                    } BIT;                      /*              */
             } VIDEO_VSYNC_TIM2;                /*              */
       union {                                  /* GRA_VSYNC_TIM */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :22;                /*              */
                    _UDWORD GRA_VSYNC_START:10; /*   GRA_VSYNC_START */
                    } BIT;                      /*              */
             } GRA_VSYNC_TIM;                   /*              */
       union {                                  /* AC_LINE_NUM  */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :27;                /*              */
                    _UDWORD AC_LINE_NUM:5;      /*   AC_LINE_NUM */
                    } BIT;                      /*              */
             } AC_LINE_NUM;                     /*              */
       _UBYTE wk7[8];                           /*              */
       union {                                  /* DE_SIZE      */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD DE_HEIGHT:10;       /*   DE_HEIGHT  */
                    _UDWORD :5;                 /*              */
                    _UDWORD DE_WIDTH:11;        /*   DE_WIDTH   */
                    } BIT;                      /*              */
             } DE_SIZE;                         /*              */
       union {                                  /* DE_START     */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    _UBYTE HH;                  /*   High, High */
                    _UBYTE HL;                  /*   High, Low  */
                    _UBYTE LH;                  /*   Low, High  */
                    _UBYTE LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UDWORD :6;                 /*              */
                    _UDWORD DE_START_V:10;      /*   DE_START_V */
                    _UDWORD :5;                 /*              */
                    _UDWORD DE_START_H:11;      /*   DE_START_H */
                    } BIT;                      /*              */
             } DE_START;                        /*              */
};
struct st_src0 {                                /* struct SRC0  */
       union {                                  /* SRCID        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             } SRCID;                           /*              */
       union {                                  /* SRCOD        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             } SRCOD;                           /*              */
       union {                                  /* SRCIDCTRL    */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :6;                  /*              */
                    _UWORD IED:1;               /*   IED        */
                    _UWORD IEN:1;               /*   IEN        */
                    _UWORD :6;                  /*              */
                    _UWORD IFTRG:2;             /*   IFTRG      */
                    } BIT;                      /*              */
             } SRCIDCTRL;                       /*              */
       union {                                  /* SRCODCTRL    */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :5;                  /*              */
                    _UWORD OCH:1;               /*   OCH        */
                    _UWORD OED:1;               /*   OED        */
                    _UWORD OEN:1;               /*   OEN        */
                    _UWORD :6;                  /*              */
                    _UWORD OFTRG:2;             /*   OFTRG      */
                    } BIT;                      /*              */
             } SRCODCTRL;                       /*              */
       union {                                  /* SRCCTRL      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :2;                  /*              */
                    _UWORD CEEN:1;              /*   CEEN       */
                    _UWORD SRCEN:1;             /*   SRCEN      */
                    _UWORD UDEN:1;              /*   UDEN       */
                    _UWORD OVEN:1;              /*   OVEN       */
                    _UWORD FL:1;                /*   FL         */
                    _UWORD CL:1;                /*   CL         */
                    _UWORD IFS:4;               /*   IFS        */
                    _UWORD :3;                  /*              */
                    _UWORD OFS:1;               /*   OFS        */
                    } BIT;                      /*              */
             } SRCCTRL;                         /*              */
       union {                                  /* SRCSTAT      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD OFDN:5;              /*   OFDN       */
                    _UWORD IFDN:4;              /*   IFDN       */
                    _UWORD :1;                  /*              */
                    _UWORD CEF:1;               /*   CEF        */
                    _UWORD FLF:1;               /*   FLF        */
                    _UWORD UDF:1;               /*   UDF        */
                    _UWORD OVF:1;               /*   OVF        */
                    _UWORD IINT:1;              /*   IINT       */
                    _UWORD OINT:1;              /*   OINT       */
                    } BIT;                      /*              */
             } SRCSTAT;                         /*              */
};                                              /*              */
struct st_src1 {                                /* struct SRC1  */
       union {                                  /* SRCID        */
             _UWORD WORD;                       /*  Word Access */
             } SRCID;                           /*              */
       _UBYTE wk0[2];                           /*              */
       union {                                  /* SRCOD        */
             _UDWORD LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    _UWORD H;                   /*   High       */
                    _UWORD L;                   /*   Low        */
                    } WORD;                     /*              */
             } SRCOD;                           /*              */
       union {                                  /* SRCIDCTRL    */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :6;                  /*              */
                    _UWORD IED:1;               /*   IED        */
                    _UWORD IEN:1;               /*   IEN        */
                    _UWORD :6;                  /*              */
                    _UWORD IFTRG:2;             /*   IFTRG      */
                    } BIT;                      /*              */
             } SRCIDCTRL;                       /*              */
       union {                                  /* SRCODCTRL    */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :5;                  /*              */
                    _UWORD OCH:1;               /*   OCH        */
                    _UWORD OED:1;               /*   OED        */
                    _UWORD OEN:1;               /*   OEN        */
                    _UWORD :6;                  /*              */
                    _UWORD OFTRG:2;             /*   OFTRG      */
                    } BIT;                      /*              */
             } SRCODCTRL;                       /*              */
       union {                                  /* SRCCTRL      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :2;                  /*              */
                    _UWORD CEEN:1;              /*   CEEN       */
                    _UWORD SRCEN:1;             /*   SRCEN      */
                    _UWORD UDEN:1;              /*   UDEN       */
                    _UWORD OVEN:1;              /*   OVEN       */
                    _UWORD FL:1;                /*   FL         */
                    _UWORD CL:1;                /*   CL         */
                    _UWORD :7;                  /*              */
                    _UWORD OFS:1;               /*   OFS        */
                    } BIT;                      /*              */
             } SRCCTRL;                         /*              */
       union {                                  /* SRCSTAT      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :2;                  /*              */
                    _UWORD OFDN:3;              /*   OFDN       */
                    _UWORD IFDN:4;              /*   IFDN       */
                    _UWORD :1;                  /*              */
                    _UWORD CEF:1;               /*   CEF        */
                    _UWORD FLF:1;               /*   FLF        */
                    _UWORD UDF:1;               /*   UDF        */
                    _UWORD OVF:1;               /*   OVF        */
                    _UWORD IINT:1;              /*   IINT       */
                    _UWORD OINT:1;              /*   OINT       */
                    } BIT;                      /*              */
             } SRCSTAT;                         /*              */
};                                              /*              */
struct st_dcu {                                 /* struct DCU   */
       union {                                  /* DCUCR        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE DCUE:1;              /*   DCUE       */
                    _UBYTE DCUEEN:1;            /*   DCUEEN     */
                    _UBYTE IFEN:1;              /*   IFEN       */
                    _UBYTE OFEN:1;              /*   OFEN       */
                    _UBYTE IDTRG:2;             /*   IDTRG      */
                    _UBYTE ODTRG:2;             /*   ODTRG      */
                    } BIT;                      /*              */
             } DCUCR;                           /*              */
       union {                                  /* DCUISR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :3;                  /*              */
                    _UBYTE IDFE:1;              /*   IDFE       */
                    _UBYTE IFOVF:1;             /*   IFOVF      */
                    _UBYTE OFUDF:1;             /*   OFUDF      */
                    _UBYTE IFEMP:1;             /*   IFEMP      */
                    _UBYTE OFFUL:1;             /*   OFFUL      */
                    } BIT;                      /*              */
             } DCUISR;                          /*              */
       _UBYTE wk0[2];                           /*              */
       union {                                  /* DCUID        */
             _UDWORD LONG;                      /*  Long Access */
             _UWORD WORD;                       /*  Word Access */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UDWORD ID:32;              /*   ID         */
                    } BIT;                      /*              */
             } DCUID;                           /*              */
       union {                                  /* DCUOD        */
             _UDWORD LONG;                      /*  Long Access */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UDWORD OD:32;              /*   OD         */
                    } BIT;                      /*              */
             } DCUOD;                           /*              */
       union {                                  /* DCUIFSR      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :3;                  /*              */
                    _UBYTE IFVD:5;              /*   IFVD       */
                    } BIT;                      /*              */
             } DCUIFSR;                         /*              */
       union {                                  /* DCUOFSR      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE OFVD:6;              /*   OFVD       */
                    } BIT;                      /*              */
             } DCUOFSR;                         /*              */
       union {                                  /* DCURGBC      */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :6;                  /*              */
                    _UBYTE RGBC:2;              /*   RGBC       */
                    } BIT;                      /*              */
             } DCURGBC;                         /*              */
};                                              /*              */
struct st_gpio {                                /* struct GPIO  */
       union {                                  /* PAIOR0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :8;                  /*              */
                    _UBYTE :4;                  /*              */
                    _UBYTE PA3IOR:1;            /*   PA3IOR     */
                    _UBYTE PA2IOR:1;            /*   PA2IOR     */
                    _UBYTE PA1IOR:1;            /*   PA1IOR     */
                    _UBYTE PA0IOR:1;            /*   PA0IOR     */
                    } BIT;                      /*              */
             } PAIOR0;                          /*              */
       union {                                  /* PADR1        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :7;                  /*              */
                    _UBYTE PA3DR:1;             /*   PA3DR      */
                    _UBYTE :7;                  /*              */
                    _UBYTE PA2DR:1;             /*   PA2DR      */
                    } BIT;                      /*              */
             } PADR1;                           /*              */
       union {                                  /* PADR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :7;                  /*              */
                    _UBYTE PA1DR:1;             /*   PA1DR      */
                    _UBYTE :7;                  /*              */
                    _UBYTE PA0DR:1;             /*   PA0DR      */
                    } BIT;                      /*              */
             } PADR0;                           /*              */
       _UBYTE wk0[2];                           /*              */
       union {                                  /* PAPR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :8;                  /*              */
                    _UBYTE :4;                  /*              */
                    _UBYTE PA3PR:1;             /*   PA3PR      */
                    _UBYTE PA2PR:1;             /*   PA2PR      */
                    _UBYTE PA1PR:1;             /*   PA1PR      */
                    _UBYTE PA0PR:1;             /*   PA0PR      */
                    } BIT;                      /*              */
             } PAPR0;                           /*              */
       _UBYTE wk1[8];                           /*              */
       union {                                  /* PBCR5        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :6;                  /*              */
                    _UBYTE PB22MD:2 ;           /*   PB22MD     */
                    _UBYTE :3;                  /*              */
                    _UBYTE PB21MD:1;            /*   PB21MD     */
                    _UBYTE :3;                  /*              */
                    _UBYTE PB20MD:1;            /*   PB20MD     */
                    } BIT;                      /*              */
             } PBCR5;                           /*              */
       union {                                  /* PBCR4        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB19MD:2;            /*   PB19MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB18MD:2;            /*   PB18MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB17MD:2;            /*   PB17MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB16MD:2;            /*   PB16MD     */
                    } BIT;                      /*              */
             } PBCR4;                           /*              */
       union {                                  /* PBCR3        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB15MD:2;            /*   PB15MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB14MD:2;            /*   PB14MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB13MD:2;            /*   PB13MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB12MD:2;            /*   PB12MD     */
                    } BIT;                      /*              */
             } PBCR3;                           /*              */
       union {                                  /* PBCR2        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB11MD:2;            /*   PB11MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB10MD:2;            /*   PB10MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB9MD:2;             /*   PB9MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB8MD:2;             /*   PB8MD      */
                    } BIT;                      /*              */
             } PBCR2;                           /*              */
       union {                                  /* PBCR1        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB7MD:2;             /*   PB7MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB6MD:2;             /*   PB6MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB5MD:2;             /*   PB5MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PB4MD:2;             /*   PB4MD      */
                    } BIT;                      /*              */
             } PBCR1;                           /*              */
       union {                                  /* PBCR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :3;                  /*              */
                    _UBYTE PB3MD:1;             /*   PB3MD      */
                    _UBYTE :3;                  /*              */
                    _UBYTE PB2MD:1;             /*   PB2MD      */
                    _UBYTE :3;                  /*              */
                    _UBYTE PB1MD:1;             /*   PB1MD      */
                    _UBYTE :4;                  /*              */
                    } BIT;                      /*              */
             } PBCR0;                           /*              */
       union {                                  /* PBIOR1       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :8;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE PB22IOR:1;           /*   PB22IOR    */
                    _UBYTE PB21IOR:1;           /*   PB21IOR    */
                    _UBYTE PB20IOR:1;           /*   PB20IOR    */
                    _UBYTE PB19IOR:1;           /*   PB19IOR    */
                    _UBYTE PB18IOR:1;           /*   PB18IOR    */
                    _UBYTE PB17IOR:1;           /*   PB17IOR    */
                    _UBYTE PB16IOR:1;           /*   PB16IOR    */
                    } BIT;                      /*              */
             } PBIOR1;                          /*              */
       union {                                  /* PBIOR0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE PB15IOR:1;           /*   PB15IOR    */
                    _UBYTE PB14IOR:1;           /*   PB14IOR    */
                    _UBYTE PB13IOR:1;           /*   PB13IOR    */
                    _UBYTE PB12IOR:1;           /*   PB12IOR    */
                    _UBYTE PB11IOR:1;           /*   PB11IOR    */
                    _UBYTE PB10IOR:1;           /*   PB10IOR    */
                    _UBYTE PB9IOR:1;            /*   PB9IOR     */
                    _UBYTE PB8IOR:1;            /*   PB8IOR     */
                    _UBYTE PB7IOR:1;            /*   PB7IOR     */
                    _UBYTE PB6IOR:1;            /*   PB6IOR     */
                    _UBYTE PB5IOR:1;            /*   PB5IOR     */
                    _UBYTE PB4IOR:1;            /*   PB4IOR     */
                    _UBYTE PB3IOR:1;            /*   PB3IOR     */
                    _UBYTE PB2IOR:1;            /*   PB2IOR     */
                    _UBYTE PB1IOR:1;            /*   PB1IOR     */
                    _UBYTE :1;                  /*              */
                    } BIT;                      /*              */
             } PBIOR0;                          /*              */
       union {                                  /* PBDR1        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :8;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE PB22DR:1;            /*   PB22DR     */
                    _UBYTE PB21DR:1;            /*   PB21DR     */
                    _UBYTE PB20DR:1;            /*   PB20DR     */
                    _UBYTE PB19DR:1;            /*   PB19DR     */
                    _UBYTE PB18DR:1;            /*   PB18DR     */
                    _UBYTE PB17DR:1;            /*   PB17DR     */
                    _UBYTE PB16DR:1;            /*   PB16DR     */
                    } BIT;                      /*              */
             } PBDR1;                           /*              */
       union {                                  /* PBDR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE PB15DR:1;            /*   PB15DR     */
                    _UBYTE PB14DR:1;            /*   PB14DR     */
                    _UBYTE PB13DR:1;            /*   PB13DR     */
                    _UBYTE PB12DR:1;            /*   PB12DR     */
                    _UBYTE PB11DR:1;            /*   PB11DR     */
                    _UBYTE PB10DR:1;            /*   PB10DR     */
                    _UBYTE PB9DR:1;             /*   PB9DR      */
                    _UBYTE PB8DR:1;             /*   PB8DR      */
                    _UBYTE PB7DR:1;             /*   PB7DR      */
                    _UBYTE PB6DR:1;             /*   PB6DR      */
                    _UBYTE PB5DR:1;             /*   PB5DR      */
                    _UBYTE PB4DR:1;             /*   PB4DR      */
                    _UBYTE PB3DR:1;             /*   PB3DR      */
                    _UBYTE PB2DR:1;             /*   PB2DR      */
                    _UBYTE PB1DR:1;             /*   PB1DR      */
                    _UBYTE :1;                  /*              */
                    } BIT;                      /*              */
             } PBDR0;                           /*              */
       union {                                  /* PBPR1        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :8;                  /*              */
                    _UBYTE :1;                  /*              */
                    _UBYTE PB22PR:1;            /*   PB22PR     */
                    _UBYTE PB21PR:1;            /*   PB21PR     */
                    _UBYTE PB20PR:1;            /*   PB20PR     */
                    _UBYTE PB19PR:1;            /*   PB19PR     */
                    _UBYTE PB18PR:1;            /*   PB18PR     */
                    _UBYTE PB17PR:1;            /*   PB17PR     */
                    _UBYTE PB16PR:1;            /*   PB16PR     */
                    } BIT;                      /*              */
             } PBPR1;                           /*              */
       union {                                  /* PBPR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE PB15PR:1;            /*   PB15PR     */
                    _UBYTE PB14PR:1;            /*   PB14PR     */
                    _UBYTE PB13PR:1;            /*   PB13PR     */
                    _UBYTE PB12PR:1;            /*   PB12PR     */
                    _UBYTE PB11PR:1;            /*   PB11PR     */
                    _UBYTE PB10PR:1;            /*   PB10PR     */
                    _UBYTE PB9PR:1;             /*   PB9PR      */
                    _UBYTE PB8PR:1;             /*   PB8PR      */
                    _UBYTE PB7PR:1;             /*   PB7PR      */
                    _UBYTE PB6PR:1;             /*   PB6PR      */
                    _UBYTE PB5PR:1;             /*   PB5PR      */
                    _UBYTE PB4PR:1;             /*   PB4PR      */
                    _UBYTE PB3PR:1;             /*   PB3PR      */
                    _UBYTE PB2PR:1;             /*   PB2PR      */
                    _UBYTE PB1PR:1;             /*   PB1PR      */
                    _UBYTE :1;                  /*              */
                    } BIT;                      /*              */
             } PBPR0;                           /*              */
       _UBYTE wk2[14];                          /*              */
       union {                                  /* PCCR2        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :7;                  /*              */
                    _UBYTE PC10MD:1;            /*   PC10MD     */
                    _UBYTE :3;                  /*              */
                    _UBYTE PC9MD:1;             /*   PC9MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PC8MD:2;             /*   PC8MD      */
                    } BIT;                      /*              */
             } PCCR2;                           /*              */
       union {                                  /* PCCR1        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PC7MD:2;             /*   PC7MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PC6MD:2;             /*   PC6MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PC5MD:2;             /*   PC5MD      */
                    _UBYTE :3;                  /*              */
                    _UBYTE PC4MD:1;             /*   PC4MD      */
                    } BIT;                      /*              */
             } PCCR1;                           /*              */
       union {                                  /* PCCR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :3;                  /*              */
                    _UBYTE PC3MD:1;             /*   PC3MD      */
                    _UBYTE :3;                  /*              */
                    _UBYTE PC2MD:1;             /*   PC2MD      */
                    _UBYTE :3;                  /*              */
                    _UBYTE PC1MD:1;             /*   PC1MD      */
                    _UBYTE :3;                  /*              */
                    _UBYTE PC0MD:1;             /*   PC0MD      */
                    } BIT;                      /*              */
             } PCCR0;                           /*              */
       _UBYTE wk3[2];                           /*              */
       union {                                  /* PCIOR0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :5;                  /*              */
                    _UBYTE PC10IOR:1;           /*   PC10IOR    */
                    _UBYTE PC9IOR:1;            /*   PC9IOR     */
                    _UBYTE PC8IOR:1;            /*   PC8IOR     */
                    _UBYTE PC7IOR:1;            /*   PC7IOR     */
                    _UBYTE PC6IOR:1;            /*   PC6IOR     */
                    _UBYTE PC5IOR:1;            /*   PC5IOR     */
                    _UBYTE PC4IOR:1;            /*   PC4IOR     */
                    _UBYTE PC3IOR:1;            /*   PC3IOR     */
                    _UBYTE PC2IOR:1;            /*   PC2IOR     */
                    _UBYTE PC1IOR:1;            /*   PC1IOR     */
                    _UBYTE PC0IOR:1;            /*   PC0IOR     */
                    } BIT;                      /*              */
             } PCIOR0;                          /*              */
       _UBYTE wk4[2];                           /*              */
       union {                                  /* PCDR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :5;                  /*              */
                    _UBYTE PC10DR:1;            /*   PC10DR     */
                    _UBYTE PC9DR:1;             /*   PC9DR      */
                    _UBYTE PC8DR:1;             /*   PC8DR      */
                    _UBYTE PC7DR:1;             /*   PC7DR      */
                    _UBYTE PC6DR:1;             /*   PC6DR      */
                    _UBYTE PC5DR:1;             /*   PC5DR      */
                    _UBYTE PC4DR:1;             /*   PC4DR      */
                    _UBYTE PC3DR:1;             /*   PC3DR      */
                    _UBYTE PC2DR:1;             /*   PC2DR      */
                    _UBYTE PC1DR:1;             /*   PC1DR      */
                    _UBYTE PC0DR:1;             /*   PC0DR      */
                    } BIT;                      /*              */
             } PCDR0;                           /*              */
       _UBYTE wk5[2];                           /*              */
       union {                                  /* PCPR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :5;                  /*              */
                    _UBYTE PC10PR:1;            /*   PC10PR     */
                    _UBYTE PC9PR:1;             /*   PC9PR      */
                    _UBYTE PC8PR:1;             /*   PC8PR      */
                    _UBYTE PC7PR:1;             /*   PC7PR      */
                    _UBYTE PC6PR:1;             /*   PC6PR      */
                    _UBYTE PC5PR:1;             /*   PC5PR      */
                    _UBYTE PC4PR:1;             /*   PC4PR      */
                    _UBYTE PC3PR:1;             /*   PC3PR      */
                    _UBYTE PC2PR:1;             /*   PC2PR      */
                    _UBYTE PC1PR:1;             /*   PC1PR      */
                    _UBYTE PC0PR:1;             /*   PC0PR      */
                    } BIT;                      /*              */
             } PCPR0;                           /*              */
       _UBYTE wk6[12];                          /*              */
       union {                                  /* PDCR3        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD15MD:2;            /*   PD15MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD14MD:2;            /*   PD14MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD13MD:2;            /*   PD13MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD12MD:2;            /*   PD12MD     */
                    } BIT;                      /*              */
             } PDCR3;                           /*              */
       union {                                  /* PDCR2        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD11MD:2;            /*   PD11MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD10MD:2;            /*   PD10MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD9MD:2;             /*   PD9MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD8MD:2;             /*   PD8MD      */
                    } BIT;                      /*              */
             } PDCR2;                           /*              */
       union {                                  /* PDCR1        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD7MD:2;             /*   PD7MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD6MD:2;             /*   PD6MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD5MD:2;             /*   PD5MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD4MD:2;             /*   PD4MD      */
                    } BIT;                      /*              */
             } PDCR1;                           /*              */
       union {                                  /* PDCR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD3MD:2;             /*   PD3MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD2MD:2;             /*   PD2MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD1MD:2;             /*   PD1MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PD0MD:2;             /*   PD0MD      */
                    } BIT;                      /*              */
             } PDCR0;                           /*              */
       _UBYTE wk7[2];                           /*              */
       union {                                  /* PDIOR0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE PD15IOR:1;           /*   PD15IOR    */
                    _UBYTE PD14IOR:1;           /*   PD14IOR    */
                    _UBYTE PD13IOR:1;           /*   PD13IOR    */
                    _UBYTE PD12IOR:1;           /*   PD12IOR    */
                    _UBYTE PD11IOR:1;           /*   PD11IOR    */
                    _UBYTE PD10IOR:1;           /*   PD10IOR    */
                    _UBYTE PD9IOR:1;            /*   PD9IOR     */
                    _UBYTE PD8IOR:1;            /*   PD8IOR     */
                    _UBYTE PD7IOR:1;            /*   PD7IOR     */
                    _UBYTE PD6IOR:1;            /*   PD6IOR     */
                    _UBYTE PD5IOR:1;            /*   PD5IOR     */
                    _UBYTE PD4IOR:1;            /*   PD4IOR     */
                    _UBYTE PD3IOR:1;            /*   PD3IOR     */
                    _UBYTE PD2IOR:1;            /*   PD2IOR     */
                    _UBYTE PD1IOR:1;            /*   PD1IOR     */
                    _UBYTE PD0IOR:1;            /*   PD0IOR     */
                    } BIT;                      /*              */
             } PDIOR0;                          /*              */
       _UBYTE wk8[2];                           /*              */
       union {                                  /* PDDR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE PD15DR:1;            /*   PD15DR     */
                    _UBYTE PD14DR:1;            /*   PD14DR     */
                    _UBYTE PD13DR:1;            /*   PD13DR     */
                    _UBYTE PD12DR:1;            /*   PD12DR     */
                    _UBYTE PD11DR:1;            /*   PD11DR     */
                    _UBYTE PD10DR:1;            /*   PD10DR     */
                    _UBYTE PD9DR:1;             /*   PD9DR      */
                    _UBYTE PD8DR:1;             /*   PD8DR      */
                    _UBYTE PD7DR:1;             /*   PD7DR      */
                    _UBYTE PD6DR:1;             /*   PD6DR      */
                    _UBYTE PD5DR:1;             /*   PD5DR      */
                    _UBYTE PD4DR:1;             /*   PD4DR      */
                    _UBYTE PD3DR:1;             /*   PD3DR      */
                    _UBYTE PD2DR:1;             /*   PD2DR      */
                    _UBYTE PD1DR:1;             /*   PD1DR      */
                    _UBYTE PD0DR:1;             /*   PD0DR      */
                    } BIT;                      /*              */
             } PDDR0;                           /*              */
       _UBYTE wk9[2];                           /*              */
       union {                                  /* PDPR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE PD15PR:1;            /*   PD15PR     */
                    _UBYTE PD14PR:1;            /*   PD14PR     */
                    _UBYTE PD13PR:1;            /*   PD13PR     */
                    _UBYTE PD12PR:1;            /*   PD12PR     */
                    _UBYTE PD11PR:1;            /*   PD11PR     */
                    _UBYTE PD10PR:1;            /*   PD10PR     */
                    _UBYTE PD9PR:1;             /*   PD9PR      */
                    _UBYTE PD8PR:1;             /*   PD8PR      */
                    _UBYTE PD7PR:1;             /*   PD7PR      */
                    _UBYTE PD6PR:1;             /*   PD6PR      */
                    _UBYTE PD5PR:1;             /*   PD5PR      */
                    _UBYTE PD4PR:1;             /*   PD4PR      */
                    _UBYTE PD3PR:1;             /*   PD3PR      */
                    _UBYTE PD2PR:1;             /*   PD2PR      */
                    _UBYTE PD1PR:1;             /*   PD1PR      */
                    _UBYTE PD0PR:1;             /*   PD0PR      */
                    } BIT;                      /*              */
             } PDPR0;                           /*              */
       _UBYTE wk10[16];                         /*              */
       union {                                  /* PECR1        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :8;                  /*              */
                    _UBYTE :2;                  /*              */
                    _UBYTE PE5MD:2;             /*   PE5MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PE4MD:2;             /*   PE4MD      */
                    } BIT;                      /*              */
             } PECR1;                           /*              */
       union {                                  /* PECR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PE3MD:2;             /*   PE3MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PE2MD:2;             /*   PE2MD      */
                    _UBYTE :1;                  /*              */
                    _UBYTE PE1MD:3;             /*   PE1MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PE0MD:2;             /*   PE0MD      */
                    } BIT;                      /*              */
             } PECR0;                           /*              */
       _UBYTE wk11[2];                          /*              */
       union {                                  /* PEIOR0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :8;                  /*              */
                    _UBYTE :2;                  /*              */
                    _UBYTE PE5IOR:1;            /*   PE5IOR     */
                    _UBYTE PE4IOR:1;            /*   PE4IOR     */
                    _UBYTE PE3IOR:1;            /*   PE3IOR     */
                    _UBYTE PE2IOR:1;            /*   PE2IOR     */
                    _UBYTE PE1IOR:1;            /*   PE1IOR     */
                    _UBYTE PE0IOR:1;            /*   PE0IOR     */
                    } BIT;                      /*              */
             } PEIOR0;                          /*              */
       _UBYTE wk12[2];                          /*              */
       union {                                  /* PEDR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :8;                  /*              */
                    _UBYTE :2;                  /*              */
                    _UBYTE PE5DR:1;             /*   PE5DR      */
                    _UBYTE PE4DR:1;             /*   PE4DR      */
                    _UBYTE PE3DR:1;             /*   PE3DR      */
                    _UBYTE PE2DR:1;             /*   PE2DR      */
                    _UBYTE PE1DR:1;             /*   PE1DR      */
                    _UBYTE PE0DR:1;             /*   PE0DR      */
                    } BIT;                      /*              */
             } PEDR0;                           /*              */
       _UBYTE wk13[2];                          /*              */
       union {                                  /* PEPR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :8;                  /*              */
                    _UBYTE :2;                  /*              */
                    _UBYTE PE5PR:1;             /*   PE5PR      */
                    _UBYTE PE4PR:1;             /*   PE4PR      */
                    _UBYTE PE3PR:1;             /*   PE3PR      */
                    _UBYTE PE2PR:1;             /*   PE2PR      */
                    _UBYTE PE1PR:1;             /*   PE1PR      */
                    _UBYTE PE0PR:1;             /*   PE0PR      */
                    } BIT;                      /*              */
             } PEPR0;                           /*              */
       _UBYTE wk14[12];                         /*              */
       union {                                  /* PFCR3        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :8;                  /*              */
                    _UBYTE :5;                  /*              */
                    _UBYTE PF12MD:3;            /*   PF12MD     */
                    } BIT;                      /*              */
             } PFCR3;                           /*              */
       union {                                  /* PFCR2        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE PF11MD:3;            /*   PF11MD     */
                    _UBYTE :1;                  /*              */
                    _UBYTE PF10MD:3;            /*   PF10MD     */
                    _UBYTE :1;                  /*              */
                    _UBYTE PF9MD:3;             /*   PF9MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PF8MD:2;             /*   PF8MD      */
                    } BIT;                      /*              */
             } PFCR2;                           /*              */
       union {                                  /* PFCR1        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE PF7MD:3;             /*   PF7MD      */
                    _UBYTE :1;                  /*              */
                    _UBYTE PF6MD:3;             /*   PF6MD      */
                    _UBYTE :1;                  /*              */
                    _UBYTE PF5MD:3;             /*   PF5MD      */
                    _UBYTE :1;                  /*              */
                    _UBYTE PF4MD:3;             /*   PF4MD      */
                    } BIT;                      /*              */
             } PFCR1;                           /*              */
       union {                                  /* PFCR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE PF3MD:3;             /*   PF3MD      */
                    _UBYTE :1;                  /*              */
                    _UBYTE PF2MD:3;             /*   PF2MD      */
                    _UBYTE :1;                  /*              */
                    _UBYTE PF1MD:3;             /*   PF1MD      */
                    _UBYTE :1;                  /*              */
                    _UBYTE PF0MD:3;             /*   PF0MD      */
                    } BIT;                      /*              */
             } PFCR0;                           /*              */
       _UBYTE wk15[2];                          /*              */
       union {                                  /* PFIOR0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :3;                  /*              */
                    _UBYTE PF12IOR:1;           /*   PF12IOR    */
                    _UBYTE PF11IOR:1;           /*   PF11IOR    */
                    _UBYTE PF10IOR:1;           /*   PF10IOR    */
                    _UBYTE PF9IOR:1;            /*   PF9IOR     */
                    _UBYTE PF8IOR:1;            /*   PF8IOR     */
                    _UBYTE PF7IOR:1;            /*   PF7IOR     */
                    _UBYTE PF6IOR:1;            /*   PF6IOR     */
                    _UBYTE PF5IOR:1;            /*   PF5IOR     */
                    _UBYTE PF4IOR:1;            /*   PF4IOR     */
                    _UBYTE PF3IOR:1;            /*   PF3IOR     */
                    _UBYTE PF2IOR:1;            /*   PF2IOR     */
                    _UBYTE PF1IOR:1;            /*   PF1IOR     */
                    _UBYTE PF0IOR:1;            /*   PF0IOR     */
                    } BIT;                      /*              */
             } PFIOR0;                          /*              */
       _UBYTE wk16[2];                          /*              */
       union {                                  /* PFDR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :3;                  /*              */
                    _UBYTE PF12DR:1;            /*   PF12DR     */
                    _UBYTE PF11DR:1;            /*   PF11DR     */
                    _UBYTE PF10DR:1;            /*   PF10DR     */
                    _UBYTE PF9DR:1;             /*   PF9DR      */
                    _UBYTE PF8DR:1;             /*   PF8DR      */
                    _UBYTE PF7DR:1;             /*   PF7DR      */
                    _UBYTE PF6DR:1;             /*   PF6DR      */
                    _UBYTE PF5DR:1;             /*   PF5DR      */
                    _UBYTE PF4DR:1;             /*   PF4DR      */
                    _UBYTE PF3DR:1;             /*   PF3DR      */
                    _UBYTE PF2DR:1;             /*   PF2DR      */
                    _UBYTE PF1DR:1;             /*   PF1DR      */
                    _UBYTE PF0DR:1;             /*   PF0DR      */
                    } BIT;                      /*              */
             } PFDR0;                           /*              */
       _UBYTE wk17[2];                          /*              */
       union {                                  /* PFPR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :3;                  /*              */
                    _UBYTE PF12PR:1;            /*   PF12PR     */
                    _UBYTE PF11PR:1;            /*   PF11PR     */
                    _UBYTE PF10PR:1;            /*   PF10PR     */
                    _UBYTE PF9PR:1;             /*   PF9PR      */
                    _UBYTE PF8PR:1;             /*   PF8PR      */
                    _UBYTE PF7PR:1;             /*   PF7PR      */
                    _UBYTE PF6PR:1;             /*   PF6PR      */
                    _UBYTE PF5PR:1;             /*   PF5PR      */
                    _UBYTE PF4PR:1;             /*   PF4PR      */
                    _UBYTE PF3PR:1;             /*   PF3PR      */
                    _UBYTE PF2PR:1;             /*   PF2PR      */
                    _UBYTE PF1PR:1;             /*   PF1PR      */
                    _UBYTE PF0PR:1;             /*   PF0PR      */
                    } BIT;                      /*              */
             } PFPR0;                           /*              */
       _UBYTE wk18[4];                          /*              */
       union {                                  /* PGCR7        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :8;                  /*              */
                    _UBYTE :5;                  /*              */
                    _UBYTE PG0MD:3;             /*   PG0MD      */
                    } BIT;                      /*              */
             } PGCR7;                           /*              */
       union {                                  /* PGCR6        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :8;                  /*              */
                    _UBYTE :6;                  /*              */
                    _UBYTE PG24MD:2;            /*   PG24MD     */
                    } BIT;                      /*              */
             } PGCR6;                           /*              */
       union {                                  /* PGCR5        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PG23MD:2;            /*   PG23MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PG22MD:2;            /*   PG22MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PG21MD:2;            /*   PG21MD     */
                    _UBYTE :1;                  /*              */
                    _UBYTE PG20MD:3;            /*   PG20MD     */
                    } BIT;                      /*              */
             } PGCR5;                           /*              */
       union {                                  /* PGCR4        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE PG19MD:3;            /*   PG19MD     */
                    _UBYTE :1;                  /*              */
                    _UBYTE PG18MD:3;            /*   PG18MD     */
                    _UBYTE :1;                  /*              */
                    _UBYTE PG17MD:3;            /*   PG17MD     */
                    _UBYTE :1;                  /*              */
                    _UBYTE PG16MD:3;            /*   PG16MD     */
                    } BIT;                      /*              */
             } PGCR4;                           /*              */
       union {                                  /* PGCR3        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE PG15MD:3;            /*   PG15MD     */
                    _UBYTE :1;                  /*              */
                    _UBYTE PG14MD:3;            /*   PG14MD     */
                    _UBYTE :1;                  /*              */
                    _UBYTE PG13MD:3;            /*   PG13MD     */
                    _UBYTE :1;                  /*              */
                    _UBYTE PG12MD:3;            /*   PG12MD     */
                    } BIT;                      /*              */
             } PGCR3;                           /*              */
       union {                                  /* PGCR2        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :1;                  /*              */
                    _UBYTE PG11MD:3;            /*   PG11MD     */
                    _UBYTE :1;                  /*              */
                    _UBYTE PG10MD:3;            /*   PG10MD     */
                    _UBYTE :1;                  /*              */
                    _UBYTE PG9MD:3;             /*   PG9MD      */
                    _UBYTE :1;                  /*              */
                    _UBYTE PG8MD:3;             /*   PG8MD      */
                    } BIT;                      /*              */
             } PGCR2;                           /*              */
       union {                                  /* PGCR1        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PG7MD:2;             /*   PG7MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PG6MD:2;             /*   PG6MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PG5MD:2;             /*   PG5MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PG4MD:2;             /*   PG4MD      */
                    } BIT;                      /*              */
             } PGCR1;                           /*              */
       union {                                  /* PGCR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PG3MD:2;             /*   PG3MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PG2MD:2;             /*   PG2MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PG1MD:2;             /*   PG1MD      */
                    _UBYTE :4;                  /*              */
                    } BIT;                      /*              */
             } PGCR0;                           /*              */
       union {                                  /* PGIOR1       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :7;                  /*              */
                    _UBYTE PG24IOR:1;           /*   PG24IOR    */
                    _UBYTE PG23IOR:1;           /*   PG23IOR    */
                    _UBYTE PG22IOR:1;           /*   PG22IOR    */
                    _UBYTE PG21IOR:1;           /*   PG21IOR    */
                    _UBYTE PG20IOR:1;           /*   PG20IOR    */
                    _UBYTE PG19IOR:1;           /*   PG19IOR    */
                    _UBYTE PG18IOR:1;           /*   PG18IOR    */
                    _UBYTE PG17IOR:1;           /*   PG17IOR    */
                    _UBYTE PG16IOR:1;           /*   PG16IOR    */
                    } BIT;                      /*              */
             } PGIOR1;                          /*              */
       union {                                  /* PGIOR0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE PG15IOR:1;           /*   PG15IOR    */
                    _UBYTE PG14IOR:1;           /*   PG14IOR    */
                    _UBYTE PG13IOR:1;           /*   PG13IOR    */
                    _UBYTE PG12IOR:1;           /*   PG12IOR    */
                    _UBYTE PG11IOR:1;           /*   PG11IOR    */
                    _UBYTE PG10IOR:1;           /*   PG10IOR    */
                    _UBYTE PG9IOR:1;            /*   PG9IOR     */
                    _UBYTE PG8IOR:1;            /*   PG8IOR     */
                    _UBYTE PG7IOR:1;            /*   PG7IOR     */
                    _UBYTE PG6IOR:1;            /*   PG6IOR     */
                    _UBYTE PG5IOR:1;            /*   PG5IOR     */
                    _UBYTE PG4IOR:1;            /*   PG4IOR     */
                    _UBYTE PG3IOR:1;            /*   PG3IOR     */
                    _UBYTE PG2IOR:1;            /*   PG2IOR     */
                    _UBYTE PG1IOR:1;            /*   PG1IOR     */
                    _UBYTE PG0IOR:1;            /*   PG0IOR     */
                    } BIT;                      /*              */
             } PGIOR0;                          /*              */
       union {                                  /* PGDR1        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :7;                  /*              */
                    _UBYTE PG24DR:1;            /*   PG24DR     */
                    _UBYTE PG23DR:1;            /*   PG23DR     */
                    _UBYTE PG22DR:1;            /*   PG22DR     */
                    _UBYTE PG21DR:1;            /*   PG21DR     */
                    _UBYTE PG20DR:1;            /*   PG20DR     */
                    _UBYTE PG19DR:1;            /*   PG19DR     */
                    _UBYTE PG18DR:1;            /*   PG18DR     */
                    _UBYTE PG17DR:1;            /*   PG17DR     */
                    _UBYTE PG16DR:1;            /*   PG16DR     */
                    } BIT;                      /*              */
             } PGDR1;                           /*              */
       union {                                  /* PGDR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE PG15DR:1;            /*   PG15DR     */
                    _UBYTE PG14DR:1;            /*   PG14DR     */
                    _UBYTE PG13DR:1;            /*   PG13DR     */
                    _UBYTE PG12DR:1;            /*   PG12DR     */
                    _UBYTE PG11DR:1;            /*   PG11DR     */
                    _UBYTE PG10DR:1;            /*   PG10DR     */
                    _UBYTE PG9DR:1;             /*   PG9DR      */
                    _UBYTE PG8DR:1;             /*   PG8DR      */
                    _UBYTE PG7DR:1;             /*   PG7DR      */
                    _UBYTE PG6DR:1;             /*   PG6DR      */
                    _UBYTE PG5DR:1;             /*   PG5DR      */
                    _UBYTE PG4DR:1;             /*   PG4DR      */
                    _UBYTE PG3DR:1;             /*   PG3DR      */
                    _UBYTE PG2DR:1;             /*   PG2DR      */
                    _UBYTE PG1DR:1;             /*   PG1DR      */
                    _UBYTE PG0DR:1;             /*   PG0DR      */
                    } BIT;                      /*              */
             } PGDR0;                           /*              */
       union {                                  /* PGPR1        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :7;                  /*              */
                    _UBYTE PG24PR:1;            /*   PG24PR     */
                    _UBYTE PG23PR:1;            /*   PG23PR     */
                    _UBYTE PG22PR:1;            /*   PG22PR     */
                    _UBYTE PG21PR:1;            /*   PG21PR     */
                    _UBYTE PG20PR:1;            /*   PG20PR     */
                    _UBYTE PG19PR:1;            /*   PG19PR     */
                    _UBYTE PG18PR:1;            /*   PG18PR     */
                    _UBYTE PG17PR:1;            /*   PG17PR     */
                    _UBYTE PG16PR:1;            /*   PG16PR     */
                    } BIT;                      /*              */
             } PGPR1;                           /*              */
       union {                                  /* PGPR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE PG15PR:1;            /*   PG15PR     */
                    _UBYTE PG14PR:1;            /*   PG14PR     */
                    _UBYTE PG13PR:1;            /*   PG13PR     */
                    _UBYTE PG12PR:1;            /*   PG12PR     */
                    _UBYTE PG11PR:1;            /*   PG11PR     */
                    _UBYTE PG10PR:1;            /*   PG10PR     */
                    _UBYTE PG9PR:1;             /*   PG9PR      */
                    _UBYTE PG8PR:1;             /*   PG8PR      */
                    _UBYTE PG7PR:1;             /*   PG7PR      */
                    _UBYTE PG6PR:1;             /*   PG6PR      */
                    _UBYTE PG5PR:1;             /*   PG5PR      */
                    _UBYTE PG4PR:1;             /*   PG4PR      */
                    _UBYTE PG3PR:1;             /*   PG3PR      */
                    _UBYTE PG2PR:1;             /*   PG2PR      */
                    _UBYTE PG1PR:1;             /*   PG1PR      */
                    _UBYTE PG0PR:1;             /*   PG0PR      */
                    } BIT;                      /*              */
             } PGPR0;                           /*              */
       _UBYTE wk19[16];                         /*              */
       union {                                  /* PHCR1        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :3;                  /*              */
                    _UBYTE PH7MD:1;             /*   PH7MD      */
                    _UBYTE :3;                  /*              */
                    _UBYTE PH6MD:1;             /*   PH6MD      */
                    _UBYTE :3;                  /*              */
                    _UBYTE PH5MD:1;             /*   PH5MD      */
                    _UBYTE :3;                  /*              */
                    _UBYTE PH4MD:1;             /*   PH4MD      */
                    } BIT;                      /*              */
             } PHCR1;                           /*              */
       union {                                  /* PHCR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :3;                  /*              */
                    _UBYTE PH3MD:1;             /*   PH3MD      */
                    _UBYTE :3;                  /*              */
                    _UBYTE PH2MD:1;             /*   PH2MD      */
                    _UBYTE :3;                  /*              */
                    _UBYTE PH1MD:1;             /*   PH1MD      */
                    _UBYTE :3;                  /*              */
                    _UBYTE PH0MD:1;             /*   PH0MD      */
                    } BIT;                      /*              */
             } PHCR0;                           /*              */
       _UBYTE wk20[10];                         /*              */
       union {                                  /* PHPR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :8;                  /*              */
                    _UBYTE PH7PR:1;             /*   PH7PR      */
                    _UBYTE PH6PR:1;             /*   PH6PR      */
                    _UBYTE PH5PR:1;             /*   PH5PR      */
                    _UBYTE PH4PR:1;             /*   PH4PR      */
                    _UBYTE PH3PR:1;             /*   PH3PR      */
                    _UBYTE PH2PR:1;             /*   PH2PR      */
                    _UBYTE PH1PR:1;             /*   PH1PR      */
                    _UBYTE PH0PR:1;             /*   PH0PR      */
                    } BIT;                      /*              */
             } PHPR0;                           /*              */
       _UBYTE wk21[14];                         /*              */
       union {                                  /* PJCR2        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PJ11MD:2;            /*   PJ11MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PJ10MD:2;            /*   PJ10MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PJ9MD:2;             /*   PJ9MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PJ8MD:2;             /*   PJ8MD      */
                    } BIT;                      /*              */
             } PJCR2;                           /*              */
       union {                                  /* PJCR1        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PJ7MD:2;             /*   PJ7MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PJ6MD:2;             /*   PJ6MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PJ5MD:2;             /*   PJ5MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PJ4MD:2;             /*   PJ4MD      */
                    } BIT;                      /*              */
             } PJCR1;                           /*              */
       union {                                  /* PJCR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PJ3MD:2;             /*   PJ3MD      */
                    _UBYTE :1;                  /*              */
                    _UBYTE PJ2MD:3;             /*   PJ2MD      */
                    _UBYTE :1;                  /*              */
                    _UBYTE PJ1MD:3;             /*   PJ1MD      */
                    _UBYTE :1;                  /*              */
                    _UBYTE PJ0MD:3;             /*   PJ0MD      */
                    } BIT;                      /*              */
             } PJCR0;                           /*              */
       _UBYTE wk22[2];                          /*              */
       union {                                  /* PJIOR0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :4;                  /*              */
                    _UBYTE PJ11IOR:1;           /*   PJ11IOR    */
                    _UBYTE PJ10IOR:1;           /*   PJ10IOR    */
                    _UBYTE PJ9IOR:1;            /*   PJ9IOR     */
                    _UBYTE PJ8IOR:1;            /*   PJ8IOR     */
                    _UBYTE PJ7IOR:1;            /*   PJ7IOR     */
                    _UBYTE PJ6IOR:1;            /*   PJ6IOR     */
                    _UBYTE PJ5IOR:1;            /*   PJ5IOR     */
                    _UBYTE PJ4IOR:1;            /*   PJ4IOR     */
                    _UBYTE PJ3IOR:1;            /*   PJ3IOR     */
                    _UBYTE PJ2IOR:1;            /*   PJ2IOR     */
                    _UBYTE PJ1IOR:1;            /*   PJ1IOR     */
                    _UBYTE PJ0IOR:1;            /*   PJ0IOR     */
                    } BIT;                      /*              */
             } PJIOR0;                          /*              */
       _UBYTE wk23[2];                          /*              */
       union {                                  /* PJDR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :4;                  /*              */
                    _UBYTE PJ11DR:1;            /*   PJ11DR     */
                    _UBYTE PJ10DR:1;            /*   PJ10DR     */
                    _UBYTE PJ9DR:1;             /*   PJ9DR      */
                    _UBYTE PJ8DR:1;             /*   PJ8DR      */
                    _UBYTE PJ7DR:1;             /*   PJ7DR      */
                    _UBYTE PJ6DR:1;             /*   PJ6DR      */
                    _UBYTE PJ5DR:1;             /*   PJ5DR      */
                    _UBYTE PJ4DR:1;             /*   PJ4DR      */
                    _UBYTE PJ3DR:1;             /*   PJ3DR      */
                    _UBYTE PJ2DR:1;             /*   PJ2DR      */
                    _UBYTE PJ1DR:1;             /*   PJ1DR      */
                    _UBYTE PJ0DR:1;             /*   PJ0DR      */
                    } BIT;                      /*              */
             } PJDR0;                           /*              */
       _UBYTE wk24[2];                          /*              */
       union {                                  /* PJPR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :4;                  /*              */
                    _UBYTE PJ11PR:1;            /*   PJ11PR     */
                    _UBYTE PJ10PR:1;            /*   PJ10PR     */
                    _UBYTE PJ9PR:1;             /*   PJ9PR      */
                    _UBYTE PJ8PR:1;             /*   PJ8PR      */
                    _UBYTE PJ7PR:1;             /*   PJ7PR      */
                    _UBYTE PJ6PR:1;             /*   PJ6PR      */
                    _UBYTE PJ5PR:1;             /*   PJ5PR      */
                    _UBYTE PJ4PR:1;             /*   PJ4PR      */
                    _UBYTE PJ3PR:1;             /*   PJ3PR      */
                    _UBYTE PJ2PR:1;             /*   PJ2PR      */
                    _UBYTE PJ1PR:1;             /*   PJ1PR      */
                    _UBYTE PJ0PR:1;             /*   PJ0PR      */
                    } BIT;                      /*              */
             } PJPR0;                           /*              */
       _UBYTE wk25[14];                         /*              */
       union {                                  /* PKCR2        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PK11MD:2;            /*   PK11MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PK10MD:2;            /*   PK10MD     */
                    _UBYTE :2;                  /*              */
                    _UBYTE PK9MD:2;             /*   PK9MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PK8MD:2;             /*   PK8MD      */
                    } BIT;                      /*              */
             } PKCR2;                           /*              */
       union {                                  /* PKCR1        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PK7MD:2;             /*   PK7MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PK6MD:2;             /*   PK6MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PK5MD:2;             /*   PK5MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PK4MD:2;             /*   PK4MD      */
                    } BIT;                      /*              */
             } PKCR1;                           /*              */
       union {                                  /* PKCR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE PK3MD:2;             /*   PK3MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PK2MD:2;             /*   PK2MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PK1MD:2;             /*   PK1MD      */
                    _UBYTE :2;                  /*              */
                    _UBYTE PK0MD:2;             /*   PK0MD      */
                    } BIT;                      /*              */
             } PKCR0;                           /*              */
       _UBYTE wk26[2];                          /*              */
       union {                                  /* PKIOR0       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :4;                  /*              */
                    _UBYTE PK11IOR:1;           /*   PK11IOR    */
                    _UBYTE PK10IOR:1;           /*   PK10IOR    */
                    _UBYTE PK9IOR:1;            /*   PK9IOR     */
                    _UBYTE PK8IOR:1;            /*   PK8IOR     */
                    _UBYTE PK7IOR:1;            /*   PK7IOR     */
                    _UBYTE PK6IOR:1;            /*   PK6IOR     */
                    _UBYTE PK5IOR:1;            /*   PK5IOR     */
                    _UBYTE PK4IOR:1;            /*   PK4IOR     */
                    _UBYTE PK3IOR:1;            /*   PK3IOR     */
                    _UBYTE PK2IOR:1;            /*   PK2IOR     */
                    _UBYTE PK1IOR:1;            /*   PK1IOR     */
                    _UBYTE PK0IOR:1;            /*   PK0IOR     */
                    } BIT;                      /*              */
             } PKIOR0;                          /*              */
       _UBYTE wk27[2];                          /*              */
       union {                                  /* PKDR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :4;                  /*              */
                    _UBYTE PK11DR:1;            /*   PK11DR     */
                    _UBYTE PK10DR:1;            /*   PK10DR     */
                    _UBYTE PK9DR:1;             /*   PK9DR      */
                    _UBYTE PK8DR:1;             /*   PK8DR      */
                    _UBYTE PK7DR:1;             /*   PK7DR      */
                    _UBYTE PK6DR:1;             /*   PK6DR      */
                    _UBYTE PK5DR:1;             /*   PK5DR      */
                    _UBYTE PK4DR:1;             /*   PK4DR      */
                    _UBYTE PK3DR:1;             /*   PK3DR      */
                    _UBYTE PK2DR:1;             /*   PK2DR      */
                    _UBYTE PK1DR:1;             /*   PK1DR      */
                    _UBYTE PK0DR:1;             /*   PK0DR      */
                    } BIT;                      /*              */
             } PKDR0;                           /*              */
       _UBYTE wk28[2];                          /*              */
       union {                                  /* PKPR0        */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    _UBYTE H;                   /*   High       */
                    _UBYTE L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    _UBYTE :4;                  /*              */
                    _UBYTE PK11PR:1;            /*   PK11PR     */
                    _UBYTE PK10PR:1;            /*   PK10PR     */
                    _UBYTE PK9PR:1;             /*   PK9PR      */
                    _UBYTE PK8PR:1;             /*   PK8PR      */
                    _UBYTE PK7PR:1;             /*   PK7PR      */
                    _UBYTE PK6PR:1;             /*   PK6PR      */
                    _UBYTE PK5PR:1;             /*   PK5PR      */
                    _UBYTE PK4PR:1;             /*   PK4PR      */
                    _UBYTE PK3PR:1;             /*   PK3PR      */
                    _UBYTE PK2PR:1;             /*   PK2PR      */
                    _UBYTE PK1PR:1;             /*   PK1PR      */
                    _UBYTE PK0PR:1;             /*   PK0PR      */
                    } BIT;                      /*              */
             } PKPR0;                           /*              */
};                                              /*              */
struct st_hudi {                                /* struct HUDI  */
       union {                                  /* SDIR         */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD TI:8;                /*   TI         */
                    _UWORD :8;                  /*              */
                    } BIT;                      /*              */
             } SDIR;                            /*              */
};                                              /*              */
struct st_pwm {                                 /* struct PWM   */
       union {                                  /* PWBTCR       */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE BTC2G:1;             /*   BTC2G      */
                    _UBYTE BTC2E:1;             /*   BTC2E      */
                    _UBYTE BTC2C:1;             /*   BTC2C      */
                    _UBYTE BTC2A:1;             /*   BTC2A      */
                    _UBYTE BTC1G:1;             /*   BTC1G      */
                    _UBYTE BTC1E:1;             /*   BTC1E      */
                    _UBYTE BTC1C:1;             /*   BTC1C      */
                    _UBYTE BTC1A:1;             /*   BTC1A      */
                    } BIT;                      /*              */
             } PWBTCR;                          /*              */
       _UBYTE wk0[217];                         /*              */
       union {                                  /* PWCR1        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE CMF:1;               /*   CMF        */
                    _UBYTE CST:1;               /*   CST        */
                    _UBYTE CKS:3;               /*   CKS        */
                    } BIT;                      /*              */
             } PWCR1;                           /*              */
       _UBYTE wk1[3];                           /*              */
       union {                                  /* PWPR1        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE OPS1H:1;             /*   OPS1H      */
                    _UBYTE OPS1G:1;             /*   OPS1G      */
                    _UBYTE OPS1F:1;             /*   OPS1F      */
                    _UBYTE OPS1E:1;             /*   OPS1E      */
                    _UBYTE OPS1D:1;             /*   OPS1D      */
                    _UBYTE OPS1C:1;             /*   OPS1C      */
                    _UBYTE OPS1B:1;             /*   OPS1B      */
                    _UBYTE OPS1A:1;             /*   OPS1A      */
                    } BIT;                      /*              */
             } PWPR1;                           /*              */
       _UBYTE wk2[1];                           /*              */
       union {                                  /* PWCYR1       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD PWCY15:1;            /*   PWCY15     */
                    _UWORD PWCY14:1;            /*   PWCY14     */
                    _UWORD PWCY13:1;            /*   PWCY13     */
                    _UWORD PWCY12:1;            /*   PWCY12     */
                    _UWORD PWCY11:1;            /*   PWCY11     */
                    _UWORD PWCY10:1;            /*   PWCY10     */
                    _UWORD PWCY9:1;             /*   PWCY9      */
                    _UWORD PWCY8:1;             /*   PWCY8      */
                    _UWORD PWCY7:1;             /*   PWCY7      */
                    _UWORD PWCY6:1;             /*   PWCY6      */
                    _UWORD PWCY5:1;             /*   PWCY5      */
                    _UWORD PWCY4:1;             /*   PWCY4      */
                    _UWORD PWCY3:1;             /*   PWCY3      */
                    _UWORD PWCY2:1;             /*   PWCY2      */
                    _UWORD PWCY1:1;             /*   PWCY1      */
                    _UWORD PWCY0:1;             /*   PWCY0      */
                    } BIT;                      /*              */
             } PWCYR1;                          /*              */
       union {                                  /* PWBFR1A      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :3;                  /*              */
                    _UWORD OTS:1;               /*   OTS        */
                    _UWORD :2;                  /*              */
                    _UWORD DT9:1;               /*   DT9        */
                    _UWORD DT8:1;               /*   DT8        */
                    _UWORD DT7:1;               /*   DT7        */
                    _UWORD DT6:1;               /*   DT6        */
                    _UWORD DT5:1;               /*   DT5        */
                    _UWORD DT4:1;               /*   DT4        */
                    _UWORD DT3:1;               /*   DT3        */
                    _UWORD DT2:1;               /*   DT2        */
                    _UWORD DT1:1;               /*   DT1        */
                    _UWORD DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR1A;                         /*              */
       union {                                  /* PWBFR1C      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :3;                  /*              */
                    _UWORD OTS:1;               /*   OTS        */
                    _UWORD :2;                  /*              */
                    _UWORD DT9:1;               /*   DT9        */
                    _UWORD DT8:1;               /*   DT8        */
                    _UWORD DT7:1;               /*   DT7        */
                    _UWORD DT6:1;               /*   DT6        */
                    _UWORD DT5:1;               /*   DT5        */
                    _UWORD DT4:1;               /*   DT4        */
                    _UWORD DT3:1;               /*   DT3        */
                    _UWORD DT2:1;               /*   DT2        */
                    _UWORD DT1:1;               /*   DT1        */
                    _UWORD DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR1C;                         /*              */
       union {                                  /* PWBFR1E      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :3;                  /*              */
                    _UWORD OTS:1;               /*   OTS        */
                    _UWORD :2;                  /*              */
                    _UWORD DT9:1;               /*   DT9        */
                    _UWORD DT8:1;               /*   DT8        */
                    _UWORD DT7:1;               /*   DT7        */
                    _UWORD DT6:1;               /*   DT6        */
                    _UWORD DT5:1;               /*   DT5        */
                    _UWORD DT4:1;               /*   DT4        */
                    _UWORD DT3:1;               /*   DT3        */
                    _UWORD DT2:1;               /*   DT2        */
                    _UWORD DT1:1;               /*   DT1        */
                    _UWORD DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR1E;                         /*              */
       union {                                  /* PWBFR1G      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :3;                  /*              */
                    _UWORD OTS:1;               /*   OTS        */
                    _UWORD :2;                  /*              */
                    _UWORD DT9:1;               /*   DT9        */
                    _UWORD DT8:1;               /*   DT8        */
                    _UWORD DT7:1;               /*   DT7        */
                    _UWORD DT6:1;               /*   DT6        */
                    _UWORD DT5:1;               /*   DT5        */
                    _UWORD DT4:1;               /*   DT4        */
                    _UWORD DT3:1;               /*   DT3        */
                    _UWORD DT2:1;               /*   DT2        */
                    _UWORD DT1:1;               /*   DT1        */
                    _UWORD DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR1G;                         /*              */
       union {                                  /* PWCR2        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE :2;                  /*              */
                    _UBYTE IE:1;                /*   IE         */
                    _UBYTE CMF:1;               /*   CMF        */
                    _UBYTE CST:1;               /*   CST        */
                    _UBYTE CKS:3;               /*   CKS        */
                    } BIT;                      /*              */
             } PWCR2;                           /*              */
       _UBYTE wk3[3];                           /*              */
       union {                                  /* PWPR2        */
             _UBYTE BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    _UBYTE OPS2H:1;             /*   OPS2H      */
                    _UBYTE OPS2G:1;             /*   OPS2G      */
                    _UBYTE OPS2F:1;             /*   OPS2F      */
                    _UBYTE OPS2E:1;             /*   OPS2E      */
                    _UBYTE OPS2D:1;             /*   OPS2D      */
                    _UBYTE OPS2C:1;             /*   OPS2C      */
                    _UBYTE OPS2B:1;             /*   OPS2B      */
                    _UBYTE OPS2A:1;             /*   OPS2A      */
                    } BIT;                      /*              */
             } PWPR2;                           /*              */
       _UBYTE wk4[1];                           /*              */
       union {                                  /* PWCYR2       */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD PWCY15:1;            /*   PWCY15     */
                    _UWORD PWCY14:1;            /*   PWCY14     */
                    _UWORD PWCY13:1;            /*   PWCY13     */
                    _UWORD PWCY12:1;            /*   PWCY12     */
                    _UWORD PWCY11:1;            /*   PWCY11     */
                    _UWORD PWCY10:1;            /*   PWCY10     */
                    _UWORD PWCY9:1;             /*   PWCY9      */
                    _UWORD PWCY8:1;             /*   PWCY8      */
                    _UWORD PWCY7:1;             /*   PWCY7      */
                    _UWORD PWCY6:1;             /*   PWCY6      */
                    _UWORD PWCY5:1;             /*   PWCY5      */
                    _UWORD PWCY4:1;             /*   PWCY4      */
                    _UWORD PWCY3:1;             /*   PWCY3      */
                    _UWORD PWCY2:1;             /*   PWCY2      */
                    _UWORD PWCY1:1;             /*   PWCY1      */
                    _UWORD PWCY0:1;             /*   PWCY0      */
                    } BIT;                      /*              */
             } PWCYR2;                          /*              */
       union {                                  /* PWBFR2A      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :3;                  /*              */
                    _UWORD OTS:1;               /*   OTS        */
                    _UWORD :2;                  /*              */
                    _UWORD DT9:1;               /*   DT9        */
                    _UWORD DT8:1;               /*   DT8        */
                    _UWORD DT7:1;               /*   DT7        */
                    _UWORD DT6:1;               /*   DT6        */
                    _UWORD DT5:1;               /*   DT5        */
                    _UWORD DT4:1;               /*   DT4        */
                    _UWORD DT3:1;               /*   DT3        */
                    _UWORD DT2:1;               /*   DT2        */
                    _UWORD DT1:1;               /*   DT1        */
                    _UWORD DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR2A;                         /*              */
       union {                                  /* PWBFR2C      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :3;                  /*              */
                    _UWORD OTS:1;               /*   OTS        */
                    _UWORD :2;                  /*              */
                    _UWORD DT9:1;               /*   DT9        */
                    _UWORD DT8:1;               /*   DT8        */
                    _UWORD DT7:1;               /*   DT7        */
                    _UWORD DT6:1;               /*   DT6        */
                    _UWORD DT5:1;               /*   DT5        */
                    _UWORD DT4:1;               /*   DT4        */
                    _UWORD DT3:1;               /*   DT3        */
                    _UWORD DT2:1;               /*   DT2        */
                    _UWORD DT1:1;               /*   DT1        */
                    _UWORD DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR2C;                         /*              */
       union {                                  /* PWBFR2E      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :3;                  /*              */
                    _UWORD OTS:1;               /*   OTS        */
                    _UWORD :2;                  /*              */
                    _UWORD DT9:1;               /*   DT9        */
                    _UWORD DT8:1;               /*   DT8        */
                    _UWORD DT7:1;               /*   DT7        */
                    _UWORD DT6:1;               /*   DT6        */
                    _UWORD DT5:1;               /*   DT5        */
                    _UWORD DT4:1;               /*   DT4        */
                    _UWORD DT3:1;               /*   DT3        */
                    _UWORD DT2:1;               /*   DT2        */
                    _UWORD DT1:1;               /*   DT1        */
                    _UWORD DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR2E;                         /*              */
       union {                                  /* PWBFR2G      */
             _UWORD WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    _UWORD :3;                  /*              */
                    _UWORD OTS:1;               /*   OTS        */
                    _UWORD :2;                  /*              */
                    _UWORD DT9:1;               /*   DT9        */
                    _UWORD DT8:1;               /*   DT8        */
                    _UWORD DT7:1;               /*   DT7        */
                    _UWORD DT6:1;               /*   DT6        */
                    _UWORD DT5:1;               /*   DT5        */
                    _UWORD DT4:1;               /*   DT4        */
                    _UWORD DT3:1;               /*   DT3        */
                    _UWORD DT2:1;               /*   DT2        */
                    _UWORD DT1:1;               /*   DT1        */
                    _UWORD DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR2G;                         /*              */
};                                              /*              */
#define CPG (*(volatile struct st_cpg *)0xFFFE0010)    /* CPG Address  */
#define INTC (*(volatile struct st_intc *)0xFFFE0800)  /* INTC Address */
#define CCNT (*(volatile struct st_ccnt *)0xFFFC1000)  /* CCNT Address */
#define BSC (*(volatile struct st_bsc *)0xFFFC0000)    /* BSC Address  */
#define DMAC (*(volatile struct st_dmac *)0xFFFE1000)  /* DMAC Address */
#define MTU2 (*(volatile struct st_mtu2 *)0xFFFE4000)  /* MTU2 Address */
#define CMT (*(volatile struct st_cmt *)0xFFFEC000)    /* CMT Address  */
#define WDT (*(volatile struct st_wdt *)0xFFFE0000)    /* WDT Address  */
#define RTC (*(volatile struct st_rtc *)0xFFFE6000)    /* RTC Address  */
#define SCIF0 (*(volatile struct st_scif02 *)0xFFFE8000)/* SCIF0 Address */
#define SCIF1 (*(volatile struct st_scif13 *)0xFFFE8800)/* SCIF1 Address */
#define SCIF2 (*(volatile struct st_scif02 *)0xFFFE9000)/* SCIF2 Address */
#define SCIF3 (*(volatile struct st_scif13 *)0xFFFE9800)/* SCIF3 Address */
#define SCIF4 (*(volatile struct st_scif4567 *)0xFFFEA000)/* SCIF4 Address */
#define SCIF5 (*(volatile struct st_scif4567 *)0xFFFEA800)/* SCIF5 Address */
#define SCIF6 (*(volatile struct st_scif4567 *)0xFFFEB000)/* SCIF6 Address */
#define SCIF7 (*(volatile struct st_scif4567 *)0xFFFEB800)/* SCIF7 Address */
#define RSPI0 (*(volatile struct st_rspi *)0xFFFF8000)  /* RSPI0 Address */
#define RSPI1 (*(volatile struct st_rspi *)0xFFFF8800)  /* RSPI1 Address */
#define IIC3_0 (*(volatile struct st_iic3 *)0xFFFEE000)/* IIC3_0 Address */
#define IIC3_1 (*(volatile struct st_iic3 *)0xFFFEE400)/* IIC3_1 Address */
#define IIC3_2 (*(volatile struct st_iic3 *)0xFFFEE800)/* IIC3_2 Address */
#define SSIF0 (*(volatile struct st_ssif *)0xFFFF0000)/* SSIF0 Address */
#define SSIF1 (*(volatile struct st_ssif *)0xFFFF0800)/* SSIF1 Address */
#define SSIF2 (*(volatile struct st_ssif *)0xFFFF1000)/* SSIF2 Address */
#define SSIF3 (*(volatile struct st_ssif *)0xFFFF1800)/* SSIF3 Address */
#define SIOF (*(volatile struct st_siof *)0xFFFF4800)  /* SIOF Address */
#define RCAN0 (*(volatile struct st_rcan *)0xFFFE5000) /* RCAN0 Address */
#define RCAN1 (*(volatile struct st_rcan *)0xFFFE5800) /* RCAN1 Address */
#define IEB (*(volatile struct st_ieb *)0xFFFEF000)    /* IEB Address  */
#define SPDIF (*(volatile struct st_spdif *)0xFFFF5800)/* SPDIF Address */
#define ROMDEC (*(volatile struct st_romdec *)0xFFFF9000)/* ROMDEC Address */
#define ADC (*(volatile struct st_adc *)0xFFFF9800)    /* ADC Address  */
#define FLCTL (*(volatile struct st_flctl *)0xFFFF4000)/* FLCTL Address */
#define USB (*(volatile struct st_usb *)0xFFFFC000)    /* USB Address  */
#define VDC3 (*(volatile struct st_vdc3 *)0xFFFF2000)  /* VDC3 Address */
#define SRC0 (*(volatile struct st_src0 *)0xFFFE7000)  /* SRC0 Address */
#define SRC1 (*(volatile struct st_src1 *)0xFFFE7800)  /* SRC1 Address */
#define DCU (*(volatile struct st_dcu *)0xFFFF5000)    /* DCU Address  */
#define PORT (*(volatile struct st_gpio *)0xFFFE3812)  /* GPIO Address */
#define HUDI (*(volatile struct st_hudi *)0xFFFE2000)  /* HUDI Address */
#define PWM (*(volatile struct st_pwm *)0xFFFEF406)    /* PWM Address  */

#endif /* _IODEFINE_H_ */
