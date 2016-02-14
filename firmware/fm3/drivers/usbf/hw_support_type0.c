/**
* @file          hw_support.c
* @brief         hardware support function
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "cpu_config.h"
/*#include "gpio.h"*/
#include "hw_support.h"
#include "usbfunc_demoapp_config.h"

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/
#define HW_WDT_LOCK_VALUE          (0x1ACCE551) /* lock value of hardware watchdog timer           */
#define BSC_PSR_INIT_VALUE         (0x00)       /* initial value of base clock prescaler register  */
#define APBC0_PSR_INIT_VALUE       (0x01)       /* initial value of APB0 prescaler register        */
#define APBC1_PSR_INIT_VALUE       (0x81)       /* initial value of APB1 prescaler register        */
#define APBC2_PSR_INIT_VALUE       (0x81)       /* initial value of APB2 prescaler register        */
#define TTC_PSR_INIT_VALUE         (0x00)       /* initial value of trace clock prescaler register */
#define CSW_TMR_INIT_VALUE         (0x09)       /* initial value of MAIN/SUB clock stabilization time select register */
#define PSW_TMR_INIT_VALUE         (0x03)       /* initial value of PLL clock stabilization time select register */
#define PLL_CTL1_INIT_VALUE        (0x00)       /* initial value of PLL control register1          */
#define PLL_CTL2_INIT_VALUE        (0x0F)       /* initial value of PLL control register2          */
#define MASTER_CLOCK_SELECT_PLL    (0x02)       /* select master clock(PLL clock)                  */
#define UPCR2_UPOWT_INIT_VALUE     (0x07)       /* initial value of UPCR2 register's UPOWT bits    */
#define UPCR3_UPLLK_INIT_VALUE     (0x00)       /* initial value of UPCR3 register's UPLLK bits    */
#define UPCR4_UPLLN_INIT_VALUE     (0x17)       /* initial value of UPCR4 register's UPLLN bits    */
#define ADE_INIT_VALUE             (0x0000)     /* initial value of A/D channel enable register    */

/*----------------------------------------------------------------------------*/
/* Type     Definition                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Static Variable Definition                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/
#ifdef DEBUG_PRINT
static void Init_UART4(void);
#endif  /* DEBUG_PRINT */


#ifdef	__CPU_MB9B618__
#define	BTIM_IRQn					BTIM0_7_IRQn
#endif

static void set_usb_clock(void);

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

