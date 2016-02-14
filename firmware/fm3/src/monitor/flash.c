/**********************************************************************
 *	FM3 Flash自己書き換え関数(fm3_flash.c) をRAM上に転送して実行する.
 **********************************************************************
 */

#include <stdio.h>
#include <string.h>
#include "flash.h"



//	FM3_FlashEraseWrite関数のwordサイズ.
#define	FUNC_SIZE	0x80

//	転送先RAM
static	int	func_buffer[FUNC_SIZE];	// 512byte

//	RAM上に置く関数のプロトタイプ.
typedef int (*t_FlashEraseWrite)(int cmd,int adrs,short *buf,int size);

t_FlashEraseWrite FlashEraseWrite;


//	FLASH の書き込みbufferサイズ.
#define	PAGE_SIZE			4096
#define	PAGE_OFFSET_MASK	(PAGE_SIZE-1)
#define	PAGE_SHIFT			12

//	書き込みバッファ.
#define		SRC_SIZE	PAGE_SIZE
int	 srcbuf[SRC_SIZE/sizeof(int)];

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
	adrs は 0x0_0000 (16k)
			0x0_4000 (16k)
			0x0_8000 (96k)
			0x2_0000 (128k)
			0x4_0000 (128k)
			0x6_0000 (128k)
			0x8_0000 (128k)
			0xa_0000 (128k)
			0xc_0000 (128k)
			0xe_0000 (128k)
	に限る.
 */
static	void flash_erase(int adrs)
{
	if(adrs < 0x004000) return;	// ROM番地16kをアンダーラン.
	if(adrs >=0x100000) return;	// ROM番地1MBをオーバーラン.

	//上記アドレス以外を撥ねる.

	if(adrs & 0x01ffff) {		// 128k境界でない.
		if(adrs > 0x8000 ) {
			return;				// 128k境界でないのでエラー.
		}else{					// 0,4000,8000は例外的にOK
			if(adrs & 0x3fff) return;
		}
	}

	// ページ消去の実行.

	FlashEraseWrite(FM3_ERASE_CMD,adrs,NULL,0);
}

/********************************************************************
 *	4Kページ全書き込み.
 ********************************************************************
 *	書き込み元: srcbuf[]
 *	書き込み先: adrs
 */
static	void flash_write(int adrs)
{
	if(adrs < 0x004000) return;	// ROM番地16kをアンダーラン.
	if(adrs >=0x100000) return;	// ROM番地1MBをオーバーラン.

	FlashEraseWrite(FM3_WRITE_CMD,adrs,(short*) srcbuf,PAGE_SIZE);
}

/********************************************************************
 *	STM32のペリフェラル関数に合わせたもの(bufferへの書き込みで代行する)
 ********************************************************************
void	FLASH_ProgramWord(int adr,int data)// 1word単位のFLASH書き込み.
 */
void FLASH_ProgramWord(int adr,int data)
{
	int off = adr & PAGE_OFFSET_MASK;

	srcbuf[off/sizeof(int)] = data;
}


/********************************************************************
 *	LPC1343/LPC2388 などで使用するページ単位の書き込み.
 ********************************************************************
	page単位のFLASH書き込みと、次回書き込みのためのbufferデータ初期化.
 */
void FLASH_ProgramPage(int adrs)
{
	flash_write(adrs);
	erase_buf();
}

/********************************************************************
 *	
 ********************************************************************
 */
void FLASH_ErasePage(int adrs)
{
	if((adrs & PAGE_OFFSET_MASK ) == 0) {
		flash_erase(adrs);
		erase_buf();
	}
}


// flash書き込みlockを掛ける.
void 	FLASH_Lock(void)
{
	FlashEraseWrite = NULL;		//関数ポインタをNULLに.
}



// flash書き込みlockを外す.
//	---> FLASHアクセスルーチンをRAMに移す.

void 	FLASH_Unlock(void)
{
	// RAM上に置くFLASHアクセスルーチン.
	char *func_ptr = (char *) func_buffer;
	func_ptr++;	// Thumbステートなので LSBを立てる.

	// ROM上のFLASHアクセスルーチン(呼び出し出来ない).
	int   func_rom = (int) &FM3_FlashEraseWrite;
	func_rom &= ~1;	// LSBの立ったアドレスから、実アドレスに戻す.

	//関数の実体をRAMにコピー.
	memcpy(func_buffer,(void*) func_rom,sizeof(func_buffer));

	//関数ポインタをfunc_bufferのアドレスにセット.
	FlashEraseWrite = (t_FlashEraseWrite) func_ptr;

}


