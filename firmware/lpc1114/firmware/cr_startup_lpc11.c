//=========================================================
// LPC1114 Project
//=========================================================
// File Name : cr_startup_lpc11.c
// Function  : Startup Routine for LPC1114 in MARY System
//---------------------------------------------------------
// Rev.01 2011.04.17 Munetomo Maruyama
//=========================================================

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

//-------------------------------
// Linker Related Definitions
//-------------------------------
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;
extern void _vStackTop(void); // Stack Top

//----------------------------
// Main Routine Entry
//----------------------------
#if defined (__REDLIB__)
extern void __main(void); // entry point for Redlib
#else
extern int main(void);    // entry point for Newlib
#endif

//-----------------------------------------
// Define Default Exception Handlers
//-----------------------------------------
#define WEAK __attribute__ ((weak))
#define ALIAS(func) __attribute__ ((weak, alias (#func)))
//
void ResetISR(void);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void SVCall_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);
WEAK void IntDefaultHandler(void);
//
void I2C_IRQHandler(void)       ALIAS(IntDefaultHandler);
void TIMER16_0_IRQHandler(void) ALIAS(IntDefaultHandler);
void TIMER16_1_IRQHandler(void) ALIAS(IntDefaultHandler);
void TIMER32_0_IRQHandler(void) ALIAS(IntDefaultHandler);
void TIMER32_1_IRQHandler(void) ALIAS(IntDefaultHandler);
void SSP_IRQHandler(void)       ALIAS(IntDefaultHandler);
void UART_IRQHandler(void)      ALIAS(IntDefaultHandler);
void USB_IRQHandler(void)       ALIAS(IntDefaultHandler);
void USB_FIQHandler(void)       ALIAS(IntDefaultHandler);
void ADC_IRQHandler(void)       ALIAS(IntDefaultHandler);
void WDT_IRQHandler(void)       ALIAS(IntDefaultHandler);
void BOD_IRQHandler(void)       ALIAS(IntDefaultHandler);
void FMC_IRQHandler(void)       ALIAS(IntDefaultHandler);
void PIOINT3_IRQHandler(void)   ALIAS(IntDefaultHandler);
void PIOINT2_IRQHandler(void)   ALIAS(IntDefaultHandler);
void PIOINT1_IRQHandler(void)   ALIAS(IntDefaultHandler);
void PIOINT0_IRQHandler(void)   ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandler(void)    ALIAS(IntDefaultHandler);
// for MARY System
void ARRAY_COM_Rx_N_IRQHandler(void) ALIAS(IntDefaultHandler);
void ARRAY_COM_Rx_S_IRQHandler(void) ALIAS(IntDefaultHandler);
void ARRAY_COM_Rx_W_IRQHandler(void) ALIAS(IntDefaultHandler);
void ARRAY_COM_Rx_E_IRQHandler(void) ALIAS(IntDefaultHandler);

