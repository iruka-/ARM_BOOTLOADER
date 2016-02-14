/**
* @file          timer.c
* @brief         timer module
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
#include "timer_config.h"
#include "timer.h"

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

/* max num of callback table */
#define TIMER_TBL_MAX           ((uint8_t)((TIMER_CH0_CALLBACK_MAX >= TIMER_CH1_CALLBACK_MAX) ? \
                                         TIMER_CH0_CALLBACK_MAX : TIMER_CH1_CALLBACK_MAX))

/* initial value of the registers related to reload timer */
/* initial value of reload timer ch.0 TMCR */
#define TMCR0_INIT_VALUE  ((uint16_t)0x1030)
                           /* bit15(0)     : reserved bit                                     */
                           /* bit14~12(001): peripheral clock (PCLK) divided by 4             */
                           /* bit11(0)     : disable restart                                  */
                           /* bit10(0)     : normal output                                    */
                           /* bit9,8(00)   : disable trigger input                            */
                           /* bit7(0)      : 16 bit timer mode                                */
                           /* bit6~4(011)  : select reload timer function                     */
                           /* bit3(0)      : normal polarity ("L" level)                      */
                           /* bit2(0)      : reload mode                                      */
                           /* bit1(0)      : stops the count operation                        */
                           /* bit0(0)      : ignored                                          */

/* initial value of reload timer ch.0 TMCR2 */
#define TMCR20_INIT_VALUE ((uint8_t)0x00)

/* initial value of reload timer ch.0, ch1 STC */
#define STC0_INIT_VALUE   ((uint8_t)0x10)
#define STC1_INIT_VALUE   ((uint8_t)0x10)
                           /* bit7(0): reserved bit                                           */
                           /* bit6(0): disable trigger interrupt                              */
                           /* bit5(0): reserved bit                                           */
                           /* bit4(1): enable underflow interrupt                             */
                           /* bit3(0): reserved bit                                           */
                           /* bit2(0): clear trigger interrupt flag                           */
                           /* bit1(0): reserved bit                                           */
                           /* bit0(0): clear underflow interrupt flag                         */

/* initial value of reload timer ch.0 PCSR */
#define PCSR0_INIT_VALUE (uint16_t)((TIMER_PER_CLOCK * 250) - 1)
                           /* note: 1ms: (PCLK*10^6/4/1*10^(-3)) - 1 */

/*  peripheral clock (PCLK) divided value */
#define PCLK_DIVIDED_BY_1     (0)   /* peripheral clock (PCLK) divided by 1  */
#define PCLK_DIVIDED_BY_4     (2)   /* peripheral clock (PCLK) divided by 4  */
#define PCLK_DIVIDED_BY_16    (4)   /* peripheral clock (PCLK) divided by 16  */
#define PCLK_DIVIDED_BY_128   (7)   /* peripheral clock (PCLK) divided by 128  */
#define PCLK_DIVIDED_BY_256   (8)   /* peripheral clock (PCLK) divided by 256  */
#define PCLK_DIVIDED_BY_512   (9)   /* peripheral clock (PCLK) divided by 512 */
#define PCLK_DIVIDED_BY_1024  (10)   /* peripheral clock (PCLK) divided by 1024 */
#define PCLK_DIVIDED_BY_2048  (11)   /* peripheral clock (PCLK) divided by 2048 */

/* when the Peripheral clock (PCLK) divided by 1/4/16/128/256/512/1024/2048  */
/* initial value of reload timer ch.1 TMCR */
#define TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_1    ((uint16_t)0x0030)
#define TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_4    ((uint16_t)0x1030)
#define TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_16   ((uint16_t)0x2030)
#define TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_128  ((uint16_t)0x3030)
#define TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_256  ((uint16_t)0x4030)
#define TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_512  ((uint16_t)0x0030)
#define TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_1024 ((uint16_t)0x1030)
#define TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_2048 ((uint16_t)0x2030)
                           /* bit15(0)     : reserved bit                                     */
                           /* TMCR21‚Ìbit0 + bit14~12(XXXX): peripheral clock (PCLK) divided by 1/4/16/128/256/512/1024/2048 */
                           /*        (0000): peripheral clock (PCLK) divided by 1             */
                           /*        (0001): peripheral clock (PCLK) divided by 4             */
                           /*        (0010): peripheral clock (PCLK) divided by 16            */
                           /*        (0011): peripheral clock (PCLK) divided by 128           */
                           /*        (0100): peripheral clock (PCLK) divided by 256           */
                           /*        (1000): peripheral clock (PCLK) divided by 512           */
                           /*        (1001): peripheral clock (PCLK) divided by 1024          */
                           /*        (1010): peripheral clock (PCLK) divided by 2048          */
                           /* bit11(0)     : disable restart                                  */
                           /* bit10(0)     : normal output                                    */
                           /* bit9,8(00)   : disable trigger input                            */
                           /* bit7(0)      : 16 bit timer mode                                */
                           /* bit6~4(011)  : select reload timer function                     */
                           /* bit3(0)      : normal polarity ("L" level)                      */
                           /* bit2(0)      : reload mode                                      */
                           /* bit1(0)      : stops the count operation                        */
                           /* bit0(0)      : ignored                                          */

