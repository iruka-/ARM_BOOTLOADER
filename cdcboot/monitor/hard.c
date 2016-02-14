/* *****************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 LeafLabs LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ****************************************************************************/

/**
 *  @file hardware.c
 *
 *  @brief init routines to setup clocks, interrupts, also destructor functions.
 *  does not include USB stuff. EEPROM read/write functions.
 *
 */

#include "hard.h"
#include "boards/boards.h"
#include "systick.h"
#include "timer.h"

#include "util.h"

#include "nvic.h"
#include "monit.h"


//-----------------------------------------------
//	
//-----------------------------------------------
void gpio_init_all();
void toggleLED();
void LEDtest();

//-----------------------------------------------
//	
//-----------------------------------------------
void systemReset(void)
{
	SET_REG(RCC_CR, GET_REG(RCC_CR)     | 0x00000001);
	SET_REG(RCC_CFGR, GET_REG(RCC_CFGR) & 0xF8FF0000);
	SET_REG(RCC_CR, GET_REG(RCC_CR)     & 0xFEF6FFFF);
	SET_REG(RCC_CR, GET_REG(RCC_CR)     & 0xFFFBFFFF);
	SET_REG(RCC_CFGR, GET_REG(RCC_CFGR) & 0xFF80FFFF);

	SET_REG(RCC_CIR, 0x00000000);  /* disable all RCC interrupts */
}

#ifdef	BOARD_STBee
#define		XTAL_12MHz
#endif
#ifdef	BOARD_STBeeMini
#define		XTAL_12MHz
#endif

#ifndef	BOARD_STBeeMini
#define	LED_ON	1	// LEDが正論理の場合.
#else
#define	LED_ON	0	// LEDが負論理の場合.
#endif
//-----------------------------------------------
//	
//-----------------------------------------------
void setupCLK (void)
{
	/* enable HSE */
	SET_REG(RCC_CR,GET_REG(RCC_CR) | 0x00010001);
	while ((GET_REG(RCC_CR) & 0x00020000) == 0); /* for it to come on */

	/* enable flash prefetch buffer */
	SET_REG(FLASH_ACR, 0x00000012);

#ifdef	XTAL_12MHz
	/* Configure PLL */
//  RCC_PLLMUL_6 = (0x4 << 18) 0001_0000
	SET_REG(RCC_CFGR,GET_REG(RCC_CFGR) | 0x00110400);  /* pll=72Mhz,APB1=36Mhz,AHB=72Mhz */
#else
	/* Configure PLL */
//  RCC_PLLMUL_9 = (0x7 << 18) 0001_1100
	SET_REG(RCC_CFGR,GET_REG(RCC_CFGR) | 0x001D0400);  /* pll=72Mhz,APB1=36Mhz,AHB=72Mhz */
#endif
	SET_REG(RCC_CR,GET_REG(RCC_CR)     | 0x01000000);  /* enable the pll */
	while ((GET_REG(RCC_CR) & 0x03000000) == 0);         /* wait for it to come on */

	/* Set SYSCLK as PLL */
	SET_REG(RCC_CFGR,GET_REG(RCC_CFGR) | 0x00000002);
	while ((GET_REG(RCC_CFGR) & 0x00000008) == 0); /* wait for it to come on */
}

#if	0
//-----------------------------------------------
//	
//-----------------------------------------------
void setupFLASH()
{
	/* configure the HSI oscillator */
	if ((pRCC->CR & 0x01) == 0x00) {
		u32 rwmVal = pRCC->CR;
		rwmVal |= 0x01;
		pRCC->CR = rwmVal;
	}

	/* wait for it to come on */
	while ((pRCC->CR & 0x02) == 0x00) {}
}

//-----------------------------------------------
//	
//-----------------------------------------------
bool checkUserCode (u32 usrAddr)
{
	u32 sp = *(vu32*) usrAddr;

	if ((sp & 0x2FFE0000) == 0x20000000) {
		return (TRUE);
	} else {
		return (FALSE);
	}
}

