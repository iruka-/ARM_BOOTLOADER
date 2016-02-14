//=========================================================
// LPC1114 Project
//=========================================================
// File Name : array_com.c
// Function  : Array Communication
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

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include "array_com.h"
#include "gpio.h"
#include "oled.h"
#include "systick.h"

//---------------
// Note
//---------------
// North Port (CN10) : PIO3_5 (slave)
// South Port (CN8 ) : PIO0_3 (master)
// West  Port (CN7 ) : PIO3_2 (slave)
// East  Port (CN9 ) : PIO3_4 (master)
//
//  LPC_TMR16B1 Assignment
//  Free-Run
//  MR0: Timing for North Port
//  MR1: Timing for South Port
//  MR2: Timing for West Port
//  MR3: Timing for East Port

//===================
// Define Constants
//===================
#define FRAME_LENGTH    11 // padding, start, d0, d1, ..., d7, stop
#define PRESCALER       48
#define FREQ_PCLK 48000000 // Hz
#define FREQ_BIT     10000 // Hz
#define FREQ_COUNT     ((FREQ_PCLK) / (PRESCALER  )) // 1000kHz
#define FREQ_FRAME     ((FREQ_BIT) / (FRAME_LENGTH))
#define INTERVAL_FRAME ((FREQ_COUNT) / (FREQ_FRAME))
#define INTERVAL_BIT   ((FREQ_COUNT) / (FREQ_BIT  ))
//
#define FREQ_TICK   100 // Hz
#define SYNC_BYTES    6
#define SYNC_SHIFT ((FREQ_TICK) / ((FREQ_BIT) / (FRAME_LENGTH) / (SYNC_BYTES)))
//
#define PACKET32_BYTES 6
#define PACKET32_TIMEOUT (10 * (FREQ_TICK) / ((FREQ_BIT) / (FRAME_LENGTH) / (SYNC_BYTES)))
#define PACKET32_FOREVER (0xffffffff)
//
#define MCR_INT_PORT(p) (1<<((p)*3))
//
#define IR_INT_PORT(p) (1<<(p))

// Port Position
//enum Array_COM_PORT {N = 0, S, W, E};
static const uint32_t P[4] = {3, 0, 3, 3};
static const uint32_t B[4] = {5, 3, 2, 4};

// Tx/Rx Control State
enum Array_COM_STATE
{
    IDLE,      TX_START, TX_D0, TX_D1, TX_D2, TX_D3, TX_D4, TX_D5, TX_D6, TX_D7, TX_STOP, TX_FINISH,
    RX_DETECT, RX_START, RX_D0, RX_D1, RX_D2, RX_D3, RX_D4, RX_D5, RX_D6, RX_D7, RX_STOP, RX_FINISH
};

// Address of MR Register in Timer16B1
static __IO uint32_t* LPC_TMR16B1_MR[4] = {&(LPC_TMR16B1->MR0), &(LPC_TMR16B1->MR1), &(LPC_TMR16B1->MR2), &(LPC_TMR16B1->MR3)};

// Software Interrupt for Rx
static IRQn_Type ARRAY_COM_RX_IRQn[4] = {13, 14, 22, 23};

//================
// Define Globals
//================
// Sequence Control
volatile uint32_t gCOMState[4]; // Tx/Rx Control State
volatile uint32_t gCOMByte[4];  // Tx/Rx Byte in communicating
//
// Packet Globals
volatile uint8_t  gCOMPacket[4][256+2];
volatile uint32_t gCOMPacketRxFull[4] = {0, 0, 0, 0};
//
// Sync Counter
volatile uint32_t gTxSync_Count = 0;
volatile uint32_t gRxSync_Count = 0;
//
// ID Assignment
volatile uint32_t gCOMPortOpen[4];
volatile uint32_t gCOMTotalNodes;
volatile uint32_t gCOM_ID;
volatile sCOM_ID gsCOM_ID;
//
// Debug
volatile uint32_t gStartBitError = 0;
volatile uint32_t gStopBitError  = 0;

