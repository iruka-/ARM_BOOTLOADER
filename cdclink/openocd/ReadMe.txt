�� cdclink�̊T�v

   �ucdclink�v�́AOpenOCD �iARM�p�̃I�[�v���\�[�X JTAG-ICE �f�o�b�O�c�[���j
   ����g�p�ł��� JTAG�C���^�[�t�F�[�X�i�����N�P�[�u���j�̂P�ł��B

   ���L�̂悤�ȍ\����p���āAARM�}�C�R���̃f�o�b�O��FLASH�������݂Ɏg�p�ł��܂��B

   +-----------------+
   |                 |
   | Windows�p�\�R�� |   <=== GDB + OpenOCD + cdclink.dll�i�{�\�t�g�j
   |                 |
   +-----------------+
          ||
          || USB�P�[�u��
          ||
   +-----------------+
   | cdclink �}�C�R��|   <=== JTAG�C���^�[�t�F�[�X(USB�@�\���̏����ȃ}�C�R��)
   +-----------------+        �i����Љ��n�[�h�E�F�A�j
          ||
          || JTAG �P�[�u��
          ||
          vv
     +-------------+
     | ARM�}�C�R�� |      <=== �f�o�b�O�Ώۂ� �^�[�Q�b�g�E�}�C�R���{�[�h
     +-------------+


�� ����

   cdclink��Windows�p�\�R������́A�V���A���|�[�g(USB-CDC�N���X)�Ƃ���
   �F������܂��B
   
   OpenOCD�̃h���C�o�[�Ƃ��Ă̎������@�͊O���� DLL ��u��(cdclink.dll)
   ���ƂŁA�������������݂Ă��܂��B

   �e��cdclink�}�C�R���̃t�@�[���E�F�A�\�[�X�����J����Ă��܂��̂ŁA
   OpenOCD�Ƒg�ݍ��킹�� JTAG�A�_�v�^�[��Ǝ��Ɏ��삷�邱�Ƃ��\�ł��B


�� ������

  openocd/openocd.exe ���ȉ��̂悤�ȃI�v�V������t���ċN�����܂��B

  D:> openocd.exe -f cdc_if.cfg -f stm32.cfg -f batch.cfg

  �I�v�V���� '-f' �Ŏw�肷��config�t�@�C���̏ڍׂ́A�ȉ��̒ʂ�ł�
  -----------------------------------------------------------------------------
  cdc_if.cfg �� �ucdclink�v�C���^�[�t�F�[�X �̍\�����L�q�������̂ł��B
  stm32.cfg  �� �^�[�Q�b�g�}�C�R��(stm32)�̍\�����L�q�������̂ł��B
  batch.cfg  �� openocd.exe �N����̐U�镑���i�o�b�`�������j���L�q�������̂ł��B
  -----------------------------------------------------------------------------



�� openocd.exe�̓���T�v

  openocd.exe �̓R�}���h���C������N������ƁA(shutdown����܂ł�) TCP/IP ��
  3333�ԃ|�[�g��4444�ԃ|�[�g���󂯎��f�o�b�O�T�[�o�[�Ƃ��ē����܂��B

  ���̏�ԂŁAgdb����Alocalhost:3333 �ɐڑ����邱�ƂŁA�^�[�Q�b�g�}�C�R����
  gdb�̊Ԃ���莝���Ƃ��ł��܂��B
  
  �܂��Alocalhost:4444�Ԃ� telnet�v���g�R���Őڑ����邱�ƂŁA�Θb�I�ȑ���
  �i�Ⴆ��FLASH ROM�̏������݂Ȃǁj���s�Ȃ����Ƃ��ł��܂��B




�� �f�B���N�g���\��

 openocd -+- �\�[�X.
          |
          +--helper\   �w�b�_�[�t�@�C��.
          +--jtag\     �w�b�_�[�t�@�C��.
          +--jimtcl\   �w�b�_�[�t�@�C��.
          |
          +--openocd_patch\  openocd�{�̑��쐬�p�̉����_

 helper,jtag,jimtcl�f�B���N�g���̃w�b�_�[�t�@�C���͎g�p����openocd.exe���r���h
 �����\�[�X�Ɠ������̂ł��B


�� �v���O�����̍ăr���h���@

   Windows���MinGW-gcc�R���p�C����p����make���Ă��������B
   make����ƁAhidblast.dll ���쐬����܂��B

   openocd.exe�{�̂��ăr���h������@�́A�ȉ���URL���Q�Ƃ��Ă��������B

-http://hp.vector.co.jp/authors/VA000177/html/2010-09.html
   
   ����̉��������\�[�X��openocd_patch/ �f�B���N�g���ɒu���Ă��܂��B

   �o���オ���� openocd.exe �{�̂́A�h���C�o�[�Ƃ��āA����f�B���N�g���ɑ��݂���
   cdclink.dll ���N�����ɌĂяo���܂��B
   (cdclink.dll�̓ǂݍ��݂ɐ��������ꍇ�́Adummy�h���C�o�[�ƒu�������܂�)
   (cdclink.dll�����݂��Ȃ���΁Adummy�h���C�o�[�̂܂܂ɂȂ�܂�)

   openocd.exe�r���h���̃I�v�V�����͈ȉ��̂悤�ɍs�Ȃ��܂����B
   libUSB-win32�o�R�ł�ft2232��jlink�C���^�[�t�F�[�X�Ȃǂ��g�p�\�ł��B
   ------
   ./guess-rev.sh
   ./bootstrap
   ./configure --build=i686-pc-linux-gnu \
			--host=i586-mingw32msvc \
			--enable-maintainer-mode \
			--disable-werror \
			--enable-dummy \
			--enable-jlink \
			--enable-vsllink \
			--enable-usbprog \
			--enable-usb_blaster_libftdi \
			--enable-ft2232_libftdi	\



�� openocd-nogdb-hacks.exe �ɂ���

   openocd.exe ���r���h���邳���ɁA�ȉ��̒�`
   ----
   /* define for enabling armv7 gdb workarounds */
   #define ARMV7_GDB_HACKS
   ----

   ���R�����g�A�E�g�������̂����p�ӂ������܂����B(openocd-nogdb-hacks.exe)

   �����A�g�p���� gdb �� Insight �̃o�[�W�����ɂ����openocd �Ƃ̑�����
   �����ꍇ�́Aopenocd.exe �̑���ɁA���̎��s�t�@�C�������g�p���������B



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


