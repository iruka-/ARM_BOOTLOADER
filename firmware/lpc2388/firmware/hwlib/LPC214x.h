
// not supported.

#define MAMCR		*(volatile unsigned int *)0xE01FC000
#define MAMTIM		*(volatile unsigned int *)0xE01FC004

#define PLLCON		*(volatile unsigned int *)0xE01FC080
#define PLLCFG		*(volatile unsigned int *)0xE01FC084
#define PLLSTAT		*(volatile unsigned int *)0xE01FC088
#define PLLFEED		*(volatile unsigned int *)0xE01FC08C

#define VPBDIV		*(volatile unsigned int *)0xE01FC100



// interrupts
#define VICIntSelect   *((volatile unsigned int *) 0xFFFFF00C)
#define VICIntEnable   *((volatile unsigned int *) 0xFFFFF010)
#define VICVectAddr    *((volatile unsigned int *) 0xFFFFF030)
#define VICVectAddr0   *((volatile unsigned int *) 0xFFFFF100)
#define VICVectCntl0   *((volatile unsigned int *) 0xFFFFF200)

#define	INT_VECT_NUM	0

#define IRQ_MASK 0x00000080

