//=========================================================
// LPC1114 Project
//=========================================================
// File Name : key.c
// Function  : Key/Switch Control
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

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include "gpio.h"
#include "key.h"

//==============
// Key Port
//==============
static const uint32_t KEY_P[6] = {1, 1, 0, 0, 0, 0};
static const uint32_t KEY_B[6] = {8, 9, 2, 6, 8, 9};

//======================
// Key Detection Status
//======================
enum KEY_DETECT_STATUS {KEY_IDLE, KEY_DETECT, KEY_CONFIRM, KEY_PUSHED, KEY_WAIT_OFF};

//============
// Globals
//============
uint8_t gKey[MAX_KEY];

//======================
// Initialize Key
//======================
void Init_Key(void)
{
    uint32_t i;
    // 4-direction
    // KEY_UP : PIO1_8
    // KEY_DN : PIO1_9
    // KEY_LT : PIO0_2
    // KEY_RT : PIO0_6
    // KEY_CT : PIO0_8
    // Tact
    // KEY_SW : PIO0_9
    LPC_IOCON->PIO1_8 = 0x00000010; // GPIO, pullup
    LPC_IOCON->PIO1_9 = 0x00000010; // GPIO, pullup
    LPC_IOCON->PIO0_2 = 0x00000010; // GPIO, pullup
    LPC_IOCON->PIO0_6 = 0x00000010; // GPIO, pullup
    LPC_IOCON->PIO0_8 = 0x00000010; // GPIO, pullup
    LPC_IOCON->PIO0_9 = 0x00000010; // GPIO, pullup
    GPIOSetDir(1, 8, 0); // input
    GPIOSetDir(1, 9, 0); // input
    GPIOSetDir(0, 2, 0); // input
    GPIOSetDir(0, 6, 0); // input
    GPIOSetDir(0, 8, 0); // input
    GPIOSetDir(0, 9, 0); // input
    //
    for (i = 0; i < MAX_KEY; i++) gKey[i] = 0;
}

//=========================
// Get Key Status
//=========================
uint8_t Check_Key_Status(uint8_t key)
{
    uint8_t status;
    status = (uint8_t) (GPIOGetValue(KEY_P[key], KEY_B[key]) ^ 0x01);
    return status;
}

//=====================
// Get Key Pushed
//=====================
uint8_t Check_Key_Pushed(uint8_t key)
{
    uint8_t push;
    push = (gKey[key] == KEY_PUSHED);
    if (push) gKey[key] = KEY_WAIT_OFF;
    return push;
}

//===================
// Detect Key Push
//===================
// it should be called from SysTick interrupt
void Detect_Key_Push(void)
{
    uint32_t i;
    uint8_t key;

    for (i = 0; i < MAX_KEY; i++)
    {
        key = Check_Key_Status(i);
        //
        switch(gKey[i])
        {
            case KEY_IDLE:
            {
                gKey[i] = (key)? KEY_DETECT : KEY_IDLE;
                break;
            }
            case KEY_DETECT:
            {
                gKey[i] = (key)? KEY_CONFIRM : KEY_IDLE;
                break;
            }
            case KEY_CONFIRM:
            {
                gKey[i] = (key)? KEY_PUSHED : KEY_IDLE;
                break;
            }
            case KEY_PUSHED:
            {
                break;
            }
            case KEY_WAIT_OFF:
            {
                gKey[i] = (key)? KEY_WAIT_OFF : KEY_IDLE;
            }
            default:
            {
                break; // do nothing
            }
        }
    }
}

//=========================================================
// End of Program
//=========================================================
