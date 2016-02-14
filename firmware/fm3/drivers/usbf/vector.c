/**
* @file          vector.c
* @brief         interrupt vector table definition
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "cpu_config.h"
#include "timer.h"
#include "usbf.h"

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/
#define MASTER_CLOCK_SELECT_MAIN  (0x01) /* select master clock(main clock divided by 2) */

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

/**
* @brief            this function for default interrupt
*
* @param            none
* @return           none
*
**/
void IntDefaultHandler(void)
{
    char c;

    /* disable interrupt */
    __disable_irq();

    /* select main clock divided by 2 as the master clock */
    c = REG_R(FM3_CRG->SCM_CTL);
    c = c & 0x1F;
    c = c | (MASTER_CLOCK_SELECT_MAIN<<5);
    REG_W(FM3_CRG->SCM_CTL, c);
    
    /* wait for main clock ready */
    while ((REG_R(FM3_CRG->SCM_STR)>>5) != MASTER_CLOCK_SELECT_MAIN)
    {
        ;
    }

    /* software reset */
    NVIC_SystemReset();

    return;
}


/**
* @brief            base timer interrupt
*
* @param            none
* @return           none
*
**/
void BT_IRQHandler(void)
{
    TIMER_RldIRQHandler();

    return;
}

/**
* @brief            USB function(DRQ of End Point 1 to 5) Handler
*
* @param            none
* @return           none
*
**/
void USBF_Handler(void)
{
    USBF_UsbFunction1_IRQHandler();

    return;
}

/**
* @brief            USB function(DRQI, DRQO of End Point 0 and each status) and USB HOST (Each status) Handler
*
* @param            none
* @return           none
*
**/
void USB_Handler(void)
{
    USBF_UsbFunction2_IRQHandler();

    return;
}
