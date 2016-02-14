//=========================================================
// LPC1114 Project
//=========================================================
// File Name : gpio.h
// Function  : GPIO Control Header
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

#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

//=======================================
// Define LPC_GPIO[4] (each top address)
//=======================================
static LPC_GPIO_TypeDef (* const LPC_GPIO[4]) = { LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3 };

//============
// Prototypes
//============
LPC_GPIO_TypeDef *GPIOPortAdr( uint32_t portNum );

void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir );
void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal );
uint32_t GPIOGetValue( uint32_t portNum, uint32_t bitPosi);
void GPIOSetIS( uint32_t portNum, uint32_t bitPosi, uint32_t is );
void GPIOSetIBE( uint32_t portNum, uint32_t bitPosi, uint32_t ibe );
void GPIOSetIEV( uint32_t portNum, uint32_t bitPosi, uint32_t ibv );
void GPIOSetIE( uint32_t portNum, uint32_t bitPosi, uint32_t ie );
uint32_t GPIOGetIMS( uint32_t portNum, uint32_t bitPosi );
void GPIOClearInt( uint32_t portNum, uint32_t bitPosi );

#endif // __GPIO_H__

//=========================================================
// End of Program
//=========================================================
