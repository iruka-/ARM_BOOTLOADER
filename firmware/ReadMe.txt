/**********************************************************************
    �f�B���N�g���\��
 **********************************************************************
firmware ---+--- fm3     ---- Fujitsu FM3�p
            +--- stm32   ---- STM32   CQ-STARM / STM8S-D / STBee / STBeeMini�p.
            +--- rx62n   ---- Renesas FRK-RX62N�p
            +--- sh2a    ---- Renesas SH2A�p
            +--- lpc2388 ---- NXP CQ-FRK-NXP (LPC2388)�p
            +--- mary    ---- NXP LPC1114 Mary�p
            |
            |                 host��Tool
            +--- tools  --+-- cdctool -------- �ȈՃ��j�^�[
                          +-- w32term -------- RS232C�]���x���`�}�[�N,���菇�[��



/**********************************************************************
    �r���h���@�Ȃ�
 **********************************************************************

�t�@�[���E�F�A�̃r���h:

-��{�I�Ƀt�@�[���E�F�A��gcc�N���X�R���p�C���Ńr���h���܂��B
-ARM�n�̏ꍇ�́ACodeSourcery G++ Lite 4.4.x ���g�p���܂��B(Windows/Linux)
-RX62N,SH2A�̏ꍇ�́Acygwin��Ńr���h���ꂽgcc�N���X�R���p�C�����g�p���܂��B

-- Makefile ���p�ӂ���Ă��܂��BGNU make ���g�p���ăr���h���܂��B



�Q�l�t�q�k

�t���[ARM�p�R���p�C���̓����Ȃ�

CodeSourcery G++ Lite:
  https://sourcery.mentor.com/sgpp/lite/arm/portal/subscription3053

Maple IDE �ɂ� CodeSourcery G++ ���܂܂�Ă��܂��B
  http://leaflabs.com/docs/maple-ide-install.html#download

SH2A�p��Cygwin-gcc
  http://www.kumikomi.net/interface/editors/2010/05/sh-2agcc.php

RX62N�p��Cygwin-gcc
  http://www.tokudenkairo.co.jp/rx62n/download.html


���̑��̃t���[ARM�p�R���p�C��

NETX-ARM-EABI-GCC
  http://sourceforge.jp/projects/sfnet_netx-arm-elf-gc/releases/?fulllist=1

YAGARTO GNU ARM toolchain
  http://www.yagarto.de/#download



 **********************************************************************

Windows�z�X�g���c�[���̃r���h:
-��{�I��Windows�z�X�g���c�[����MinGW gcc�Ńr���h���܂��B

-- ������ Makefile ���p�ӂ���Ă��܂��BGNU make ���g�p���ăr���h���܂��B

MinGW gcc �R���p�C���̓����

MinGW
  http://www.mingw.org/
  http://sourceforge.net/projects/mingw/files/


 **********************************************************************


/**********************************************************************
    �t�@�[���E�F�A�̃J�X�^�}�C�Y�Ɋւ���q���g
 **********************************************************************

�t�@�[���E�F�A�̃J�X�^�}�C�Y�Ɋւ���q���g�@�Ɋւ��܂��ẮA�ȉ���
�h�L�������g�����ǂ݂��������B

firmware/fm3/src/FIRMWARE-HACKING-GUIDE.txt


 **********************************************************************