/**
* @brief            mb9b500 startkit system boot process
*
* @param            none
* @return           none
*
**/
void mb9b500_startkit_boot(void)
{
#if	0	//ndef DEBUG_MONITOR

    char c;

    /* HW WDT disable */
//    REG_W(FM3_HWWDT->WDG_LCK, HW_WDT_LOCK_VALUE);
//    REG_W(FM3_HWWDT->WDG_LCK, ~HW_WDT_LOCK_VALUE);
//    REG_W(FM3_HWWDT->WDG_CTL, 0x00);

    /* set the priority grouping in NVIC interrupt controller */
    NVIC_SetPriorityGrouping(__NVIC_PRIO_BITS);

    /* base clock prescaler register initialize */
    /* bit7~bit3(00000): reserved bits */
    /* bit2~bit0(000): 1/1 */
    REG_W(FM3_CRG->BSC_PSR, BSC_PSR_INIT_VALUE);

    /* APB0 prescaler register initialize */
    /* bit7~bit2(000000): reserved bits */
    /* bit1~bit0(01): 1/2 */
    REG_W(FM3_CRG->APBC0_PSR, APBC0_PSR_INIT_VALUE);

    /* APB1 prescaler register initialize */
    /* bit7(1): APB1 clock enable */
    /* bit6~bit5(00): reserved bits */
    /* bit4(1): APB1 bus reset unactivated */
    /* bit3~bit2(00): reserved bits */
    /* bit1~bit0(01): 1/2 */
    REG_W(FM3_CRG->APBC1_PSR, APBC1_PSR_INIT_VALUE);

    /* APB2 prescaler register initialize */
    /* bit7(1): APB2 clock enable */
    /* bit6~bit5(00): reserved bits */
    /* bit4(1): APB2 bus reset unactivated */
    /* bit3~bit2(00): reserved bits */
    /* bit1~bit0(01): 1/2 */
    REG_W(FM3_CRG->APBC2_PSR, APBC2_PSR_INIT_VALUE);

    /* trace clock prescaler register initialize */
    /* bit7~bit1(0000000): reserved bits */
    /* bit0(0): 1/1 */
    REG_W(FM3_CRG->TTC_PSR, TTC_PSR_INIT_VALUE);

    /* MAIN/SUB clock stabilization time select register initialize */
    /* bit7(0): reserved bits */
    /* bit6~bit4(000): 2~10/Fch */
    /* bit3~bit0(0111): 2~11/Fch */
    REG_W(FM3_CRG->CSW_TMR, CSW_TMR_INIT_VALUE);

    /* MAIN clock stabilize interrupt disable */
    REG_W(bFM3_CRG_INT_ENR_MCSE, BIT_CLR);

    /* clear MAIN clock stabilize interrupt flag */
    REG_W(bFM3_CRG_INT_CLR_MCSC, BIT_SET);

    /* start MAIN clock */
    REG_W(bFM3_CRG_SCM_CTL_MOSCE, BIT_SET);

    /* wait for MAIN clock ready */
    while (REG_R(bFM3_CRG_SCM_STR_MORDY) == BIT_CLR)
    {
        ;
    }

    /* PLL clock stabilization time select register initialize */
    /* bit7~bit5(000): reserved bits */
    /* bit4(0): select main clock */
    /* bit3(0): reserved bit */
    /* bit2~bit0(000): 2~9/Fch */
    REG_W(FM3_CRG->PSW_TMR, PSW_TMR_INIT_VALUE);

    /* PLL clock control register1 initialize */
    /* bit7~bit4(0000): PLLK = 1/1 */
    /* bit3~bit0(0000): PLLM = 1/1 */
    REG_W(FM3_CRG->PLL_CTL1, PLL_CTL1_INIT_VALUE);

    /* PLL clock control register2 initialize */
    /* bit7~bit5(000): reserved bits */
    /* bit4~bit0(01111): PLLN = 1/16 */
    REG_W(FM3_CRG->PLL_CTL2, PLL_CTL2_INIT_VALUE);

    /* PLL clock stabilize interrupt disable */
    REG_W(bFM3_CRG_INT_ENR_PCSE, BIT_CLR);

    /* clear PLL clock stabilize interrupt flag */
    REG_W(bFM3_CRG_INT_CLR_PCSC, BIT_SET);

    /* start PLL clock */
    REG_W(bFM3_CRG_SCM_CTL_PLLE, BIT_SET);

    /* wait for PLL clock ready */
    while (REG_R(bFM3_CRG_SCM_STR_PLRDY) == BIT_CLR)
    {
        ;
    }

    /* select PLL clock as the master clock */
    c = REG_R(FM3_CRG->SCM_CTL);
    c = c & 0x1F;
    c = c | (MASTER_CLOCK_SELECT_PLL<<5);
    REG_W(FM3_CRG->SCM_CTL, c);

    /* wait for PLL clock as the source clock */
    while ((REG_R(FM3_CRG->SCM_STR)>>5) != MASTER_CLOCK_SELECT_PLL)
    {
        ;
    }

    /* set USB clock */
//    set_usb_clock();

#endif /* DEBUG_MONITOR */

    /* set USB clock */
    set_usb_clock();

    /* A/D channel enable register initialize */
    REG_W(FM3_GPIO->ADE, ADE_INIT_VALUE);

    /* set interrupt level of reload timer ch.0/ch.1 */
    NVIC_SetPriority(BTIM_IRQn, INTR_LV_10);
	/* enable interrupt in NVIC interrupt controller */
    NVIC_EnableIRQ(BTIM_IRQn);

#ifdef DEBUG_PRINT
    Init_UART4();
#endif  /* DEBUG_PRINT */

    return;
}


#ifdef	__CPU_MB9B500__
/**
* @brief            set usb clock
*
* @param            none
* @return           none
*
**/
static void set_usb_clock(void)
{
    /* disable USB clock */
    REG_W(bFM3_USBCLK_UCCR_UCEN, BIT_CLR);

    /* wait for USB clock stop */
    while (REG_R(bFM3_USBCLK_UCCR_UCEN) != BIT_CLR)
    {
        ;
    }

    /* disable USB-PLL clock */
    REG_W(bFM3_USBCLK_UPCR1_UPLLEN, BIT_CLR);

    /* select PLL macro clock */
    REG_W(bFM3_USBCLK_UCCR_UCSEL, BIT_SET);

    /* select main clock as input clock */
    REG_W(bFM3_USBCLK_UPCR1_UPINC, BIT_CLR);

    /* select clock stabilization time */
    REG_W(FM3_USBCLK->UPCR2, UPCR2_UPOWT_INIT_VALUE);

    /* USB-PLL=Fin*N/K -> 96MHz=4MHz*24/1 */
    /* USB-PLL clock configuration register(K) initialize */
    REG_W(FM3_USBCLK->UPCR3, UPCR3_UPLLK_INIT_VALUE);

    /* USB-PLL clock configuration register(N) initialize */
    REG_W(FM3_USBCLK->UPCR4, UPCR4_UPLLN_INIT_VALUE);

    /* USB-PLL clock stabilize interrupt disable  */
    REG_W(bFM3_USBCLK_UPINT_ENR_UPCSE, BIT_CLR);

    /* enable USB-PLL clock */
    REG_W(bFM3_USBCLK_UPCR1_UPLLEN, BIT_SET);

    /* wait for USB-PLL clock ready */
    while (REG_R(bFM3_USBCLK_UP_STR_UPRDY) == BIT_CLR)
    {
        ;
    }

    /* enable USB clock */
    REG_W(bFM3_USBCLK_UCCR_UCEN, BIT_SET);

    /* wait for 5 cycle */
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();

    /* enable USB controller */
    REG_W(bFM3_USBCLK_USBEN_USBEN, BIT_SET);

    return;
}