//======================================
// Initialize Array Inter-Communication
//======================================
void Init_Array_COM(void)
{
    uint32_t port;
    //
    // Set GPIO
    //
    LPC_IOCON->PIO3_5 = 0x00000010; // GPIO, pullup
    LPC_IOCON->PIO0_3 = 0x00000010; // GPIO, pullup
    LPC_IOCON->PIO3_2 = 0x00000010; // GPIO, pullup
    LPC_IOCON->PIO3_4 = 0x00000010; // GPIO, pullup
    //
    for (port = 0; port < 4; port++) // port = N, S, W, E
    {
        GPIOSetValue(P[port], B[port], 1); // high
        GPIOSetIS   (P[port], B[port], 0); // interrupt by edge sense
        GPIOSetIBE  (P[port], B[port], 0); // interrupt by an edge
        GPIOSetIEV  (P[port], B[port], 0); // interrupt by falling edge
        ARRAY_COM_Set_Port_Direction(port, 0); // GPIO direction is input
        ARRAY_COM_Set_GPIO_Interrupt(port, 1); // GPIO interrupts enabled
        gCOMPortOpen[port] = 0; // all ports are closed
    }
    //
    // Set Globals
    //
    for (port = 0; port < 4; port++) // port = N, S, W, E
    {
        gCOMState[port] = IDLE;
    }
    //
    // Initialize Timer16B1
    //
    // Timer16 Clock ON (48MHz)
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<8);
    // Set Counter Freq
    LPC_TMR16B1->PR  = PRESCALER - 1;   // Pre-Scaler

    // Free Running Operation, and Disable all MRx interrupt
    ARRAY_COM_Set_TMR16B1_Interrupt(0, 0);
    ARRAY_COM_Set_TMR16B1_Interrupt(1, 0);
    ARRAY_COM_Set_TMR16B1_Interrupt(2, 0);
    ARRAY_COM_Set_TMR16B1_Interrupt(3, 0);

    // Enable Interrupt
    NVIC_SetPriority(EINT0_IRQn,      1); // 2nd priority
    NVIC_SetPriority(EINT3_IRQn,      1); // 2nd priority
    NVIC_SetPriority(TIMER_16_1_IRQn, 0); // top priority
    NVIC_SetPriority(ARRAY_COM_RX_IRQn[N], 2); // 3rd priority
    NVIC_SetPriority(ARRAY_COM_RX_IRQn[S], 2); // 3rd priority
    NVIC_SetPriority(ARRAY_COM_RX_IRQn[W], 2); // 3rd priority
    NVIC_SetPriority(ARRAY_COM_RX_IRQn[E], 2); // 3rd priority
    NVIC_EnableIRQ(EINT0_IRQn     );
    NVIC_EnableIRQ(EINT3_IRQn     );
    NVIC_EnableIRQ(TIMER_16_1_IRQn);
    NVIC_EnableIRQ(ARRAY_COM_RX_IRQn[N]);
    NVIC_EnableIRQ(ARRAY_COM_RX_IRQn[S]);
    NVIC_EnableIRQ(ARRAY_COM_RX_IRQn[W]);
    NVIC_EnableIRQ(ARRAY_COM_RX_IRQn[E]);

    // Start Timer
    LPC_TMR16B1->TCR = 1;
}

//==============================
// Set TMR16B1 Interrupt
//==============================
// Not to corrupt the data by read-modify-write in interrupt contentions.
void ARRAY_COM_Set_TMR16B1_Interrupt(uint32_t port, uint32_t onoff)
{
    __disable_irq();
    LPC_TMR16B1->MCR = LPC_TMR16B1->MCR & ~(1     << (port * 3));
    LPC_TMR16B1->MCR = LPC_TMR16B1->MCR |  (onoff << (port * 3));
    __enable_irq();
}

//==============================
// Set GPIO Interrupt
//==============================
// Not to corrupt the data by read-modify-write in interrupt contentions.
void ARRAY_COM_Set_GPIO_Interrupt(uint32_t port, uint32_t onoff)
{
    __disable_irq();
    GPIOSetIE(P[port], B[port], onoff);
    __enable_irq();
}

//=======================
// Set Port Direction
//=======================
// Not to corrupt the data by read-modify-write in interrupt contentions.
void ARRAY_COM_Set_Port_Direction(uint32_t port, uint32_t dir)
{
    __disable_irq();
    GPIOSetDir(P[port], B[port], dir);
    __enable_irq();
}

//==============================================
// Interrupt Handler of Array COM Rx from North
//==============================================
void ARRAY_COM_Rx_N_IRQHandler(void)
{
    uint32_t port = N;

    // Store the byte into Packet
    Array_COM_Rx_Packet(port, gCOMByte[port]);

    // Clear flag
    NVIC_ClearPendingIRQ(ARRAY_COM_RX_IRQn[port]);
}

