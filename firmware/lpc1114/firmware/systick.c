//=========================================================
// LPC1114 Project
//=========================================================
// File Name : systick.c
// Function  : SysTick Control
//---------------------------------------------------------
// Rev.01 2010.08.01 Munetomo Maruyama
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

#include "key.h"
#include "systick.h"

//===============
// Globals
//===============
volatile uint32_t gTicks = 0; //counts 1ms timeTicks
volatile uint32_t gTicks_update = 0;
volatile uint32_t gTicks_new;

//======================
// Initialize SysTick
//======================
void Init_SysTick(void)
{
    // Setup SysTick Timer for 10 msec interrupts
    if (SysTick_Config(SystemCoreClock / 100))
    {
        while (1); // Error
    }
}

//======================
// SysTick Handler
//======================
void SysTick_Handler(void)
{
    if (gTicks_update)
    {
        gTicks = gTicks_new;
        gTicks_update = 0;
    }
    else
    {
        gTicks++;
    }
    //
    // Key Detection Control
    Detect_Key_Push();
}

//==================
// Read Ticks
//==================
uint32_t Get_Ticks(void)
{
    return gTicks;
}

//==================
// Set Ticks
//==================
void Set_Ticks(uint32_t new_ticks)
{
    gTicks_new = new_ticks;
    gTicks_update = 1;
}

//==================
// Wait for N Ticks
//==================
void Wait_N_Ticks(uint32_t wait)
{
    uint32_t tick_curr;
    uint32_t tick_prev;
    uint32_t tick_intv;

    tick_prev = Get_Ticks();
    while(1)
    {
        tick_curr = Get_Ticks();
        tick_intv = tick_curr - tick_prev;
        if (tick_intv > wait) break;
    }
}

//======================
// Wait New Time Slot
//======================
uint32_t Wait_New_Timeslot(uint32_t cycle)
{
    // Ticks     0 1 2 3 4 5 6 7 8 9 A B C D E F ...
    // cycle=1     0 1 2 3 4 5 6 7 8 9 A B C D E ...
    // cycle=2       0 . 1 . 2 . 3 . 4 . 5 . 6 . ...
    // cycle=3         0 . . 1 . . 2 . . 3 . . 4 ...

    uint32_t tick_prev;
    uint32_t tick_curr;
    uint32_t timeslot;

    cycle = (cycle == 0)? 1 : cycle;

    tick_prev = Get_Ticks();
    timeslot = tick_prev / cycle;
    tick_prev = timeslot * cycle;

    while(1)
    {
        tick_curr = Get_Ticks();
        if (tick_curr >= (tick_prev + cycle)) break;
    }
    return timeslot;
}

//=========================================================
// End of Program
//=========================================================
