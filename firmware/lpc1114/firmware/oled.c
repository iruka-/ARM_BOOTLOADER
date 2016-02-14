//=========================================================
// LPC1114 Project
//=========================================================
// File Name : oled.c
// Function  : OLED Control
//---------------------------------------------------------
// Rev.01 2010.08.13 Munetomo Maruyama
// Rev.02 2011.03.20 Munetomo Maruyama
//        - Add Orientation Mode
//        - Add OLED_GRY (Color Definition)
//        - Correct OLED_printf_Color()
//        - Correct OLED SPI Clock (24MHz-->12MHz)
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

#include <stdarg.h>
#include "font.h"
#include "gpio.h"
#include "oled.h"
#include "spi.h"
#include "systick.h"
#include "utility.h"

//=================
// Font Parameters
//=================
#define OLED_FONT_XSIZE 8
#define OLED_FONT_YSIZE 8

//======================
// Define OLED Commands
//======================
#define OLED_COMMAND 0
#define OLED_DATA    1
//
#define C_SET_COLUMN_ADDRESS 0x0215
#define C_SET_ROW_ADDRESS    0x0275
#define D_START_ADDRESS 1
#define D_END_ADDRESS   2
//
#define C_WRITE_RAM_COMMAND 0x005c
#define C_READ_RAM_COMMAND  0x005d
//
#define C_SET_REMAP_COLOR_DEPTH 0x01a0
#define D_SET_REMAP_COLOR_DEPTH 1
//
#define C_SET_DISPLAY_START_LINE 0x01a1
#define D_SET_DISPLAY_START_LINE 1
//
#define C_SET_DISPLAY_OFFSET 0x01a2
#define D_SET_DISPLAY_OFFSET 1
//
#define C_SET_DISPLAY_MODE_ALL_OFF 0x00a4
#define C_SET_DISPLAY_MODE_ALL_ON  0x00a5
#define C_SET_DISPLAY_MODE_RESET   0x00a6
#define C_SET_DISPLAY_MODE_INVERSE 0x00a7
//
#define C_FUNCTION_SELECTION 0x01ab
#define D_FUNCTION_SELECTION 1
//
#define C_SET_SLEEP_MODE_ON  0x00ae
#define C_SET_SLEEP_MODE_OFF 0x00af
//
#define C_SET_RESET_PRECHARGE_PERIOD 0x01b1
#define D_SET_RESET_PRECHARGE_PERIOD 1
//
#define C_ENHANCE_DRIVING_SCHEME_CAPABILITY 0x03b2
#define D_ENHANCE_DRIVING_SCHEME_CAPABILITY_1 1
#define D_ENHANCE_DRIVING_SCHEME_CAPABILITY_2 2
#define D_ENHANCE_DRIVING_SCHEME_CAPABILITY_3 3
//
#define C_FRONT_CLOCK_DRIVER_OSCILLATOR_FREQUENCY 0x01b3
#define D_FRONT_CLOCK_DRIVER_OSCILLATOR_FREQUENCY 1
//
#define C_SET_SEGMENT_LOW_VOLTAGE 0x03b4
#define D_SET_SEGMENT_LOW_VOLTAGE_1 1 // 0xa0 or 0xa2
#define D_SET_SEGMENT_LOW_VOLTAGE_2 2 // 0xb5
#define D_SET_SEGMENT_LOW_VOLTAGE_3 3 // 0x55
//
#define C_SET_GPIO 0x01b5
#define D_SET_GPIO 1
//
#define C_SET_SECOND_PRECHARGE_PERIOD 0x01b6
#define D_SET_SECOND_PRECHARGE_PERIOD 1
//
#define C_LOOKUP_TABLE_FOR_GRAY_SCALE_PULSE_WIDTH 0x3fb8
static const uint8_t GAMMA_TABLE[63] =
{
    0x02, 0x03, 0x04, 0x05,
    0x06, 0x07, 0x08, 0x09,
    0x0a, 0x0b, 0x0c, 0x0d,
    0x0e, 0x0f, 0x10, 0x11,
    //
    0x12, 0x13, 0x15, 0x17,
    0x19, 0x1b, 0x1d, 0x1f,
    0x21, 0x23, 0x25, 0x27,
    0x2a, 0x2d, 0x30, 0x33,
    //
    0x36, 0x39, 0x3c, 0x3f,
    0x42, 0x45, 0x48, 0x4c,
    0x50, 0x54, 0x58, 0x5c,
    0x60, 0x64, 0x68, 0x6c,
    //
    0x70, 0x74, 0x78, 0x7d,
    0x82, 0x87, 0x8c, 0x91,
    0x96, 0x9b, 0xa0, 0xa5,
    0xaa, 0xaf, 0xb4
};
//
#define C_USE_BUILT_IN_LINEAR_LUT 0x00b9
//
#define C_SET_PRECHARGE_VOLTAGE 0x01bb
#define D_SET_PRECHARGE_VOLTAGE 1
//
#define C_SET_VCOMH_VOLTAGE 0x01be
#define D_SET_VCOMH_VOLTAGE 1
//
#define C_SET_CONTRAST_CURRENT_FOR_COLOR_ABC 0x03c1
#define D_SET_CONTRAST_CURRENT_FOR_COLOR_A 1
#define D_SET_CONTRAST_CURRENT_FOR_COLOR_B 2
#define D_SET_CONTRAST_CURRENT_FOR_COLOR_C 3
//
#define C_MASTER_CONTRAST_CURRENT_CONTROL 0x01c7
#define D_MASTER_CONTRAST_CURRENT_CONTROL 1
//
#define C_SET_MUX_RATIO 0x01ca
#define D_SET_MUX_RATIO 1
//
#define C_SET_COMMAND_LOCK 0x01fd
#define D_SET_COMMAND_LOCK 1
//
#define C_HORIZONTAL_SCROLL 0x0596
#define D_HORIZONTAL_SCROLL_A 1
#define D_HORIZONTAL_SCROLL_B 2
#define D_HORIZONTAL_SCROLL_C 3
#define D_HORIZONTAL_SCROLL_D 4
#define D_HORIZONTAL_SCROLL_E 5
//
#define C_STOP_MOVING  0x009e
#define C_START_MOVING 0x009f

