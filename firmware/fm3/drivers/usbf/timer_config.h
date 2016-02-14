/**
* @file          timer_config.h
* @brief         timer module configuration
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/
#ifndef __TIMER_CONFIG_INC__
#define __TIMER_CONFIG_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Configuration  (variable)                                                  */
/*----------------------------------------------------------------------------*/
/* peripheral clock (PCLK) (MHz) settings */
#define TIMER_PER_CLOCK         (32)
                                /* note: the value is must setted to in multiples of 4 and above 0 */

/* callback function numbers specifications */
#define TIMER_CH0_CALLBACK_MAX       (5)   /* max num of ch.0 callback function */
#define TIMER_CH1_CALLBACK_MAX       (1)   /* max num of ch.1 callback function */

#endif