//-----------------------------------------------
//	
//-----------------------------------------------
void jumpToUser (u32 usrAddr)
{
	typedef void (*funcPtr)(void);

	u32 jumpAddr = *(vu32*) (usrAddr + 0x04); /* reset ptr in vector table */
	funcPtr usrMain = (funcPtr) jumpAddr;

	/* tear down all the dfu related setup */
	// disable usb interrupts, clear them, turn off usb, set the disc pin
	// todo pick exactly what we want to do here, now its just a conservative
	flashLock();
	usbDsbISR();
	nvicDisableInterrupts();
	setPin(GPIOC,12); // disconnect usb from host. todo, macroize pin
	systemReset(); // resets clocks and periphs, not core regs


	__MSR_MSP(*(vu32*) usrAddr);              /* set the users stack ptr */

	usrMain();                                /* go! */
}

//-----------------------------------------------
//	
//-----------------------------------------------
void nvicInit(NVIC_InitTypeDef* NVIC_InitStruct)
{
	u32 tmppriority = 0x00;
	u32 tmpreg      = 0x00;
	u32 tmpmask     = 0x00;
	u32 tmppre      = 0;
	u32 tmpsub      = 0x0F;

	SCB_TypeDef* rSCB = (SCB_TypeDef *) SCB_BASE;
	NVIC_TypeDef* rNVIC = (NVIC_TypeDef *) NVIC_BASE;


	/* Compute the Corresponding IRQ Priority --------------------------------*/
	tmppriority = (0x700 - (rSCB->AIRCR & (u32)0x700))>> 0x08;
	tmppre = (0x4 - tmppriority);
	tmpsub = tmpsub >> tmppriority;

	tmppriority = (u32)NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority << tmppre;
	tmppriority |=  NVIC_InitStruct->NVIC_IRQChannelSubPriority & tmpsub;

	tmppriority = tmppriority << 0x04;
	tmppriority = ((u32)tmppriority) << ((NVIC_InitStruct->NVIC_IRQChannel & (u8)0x03) * 0x08);

	tmpreg = rNVIC->IPR[(NVIC_InitStruct->NVIC_IRQChannel >> 0x02)];
	tmpmask = (u32)0xFF << ((NVIC_InitStruct->NVIC_IRQChannel & (u8)0x03) * 0x08);
	tmpreg &= ~tmpmask;
	tmppriority &= tmpmask;
	tmpreg |= tmppriority;

	rNVIC->IPR[(NVIC_InitStruct->NVIC_IRQChannel >> 0x02)] = tmpreg;

	/* Enable the Selected IRQ Channels --------------------------------------*/
	rNVIC->ISER[(NVIC_InitStruct->NVIC_IRQChannel >> 0x05)] =
	    (u32)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (u8)0x1F);
}


//-----------------------------------------------
//	
//-----------------------------------------------
void nvicDisableInterrupts()
{
	NVIC_TypeDef* rNVIC = (NVIC_TypeDef *) NVIC_BASE;
	rNVIC->ICER[0] = 0xFFFFFFFF;
	rNVIC->ICER[1] = 0xFFFFFFFF;
	rNVIC->ICPR[0] = 0xFFFFFFFF;
	rNVIC->ICPR[1] = 0xFFFFFFFF;

	SET_REG(STK_CTRL,0x04); /* disable the systick, which operates separately from nvic */
}


//-----------------------------------------------
//	
//-----------------------------------------------
void systemHardReset(void)
{
	SCB_TypeDef* rSCB = (SCB_TypeDef *) SCB_BASE;

	/* Reset  */
	rSCB->AIRCR = (u32)AIRCR_RESET_REQ;

	/*  should never get here */
	while (1) {
		asm volatile("nop");
	}
}

#endif

//-----------------------------------------------
//	
//-----------------------------------------------
bool flashErasePage(u32 pageAddr)
{
	u32 rwmVal = GET_REG(FLASH_CR);
	rwmVal = FLASH_CR_PER;
	SET_REG(FLASH_CR,rwmVal);

	while (GET_REG(FLASH_SR) & FLASH_SR_BSY) {}
	SET_REG(FLASH_AR,pageAddr);
	SET_REG(FLASH_CR,FLASH_CR_START | FLASH_CR_PER);
	while (GET_REG(FLASH_SR) & FLASH_SR_BSY) {}

	/* todo: verify the page was erased */

	rwmVal = 0x00;
	SET_REG(FLASH_CR,rwmVal);

	return TRUE;
}
bool flashErasePages(u32 pageAddr, u16 n)
{
	while (n-->0) {
		if (!flashErasePage(pageAddr+0x400*n)) {
			return FALSE;
		}
	}

	return TRUE;
}