//=============
// Globals
//=============
volatile uint32_t gOLED_printf_Font   = OLED_FONT_SMALL;
volatile uint32_t gOLED_printf_ColorF = OLED_WHT;
volatile uint32_t gOLED_printf_ColorB = OLED_BLK;
volatile uint32_t gOLED_printf_PosX = 0;
volatile uint32_t gOLED_printf_PosY = 0;
//
volatile uint32_t gOELD_Orientation_Mode = OLED_TOP_N;

//======================
// Initialize OLED
//======================
void Init_OLED(void)
{
    uint32_t i;
    uint32_t oled[64];

    // OLED Vcc OFF
    // PIO1_4 : OLED Vcc Power
    LPC_IOCON->PIO1_4 = 0x00000000; // GPIO, disable pu/pd mos
    GPIOSetValue(1, 4, 0); // low
    GPIOSetDir(1, 4, 1); // PIO1_4 out
    GPIOSetValue(1, 4, 0); // low

    // OLED Reset
    // PIO1_8 : OLED_/RES GPIO OUT
    LPC_IOCON->PIO1_8 = 0x00000000; // GPIO, disable pu/pd mos
    GPIOSetDir(1, 8, 1); // PIO1_8 out
    GPIOSetValue(1, 8, 0); // low
    Wait_N_Ticks(10); // wait for 100ms
    GPIOSetValue(1, 8, 1); // high (reset)

    // Initialize Hardware
    Init_SPI(9, SPI_FAST, SPI_GENERIC); // bit length = 9bit
    // Re-Adjust SPI Clock to 12MHz for OLED Module
    // Bit Frequency = 12MHz, PCLK=48MHz
    //     CPSDVSR*(SCR+1)=4, SCR=0, CPSR=4
    LPC_SSP0->CPSR = 4;

    // Display OFF
    oled[OLED_COMMAND] = C_SET_DISPLAY_MODE_ALL_OFF;
    OLED_Send_Command(oled);
    //
    // Initialization Sequence of OLED
    //
    oled[OLED_COMMAND] = C_SET_COMMAND_LOCK;
    oled[D_SET_COMMAND_LOCK] = 0x12; // unlock
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_COMMAND_LOCK;
    oled[D_SET_COMMAND_LOCK] = 0xb1; // unlock
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_SLEEP_MODE_ON;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_FRONT_CLOCK_DRIVER_OSCILLATOR_FREQUENCY;
    oled[D_FRONT_CLOCK_DRIVER_OSCILLATOR_FREQUENCY] = 0xf1;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_MUX_RATIO;
    oled[D_SET_MUX_RATIO] = 0x7f;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_DISPLAY_OFFSET;
    oled[D_SET_DISPLAY_OFFSET] = 0x00;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_DISPLAY_START_LINE;
    oled[D_SET_DISPLAY_START_LINE] = 0x00;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_REMAP_COLOR_DEPTH;
    oled[D_SET_REMAP_COLOR_DEPTH] = 0x74; // 64k colors
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_GPIO;
    oled[D_SET_GPIO] = 0x00;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_FUNCTION_SELECTION;
    oled[D_FUNCTION_SELECTION] = 0x01;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_SEGMENT_LOW_VOLTAGE;
    oled[D_SET_SEGMENT_LOW_VOLTAGE_1] = 0xa0; // use external VSL
    oled[D_SET_SEGMENT_LOW_VOLTAGE_2] = 0xb5;
    oled[D_SET_SEGMENT_LOW_VOLTAGE_3] = 0x55;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_CONTRAST_CURRENT_FOR_COLOR_ABC;
    oled[D_SET_CONTRAST_CURRENT_FOR_COLOR_A] = 0xc8;
    oled[D_SET_CONTRAST_CURRENT_FOR_COLOR_B] = 0x80;
    oled[D_SET_CONTRAST_CURRENT_FOR_COLOR_C] = 0xc8;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_MASTER_CONTRAST_CURRENT_CONTROL;
    oled[D_MASTER_CONTRAST_CURRENT_CONTROL] = 0x0f;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_LOOKUP_TABLE_FOR_GRAY_SCALE_PULSE_WIDTH;
    for (i = 1; i < 64; i++)
    {
        oled[i] = (uint32_t) GAMMA_TABLE[i - 1];
    }
    OLED_Send_Command(oled);
    //
  //oled[OLED_COMMAND] = C_USE_BUILT_IN_LINEAR_LUT;
  //OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_RESET_PRECHARGE_PERIOD;
    oled[D_SET_RESET_PRECHARGE_PERIOD] = 0x32;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_ENHANCE_DRIVING_SCHEME_CAPABILITY;
    oled[D_ENHANCE_DRIVING_SCHEME_CAPABILITY_1] = 0xa4;
    oled[D_ENHANCE_DRIVING_SCHEME_CAPABILITY_2] = 0x00;
    oled[D_ENHANCE_DRIVING_SCHEME_CAPABILITY_3] = 0x00;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_PRECHARGE_VOLTAGE;
    oled[D_SET_PRECHARGE_VOLTAGE] = 0x17;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_SECOND_PRECHARGE_PERIOD;
    oled[D_SET_SECOND_PRECHARGE_PERIOD] = 0x01;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_VCOMH_VOLTAGE;
    oled[D_SET_VCOMH_VOLTAGE] = 0x05;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_DISPLAY_MODE_RESET;
    OLED_Send_Command(oled);
    //
    // Clear Screen
    oled[OLED_COMMAND] = C_SET_COLUMN_ADDRESS;
    oled[D_START_ADDRESS] = 0;
    oled[D_END_ADDRESS]   = 127;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_SET_ROW_ADDRESS;
    oled[D_START_ADDRESS] = 0;
    oled[D_END_ADDRESS]   = 127;
    OLED_Send_Command(oled);
    //
    oled[OLED_COMMAND] = C_WRITE_RAM_COMMAND;
    OLED_Send_Command(oled);
    //
    for (i = 0; i < (128 * 128); i++)
    {
        OLED_Send_Pixel(OLED_BLK);
    }
    // OLED Vcc ON
    GPIOSetValue(1, 4, 1); // high
    Wait_N_Ticks(20); // wait for 200ms
    //
    // Display ON
    oled[OLED_COMMAND] = C_SET_SLEEP_MODE_OFF;
    OLED_Send_Command(oled);
    //
    // Dummy Print (to make correct link)
    OLED_printf(" ");
    OLED_printf_Position(0, 0);
}

