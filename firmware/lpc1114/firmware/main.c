//=========================================================
// LPC1114 Project
//=========================================================
// File Name : main.c
// Function  : Main Routine
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

#include "color_led.h"
#include "systick.h"
#include "integer.h"
#include "uart.h"
#include "monitor/usbio.h"

//==================
// Color LED
//==================
#define	ECHOBACK_TEST	(0)

void USBmonit(void);

void led_off()
{
	Set_Color_LED(0);
}

#if	0
uint8_t *Line[32];

//---------------------
// Get Line Input
//---------------------
static void get_line (uint8_t *buff, uint8_t len)
{
    uint8_t c, i;

    i = 0;
    for (;;) {
        //c = rcvr();
        c = (BYTE) UARTReceive_Byte();
        UART_printf("%c", c);
        if (c == '\r') break;
        if ((c == '\b') && i) i--;
        if ((c >= ' ') && (i < len - 1))
                buff[i++] = c;
    }
    buff[i] = 0;
    UART_printf("\n\r");
}

#endif


#if	ECHOBACK_TEST
int loop(void)
{
	int c = UARTReceive_Byte();
	UARTSend_Byte(c);
}
#else

int loop(void)
{
    while (1) {
		int ch = USBgetch();
		if(	ch == 0xaa) {
			ch = USBgetch();
			if(	ch == 0x55) {
				USBmonit();
			}else if(ch == 0xa5) {
				USBputch(0x55);
			}
		}
	}
}
#endif
//-----------------------
// Main Routine
//-----------------------
int main(void)
{
    //
    // Initialization
    //
	Init_SysTick();
    Init_Color_LED();

//===@BaudRate Setup ===
//	UARTInit(38400);
	UARTInit(115200);
//	UARTInit(230400);
//	UARTInit(460800);
//	UARTInit(500000);
//	UARTInit(1000000);	// 1Mbps

//=== GPIO 0_6 0_8 0_9  R1_0 Function Setup .
    LPC_IOCON->R_PIO1_0 = 0x00000091; // digital , pullup ,GPIO :See LPC111x Users Manual.
    LPC_IOCON->PIO0_6 = 0x00000010; // GPIO, pullup
    LPC_IOCON->PIO0_8 = 0x00000010; // GPIO, pullup
    LPC_IOCON->PIO0_9 = 0x00000010; // GPIO, pullup
    //
    // Main Loop
    //
    while(1)
    {
        //
        // Draw Color LED
        //
//        Draw_Color_LED();
		Set_Color_LED(7);
        //
        // Get Command
        //
		loop();

//        UART_printf(">");
//        get_line(Line, sizeof(Line));

    }
    return 0;
}

//=========================================================
// End of Program
//=========================================================
