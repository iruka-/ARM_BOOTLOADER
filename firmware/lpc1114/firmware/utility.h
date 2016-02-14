//=========================================================
// LPC1114 Project
//=========================================================
// File Name : utiity.h
// Function  : Utility Routine Header
//---------------------------------------------------------
// Rev.01 2010.08.29 Munetomo Maruyama
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

#ifndef __UTILITY_H__
#define __UTILITY_H__

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include <stdarg.h>
#include "fixedpoint.h"

//=============
// Prototypes
//=============
int32_t power(int32_t x, int32_t n);
uint8_t BCD_INT(uint8_t num);
uint8_t INT_BCD(uint8_t bcd);
uint32_t xatoi(uint8_t **str, int32_t *data);
uint8_t *xitoa(uint8_t *str, uint32_t *pLength, int32_t value, int8_t radix, int8_t width);
uint8_t *xsnprintf(uint8_t *str, uint32_t length, const char *format, ...);
uint8_t *xvsnprintf(uint8_t *str, uint32_t length, const char *format, va_list ap);

uint8_t* Append_String_UI32 (uint32_t value, uint8_t *string, uint32_t radix, uint32_t length);
uint8_t* Append_String_SI32 (int32_t value, uint8_t *string, uint32_t radix, uint32_t length);
uint8_t* Append_String_Fixed(fix32_t fvalue, uint8_t *string, uint32_t format);

uint8_t* Append_String_UI08_Ndigit(uint32_t value, uint8_t *string, uint32_t digit);
uint8_t* Append_String_UI08_Ndigit(uint32_t value, uint8_t *string, uint32_t digit);
uint8_t* Append_String(uint8_t *pDst, uint8_t *pSrc);


#endif // __UTILITY_H__

//=========================================================
// End of Program
//=========================================================