//==============================================
// Interrupt Handler of Array COM Rx from South
//==============================================
void ARRAY_COM_Rx_S_IRQHandler(void)
{
    uint32_t port = S;

    // Store the byte into Packet
    Array_COM_Rx_Packet(port, gCOMByte[port]);

    // Clear flag
    NVIC_ClearPendingIRQ(ARRAY_COM_RX_IRQn[port]);
}

//==============================================
// Interrupt Handler of Array COM Rx from West
//==============================================
void ARRAY_COM_Rx_W_IRQHandler(void)
{
    uint32_t port = W;

    // Store the byte into Packet
    Array_COM_Rx_Packet(port, gCOMByte[port]);

    // Clear flag
    NVIC_ClearPendingIRQ(ARRAY_COM_RX_IRQn[port]);
}

//==============================================
// Interrupt Handler of Array COM Rx from East
//==============================================
void ARRAY_COM_Rx_E_IRQHandler(void)
{
    uint32_t port = E;

    // Store the byte into Packet
    Array_COM_Rx_Packet(port, gCOMByte[port]);

    // Clear flag
    NVIC_ClearPendingIRQ(ARRAY_COM_RX_IRQn[port]);
}

//=============================
// Interrupt Handler of PIO0
//=============================
void PIOINT0_IRQHandler(void)
{
    if (GPIOGetIMS(P[S], B[S])) // south
    {
        if (gCOMState[S] == IDLE)
        {
            // Disable this Interrupt
            ARRAY_COM_Set_GPIO_Interrupt(S, 0);
            // Set Interval
            *LPC_TMR16B1_MR[S] = LPC_TMR16B1->TC + (INTERVAL_BIT / 2);
            // Enable TMR16B1 Interrupt
            ARRAY_COM_Set_TMR16B1_Interrupt(S, 1);
            // Set State
            gCOMState[S] = RX_START;
        }
        // Clear Flag
        GPIOClearInt(P[S], B[S]);
    }
}

//=============================
// Interrupt Handler of PIO3
//=============================
void PIOINT3_IRQHandler(void)
{
    uint32_t port;

    for (port = 0; port < 4; port++)
    {
        if (port == S) continue;
        //
        if (GPIOGetIMS(P[port], B[port]))
        {
            if (gCOMState[port] == IDLE)
            {
                // Disable this Interrupt
                ARRAY_COM_Set_GPIO_Interrupt(port, 0);
                // Set Interval
                *LPC_TMR16B1_MR[port] = LPC_TMR16B1->TC + (INTERVAL_BIT / 2);
                // Enable TMR16B1 Interrupt
                ARRAY_COM_Set_TMR16B1_Interrupt(port, 1);
                // Set State
                gCOMState[port] = RX_START;
            }
            // Clear Flag
            GPIOClearInt(P[port], B[port]);
        }
    }
}

//================================
// Interrupt Handler of Timer16B1
//================================
void TIMER16_1_IRQHandler(void)
{
    uint32_t port;

    for (port = 0; port < 4; port++)
    {
        if (LPC_TMR16B1->IR & IR_INT_PORT(port))
        {
            // Sequence Control
            Array_COM_Sequence(port);

            // Clear Flag
            LPC_TMR16B1->IR = IR_INT_PORT(port);
        }
    }
}

//=================
// Array Tx Byte
//=================
// caller from main
uint32_t Array_COM_TxByte(uint32_t port, uint32_t byte)
{
    // busy or not?
    if (gCOMState[port] != IDLE) return 0; // busy

    // Disable GPIO Interrupt during Tx
    ARRAY_COM_Set_GPIO_Interrupt(port, 0);

    // Set Tx Level (pre-start bit)
    ARRAY_COM_Set_Port_Direction(port, 1); // output
    GPIOSetValue(P[port], B[port], 1);     // still high

    // Set Interval
    *LPC_TMR16B1_MR[port] = LPC_TMR16B1->TC + INTERVAL_BIT;
    // Enable TMR16B1 Interrupt
    ARRAY_COM_Set_TMR16B1_Interrupt(port, 1);

    // Set Next State
    gCOMState[port] = TX_START;
    gCOMByte[port] = byte;

    // success
    return 1;
}