#endif	//#ifdef	__CPU_MB9B500__



#ifdef	__CPU_MB9B618__
/**
* @brief            set usb clock for USBETHERNETCLK
*
* @param            none
* @return           none
*
**/
static void set_usb_clock(void)
{
    /* disable USB clock */
    REG_W(bFM3_USBETHERNETCLK_UCCR_UCEN0, BIT_CLR);

    /* wait for USB clock stop */
    while (REG_R(bFM3_USBETHERNETCLK_UCCR_UCEN0) != BIT_CLR)
    {
        ;
    }

    /* disable USB-PLL clock */
    REG_W(bFM3_USBETHERNETCLK_UPCR1_UPLLEN, BIT_CLR);

    /* select PLL macro clock */
    REG_W(bFM3_USBETHERNETCLK_UCCR_UCSEL0, BIT_SET);
    REG_W(bFM3_USBETHERNETCLK_UCCR_UCSEL1, BIT_CLR);

    /* select main clock as input clock */
    REG_W(bFM3_USBETHERNETCLK_UPCR1_UPINC, BIT_CLR);

    /* select clock stabilization time */
    REG_W(FM3_USBETHERNETCLK->UPCR2, UPCR2_UPOWT_INIT_VALUE);

    /* USB-PLL=Fin*N/K -> 96MHz=4MHz*24/1 */
    /* USB-PLL clock configuration register(K) initialize */
    REG_W(FM3_USBETHERNETCLK->UPCR3, UPCR3_UPLLK_INIT_VALUE);

    /* USB-PLL clock configuration register(N) initialize */
    REG_W(FM3_USBETHERNETCLK->UPCR4, 0x3b); //UPLLN •ªŽü”ä N = 60 (•ªŽü).

    REG_W(FM3_USBETHERNETCLK->UPCR5, 0x04); //UPLLM •ªŽü”ä M =  5 (•ªŽü).

    REG_W(FM3_USBETHERNETCLK->UPCR6, 0x02); //UBSR  •ªŽü”ä M =  3 (•ªŽü).

    REG_W(FM3_USBETHERNETCLK->UPCR7, 0x01); //EPLLEN (USB/Ethernet-PLL‚ð’âŽ~‚µ‚È‚¢)


    /* USB-PLL clock stabilize interrupt disable  */
    REG_W(bFM3_USBETHERNETCLK_UPINT_ENR_UPCSE, BIT_CLR);

    /* enable USB-PLL clock */
    REG_W(bFM3_USBETHERNETCLK_UPCR1_UPLLEN, BIT_SET);

    /* wait for USB-PLL clock ready */
    while (REG_R(bFM3_USBETHERNETCLK_UP_STR_UPRDY) == BIT_CLR)
    {
        ;
    }

    /* enable USB clock */
    REG_W(bFM3_USBETHERNETCLK_UCCR_UCEN0, BIT_SET);

    /* wait for 5 cycle */
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();

    /* enable USB controller */
    REG_W(bFM3_USBETHERNETCLK_USBEN0_USBEN0, BIT_SET);

    return;
}

#endif	//#ifdef	__CPU_MB9B618__



#ifdef DEBUG_PRINT
/**
* @brief            UART funciton initialize
*
* @param            none
* @return           none
*
**/
static void Init_UART4(void)
{
    /* PCLK 32 MHz */

    /* IO initialize */
    bFM3_GPIO_PFR0_PB = 1;
    bFM3_GPIO_EPFR08_SOT4B0 = 1;
    bFM3_GPIO_EPFR08_SOT4B1 = 0;

    /* UART0 */
    bFM3_MSF4_UART_SCR_RXE  = 0;
    bFM3_MSF4_UART_SCR_TXE  = 0;
    FM3_MFS4_UART->BGR0     = 0x81;
    FM3_MFS4_UART->BGR1     = 0x06;
    bFM3_MSF4_UART_SCR_UPCL = 0;

    FM3_MFS4_UART->SMR      = 0x01;
    FM3_MFS4_UART->SSR      = 0x00;
    FM3_MFS4_UART->ESCR     = 0;
    FM3_MFS4_UART->SCR      = 0x01;
    bFM3_MSF4_UART_SSR_REC  = 0;

    return;
}

#endif  /* DEBUG_PRINT */
