//
//	HIDcmd
//
//======================================================================
//	��{�|���V�[
//		���ʃR�}���h 0x00�`0x0f
//		PIC���C�^�[  0x10�`0x17
//		JTAG���C�^�[ 0x18�`0x1f
//		AVR���C�^�[  0x20�`0x2f
//	LSB=1(�)�R�}���h�́A��{�I�ɕԓ��p�P�b�g��Ԃ�.
//======================================================================

//	���ʃR�}���h
#define HIDASP_TEST           0x01	//�ڑ��e�X�g

#define	HIDASP_SAMPLING		  0x03	//�T���v�����O�̎��s.
#define HIDASP_POKE           0x04	//�������[��������.
#define HIDASP_PEEK           0x05	//�������[�ǂݏo��.
#define HIDASP_JMP	          0x06	//�w��Ԓn�̃v���O�������s.
#define HIDASP_SET_MODE       0x07	//�f�[�^����胂�[�h�̎w��(00=poll c0=puts c9=exit)
#define HIDASP_PAGE_ERASE	  0x08  //Flash����.
#define HIDASP_KEYINPUT		  0x09	//�z�X�g���̃L�[�{�[�h�𑗂�.
#define HIDASP_PAGE_WRITE     0x0a	//Flash����.

#define HIDASP_USER_CMD		  0x0c	//���[�U�[��`�̃R�}���h�����s����.
#define HIDASP_GET_STRING	  0x0d	//��������������.
#define HIDASP_FLASH_LOCK     0x0e	//ARM FlashROM�G���A�̏������݃��b�N/�������s�Ȃ�.


//	PIC���C�^�[�n (0x10�`0x17) 
//======================================================================
#define PICSPX_SETADRS24      0x10	// 24bit�̃A�h���X��TBLPTR�ɃZ�b�g����.
// SETADRS24 [ CMD ] [ --- ] [ADRL] [ADRH] [ADRU] 
#define PICSPX_GETDATA8L      0x11	//  8bit�f�[�^����擾����.
// GETDATA8L [ CMD ] [ LEN ] [ADRL] [ADRH] [ADRU] 
#define PICSPX_SETCMD16L      0x12	// 16bit�f�[�^�����������.
// SETCMD16L [ CMD ] [ LEN ] [ADRL] [ADRH] [ADRU] [CMD4] [ MS ] data[32]
#define PICSPX_BITBANG        0x14	// MCLR,PGM,PGD,PGC�ebit�𐧌䂷��.
// BITBANG   [ CMD ] [ LEN ] data[60]
#define PICSPX_BITREAD        0x15	// MCLR,PGM,PGD,PGC�ebit�𐧌䂵�A�ǂݎ�茋�ʂ��Ԃ�.
// BITREAD   [ CMD ] [ LEN ] data[60]


#define PICSPX_WRITE24F       0x16	// PIC24F�f�[�^�����������.
#define PICSPX_READ24F     	  0x17	// PIC24F�f�[�^���ǂݏo��.

//	JTAG���C�^�[�n (0x18�`0x1f) 
//======================================================================
#define HIDASP_JTAG_WRITE	  0x18	//JTAG ��������.
#define HIDASP_JTAG_READ	  0x19	//JTAG �ǂݏ���.
#define HIDASP_JTAG_CTRLW	  0x1a	//JTAG �e�M�����̐���.
#define HIDASP_JTAG_CTRLR	  0x1b	//JTAG �e�M�����̐���.


//	AVR�������݃R�}���h�n (0x20�`0x2f)
//======================================================================
// +0      +1      +2        +3         +35
//[ID] [HIDASP_PAGE_TX_*] [len=32] [data(32)] [ isp(4) ]
#define HIDASP_PAGE_TX        0x20	//MODE=00
#define HIDASP_PAGE_RD        0x21	//MODE=01
#define HIDASP_PAGE_TX_START  0x22	//MODE=02  ADDR ������.
#define HIDASP_PAGE_TX_FLUSH  0x24	//MODE=04  �]���� ISP
//                            0x27 �܂ł�PAGE_TX�Ŏg�p����.

#define CMD_MASK              0xf8	//AVR�������݃R�}���h�̌��o�}�X�N.
#define MODE_MASK             0x07	//AVR�������݃R�}���h�̎�ʃ}�X�N.

//	AVR�������݃R�}���h(Control)
#define HIDASP_CMD_TX         0x28	//ISP�R�}���h��4byte����.
#define HIDASP_CMD_TX_1       0x29	//ISP�R�}���h��4byte�����āA����4byte���󂯎��.
#define HIDASP_SET_PAGE       0x2a	//AVR�̃y�[�W�A�h���X���Z�b�g����.
#define HIDASP_SET_STATUS     0x2b	//AVR���C�^�[���[�h�̐ݒ�
#define HIDASP_SET_DELAY      0x2c	//SPI�������ݒx�����Ԃ̐ݒ�.
#define HIDASP_WAIT_MSEC      0x2e	//1mS�P�ʂ̒x�����s��.

//	�R�}���h�͂����܂�.
//======================================================================



//�f�o�C�XID
#define	DEV_ID_FUSION			0x55
#define	DEV_ID_STD				0x5a
#define	DEV_ID_MEGA88			0x88
#define	DEV_ID_MEGA88_USERMODE	0x89
#define	DEV_ID_PIC				0x25
#define	DEV_ID_PIC18F14K		0x14
#define	DEV_ID_SH2A				0x2a
#define	DEV_ID_RX62N			0x62
#define	DEV_ID_ARM_M0			0xa0
#define	DEV_ID_ARM_M3			0xa3
#define	DEV_ID_ARM_FM3			0xf3

