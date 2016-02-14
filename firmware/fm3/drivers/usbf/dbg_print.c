/**
* @file          dbg_print.c
* @brief         implementation of the debug print support function
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
/*#include "gpio.h"*/
#include "usbfunc_demoapp_config.h"
#include "hw_support.h"

#ifdef DEBUG_PRINT
/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Type     Definition                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Static Variable Definition                                                 */
/*----------------------------------------------------------------------------*/
/* buffer for send message */
uint8_t  g_DbgBuffer[256];

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/
/**
* @brief            send message via UART
*
* @param            StrMsg  : pointer to message
* @param            MsgLen  : length of message
* @return           none
*
**/
void SendMsg(uint8_t* StrMsg, uint16_t MsgLen)
{
    /* enable transmission */
    bMFS4_UART_SCR_TEX = BIT_SET;

    /* send message */
    while (MsgLen != 0)
    {
        if (*StrMsg == '\n')
        {
            MFS4_UART->TDR = '\r';
            while (bMFS4_UART_SSR_TDRE == BIT_CLR) {}
        }
        MFS4_UART->TDR = *StrMsg;
        while (bMFS4_UART_SSR_TDRE == BIT_CLR) {}
        StrMsg++;
        MsgLen--;
    }

    while (bMFS4_UART_SSR_TDRE == BIT_CLR) {}

    return;
}
#endif  /* DEBUG_PRINT */
