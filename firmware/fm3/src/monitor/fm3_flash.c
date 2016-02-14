/********************************************************************
 *	FM3 Flash���ȏ�������
 ********************************************************************

int	FM3_FlashEraseWrite(int cmd,int adrs,short *buf,int size);
 cmd=1: erase
 cmd=2: write
 
����:
	- HW�̐����AFlashROM��ł͎��s�ł��܂���.
	- ���̊֐��͕K��RAM��ɃR�s�[���������ŁA���荞�݋֎~��ԂŎ��s����
	  �K�v������܂��B
	- �܂��A�֐�������FlashROM��̊֐���f�[�^��ǂݏo�����Ƃ��o���܂���B
	- erase�̓Z�N�^�[�P�ʂōs�Ȃ��܂��B�Z�N�^�T�C�Y�̓f�o�C�X���ƂɈقȂ�A
	  ����f�o�C�X�ł��قȂ�T�C�Y�̃Z�N�^�������܂��B
	- FM3�̍\����A0-4000�Ԓn�̏����⏑�����݂͕s�ł��B
	�@(vector�����ł���̂ŁA���̊֐����烊�^�[���������ƂŖ\�����܂�)

����:
	- adrs ��FlashROM�̃A�h���X���w���܂��B
	- erase���́A���� buf,size���g���܂���Bsize�͈ÖقɌ��܂��Ă��܂��B
	- write���́A�Z�N�^�[�S�̂łȂ��Ă��������݂ł��܂����Asize��4�̔{���A
	  adrs ��4�̔{���ɂ���K�v������悤�ł��B
 */
#include "common.h"	//FM3�ˑ�.

#include "hidcmd.h"
#include "monit.h"
#include "flash.h"


//	����A�h���X.
#define FM3_ROM_CMD1 (0x00001550)
#define FM3_ROM_CMD2 (0x00000aa8)
#define FM3_ROM_MASK (0xffffc000)	//�A�h���X�}�X�N.

//	�������ݒ���Flash�G���A�ǂݏo���f�[�^(�V�[�P���X�t���O).
#define	DPOL	0x80
#define	TOGG	0x40
#define	TLOV	0x20
#define	SETI	0x08

//	Flash�G���A���w��volatile�ȃ|�C���^.
unsigned short volatile *rom_cmd1;
unsigned short volatile *rom_cmd2;
unsigned short volatile *rom_cmdp;

//	�_�~�[���[�h�Ŏg�p����i�[�ꏊ.
int		 dummy_val;

//	Flash�G���A�փf�[�^���������ރ}�N��.
#define	ROMCMD1(x)	*rom_cmd1 = (x)
#define	ROMCMD2(y)	*rom_cmd2 = (y)
#define	ROMCMDP(z)	*rom_cmdp = (z)

//	�R�}���h�V�[�P���X�FFlash����.
#define	ERASE_CMD()	\
		ROMCMD1(0xaa);	\
		ROMCMD2(0x55);	\
		ROMCMD1(0x80);	\
		ROMCMD1(0xaa);	\
		ROMCMD2(0x55);

//	�R�}���h�V�[�P���X�FFlash��������.
#define	WRITE_CMD()	\
		ROMCMD1(0xaa);	\
		ROMCMD2(0x55);	\
		ROMCMD1(0xa0);



/********************************************************************
 *	FlashROM�̏����A�܂��͏������݂� RAM��Ŏ��s����.
 ********************************************************************
    ���̊֐���FlashROM��̃R�[�h�𒼐ڌĂ�ł͂����܂���B

    RAM��ɃR�s�[�������̂��Ăяo���Ă��������B

 */
