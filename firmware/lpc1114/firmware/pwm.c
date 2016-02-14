//=========================================================
// LPC1114 Project
//=========================================================
// File Name : pwm.c
// Function  : PWM(Timer32B1) Control
//---------------------------------------------------------
// Rev.01 2010.09.10 Munetomo Maruyama
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

#include "gpio.h"
#include "pwm.h"

#define MIN_PWM 0
#define MAX_PWM 999
#define MAX_FREQ 24000
#define SOUND_100ms 4800

//==============
// Globals
//==============
volatile uint32_t gSoundFreq;    // unit: Hz
volatile uint32_t gSoundLength;  // unit: 20.83us (if 0.1sec, set 4800)
volatile uint32_t gDuty0;
volatile uint32_t gDuty1;

//===================
// Initialize PWM
//===================
void Init_PWM(void)
{
    // Sound Off
    gSoundFreq   = 0;
    gSoundLength = 0;
    gDuty0 = 0;
    gDuty1 = 0;

    // MAT0, MAT1 output
    LPC_IOCON->R_PIO1_1   = 0xc3; // MAT0 (AOUT0)
    LPC_IOCON->R_PIO1_2 = 0xc3; // MAT1 (AOUT1)

    // Sounder output
    LPC_IOCON->PIO1_4 = 0x00000000; // GPIO, disable pu/pd mos
    GPIOSetDir(1, 4, 1);   // PIO1_4 out
    GPIOSetValue(1, 4, 0); // low

    // Timer32B1 Clock ON (48MHz)
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);

    // Carrier Cycle = 20.83us (48KHz)
    // Timer Count = 0 ~ 999
    // Count Cycle = 20.83ns
    LPC_TMR32B1->PR  = 0;   // Pre-Scaler
    LPC_TMR32B1->MR2 = MAX_PWM; // Interval

    // Interval Interrupt Operation
    LPC_TMR32B1->MCR = (0x3 << 6); // clear at MR2 and Interrupt

    // PWM Control
    LPC_TMR32B1->PWMC = 0b1011; // PWMs are on MAT3, MAT1 and MAT0
    LPC_TMR32B1->MR0 = MIN_PWM; // Duty
    LPC_TMR32B1->MR1 = MIN_PWM; // Duty
    LPC_TMR32B1->MR3 = MIN_PWM; // Duty

    // Enable Interrupt
    NVIC_EnableIRQ(TIMER_32_1_IRQn);
    NVIC_SetPriority(TIMER_32_1_IRQn, 0); // highest

    // Start Timer
    LPC_TMR32B1->TCR = 1;
}

//============================
// Timer32B1 Interrupt Handler
//============================
void TIMER32_1_IRQHandler(void)
{
    if (LPC_TMR32B1->IR & (1 << 2)) // MR2 Interrupt
    {
        // Set Duty
        LPC_TMR32B1->MR0 = gDuty0;
        LPC_TMR32B1->MR1 = gDuty1;

        // Generate Sound
        Generate_Sound();

        // Clear Flag
        LPC_TMR32B1->IR = (1 << 2);
    }
}

//======================
// Set PWM Duty (0-999)
//======================
void PWM_Set_Duty(uint32_t ch, uint32_t duty)
{
    duty = (duty > MAX_PWM)? MAX_PWM : duty;
    if (ch == 0) gDuty0 = duty;
    if (ch == 1) gDuty1 = duty;
}

//====================
// Generate Sound
//====================
// 48kHz |-----|-----|-----|-----|
// 24KHz  00000 11111 00000 11111
// 12kHz  00000 00000 11111 11111
void Generate_Sound(void)
{
    static uint32_t freq = 0;

    if (gSoundLength == 0)
    {
        GPIOSetValue(1, 4, 0); // low
        freq = 0;
    }
    else if (gSoundFreq > MAX_FREQ)
    {
        GPIOSetValue(1, 4, 0); // low
        freq = 0;
    }
    else if (freq < (MAX_FREQ >> 1))
    {
        GPIOSetValue(1, 4, 0); // low
    }
    else
    {
        GPIOSetValue(1, 4, 1); // high
    }
    //
    freq = freq + (gSoundFreq >> 1);
    if (freq > MAX_FREQ) freq = 0;
    //
    if (gSoundLength > 0) gSoundLength--;
}

//================
// Play Sound
//================
void Play_Sound(uint32_t freqHz, uint32_t length100ms)
{
    gSoundFreq = freqHz;
    gSoundLength = length100ms * SOUND_100ms;
}

//=========================================================
// End of Program
//=========================================================
