/********************************************************************
 *	FLASH 書き換え
 ********************************************************************

LPC2388の512kBのFlashの構成
 先頭と後尾の32kBは4kB/sector.
 その間(0x8000～0x77fff)は32kB/sector
になっている。
 
セクタ番号は先頭から0,1,2...と振られているのでアドレスから単純計算
 出来ない.

ERASEやPREPAREは32kb単位になることはあるが、書き込み(COPY RAMtoFLASH)
は 256,512,1024,4096 のいずれかの単位を選べる.(先頭アラインメントは256)

ERASEやCOPYRAMtoFLASHの前には必ずPREPAREがそれぞれ必要。

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

//	CPUクロック.
#define	CLOCK_KHz	72000

//	FLASH のページサイズ.
#define	PAGE_SIZE			4096
#define	PAGE_OFFSET_MASK	(PAGE_SIZE-1)
#define	PAGE_SHIFT			12

//	コマンド番号.
#define	IAP_PREPARE			50
#define	IAP_RAM2FLASH		51
#define	IAP_ERASE			52

//	書き込みバッファ.
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
		page = adr >> PAGE_SHIFT;	// 4kB/PAGEの場合.
		return page;
	}
	if(adr < 0x78000) {
		page = adr >> 15;			// 32kB/PAGEの場合.
		return (7 + page);			// 8,9,a,b ...
	}
//	if(adr < 0x80000) 
	{
		adr &= 0x7fff;
		page = adr >> PAGE_SHIFT;	// 4kB/PAGEの場合.
		return 0x16 + page;			// 0x16(22),0x17...,0x1b
	}
	return page;
}
int	addr2erase(int adr)
{
	int page = adr >> PAGE_SHIFT;	// 4kB単位で数える.
	if((adr >= 0x8000) && (adr < 0x78000)) {
		//この範囲の場合は、32kB境界でのみERASEする.
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
 *	BOOT-ROM のエントリーポイントを呼び出す.
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
 *	PAGE buffer [] 4kB を 0xffで埋める.
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
 *	FLASH 消去
 ********************************************************************
 */
static	void flash_erase(int dst)
{
	int f_page = addr2page(dst);
	int rc;

	if( addr2erase(dst) == 0) return;	// 32kB/Sectorの場合は先頭でのみ消去を実行.
	// ToDo. 32kB/Sectorの領域の途中から書き込みする場合に、消去されない場合がある.
	//		 とりあえず手動で消去してもらうことで対処.

	rc = call_IAP(IAP_PREPARE,f_page,f_page	  ,CLOCK_KHz,0);
//	hex_print("prepare=",rc);

	rc = call_IAP(IAP_ERASE  ,f_page,f_page	  ,CLOCK_KHz,0);
//	hex_print("erase=",rc);
}

/********************************************************************
 *	4Kページ全書き込み.
 ********************************************************************
 *	書き込み元: srcbuf[]
 *	書き込み先: dst  ( 4kB 単位)
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
