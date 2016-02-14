/**
* @file          timer.h
* @brief         timer module
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/
/* multi-include refuse */
#ifndef __TIMER_INC__
#define __TIMER_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Type Definition                                                            */
/*----------------------------------------------------------------------------*/
/* callback function */
typedef void TIMER_CallbackIrq(void* pContext);

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/
/* API return value */
#define TIMER_RET_OK                    ((uint8_t)0x00) /* successfully complete */
#define TIMER_RET_INVALID_PARAMETER     ((uint8_t)0x01) /* parameter invalid     */
#define TIMER_RET_ILLEGAL_ERROR         ((uint8_t)0x02) /* illegal error         */

/*  timer channel */
#define TIMER_CH0                       ((uint8_t)0x00) /* ch.0 reload timer     */
#define TIMER_CH1                       ((uint8_t)0x01) /* ch.1 reload timer     */
#define TIMER_CH_MAX                    ((uint8_t)0x02) /* max num of channels   */

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/
/* timer module initialize */
extern void TIMER_Init(void);

/* set cycle of ch.1 timer */
extern STATUS TIMER_SetCh1Cycle(uint32_t Cycle);

/* start individual timer of specified channel */
extern STATUS TIMER_Start(uint8_t IdCh);

/* stop individual timer of specified channel */
extern STATUS TIMER_Stop(uint8_t IdCh);

/* register callback function of specified channel */
extern STATUS TIMER_Callback_Reg(uint8_t IdCh, TIMER_CallbackIrq* pCallbackFunc, uint16_t Interval, void* pContext);

/* unregister callback function of specified channel */
extern STATUS TIMER_Callback_UnReg(uint8_t IdCh, TIMER_CallbackIrq* pCallbackFunc);

/* 16 bit reload timer interrupt handler */
extern void TIMER_RldIRQHandler(void);

#endif
