/**
* @file          dbg_print.h
* @brief         implementation of the debug print support function
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/
#ifdef DEBUG_PRINT
void DbgPrintMsg(char *msg);
void DbgPrintValue(char *msg,int val);
#else
#define DbgPrintMsg( msg )		{/*nothing*/}
#define DbgPrintValue(msg,val)	{/*nothing*/}
#endif

#if	0	//ndef __DBG_PRINT_INC__
#define __DBG_PRINT_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "usbfunc_demoapp_config.h"
#include <stdio.h>

#ifdef DEBUG_PRINT
/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variable Declaration                                                */
/*----------------------------------------------------------------------------*/
extern uint8_t g_DbgBuffer[256];

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/
/* send message via UART */
extern void SendMsg(uint8_t* StrMsg, uint16_t MsgLen);

/* macro for only print message */
#define DbgPrintMsg(a);                 {SendMsg((uint8_t*)(a), strlen((const char * )a));}
/* macro for print message and data */
#define DbgPrintValue(a, b)             {sprintf((char*)g_DbgBuffer, a, b); DbgPrintMsg(g_DbgBuffer);}

#else

/* dummy */
#define DbgPrintMsg(a);                 {;}
#define DbgPrintValue(a,b)              {;}

#endif  /* DEBUG_PRINT */

#endif /* __DBG_PRINT_INC__ */

