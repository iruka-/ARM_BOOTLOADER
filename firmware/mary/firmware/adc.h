//=========================================================
// LPC1114 Project
//=========================================================
// File Name : adc.h
// Function  : ADC Control Header
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

#ifndef __ADC_H__
#define __ADC_H__

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

//====================
// A/D Input Channels
//====================
#define ADC_LIN 0
#define ADC_RIN 1
#define ADC_MIC 7

//============
// Prototypes
//============
void Init_ADC(void);
uint32_t Get_ADC_Data(uint32_t channel);

#endif // __ADC_H__

//=========================================================
// End of Program
//=========================================================