//===========================
// Array Tx Sequence Control
//===========================
void Array_COM_Sequence(uint32_t port)
{
    uint32_t bit;

    switch(gCOMState[port])
    {
        //----------------
        // Rx Control
        //----------------
        case RX_START:
        {
            // Set Interval
            *LPC_TMR16B1_MR[port] = LPC_TMR16B1->TC + INTERVAL_BIT;
            // Get bit
            bit = GPIOGetValue(P[port], B[port]);
            if (bit == 1) // not start bit
            {
                // Disable TMR16B1 Interrupt
                ARRAY_COM_Set_TMR16B1_Interrupt(port, 0);
                // Enable GPIO Interrupt to wait for next Rx
                GPIOClearInt(P[port], B[port]);
                ARRAY_COM_Set_GPIO_Interrupt(port, 1);
                // Set State
                gCOMState[port] = IDLE;
                gStartBitError++;
                break;
            }
            // Set State
            gCOMByte[port] = 0;
            gCOMState[port] = RX_D0;
            break;
        }
        case RX_D0:
        case RX_D1:
        case RX_D2:
        case RX_D3:
        case RX_D4:
        case RX_D5:
        case RX_D6:
        case RX_D7:
        {
            // Set Interval
            *LPC_TMR16B1_MR[port] = LPC_TMR16B1->TC + INTERVAL_BIT;
            // Get bit
            bit = GPIOGetValue(P[port], B[port]);
            gCOMByte[port] = gCOMByte[port] | (bit << (gCOMState[port] - RX_D0));
            // Set State
            gCOMState[port] = gCOMState[port] + 1;
            break;
        }
        case RX_STOP:
        {
            // Set Interval
            *LPC_TMR16B1_MR[port] = LPC_TMR16B1->TC + (INTERVAL_BIT / 2);
            // Get bit
            bit = GPIOGetValue(P[port], B[port]);
            if (bit == 0) // not stop bit (framing error)
            {
                gStopBitError++;
                // so far, continue
            }
            // Set State
            gCOMState[port] = RX_FINISH;
            break;
        }
        case RX_FINISH:
        {
            // Disable TMR16B1 Interrupt
            ARRAY_COM_Set_TMR16B1_Interrupt(port, 0);
            // Generate Rx Interrupt by software
            NVIC_SetPendingIRQ(ARRAY_COM_RX_IRQn[port]);
            // Clear Remained GPIO Edge Sense
            GPIOClearInt(P[port], B[port]);
            //Enable GPIO Interrupt to wait for next Rx
            ARRAY_COM_Set_GPIO_Interrupt(port, 1);
            // Go to IDLE
            gCOMState[port] = IDLE;
            break;
        }
        //----------------
        // Tx Control
        //----------------
        case TX_START:
        {
            // Set Interval
            *LPC_TMR16B1_MR[port] = LPC_TMR16B1->TC + INTERVAL_BIT;
            // Start Bit
            GPIOSetValue(P[port], B[port],0);
            // Set State
            gCOMState[port] = TX_D0;
            break;
        }
        case TX_D0:
        case TX_D1:
        case TX_D2:
        case TX_D3:
        case TX_D4:
        case TX_D5:
        case TX_D6:
        case TX_D7:
        {
            // Set Interval
            *LPC_TMR16B1_MR[port] = LPC_TMR16B1->TC + INTERVAL_BIT;
            // Set Tx Output bit (data)
            bit = (gCOMByte[port] >> (gCOMState[port] - TX_D0)) & 0x01;
            GPIOSetValue(P[port], B[port], bit);
            // Set State
            gCOMState[port] = gCOMState[port] + 1;
            break;
        }
        case TX_STOP:
        {
            // Set Interval
            *LPC_TMR16B1_MR[port] = LPC_TMR16B1->TC + INTERVAL_BIT;
            // Set Tx Output bit (stop bit)
            bit = 1;
            GPIOSetValue(P[port], B[port], bit);
            // Set State
            gCOMState[port] = TX_FINISH;
            break;
        }
        case TX_FINISH:
        {
            // Disable TMR16B1 Interrupt
            ARRAY_COM_Set_TMR16B1_Interrupt(port, 0);
            // Set Port Input
            ARRAY_COM_Set_Port_Direction(port, 0); // input
            // Clear Remained GPIO Edge Sense here
            GPIOClearInt(P[port], B[port]);
            // Enable GPIO Interrupt to detect contiguous Rx start bit.
            ARRAY_COM_Set_GPIO_Interrupt(port, 1);
            // Set State
            gCOMState[port] = IDLE;
            break;
        }
        default: // you should not reach here.
        {
            // Disable TMR16B1 Interrupt
            ARRAY_COM_Set_TMR16B1_Interrupt(port, 0);
            // Clear Remained GPIO Edge Sense
            GPIOClearInt(P[port], B[port]);
            // Enable GPIO Interrupt to wait for next Rx
            ARRAY_COM_Set_GPIO_Interrupt(port, 1);
            // Set State
            gCOMState[port] = IDLE;
            break;
        }
    }
}

