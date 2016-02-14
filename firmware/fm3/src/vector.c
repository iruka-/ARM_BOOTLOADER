//
//	=====================================================================
//	���Z�b�g����̃u�[�g���[�_�[�ŗL�̏���.
//	=====================================================================
//	�T�v:
//		0000_0000 �Ԓn�J�n�̃u�[�g���[�_�[�Ƃ��Ďg�p����ꍇ�ɁA
//		���[�U�[���t�@�[���E�F�A(0000_4000�Ԓn) ���N��������W�����p�[��
//		ON�Ȃ�A������ɐ؂�ւ��ċN������.
//
//		�����ЂƂ̋@�\�Ƃ��ẮAarmon/armboot��boot�R�}���h�����s���ꂽ
//		�ꍇ�ɁA�\�t�g�E�F�A���Z�b�g����ɂ�����iRAM��̏����R�[�h�ɂ��j
//		���ʂ��āA���l�̃��[�U�[���t�@�[���E�F�A���N�����鏈���Ɉڂ�.
//
//	����:
//		���̊֐� check_user_boot() ��crt0.c�̏������ȑO�ɌĂяo����邽�߁A
//		.data�Z�N�V�����̃R�s�[��.bss�Z�N�V�����̃N���A���s�Ȃ��Ă��Ȃ�
//		(����Ɋe��I/O���������s�Ȃ��Ă��Ȃ�)��ԂŌĂяo����邱�Ƃɒ���!
//
#include "mcu.h"

#if	_ROMADRS

//
//	���Z�b�g�x�N�^�[.
//
void SetSoftwareResetVector(int *vect)
{
	// dummy.
}
	// ---------------------------------------------------
	// 0000_4000�Ԓn�Ƀr���h����ꍇ�͕s�v.
	// ---------------------------------------------------

#else

	//
	// ---------------------------------------------------
	// 0000_0000�Ԓn�Ƀr���h����ꍇ�͈ȉ��̂悤��,
	// RAM�x�N�^�[�֔�ԗ]���ȃe�[�u�����ǉ������.
	// ---------------------------------------------------
	//


#include "monitor/gpio.h"



//
// cdctool/cdcboot���烆�[�U�[�v���O������BOOT����@�\������.
//
#define	REBOOT_FUNCTION	(1)	
//
// �d���������APort P39 �� GND�ڒn����Ă����烆�[�U�[�v���O������BOOT�������Ȃ��`.
//
#define	USER_BOOT_FUNCTION	Yes

#define	USER_BOOT_JUMPER	P39			// �W�����p�[�̎w��.
#define	USER_BOOT_VECTOR	0x4000		// ���[�U�[�v���O�����̎w��.


#define	FM3_RAMADRS_LO		0x1fff0000	// RAM�����Ԓn
#define	FM3_RAMADRS_HI		0x20010000
#define	FM3_ROMADRS_LO		0x00000000	// ROM�����Ԓn
#define	FM3_ROMADRS_HI		0x00100000



	// ---------------------------------------------------
	// 0000_0000�Ԓn�Ƀr���h����ꍇ��
	// Software Reset ���ォ�ǂ����𔻒肵�āA
	// �ʂ̃A�v���P�[�V�������N������R�[�h���ǉ������.
	// ---------------------------------------------------
	//
#define	MARK0	0x55aa0f00
#define	MARK1	0x55aa0f01
#define	MARK2	0x55aa0f02

static int software_Reset_State[3]={0,0,0};
static int *software_Reset_New_Vector=0;

//void 	*g_Vectors[64];

void	Reset_Handler(void) ;
#define	ROM_VECTOR	(uint*) 0x00000100
#define	RAM_VECTOR	(uint*) 0x1fff0000

void	check_user_boot()
{

#if	REBOOT_FUNCTION		// cdctool/cdcboot���烆�[�U�[�v���O������BOOT����@�\������.
	if(	(software_Reset_State[0]==MARK0 ) &&
		(software_Reset_State[1]==MARK1 ) &&
		(software_Reset_State[2]==MARK2 )) {

		// Software Reset ����Ɣ���.
		software_Reset_State[0]=0;

		//
		// ���[�U�[�x�N�^�[�A�h���X��VTOR�ɃZ�b�g.
		//
		SCB->VTOR = software_Reset_New_Vector;

		void (*func)(void);
		func = (void (*)()) software_Reset_New_Vector[1];	// SP , PC ,...
		// ���.
		func();
	}
#endif

#ifdef	USER_BOOT_FUNCTION 	// Port P39 �� GND�ڒn����Ă����烆�[�U�[�v���O������BOOT.

	// BootJumper��PULLUP����.
	pinModePullup(USER_BOOT_JUMPER,1);
	
	//
	// 4000�Ԓn�̃��[�U�[�x�N�^�[������.
	//
	int *user_vector_p = (int*) USER_BOOT_VECTOR;
	int user_sp = user_vector_p[0];
	int user_pc = user_vector_p[1];

	//
	// 4000�Ԓn�̃��[�U�[�x�N�^�[�����������ǂ����m�F����.
	//
	if(( user_sp >= FM3_RAMADRS_LO )
	 &&( user_sp <  FM3_RAMADRS_HI )
	 &&( user_pc >= USER_BOOT_VECTOR)
	 &&( user_pc <  FM3_ROMADRS_HI)  ){
		if( digitalRead(USER_BOOT_JUMPER)==0 ) {
			void (*func)(void);
			//
			// 4000�Ԓn�̃��[�U�[�x�N�^�[��VTOR�ɃZ�b�g.
			//
			SCB->VTOR = USER_BOOT_VECTOR;
			//
			// 4000�Ԓn�̃��[�U�[�x�N�^�[�֔��.
			//
			func = (void (*)()) (USER_BOOT_VECTOR+4);	// Initial PC.
			func();
		}
	}
#endif

}

//
//	�\�t�g�E�F�A���Z�b�g�����s���钼�O�ɃA�v���P�[�V�����̃x�N�^�[�e�[�u��
//	��ݒ肷��.
//
void SetSoftwareResetVector(int *vect)
{
	software_Reset_State[0]=MARK0;
	software_Reset_State[1]=MARK1;
	software_Reset_State[2]=MARK2;

	software_Reset_New_Vector = vect;
}


#endif	// _ROMADRS
