*libmaple���g�����Ȃ� !! ��R�e


**�T�v
-----------------------------------------------------------------------------------
����́Alibmaple 
		�ileaflabs���J�����Amaple�Ŏg���Ă���STM32�p��Arduino�����C�u�����j
���x�[�X�ɂ����b�����p�̃t���[�����[�N�ł��B�i�b�{�{����T�|�[�g���폜���Ă��܂��j


CQ-STARM , STM8S-DISCOVERY , STBee , STBeeMini �̊�ɑΉ����Ă��܂��B


����̓���́A���zCOM�f�o�C�X�ɂȂ��āA�P���ȕ����P�ʂ̃G�R�[�o�b�N���s���Ă��܂��B


**�p�r
-----------------------------------------------------------------------------------
���΂�A���zCOM�u�[�g���[�_�[�쐬��p�ł��B

���̑��̗p�r�ł́A�i�R�[�h�팸�̂��߂́j �b�������ɂ���K�v���͂قƂ�ǂ���܂���B







**QuickStart 
-----------------------------------------------------------------------------------
CQ-STARM���:
-cdcboot.zip���𓀂��܂��B
-cdcboot/ ������Makefile������܂��̂ŁA
--Linux �R�}���h���C��
--��������Windows�̃R�}���h�v�����v�g����
 D:\cdcboot> make
             ~~~~
--�����s���Ă��������B
-make�ɐ���������Acdcboot/build/CQSTARM.hex ����������Ă��܂�(0x0800_3000�Ԓn�X�^�[�g)
-armboot���g�p����ꍇ�́A
 D:\cdcboot> w.bat
-�������́Adfuw.exe���g�p����CQ-STARM�ɓ]�����A���s���Ă��������B

-�V���A���|�[�g��inf�t�@�C����win32drivers/serial/ �ɂ���܂��B










**�������@�A�r���h���@
-----------------------------------------------------------------------------------

�i�P�jcdcboot/Makefile �Ɋ�I���̒�`������܂��B
 # Valid BOARDs: maple, maple_native, ...
 #BOARD ?= maple
 #BOARD ?= STBee
 #BOARD ?= STBeeMini
 #BOARD ?= STM32VLD
 BOARD ?= CQSTARM
 #BOARD ?= DISCOVERY
-�ǂꂩ�P��L���ɂ��Ă��������B

~
�i�Q�jCQSTARM , DISCOVERY�� 0x0800_0300 ����N������elf/hex�𐶐����܂��B
-�J�n�Ԓn��ύX�������ꍇ��libmaple/support/ld/���/flash.ld �ƁAlibmaple/libmaple/libmaple.h ���̔Ԓn��`�̗���������������K�v������܂��B

�i�R�jHEX�t�@�C����elf�Ɠ����ɐ�������܂��B
-libmaple/*.bat �Ƃ����o�b�`�t�@�C���ɂāAarmboot�o�R��hex����������ő����s���܂��B

�i�S�j�r���h��(CodeSourceryG++ Lite)��CodeSourcery�̃T�C�g������肵�Ă��ǂ��ł����A
-maple-0.0.11 ��windows�ł���肵�Ă����W�J���ē����R���p�C����bin�f�B���N�g����
���̂܂܎��s�p�X��ʂ��Ă��n�j�ł��B

�i�T�jCQSTARM��DISCOVERY(STM8S��STLINK)�̃|�[�g���̂�STBee��Mini�̒�`���ɏ����Ă��܂��B
--�Ƃ�������STBee.cpp�����̂܂ܗ��p�����Ă��������Ă���܂��B

~

**�\�[�X����
-----------------------------------------------------------------------------------
-wirish/ ��libraries/ �f�B���N�g�����폜���Ă��܂��B
-monitor/ �ȉ��ɊȈՃ��j�^�[�̃\�[�X������܂��B
--���̒��̊֐��̈ꕔ�̂݋@�\���Ă��܂��B

**�r���h�̃t���[�����[�N�ɂ���
-----------------------------------------------------------------------------------
�͂��߂́Amaple-bootloader�̃t���[�����[�N���g�p���Ă��܂������Alibmaple/ �ȉ���
�\�[�X�Ƃ̐e�a���ɓ�_������i���荞�݊֐��̖��̂��قȂ�j���Ƃ����������̂ŁA
libmaple�̃t���[�����[�N�ɖ߂��āAstartup�̂b���ꃂ�W���[���̃r���h�I�v�V�����̂�
-Os �ɕύX���čăr���h���܂����B

wirish/�ȉ���usb�֐��́A��֕i�Ƃ��� monitor/usbio.c �Ɉڂ��Ă���܂��B
wirish/�ȉ���digitalWrite()�֐��ɂ��Ă��A��֕i�Ƃ���monitor/util.c ��u���܂����B


���낢�뎎�s������s�������߁A�����̃t���[�����[�N�̎c�[���c�����܂܂ɂȂ��Ă��܂��B


**�ӎ�
-----------------------------------------------------------------------------------

-STBee , STBeeMini , STM32VLD �ւ̑Ή��ɂ�����A[[xshige����:http://psp.dip.jp/web/cgi-bin/note/index.cgi?%b7%c7%bc%a8%c8%c4]]��[[Maple_0011_Adaptation_20110611.zip:http://cid-bbd4ba3beeca403e.office.live.com/self.aspx/pub/Maple%5E_0011%5E_Adaptation%5E_20110611.zip]]
�̐��ʂ��v��������g�킹�Ă��������܂����̂ŁA�����Ɏӎ���\���グ�܂��B

