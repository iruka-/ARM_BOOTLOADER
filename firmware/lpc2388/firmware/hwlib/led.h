/*********************************************************************
 *	ＣＱ付録ＮＸＰ-ＡＲＭ 基板用	ＬＥＤモニタ.
 *********************************************************************
 */
#ifndef	_led_h_
#define	_led_h_

#include "../type_t.h"

//	1つの PORT は32bit幅. 16bit(U/L)アクセスも可能.
typedef	struct {
	uint32_t DIR;			// +0x00: (R/W)port direction control reg.
						// 1を書くとoutput.
	uint32_t reserved1[3];

	uint32_t MASK;			// +0x10: (R/W)fast mask reg.マスク書き込み bit 0=有効bit.1=無視bit
	uint32_t PIN;			// +0x14: (R/W)pin reg.   生データ読み書き用. マスク有効.
	uint32_t SET;			// +0x18: (R/W)set reg.   1が立っているbitをsetする.マスク有効.
	uint32_t CLR;			// +0x1c: (WO) clear reg. 1が立っているbitをclearする.マスク有効.
} FGPIO_PORT;


#define	LPC_GPIO0	(FGPIO_PORT *) (FIO_BASE_ADDR + 0x00)
#define	LPC_GPIO1	(FGPIO_PORT *) (FIO_BASE_ADDR + 0x20)
#define	LPC_GPIO2	(FGPIO_PORT *) (FIO_BASE_ADDR + 0x40)
#define	LPC_GPIO3	(FGPIO_PORT *) (FIO_BASE_ADDR + 0x60)
#define	LPC_GPIO4	(FGPIO_PORT *) (FIO_BASE_ADDR + 0x80)
                 

FGPIO_PORT *GPIOPortAdr(int portNum);
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir );
void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal );
uint32_t GPIOGetValue( uint32_t portNum, uint32_t bitPosi);
void GPIOInitDir( uint32_t portNum, uint32_t dir );

void LED_On(void);
void LED_Off(void);
void LED_Flip(int period);
void LED_Flip_Loop(int period,int cnt);

void TIMER0_Init(void);

#endif
/*********************************************************************
 *	
 *********************************************************************
 */