//=======================
// Array COM Tx Packet
//=======================
void Array_COM_Tx_Packet(uint32_t port)
{
    uint32_t i;
    uint32_t count;
    uint32_t byte;

    byte = gCOMPacket[port][PACKET_OPCODE];
    while (Array_COM_TxByte(port, byte) == 0);
    //
    count = gCOMPacket[port][PACKET_COUNTS];
    while (Array_COM_TxByte(port, count) == 0);
    //
    for (i = 0; i < count; i++)
    {
        byte = gCOMPacket[port][PACKET_BYTES + i];
        while (Array_COM_TxByte(port, byte) == 0);
    }
}

//=====================
// Array COM Rx Packet
//=====================
void Array_COM_Rx_Packet(uint32_t port, uint32_t byte)
{
    static uint32_t index[4] = {PACKET_OPCODE, PACKET_OPCODE, PACKET_OPCODE, PACKET_OPCODE};
    static uint32_t what[4];
    static uint32_t counts[4];

    if (index[port] == PACKET_OPCODE)
    {
        gCOMPacket[port][index[port]] = byte;
        what[port] = byte;
        index[port]++;
    }
    else if (index[port] == PACKET_COUNTS)
    {
        gCOMPacket[port][index[port]] = byte;
        counts[port] = byte;
        index[port]++;
    }
    else
    {
        gCOMPacket[port][index[port]] = byte;
        index[port]++;
        //
        // Finish?
        //
        if ((PACKET_BYTES + counts[port]) == index[port])
        {
            index[port] = PACKET_OPCODE;
            //
            // Sync Packet or Data Packet?
            if (gCOMPacket[port][PACKET_OPCODE] == PACKET_SYNC)
            {
                Sync_Ticks_Rx(port);
            }
            else if (gCOMPacket[port][PACKET_OPCODE] == PACKET_DATA)
            {
                gCOMPacketRxFull[port] = 1;
            }
        }
    }
}

//=================
// Sync Ticks Tx
//=================
void Sync_Ticks_Tx(uint32_t port)
{
    uint32_t i;
    uint32_t ticks;
    uint32_t byte;

    ticks = Get_Ticks();

    gCOMPacket[port][PACKET_OPCODE] = PACKET_SYNC;
    gCOMPacket[port][PACKET_COUNTS] = 0x04;

    for (i = 0; i < 4; i++)
    {
        byte = (ticks >> (i * 8)) & 0x0ff;
        gCOMPacket[port][PACKET_BYTES + i] = byte;
    }

    Array_COM_Tx_Packet(port);
}

//=================
// Sync Ticks Rx
//=================
void Sync_Ticks_Rx(uint32_t port)
{
    uint32_t ticks;

    ticks =         (gCOMPacket[port][PACKET_BYTES + 0] <<  0);
    ticks = ticks | (gCOMPacket[port][PACKET_BYTES + 1] <<  8);
    ticks = ticks | (gCOMPacket[port][PACKET_BYTES + 2] << 16);
    ticks = ticks | (gCOMPacket[port][PACKET_BYTES + 3] << 24);

    Set_Ticks(ticks + SYNC_SHIFT);
    gRxSync_Count++;
}

//======================
// Array COM Tx Data32
//======================
void Array_COM_Tx_Data32(uint32_t port, uint32_t data32)
{
    uint32_t i;
    uint32_t byte;

    gCOMPacket[port][PACKET_OPCODE] = PACKET_DATA;
    gCOMPacket[port][PACKET_COUNTS] = 0x04;

    for (i = 0; i < 4; i++)
    {
        byte = (data32 >> (i * 8)) & 0x0ff;
        gCOMPacket[port][PACKET_BYTES + i] = byte;
    }

    Array_COM_Tx_Packet(port);
}

//======================
// Array COM Rx Data32
//======================
uint32_t Array_COM_Rx_Data32(uint32_t port, uint32_t *data32, uint32_t timeout)
{
    uint32_t ticks_prev;
    uint32_t data;
    uint32_t received;

    ticks_prev  = Get_Ticks();
    while(timeout > (Get_Ticks() - ticks_prev))
    {
      //if (gCOMPacketRxFull[port]) break;
    }
    if (gCOMPacketRxFull[port])
    {
        data =        (gCOMPacket[port][PACKET_BYTES + 0] <<  0);
        data = data | (gCOMPacket[port][PACKET_BYTES + 1] <<  8);
        data = data | (gCOMPacket[port][PACKET_BYTES + 2] << 16);
        data = data | (gCOMPacket[port][PACKET_BYTES + 3] << 24);
        *data32 = data;
        gCOMPacketRxFull[port] = 0;
        received = 1;
    }
    else
    {
        received = 0;
    }
    return received;
}

