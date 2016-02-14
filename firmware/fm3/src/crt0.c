/* -=* << crt0.S をアセンブラ風味で書いてみました >> *=- */

#include "crt0.h"

//;
//;		C Runtime Startup for FM3:MB9B618T
//;
EXTERN		_estack			;
//;
//;		割り込みベクトル:
//;
	__attribute__ ((section(".isr_vector"))) void (*const 
g_Vectors					[])(void) = {
	LPVOID  &_estack	       ,/* Top of Stack */
	DCD     Reset_Handler      ,/* Reset Handler */
	DCD     NMI_Handler        ,/* NMI Handler */
	DCD     HardFault_Handler  ,/* Hard Fault Handler */
	DCD     MemManage_Handler  ,/* MPU Fault Handler */
	DCD     BusFault_Handler   ,/* Bus Fault Handler */
	DCD     UsageFault_Handler ,/* Usage Fault Handler */
	DCD     0                  ,/* Reserved */
	DCD     0                  ,/* Reserved */
	DCD     0                  ,/* Reserved */
	DCD     0                  ,/* Reserved */
	DCD     SVC_Handler        ,/* SVCall Handler */
	DCD     DebugMon_Handler   ,/* Debug Monitor Handler */
	DCD     0                  ,/* Reserved */
	DCD     PendSV_Handler     ,/* PendSV Handler */
	DCD     SysTick_Handler    ,/* SysTick Handler */
	DCD     CSV_Handler        ,/* 0: Clock Super Visor */
	DCD     SWDT_Handler       ,/* 1: Software Watchdog Timer */
	DCD     LVD_Handler        ,/* 2: Low Voltage Detector */
	DCD     MFT_WG_IRQHandler  ,/* 3: Wave Form Generator / DTIF */
	DCD     INT0_7_Handler     ,/* 4: External Interrupt Request ch.0 to ch.7 */
	DCD     INT8_15_Handler    ,/* 5: External Interrupt Request ch.8 to ch.15 */
	DCD     DT_Handler         ,/* 6: Dual Timer / Quad Decoder */
	DCD     MFS0RX_IRQHandler  ,/* 7: MultiFunction Serial ch.0 */
	DCD     MFS0TX_IRQHandler  ,/* 8: MultiFunction Serial ch.0 */
	DCD     MFS1RX_IRQHandler  ,/* 9: MultiFunction Serial ch.1 */
	DCD     MFS1TX_IRQHandler  ,/* 10: MultiFunction Serial ch.1 */
	DCD     MFS2RX_IRQHandler  ,/* 11: MultiFunction Serial ch.2 */
	DCD     MFS2TX_IRQHandler  ,/* 12: MultiFunction Serial ch.2 */
	DCD     MFS3RX_IRQHandler  ,/* 13: MultiFunction Serial ch.3 */
	DCD     MFS3TX_IRQHandler  ,/* 14: MultiFunction Serial ch.3 */
	DCD     MFS4RX_IRQHandler  ,/* 15: MultiFunction Serial ch.4 */
	DCD     MFS4TX_IRQHandler  ,/* 16: MultiFunction Serial ch.4 */
	DCD     MFS5RX_IRQHandler  ,/* 17: MultiFunction Serial ch.5 */
	DCD     MFS5TX_IRQHandler  ,/* 18: MultiFunction Serial ch.5 */
	DCD     MFS6RX_IRQHandler  ,/* 19: MultiFunction Serial ch.6 */
	DCD     MFS6TX_IRQHandler  ,/* 20: MultiFunction Serial ch.6 */
	DCD     MFS7RX_IRQHandler  ,/* 21: MultiFunction Serial ch.7 */
	DCD     MFS7TX_IRQHandler  ,/* 22: MultiFunction Serial ch.7 */
	DCD     PPG_Handler        ,/* 23: PPG */
	DCD     TIM_IRQHandler     ,/* 24: OSC / PLL / Watch Counter */
	DCD     ADC0_IRQHandler    ,/* 25: ADC0 */
	DCD     ADC1_IRQHandler    ,/* 26: ADC1 */
	DCD     ADC2_IRQHandler    ,/* 27: ADC2 */
	DCD     MFT_FRT_IRQHandler ,/* 28: Free-run Timer */
	DCD     MFT_IPC_IRQHandler ,/* 29: Input Capture */
	DCD     MFT_OPC_IRQHandler ,/* 30: Output Compare */
	DCD     BT_IRQHandler      ,/* 31: Base Timer ch.0 to ch.7 */
	DCD     CAN0_IRQHandler    ,/* 32: CAN ch.0 */
	DCD     CAN1_IRQHandler    ,/* 33: CAN ch.1 */
	DCD     USBF_Handler       ,/* 34: USB Function */
	DCD     USB_Handler        ,/* 35: USB Function / USB HOST */
	DCD     DummyHandler       ,/* 36: Reserved */
	DCD     DummyHandler       ,/* 37: Reserved */
	DCD     DMAC0_Handler      ,/* 38: DMAC ch.0 */
	DCD     DMAC1_Handler      ,/* 39: DMAC ch.1 */
	DCD     DMAC2_Handler      ,/* 40: DMAC ch.2 */
	DCD     DMAC3_Handler      ,/* 41: DMAC ch.3 */
	DCD     DMAC4_Handler      ,/* 42: DMAC ch.4 */
	DCD     DMAC5_Handler      ,/* 43: DMAC ch.5 */
	DCD     DMAC6_Handler      ,/* 44: DMAC ch.6 */
	DCD     DMAC7_Handler      ,/* 45: DMAC ch.7 */
	DCD     DummyHandler       ,/* 46: Reserved */
	DCD     DummyHandler       ,/* 47: Reserved */
};


//;
//;	リンカースクリプト・シンボル:
//;
EXTERN	_sidata	;/* start address for the initialization values of the .data section.*/
EXTERN	_sdata	;/* start address for the .data section.  */
EXTERN	_edata	;/* end address for the .data section.    */
EXTERN	_sbss	;/* start address for the .bss section.   */
EXTERN	_ebss	;/* end address for the .bss section.     */

//;
//;	リセット処理:
//;

void
Reset_Handler			(void) { uint *s,*t;

#if	_ROMADRS
	// NOTHING TO DO.
#else
	check_user_boot();
#endif

	s = &_sidata;
	t = &_sdata;

//;
//;	データセクションをRAMにコピーする.
//;
	while( t < &_edata ) {
		*t++ = *s++;
	}

//;
//;	bssセクションをゼロ・クリアする.
//;
	t = &_sbss;
	while( t < &_ebss ) {
		*t++ = 0;
	}

	main() ;

//;
//;	終了時 無限ループ.
//;
	while(1) {};
}


//;	デフォルト・ハンドラー	(無限ループ捕捉型)
HANDLER(	NMI_Handler     	)
HANDLER(	HardFault_Handler   )
HANDLER(	MemManage_Handler   )
HANDLER(	BusFault_Handler    )
HANDLER(	UsageFault_Handler  )
HANDLER(	SVCall_Handler      )

HANDLER(	Default_Handler     )

