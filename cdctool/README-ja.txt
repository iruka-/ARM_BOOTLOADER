*libmaple���g�����Ȃ�!! ���e


**�T�v
-----------------------------------------------------------------------------------
libmaple �ileaflabs���J�����Amaple�Ŏg���Ă���STM32�p��Arduino�����C�u�����j��
�x�[�X�ɂ��č쐬����ARM�p�̊ȈՃ��j�^�[�v���O�����ł��B

CQ-STARM , STM8S-DISCOVERY , STBee , STBeeMini �̊�ɑΉ����Ă��܂��B


**QuickStart 
-----------------------------------------------------------------------------------
CQ-STARM���:
-cdctool.zip���𓀂��܂��B
-cdctool/ ������Makefile������܂��̂ŁA
--Linux �R�}���h���C��
--��������Windows�̃R�}���h�v�����v�g����
 D:\cdctool> make
             ~~~~
--�����s���Ă��������B
-make�ɐ���������Acdctool/build/CQSTARM.hex ����������Ă��܂�(0x0800_3000�Ԓn�X�^�[�g)
-armboot���g�p����ꍇ�́A
 D:\cdctool> w.bat
-�������́Adfuw.exe���g�p����CQ-STARM�ɓ]�����A���s���Ă��������B

-�V���A���|�[�g��inf�t�@�C����win32drivers/serial/ �ɂ���܂��B
--(STM32�̃T���v���pinf���g�p����ꍇ��VID:PID�����������邩�t�@�[������VID:PID��STM32�̂��̂ɍ��킹�邩���āA�f�o�C�X�F������悤�ɂ��������j

-�f�o�C�X�F��������Ahost/cdctool.exe ���N�����ă������[�_���v��t�A�Z���u���������Ă݂�
���������B
-script���w�肵�ċN�����邱�ƂŁAL�`�J�Ȃǂ����s���邱�Ƃ��\�ł��B



**�������@�A�r���h���@
-----------------------------------------------------------------------------------

�i�P�jcdctool/Makefile �Ɋ�I���̒�`������܂��B
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

**���
-----------------------------------------------------------------------------------
-���̃��j�^�[�v���O�����́A���zCOM�f�o�C�X�o�R��CQ-STARM�Ȃǂ̊�ɐڑ����܂��B
-�v���g�R����armon/armboot�̂��̂𗬗p���Ă��܂��B
-�ȒP�ɐ��������

-- 0xaa,0x55 ����M���邱�ƂŁA���j�^�[���[�h�Ɉڍs���܂��B
-- 0x01,[ armon�̃R�}���h��(31byte) ] �Ƃ����p�P�b�g(32byte�Œ蒷)���󂯎��ƁA
   ������R�}���h�Ƃ��ĉ��߂��āA�ԓ����K�v�Ȃ�32byte�Œ蒷�̕ԓ��p�P�b�g��CDC��
   �Ԃ��܂��B

-- COM�f�o�C�X�̒T���́AWin32���W�X�g����COM�f�o�C�X��񋓂��āA�Ō�ɗ񋓂��ꂽ
   com�|�[�g�����߂����ŃI�[�v�����Ă��܂��B(�Ƃ肠�����B)

-�g������armon/armboot�Ƃقړ����ł��B
-AVR/PIC/PIC24F ���C�^�[�Ƃ��Ă̓���m�F�͂܂��ł��B


**���_
-----------------------------------------------------------------------------------

- wirish �� SerialUSB.read(buf,count)��SerialUSB.read() �̉��ʑw�ł���A
  usbReceiveBytes(uint8* recvBuf, uint32 len); �̎����ɂ͂������悤�ł��B

  �iCOM�|�[�g�ɏ������܂ꂽ��M�f�[�^��FIFO�Ƃ��Ĉ����Ă��炸�APC���瑗��ꂽ
  �@�p�P�b�g�P�ʂŕێ����Ă��邽�߁A�o�b����A���ő���ꂽ���̂��A��M����
  �@���o�C�g���Â������ɂ���read()����Ɠǂ񂾃f�[�^���ēx�ǂ܂����󋵂�
  �@�������܂��B�j

- ����A�΍�͍���Ȃ̂Ń��j�^�[���̎�����32byte�Œ蒷���R�[�h�ő���M����悤��
  ���ăo�O��������Ă��܂��B




**�ӎ�
-----------------------------------------------------------------------------------

-STBee , STBeeMini , STM32VLD �ւ̑Ή��ɂ�����A[[xshige����:http://psp.dip.jp/web/cgi-bin/note/index.cgi?%b7%c7%bc%a8%c8%c4]]��[[Maple_0011_Adaptation_20110611.zip:http://cid-bbd4ba3beeca403e.office.live.com/self.aspx/pub/Maple%5E_0011%5E_Adaptation%5E_20110611.zip]]
�̐��ʂ��v��������g�킹�Ă��������܂����̂ŁA�����Ɏӎ���\���グ�܂��B

