//=========================================================
// LPC1114 Project
//=========================================================
// File Name : led_array.c
// Function  : LED Array Control
//---------------------------------------------------------
// Rev.01 2010.08.07 Munetomo Maruyama
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
#include "led_array.h"
#include "spi.h"
#include "systick.h"
#include "timer16.h"

//====================
// Global : LED Array
//====================
// for each element
//   bit1 : Green
//   bit0 : Red
uint32_t gLED_ARRAY[8][8];

//======================
// Initialize LED Array
//======================
void Init_LED_Array(void)
{
    uint32_t x, y;

    // PIO1_8 : /SRCLR GPIO OUT
    // PIO1_9 : RCLK   GPIO OUT
    LPC_IOCON->PIO1_8 = 0x00000000; // GPIO, disable pu/pd mos
    LPC_IOCON->PIO1_9 = 0x00000000; // GPIO, disable pu/pd mos
    GPIOSetDir(1, 8, 1); // PIO1_8 out
    GPIOSetDir(1, 9, 1); // PIO1_9 out
    GPIOSetValue(1, 8, 0); // low
    GPIOSetValue(1, 9, 0); // low
    Wait_N_Ticks(10); // wait for 100ms
    GPIOSetValue(1, 8, 1); // high (reset)

    // Initialize Hardware
    Init_SPI(8, SPI_FAST, SPI_GENERIC); // bit length = 8bit
    Init_Timer16();

    // Clear LED Array Buffer
    for (y = 0; y < 8; y++)
    {
        for (x = 0; x < 8; x++)
        {
            gLED_ARRAY[y][x] = 0;
        }
    }
}

//==================
// Scan LED Array
//==================
// Caller of TIMER16_0_IRQHandler()
void Scan_LED_Array(void)
{
    static uint32_t x = 0;
    uint32_t y;
    uint32_t row;
    uint32_t col_LED_ARRAY_RED;
    uint32_t col_LED_ARRAY_GRN;

    row = 1 << x;
    //
    col_LED_ARRAY_RED = 0;
    for (y = 0; y < 8; y++)
    {
        col_LED_ARRAY_RED = col_LED_ARRAY_RED | ((gLED_ARRAY[y][x] & 0x01) << (7 - y));
    }
    //
    col_LED_ARRAY_GRN = 0;
    for (y = 0; y < 8; y++)
    {
        col_LED_ARRAY_GRN = col_LED_ARRAY_GRN | (((gLED_ARRAY[y][x] & 0x02) >> 1) << (7 - y));
    }
    //
    SPI_TxData(row);
    SPI_TxData(col_LED_ARRAY_RED);
    SPI_TxData(col_LED_ARRAY_GRN);
    //
    GPIOSetValue(1, 9, 1); // high
    GPIOSetValue(1, 9, 0); // low (latch)
    //
    x = (x + 1) & 0x07;
}

//===================
// Set LED Array
//===================
void Set_LED_Array(uint32_t x, uint32_t y, uint32_t color)
{
    gLED_ARRAY[y][x] = color;
}

//===================
// Get LED Array
//===================
uint32_t Get_LED_Array(uint32_t x, uint32_t y)
{
    return gLED_ARRAY[y][x];
}

//=========================================================
// End of Program
//=========================================================
