/*********************************************************************
 *	C D C L i n k
 *********************************************************************
 */

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define ERROR_OK					(0)
#define	LOG_ERROR(...)
#define	HAVE_STDINT_H

#include "helper/command.h"

#include "rs232c.h"
#include "packet.h"

#define	DLL_int	__declspec(dllexport) int __stdcall

extern	struct jtag_interface hidblast_interface;
static	struct jtag_command   **jtag_command_arg;

struct jtag_command *get_command_arg(void) {
//	printf("=*=get_command_arg(%x)=%x\n",(int)jtag_command_arg,(int)jtag_command_arg[0]);
	return *jtag_command_arg;
}
/*********************************************************************
 *	OpenOCD�h���C�o�[�Ƃ��Ă̏���������уR�}���h�L���[�A�h���X�󂯎��
 *********************************************************************
 */
DLL_int get_if_spec(struct jtag_command **q)
{
	jtag_command_arg = q;
//	printf("=*=get_if_spec(%x)=%x\n",(int)q,(int)q[0]);
	return (int) &hidblast_interface;
}










/*********************************************************************
 *	�ȉ���API�G���g���[��CDCLink�f�o�C�X���ʃR���g���[������ꍇ�̂���
 *********************************************************************
 *	OpenOCD �� ����Ăяo����邱�Ƃ͂���܂���B
 */

/*********************************************************************
 *	CDCLink�f�o�C�X�̎g�p�J�n
 *********************************************************************
 */
DLL_int openDevice(int argc,char *argv)
{
	(void) argc;
	(void) argv;
	char *cdc_serial_number  =NULL;
	char *cdc_serial_baudrate=NULL;

#if	RX_DEFFERED				//�x���ǂݍ��݂�����.
	rxInitBuf();
#endif

	int dev_id = UsbInit(1,0,cdc_serial_number,cdc_serial_baudrate);
	if( dev_id == 0) {
		return -1;
	}
	return dev_id;
}

/*********************************************************************
 *	CDCLink�f�o�C�X�փR�}���h�𑗐M.
 *********************************************************************
 */
DLL_int sendCommand(char *cmd ,int size )
{
	return RS_putdata_sub( cmd , size );
}
/*********************************************************************
 *	CDCLink�f�o�C�X�փR�}���h�𑗐M.
 *********************************************************************
 */
DLL_int getResult(char *result ,int size )
{
	return RS_getdata( result , size );
}
/*********************************************************************
 *	CDCLink�f�o�C�X�̎g�p�I��
 *********************************************************************
 */
DLL_int closeDevice(void)
{
	UsbExit();
	return 0;					// OK.
}

/*********************************************************************
 *
 *********************************************************************
 */

/*
 * DLL�G���g���|�C���g�֐��̌^�ƓT�^�I�ȃX�P���g���R�[�h
 */
BOOL APIENTRY					/* int __stdcall */
DllMain(HINSTANCE hInstance, DWORD ul_reason_for_call, LPVOID pParam)
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		/* �����ɃO���[�o���ϐ����̏������R�[�h������ */
		/* ��loaddll ��DLL�����[�h�������͂��������s����� */
		break;

	case DLL_PROCESS_DETACH:
		/* �����ɃO���[�o���ϐ����̌�n���R�[�h������ */
		/* ��freedll ��DLL���A�����[�h�������͂��������s����� */
		break;

	case DLL_THREAD_ATTACH:
		/* �����ɃX���b�h���ɕK�v�ȕϐ����̏������R�[�h������ */
		break;

	case DLL_THREAD_DETACH:
		/* �����ɃX���b�h���ɕK�v�ȕϐ����̌�n���R�[�h������ */
		break;
	}
	return TRUE;
}
/*********************************************************************
 *
 *********************************************************************
 */
