/**
* @file          joystick.c
* @brief         joystick module
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
#include "joystick_config.h"
#include "joystick.h"
#include "timer.h"

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

/* initial value of status for each button of joystick */
#if (JOYSTICK_IO_VALID_LEVEL == LOW)
    #define STATUS_INIT_VALUE    (0xFF)
#else
    #define STATUS_INIT_VALUE    (0x00)
#endif

/*----------------------------------------------------------------------------*/
/* Static Variable Definition                                                 */
/*----------------------------------------------------------------------------*/
/* chattering counter for each button of joystick */
#if defined(JOYSTICK_PSW1_PDR) && defined(JOYSTICK_PSW1_DDR)
static uint8_t s_JoyStickPsw1ChtCnt;   /* chattering counter for PSW1 button */
#endif

#if defined(JOYSTICK_PSW2_PDR) && defined(JOYSTICK_PSW2_DDR)
static uint8_t s_JoyStickPsw2ChtCnt;   /* chattering counter for PSW2 button */
#endif

#if defined(JOYSTICK_LEFT_PDR) && defined(JOYSTICK_LEFT_DDR)
static uint8_t s_JoyStickLeftChtCnt;     /* chattering counter for left button of joystick   */
#endif

#if defined(JOYSTICK_RIGHT_PDR) && defined(JOYSTICK_RIGHT_DDR)
static uint8_t s_JoyStickRightChtCnt;    /* chattering counter for right button of joystick  */
#endif

#if defined(JOYSTICK_UP_PDR) && defined(JOYSTICK_UP_DDR)
static uint8_t s_JoyStickUpChtCnt;      /* chattering counter for up button of joystick      */
#endif

#if defined(JOYSTICK_DOWN_PDR) && defined(JOYSTICK_DOWN_DDR)
static uint8_t s_JoyStickDownChtCnt;    /* chattering counter for down button of joystick    */
#endif

/* last status for each button of joystick */
static uint8_t s_JoyStickLastStatus;
/*              B'---XXXXXX                      */
/*                   ||||||_PSW2   button        */
/*                   |||||__left   button        */
/*                   ||||___right  button        */
/*                   |||____PSW1   button        */
/*                   ||_____up     button        */
/*                   |______down   button        */

/* now status for each button of joystick */
static uint8_t s_JoyStickNowStatus;
/*              B'---XXXXXX                      */
/*                   ||||||_PSW2   button        */
/*                   |||||__left   button        */
/*                   ||||___right  button        */
/*                   |||____PSW1   button        */
/*                   ||_____up     button        */
/*                   |______down   button        */

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

/**
* @brief            joystick module initialize
*
* @param            none
* @return           JOYSTICK_RET_OK               : successfully complete
* @return           JOYSTICK_RET_ILLEGAL_ERROR    : illegal error
*
**/
STATUS JOYSTICK_Init(void)
{
    uint8_t ret = JOYSTICK_RET_ILLEGAL_ERROR;     /* return value */

    /* set DDR for each button of joystick */
#if defined(JOYSTICK_PSW1_PDR) && defined(JOYSTICK_PSW1_DDR)
    /* use the port as input port of PSW1 button */
    REG_W(JOYSTICK_PSW1_DDR, LOW);
#endif

#if defined(JOYSTICK_PSW2_PDR) && defined(JOYSTICK_PSW2_DDR)
    /* use the port as input port of PSW2 button */
    REG_W(JOYSTICK_PSW2_DDR, LOW);
#endif

#if defined(JOYSTICK_LEFT_DDR) && defined(JOYSTICK_LEFT_PDR)
    /* use the port as input port of left button of joystick */
    REG_W(JOYSTICK_LEFT_DDR, LOW);
#endif

#if defined(JOYSTICK_RIGHT_DDR) && defined(JOYSTICK_RIGHT_PDR)
    /* use the port as input port of right button of joystick */
    REG_W(JOYSTICK_RIGHT_DDR, LOW);
#endif

#if defined(JOYSTICK_UP_DDR) && defined(JOYSTICK_UP_PDR)
    /* use the port as input port of up button of joystick */
    REG_W(JOYSTICK_UP_DDR, LOW);
#endif

#if defined(JOYSTICK_DOWN_DDR) && defined(JOYSTICK_DOWN_PDR)
    /* use the port as input port of down button of joystick */
    REG_W(JOYSTICK_DOWN_DDR, LOW);
#endif

    /* static variable initialize  */
    /* last status for each button of joystick initialize */
    s_JoyStickLastStatus = STATUS_INIT_VALUE;
    /* now status for each button of joystick initialize */
    s_JoyStickNowStatus = STATUS_INIT_VALUE;

    /* chattering counter initialize */
#if defined(JOYSTICK_PSW1_PDR) && defined(JOYSTICK_PSW1_DDR)
    s_JoyStickPsw1ChtCnt = 0;
#endif

#if defined(JOYSTICK_PSW2_PDR) && defined(JOYSTICK_PSW2_DDR)
    s_JoyStickPsw2ChtCnt = 0;
#endif

#if defined(JOYSTICK_LEFT_PDR) && defined(JOYSTICK_LEFT_DDR)
    s_JoyStickLeftChtCnt = 0;
#endif

#if defined(JOYSTICK_RIGHT_PDR) && defined(JOYSTICK_RIGHT_DDR)
    s_JoyStickRightChtCnt = 0;
#endif

#if defined(JOYSTICK_UP_PDR) && defined(JOYSTICK_UP_DDR)
    s_JoyStickUpChtCnt = 0;
#endif

#if defined(JOYSTICK_DOWN_PDR) && defined(JOYSTICK_DOWN_DDR)
    s_JoyStickDownChtCnt = 0;
#endif

    /* register callback function with joystick scan process(chattering) to timer module */
    if (TIMER_Callback_Reg(TIMER_CH0, (TIMER_CallbackIrq *)&JOYSTICK_Scan, JOYSTICK_SCAN_INTERVAL, NULL) == TIMER_RET_OK)
    {
        /* set successfully complete to return value */
        ret = JOYSTICK_RET_OK;
    }

    return(ret);
}

