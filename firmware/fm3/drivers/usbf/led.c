/**
* @file          led.c
* @brief         LED module
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "common.h"
/*#include "gpio.h"*/
#include "led_config.h"
#include "led.h"

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/
/* LED initial status */
#define LED_STATUS_UNINIT       ((uint8_t)0x00)   /* uninitialized       */
#define LED_STATUS_INIT         ((uint8_t)0x01)   /* initialized         */

#if (LED_ON_IO_LEVEL == LOW)
    #define LED_OFF_IO_LEVEL    (HIGH)          /* led off IO level    */
#else
    #define LED_OFF_IO_LEVEL    (LOW)          /* led on IO level     */
#endif

/*----------------------------------------------------------------------------*/
/* Static Variable Definition                                                 */
/*----------------------------------------------------------------------------*/
static uint8_t s_LedInitStatus;                  /* LED initial status  */

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

/**
* @brief            LED module initialize
*
* @param            none
* @return           none
*
**/
void LED_Init(void)
{
    /* turn all ch LED off */
    REG_W(LED_CH1_PDR, LED_OFF_IO_LEVEL);       /* set ch.1 LED PDR */
    REG_W(LED_CH2_PDR, LED_OFF_IO_LEVEL);       /* set ch.2 LED PDR */
    REG_W(LED_CH3_PDR, LED_OFF_IO_LEVEL);       /* set ch.3 LED PDR */
    REG_W(LED_CH4_PDR, LED_OFF_IO_LEVEL);       /* set ch.4 LED PDR */
    REG_W(LED_CH5_PDR, LED_OFF_IO_LEVEL);       /* set ch.5 LED PDR */
    REG_W(LED_CH6_PDR, LED_OFF_IO_LEVEL);       /* set ch.6 LED PDR */
    REG_W(LED_CH7_PDR, LED_OFF_IO_LEVEL);       /* set ch.7 LED PDR */
    REG_W(LED_CH8_PDR, LED_OFF_IO_LEVEL);       /* set ch.8 LED PDR */

    /* use the port related to LED as output port */
    REG_W(LED_CH1_DDR, HIGH);                 /* set ch.1 LED DDR */
    REG_W(LED_CH2_DDR, HIGH);                 /* set ch.2 LED DDR */
    REG_W(LED_CH3_DDR, HIGH);                 /* set ch.3 LED DDR */
    REG_W(LED_CH4_DDR, HIGH);                 /* set ch.4 LED DDR */
    REG_W(LED_CH5_DDR, HIGH);                 /* set ch.5 LED DDR */
    REG_W(LED_CH6_DDR, HIGH);                 /* set ch.6 LED DDR */
    REG_W(LED_CH7_DDR, HIGH);                 /* set ch.7 LED DDR */
    REG_W(LED_CH8_DDR, HIGH);                 /* set ch.8 LED DDR */

    /* set LED initial status to initialized status */
    s_LedInitStatus  = LED_STATUS_INIT;

    return;
}

/**
* @brief            set LED on/off
*
* @param            IdCh      : LED channel
* @param            LEDStatus : LED on/off status
* @return           LED_RET_OK               : successfully complete
* @return           LED_RET_INVALID_PARAMETER: parameter invalid
* @return           LED_RET_ILLEGAL_ERROR    : illegal error
*
**/
STATUS LED_Set(uint16_t IdCh, uint8_t LEDStatus)
{
    uint8_t ret = LED_RET_OK;                  /* return value */
    uint8_t led_value;                         /* value using for LED setting */

    /* check parameter LED channel */
    if (IdCh >  LED_ALL)
    {
        /* set return value to parameter invalid */
        ret = LED_RET_INVALID_PARAMETER;
    }
    /* check LED initial status */
    else if (s_LedInitStatus  == LED_STATUS_UNINIT)
    {
        /* set return value to illegal error */
        ret = LED_RET_ILLEGAL_ERROR;
    }
    else
    {
#if (LED_ON_IO_LEVEL == LOW)
        /* get LED setting value */
        led_value = ~LEDStatus;
#else
        /* get LED setting value */
        led_value = LEDStatus;
#endif
        /* set ch.1 LED  */
        if ((IdCh & LED_CH1) == LED_CH1)
        {
            REG_W(LED_CH1_PDR, led_value);
        }
        /* set ch.2 LED  */
        if ((IdCh & LED_CH2) == LED_CH2)
        {
            REG_W(LED_CH2_PDR, led_value >> 1);
        }
        /* set ch.3 LED  */
        if ((IdCh & LED_CH3) == LED_CH3)
        {
            REG_W(LED_CH3_PDR, led_value >> 2);
        }
        /* set ch.4 LED  */
        if ((IdCh & LED_CH4) == LED_CH4)
        {
            REG_W(LED_CH4_PDR, led_value >> 3);
        }
        /* set ch.5 LED  */
        if ((IdCh & LED_CH5) == LED_CH5)
        {
            REG_W(LED_CH5_PDR, led_value >> 4);
        }
        /* set ch.6 LED  */
        if ((IdCh & LED_CH6) == LED_CH6)
        {
            REG_W(LED_CH6_PDR, led_value >> 5);
        }
        /* set ch.7 LED  */
        if ((IdCh & LED_CH7) == LED_CH7)
        {
            REG_W(LED_CH7_PDR, led_value >> 6);
        }
        /* set ch.8 LED  */
        if ((IdCh & LED_CH8) == LED_CH8)
        {
            REG_W(LED_CH8_PDR, led_value >> 7);
        }
    }

    return (ret);
}
