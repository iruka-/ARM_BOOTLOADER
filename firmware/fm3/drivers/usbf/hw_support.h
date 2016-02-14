/**
* @file          hw_support.h
* @brief         hardware support function
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/
#ifndef __HW_SUPPORT_INC__
#define __HW_SUPPORT_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/
/* interrupt level */
enum EM_INTR_LEVEL
{
    INTR_LV_0,         /* interrupt level0(highest level that can be set) */
    INTR_LV_1,         /* interrupt level1                                */
    INTR_LV_2,         /* interrupt level2                                */
    INTR_LV_3,         /* interrupt level3                                */
    INTR_LV_4,         /* interrupt level4                                */
    INTR_LV_5,         /* interrupt level5                                */
    INTR_LV_6,         /* interrupt level6                                */
    INTR_LV_7,         /* interrupt level7                                */
    INTR_LV_8,         /* interrupt level8                                */
    INTR_LV_9,         /* interrupt level9                                */
    INTR_LV_10,        /* interrupt level10                               */
    INTR_LV_11,        /* interrupt level11                               */
    INTR_LV_12,        /* interrupt level12                               */
    INTR_LV_13,        /* interrupt level13                               */
    INTR_LV_14,        /* interrupt level14                               */
    INTR_LV_15,        /* interrupt level15                               */
    INTR_MAX           /* max number                                      */
};

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/
/* mb9b500 startkit system boot process */
extern void mb9b500_startkit_boot(void);

#endif /* __HW_SUPPORT_INC__ */
