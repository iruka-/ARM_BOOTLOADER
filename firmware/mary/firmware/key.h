//=========================================================
// LPC1114 Project
//=========================================================
// File Name : key.h
// Function  : Key/Switch Control Header
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

#ifndef __KEY_H__
#define __KEY_H__

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

//===================
// Key Switch Name
//===================
#define MAX_KEY 6
enum KEY_NAME {KEY_UP, KEY_DN, KEY_LT, KEY_RT, KEY_CT, KEY_SW};

//===============
// Prototypes
//===============
void Init_Key(void);
uint8_t Check_Key_Status(uint8_t key);
uint8_t Check_Key_Pushed(uint8_t key);
//
void Detect_Key_Push(void);

#endif // __KEY_H__

//=========================================================
// End of Program
//=========================================================