//==================================
// Initialize OLED with Orientation
//==================================
void Init_OLED_with_Orientation(uint32_t mode)
{
    uint8_t command;
    uint32_t oled[64];

    Init_OLED();
    gOELD_Orientation_Mode = mode;
    //
    command = (mode == OLED_TOP_W)? 0x65 :
              (mode == OLED_TOP_S)? 0x66 :
              (mode == OLED_TOP_E)? 0x77 : 0x74;
    oled[OLED_COMMAND] = C_SET_REMAP_COLOR_DEPTH;
    oled[D_SET_REMAP_COLOR_DEPTH] = command;
    OLED_Send_Command(oled);
}

//=====================
// OLED Send Command
//=====================
void OLED_Send_Command(uint32_t *oled)
{
    uint32_t i;
    uint32_t count;
    uint32_t command;
    uint32_t data;

    count = (oled[OLED_COMMAND] >> 8) & 0x0ff;
    command = (oled[OLED_COMMAND] & 0x0ff) | 0x000;
    SPI_TxData(command);
    //
    i = 0;
    while(i < count)
    {
        data = oled[OLED_DATA + i] | 0x100;
        SPI_TxData(data);
        i++;
    }
}

//====================
// OLED Send a Pixel
//====================
void OLED_Send_Pixel(uint32_t color)
{
    uint32_t data1, data2;

    data1 = ((color >> 8) & 0xff) | 0x100;
    data2 = (color & 0x0ff) | 0x100;

    SPI_TxData(data1);
    SPI_TxData(data2);
}

