�� �T�v

  ����́A�b�p�o�ŎЎG�� Interface�}�K�W��2009-5������ CQ-FRK-NXP �t�^��p��HID�f�o�C�X�p��
  HID bootloader �� �Ȉ�monitor�ł��B


�� ���݂̃X�e�[�^�X

   �������[�_���v�A�t�A�Z���u���������s�ł��܂��B
   2000�Ԓn�ȍ~�̃A�h���X�ɔC�ӂ� application (hex�t�@�C��) ����������Ŏ��s���邱�Ƃ��o���܂��B

   �����I�ɂ�AVR/PIC���C�^�[�@�\�Ȃǂ��������邱�Ƃ��l���Ă��܂��B


�� ������

  (1)ELM-ChaN����̃T�C�g����lpcsp�������݃c�[�����_�E�����[�h����Windows�ɓ���܂��B
  (2)JP2(BOOT)�W�����p�[��Close������Ԃ�CP2102(USB-�V���A���ϊ��`�b�v)�o�R��PC�ɐڑ����܂��B
  (3)NXP�`�b�v����USB����USB�R�l�N�^�����炩����PC�ɐڑ����Ă����܂��B
  (4)w.bat �ɂ� firmware/main-0000.hex ��Flash�ɏ������݂܂��B
  (5)JP2(BOOT)�W�����p�[��Close������ԂŃ��Z�b�g���܂��B
  (6)a.bat �ɂ� host/armon.exe ���N�����܂��B�����Ń������[�_���v�������s���܂��B

HID�u�[�g���[�_�[�̃e�X�g���@:
  ��Lmain-0000.hex �����삵�Ă����ԂŁA
  (7)firmware�f�B���N�g���� b.bat �����s���܂�
     ����ƁAmain-2000.hex(application) ��Flash��2000�Ԓn�`�� ��������ŋN�����܂��B

  �Ebootloader��application�́A�啔���̃\�[�X�����p����Ă��܂��B
  �Eapplication�ɂ̓T�C�Y�������Ȃ��̂ŁA���낢��Ȋg�����s�����Ƃ��o���܂��B


�� �f�B���N�g���\��



 arm7mon --+-- host/ ----------------   �z�X�gPC�c�[��
           |
 		   +-- firmware/ --+--- hwlib/  ���C�u������
                           |
                           +--- usb/    USB�t���[�����[�N��

�� �o�b�`�t�@�C��
   w.bat :
   		�o����main-0000.hex ���V���A���o�R��LPC2388�ɏ������݂܂��B(�vlpcsp.exe,lpcsp.ini)
   b.bat :
   		�o����main-2000.hex ��USB�o�R(HID�f�o�C�X���쒆)��LPC2388�ɏ�������Ŏ��s���܂��B
   mkall.bat:
   		main-0000.hex�i0�Ԓn:�u�[�g���[�_�[�j ��main-2000.hex (2000�Ԓn:�A�v��)
   		�̗������܂Ƃ߂č쐬���܂��B


�� �v���O�����̍ăr���h���@

   �����ł� LPCXpresso ���p�ӂ����J�������g�킸�ɁA���ʂ�ARM�pgcc��Make��
�@ �g���ăr���h���܂��B

   �i�P�jCoreSourcery G++ Lite(ARM�p:����)���C���X�g�[�����܂��B

   �i�Q�jWinARM��������WinAVR ���C���X�g�[�����āAC:/WinARM/utils/bin 
         �������́AC:/WinAVR/utils/bin �݂̂Ɏ��sPATH��ʂ��Ă����܂��B
         �i����́Amake.exe �� rm.exe �Ȃǂ�unix�n�R�}���h���g�����߂ł��j

   �i�R�jsrc/ �f�B���N�g���ɂĕ��ʂ�make���Ă��������B


�� Linux�ł̓�����@

   �i�P�jLinux ( �����ł� ubuntu10.4��z�肵�܂� ) �Ŏg�p���邱�Ƃ��o���܂��B
   �i�Q�j�r���h�̏���(libusb�̎擾)
        # apt-get install libusb-dev
   �i�R�jhost/ �f�B���N�g���ɂāAarmon / armboot ���r���h���܂��B
        $ cd host
        $ make
   �i�S�jroot�����Ŏ��s���Ă��������B
        $ sudo ./armon

    USB�f�o�C�X�̎g�p�������n�r���œK�؂ɐݒ肷��΁A���[�U�[�����œ��삷�邱�Ƃ��o���܂��B


�� ����̖��_

   �i�P�j �u�[�g���[�_�[�Ƃ��Ă̋@�\�͎������܂����B
		  �������A���݃u�[�g�W�����p�[�̔��菈���͂���܂���̂ŁA
		  	C:> armboot -r
		  ��������
		  	C:> armon
		  	ARM> boot 2000
		  �̂����ꂩ�̕��@�ŁA2000�Ԓn�J�n�̃t�@�[���E�F�A���N������悤�ɂ��܂��B

   �i�Q�j FLASH ROM��0x8000�`0x78000�Ԓn�̗̈��FLASH��SECTOR�T�C�Y��4kB�ł͂Ȃ�32kB��
		�@�Ȃ��Ă��܂��B�ł��̂ŁA���̗̈��(32kB�u���b�N��)�r������HEX���������݂����ꍇ�́A
          ���炩����
		  C:> armboot -E
		  �̂悤�ɑS�������Ă����Ă��珑�����݂���悤�ɂ��Ă��������B
         (0x2000�Ԓn����A���ŏ������ޏꍇ�A32kB�u���b�N�͐�������������܂�)

		  ���̐�����FLASH�̃Z�N�^�����Œ�T�C�Y�̑���ARM�f�o�C�X�Ƌ��ʏ����ɂȂ��Ă���
		�@���߂ɐ����Ă��܂��B

	�i�R�jbb.bat �͎g�p�ł��܂���B(USB����u�[�g���[�_�[�̏��������͎g�p�ł��܂���B)

		�@���̗��R�́AFLASH ROM��0x00�`0xff�̂悤�ȔԒn�ɗ�O�x�N�^�[�����݂��Ă��āA�����
		�@�������Ă��܂���(USB�Ȃǂ̊��荞�݂��r�₦��)�\�����Ă��܂�����ł��B

		  ����ARM�f�o�C�X�ł�NVIC ���g�p���Ċ��荞�݂̗�O�x�N�^�[���O�Ԓn�ȊO�̏ꏊ��
          �ݒ肷�邱�Ƃ��o����̂Ŗ�肠��܂���B




�� ���C�Z���X

  BSD���C�Z���X

/*
	LPCUSB, an USB device driver for LPC microcontrollers	
	Copyright (C) 2006 Bertrik Sikken (bertrik@sikken.nl)

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	   derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



�� ���̑�

�������[�_���v,�t�A�Z���u���ȊO�̃R�}���h�͂��܂��������Ă��܂���B



