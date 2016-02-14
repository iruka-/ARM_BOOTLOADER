//=========================================================
// LPC1114 Project
//=========================================================
// File Name : spi.c
// Function  : SPI Control
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
#include "spi.h"

//===================
// Define Constants
//===================
#define SSP_BUFSIZE  16
#define SSP_FIFOSIZE  8
//
/* SSP Status register */
#define SSPSR_TFE       (1 << 0)
#define SSPSR_TNF       (1 << 1)
#define SSPSR_RNE       (1 << 2)
#define SSPSR_RFF       (1 << 3)
#define SSPSR_BSY       (1 << 4)

/* SSP CR0 register */
#define SSPCR0_DSS      (1 << 0)
#define SSPCR0_FRF      (1 << 4)
#define SSPCR0_SPO      (1 << 6)
#define SSPCR0_SPH      (1 << 7)
#define SSPCR0_SCR      (1 << 8)

/* SSP CR1 register */
#define SSPCR1_LBM      (1 << 0)
#define SSPCR1_SSE      (1 << 1)
#define SSPCR1_MS       (1 << 2)
#define SSPCR1_SOD      (1 << 3)

/* SSP Interrupt Mask Set/Clear register */
#define SSPIMSC_RORIM   (1 << 0)
#define SSPIMSC_RTIM    (1 << 1)
#define SSPIMSC_RXIM    (1 << 2)
#define SSPIMSC_TXIM    (1 << 3)

/* SSP0 Interrupt Status register */
#define SSPRIS_RORRIS   (1 << 0)
#define SSPRIS_RTRIS    (1 << 1)
#define SSPRIS_RXRIS    (1 << 2)
#define SSPRIS_TXRIS    (1 << 3)

/* SSP0 Masked Interrupt register */
#define SSPMIS_RORMIS   (1 << 0)
#define SSPMIS_RTMIS    (1 << 1)
#define SSPMIS_RXMIS    (1 << 2)
#define SSPMIS_TXMIS    (1 << 3)

/* SSP0 Interrupt clear register */
#define SSPICR_RORIC    (1 << 0)
#define SSPICR_RTIC     (1 << 1)

//======================
// Initialize SPI
//======================
void Init_SPI(uint32_t bitlen, uint32_t speed, uint32_t spitype)
{
    uint32_t i;
    uint32_t Dummy;

    // Initialize SPI Port
    //     PIO0_2 : SSEL
    //     PIO0_6 : SCLK
    //     PIO0_9 : MOSI
    //     PIO0_8 : MISO
    if (spitype == SPI_GENERIC)
    {
        LPC_IOCON->PIO0_2  = 0x00000001; // SSEL, disable pu/pd mos
    }
    else // if (spitype == SPI_SDCARD)
    {
        GPIOSetDir(0, 2, 1); // PIO0_2 out
        LPC_IOCON->PIO0_2  = 0x00000000; // GPIO, disable pu/pd mos
        GPIOSetValue(0, 2, 1); // high
    }
    LPC_IOCON->SCK_LOC = 0x00000002; // SCK is PIO0_6
    LPC_IOCON->PIO0_6  = 0x00000002; // SCK, disable pu/pd mos
    LPC_IOCON->PIO0_9 = 0x00000001; // MOSI, disable pu/pd mos
    LPC_IOCON->PIO0_8 = 0x00000011; // MISO, enable pu/pd mos
    //
    // Initialize SPI
    //
    LPC_SYSCON->PRESETCTRL |= (0x1<<0);
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<11);
    LPC_SYSCON->SSP0CLKDIV = 0x01; // Divided by 1

    if (speed == SPI_FAST)
    {
        // Data Length = bitlen
        // Frame format = SPI, CPOL = 0, CPHA = 0
        // Bit Frequency = 24MHz, PCLK=48MHz
        //     CPSDVSR*(SCR+1)=2, SCR=0, CPSR=2
        LPC_SSP0->CR0 = 0x0000 + (bitlen - 1);
        LPC_SSP0->CPSR = 2;
    }
    else // if (speed == SPI_SLOW)
    {
        // Data Length = bitlen
        // Frame format = SPI, CPOL = 0, CPHA = 0
        // Bit Frequency = 0.4MHz, PCLK=48MHz
        //     CPSDVSR*(SCR+1)=120, SCR=3, CPSR=30
        LPC_SSP0->CR0 = 0x0300 + (bitlen - 1);
        LPC_SSP0->CPSR = 30;
    }

    // Clear the RxFIFO
    for ( i = 0; i < SSP_FIFOSIZE; i++ ) Dummy = LPC_SSP0->DR;

    // Enable the SSP Interrupt
    NVIC_EnableIRQ(SSP0_IRQn);
    NVIC_SetPriority(SSP0_IRQn, 3);

    // Device select as master, SSP Enabled
    LPC_SSP0->CR1 = SSPCR1_SSE;

    // Enable RXMIS interrupt
  //LPC_SSP0->IMSC = SSPIMSC_RXIM;
}

//========================
// SSP Interrupt Handler
//========================
void SSP_IRQHandler(void)
{
    uint32_t regValue;

    regValue = LPC_SSP0->MIS;

    // Overrun?
    if (regValue & SSPMIS_RORMIS)
    {
        LPC_SSP0->ICR = SSPICR_RORIC; // Clear
    }
    // Timeout?
    if ( regValue & SSPMIS_RTMIS )
    {
        LPC_SSP0->ICR = SSPICR_RTIC; // Clear
    }
    // Tx FIFO Half Empty?
    if ( regValue & SSPMIS_TXMIS )
    {
        // none
    }
    // Rx FIFO not FULL?
    if ( regValue & SSPMIS_RXMIS )
    {
        // none
        //uint32_t dummy;
        //while (SPI_RxData(&dummy) == 0); //dummy read
    }
}

//===============================
// SPI Transfer and Receive Data
//===============================
uint32_t SPI_TxRxData(uint32_t txdata)
{
    uint32_t rxdata;

    // Tx
    while ((LPC_SSP0->SR & (SSPSR_TNF|SSPSR_BSY)) != SSPSR_TNF);
    // Send a byte
    LPC_SSP0->DR = txdata;
    // Wait until the Busy bit is cleared
    while ( LPC_SSP0->SR & SSPSR_BSY );

    // Rx
    while ((LPC_SSP0->SR & SSPSR_RNE) != SSPSR_RNE);
    rxdata = LPC_SSP0->DR;

    return rxdata;
}

//======================
// SPI Transfer Data
//======================
void SPI_TxData(uint32_t txdata)
{
    SPI_TxRxData(txdata);
}

//======================
// SPI Receive Data
//======================
uint32_t SPI_RxData(void)
{
    uint32_t rxdata;

    rxdata = SPI_TxRxData(0xff);
    return rxdata;
}

//=========================================================
// End of Program
//=========================================================