//=================
// OLED_Draw_Text
//=================
uint32_t OLED_Draw_Text_Small(char *pStr, uint32_t posx, uint32_t posy, uint32_t color_f, uint32_t color_b)
{
    while(*pStr != '\0')
    {
        OLED_Draw_Char(*pStr, posx, posy, color_f, color_b, OLED_FONT_SMALL);
        pStr++;
        posx++;
    }
    return posx;
}
uint32_t OLED_Draw_Text_Medium(char *pStr, uint32_t posx, uint32_t posy, uint32_t color_f, uint32_t color_b)
{
    while(*pStr != '\0')
    {
        OLED_Draw_Char(*pStr, posx, posy, color_f, color_b, OLED_FONT_MEDIUM);
        pStr++;
        posx++;
    }
    return posx;
}
uint32_t OLED_Draw_Text_LARGE(char *pStr, uint32_t posx, uint32_t posy, uint32_t color_f, uint32_t color_b)
{
    while(*pStr != '\0')
    {
        OLED_Draw_Char(*pStr, posx, posy, color_f, color_b, OLED_FONT_LARGE);
        pStr++;
        posx++;
    }
    return posx;
}

//=======================
// OLED Draw a Character
//=======================
// scale should be 0, 1 or 2
void OLED_Draw_Char(char ch, uint32_t posx, uint32_t posy, uint32_t color_f, uint32_t color_b, uint32_t scale)
{
    uint32_t oled[8];
    uint32_t x0, y0;
    uint32_t xsize, ysize;
    uint32_t x, y;
    uint32_t xfont, yfont;
    uint32_t pixel;
    uint32_t color;
    //
    uint32_t col0, col1, row0, row1;

    ch = (ch < 0x20)? 0x20 : (ch > 0x7f)? 0x7f : ch;
    //
    x0 = posx * (OLED_FONT_XSIZE << scale);
    y0 = posy * (OLED_FONT_YSIZE << scale);
    //
    xsize = OLED_FONT_XSIZE * (1 << scale);
    ysize = OLED_FONT_YSIZE * (1 << scale);
    //
    if ((x0 <= (128 - xsize)) && (y0 <= (128 - ysize)))
    {
        col0 = (gOELD_Orientation_Mode == OLED_TOP_W)? y0       :
               (gOELD_Orientation_Mode == OLED_TOP_S)? x0 :
               (gOELD_Orientation_Mode == OLED_TOP_E)? y0 : x0;
        col1 = (gOELD_Orientation_Mode == OLED_TOP_W)? y0 + ysize - 1 :
               (gOELD_Orientation_Mode == OLED_TOP_S)? x0 + xsize - 1 :
               (gOELD_Orientation_Mode == OLED_TOP_E)? y0 + ysize - 1 : x0 + xsize - 1;
        row0 = (gOELD_Orientation_Mode == OLED_TOP_W)? x0 :
               (gOELD_Orientation_Mode == OLED_TOP_S)? y0 :
               (gOELD_Orientation_Mode == OLED_TOP_E)? x0 : y0;
        row1 = (gOELD_Orientation_Mode == OLED_TOP_W)? x0 + xsize - 1 :
               (gOELD_Orientation_Mode == OLED_TOP_S)? y0 + ysize - 1 :
               (gOELD_Orientation_Mode == OLED_TOP_E)? x0 + xsize - 1 : y0 + ysize - 1;
        //
        oled[OLED_COMMAND] = C_SET_COLUMN_ADDRESS;
        oled[D_START_ADDRESS] = (col0 > 127)? 127 : col0;
        oled[D_END_ADDRESS]   = (col1 > 127)? 127 : col1;
        OLED_Send_Command(oled);
        //
        oled[OLED_COMMAND] = C_SET_ROW_ADDRESS;
        oled[D_START_ADDRESS] = (row0 > 127)? 127 : row0;
        oled[D_END_ADDRESS]   = (row1 > 127)? 127 : row1;
        OLED_Send_Command(oled);
        //
        oled[OLED_COMMAND] = C_WRITE_RAM_COMMAND;
        OLED_Send_Command(oled);
        //
        for (y = 0; y < ysize; y++)
        {
            for (x = 0; x < xsize; x++)
            {
                xfont = x >> scale;
                yfont = y >> scale;
                pixel = FONT[((uint32_t) ch - 0x20) * 8 + yfont];
                pixel = (pixel >> (OLED_FONT_XSIZE - 1 - xfont)) & 0x01;
                color = (pixel == 1)? color_f : color_b;
                OLED_Send_Pixel(color);
            }
        }
    }
}

