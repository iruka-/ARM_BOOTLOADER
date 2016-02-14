/**
* @file          led_config.h
* @brief         LED module configuration
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/
#ifndef __LED_CONFIG_INC__
#define __LED_CONFIG_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Configuration  (variable)                                                  */
/*----------------------------------------------------------------------------*/

/* the data registers related to LED specifications */
#define LED_CH1_PDR         (bFM3_GPIO_PDOR3_P2)   /* data register of ch.1 LED      */
#define LED_CH2_PDR         (bFM3_GPIO_PDOR3_P3)   /* data register of ch.2 LED      */
#define LED_CH3_PDR         (bFM3_GPIO_PDOR3_P4)   /* data register of ch.3 LED      */
#define LED_CH4_PDR         (bFM3_GPIO_PDOR3_P5)   /* data register of ch.4 LED      */
#define LED_CH5_PDR         (bFM3_GPIO_PDOR3_P6)   /* data register of ch.5 LED      */
#define LED_CH6_PDR         (bFM3_GPIO_PDOR3_P7)   /* data register of ch.6 LED      */
#define LED_CH7_PDR         (bFM3_GPIO_PDOR3_P8)   /* data register of ch.7 LED      */
#define LED_CH8_PDR         (bFM3_GPIO_PDOR3_P9)   /* data register of ch.8 LED      */

/* the direction registers related to LED specifications */
#define LED_CH1_DDR         (bFM3_GPIO_DDR3_P2)   /* direction register of ch.1 LED */
#define LED_CH2_DDR         (bFM3_GPIO_DDR3_P3)   /* direction register of ch.2 LED */
#define LED_CH3_DDR         (bFM3_GPIO_DDR3_P4)   /* direction register of ch.3 LED */
#define LED_CH4_DDR         (bFM3_GPIO_DDR3_P5)   /* direction register of ch.4 LED */
#define LED_CH5_DDR         (bFM3_GPIO_DDR3_P6)   /* direction register of ch.5 LED */
#define LED_CH6_DDR         (bFM3_GPIO_DDR3_P7)   /* direction register of ch.6 LED */
#define LED_CH7_DDR         (bFM3_GPIO_DDR3_P8)   /* direction register of ch.7 LED */
#define LED_CH8_DDR         (bFM3_GPIO_DDR3_P9)   /* direction register of ch.8 LED */

/* the level turning on LED related to LED specifications */
#define LED_ON_IO_LEVEL     (0U)                /* 0:by "L"level turn on LED */
                                                /* 1:by "H"level turn on LED */

#endif
