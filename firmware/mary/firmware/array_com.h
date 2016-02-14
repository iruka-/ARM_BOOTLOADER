//=========================================================
// LPC1114 Project
//=========================================================
// File Name : array_com.h
// Function  : Array Communication Header
//---------------------------------------------------------
// Rev.01 2010.08.10 Munetomo Maruyama
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

#ifndef __ARRAY_COM_H__
#define __ARRAY_COM_H__

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

//======================
// Define Port Position
//======================
enum Array_COM_PORT {N = 0, S, W, E};

//==================
// Define COM ID
//==================
typedef struct
{
    uint32_t posx;
    uint32_t posy;
    uint32_t id;
} sCOM_ID;

//================
// Define Packet
//================
// Sync Packet
// [0] 0x00 (opcode)
// [1] 0x04
// [2] Tick bit07-00
// [3] Tick bit15-08
// [4] Tick bit23-16
// [5] Tick bit31-24
//
// Data Packet
// [0] 0x01 (opcode)
// [1] byte counts (0 means 256)
// [2] data
// ...
// [257] data (at most)
enum enumCOMPacketWhat  {PACKET_SYNC, PACKET_DATA};
enum enumCOMPacketIndex {PACKET_OPCODE, PACKET_COUNTS, PACKET_BYTES};

//============
// Prototypes
//============
void Init_Array_COM(void);
void ARRAY_COM_Set_TMR16B1_Interrupt(uint32_t port, uint32_t onoff);
void ARRAY_COM_Set_GPIO_Interrupt(uint32_t port, uint32_t onoff);
void ARRAY_COM_Set_Port_Direction(uint32_t port, uint32_t dir);
//
uint32_t Array_COM_TxByte(uint32_t port, uint32_t byte);
void     Array_COM_Sequence(uint32_t port);
//
void Array_COM_Tx_Packet(uint32_t port);
void Array_COM_Rx_Packet(uint32_t port, uint32_t byte);
//
void Sync_Ticks_Tx(uint32_t port);
void Sync_Ticks_Rx(uint32_t port);
//
void     Array_COM_Tx_Data32(uint32_t port, uint32_t data32);
uint32_t Array_COM_Rx_Data32(uint32_t port, uint32_t *data32, uint32_t timeout);
//
void     Array_COM_Tx_Multi_Data32(uint32_t port, uint32_t *pMultiData, uint32_t count);
uint32_t Array_COM_Rx_Multi_Data32(uint32_t port, uint32_t *pMultiData, uint32_t timeout);
void     Array_COM_Tx_Multi_Bytes(uint32_t port, uint8_t *pMultiData, uint32_t count);
uint32_t Array_COM_Rx_Multi_Bytes(uint32_t port, uint8_t *pMultiData, uint32_t timeout);
//
void Array_COM_Rx_Cancel(uint32_t port);
//
void Array_COM_ID_Assignment(void);
//
uint32_t Array_COM_Port_Open(uint32_t port);
uint32_t Array_COM_Get_ID_PosX(void);
uint32_t Array_COM_Get_ID_PosY(void);
uint32_t Array_COM_Get_ID(void);
uint32_t Array_COM_Total_Nodes(void);
uint32_t Array_COM_Pack_ID(sCOM_ID *psCOM_ID);
void Array_COM_UnPack_ID(sCOM_ID *psCOM_ID, uint32_t data32);
//
void ARRAY_COM_Rx_N_IRQHandler(void);
void ARRAY_COM_Rx_S_IRQHandler(void);
void ARRAY_COM_Rx_W_IRQHandler(void);
void ARRAY_COM_Rx_E_IRQHandler(void);
//
void PIOINT0_IRQHandler(void);
void PIOINT3_IRQHandler(void);
void TIMER16_1_IRQHandler(void);

#endif // __ARRAY_COM_H__

//=========================================================
// End of Program
//=========================================================