/* initial value of reload timer ch.1 TMCR2 */
#define TMCR21_CKS3_VALUE_0 ((uint8_t)0x00)
#define TMCR21_CKS3_VALUE_1 ((uint8_t)0x01)

/*----------------------------------------------------------------------------*/
/* Type     Definition                                                        */
/*----------------------------------------------------------------------------*/
/* callback information struct */
typedef struct _TIMER_ST_CALLBACK
{
    void*              Context;                 /* user pointer                  */
    TIMER_CallbackIrq* Function;                /* callback function             */
    uint16_t           Interval;                /* callback interval             */
    uint16_t           Cnt;                     /* interrupt generation counter  */
} TIMER_ST_CALLBACK;

/*----------------------------------------------------------------------------*/
/* Static Variable Definition                                                 */
/*----------------------------------------------------------------------------*/
/* timer initial status */
static uint8_t s_TimerInitStatus;

/* timer start status */
static uint8_t s_TimerStartStatus[TIMER_CH_MAX];

/* callback table  */
static TIMER_ST_CALLBACK s_CallBackTable[TIMER_CH_MAX][TIMER_TBL_MAX];

/* callback register counter */
static uint8_t s_CallbackRegCnt[TIMER_CH_MAX];

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

/**
* @brief            timer module initializ
*
* @param            none
* @return           none
*
**/
void TIMER_Init(void)
{
    /* ----------- register initialize ----------- */
    /* set timer control register of reload timer ch.0 */
    REG_W(FM3_BT0_RT->TMCR, TMCR0_INIT_VALUE);
    REG_W(FM3_BT0_RT->TMCR2, TMCR20_INIT_VALUE);
    REG_W(FM3_BT0_RT->STC, STC0_INIT_VALUE);
    /* set reload value of reload timer ch.0 */
    REG_W(FM3_BT0_RT->PCSR, PCSR0_INIT_VALUE);

    /* ----------- static variable initialize ----------- */
    /* callback register counter initialize */
    (void)memset(s_CallbackRegCnt, 0x00, sizeof(s_CallbackRegCnt));
    /* callback table initialize */
    (void)memset(s_CallBackTable, 0x00, sizeof(s_CallBackTable));
    /* set timer start status to unstarted status for each channel */
    (void)memset(s_TimerStartStatus, 0x00, sizeof(s_TimerStartStatus));
    /* set timer initial status to initialized status */
    s_TimerInitStatus  = ON;

    return;
}

