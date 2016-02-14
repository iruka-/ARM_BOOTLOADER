/********************************************************************
 *	FLASH ��������
 ********************************************************************

LPC2388��512kB��Flash�̍\��
 �擪�ƌ����32kB��4kB/sector.
 ���̊�(0x8000�`0x77fff)��32kB/sector
�ɂȂ��Ă���B
 
�Z�N�^�ԍ��͐擪����0,1,2...�ƐU���Ă���̂ŃA�h���X����P���v�Z
 �o���Ȃ�.

ERASE��PREPARE��32kb�P�ʂɂȂ邱�Ƃ͂��邪�A��������(COPY RAMtoFLASH)
�� 256,512,1024,4096 �̂����ꂩ�̒P�ʂ�I�ׂ�.(�擪�A���C�������g��256)

ERASE��COPYRAMtoFLASH�̑O�ɂ͕K��PREPARE�����ꂼ��K�v�B

 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

#ifdef	LPC2388


//	NXP arm7tdmi-s
#include <string.h>
#include "hwlib/led.h"
#include "hwlib/LPC23xx.h"
#include "hwlib/armVIC.h"
#include "utype.h"
#include "stm32dmy.h"


#else

#include "platform_config.h"
#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "stm32_eval.h"
#include "hidcmd.h"
#include "monit.h"

#endif


#ifdef	LPC2388
//	ARM7 Thumb code.
#define	IAP_ENTRY_ADDR	0x7ffffff1
#else
//	LPC1343
#define	IAP_ENTRY_ADDR	0x1fff1ff1
#endif

//	CPU�N���b�N.
#define	CLOCK_KHz	72000

//	FLASH �̃y�[�W�T�C�Y.
#define	PAGE_SIZE			4096
#define	PAGE_OFFSET_MASK	(PAGE_SIZE-1)
#define	PAGE_SHIFT			12

//	�R�}���h�ԍ�.
#define	IAP_PREPARE			50
#define	IAP_RAM2FLASH		51
#define	IAP_ERASE			52

//	�������݃o�b�t�@.
#define		SRC_SIZE	PAGE_SIZE
int	 srcbuf[SRC_SIZE/sizeof(int)];



typedef void (*IAP)(uint32_t [], uint32_t []);

IAP iap_entry = (IAP)IAP_ENTRY_ADDR;

uint32_t command[5], result[4];


#ifdef	LPC2388
int	addr2page(int adr)
{
	int page;
	if(adr < 0x8000) {
		page = adr >> PAGE_SHIFT;	// 4kB/PAGE�̏ꍇ.
		return page;
	}
	if(adr < 0x78000) {
		page = adr >> 15;			// 32kB/PAGE�̏ꍇ.
		return (7 + page);			// 8,9,a,b ...
	}
//	if(adr < 0x80000) 
	{
		adr &= 0x7fff;
		page = adr >> PAGE_SHIFT;	// 4kB/PAGE�̏ꍇ.
		return 0x16 + page;			// 0x16(22),0x17...,0x1b
	}
	return page;
}
int	addr2erase(int adr)
{
	int page = adr >> PAGE_SHIFT;	// 4kB�P�ʂŐ�����.
	if((adr >= 0x8000) && (adr < 0x78000)) {
		//���͈̔͂̏ꍇ�́A32kB���E�ł̂�ERASE����.
		if((page & 7)==0) return 1;
		return 0;
	}
	return 1;
}
#else

int	addr2page(int adr)
{
	int page = adr >> PAGE_SHIFT;
	return page;
}

int	addr2erase(int adr)
{
	return 1;	// true
}
#endif


/********************************************************************
 *	BOOT-ROM �̃G���g���[�|�C���g���Ăяo��.
 ********************************************************************
 */
static	int	call_IAP(int cmd,int arg1,int arg2,int arg3,int arg4)
{
	/* Send Reinvoke ISP command to ISP entry point*/
	command[0] = cmd;
	command[1] = arg1;
	command[2] = arg2;
	command[3] = arg3;
	command[4] = arg4;

	/* Set stack pointer to ROM value (reset default) This must be the last
	   piece of code executed before calling ISP, because most C expressions
	   and function returns will fail after the stack pointer is changed. */
	// __set_MSP(*((uint32_t *)0x1FFF0000)); /* inline asm function */

	/* Enter ISP. We call "iap_entry" to enter ISP because the ISP entry is done
	   through the same command interface as IAP. */
	{
		int cpsr = disableIRQ();

		iap_entry(command, result);

		restoreIRQ(cpsr);
	}
	return result[0];
}

