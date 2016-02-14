//=========================================================
// LPC1114 Project
//=========================================================
// File Name : color_led.c
// Function  : Color LED Control
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

#include "color_led.h"
#include "gpio.h"
#include "systick.h"

//======================
// Initialize Color LED
//======================
void Init_Color_LED(void)
{
    //
    // Initialize LED Port
    //
    LPC_IOCON->PIO2_0 = 0x00000000; // GPIO, disable pu/pd mos
    LPC_IOCON->PIO0_7 = 0x00000000; // GPIO, disable pu/pd mos
    LPC_IOCON->PIO1_5 = 0x00000000; // GPIO, disable pu/pd mos
    Set_RED_LED(0);
    Set_GRN_LED(0);
    Set_BLU_LED(0);
}

//=======================
// Set Color LED State
//=======================
// bit0 of color ; RED
// bit1 of color ; GRN
// bit2 of color ; BLU
void Set_Color_LED(uint32_t color)
{
    Set_RED_LED(color & 0x01);
    Set_GRN_LED(color & 0x02);
    Set_BLU_LED(color & 0x04);
}

//=======================
// Set RED LED State
//=======================
void Set_RED_LED(uint32_t state)
{
    if (state) // on
    {
        GPIOSetDir(2, 0, 1); // RED P2_0 out
        GPIOSetValue(2, 0, 0); // low (value should be set after fixing direction)
    }
    else // off
    {
        GPIOSetDir(2, 0, 0); // RED P2_0 in
        GPIOSetValue(2, 0, 1); // high
    }
}

//=======================
// Set GRN LED State
//=======================
void Set_GRN_LED(uint32_t state)
{
    if (state) // on
    {
        GPIOSetDir(0, 7, 1); // RED P2_0 out
        GPIOSetValue(0, 7, 0); // low
    }
    else // off
    {
        GPIOSetDir(0, 7, 0); // RED P2_0 in
        GPIOSetValue(0, 7, 1); // high
    }
}

//=======================
// Set BLU LED State
//=======================
void Set_BLU_LED(uint32_t state)
{
    if (state) // on
    {
        GPIOSetDir(1, 5, 1); // RED P2_0 out
        GPIOSetValue(1, 5, 0); // low
    }
    else // off
    {
        GPIOSetDir(1, 5, 0); // RED P2_0 in
        GPIOSetValue(1, 5, 1); // high
    }
}

//==================
// Draw Color LED
//==================
void Draw_Color_LED(void)
{
    uint32_t tick_curr;
    uint32_t led;

    tick_curr = Get_Ticks();
    led = (tick_curr >> 6) & 0x07;
    Set_Color_LED(led);
}

//=========================================================
// End of Program
//=========================================================
