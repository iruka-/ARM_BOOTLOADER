/***************************************************************
	CPU固有レジスタ定義 ヘッダ (SH7262/7264用)
***************************************************************/

#ifndef __CPU__
#define __CPU__

#define	CPU_NAME	"SH-2A(SH7262)"	/* SH-2A(SH762) */

#define	LITTLE_ENDIAN		0		/* SH-2Aシリーズ：ビッグエンディアン */


void CPU_EnableInterrupt(void);		/* 割り込み許可(cpu_init.c内に関数を記述) */
int  CPU_DisableInterrupt(void);	/* 割り込み禁止(cpu_init.c内に関数を記述) */




/* クロック設定コントローラ制御レジスタ */
#define FRQCR	((volatile unsigned short int *)(0xFFFE0010))


/* 外部バスコントローラ制御レジスタ */
#define CS0BCR	((volatile unsigned int       *)(0xFFFC0004))
#define CS1BCR	((volatile unsigned int       *)(0xFFFC0008))
#define CS2BCR	((volatile unsigned int       *)(0xFFFC000C))
#define CS3BCR	((volatile unsigned int       *)(0xFFFC0010))
#define CS4BCR	((volatile unsigned int       *)(0xFFFC0014))
#define CS0WCR	((volatile unsigned int       *)(0xFFFC0028))
#define CS1WCR	((volatile unsigned int       *)(0xFFFC0030))
#define CS2WCR	((volatile unsigned int       *)(0xFFFC0034))
#define CS3WCR	((volatile unsigned int       *)(0xFFFC0038))
#define CS4WCR	((volatile unsigned int       *)(0xFFFC003C))

/* ポートコントローラ制御レジスタ */
#define PAIOR0	((volatile unsigned short int *)(0xFFFE3812))
#define PADR1	((volatile unsigned short int *)(0xFFFE3814))
#define PADR0	((volatile unsigned short int *)(0xFFFE3816))
#define PAPR0	((volatile unsigned short int *)(0xFFFE381A))
#define PBCR5	((volatile unsigned short int *)(0xFFFE3824))
#define PBCR4	((volatile unsigned short int *)(0xFFFE3826))
#define PBCR3	((volatile unsigned short int *)(0xFFFE3828))
#define PBCR2	((volatile unsigned short int *)(0xFFFE382A))
#define PBCR1	((volatile unsigned short int *)(0xFFFE382C))
#define PBCR0	((volatile unsigned short int *)(0xFFFE382E))
#define PBIOR1	((volatile unsigned short int *)(0xFFFE3830))
#define PBIOR0	((volatile unsigned short int *)(0xFFFE3832))
#define PBDR1	((volatile unsigned short int *)(0xFFFE3834))
#define PBDR0	((volatile unsigned short int *)(0xFFFE3836))
#define PBPR1	((volatile unsigned short int *)(0xFFFE3838))
#define PBPR0	((volatile unsigned short int *)(0xFFFE383A))
#define PCCR2	((volatile unsigned short int *)(0xFFFE384A))
#define PCCR1	((volatile unsigned short int *)(0xFFFE384C))
#define PCCR0	((volatile unsigned short int *)(0xFFFE384E))
#define PCIOR0	((volatile unsigned short int *)(0xFFFE3852))
#define PCDR0	((volatile unsigned short int *)(0xFFFE3856))
#define PCPR0	((volatile unsigned short int *)(0xFFFE385A))
#define PDCR3	((volatile unsigned short int *)(0xFFFE3868))
#define PDCR2	((volatile unsigned short int *)(0xFFFE386A))
#define PDCR1	((volatile unsigned short int *)(0xFFFE386C))
#define PDCR0	((volatile unsigned short int *)(0xFFFE386E))
#define PDIOR0	((volatile unsigned short int *)(0xFFFE3872))
#define PDDR0	((volatile unsigned short int *)(0xFFFE3876))
#define PDPR0	((volatile unsigned short int *)(0xFFFE387A))
#define PECR1	((volatile unsigned short int *)(0xFFFE388C))
#define PECR0	((volatile unsigned short int *)(0xFFFE388E))
#define PEIOR0	((volatile unsigned short int *)(0xFFFE3892))
#define PEDR0	((volatile unsigned short int *)(0xFFFE3896))
#define PEPR0	((volatile unsigned short int *)(0xFFFE389A))
#define PFCR3	((volatile unsigned short int *)(0xFFFE38A8))
#define PFCR2	((volatile unsigned short int *)(0xFFFE38AA))
#define PFCR1	((volatile unsigned short int *)(0xFFFE38AC))
#define PFCR0	((volatile unsigned short int *)(0xFFFE38AE))
#define PFIOR0	((volatile unsigned short int *)(0xFFFE38B2))
#define PFDR0	((volatile unsigned short int *)(0xFFFE38B6))
#define PFPR0	((volatile unsigned short int *)(0xFFFE38BA))
#define PGCR7	((volatile unsigned short int *)(0xFFFE38C0))
#define PGCR6	((volatile unsigned short int *)(0xFFFE38C2))
#define PGCR5	((volatile unsigned short int *)(0xFFFE38C4))
#define PGCR4	((volatile unsigned short int *)(0xFFFE38C6))
#define PGCR3	((volatile unsigned short int *)(0xFFFE38C8))
#define PGCR2	((volatile unsigned short int *)(0xFFFE38CA))
#define PGCR1	((volatile unsigned short int *)(0xFFFE38CC))
#define PGCR0	((volatile unsigned short int *)(0xFFFE38CE))
#define PHCR1	((volatile unsigned short int *)(0xFFFE38EC))
#define PHCR0	((volatile unsigned short int *)(0xFFFE38EE))
#define PJCR2	((volatile unsigned short int *)(0xFFFE390A))
#define PJCR1	((volatile unsigned short int *)(0xFFFE390C))
#define PJCR0	((volatile unsigned short int *)(0xFFFE390E))
#define PKCR2	((volatile unsigned short int *)(0xFFFE392A))
#define PKCR1	((volatile unsigned short int *)(0xFFFE392C))
#define PKCR0	((volatile unsigned short int *)(0xFFFE392E))


/* モジュールスタンバイコントローラ制御レジスタ */
#define STBCR4	((volatile unsigned char      *)(0xFFFE040C))


/* 割り込みコントローラ制御レジスタ */
#define ICR0	((volatile unsigned short int *)(0xFFFE0800))
#define ICR1	((volatile unsigned short int *)(0xFFFE0802))
#define IRQRR	((volatile unsigned short int *)(0xFFFE0806))
#define IPR01	((volatile unsigned short int *)(0xFFFE0818))



#endif
