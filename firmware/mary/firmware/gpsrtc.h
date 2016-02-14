//=========================================================
// LPC1114 Project
//=========================================================
// File Name : gpsrtc.h
// Function  : GPS&RTC Control Header
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

#ifndef __GPSRTC_H__
#define __GPSRTC_H__

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

//=============
// GPSRTC Data
//=============
typedef struct
{
    uint8_t  bGPS_UTC_hour; // UTC hour
    uint8_t  bGPS_UTC_min;  // UTC minute
    uint8_t  bGPS_UTC_sec;  // UTC second
    uint8_t  cGPS_LAT;      // Latitude N or S
    uint8_t  bGPS_LAT_deg;  // Latitude degree
    uint8_t  bGPS_LAT_min;  // Latitude minute
    uint8_t  bGPS_LAT_sec;  // Latitude second
    uint8_t  cGPS_LNG;      // Longitude E or W
    uint8_t  bGPS_LNG_deg;  // Longitude degree
    uint8_t  bGPS_LNG_min;  // Longitude minute
    uint8_t  bGPS_LNG_sec;  // Longitude second
    uint8_t  cGPS_Quality;  // GPS Quality (0:Invalid, 1: SPS, 2:D-GPS)
    uint8_t  bGPS_Sat;      // Satellite Count
    uint8_t  bGPS_HDOP_I;   // Horizontal Dilution of Precision, Integer
    uint8_t  bGPS_HDOP_D;   // Horizontal Dilution of Precision, Decimal
    int32_t  wGPS_ASL_m;    // Altitude above Sea Level in meter
    int32_t  wGPS_GEO_m;    // Geoid Separation in meter
    //
    uint8_t  bRTC_year; // RTC year
    uint8_t  bRTC_mon;  // RTC month
    uint8_t  bRTC_day;  // RTC day
    uint8_t  bRTC_week; // RTC week
    uint8_t  bRTC_hour; // RTC hour
    uint8_t  bRTC_min;  // RTC minute
    uint8_t  bRTC_sec;  // RTC second
} sGPSRTC;

//===================
// Define Weekdays
//===================
enum WEEK {SUN = 0, MON, TUE, WED, THU, FRI, SAT};

//=============
// Prototypes
//=============
uint8_t* Get_Number_from_GPS(uint8_t *pStr,
        int32_t *pInteger, int32_t *pIntrnd, int32_t *pDecimal, uint32_t *pDeclen);
void Get_GPGGA_Data(sGPSRTC *pG);
//
void Init_RTC(uint32_t do_adj, uint8_t year, uint8_t month,  uint8_t day,
              uint8_t week,    uint8_t hour, uint8_t minute, uint8_t second);
void Get_RTC_Data(sGPSRTC *psGPSRTC);
uint8_t *Get_Week_String(uint32_t week);
//
void RTC_Write_Reg(uint32_t addr, uint32_t data);
uint32_t RTC_Read_Reg(uint32_t addr);

#endif // __GPSRTC_H__

//=========================================================
// End of Program
//=========================================================