/**
* @brief            set cycle of CH.1 timer(unit: us)
*
* @param            Cycle : cycle value
* @return           TIMER_RET_OK               : successfully complete
* @return           TIMER_RET_INVALID_PARAMETER: parameter invalid
*
**/
STATUS TIMER_SetCh1Cycle(uint32_t Cycle)
{
    STATUS   ret = TIMER_RET_OK;         /* return value */
    uint32_t value;                      /* reload value */
    uint16_t hi_value;                   /* higher 16 bit of reload value */

    /* calculating the reload value */
    value = (uint32_t)TIMER_PER_CLOCK * (uint32_t)Cycle;

    /* get higher 16 bit of reload value */
    hi_value = HIHWORD(value);

    /* if reload value is overflow or underflow, set return value to parameter invalid */
    if ((hi_value >= (1U << PCLK_DIVIDED_BY_2048)) || (Cycle == 0))
    {
        ret = TIMER_RET_INVALID_PARAMETER;
    }
    else
    {
        /* calculating index of initial value table */
        if (hi_value >= (1U << PCLK_DIVIDED_BY_1024))
        {
            /* set timer control register of reload timer ch.1 */
            REG_W(FM3_BT1_RT->TMCR, TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_2048);
            REG_W(FM3_BT1_RT->TMCR2, TMCR21_CKS3_VALUE_1);
            REG_W(FM3_BT1_RT->PCSR, ((uint16_t)(value >> PCLK_DIVIDED_BY_2048) - 1));
        }
        else if (hi_value >= (1U << PCLK_DIVIDED_BY_512))
        {
            /* set timer control register of reload timer ch.1 */
            REG_W(FM3_BT1_RT->TMCR, TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_1024);
            REG_W(FM3_BT1_RT->TMCR2, TMCR21_CKS3_VALUE_1);
            REG_W(FM3_BT1_RT->PCSR, ((uint16_t)(value >> PCLK_DIVIDED_BY_1024) - 1));
        }
        else if (hi_value >= (1U << PCLK_DIVIDED_BY_256))
        {
            /* set timer control register of reload timer ch.1 */
            REG_W(FM3_BT1_RT->TMCR, TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_512);
            REG_W(FM3_BT1_RT->TMCR2, TMCR21_CKS3_VALUE_1);
            REG_W(FM3_BT1_RT->PCSR, ((uint16_t)(value >> PCLK_DIVIDED_BY_512) - 1));
        }
        else if (hi_value >= (1U << PCLK_DIVIDED_BY_128))
        {
            /* set timer control register of reload timer ch.1 */
            REG_W(FM3_BT1_RT->TMCR, TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_256);
            REG_W(FM3_BT1_RT->TMCR2, TMCR21_CKS3_VALUE_0);
            REG_W(FM3_BT1_RT->PCSR, ((uint16_t)(value >> PCLK_DIVIDED_BY_256) - 1));
        }
        else if (hi_value >= (1U << PCLK_DIVIDED_BY_16))
        {
            /* set timer control register of reload timer ch.1 */
            REG_W(FM3_BT1_RT->TMCR, TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_128);
            REG_W(FM3_BT1_RT->TMCR2, TMCR21_CKS3_VALUE_0);
            REG_W(FM3_BT1_RT->PCSR, ((uint16_t)(value >> PCLK_DIVIDED_BY_128) - 1));
        }
        else if (hi_value >= (1U << PCLK_DIVIDED_BY_4))
        {
            /* set timer control register of reload timer ch.1 */
            REG_W(FM3_BT1_RT->TMCR, TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_16);
            REG_W(FM3_BT1_RT->TMCR2, TMCR21_CKS3_VALUE_0);
            REG_W(FM3_BT1_RT->PCSR, ((uint16_t)(value >> PCLK_DIVIDED_BY_16) - 1));
        }
        else if (hi_value > (1U << PCLK_DIVIDED_BY_1))
        {
            /* set timer control register of reload timer ch.1 */
            REG_W(FM3_BT1_RT->TMCR, TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_4);
            REG_W(FM3_BT1_RT->TMCR2, TMCR21_CKS3_VALUE_0);
            REG_W(FM3_BT1_RT->PCSR, ((uint16_t)(value >> PCLK_DIVIDED_BY_4) - 1));
        }
        else
        {
            /* set timer control register of reload timer ch.1 */
            REG_W(FM3_BT1_RT->TMCR, TMCR1_INIT_VALUE_PCLK_DIVIDED_BY_1);
            REG_W(FM3_BT1_RT->TMCR2, TMCR21_CKS3_VALUE_0);
            REG_W(FM3_BT1_RT->PCSR, ((uint16_t)(value >> PCLK_DIVIDED_BY_1) - 1));
        }

        /* set timer control status register of reload timer ch.1 */
        REG_W(FM3_BT1_RT->STC, STC1_INIT_VALUE);
    }

    return (ret);
}

/**
* @brief            start individual timer of specified channel
*
* @param            IdCh  : timer channel(TIMER_CH0/TIMER_CH1)
* @return           TIMER_RET_OK               : successfully complete
* @return           TIMER_RET_INVALID_PARAMETER: parameter invalid
* @return           TIMER_RET_ILLEGAL_ERROR    : illegal error
*
**/
STATUS TIMER_Start(uint8_t IdCh)
{
    uint8_t ret = TIMER_RET_OK;                    /* return value */

    /* check parameter timer channel */
    if (IdCh >= TIMER_CH_MAX)
    {
         /* set return value to parameter invalid */
         ret = TIMER_RET_INVALID_PARAMETER;
    }
    /* check timer initial status and individual timer start status */
    else if ((s_TimerInitStatus == OFF) || (s_TimerStartStatus[IdCh] == ON))
    {
        /* set return value to illegal error */
        ret = TIMER_RET_ILLEGAL_ERROR;
    }
    else
    {
        /* start ch.0 reload timer */
        if (IdCh == TIMER_CH0)
        {
            REG_W(bFM3_BT0_RT_TMCR_CTEN, BIT_SET); /* enable the count operation       */
            REG_W(bFM3_BT0_RT_TMCR_STRG, BIT_SET);  /* activate the reload timer */
        }
        /* start ch.1 reload timer */
        else
        {
            REG_W(bFM3_BT1_RT_TMCR_CTEN, BIT_SET); /* enable the count operation       */
            REG_W(bFM3_BT1_RT_TMCR_STRG, BIT_SET);  /* activate the reload timer */
        }

        /* set individual timer start status to started status */
        s_TimerStartStatus[IdCh] = ON;
    }

    return (ret);
}

