�� SH2A USB-CDC monitor �ɂ���

  ����́Ash7264_usb_function_sample.zip ��gcc�Ή������āA�u�[�g���[�_�[
  �Ƃ��Ďg�p�ł���悤�ɂ������̂ł��B


�� �g����

�i�P�j�u�[�g���[�_�[�̏������݁B
  SH2A-FFF82000.bin �� USERROM.BIN �Ƀ��l�[�����āA�V���A��EEPROM��
  �㔼32kB �ɏ������݂܂��B ( �c�[���Ƃ��� SPIwriterUser ���g�p���܂��B )

  �������񂾌�AJPP2�N���[�Y���čĐڑ����Ă��������B
  
  �ڑ��m�F�� cdctool/cdctool.exe ��p���܂��B
  �u�[�g���[�_�[�� cdctool/cdcboot.exe ���g�p���Ă��������B

�i�Q�jUSB-CDC�t���[�����[�N�Ƃ��Ă̎g�p
  a.bat �����s����ƁASH2A-1C000000.mot �� 1C000000�Ԓn�Ƀ��[�h���Ď��s���܂��B


�i�Q�jUSB-CDC�G�R�[�o�b�N�e�X�g
  usbmain.c ���̒�`��
  
  #define	ECHOBACK_TEST				(1)

  �ɕύX���ăr���h�������̂����s����ƁA�t�@�[���E�F�A���ł̃G�R�[�o�b�N�e�X�g
  ��p�̃t�@�[���ɂȂ�܂��B
  �K���Ȗ��菇�[�����N�����ăG�R�[�o�b�N�e�X�g�����s���Ă��������B


�� �����_�ł̖��B

   FPU���g���܂���B


�� FPU ���B

  �Ȃ��� FPU���g���܂���B
  gcc�̃R���p�C���I�v�V���� -m2a-nofpu ��^���Ă��������B

  fpscr���W�X�^���݂̖�肪����悤�Ɏv���܂��B
  gcc���Â�������������܂���B

�� Windows�z�X�g�c�[���ɂ��āB

  cdctool.exe �ŋt�A�Z���u�������s�����ARM�Ǝv���ċt�A�Z���u������s�������܂��B

  cdcboot.exe ��ihex��mot�t�@�C���������߂��܂���B
  cdcboot.exe ���u�[�g�o����G���A��0x1C00_0000�`1MB �̋�Ԃ݂̂ł��B


�� ���荞�݂ɂ��āB

  ���荞�݃n���h���[�� USB��CMT0�̂Q���g�p���Ă��܂��B
  ����ȊO�̊��荞�݂͑S��Dummy�ɔ�΂����ƂŃ������[�ߖ񂵂Ă��܂��B(vect_alias.h)

  ����Ɋ��荞�݂��K�v�ɂȂ����ꍇ�́Acommon/vect_alias.h �ɂ��� Dummy�ւ̒u��������
  �i�ꕔ�A���邢�͑S���j�������Ďg�p���Ă��������B


�� �u�[�g���[�_�[���A�v���Ƃ��Ďg�����@�B

  �u�[�g�̈�FFF8_2000�^�[�Q�b�g�ɂ������� �ʏ��1MB SRAM�̈� 1C00_0000 ��
  �o�C�i���[������āA���[�h���邱�ƂŃA�v���P�[�V�����Ƃ��Ďg���܂��B
  
  ���̃f�B���N�g���ŕ��ʂ�make���Ă��������B
  make��Ag.bat �ŏ�������ł��������B
  
  
�� �r���h������@

  CQ�̃T�C�g�ɂ���Cygwin��gcc���g���܂��B
  
  gcc�� cygwin���C���X�g�[�������f�B���N�g���i���� D:\cygwin �Ƃ��܂��j
  ���瑊�΂� usr/local/sh-tools �ɒu���܂��B
  ( ��̗�ł� D:\cygwin\usr\local\sh-tools\ �ƂȂ�܂� )


�� ���荞�݃n���h���[�̂b�L�q/ASM�L�q�̐؂�ւ��ɂ���

�b�L�q�ɂ���ꍇ��common/intprg.c ��#define INTASM ���R�����g�A�E�g�B

//	���ۂ̃n���h���[��sh2int.S �A�Z���u���Œ�`����.
//
#define	INTASM

���ꂩ��AMakefile�� ASRCARM = �̍��Ash2int.S �������܂��B

�� gcc�̕s��ɂ��AFPU�g�p�I�v�V���� -m2a ���͂b�L�q����ƃ��W�X�^��
���邽�߂ɐ��퓮�삵�܂���B



�� RESBANK�ɂ���

���W�X�^�o���N�̗L��/������Makefile�Ő؂�ւ��Ă��܂��B

CDEFS =  -D$(RUN_MODE) -DUSE_RESBANK

FPU���g�p����ꍇ�́A���荞�݃n���h���[��ASM�L�q�ɂ���K�v������܂��B
�܂��AASM�L�q�̓s����AUSE_RESBANK�w�肪�K�{�ɂȂ�܂��B

�� ���W�X�^�o���N�L���ɂ����ꍇ�A15���x���ȏ�̑��d���荞�݂Ńo���N���I�[�o�[�t���[
���܂��̂ŁA��{�I�ɂ͑��d���荞�݂𔭐������Ȃ��悤�Ƀv���O�������Ă��������B


�� FPU�̎g�p�ɂ���

FPU�̗L��/������Makefile�Ő؂�ւ��Ă��܂��B

# FPU���g��.
#
#USE_FPU = YES

�� ��L�R�����g�A�E�g���O����FPU���g�p����悤�ɂȂ�܂����A�����gcc��newlib��
�����ĉ��炩�̃o�O������Aprintf("%g",x)�Ȃǂ�����ɓ����܂���B
�Ƃ肠�����AUSE_FPU�̓R�����g�A�E�g�����܂܂ł̂��g�p�������߂��܂��B


�� KPIT�� sh-elf-gcc�g�p�ɂ���

Cygwin�� gcc�̂�����KPIT�� sh-elf-gcc���g�p����ꍇ�� Makefile-kpit-gcc
���g�p���Ă��������B

KPIT�� sh-elf-gcc���g�p����ꍇ�� FPU �g�p�\�ł��B


�� usercmd() �ɂ��āB

cdctool.exe ���� user�R�}���h�����s����ƁAmonit/usercmd.c ����usercmd�֐���
�Ăяo����܂��B�����ɊȒP�ȃe�X�g�v���O�������������ƂŁA�t�@�[���E�F�A��
�ł̂����ȃe�X�g���C�y�ɍs�Ȃ����Ƃ��o���܂��B
printf��L���ɂ����ꍇ�́Alibc�̑����̊֐��������N����܂��̂ŁA�R�[�h�ʂ�
�{�����܂��B�i���̏ꍇ32kB�ɓ���Ȃ��̂�ROM���o���܂���j

printf��cdctool.exe�̃R���\�[���ɏo�͂���o�H�́A�ȉ��̂悤�ɂȂ��Ă��܂��B
- printf() =�����x��stdio
- write()  =�჌�x��I/O
- __trap34() �V�X�e���R�[���Ăяo��( ASM�\�[�X���ł� ___trap34: )
     ���ۂɂ� trapa #34 ���s�Ȃ킸�ɁAsh2int.S���ɂĉ����
- trap_write() �Ăяo��
- _user_putc() �Ăяo��
- USBputch() �Ăяo��
- �z�X�g���R���\�[����




�t�@�C���ꗗ
------------------------
usb_firm/src/p_main.c
main.c
usbmain.c
debug.c
gpio.c
sh2asm.c
sh2int.S
sh2fpint.S
monitor/board.c
monitor/fifo.c
monitor/gpio.c
monitor/monit.c
monitor/picwrt.c
monitor/usbio.c
monitor/usercmd.c
usb_firm/src/c_datatbl.c
usb_firm/src/common/c_dataio.c
usb_firm/src/common/c_global.c
usb_firm/src/common/c_intrn.c
usb_firm/src/common/c_lib7264.c
usb_firm/src/common/c_libint.c
usb_firm/src/common/c_usbint.c
usb_firm/src/common/c_usb_dly.c
usb_firm/src/common/c_usb_dma.c
usb_firm/src/peri/p_changeep.c
usb_firm/src/peri/p_classvendor.c
usb_firm/src/peri/p_controlrw.c
usb_firm/src/peri/p_descrip.c
usb_firm/src/peri/p_intrn.c
usb_firm/src/peri/p_lib7264.c
usb_firm/src/peri/p_libint.c
usb_firm/src/peri/p_status.c
usb_firm/src/peri/p_stdreqget.c
usb_firm/src/peri/p_stdreqset.c
usb_firm/src/peri/p_usbint.c
usb_firm/src/peri/p_usbsig.c
common/cpg.c
common/dbsct.c
common/hwsetup.c
common/intprg.c
common/resetprg.c
common/sbrk.c
common/siochar.c
common/vecttbl.c