//-----------------------------------------------
//	
//-----------------------------------------------
int flashWriteWord(u32 addr, u32 word)
{
	vu16 *flashAddr = (vu16*)addr;
	vu32 lhWord =  (vu32)word & 0x0000FFFF;
	vu32 hhWord = ((vu32)word & 0xFFFF0000)>>16;

	u32 rwmVal = GET_REG(FLASH_CR);

	SET_REG(FLASH_CR,FLASH_CR_PG);

	/* apparently we need not write to FLASH_AR and can
	   simply do a native write of a half word */
	while (GET_REG(FLASH_SR) & FLASH_SR_BSY) {}
	*(flashAddr+0x01) = (vu16)hhWord;

	while (GET_REG(FLASH_SR) & FLASH_SR_BSY) {}
	*(flashAddr) = (vu16)lhWord;

	while (GET_REG(FLASH_SR) & FLASH_SR_BSY) {}

	rwmVal &= 0xFFFFFFFE;
	SET_REG(FLASH_CR,rwmVal);

	/* verify the write */
	if (*(vu32*)addr != word) {
		return 1;
	}

	return 0;
}

//-----------------------------------------------
//	
//-----------------------------------------------
void flashLock()
{
	/* take down the HSI oscillator? it may be in use elsewhere */

	/* ensure all FPEC functions disabled and lock the FPEC */
	SET_REG(FLASH_CR,0x00000080);
}

//-----------------------------------------------
//	
//-----------------------------------------------
void flashUnlock()
{
	/* unlock the flash */
	SET_REG(FLASH_KEYR,FLASH_KEY1);
	SET_REG(FLASH_KEYR,FLASH_KEY2);
}



//-----------------------------------------------
//	
//-----------------------------------------------
static void setupNVIC()
{
//	nvic_init(USER_ADDR_ROM, 0);
    nvic_init(0x08000000 | ROMADRS , 0);
}


int	getBootSW(void);
void call_func(int adrs);

//extern	char _estack[];
extern	char __cs3_stack[];


#if	0
//register char *stack_ptr asm ("sp");
/********************************************************************
 *	hot start (bootコマンド)されたかどうかをSTACK LEVELで判定する.
 ********************************************************************
 *	戻り値:
 *		0: HOT START
 *		1: COLD START
 */
static int check_cold(int sp)
{
//	int sp0 = _estack;
	int sp0 = (int) __cs3_stack;

	int level = sp0-sp;
	
//	int *p = (int*) 0x20002008;
//	p[0]=level;

	if(level < 0x20 ) return 1;	// COLD START
	return 0;					// HOT START

//	トリック: 
//		COLD STARTした場合は、CPUのリセットシーケンスにより、
//		sp = _estack(0番地) という初期化が実行される.
//
//		HOT START した場合(bootコマンド実行時) はspの初期値代入
//		が行われない(手抜きしている)ため、COLD START時よりスタックが深くなる.
//
//		この違いを利用して、HOT START時はBOOT JUMPERを無視させることが可能.
}
#endif



#if	BOOTLOADER_MODE
static void check_jumper_sw(int sp)
{
	int	boot_jumper = getBootSW();
//	if(	check_cold(sp) ) 
	{
		if(	boot_jumper ) {	//ジャンパーオープン.

			int *resvec = (int *) 0x08003000;	// reset vector
			int adrs = resvec[1];
			if((adrs & 0xfff00000) == 0x08000000) {	// 0x0800_0000 ～ 0x080f_ffff までの間にjump.
				call_func(adrs);
			}

		}
	}
}

#else

static void check_jumper_sw(int sp)
{
	(void)sp;
}

#endif

void disableDebugPorts(void);
void afio_init(void);



//-----------------------------------------------
//	
//-----------------------------------------------
void init()
{
	int sp;

	systemReset();
	setupCLK();
	setupNVIC();
	gpio_init_all();
    afio_init();
#ifdef	BOARD_STBeeMini
	disableDebugPorts();
#endif

//LEDtest();

	check_jumper_sw( (int) &sp);
	systick_init(SYSTICK_RELOAD_VAL);
    PinMode(BOARD_LED_PIN, OUTPUT);
	DigitalWrite(BOARD_LED_PIN,LED_ON);
	setupUSB();
}

