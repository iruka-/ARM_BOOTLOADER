�� �T�v

   ����́AOpenOCD �� DLL �n�b�N�i�����j�ł��B
   �����A�O���� DLL ��u���āAJTAG�A�_�v�^�[�̃h���C�o�[�̕������������݂Ă��܂��B

   JTAG�A�_�v�^�[�Ƃ̒ʐM��Windows�ėp��USB-CDC�f�o�C�X�i�V���A���ʐM�j�ōs���܂��B

�� ���݂̃X�e�[�^�X

   �e��ARM����g�p����JTAG�A�_�v�^�[�����삵�Ă��܂��B

   - CQ-STARM (STM32)
   - CQ-FRK-NXP-ARM (LPC2388)
   - MARY��� (LPC1114)

   �ʐM���x��FT2232��p����JTAG�A�_�v�^�[�ɔ�ׂ�ƒᑬ�ł��B


  * �t�@�[���E�F�A�̏�������
  
  +----------------------------------------------+
  | ���         | �t�@�[���E�F�A(Hex�t�@�C��) |
  +----------------------------------------------+
  | CQ-STARM       | CQ_STARM-3000.hex           |
  | STBee          | STBEE-3000.hex              |
  | STBeeMini      | STBEE_MINI-3000.hex         |
  +----------------------------------------------+
  | MARY           | MARY-0000.hex               |
  | CQ-FRK-NXP-ARM | LPC2388-0000.hex            |
  +----------------------------------------------+

�� ������

  �^�[�Q�b�g�}�C�R���̎�ނɍ��킹�āAcq-binary �f�B���N�g���ɂ���A
  start-stm32.bat   (STM32�}�C�R���̃f�o�b�O�p) ���A
  start-fm3.bat     (FM3�}�C�R���̃f�o�b�O�p) ���A
  start-lpc2388.bat (LPC2388�}�C�R���̃f�o�b�O�p) �̂ǂꂩ��
  Windows��DOS������N�����邩�AWindows�̃G�N�X�v���[���[���璼��
  �_�u���N���b�N�ŋN�����܂��B
  
  OpenOCD���N������ƁADOS���ɂP�O�s���x�̃��b�Z�[�W���O���o�āA�^�[�Q�b�g
  �}�C�R���Ɛڑ��������Ƃ�m�点�Ă���܂��B
  �ڑ������܂������Ȃ��ꍇ��Error�̃��b�Z�[�W���o�Ă��܂��B

  �p�ꃁ�b�Z�[�W�Ȃ̂ŕ�����ɂ����ł����A�G���[�����ꍇ�͂��̓��e��
  �悭�ǂ�ł݂Ă��������B
  ���̏ꍇ�͔z���~�X��d�������̕s��ŃG���[�ɂȂ�܂��B

  �܂��AOpenOCD��localhost:3333��(GDB)��4444��(telnet)��Listen����
  TCP/IP�T�[�o�[�Ƃ��ĐU�����܂��B

  �N�������Windows�t�@�C�A�[�E�H�[���Ȃǂ��AOpenOCD��IP�ڑ������o���āA
  ������u���b�N���邩�����邩�����[�U�[�ɕ����Ă��܂��̂ŁA�i�ŏ��̂P�񂾂��ł��j
  ����I�����Ă����Ă��������B

  OpenOCD�́ADOS����Ƀ��O��print���邾���́A�������Ȃ��R�}���h���C���c�[��
  �ɉ߂��܂���̂ŁA���[�U�[���R���g���[���������ꍇ��teraterm�Ȃǂ�telnet�[����
  �N�����āAlocalhost �� 4444�ԃ|�[�g�� telnet �Őڑ����܂��B
  
   ����ɐڑ��o���Ă���悤�ł�����Atelnet�� localhost:4444 �Ԃɐڑ����āAOpenOCD
   �R�}���h (��: scan_chain �Ȃ�) �����s���Ă݂Ă��������B


�� �f�B���N�g���\��

 cdclink --+- openocd -+  openocd.exe �{�� �� cdclink.dll �̃\�[�X.
                       |
                       +--helper\   �w�b�_�[�t�@�C��.
                       +--jtag\     �w�b�_�[�t�@�C��.
                       +--jimtcl\   �w�b�_�[�t�@�C��.
                       |
                       +--openocd_patch\  openocd�{�̑��쐬�p�̉����p�b�`.

 firmware -+-- stm32\ ----- CQ-STARM�p��firmware�\�[�X
           |
           +-- lpc2388\ --- CQ-FRK-NXP-ARM �p��firmware�\�[�X
           |
           +-- mary\ ------ MARY��p��firmware�\�[�X


�� �v���O�����̍ăr���h���@

   WindowsXP���MinGW-gcc�R���p�C����p���� openocd/ �f�B���N�g���ɂ� make���Ă��������B
   make����ƁAcdclink.dll ���쐬����܂��B

   openocd.exe�{�̂��ăr���h������@�́A�ȉ���URL���Q�Ƃ��Ă��������B

-http://hp.vector.co.jp/authors/VA000177/html/2010-09.html
   
   ����̉��������\�[�X��openocd_patch/ �f�B���N�g���ɒu���Ă��܂��B

   Linux��ł̃r���h�I�v�V�����́A����Ȋ����ł��B
   $ ./configure \
       --build=i686-pc-linux-gnu \
       --host=i586-mingw32msvc \
       --enable-maintainer-mode \
       --enable-dummy

   �o���オ���� openocd.exe �{�̂́A�h���C�o�[�Ƃ��āA����f�B���N�g���ɑ��݂��� 
   cdclink.dll ���N�����ɌĂяo���܂��B

   (���݂��Ȃ���΁Adummy�h���C�o�[�݂̂��g�ݍ��܂�܂�)


�� ���C�Z���X

   OpenOCD�̔z�z���C�Z���X�ɏ����܂��B


�� �W�]

   cdclink.dll �t�@�C����(���͂�)�����ւ��邾���ŁA����f�o�C�X���T�|�[�g�\�ɂȂ�܂��B
   �i���Ƃ���ATtiny2313���g�p����JTAG�A�_�v�^�[�Ȃǂ��T�|�[�g�o����\��������܂��j

   cdclink.dll �̃G���g���[�|�C���g�́A
      DLL_int get_if_spec(struct jtag_command **q);
   �����ł��B������struct jtag_command **q��q�ɂ́Aopenocd�{�̂�jtag_command_queue�Ƃ���
   �O���[�o���ϐ��̃A�h���X��n���܂��B
   �߂�l�́A(int�ɂȂ��Ă��܂���) �h���C�o�[�L�q�\���̂̃A�h���X�ɂȂ�܂��B





�� JTAG�R�}���h�̒ǉ��ƃv���g�R���ɂ���

-hidcmd.h
 #define HIDASP_JTAG_WRITE	  0x18	//JTAG ��������.
 #define HIDASP_JTAG_READ	  0x19	//JTAG �ǂݏ���.
-���ǉ�����Ă܂��B

-HidReport�̉���X�g���[��(PC->AVR) �T�C�Y�͍ő�64�o�C�g�܂łł�.
 +------+------+-------------------+------+-------------------+------+-------------+-----+
 | 0x18 | jcmd |  data��           | jcmd |  data��           | jcmd |  data��     | 0x00|
 +------+------+-------------------+------+-------------------+------+-------------+-----+

--jcmd��1�o�C�g�͈ȉ��̂悤�ɒ�`�i���̂P�j
 bit 7   6   5   4   3   2   1   0  
   +---+---+---+---+---+---+---+---+
   | 0 | b6| JTAG�]��bit��(TDI�̐�)|   �{ JTAG�]��bit������ TDI�r�b�g�iLSB�t�@�[�X�g�j  
   +---+---+---+---+---+---+---+---+
--- b6��TDI���o�̍ŏIbit��TMS��1�ɂ���Ȃ�1 ���Ȃ��Ȃ� 0 �F TMS�͍ŏIbit�ȊO�͏펞0

--jcmd��1�o�C�g�͈ȉ��̂悤�ɒ�`�i���̂Q�j
 bit 7   6   5   4   3   2   1   0  
   +---+---+---+---+---+---+---+---+
   | 1 |BITBANG�]����(�㑱byte��)|   �{ BITBANG�]���񐔕�(byte��)�̃f�[�^
   +---+---+---+---+---+---+---+---+
--BITBANG�f�[�^�̂P�񕪂́ATCK=LOW�̃T���v����TCK=HIGH�̃T���v�����p�b�N�����f�[�^�B
 bit 7   6   5   4   3   2   1   0  
   +---+---+---+---+---+---+---+---+
   |TCK|TDI| - |TMS|TCK|TDI| - |TMS|  �i��ʂS�r�b�g���ŏ��ɃZ�b�g����A���ɉ��ʂS�r�b�g���Z�b�g����܂��j
   +---+---+---+---+---+---+---+---+
--TCK��ω����������Ȃ��Ƃ��́A������TCK�r�b�g�𓯂��l�ɂ��܂��B


-HidReport�̓o��X�g���[��(AVR->PC) �T�C�Y�͍ő�64�o�C�g�܂�. HIDASP_JTAG_READ���s���̂ݐ܂�Ԃ��ԑ�����܂�.
 +----------------------------------------+
 | JTAG��M�f�[�^(TDO�̓ǂݎ��r�b�g��)  |  �i�ő�64�o�C�g�܂Łj
 +----------------------------------------+
--�r�b�g���LSB�t�@�[�X�g. ���M���ꂽTDI�r�b�g��Ƃ��̂܂ܑΉ����Ă��܂�.

----
���̑��⑫
 #define HIDASP_JTAG_READ	  0x19	//JTAG �ǂݏ���.
-�����s����Ƃ��́AHidReport�̉���X�g���[��(PC->AVR)��P���Ȍ`���i�P�R�}���h�̂݁j�ɂ��܂��B
 +------+------+-------------------+------+
 | 0x19 | jcmd |  data��           | 0x00 |
 +------+------+-------------------+------+
-jcmd��BitBang���[�h�̂Ƃ��́A�ԓ��f�[�^�͂���܂���BJTAG(TDI��)�̂Ƃ��̂�(TDO��)��Ԃ��܂��B
-JTAG�X�g���[���������ꍇ(56bit�ȏ��TDI�𑗂���TDO���󂯎��)�́A56bit�P�ʂɕ����]�����܂��B
-���̏ꍇ�A�Ō�̃X�g���[���̍ŏIbit�̂݁ATMS��1�ɂ��鏈��������܂��B(b6=1�̃p�P�b�g��p�ӂ��܂�)


-armblaster�̃t�@�[�����ăr���h����Ƃ��́ACodeSourcery G++ Lite ���g�p���Ă��������B


�� OpenOCD�̊ȒP�Ȏg���� 

-�N��������܂��Alocalhost:4444�ԃ|�[�g��telnet��(TeraTerm�Ȃǂ��g����)�q���ł��������B

-TeraTerm����A�ȉ��̂悤�ȃR�}���h���^�C�v����ƁA���ʂ��\������܂��B

|OpenOCD�R�}���h|�Ӗ�|
|scan_chain		|�ڑ�����Ă���s�`�o�̃��X�g������B|
|reset halt		|�^�[�Q�b�gCPU��HALT������|
|reg			|�^�[�Q�b�gCPU�̃��W�X�^������|
|mdw �A�h���X �J�E���g|display memory words <addr> [count]�������[�_���v|
|step			|�b�o�t���X�e�b�v���s������|
|flash write_image erase main.hex	|main.hex��FLASH ROM�ɏ�������(���̂܂���CPU��HALT�ɂ��Ă����܂�)|


�� �Q�l����(LINK)

OpenOCD�������܂� (kimura Lab)
-http://www.kimura-lab.net/wiki/index.php/OpenOCD%E3%81%8C%E5%8B%95%E3%81%8F%E3%81%BE%E3%81%A7

OpenOCD (�x�X�g�e�N�m���W�[����)
-http://www.besttechnology.co.jp/modules/knowledge/?OpenOCD

OpenOCD�{��
-http://openocd.berlios.de/web/
