/**********************************************************************
 *	FM3 Flash���ȏ��������֐�(fm3_flash.c) ��RAM��ɓ]�����Ď��s����.
 **********************************************************************
 */

#include <stdio.h>
#include <string.h>
#include "flash.h"



//	FM3_FlashEraseWrite�֐���word�T�C�Y.
#define	FUNC_SIZE	0x80

//	�]����RAM
static	int	func_buffer[FUNC_SIZE];	// 512byte

//	RAM��ɒu���֐��̃v���g�^�C�v.
typedef int (*t_FlashEraseWrite)(int cmd,int adrs,short *buf,int size);

t_FlashEraseWrite FlashEraseWrite;


//	FLASH �̏�������buffer�T�C�Y.
#define	PAGE_SIZE			4096
#define	PAGE_OFFSET_MASK	(PAGE_SIZE-1)
#define	PAGE_SHIFT			12

//	�������݃o�b�t�@.
#define		SRC_SIZE	PAGE_SIZE
int	 srcbuf[SRC_SIZE/sizeof(int)];

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
	adrs �� 0x0_0000 (16k)
			0x0_4000 (16k)
			0x0_8000 (96k)
			0x2_0000 (128k)
			0x4_0000 (128k)
			0x6_0000 (128k)
			0x8_0000 (128k)
			0xa_0000 (128k)
			0xc_0000 (128k)
			0xe_0000 (128k)
	�Ɍ���.
 */
static	void flash_erase(int adrs)
{
	if(adrs < 0x004000) return;	// ROM�Ԓn16k���A���_�[����.
	if(adrs >=0x100000) return;	// ROM�Ԓn1MB���I�[�o�[����.

	//��L�A�h���X�ȊO�𝛂˂�.

	if(adrs & 0x01ffff) {		// 128k���E�łȂ�.
		if(adrs > 0x8000 ) {
			return;				// 128k���E�łȂ��̂ŃG���[.
		}else{					// 0,4000,8000�͗�O�I��OK
			if(adrs & 0x3fff) return;
		}
	}

	// �y�[�W�����̎��s.

	FlashEraseWrite(FM3_ERASE_CMD,adrs,NULL,0);
}

/********************************************************************
 *	4K�y�[�W�S��������.
 ********************************************************************
 *	�������݌�: srcbuf[]
 *	�������ݐ�: adrs
 */
static	void flash_write(int adrs)
{
	if(adrs < 0x004000) return;	// ROM�Ԓn16k���A���_�[����.
	if(adrs >=0x100000) return;	// ROM�Ԓn1MB���I�[�o�[����.

	FlashEraseWrite(FM3_WRITE_CMD,adrs,(short*) srcbuf,PAGE_SIZE);
}

/********************************************************************
 *	STM32�̃y���t�F�����֐��ɍ��킹������(buffer�ւ̏������݂ő�s����)
 ********************************************************************
void	FLASH_ProgramWord(int adr,int data)// 1word�P�ʂ�FLASH��������.
 */
void FLASH_ProgramWord(int adr,int data)
{
	int off = adr & PAGE_OFFSET_MASK;

	srcbuf[off/sizeof(int)] = data;
}


/********************************************************************
 *	LPC1343/LPC2388 �ȂǂŎg�p����y�[�W�P�ʂ̏�������.
 ********************************************************************
	page�P�ʂ�FLASH�������݂ƁA���񏑂����݂̂��߂�buffer�f�[�^������.
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


// flash��������lock���|����.
void 	FLASH_Lock(void)
{
	FlashEraseWrite = NULL;		//�֐��|�C���^��NULL��.
}



// flash��������lock���O��.
//	---> FLASH�A�N�Z�X���[�`����RAM�Ɉڂ�.

void 	FLASH_Unlock(void)
{
	// RAM��ɒu��FLASH�A�N�Z�X���[�`��.
	char *func_ptr = (char *) func_buffer;
	func_ptr++;	// Thumb�X�e�[�g�Ȃ̂� LSB�𗧂Ă�.

	// ROM���FLASH�A�N�Z�X���[�`��(�Ăяo���o���Ȃ�).
	int   func_rom = (int) &FM3_FlashEraseWrite;
	func_rom &= ~1;	// LSB�̗������A�h���X����A���A�h���X�ɖ߂�.

	//�֐��̎��̂�RAM�ɃR�s�[.
	memcpy(func_buffer,(void*) func_rom,sizeof(func_buffer));

	//�֐��|�C���^��func_buffer�̃A�h���X�ɃZ�b�g.
	FlashEraseWrite = (t_FlashEraseWrite) func_ptr;

}