//--------------------------
// Vector Table
//--------------------------
extern void (* const gpVectorHandlers[])(void);
__attribute__ ((section(".isr_vector")))
void (* const gpVectorHandlers[])(void) =
{
    &_vStackTop,                // Initial stack pointer
    ResetISR,                   // Reset handler
    NMI_Handler,                // NMI handler
    HardFault_Handler,          // Hard fault handler
    0,                          // Reserved
    0,                          // Reserved
    0,                          // Reserved
    0,                          // Reserved
    0,                          // Reserved
    0,                          // Reserved
    0,                          // Reserved
    SVCall_Handler,             // SVCall handler
    0,                          // Reserved
    0,                          // Reserved
    PendSV_Handler,             // PendSV handler
    SysTick_Handler,            // SysTick handler
    //
    WAKEUP_IRQHandler,          // PIO0_0  Wakeup
    WAKEUP_IRQHandler,          // PIO0_1  Wakeup
    WAKEUP_IRQHandler,          // PIO0_2  Wakeup
    WAKEUP_IRQHandler,          // PIO0_3  Wakeup
    WAKEUP_IRQHandler,          // PIO0_4  Wakeup
    WAKEUP_IRQHandler,          // PIO0_5  Wakeup
    WAKEUP_IRQHandler,          // PIO0_6  Wakeup
    WAKEUP_IRQHandler,          // PIO0_7  Wakeup
    WAKEUP_IRQHandler,          // PIO0_8  Wakeup
    WAKEUP_IRQHandler,          // PIO0_9  Wakeup
    WAKEUP_IRQHandler,          // PIO0_10 Wakeup
    WAKEUP_IRQHandler,          // PIO0_11 Wakeup
    WAKEUP_IRQHandler,          // PIO1_0  Wakeup
    ARRAY_COM_Rx_N_IRQHandler,  // Array COM Rx from North (for MARY System)
    ARRAY_COM_Rx_S_IRQHandler,  // Array COM Rx from South (for MARY System)
    //
    I2C_IRQHandler,             // I2C0
    TIMER16_0_IRQHandler,       // CT16B0 (16-bit Timer 0)
    TIMER16_1_IRQHandler,       // CT16B1 (16-bit Timer 1)
    TIMER32_0_IRQHandler,       // CT32B0 (32-bit Timer 0)
    TIMER32_1_IRQHandler,       // CT32B1 (32-bit Timer 1)
    SSP_IRQHandler,             // SSP0
    UART_IRQHandler,            // UART0
    //
    ARRAY_COM_Rx_W_IRQHandler,  // Array COM Rx from West (for MARY System)
    ARRAY_COM_Rx_E_IRQHandler,  // Array COM Rx from East (for MARY System)
    //
    ADC_IRQHandler,             // ADC (A/D Converter)
    WDT_IRQHandler,             // WDT (Watch dog Timer)
    BOD_IRQHandler,             // BOD (Brown out Detect)
    FMC_IRQHandler,             // Flash Memory Controller
    PIOINT3_IRQHandler,         // PIO INT3
    PIOINT2_IRQHandler,         // PIO INT2
    PIOINT1_IRQHandler,         // PIO INT1
    PIOINT0_IRQHandler,         // PIO INT0
};

//--------------------
// Reset Handler
//--------------------
void ResetISR(void)
{
    uint8_t *pSrc, *pDst;
    //
    // Copy .data from ROM to RAM
    //
    pSrc = (uint8_t*) &_etext;
    for(pDst = (uint8_t*) &_data; pDst < (uint8_t*) &_edata;)
    {
        *pDst++ = *pSrc++;
    }
    //
    // Clear .bss area
    //
    for(pDst = (uint8_t*) &_bss; pDst < (uint8_t*) &_ebss; pDst++)
    {
        *pDst = 0;
    }
    //
    // System Initialization
    //
    SystemInit();
    //
    // Goto Main
    //
#if defined (__REDLIB__)
    __main() ; // entry point for Redlib
#else
    main();    // entry point for Newlib
#endif
    //
    // Catch Loop
    //
    while(1);
}

//---------------------
// NMI Handler
//---------------------
void NMI_Handler(void)
{
    //
    // Catch Loop
    //
    while(1);
}

//-----------------------
// Hard Fault Handler
//-----------------------
void HardFault_Handler(void)
{
    //
    // Catch Loop
    //
    while(1);
}

//--------------------
// SVCall Handler
//--------------------
void SVCall_Handler(void)
{
    //
    // Catch Loop
    //
    while(1);
}

//---------------------
// PendSV Handler
//---------------------
void PendSV_Handler(void)
{
    //
    // Catch Loop
    //
    while(1);
}

//----------------------------
// SysTick Handler
// (will be replaced by systick.c)
//----------------------------
void SysTick_Handler(void)
{
    //
    // Catch Loop
    //
    while(1);
}

//-----------------------------
// Interrupt Default Handler
//-----------------------------
void IntDefaultHandler(void)
{
    //
    // Catch Loop
    //
    while(1);
}

//=========================================================
// End of Program
//=========================================================
