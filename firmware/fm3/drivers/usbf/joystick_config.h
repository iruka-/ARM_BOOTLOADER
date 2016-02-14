/**
* @file          joystick_config.h
* @brief         joystick input module configuration
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/
#ifndef __JOYSTICK_CONFIG_INC__
#define __JOYSTICK_CONFIG_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Configuration  (variable)                                                  */
/*----------------------------------------------------------------------------*/
/* chattering settings */
#define JOYSTICK_SCAN_INTERVAL       ((uint8_t)0x05)    /* chattering interval */
#define JOYSTICK_SCAN_COUNTER        ((uint8_t)0x05)    /* chattering counter  */

/* joystick valid level related to joystick */
#define JOYSTICK_IO_VALID_LEVEL      (0U)               /* 0:by "L"level valid */

/* the data registers related to joystick specifications */
#define JOYSTICK_LEFT_PDR   (bFM3_GPIO_PDIR3_P0)    /* data register of left button   */
#define JOYSTICK_RIGHT_PDR  (bFM3_GPIO_PDIR3_P1)    /* data register of right button  */

#if 0  /* because up button and down button is not used */
#define JOYSTICK_UP_PDR     (bFM3_GPIO_PDIR4_P0)    /* data register of up button    */
#define JOYSTICK_DOWN_PDR   (bFM3_GPIO_PDIR4_P1)    /* data register of down button  */
#endif

#if 0  /* because PSW1 button is not used */
#define JOYSTICK_PSW1_PDR   (bFM3_GPIO_PDIR5_P0)    /* data register of PSW1 button   */
#endif

#define JOYSTICK_PSW2_PDR   (bFM3_GPIO_PDIR5_P1)    /* data register of PSW2 button  */

/* the direction registers related to joystick specifications */
#define JOYSTICK_LEFT_DDR   (bFM3_GPIO_DDR3_P0)     /* direction register of left button   */
#define JOYSTICK_RIGHT_DDR  (bFM3_GPIO_DDR3_P1)     /* direction register of right button  */

#if 0  /* because up button and down button is not used */
#define JOYSTICK_UP_DDR     (bFM3_GPIO_DDR4_P0)     /* data register of up button    */
#define JOYSTICK_DOWN_DDR   (bFM3_GPIO_DDR4_P1)     /* data register of down button  */
#endif

#if 0  /* because PSW1 button is not used */
#define JOYSTICK_PSW1_DDR   (bFM3_GPIO_DDR5_P0)     /* direction register of PSW1 button   */
#endif

#define JOYSTICK_PSW2_DDR   (bFM3_GPIO_DDR5_P1)     /* direction register of PSW2 button  */

#endif