/********************************************************************
 *	PAGE buffer [] 4kB �� 0xff�Ŗ��߂�.
 ********************************************************************
 */
static	void erase_buf(void)
{
	int i;
	//
	for(i=0; i< (PAGE_SIZE/sizeof(int) ); i++) {
		srcbuf[i]=(-1);
	}
}

/********************************************************************
 *	FLASH ����
 ********************************************************************
 */
static	void flash_erase(int dst)
{
	int f_page = addr2page(dst);
	int rc;

	if( addr2erase(dst) == 0) return;	// 32kB/Sector�̏ꍇ�͐擪�ł̂ݏ��������s.
	// ToDo. 32kB/Sector�̗̈�̓r�����珑�����݂���ꍇ�ɁA��������Ȃ��ꍇ������.
	//		 �Ƃ肠�����蓮�ŏ������Ă��炤���ƂőΏ�.

	rc = call_IAP(IAP_PREPARE,f_page,f_page	  ,CLOCK_KHz,0);
//	hex_print("prepare=",rc);

	rc = call_IAP(IAP_ERASE  ,f_page,f_page	  ,CLOCK_KHz,0);
//	hex_print("erase=",rc);
}

/********************************************************************
 *	4K�y�[�W�S��������.
 ********************************************************************
 *	�������݌�: srcbuf[]
 *	�������ݐ�: dst  ( 4kB �P��)
 */
static	void flash_write(int dst)
{
	int f_page = addr2page(dst);
	int bytes = PAGE_SIZE;
	int rc;
	
	
	rc = call_IAP(IAP_PREPARE,f_page,f_page	  ,CLOCK_KHz,0);
//	hex_print("prepare=",rc);

	rc = call_IAP(IAP_RAM2FLASH ,dst,(int) srcbuf , bytes ,CLOCK_KHz);
//	hex_print("write=",rc);
}

/********************************************************************
 *	
 ********************************************************************
 */
#if	0
#define	PAGE 15
void erase_test(void)
{
	int rc,i;
	int bytes = SRC_SIZE;
	int dst = PAGE * PAGE_SIZE;

	for(i=0; i<1024/4; i++) {
		srcbuf[i]=(-1);
	}
	srcbuf[0]=0x55aa;
	srcbuf[255]=0xaa55;

	rc = call_IAP(IAP_PREPARE,PAGE,PAGE	  ,CLOCK_KHz,0);
	hex_print("prepare=",rc);

	rc = call_IAP(IAP_ERASE  ,PAGE,PAGE	  ,CLOCK_KHz,0);
	hex_print("erase=",rc);

	rc = call_IAP(IAP_PREPARE,PAGE,PAGE	  ,CLOCK_KHz,0);
	hex_print("prepare=",rc);

	rc = call_IAP(IAP_RAM2FLASH  ,dst,(int) srcbuf , bytes ,CLOCK_KHz);
	hex_print("write=",dst);
	hex_print("write rc=",rc);
}
#endif

/********************************************************************
 *	
 ********************************************************************
 */
void FLASH_ProgramWord(int adr,int data)
{
	int off = adr & PAGE_OFFSET_MASK;

	srcbuf[off/sizeof(int)] = data;
}

/********************************************************************
 *	
 ********************************************************************
 */
void FLASH_ProgramPage(int adr)
{
	flash_write(adr);
	erase_buf();
}

/********************************************************************
 *	
 ********************************************************************
 */
void FLASH_ErasePage(int adr)
{
	int page;
	if((adr & PAGE_OFFSET_MASK )== 0) {
		flash_erase(adr);
		erase_buf();
	}
}

/********************************************************************
 *	D U M M Y
 ********************************************************************
 */
void FLASH_Lock(void)
{}
void FLASH_Unlock(void)
{}

int	_sdata[1];

/********************************************************************
 *	
 ********************************************************************
 */
