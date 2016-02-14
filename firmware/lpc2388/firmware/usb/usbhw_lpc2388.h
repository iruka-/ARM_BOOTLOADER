/*
	LPCUSB, an USB device driver for LPC microcontrollers	
	Copyright (C) 2006 Bertrik Sikken (bertrik@sikken.nl)

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	   derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/**
	Hardware definitions for the LPC214x USB controller

	These are private to the usbhw module
*/

#include "../hwlib/LPC23xx.h"

#undef USBPortSel
#undef USBClkCtrl
#undef USBClkSt  

/* USBPortSel only available on the LPC2378 */
#define USBPortSel         (*(volatile unsigned long *)(USB_BASE_ADDR - 0xF0))

/* USB Clock Control Registers */
#define USBClkCtrl         (*(volatile unsigned long *)(USB_BASE_ADDR + 0xDF4))
#define USBClkSt           (*(volatile unsigned long *)(USB_BASE_ADDR + 0xDF8))

/* USB register definitions */
#define USBIntSt		*(volatile unsigned int *)(USB_BASE_ADDR - 0x40)

#define USBDevIntSt		*(volatile unsigned int *)(USB_BASE_ADDR + 0x00)
#define USBDevIntEn		*(volatile unsigned int *)(USB_BASE_ADDR + 0x04)
#define USBDevIntClr	*(volatile unsigned int *)(USB_BASE_ADDR + 0x08)
#define USBDevIntSet	*(volatile unsigned int *)(USB_BASE_ADDR + 0x0C)
#define USBDevIntPri	*(volatile unsigned int *)(USB_BASE_ADDR + 0x2C)

#define USBEpIntSt		*(volatile unsigned int *)(USB_BASE_ADDR + 0x30)
#define USBEpIntEn		*(volatile unsigned int *)(USB_BASE_ADDR + 0x34)
#define USBEpIntClr		*(volatile unsigned int *)(USB_BASE_ADDR + 0x38)
#define USBEpIntSet		*(volatile unsigned int *)(USB_BASE_ADDR + 0x3C)
#define USBEpIntPri		*(volatile unsigned int *)(USB_BASE_ADDR + 0x40)

#define USBReEP			*(volatile unsigned int *)(USB_BASE_ADDR + 0x44)
#define USBEpInd		*(volatile unsigned int *)(USB_BASE_ADDR + 0x48)
#define USBMaxPSize		*(volatile unsigned int *)(USB_BASE_ADDR + 0x4C)

#define USBRxData		*(volatile unsigned int *)(USB_BASE_ADDR + 0x18)
#define USBTxData		*(volatile unsigned int *)(USB_BASE_ADDR + 0x1C)
#define USBRxPLen		*(volatile unsigned int *)(USB_BASE_ADDR + 0x20)
#define USBTxPLen		*(volatile unsigned int *)(USB_BASE_ADDR + 0x24)
#define USBCtrl			*(volatile unsigned int *)(USB_BASE_ADDR + 0x28)

#define USBCmdCode		*(volatile unsigned int *)(USB_BASE_ADDR + 0x10)
#define USBCmdData		*(volatile unsigned int *)(USB_BASE_ADDR + 0x14)

