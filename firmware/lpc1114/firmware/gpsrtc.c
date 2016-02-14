//=========================================================
// LPC1114 Project
//=========================================================
// File Name : gpsrtc.c
// Function  : GPS&RTC Control
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

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include "gpio.h"
#include "gpsrtc.h"
#include "i2c.h"
#include "systick.h"
#include "type.h"
#include "uart.h"
#include "utility.h"

//======================
// Time Zone Difference
//======================
#define TZD (+9) // Japan

//====================
// RTC Device Address
//====================
#define RTC_DEV_ADDR 0xa2

//=======================
// RTC Register Address
//=======================
#define RTC_CONTROL1 0x00
#define RTC_CONTROL2 0x01
#define RTC_SECONDS  0x02
#define RTC_MINUTES  0x03
#define RTC_HOURS    0x04
#define RTC_DAYS     0x05
#define RTC_WEEKDAYS 0x06
#define RTC_C_MONTHS 0x07
#define RTC_YEARS    0x08
#define RTC_MINUTE_ALARM  0x09
#define RTC_HOUR_ALARM    0x0a
#define RTC_DAY_ALARM     0x0b
#define RTC_WEEKDAY_ALARM 0x0c
#define RTC_CLKOUT_FREQ   0x0d
#define RTC_TIMER_CONTROL 0x0e
#define RTC_TIMER         0x0f

//============
// Globals
//============
extern volatile uint32_t I2CCount;
extern volatile uint8_t  I2CMasterBuffer[BUFSIZE];
extern volatile uint8_t  I2CSlaveBuffer[BUFSIZE];
extern volatile uint32_t I2CMasterState;
extern volatile uint32_t I2CReadLength, I2CWriteLength;
//
extern volatile uint32_t UARTCount;

//========================
// Get Number from GPS
//========================
uint8_t* Get_Number_from_GPS(uint8_t *pStr,
        int32_t *pInteger, int32_t *pIntrnd, int32_t *pDecimal, uint32_t *pDeclen)
{
    uint8_t  ch;
    int32_t  found_decimal;
    int32_t  pol;
    int32_t  decimal_1st;

    found_decimal = 0;
    *pInteger = 0;
    *pDecimal = 0;
    *pDeclen = 0;
    pol = 1;
    while ((ch = *pStr++) != ',')
    {
        if (ch == '.')
        {
            found_decimal = 1;
        }
        else if (ch == '-')
        {
            pol = -1;
        }
        else
        {
            if (found_decimal == 0)
            {
                *pInteger = (*pInteger) * 10 + (ch - '0');
            }
            else
            {
                *pDecimal = (*pDecimal) * 10 + (ch - '0');
                *pDeclen = *pDeclen + 1;
            }
        }
    }
    decimal_1st = (*pDeclen > 0)? *pDecimal / power(10, *pDeclen - 1) : 0;
    *pIntrnd = (decimal_1st < 5)? *pInteger : *pInteger + 1;
    //
    *pInteger = *pInteger * pol;
    *pIntrnd  = *pIntrnd  * pol;
    *pDecimal = *pDecimal * pol;
    return pStr;
}

//===========================
// Get_GPGGA Data from GPS
//===========================
void Get_GPGGA_Data(sGPSRTC *pG)
{
    uint8_t  ch;
    uint8_t  str[256];
    uint8_t  *pStr;
    uint32_t quit = 0;
    int32_t  integer;
    int32_t  intrnd;
    int32_t  decimal;
    uint32_t declen;

    //-------------------------
    // Get String after $GPGGA
    //-------------------------
    while(quit == 0)
    {
        //--------------------------
        // Retry from 1st String
        //--------------------------
        while(quit == 0)
        {
            //----------------
            // Check "$GPGGA,"
            //----------------
            if (UARTReceive_Byte() != '$') break;
            if (UARTReceive_Byte() != 'G') break;
            if (UARTReceive_Byte() != 'P') break;
            if (UARTReceive_Byte() != 'G') break;
            if (UARTReceive_Byte() != 'G') break;
            if (UARTReceive_Byte() != 'A') break;
            if (UARTReceive_Byte() != ',') break;
            //-----------------
            // Get String
            //-----------------
            pStr = str;
            while ((ch = UARTReceive_Byte()) != '\r') // LF
            {
                *pStr++ = ch;
            }
            quit = 1;
        }
    }
    pStr = str;
    //-------------
    // UTC
    //-------------
    pStr = Get_Number_from_GPS(pStr, &integer, &intrnd, &decimal, &declen);
    pG->bGPS_UTC_hour = (uint8_t) (integer / 10000);
    pG->bGPS_UTC_min  = (uint8_t) ((integer % 10000) / 100);
    pG->bGPS_UTC_sec  = (uint8_t) (integer % 100);
    //---------------
    // Latitude
    //---------------
    pStr = Get_Number_from_GPS(pStr, &integer, &intrnd, &decimal, &declen);
    pG->bGPS_LAT_deg = (uint8_t) (integer / 100);
    pG->bGPS_LAT_min = (uint8_t) (integer % 100);
    pG->bGPS_LAT_sec = (uint8_t) ((60 * decimal) / power(10, declen));
    pG->cGPS_LAT = (*pStr != ',')?  *pStr++ : ' ';
    ch = *pStr++; // ','
    //---------------
    // Longitude
    //---------------
    pStr = Get_Number_from_GPS(pStr, &integer, &intrnd, &decimal, &declen);
    pG->bGPS_LNG_deg = (uint8_t) (integer / 100);
    pG->bGPS_LNG_min = (uint8_t) (integer % 100);
    pG->bGPS_LNG_sec = (uint8_t) ((60 * decimal) / power(10, declen));
    pG->cGPS_LNG = (*pStr != ',')?  *pStr++ : ' ';
    ch = *pStr++; // ','
    //--------------
    // GPS Quality
    //--------------
    pG->cGPS_Quality = *pStr++;
    ch = *pStr++; // ','
    //-----------------
    // Satellite Count
    //-----------------
    pStr = Get_Number_from_GPS(pStr, &integer, &intrnd, &decimal, &declen);
    pG->bGPS_Sat = (uint8_t) integer;
    //-----------------
    // HDOP
    //-----------------
    pStr = Get_Number_from_GPS(pStr, &integer, &intrnd, &decimal, &declen);
    pG->bGPS_HDOP_I = (uint8_t) integer;
    pG->bGPS_HDOP_D = (uint8_t) ((decimal + power(10, declen) / 2) / power(10, declen));
    //-------------------------
    // Altitude above Sea Level
    //-------------------------
    pStr = Get_Number_from_GPS(pStr, &integer, &intrnd, &decimal, &declen);
    pG->wGPS_ASL_m = intrnd;
    ch = *pStr++; // 'M'
    ch = *pStr++; // ','
    //-------------------------
    // Geoid Separation
    //-------------------------
    pStr = Get_Number_from_GPS(pStr, &integer, &intrnd, &decimal, &declen);
    pG->wGPS_GEO_m = intrnd;
    ch = *pStr++; // 'M'
    ch = *pStr++; // ','
}

