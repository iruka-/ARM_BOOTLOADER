//=========================================================
// LPC1114 Project
//=========================================================
// File Name : timer16.c
// Function  : Timer16 Control
//---------------------------------------------------------
// Rev.01 2010.08.07 Munetomo Maruyama
//---------------------------------------------------------
// Copyright (C) 2010-2011 Munetomo Maruyama
//=========================================================
// ---- License Information -------------------------------
// Anyone can FREELY use this code fully or partially
// under conditions shown below.
// 1. You may use this code only for individual purpose,
//    and educational purpose.
//    Do not use this code for business even if partially.
// 2. You can copy, modify and distribute this code.
// 3. You should remain this header text in your codes
//   including Copyright credit and License Information.
// 4. Your codes should inherit this license information.
//=========================================================
// ---- Patent Notice -------------------------------------
// I have not cared whether this system (hw + sw) causes
// infringement on the patent, copyright, trademark,
// or trade secret rights of others. You have all
// responsibilities for determining if your designs
// and products infringe on the intellectual property
// rights of others, when you use technical information
// included in this system for your business.
//=========================================================
// ---- Disclaimers ---------------------------------------
// The function and reliability of this system are not
// guaranteed. They may cause any damages to loss of
// properties, data, money, profits, life, or business.
// By adopting this system even partially, you assume
// all responsibility for its use.
//=========================================================

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include "led_array.h"
#include "timer16.h"

//====================
// Initialize Timer16
//====================
void Init_Timer16(void)
{
    // Timer16 Clock ON (48MHz)
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);

    // Interval = 8ms/8= 1ms (1kHz = 48MHz / 48000)
    LPC_TMR16B0->PR  = 10 - 1;   // Pre-Scaler
    LPC_TMR16B0->MR0 = 4800;     // Interval

    // Interval Interrupt Operation
    LPC_TMR16B0->MCR = 3;

    // Enable Interrupt
    NVIC_EnableIRQ(TIMER_16_0_IRQn);
    NVIC_SetPriority(TIMER_16_0_IRQn, 3);

    // Start Timer
    LPC_TMR16B0->TCR = 1;
}

//===========================
// Timer16 Interrupt Handler
//===========================
void TIMER16_0_IRQHandler(void)
{
    if (LPC_TMR16B0->IR & 0x1)
    {
        // Scan LED Array
        Scan_LED_Array();

        // Clear Flag
        LPC_TMR16B0->IR = 1;
    }
}

//=========================================================
// End of Program
//=========================================================
