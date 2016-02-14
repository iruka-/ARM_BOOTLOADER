//=========================================================
// LPC1114 Project
//=========================================================
// File Name : mems.h
// Function  : MEMS Accelerometer Control Header
//---------------------------------------------------------
// Rev.01 2010.08.13 Munetomo Maruyama
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

#ifndef __MEMS_H__
#define __MEMS_H__

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

//=============
// Prototypes
//=============
void Init_MEMS(void);
void MEMS_Write_Reg(uint32_t addr, uint32_t data);
uint32_t MEMS_Read_Reg(uint32_t addr);
int8_t MEMS_Get_X(void);
int8_t MEMS_Get_Y(void);
int8_t MEMS_Get_Z(void);

#endif // __MEMS_H__

//=========================================================
// End of Program
//=========================================================