/**
* @brief            stop individual timer of specified channel
*
* @param            IdCh  : timer channel(TIMER_CH0/TIMER_CH1)
* @return           TIMER_RET_OK               : successfully complete
* @return           TIMER_RET_INVALID_PARAMETER: parameter invalid
* @return           TIMER_RET_ILLEGAL_ERROR    : illegal error
*
**/
STATUS TIMER_Stop(uint8_t IdCh)
{
    uint8_t ret = TIMER_RET_OK;                    /* return value */

    /* check parameter timer channel */
    if (IdCh >= TIMER_CH_MAX)
    {
         /* set return value to parameter invalid */
         ret = TIMER_RET_INVALID_PARAMETER;
    }
    /* check individual timer start status */
    else if (s_TimerStartStatus[IdCh] == OFF)
    {
        /* set return value to illegal error */
        ret = TIMER_RET_ILLEGAL_ERROR;
    }
    else
    {
        /* stop ch.0 reload timer */
        if (IdCh == TIMER_CH0)
        {
            REG_W(bFM3_BT0_RT_TMCR_CTEN, BIT_CLR); /* stop the count operation     */
        }
        /* stop ch.1 reloard timer */
        else
        {
            REG_W(bFM3_BT1_RT_TMCR_CTEN, BIT_CLR); /* stop the count operation     */
        }

        /* set individual timer start status to unstarted status */
        s_TimerStartStatus[IdCh] = OFF;
    }

    return (ret);
}

/**
* @brief            register callback function of specified channel
*
* @param            IdCh          : timer channel(TIMER_CH0/TIMER_CH1)
* @param            pCallbackFunc : callback function handler
* @param            Interval      : callback interval
* @param            pContext      : user pointer
* @return           TIMER_RET_OK               : successfully complete
* @return           TIMER_RET_INVALID_PARAMETER: parameter invalid
* @return           TIMER_RET_ILLEGAL_ERROR    : illegal error
*
**/
STATUS TIMER_Callback_Reg(uint8_t IdCh, TIMER_CallbackIrq* pCallbackFunc, uint16_t Interval, void* pContext)
{
    uint8_t ret = TIMER_RET_OK;                    /* return value */
    uint8_t over_callback_flag;                    /* callback register cnt over flag */
    uint8_t i;                                     /* loop count */
    uint8_t max_num;                               /* callback max num of specified channel */

    /* check parameter timer channel and callback function handler */
    if ((IdCh >= TIMER_CH_MAX) || (pCallbackFunc == NULL))
    {
         /* set return value to parameter invalid */
         ret = TIMER_RET_INVALID_PARAMETER;
    }
    /* check timer initial status and individual timer start status */
    else if (s_TimerInitStatus == OFF)
    {
         /* set return value to illegal error */
         ret = TIMER_RET_ILLEGAL_ERROR;
    }
    else
    {
        /* get callback register cnt over flag */
        over_callback_flag = ((IdCh == TIMER_CH0)
                                    ? ((s_CallbackRegCnt[IdCh] >= TIMER_CH0_CALLBACK_MAX) ? ON : OFF)
                                    : ((s_CallbackRegCnt[IdCh] >= TIMER_CH1_CALLBACK_MAX) ? ON : OFF));

        /* callback register counter is more than max num of callback function of specified channel */
        if (over_callback_flag == ON)
        {
            /* set return value to illegal error */
            ret = TIMER_RET_ILLEGAL_ERROR;
        }
        else
        {
            /* get callback max num of specified channel */
            max_num = (IdCh == TIMER_CH0) ? TIMER_CH0_CALLBACK_MAX : TIMER_CH1_CALLBACK_MAX;
            /* search empty location in callback table */
            for (i = 0; i < max_num; i++)
            {
                if (s_CallBackTable[IdCh][i].Function == NULL)
                {
                    s_CallBackTable[IdCh][i].Context = pContext;
                    /* callback function register */
                    s_CallBackTable[IdCh][i].Function = pCallbackFunc;
                    /* callback time register */
                    s_CallBackTable[IdCh][i].Interval = Interval;
                    /* clear interrupt generation counter */
                    s_CallBackTable[IdCh][i].Cnt = BIT_CLR;

                    /* callback register counter update */
                    s_CallbackRegCnt[IdCh]++;

                    break;
                }
            }
        }
    }

    return (ret);
}

