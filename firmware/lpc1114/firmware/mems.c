//=========================================================
// LPC1114 Project
//=========================================================
// File Name : mems.c
// Function  : MEMS Accelerometer Control
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

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include "i2c.h"
#include "mems.h"
#include "type.h"

//====================
// MEMS Device Address
//====================
#define MEMS_DEV_ADDR 0x38

//=======================
// MEMS Register Address
//=======================
#define MEMS_CTRL_REG1      0x20
#define MEMS_CTRL_REG2      0x21
#define MEMS_CTRL_REG3      0x22
#define MEMS_STATUS_REG     0x27
#define MEMS_OUT_X          0x29
#define MEMS_OUT_Y          0x2b
#define MEMS_OUT_Z          0x2d
#define MEMS_FF_WU_CFG      0x30
#define MEMS_FF_WU_SRC      0x31
#define MEMS_FF_WU_THS      0x32
#define MEMS_FF_WU_DURATION 0x33

//============
// Globals
//============
extern volatile uint32_t I2CCount;
extern volatile uint8_t  I2CMasterBuffer[BUFSIZE];
extern volatile uint8_t  I2CSlaveBuffer[BUFSIZE];
extern volatile uint32_t I2CMasterState;
extern volatile uint32_t I2CReadLength, I2CWriteLength;

//==================
// Initialize MEMS
//==================
void Init_MEMS(void)
{
    // Initialize I2C
    if (I2CInit((uint32_t) I2CMASTER) == FALSE )
    {
      while (1); // Error Trap
    }

    // Enable MEMS
    MEMS_Write_Reg(MEMS_CTRL_REG1, 0x47);
}

//=====================
// MEMS Write Register
//=====================
void MEMS_Write_Reg(uint32_t addr, uint32_t data)
{
    I2CWriteLength = 3;
    I2CReadLength = 0;
    I2CMasterBuffer[0] = MEMS_DEV_ADDR;
    I2CMasterBuffer[1] = addr;
    I2CMasterBuffer[2] = data;
    I2CEngine();
}

//====================
// MEMS Read Register
//====================
uint32_t MEMS_Read_Reg(uint32_t addr)
{
    I2CWriteLength = 2;
    I2CReadLength = 1;
    I2CMasterBuffer[0] = MEMS_DEV_ADDR;
    I2CMasterBuffer[1] = addr;
    I2CMasterBuffer[2] = MEMS_DEV_ADDR | RD_BIT ;
    I2CEngine();
    //
    return I2CSlaveBuffer[0];
}

//=================
// MEMS Get X, Y, Z
//=================
int8_t MEMS_Get_X(void)
{
    return (int8_t) MEMS_Read_Reg(MEMS_OUT_X);
}
int8_t MEMS_Get_Y(void)
{
    return (int8_t) MEMS_Read_Reg(MEMS_OUT_Y);
}
int8_t MEMS_Get_Z(void)
{
    return (int8_t) MEMS_Read_Reg(MEMS_OUT_Z);
}

//=========================================================
// End of Program
//=========================================================
