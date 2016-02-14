//=========================================================
// LPC1114 Project
//=========================================================
// File Name : adc.c
// Function  : ADC Control
//---------------------------------------------------------
// Rev.01 2010.09.11 Munetomo Maruyama
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
#include "adc.h"

//====================
// Initialize ADC
//====================
void Init_ADC(void)
{
    // AD0, AD1, AD7
    LPC_IOCON->R_PIO0_11 = 0x42; // AD0
    LPC_IOCON->R_PIO1_0  = 0x42; // AD1
    LPC_IOCON->PIO1_11 = 0x41; // AD7

    // Disable Power down bit to the ADC block
    LPC_SYSCON->PDRUNCFG &= ~(0x1<<4);
    // ADC Clock ON (48MHz)
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<13);

    // Burst Start, 4.3MHz, AD0, AD1, AD7
    LPC_ADC->CR = (1 << 16) + ((11 -1) << 8) + (0b10000011);
}

//================
// Get ADC Data
//================
uint32_t Get_ADC_Data(uint32_t channel)
{
    uint32_t data;

    while (((data = LPC_ADC->DR[channel]) & 0x80000000) == 0);
    data = (data >> 6) & 0x3ff;
    return data;
}

//=========================================================
// End of Program
//=========================================================
