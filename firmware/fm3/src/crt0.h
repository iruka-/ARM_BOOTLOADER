#ifndef	crt0_h_
#define	crt0_h_

//	:: これは、 crt0.c コンパイル専用のヘッダー ::

//単純な void 関数を定義する:
#define	      VECTOR(f)		void f(void);

//ウィーク・シンボルな void 関数を定義する:
//     (重複シンボルが存在した場合、リンクの優先順位が低いため、置き換え可能である.)
#define	 WEAK_VECTOR(f)		void f(void) __attribute__ ((weak));


//ウィーク・シンボル、かつ、Default_Handler関数と同一アドレスを取る void 関数を定義する:
//	   (メモリー節約のため、実体をDefault_Handler関数に任せる)
#define	ALIAS_VECTOR(f)		void f(void) __attribute__ ((weak, alias("Default_Handler")));

//各例外ベクターのプロトタイプ宣言をここで行なっておく:
      VECTOR(		Reset_Handler   	)
      VECTOR(		NMI_Handler     	)
      VECTOR(		HardFault_Handler	)
      VECTOR(		MemManage_Handler	)
      VECTOR(		BusFault_Handler	)
      VECTOR(		UsageFault_Handler	)
ALIAS_VECTOR(		SVC_Handler			)
ALIAS_VECTOR(		DebugMon_Handler	)
ALIAS_VECTOR(		PendSV_Handler  	)
ALIAS_VECTOR(		SysTick_Handler 	)
      VECTOR(		Default_Handler 	)

ALIAS_VECTOR(		CSV_Handler	       	)
ALIAS_VECTOR(		SWDT_Handler       	)
ALIAS_VECTOR(		LVD_Handler        	)
ALIAS_VECTOR(		MFT_WG_IRQHandler  	)
ALIAS_VECTOR(		INT0_7_Handler     	)
ALIAS_VECTOR(		INT8_15_Handler    	)
ALIAS_VECTOR(		DT_Handler         	)
ALIAS_VECTOR(		MFS0RX_IRQHandler  	)
ALIAS_VECTOR(		MFS0TX_IRQHandler  	)
ALIAS_VECTOR(		MFS1RX_IRQHandler  	)
ALIAS_VECTOR(		MFS1TX_IRQHandler  	)
ALIAS_VECTOR(		MFS2RX_IRQHandler  	)
ALIAS_VECTOR(		MFS2TX_IRQHandler  	)
ALIAS_VECTOR(		MFS3RX_IRQHandler  	)
ALIAS_VECTOR(		MFS3TX_IRQHandler  	)
ALIAS_VECTOR(		MFS4RX_IRQHandler  	)
ALIAS_VECTOR(		MFS4TX_IRQHandler  	)
ALIAS_VECTOR(		MFS5RX_IRQHandler  	)
ALIAS_VECTOR(		MFS5TX_IRQHandler  	)
ALIAS_VECTOR(		MFS6RX_IRQHandler  	)
ALIAS_VECTOR(		MFS6TX_IRQHandler  	)
ALIAS_VECTOR(		MFS7RX_IRQHandler  	)
ALIAS_VECTOR(		MFS7TX_IRQHandler  	)
ALIAS_VECTOR(		PPG_Handler        	)
ALIAS_VECTOR(		TIM_IRQHandler     	)
ALIAS_VECTOR(		ADC0_IRQHandler    	)
ALIAS_VECTOR(		ADC1_IRQHandler    	)
ALIAS_VECTOR(		ADC2_IRQHandler    	)
ALIAS_VECTOR(		MFT_FRT_IRQHandler 	)
ALIAS_VECTOR(		MFT_IPC_IRQHandler 	)
ALIAS_VECTOR(		MFT_OPC_IRQHandler 	)
ALIAS_VECTOR(		BT_IRQHandler      	)
ALIAS_VECTOR(		CAN0_IRQHandler    	)
ALIAS_VECTOR(		CAN1_IRQHandler    	)
ALIAS_VECTOR(		USBF_Handler       	)
ALIAS_VECTOR(		USB_Handler        	)
ALIAS_VECTOR(		DMAC0_Handler      	)
ALIAS_VECTOR(		DMAC1_Handler      	)
ALIAS_VECTOR(		DMAC2_Handler      	)
ALIAS_VECTOR(		DMAC3_Handler      	)
ALIAS_VECTOR(		DMAC4_Handler      	)
ALIAS_VECTOR(		DMAC5_Handler      	)
ALIAS_VECTOR(		DMAC6_Handler      	)
ALIAS_VECTOR(		DMAC7_Handler      	)
ALIAS_VECTOR(		DummyHandler       	)

            
//	アセンブラ風味で書くためのマクロ.
            
#define	uint	unsigned int
#define	EXTERN	extern uint
#define	LPVOID  (void*)
#define	HANDLER(f)	void f(void) {  while(1){}  };
#define	DCD	

extern	int	main();
extern	void check_user_boot();

#endif
