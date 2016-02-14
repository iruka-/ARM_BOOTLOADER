
/* WDT control function */
#define WDTSTA  0x000FE110  /* WDT start function */
#define WDTCLR  0x000FE12A  /* WDT clear function */

/* interrupt handler */
#define RSI_H   0x000FE000  /* forced break    */
#define DBG_H   0x000FE022  /* software break  */
#define STR_H   0x000FE02A  /* step trap trace */
