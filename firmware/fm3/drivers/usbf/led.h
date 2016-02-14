/**
* @file          led.h
* @brief         LED module
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/
#ifndef __LED_INC__
#define __LED_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/
/* API return value */
#define LED_RET_OK                      ((uint8_t)0x00) /* successfully complete */
#define LED_RET_INVALID_PARAMETER       ((uint8_t)0x01) /* parameter invalid     */
#define LED_RET_ILLEGAL_ERROR           ((uint8_t)0x02) /* illegal error         */

/*  LED channel */
#define LED_CH1                         ((uint16_t)0x01) /* ch.1 LED              */
#define LED_CH2                         ((uint16_t)0x02) /* ch.2 LED              */
#define LED_CH3                         ((uint16_t)0x04) /* ch.3 LED              */
#define LED_CH4                         ((uint16_t)0x08) /* ch.4 LED              */
#define LED_CH5                         ((uint16_t)0x10) /* ch.5 LED              */
#define LED_CH6                         ((uint16_t)0x20) /* ch.6 LED              */
#define LED_CH7                         ((uint16_t)0x40) /* ch.7 LED              */
#define LED_CH8                         ((uint16_t)0x80) /* ch.8 LED              */
#define LED_ALL                         ((uint16_t)0xFF) /* all ch LED            */

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/
/* LED module initialize */
extern void LED_Init(void);

/* set LED on/off */
extern STATUS  LED_Set(uint16_t IdCh, uint8_t LEDStatus);

#endif