/**
* @brief            unregister callback function of specified channel
*
* @param            IdCh          : timer channel(TIMER_CH0/TIMER_CH1)
* @param            pCallbackFunc : callback function handler
* @return           TIMER_RET_OK               : successfully complete
* @return           TIMER_RET_INVALID_PARAMETER: parameter invalid
* @return           TIMER_RET_ILLEGAL_ERROR    : illegal error
*
**/
STATUS TIMER_Callback_UnReg(uint8_t IdCh, TIMER_CallbackIrq* pCallbackFunc)
{
    uint8_t ret;                                   /* return value */
    uint8_t i;                                     /* loop count */
    uint8_t max_num;                               /* callback max num of specified channel */

    /* check parameter timer channel */
    if (IdCh >= TIMER_CH_MAX)
    {
        /* set return value to parameter invalid */
        ret = TIMER_RET_INVALID_PARAMETER;
    }
    else
    {
        /* get callback max num of specified channel */
        max_num = (IdCh == TIMER_CH0) ? TIMER_CH0_CALLBACK_MAX : TIMER_CH1_CALLBACK_MAX;

        /* set return value to parameter invalid */
        ret = TIMER_RET_ILLEGAL_ERROR;

        /* search specified callback function in callback table */
        for (i = 0; i < max_num; i++)
        {
            if (s_CallBackTable[IdCh][i].Function == pCallbackFunc)
            {
                /* set return value to successfully complete */
                ret = TIMER_RET_OK;

                /* unregister specified callback function */
                s_CallBackTable[IdCh][i].Function = NULL;
                /* callback register counter decrement */
                s_CallbackRegCnt[IdCh]--;

                break;
            }
        }
    }

    return (ret);
}

/**
* @brief            16 bit reload timer interrupt handler
*
* @param            none
* @return           none
*
**/
void TIMER_RldIRQHandler(void)
{
    uint16_t  i;                                   /* loop counter */

    /* underflow interrupt request of ch.0 reload timer is generated */
    if (REG_R(bFM3_BT0_RT_STC_UDIR) == BIT_SET)
    {
        /* clear underflow interrupt flag */
        REG_W(bFM3_BT0_RT_STC_UDIR, BIT_CLR);

        /* check each element of callback table  */
        for (i = 0; i < s_CallbackRegCnt[TIMER_CH0]; i++)
        {
            /* interrupt generation counter increment */
            s_CallBackTable[TIMER_CH0][i].Cnt++;

            /* if reached callback interval then to do below */
            if (s_CallBackTable[TIMER_CH0][i].Interval == s_CallBackTable[TIMER_CH0][i].Cnt)
            {
                /* call callback function */
                s_CallBackTable[TIMER_CH0][i].Function(s_CallBackTable[TIMER_CH0][i].Context);
                /* clear interrupt generation counter */
                s_CallBackTable[TIMER_CH0][i].Cnt = BIT_CLR;
            }
        }
    }
    /* underflow interrupt request of ch.1 reload timer is generated */
    else if (REG_R(bFM3_BT1_RT_STC_UDIR) == BIT_SET)
    {
        /* clear underflow interrupt flag */
        REG_W(bFM3_BT1_RT_STC_UDIR, BIT_CLR);

        /* check each element of callback table  */
        for (i = 0; i < s_CallbackRegCnt[TIMER_CH1]; i++)
        {
            /* interrupt generation counter increment */
            s_CallBackTable[TIMER_CH1][i].Cnt++;

            /* if reached callback interval then to do below */
            if (s_CallBackTable[TIMER_CH1][i].Interval == s_CallBackTable[TIMER_CH1][i].Cnt)
            {
                /* call callback function */
                s_CallBackTable[TIMER_CH1][i].Function(s_CallBackTable[TIMER_CH1][i].Context);
                /* clear interrupt generation counter */
                s_CallBackTable[TIMER_CH1][i].Cnt = BIT_CLR;
            }
        }
    }
    else
    {
        ; /* do nothing */
    }
}
