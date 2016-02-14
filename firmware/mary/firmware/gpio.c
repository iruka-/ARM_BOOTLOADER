//=========================================================
// LPC1114 Project
//=========================================================
// File Name : gpio.c
// Function  : GPIO Control
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

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include "gpio.h"

LPC_GPIO_TypeDef *GPIOPortAdr( uint32_t portNum )
{
	return LPC_GPIO[portNum];
}
//===========================
// GPIO Set Direction
//===========================
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir )
{
    if(dir)
        LPC_GPIO[portNum]->DIR |= 1<<bitPosi;
    else
        LPC_GPIO[portNum]->DIR &= ~(1<<bitPosi);
}

//===========================
// GPIO Set Value
//===========================
void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal )
{
    LPC_GPIO[portNum]->MASKED_ACCESS[(1<<bitPosi)] = (bitVal<<bitPosi);
}

//===========================
// GPIO Get Value
//===========================
uint32_t GPIOGetValue( uint32_t portNum, uint32_t bitPosi)
{
    uint32_t bitVal;

    bitVal = LPC_GPIO[portNum]->MASKED_ACCESS[(1<<bitPosi)];
    bitVal = bitVal >> bitPosi;
    return bitVal;
}

//===========================
// GPIO Set Interrupt Sense
//===========================
void GPIOSetIS( uint32_t portNum, uint32_t bitPosi, uint32_t is )
{
    if(is)
        LPC_GPIO[portNum]->IS |= 1<<bitPosi;
    else
        LPC_GPIO[portNum]->IS &= ~(1<<bitPosi);
}

//=============================
// GPIO Set Interrupt Both Edge
//=============================
void GPIOSetIBE( uint32_t portNum, uint32_t bitPosi, uint32_t ibe )
{
    if(ibe)
        LPC_GPIO[portNum]->IBE |= 1<<bitPosi;
    else
        LPC_GPIO[portNum]->IBE &= ~(1<<bitPosi);
}

//=============================
// GPIO Set Interrupt Event
//=============================
void GPIOSetIEV( uint32_t portNum, uint32_t bitPosi, uint32_t ibv )
{
    if(ibv)
        LPC_GPIO[portNum]->IEV |= 1<<bitPosi;
    else
        LPC_GPIO[portNum]->IEV &= ~(1<<bitPosi);
}

//=============================
// GPIO Set Interrupt Enable
//=============================
void GPIOSetIE( uint32_t portNum, uint32_t bitPosi, uint32_t ie )
{
    if(ie)
        LPC_GPIO[portNum]->IE |= 1<<bitPosi;
    else
        LPC_GPIO[portNum]->IE &= ~(1<<bitPosi);
}

//=============================
// GPIO Get Interrupt Status
//=============================
uint32_t GPIOGetIMS( uint32_t portNum, uint32_t bitPosi )
{
    return (((LPC_GPIO[portNum]->MIS) >> bitPosi) & 0x01);
}

//=============================
// GPIO Clear Interrupt
//=============================
void GPIOClearInt( uint32_t portNum, uint32_t bitPosi )
{
    LPC_GPIO[portNum]->IC = (1 << bitPosi);
}

//=========================================================
// End of Program
//=========================================================
