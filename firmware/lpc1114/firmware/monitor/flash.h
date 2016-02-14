#ifndef	_flash_h_
#define	_flash_h_

#ifndef	SUBMDL_STM32F103
//
//	STM32のペリフェラル関数で既に用意されているもの.
//
void	FLASH_ProgramWord(int adr,int data);	// 1word単位のFLASH書き込み.
void	FLASH_ErasePage(int adr);				//  page単位のFLASH消去.
void 	FLASH_Lock(void);						// flash書き込みlockを掛ける.
void 	FLASH_Unlock(void);						// flash書き込みlockを外す.
#endif

//
//	LPC1343/LPC2388 などで使用するページ単位の書き込み.
//
void	FLASH_ProgramPage(int adr);				////  page単位のFLASH書き込み/消去.

#endif