/**
* @brief            joystick scan process(chattering)
*
* @param            none
* @return           none
*
**/
void JOYSTICK_Scan(void)
{
    /*  PSW1 button scan process(chattering) */
#if defined(JOYSTICK_PSW1_PDR) && defined(JOYSTICK_PSW1_DDR)
    /* PSW1 last status is equal to instant value of PDR */
    if (REG_R(JOYSTICK_PSW1_PDR) == ((s_JoyStickLastStatus >> JOYSTICK_BUTTON_PSW1) & 0x01))
    {
        /* chattering counter increment */
        s_JoyStickPsw1ChtCnt++;
        /* reach specified scan counter */
        if(s_JoyStickPsw1ChtCnt >= JOYSTICK_SCAN_COUNTER)
        {
            /* clear chattering counter */
            s_JoyStickPsw1ChtCnt = 0x00;
            /* update now joystick status */
            s_JoyStickNowStatus &= (uint8_t)(~(1U << JOYSTICK_BUTTON_PSW1));
            s_JoyStickNowStatus |= (uint8_t)(REG_R(JOYSTICK_PSW1_PDR)) << JOYSTICK_BUTTON_PSW1;
        }
    }
    else
    {
        /* clear chattering counter */
        s_JoyStickPsw1ChtCnt = 0x00;
        /* update joystick last status */
        s_JoyStickLastStatus &= (uint8_t)(~(1U << JOYSTICK_BUTTON_PSW1));
        s_JoyStickLastStatus |= (uint8_t)(REG_R(JOYSTICK_PSW1_PDR)) << JOYSTICK_BUTTON_PSW1;
    }
#endif  /* defined(JOYSTICK_PSW1_PDR) && defined(JOYSTICK_PSW1_DDR) */

    /*  PSW2 button scan process(chattering) */
#if defined(JOYSTICK_PSW2_PDR) && defined(JOYSTICK_PSW2_DDR)
    /* PSW2 last status is equal to instant value of PDR */
    if (REG_R(JOYSTICK_PSW2_PDR) == ((s_JoyStickLastStatus >> JOYSTICK_BUTTON_PSW2) & 0x01))
    {
        /* chattering counter increment */
        s_JoyStickPsw2ChtCnt++;
        /* reach specified scan counter */
        if(s_JoyStickPsw2ChtCnt >= JOYSTICK_SCAN_COUNTER)
        {
            /* clear chattering counter */
            s_JoyStickPsw2ChtCnt = 0x00;
            /* update now joystick status */
            s_JoyStickNowStatus &= (uint8_t)(~(1U << JOYSTICK_BUTTON_PSW2));
            s_JoyStickNowStatus |= (uint8_t)(REG_R(JOYSTICK_PSW2_PDR)) << JOYSTICK_BUTTON_PSW2;
        }
    }
    else
    {
        /* clear chattering counter */
        s_JoyStickPsw2ChtCnt = 0x00;
        /* update joystick last status */
        s_JoyStickLastStatus &= (uint8_t)(~(1U << JOYSTICK_BUTTON_PSW2));
        s_JoyStickLastStatus |= (uint8_t)(REG_R(JOYSTICK_PSW2_PDR)) << JOYSTICK_BUTTON_PSW2;
    }
#endif  /* defined(JOYSTICK_PSW2_PDR) && defined(JOYSTICK_PSW2_DDR) */


    /*  left button scan process(chattering) */
#if defined(JOYSTICK_LEFT_PDR) && defined(JOYSTICK_LEFT_DDR)
    /* joystick last status is equal to instant value of PDR */
    if (REG_R(JOYSTICK_LEFT_PDR) == ((s_JoyStickLastStatus >> JOYSTICK_BUTTON_LEFT) & 0x01))
    {
        /* chattering counter increment */
        s_JoyStickLeftChtCnt++;
        /* reach specified scan counter */
        if(s_JoyStickLeftChtCnt >= JOYSTICK_SCAN_COUNTER)
        {
            /* clear chattering counter */
            s_JoyStickLeftChtCnt = 0x00;
            /* update now joystick status */
            s_JoyStickNowStatus &= (uint8_t)(~(1U << JOYSTICK_BUTTON_LEFT));
            s_JoyStickNowStatus |= (uint8_t)(REG_R(JOYSTICK_LEFT_PDR)) << JOYSTICK_BUTTON_LEFT;
        }
    }
    else
    {
        /* clear chattering counter */
        s_JoyStickLeftChtCnt = 0x00;
        /* update joystick last status */
        s_JoyStickLastStatus &= (uint8_t)(~(1U << JOYSTICK_BUTTON_LEFT));
        s_JoyStickLastStatus |= (uint8_t)(REG_R(JOYSTICK_LEFT_PDR)) << JOYSTICK_BUTTON_LEFT;
    }
#endif  /* defined(JOYSTICK_LEFT_PDR) && defined(JOYSTICK_LEFT_DDR) */

    /*  right button scan process(chattering) */
#if defined(JOYSTICK_RIGHT_PDR) && defined(JOYSTICK_RIGHT_DDR)
    /* joystick last status is equal to instant value of PDR */
    if (REG_R(JOYSTICK_RIGHT_PDR) == ((s_JoyStickLastStatus >> JOYSTICK_BUTTON_RIGHT) & 0x01))
    {
        /* chattering counter increment */
        s_JoyStickRightChtCnt++;
        /* reach specified scan counter */
        if(s_JoyStickRightChtCnt >= JOYSTICK_SCAN_COUNTER)
        {
            /* clear chattering counter */
            s_JoyStickRightChtCnt = 0x00;
            /* update now joystick status */
            s_JoyStickNowStatus &= (uint8_t)(~(1U << JOYSTICK_BUTTON_RIGHT));
            s_JoyStickNowStatus |= (uint8_t)(REG_R(JOYSTICK_RIGHT_PDR)) << JOYSTICK_BUTTON_RIGHT;
        }
    }
    else
    {
        /* clear chattering counter */
        s_JoyStickRightChtCnt = 0x00;
        /* update joystick last status */
        s_JoyStickLastStatus &= (uint8_t)(~(1U << JOYSTICK_BUTTON_RIGHT));
        s_JoyStickLastStatus |= (uint8_t)(REG_R(JOYSTICK_RIGHT_PDR)) << JOYSTICK_BUTTON_RIGHT;
    }
#endif  /* defined(JOYSTICK_RIGHT_PDR) && defined(JOYSTICK_RIGHT_DDR) */

    /*  up button scan process(chattering) */
#if defined(JOYSTICK_UP_PDR) && defined(JOYSTICK_UP_DDR)
    /* joystick last status is equal to instant value of PDR */
    if (REG_R(JOYSTICK_UP_PDR) == ((s_JoyStickLastStatus >> JOYSTICK_BUTTON_UP) & 0x01))
    {
        /* chattering counter increment */
        s_JoyStickUpChtCnt++;
        /* reach specified scan counter */
        if(s_JoyStickUpChtCnt >= JOYSTICK_SCAN_COUNTER)
        {
            /* clear chattering counter */
            s_JoyStickUpChtCnt = 0x00;
            /* update now joystick status */
            s_JoyStickNowStatus &= (uint8_t)(~(1U << JOYSTICK_BUTTON_UP));
            s_JoyStickNowStatus |= (uint8_t)(REG_R(JOYSTICK_UP_PDR)) << JOYSTICK_BUTTON_UP;
        }
    }
    else
    {
        /* clear chattering counter */
        s_JoyStickUpChtCnt = 0x00;
        /* update joystick last status */
        s_JoyStickLastStatus &= (uint8_t)(~(1U << JOYSTICK_BUTTON_UP));
        s_JoyStickLastStatus |= (uint8_t)(REG_R(JOYSTICK_UP_PDR)) << JOYSTICK_BUTTON_UP;
    }
#endif  /* defined(JOYSTICK_UP_PDR) && defined(JOYSTICK_UP_DDR) */

    /*  down button scan process(chattering) */
#if defined(JOYSTICK_DOWN_PDR) && defined(JOYSTICK_DOWN_DDR)
    /* joystick last status is equal to instant value of PDR */
    if (REG_R(JOYSTICK_DOWN_PDR) == ((s_JoyStickLastStatus >> JOYSTICK_BUTTON_DOWN) & 0x01))
    {
        /* chattering counter increment */
        s_JoyStickDownChtCnt++;
        /* reach specified scan counter */
        if(s_JoyStickDownChtCnt >= JOYSTICK_SCAN_COUNTER)
        {
            /* clear chattering counter */
            s_JoyStickDownChtCnt = 0x00;
            /* update now joystick status */
            s_JoyStickNowStatus &= (uint8_t)(~(1U << JOYSTICK_BUTTON_DOWN));
            s_JoyStickNowStatus |= (uint8_t)(REG_R(JOYSTICK_DOWN_PDR)) << JOYSTICK_BUTTON_DOWN;
        }
    }
    else
    {
        /* clear chattering counter */
        s_JoyStickDownChtCnt = 0x00;
        /* update joystick last status */
        s_JoyStickLastStatus &= (uint8_t)(~(1U << JOYSTICK_BUTTON_DOWN));
        s_JoyStickLastStatus |= (uint8_t)(REG_R(JOYSTICK_DOWN_PDR)) << JOYSTICK_BUTTON_DOWN;
    }
#endif  /* defined(JOYSTICK_DOWN_PDR) && defined(JOYSTICK_DOWN_DDR) */

    return;
}

