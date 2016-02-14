/********************************************************************
 *	GPIOの制御.
 ********************************************************************
 *
 *	以下のArduino風関数を実装する.
 *
void	pinMode(int	pin, int mode)
void	digitalWrite(int pin, int value)
int		digitalRead(int pin)
 *
 ********************************************************************
 */

#include "mcu.h"

#include "monit.h"
#include "gpio.h"

//#define bFM3_GPIO_PFR0_P0   *((volatile unsigned int*)(0x42660000UL))
//#define bFM3_GPIO_PCR0_P0   *((volatile unsigned int*)(0x42662000UL))
//#define bFM3_GPIO_DDR0_P0   *((volatile unsigned int*)(0x42664000UL))
//#define bFM3_GPIO_PDIR0_P0  *((volatile unsigned int*)(0x42666000UL))
//#define bFM3_GPIO_PDOR0_P0  *((volatile unsigned int*)(0x42668000UL))

#define	GPIO_PFR_BASE  (&bFM3_GPIO_PFR0_P0) //機能設定(GPIO=0,その他=1)
#define	GPIO_PCR_BASE  (&bFM3_GPIO_PCR0_P0) //PullUP (1=プルアップ)
#define	GPIO_DDR_BASE  (&bFM3_GPIO_DDR0_P0) //方向(入力=0 , 出力=1)
#define	GPIO_PDIR_BASE (&bFM3_GPIO_PDIR0_P0)//端子入力データ.
#define	GPIO_PDOR_BASE (&bFM3_GPIO_PDOR0_P0)//端子出力データ.

#define	PINMASK	0x1ff
/********************************************************************
 *	GPIOの制御.
 ********************************************************************
 */
void	pinMode(int	pin, int mode)
{
	GPIO_DDR_BASE[pin & PINMASK] = mode;
}
/********************************************************************
 *	GPIO PullUpの制御. (mode = 1ならpullup)
 ********************************************************************
 */
void	pinModePullup(int pin, int mode)
{
	GPIO_PCR_BASE[pin & PINMASK] = mode;
}
/********************************************************************
 *	GPIO[0] /特殊機能[1] の切り替え.
 ********************************************************************
 */
void	pinModeEx(int	pin, int mode)
{
	GPIO_PFR_BASE[pin & PINMASK] = mode;
}
/********************************************************************
 *	GPIOの制御.
 ********************************************************************
 */
void	digitalWrite(int pin, int value)
{
	GPIO_PDOR_BASE[pin & PINMASK] = value;
}

/********************************************************************
 *	GPIOの制御.
 ********************************************************************
 */
int		digitalRead(int pin)
{
	return GPIO_PDIR_BASE[pin & PINMASK];
}

/********************************************************************
 *	GPIOの制御.
 ********************************************************************
 */
