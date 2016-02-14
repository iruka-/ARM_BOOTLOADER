//=========================================================
// LPC1114 Project
//=========================================================
// File Name : pwm.h
// Function  : PWM(Timer32B1) Control Header
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

#ifndef __PWM_H__
#define __PWM_H__

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

//============
// Prototypes
//============
void Init_PWM(void);
//
void TIMER32_1_IRQHandler(void);
void PWM_Set_Duty(uint32_t ch, uint32_t duty);
void Generate_Sound(void);
void Play_Sound(uint32_t freqHz, uint32_t length100ms);

#endif // __PWM_H__

//=========================================================
// End of Program
//=========================================================