//=============================
// Array COM Tx Multiple Data32
//=============================
void Array_COM_Tx_Multi_Data32(uint32_t port, uint32_t *pMultiData, uint32_t count)
{
    uint32_t i, j;
    uint32_t data32;
    uint32_t byte;

    gCOMPacket[port][PACKET_OPCODE] = PACKET_DATA;
    gCOMPacket[port][PACKET_COUNTS] = count * 4;

    for (j = 0; j < count; j++)
    {
        data32 = pMultiData[j];
        for (i = 0; i < 4; i++)
        {
            byte = (data32 >> (i * 8)) & 0x0ff;
            gCOMPacket[port][PACKET_BYTES + j * 4 + i] = byte;
        }

    }

    Array_COM_Tx_Packet(port);
}

//=============================
// Array COM Rx Multiple Data32
//=============================
// return data count
uint32_t Array_COM_Rx_Multi_Data32(uint32_t port, uint32_t *pMultiData, uint32_t timeout)
{
    uint32_t ticks_prev;
    uint32_t count;
    uint32_t j;
    uint32_t data;

    count = 0;
    ticks_prev  = Get_Ticks();
    while(timeout > (Get_Ticks() - ticks_prev))
    {
      //if (gCOMPacketRxFull[port]) break;
    }
    if (gCOMPacketRxFull[port])
    {
        count = gCOMPacket[port][PACKET_COUNTS] / 4;
        //
        for (j = 0; j < count; j++)
        {
            data =        (gCOMPacket[port][PACKET_BYTES + j * 4 + 0] <<  0);
            data = data | (gCOMPacket[port][PACKET_BYTES + j * 4 + 1] <<  8);
            data = data | (gCOMPacket[port][PACKET_BYTES + j * 4 + 2] << 16);
            data = data | (gCOMPacket[port][PACKET_BYTES + j * 4 + 3] << 24);
            pMultiData[j] = data;
        }
        gCOMPacketRxFull[port] = 0;
    }
    return count;
}

//============================
// Array COM Tx Multiple Bytes
//============================
void Array_COM_Tx_Multi_Bytes(uint32_t port, uint8_t *pMultiData, uint32_t count)
{
    uint32_t j;
    uint8_t  byte;

    gCOMPacket[port][PACKET_OPCODE] = PACKET_DATA;
    gCOMPacket[port][PACKET_COUNTS] = count;

    for (j = 0; j < count; j++)
    {
        byte = pMultiData[j];
        gCOMPacket[port][PACKET_BYTES + j] = byte;
    }

    Array_COM_Tx_Packet(port);
}

//============================
// Array COM Rx Multiple Bytes
//============================
// return data count
uint32_t Array_COM_Rx_Multi_Bytes(uint32_t port, uint8_t *pMultiData, uint32_t timeout)
{
    uint32_t ticks_prev;
    uint32_t count;
    uint32_t j;
    uint8_t  byte;

    count = 0;
    ticks_prev  = Get_Ticks();
    while(timeout > (Get_Ticks() - ticks_prev))
    {
      //if (gCOMPacketRxFull[port]) break;
    }
    if (gCOMPacketRxFull[port])
    {
        count = gCOMPacket[port][PACKET_COUNTS];
        //
        for (j = 0; j < count; j++)
        {
            byte = gCOMPacket[port][PACKET_BYTES + j];
            pMultiData[j] = byte;
        }
        gCOMPacketRxFull[port] = 0;
    }
    return count;
}

//==========================
// Array COM Cancel Rx Data
//==========================
void Array_COM_Rx_Cancel(uint32_t port)
{
    gCOMPacketRxFull[port] = 0;
}

