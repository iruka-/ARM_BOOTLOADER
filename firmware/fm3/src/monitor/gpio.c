/********************************************************************
 *	GPIO�̐���.
 ********************************************************************
 *
 *	�ȉ���Arduino���֐�����������.
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

#define	GPIO_PFR_BASE  (&bFM3_GPIO_PFR0_P0) //�@�\�ݒ�(GPIO=0,���̑�=1)
#define	GPIO_PCR_BASE  (&bFM3_GPIO_PCR0_P0) //PullUP (1=�v���A�b�v)
#define	GPIO_DDR_BASE  (&bFM3_GPIO_DDR0_P0) //����(����=0 , �o��=1)
#define	GPIO_PDIR_BASE (&bFM3_GPIO_PDIR0_P0)//�[�q���̓f�[�^.
#define	GPIO_PDOR_BASE (&bFM3_GPIO_PDOR0_P0)//�[�q�o�̓f�[�^.

#define	PINMASK	0x1ff
/********************************************************************
 *	GPIO�̐���.
 ********************************************************************
 */
void	pinMode(int	pin, int mode)
{
	GPIO_DDR_BASE[pin & PINMASK] = mode;
}
/********************************************************************
 *	GPIO PullUp�̐���. (mode = 1�Ȃ�pullup)
 ********************************************************************
 */
void	pinModePullup(int pin, int mode)
{
	GPIO_PCR_BASE[pin & PINMASK] = mode;
}
/********************************************************************
 *	GPIO[0] /����@�\[1] �̐؂�ւ�.
 ********************************************************************
 */
void	pinModeEx(int	pin, int mode)
{
	GPIO_PFR_BASE[pin & PINMASK] = mode;
}
/********************************************************************
 *	GPIO�̐���.
 ********************************************************************
 */
void	digitalWrite(int pin, int value)
{
	GPIO_PDOR_BASE[pin & PINMASK] = value;
}

/********************************************************************
 *	GPIO�̐���.
 ********************************************************************
 */
int		digitalRead(int pin)
{
	return GPIO_PDIR_BASE[pin & PINMASK];
}

/********************************************************************
 *	GPIO�̐���.
 ********************************************************************
 */