// poll mode
#define	POLL_NONE				0
#define	POLL_ANALOG				0xa0
#define	POLL_DIGITAL			0xa0
#define	POLL_PORT				0xc0
#define	POLL_PRINT				0xc1
#define	POLL_END				0xc9

//  BitBang�f�[�^�r�b�g.

#define	JCMD_bitVcc2 0x80
#define	JCMD_bitVcc1 0x40
#define	JCMD_bitTRST 0x20
#define	JCMD_bitSRST 0x10
#define	JCMD_bitTMS	 0x08	// TMS 
#define	JCMD_bitTDI	 0x04	// TDI MOSI
#define	JCMD_bitTCK	 0x02	// TCK
#define	JCMD_bitTDO	 0x01	// TDO MISO

//	�������W�X�^ (0=�o��)

#define	dirTMS		0x80
#define	dirTDI		0x40
#define	dirTCK		0x20
#define	dirTDO		0x10	// ����PIN�̂ݓ���(==1)


/*
 * �h�L�������g

  +--------------+-------------------------------------------------------------+
  |   �R�}���h   |  �p�����[�^              | ����                             |
  +--------------+-----------+-------------------------------------------------+
  |TEST          | ECHO      |          ���ڑ��`�F�b�N���s��.
  +--------------+------+----+------------+---------------+--------------------+
  |POKE          |subcmd|size| address    | data ....     | �������q�`�l��������
  +--------------+------+----+------------+---------------+--------------------+
  |PEEK          |subcmd|size| address    |                 �������q�`�l�ǂݏo��
  +--------------+------+----+------------+------------------------------------+
   subcmd ��Byte,Half,Word�̎w��. size�͓ǂݏ����o�C�g��(16bit) address(32bit)


AVR���C�^�[�n:
  +--------------+-------------------------------------------------------------+
  |   �R�}���h   |  �p�����[�^              | ����                             |
  +--------------+-----------+--------+----------------------------------------+
  |SET_STATUS    | PORTD     | PORTB  | ��LED������s��.
  +--------------+-----------+--------+----------------------------------------+
  |CMD_TX        | ISP_COMMAND[4]           | ��ISP�R�}���h���S�o�C�g���M
  +--------------+--------------------------+----------------------------------+
  |CMD_TX_1      | ISP_COMMAND[4]           | ��ISP�R�}���h���S�o�C�g���M���Ď�M
  +--------------+-----------+--------------+----------------------------------+
  |SET_PAGE      | PAGE_MODE |   ���y�[�W���[�h�̏������ƃA�h���X���Z�b�g
  +--------------+-----------+-------------------------------------------------+
  |PAGE_TX       | LENGTH    |   DATA[ max 32byte ]       | ���y�[�W���[�h��������
  +--------------+-----------+-------------------------------------------------+
  |PAGE_RD       | LENGTH    |                              ���y�[�W���[�h�ǂݏo��
  +--------------+-----------+-------------------------------------------------+
  |PAGE_TX_START | LENGTH    |   DATA[ max 32byte ]       | ���y�[�W���[�h�̏������ƃy�[�W���[�h��������
  +--------------+-----------+-------------------------------------------------+
  |PAGE_TX_FLUSH | LENGTH    |   DATA[ 32byte �Œ�]       | ISP_COMMAND[4]    | ���y�[�W���[�h�������݌�e���������������ݎ��s.
  +--------------+-----------+-------------------------------------------------+
  |SET_DELAY     | delay     |   ���r�o�h�N���b�N���x�̐ݒ�(delay=0..255).
  +--------------+-----------+-------------------------------------------------+

 *��1: PAGE_TX_START��PAGE_TX_FLUSH�͂ǂ����PAGE_TX�ɑ΂���C���r�b�g�Ȃ̂ŁA
       �����𕹂����p�P�b�g���쐬�\�ł��B

 *��2: �R�}���h��LSB���P�ɂȂ��Ă���ꍇ�̓R�}���h���s��A�ǂݎ��t�F�[�Y�����s����܂��B
       �ǂݎ��T�C�Y�́A�z�X�g������w�肳��܂��B

  �����t�F�[�Y�ŕԋp�����p�P�b�g�̍\��
  +--------------+-------------------------------------------------------------+
  |   �R�}���h   |  �ԋp�l                                                     |
  +--------------+--------+-----+-----+----------+------+-----------+----------+
  |TEST          | DEV_ID |VER_L|VER_H|DEV_LOADER| ECHO | FLASH_USED|FLASH_END |
  +--------------+--------+-----+-----+----------+------+-----------+----------+
  |PEEK          | DATA[ max 60 byte ]      | �������q�`�l�ǂݏo���f�[�^.
  +--------------+--------------------------+----------------------------------+

 *��3: HID Report�͐擪�ɕK��ReportID�����邽�߁A
       ��L�ԋp�l�� HID Report�̂Q�o�C�g�ڈȍ~�̈Ӗ��ƂȂ�܂��B

 *��4: DEV_ID �͌��݂̔łł́A�ȉ��̒l�ł��B 
		0x55 : �t���[�W��������AVR�t�@�[���E�F�A
		0x5A : �t���[�W�����Ȃ�AVR�t�@�[���E�F�A
		0x88 : AVR ATMEGA88			
		0x89 : AVR ATMEGA88_USERMODE
		0x25 : PIC18F2550/4550	
		0x14 : PIC18F14K		

		0x2A : Renesas SH-2A
		0x62 : Renesas RX62N
		0xa0 : ARM_Cortex_M0			
		0xa3 : ARM_Cortex_M3			
		0xf3 : ARM_Fujitsu_FM3			
       ECHO�͓n���ꂽ�l�����̂܂ܕԂ��܂�.


*/