//======================
// Array ID Assignment
//======================
void Array_COM_ID_Assignment(void)
{
    uint32_t data32;
    uint32_t rx_flag[4];
    sCOM_ID sID;

    //---------------------------
    // Wait for Adjacent Ready
    //    if there are difference between
    //    execution time of each adjacent startup routine,
    //    this wait time is required.
    //---------------------------
    Wait_N_Ticks(20); // 200ms
    //-----------------------
    // Adjacent Check
    //-----------------------
    // [1] Send Req Packet to E
    Array_COM_Tx_Data32(E, 0xaaaaaaaa);
    // [2] Wait Req Packet from W
    //         if Req within timeout, Send Ack Packet to W, W is opened.
    //         if Not within timeout, W is closed.
    if (Array_COM_Rx_Data32(W, &data32, PACKET32_TIMEOUT) && (data32 == 0xaaaaaaaa))
    {
        Array_COM_Tx_Data32(W, 0xbbbbbbbb);
        gCOMPortOpen[W] = 1;
    }
    else
    {
        Array_COM_Tx_Data32(W, 0xbbbbbbbb); // to sync program progress
        gCOMPortOpen[W] = 0;
    }
    // [3] Wait Ack Packet from E
    //         if Ack within timeout, E is opened.
    //         if Not within timeout, E is closed.
    if (Array_COM_Rx_Data32(E, &data32, PACKET32_TIMEOUT) && (data32 == 0xbbbbbbbb))
    {
        gCOMPortOpen[E] = 1;
    }
    else
    {
        gCOMPortOpen[E] = 0;
    }
    //-----------------------
    // [4] Send Req Packet to S
    Array_COM_Tx_Data32(S, 0x55555555);
    // [5] Wait Req Packet from N
    //         if Req within timeout, Send Ack Packet to N, N is opened.
    //         if Not within timeout, N is closed.
    if (Array_COM_Rx_Data32(N, &data32, PACKET32_TIMEOUT) && (data32 == 0x55555555))
    {
        Array_COM_Tx_Data32(N, 0x66666666);
        gCOMPortOpen[N] = 1;
    }
    else
    {
        Array_COM_Tx_Data32(N, 0x66666666); // to sync program progress
        gCOMPortOpen[N] = 0;
    }
    // [6] Wait Ack Packet from S
    //         if Ack within timeout, S is opened.
    //         if Not within timeout, S is closed.
    if (Array_COM_Rx_Data32(S, &data32, PACKET32_TIMEOUT) && (data32 == 0x66666666))
    {
        gCOMPortOpen[S] = 1;
    }
    else
    {
        gCOMPortOpen[S] = 0;
    }
    //-----------------------
    // ID Assignment
    //-----------------------
    gsCOM_ID.posx = 0;
    gsCOM_ID.posy = 0;
    gsCOM_ID.id   = 0;
    // [1] If N is opened, Get ID from N
    if (gCOMPortOpen[N])
    {
        while(1)
        {
            if (Array_COM_Rx_Data32(N, &data32, PACKET32_TIMEOUT))
            {
                Array_COM_UnPack_ID(&sID, data32);
                gsCOM_ID.posx = sID.posx;
                gsCOM_ID.posy = sID.posy + 1;
                sID.id = sID.id + 1;
                gsCOM_ID.id   = (gsCOM_ID.id < sID.id)? sID.id : gsCOM_ID.id;
                break;
            }
        }
    }
    // [2] If W is opened, Get ID from W
    if (gCOMPortOpen[W])
    {
        while(1)
        {
            if (Array_COM_Rx_Data32(W, &data32, PACKET32_TIMEOUT))
            {
                Array_COM_UnPack_ID(&sID, data32);
                gsCOM_ID.posx = sID.posx + 1;
                gsCOM_ID.posy = sID.posy;
                sID.id = sID.id + 1;
                gsCOM_ID.id   = (gsCOM_ID.id < sID.id)? sID.id : gsCOM_ID.id;
                break;
            }
        }
    }
    sID.posx = gsCOM_ID.posx;
    sID.posy = gsCOM_ID.posy;
    sID.id   = gsCOM_ID.id;
    // [3] If E is opened, Send ID to E
    if (gCOMPortOpen[E])
    {
        Array_COM_Tx_Data32(E,  Array_COM_Pack_ID(&sID));
        // wait for return back from E
        while(1)
        {
            if (Array_COM_Rx_Data32(E, &data32, PACKET32_TIMEOUT))
            {
                Array_COM_UnPack_ID(&sID, data32);
                // keep max id in its horizontal line
                sID.posx = gsCOM_ID.posx;
                sID.posy = gsCOM_ID.posy;
                // acknowledge
                Array_COM_Tx_Data32(E, 0xaaaaaaaa);
                break;
            }
        }
    }
    // [4] If W is opened, Return Back ID to W
    if (gCOMPortOpen[W])
    {
        Array_COM_Tx_Data32(W,  Array_COM_Pack_ID(&sID));
        // wait for acknowledge
        while (Array_COM_Rx_Data32(W, &data32, PACKET32_TIMEOUT) == 0);
    }
    // [5] If S is opened, Send ID to S (with max id in its horizontal line)
    if (gCOMPortOpen[S])
    {
        Array_COM_Tx_Data32(S,  Array_COM_Pack_ID(&sID));
    }
    //--------------------------
    // Collect Total Node Count
    //--------------------------
    // Only a node which is located at (0,0) can get correct value.
    gCOMTotalNodes = Array_COM_Get_ID();
    // [1] If either E or S are opened,
    //     If E is opened, Get Return from E
    //     If S is opened, Get Return from S
    if ((gCOMPortOpen[E]) || (gCOMPortOpen[S]))
    {
        rx_flag[E] = (gCOMPortOpen[E])? 0 : 1;
        rx_flag[S] = (gCOMPortOpen[S])? 0 : 1;
        while(1)
        {
            if ((rx_flag[E] == 0) && Array_COM_Rx_Data32(E, &data32, PACKET32_TIMEOUT))
            {
                gCOMTotalNodes = (gCOMTotalNodes < data32)? data32 : gCOMTotalNodes;
                rx_flag[E] = 1;
            }
            if ((rx_flag[S] == 0) && Array_COM_Rx_Data32(S, &data32, PACKET32_TIMEOUT))
            {
                gCOMTotalNodes = (gCOMTotalNodes < data32)? data32 : gCOMTotalNodes;
                rx_flag[S] = 1;
            }
            if ((rx_flag[E]) && (rx_flag[S])) break;
        }
    }
    // [2] Send Return to W and N
    if (gCOMPortOpen[W]) Array_COM_Tx_Data32(W, gCOMTotalNodes);
    if (gCOMPortOpen[N]) Array_COM_Tx_Data32(N, gCOMTotalNodes);
    //---------------------------
    // Broadcast Total Node Count
    //---------------------------
    gCOMTotalNodes++; // adjust as node counts (A node (0,0) has its meaning.)
    // [1] If either W or N are opened,
    //     If W is opened, Get Node Counts from W
    //     If N is opened, Get Node Counts from N
    if (Array_COM_Get_ID() != 0)
    {
        if ((gCOMPortOpen[W]) || (gCOMPortOpen[N]))
        {
            rx_flag[W] = (gCOMPortOpen[W])? 0 : 1;
            rx_flag[N] = (gCOMPortOpen[N])? 0 : 1;
            while(1)
            {
                if ((rx_flag[W] == 0) && Array_COM_Rx_Data32(W, &data32, PACKET32_TIMEOUT))
                {
                    gCOMTotalNodes = data32;
                    rx_flag[W] = 1;
                }
                if ((rx_flag[N] == 0) && Array_COM_Rx_Data32(N, &data32, PACKET32_TIMEOUT))
                {
                    gCOMTotalNodes = data32;
                    rx_flag[N] = 1;
                }
                if ((rx_flag[W]) && (rx_flag[N])) break;
            }
        }
    }
    // [2] Send Node Counts to E and S
    if (gCOMPortOpen[E]) Array_COM_Tx_Data32(E, gCOMTotalNodes);
    if (gCOMPortOpen[S]) Array_COM_Tx_Data32(S, gCOMTotalNodes);
}

//======================
// Array COM Parameters
//======================
uint32_t Array_COM_Port_Open(uint32_t port)
{
    return gCOMPortOpen[port];
}
uint32_t Array_COM_Get_ID_PosX(void)
{
    return gsCOM_ID.posx;
}
uint32_t Array_COM_Get_ID_PosY(void)
{
    return gsCOM_ID.posy;
}
uint32_t Array_COM_Get_ID(void)
{
    return gsCOM_ID.id;
}
uint32_t Array_COM_Total_Nodes(void)
{
    return gCOMTotalNodes;
}
uint32_t Array_COM_Pack_ID(sCOM_ID *psCOM_ID)
{
    uint32_t data32;
    data32 = (psCOM_ID->id << 16) + (psCOM_ID->posy << 8) + (psCOM_ID->posx << 0);
    return data32;
}
void Array_COM_UnPack_ID(sCOM_ID *psCOM_ID, uint32_t data32)
{
    psCOM_ID->id   = (data32 >> 16) & 0x0000ffff;
    psCOM_ID->posy = (data32 >>  8) & 0x000000ff;
    psCOM_ID->posx = (data32 >>  0) & 0x000000ff;
}

//=========================================================
// End of Program
//=========================================================
