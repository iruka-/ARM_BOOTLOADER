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
 *	OpenOCDドライバーとしての初期化およびコマンドキューアドレス受け取り
 *********************************************************************
 */
DLL_int get_if_spec(struct jtag_command **q)
{
	jtag_command_arg = q;
//	printf("=*=get_if_spec(%x)=%x\n",(int)q,(int)q[0]);
	return (int) &hidblast_interface;
}










/*********************************************************************
 *	以下のAPIエントリーはCDCLinkデバイスを個別コントロールする場合のもの
 *********************************************************************
 *	OpenOCD 側 から呼び出されることはありません。
 */

/*********************************************************************
 *	CDCLinkデバイスの使用開始
 *********************************************************************
 */
DLL_int openDevice(int argc,char *argv)
{
	(void) argc;
	(void) argv;
	char *cdc_serial_number  =NULL;
	char *cdc_serial_baudrate=NULL;

#if	RX_DEFFERED				//遅延読み込みを許可.
	rxInitBuf();
#endif

	int dev_id = UsbInit(1,0,cdc_serial_number,cdc_serial_baudrate);
	if( dev_id == 0) {
		return -1;
	}
	return dev_id;
}

/*********************************************************************
 *	CDCLinkデバイスへコマンドを送信.
 *********************************************************************
 */
DLL_int sendCommand(char *cmd ,int size )
{
	return RS_putdata_sub( cmd , size );
}
/*********************************************************************
 *	CDCLinkデバイスへコマンドを送信.
 *********************************************************************
 */
DLL_int getResult(char *result ,int size )
{
	return RS_getdata( result , size );
}
/*********************************************************************
 *	CDCLinkデバイスの使用終了
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
 * DLLエントリポイント関数の型と典型的なスケルトンコード
 */
BOOL APIENTRY					/* int __stdcall */
DllMain(HINSTANCE hInstance, DWORD ul_reason_for_call, LPVOID pParam)
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		/* ここにグローバル変数等の初期化コードを書く */
		/* ※loaddll でDLLをロードした時はここが実行される */
		break;

	case DLL_PROCESS_DETACH:
		/* ここにグローバル変数等の後始末コードを書く */
		/* ※freedll でDLLをアンロードした時はここが実行される */
		break;

	case DLL_THREAD_ATTACH:
		/* ここにスレッド毎に必要な変数等の初期化コードを書く */
		break;

	case DLL_THREAD_DETACH:
		/* ここにスレッド毎に必要な変数等の後始末コードを書く */
		break;
	}
	return TRUE;
}
/*********************************************************************
 *
 *********************************************************************
 */