//================
// OLED Fill Rect
//================
void OLED_Fill_Rect(int32_t x0, int32_t y0, int32_t xsize, int32_t ysize, uint32_t color)
{
    uint32_t i;
    uint32_t oled[8];
    uint32_t pixels;
    int32_t x1, y1;
    //
    uint32_t col0, col1, row0, row1;

    x1 = x0 + xsize;
    y1 = y0 + ysize;
    x0 = (x0 <   0)?  0 : x0;
    x0 = (x0 < 128)? x0 : 128;
    y0 = (y0 <   0)?  0 : y0;
    y0 = (y0 < 128)? y0 : 128;
    xsize = x1 - x0;
    ysize = y1 - y0;

    if ((x0 < 128) && (y0 < 128) && (xsize > 0) && (ysize > 0))
    {
        col0 = (gOELD_Orientation_Mode == OLED_TOP_W)? y0       :
               (gOELD_Orientation_Mode == OLED_TOP_S)? x0 :
               (gOELD_Orientation_Mode == OLED_TOP_E)? y0 : x0;
        col1 = (gOELD_Orientation_Mode == OLED_TOP_W)? y0 + ysize - 1 :
               (gOELD_Orientation_Mode == OLED_TOP_S)? x0 + xsize - 1 :
               (gOELD_Orientation_Mode == OLED_TOP_E)? y0 + ysize - 1 : x0 + xsize - 1;
        row0 = (gOELD_Orientation_Mode == OLED_TOP_W)? x0 :
               (gOELD_Orientation_Mode == OLED_TOP_S)? y0 :
               (gOELD_Orientation_Mode == OLED_TOP_E)? x0 : y0;
        row1 = (gOELD_Orientation_Mode == OLED_TOP_W)? x0 + xsize - 1 :
               (gOELD_Orientation_Mode == OLED_TOP_S)? y0 + ysize - 1 :
               (gOELD_Orientation_Mode == OLED_TOP_E)? x0 + xsize - 1 : y0 + ysize - 1;
        //
        oled[OLED_COMMAND] = C_SET_COLUMN_ADDRESS;
        oled[D_START_ADDRESS] = (col0 > 127)? 127 : col0;
        oled[D_END_ADDRESS]   = (col1 > 127)? 127 : col1;
        OLED_Send_Command(oled);
        //
        oled[OLED_COMMAND] = C_SET_ROW_ADDRESS;
        oled[D_START_ADDRESS] = (row0 > 127)? 127 : row0;
        oled[D_END_ADDRESS]   = (row1 > 127)? 127 : row1;
        OLED_Send_Command(oled);
        //
        oled[OLED_COMMAND] = C_WRITE_RAM_COMMAND;
        OLED_Send_Command(oled);
        //
        pixels = xsize * ysize;
        for (i = 0; i < pixels; i++)
        {
            OLED_Send_Pixel(color);
        }
    }
}

//=================
// OLED Draw Dot
//=================
void OLED_Draw_Dot(int32_t x, int32_t y, int32_t size, uint32_t color)
{
    OLED_Fill_Rect(x, y, size, size, color);
}

//===================
// OLED Clear Screen
//===================
void OLED_Clear_Screen(uint32_t color)
{
    OLED_Fill_Rect(0, 0, 180, 180, color);
}