int	FM3_FlashEraseWrite(int cmd,int adrs,short *buf,int size)
{
	int i,d0,d1,d2,d7,data;
	int err  = 0;
	int bank = adrs & FM3_ROM_MASK;
	FM3_FLASH_IF_TypeDef *flash_if=FM3_FLASH_IF;

	__disable_irq();
	__disable_fault_irq();

	rom_cmd1 = (ushort *) (FM3_ROM_CMD1);	// =0x1550
	rom_cmd2 = (ushort *) (FM3_ROM_CMD2);	// =0x0aa8
	rom_cmdp = (ushort *) (bank);

	flash_if->FASZR = 0x01;	// '01'	Flash ROM 16bit�A�N�Z�X
	dummy_val = flash_if->FASZR;	// �_�~�[���[�h.
	(void) dummy_val;

	if(	cmd == FM3_ERASE_CMD) {
		{
			ERASE_CMD();		//�����V�[�P���X�R�}���h���s.
			ROMCMDP(0x30);		//�����A�h���X��0x30����������.
			d0 = *rom_cmdp;		//�_�~�[���[�h.
			(void) d0;
#if	1		// FlashROM�̍\����32bit + 32bit�\���Ȃ̂ŁA
			// ����[�h�̃G���A(xxxxxxx4�Ȃ�)	���ꏏ�ɏ���.
			{
				rom_cmdp[2] = 0x30;
				d7 = *rom_cmdp & TOGG;	//�P��ړǂݏo�����s.
				d1 = *rom_cmdp;			//�Q��ړǂݏo�����s.
				// d1��SETI�r�b�g�Ď�(35uS���߃`�F�b�N)�͂����ł͏ȗ�
			}
#endif
			//��������?
			while(1) {
				d7 = *rom_cmdp & TOGG;	//�P��ړǂݏo�����s.
				d1 = *rom_cmdp;			//�Q��ړǂݏo�����s.
				if((d1 & TOGG)==d7) break;	//���]�f�[�^�łȂ���Δ�����.

				if((d1 & TLOV)) {	//TLOV==1�Ȃ�A�^�C�~���O���~�b�g����.
					d7 = *rom_cmdp & TOGG;	//�P��ړǂݏo�����s.
					d1 = *rom_cmdp;			//�Q��ړǂݏo�����s.
					if((d1 & TOGG)==d7) break;		//���]�f�[�^�łȂ���Δ�����.
					//�������݃G���[����.
					err=1;break;
				}
				//TLOV==0�Ȃ�while���[�v�̓���.
			}
		}
	}
	if(	cmd == FM3_WRITE_CMD) {
		size = (size+1) >> 1;
		rom_cmdp = (ushort *) (adrs);
		for(i=0;i<size;i++) {
			data = *buf++;
			d7 = data & DPOL;	//�������݃f�[�^��bit7.
			WRITE_CMD();		//�������݃V�[�P���X�R�}���h���s.
			ROMCMDP(data);		//�n�[�t���[�h��������.
			d0 = *rom_cmdp;		//�_�~�[���[�h.
			(void) d0;

			//�������݊���?
			while(1) {
				d1 = *rom_cmdp;	//�ǂݏo�����s.
				if((d1 & DPOL)==d7) break;		//���]�f�[�^�łȂ���Δ�����.

				if((d1 & TLOV)) {	//TLOV==1�Ȃ�A�^�C�~���O���~�b�g����.
					d2 = *rom_cmdp;	//����ɓǂݏo�����s.
					if((d2 & DPOL)==d7) break;		//���]�f�[�^�łȂ���Δ�����.
					//�������݃G���[����.
					err=1;break;
				}
				//TLOV==0�Ȃ�while���[�v�̓���.
			}
			if(err) break;	//�������݃G���[����.
			
			//���̃A�h���X.
			rom_cmdp++;
		}
	}

	flash_if->FASZR = 0x02;	// '10'	Flash ROM 32bit�A�N�Z�X
	dummy_val = flash_if->FASZR;	// �_�~�[���[�h.
	(void) dummy_val;

	__enable_irq();
	__enable_fault_irq();

	return err;
}

/********************************************************************
 *
 ********************************************************************
 */