/**
* @brief            get joystick status of specified button
*
* @param            ButtonId      : button ID
* @param            pStatus       : pointer to button status
* @return           JOYSTICK_RET_OK               : successfully complete
* @return           JOYSTICK_RET_INVALID_PARAMETER: parameter invalid
*
**/
STATUS JOYSTICK_GetStatus(uint8_t ButtonId, uint8_t *pStatus)
{
    uint8_t ret = JOYSTICK_RET_OK;   /* return value */

    /* check parameter button ID and pointer to button status */
    if (pStatus == NULL)
    {
        /* set return value to parameter invalid */
        ret = JOYSTICK_RET_INVALID_PARAMETER;
    }
    else
    {
        /* judge button ID */
        switch (ButtonId)
        {
#if defined(JOYSTICK_PSW1_PDR) && defined(JOYSTICK_PSW1_DDR)
            /* PSW1 button */
            case JOYSTICK_BUTTON_PSW1:
#endif
#if defined(JOYSTICK_PSW2_PDR) && defined(JOYSTICK_PSW2_DDR)
            /* PSW2 button */
            case JOYSTICK_BUTTON_PSW2:
#endif
#if defined(JOYSTICK_LEFT_PDR) && defined(JOYSTICK_LEFT_DDR)
            /* left button of joystick */
            case JOYSTICK_BUTTON_LEFT:
#endif
#if defined(JOYSTICK_RIGHT_PDR) && defined(JOYSTICK_RIGHT_DDR)
            /* right button of joystick */
            case JOYSTICK_BUTTON_RIGHT:
#endif
#if defined(JOYSTICK_UP_PDR) && defined(JOYSTICK_UP_DDR)
            /* up button of joystick */
            case JOYSTICK_BUTTON_UP:
#endif
#if defined(JOYSTICK_DOWN_PDR) && defined(JOYSTICK_DOWN_DDR)
            /* down button of joystick */
            case JOYSTICK_BUTTON_DOWN:
#endif

#if (JOYSTICK_IO_VALID_LEVEL == LOW)
            /* get joystick status of specified button */
            *pStatus = (((uint8_t)(~s_JoyStickNowStatus) >> ButtonId) & 0x01);
#else
            /* get joystick status of specified button */
            *pStatus = ((s_JoyStickNowStatus >> ButtonId) & 0x01);
#endif

            break;

            /* others */
            default:
                /* set return value to parameter invalid */
                ret = JOYSTICK_RET_INVALID_PARAMETER;
                break;
        }
    }

    return(ret);
}