//==================
// Initialize RTC
//==================
void Init_RTC(uint32_t do_adj, uint8_t year, uint8_t month,  uint8_t day,
              uint8_t week,    uint8_t hour, uint8_t minute, uint8_t second)
{
    // Initialize I2C
    if (I2CInit((uint32_t) I2CMASTER) == FALSE )
    {
      while (1); // Error Trap
    }

    // Initialize RTC
    if (do_adj)
    {
        Wait_N_Ticks(100); // wait 1000ms
        //
        RTC_Write_Reg(RTC_CONTROL1, 0x20); // STOP
        RTC_Write_Reg(RTC_CONTROL2, 0x00);
        //
        RTC_Write_Reg(RTC_HOURS,   BCD_INT(hour));
        RTC_Write_Reg(RTC_MINUTES, BCD_INT(minute));
        RTC_Write_Reg(RTC_SECONDS, BCD_INT(second));
        //
        RTC_Write_Reg(RTC_YEARS,    BCD_INT(year));
        RTC_Write_Reg(RTC_C_MONTHS, BCD_INT(month));
        RTC_Write_Reg(RTC_DAYS,     BCD_INT(day));
        RTC_Write_Reg(RTC_WEEKDAYS, BCD_INT(week));
        //
        RTC_Write_Reg(RTC_MINUTE_ALARM,  0x00);
        RTC_Write_Reg(RTC_HOUR_ALARM,    0x00);
        RTC_Write_Reg(RTC_DAY_ALARM,     0x00);
        RTC_Write_Reg(RTC_WEEKDAY_ALARM, 0x00);
        //
        RTC_Write_Reg(RTC_CLKOUT_FREQ,  0x00);
        RTC_Write_Reg(RTC_TIMER_CONTROL,0x00);
        RTC_Write_Reg(RTC_TIMER,        0x00);
        //
        RTC_Write_Reg(RTC_CONTROL1, 0x00); // START
    }
}

//====================
// Get RTC Data
//====================
void Get_RTC_Data(sGPSRTC *psGPSRTC)
{
    psGPSRTC->bRTC_year = INT_BCD(RTC_Read_Reg(RTC_YEARS));
    psGPSRTC->bRTC_mon  = INT_BCD(RTC_Read_Reg(RTC_C_MONTHS) & 0x1f);
    psGPSRTC->bRTC_day  = INT_BCD(RTC_Read_Reg(RTC_DAYS) & 0x3f);
    psGPSRTC->bRTC_week = RTC_Read_Reg(RTC_WEEKDAYS) & 0x07;
    psGPSRTC->bRTC_hour = INT_BCD(RTC_Read_Reg(RTC_HOURS) & 0x3f);
    psGPSRTC->bRTC_min  = INT_BCD(RTC_Read_Reg(RTC_MINUTES) & 0x7f);
    psGPSRTC->bRTC_sec  = INT_BCD(RTC_Read_Reg(RTC_SECONDS) & 0x7f);
}

//===================
// Get Week String
//===================
uint8_t *Get_Week_String(uint32_t week)
{
    static const char *WEEK[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

    return (uint8_t*) WEEK[week];
}

//=====================
// RTC Write Register
//=====================
void RTC_Write_Reg(uint32_t addr, uint32_t data)
{
    I2CWriteLength = 3;
    I2CReadLength = 0;
    I2CMasterBuffer[0] = RTC_DEV_ADDR;
    I2CMasterBuffer[1] = addr;
    I2CMasterBuffer[2] = data;
    I2CEngine();
}

//====================
// RTC Read Register
//====================
uint32_t RTC_Read_Reg(uint32_t addr)
{
    I2CWriteLength = 2;
    I2CReadLength = 1;
    I2CMasterBuffer[0] = RTC_DEV_ADDR;
    I2CMasterBuffer[1] = addr;
    I2CMasterBuffer[2] = RTC_DEV_ADDR | RD_BIT ;
    I2CEngine();
    //
    return I2CSlaveBuffer[0];
}

//=========================================================
// End of Program
//=========================================================
