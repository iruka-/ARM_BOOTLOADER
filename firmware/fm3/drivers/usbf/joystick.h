/**
* @file          joystick.h
* @brief         joystick module
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/
#ifndef __JOYSTICK_INC__
#define __JOYSTICK_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/
/* API return value */
#define JOYSTICK_RET_OK                  ((uint8_t)0x00)     /* successfully complete */
#define JOYSTICK_RET_INVALID_PARAMETER   ((uint8_t)0x01)     /* parameter invalid     */
#define JOYSTICK_RET_ILLEGAL_ERROR       ((uint8_t)0x02)     /* illegal error         */

/* button id */
#define JOYSTICK_BUTTON_PSW2    ((uint8_t)0x00)     /* PSW2   button             */
#define JOYSTICK_BUTTON_LEFT    ((uint8_t)0x01)     /* left   button of joystick */
#define JOYSTICK_BUTTON_RIGHT   ((uint8_t)0x02)     /* right  button of joystick */
#define JOYSTICK_BUTTON_PSW1    ((uint8_t)0x03)     /* PSW1   button             */
#define JOYSTICK_BUTTON_UP      ((uint8_t)0x04)     /* up     button of joystick */
#define JOYSTICK_BUTTON_DOWN    ((uint8_t)0x05)     /* down   button of joystick */

/* button status */
#define JOYSTICK_NO_PUSH                 ((uint8_t)0x00)     /* unpressed */
#define JOYSTICK_PUSH                    ((uint8_t)0x01)     /* pressed   */

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/
/* joystick module initialize */
extern STATUS JOYSTICK_Init(void);

/* joystick scan process(chattering) */
extern void JOYSTICK_Scan(void);

/* get joystick status of specified button */
extern STATUS JOYSTICK_GetStatus(uint8_t ButtonId, uint8_t *pStatus);

#endif