//====================
// OLED Num4 to Char
//====================
char OLED_Num4_to_Char(uint32_t num4)
{
    num4 = num4 & 0x0f;
    return (num4 < 0x0a)? (num4 + '0') : (num4 - 0x0a + 'A');
}

//=========================
// OLED Draw Hex (small)
//=========================
uint32_t OLED_Draw_Hex(uint32_t bitlen, uint32_t hex, uint32_t posx, uint32_t posy, uint32_t color_f, uint32_t color_b)
{
    uint32_t i;
    uint32_t num4;
    char ch;
    char *head = "0x";
    uint32_t len;

    len = bitlen >> 2;
    posx = OLED_Draw_Text_Small(head, posx, posy, color_f, color_b);
    for (i = 0; i < len; i++)
    {
        num4 = (hex >> (((len - 1) - i) * 4)) & 0x0f;
        ch = OLED_Num4_to_Char(num4);
        OLED_Draw_Char(ch, posx, posy, color_f, color_b, OLED_FONT_SMALL);
        posx++;
    }
    return posx;
}

//====================
// OLED Make Color
//====================
//      RGB num
// BLK  000 0000-003f (< 64)
// BLU  001 0040-007f (<128)
// CYN  011 0080-00bf (<192)
// GRN  010 00c0-00ff (<256)
// YEL  110 0100-013f (<320)
// WHT  111 0140-017f (<384)
// MAG  101 0180-01bf (<448)
// RED  100 01c0-01ff (<512)
// BLK  000
uint32_t OLED_Make_Color(uint32_t num)
{
    uint32_t grn, red, blu;
    uint32_t zero, full, rise, fall;
    uint32_t color;

    num = num & 0x01ff; // 0-511
    rise = num & 0x3f;
    fall = 0x3f - (num & 0x3f);
    zero = 0x00;
    full = 0x3f;

         if (num <  64) {red = zero; grn = zero; blu = rise;}
    else if (num < 128) {red = zero; grn = rise; blu = full;}
    else if (num < 192) {red = zero; grn = full; blu = fall;}
    else if (num < 256) {red = rise; grn = full; blu = zero;}
    else if (num < 320) {red = full; grn = full; blu = rise;}
    else if (num < 384) {red = full; grn = fall; blu = full;}
    else if (num < 448) {red = full; grn = zero; blu = fall;}
    else                {red = fall; grn = zero; blu = zero;}
    //
    red = red >> 1;
    blu = blu >> 1;
    color = (red << 11) + (grn << 5) + (blu << 0);
    return color;
}

//===========================
// OLED Set printf() Font
//===========================
void OLED_printf_Font(uint32_t font)
{
    gOLED_printf_Font  = font;
}

//===========================
// OLED Set printf() Color
//===========================
void OLED_printf_Color(uint32_t color_f, uint32_t color_b) // corrected 2011.03.20 MM
{
    gOLED_printf_ColorF = color_f;
    gOLED_printf_ColorB = color_b;
}

//===========================
// OLED Set printf() Position
//===========================
void OLED_printf_Position(uint32_t posx, uint32_t posy)
{
    gOLED_printf_PosX = posx;
    gOLED_printf_PosY = posy;
}

//=====================
// OLED printf
//=====================
void OLED_printf(const char *format, ...)
{
    va_list ap;
    uint8_t buf[256];
    uint8_t *pStr;

    va_start(ap, format);
    xvsnprintf(buf, 256, format, ap);
    va_end(ap);

    pStr = buf;
    while(*pStr != '\0')
    {
        if (*pStr == '\n')
        {
            gOLED_printf_PosX = 0;
            gOLED_printf_PosY++;
        }
        else
        {
            OLED_Draw_Char(*pStr, gOLED_printf_PosX, gOLED_printf_PosY,
                    gOLED_printf_ColorF, gOLED_printf_ColorB, gOLED_printf_Font);
            gOLED_printf_PosX++;
        }
        pStr++;
        //
        if (gOLED_printf_PosX >= (128 / (OLED_FONT_XSIZE << gOLED_printf_Font)))
        {
            gOLED_printf_PosX = 0;
            gOLED_printf_PosY++;
        }
        if (gOLED_printf_PosY >= (128 / (OLED_FONT_YSIZE << gOLED_printf_Font)))
        {
            gOLED_printf_PosY = 0;
        }
    }
}

//=========================================================
// End of Program
//=========================================================
