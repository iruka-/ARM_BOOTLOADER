//
//	=====================================================================
//	リセット直後のブートローダー固有の処理.
//	=====================================================================
//	概要:
//		0000_0000 番地開始のブートローダーとして使用する場合に、
//		ユーザー側ファームウェア(0000_4000番地) を起動させるジャンパーが
//		ONなら、そちらに切り替えて起動する.
//
//		もうひとつの機能としては、armon/armbootのbootコマンドが実行された
//		場合に、ソフトウェアリセット直後にそれを（RAM上の署名コードにより）
//		判別して、同様のユーザー側ファームウェアを起動する処理に移る.
//
//	注意:
//		この関数 check_user_boot() はcrt0.cの初期化以前に呼び出されるため、
//		.dataセクションのコピーと.bssセクションのクリアが行なわれていない
//		(さらに各種I/O初期化も行なわれていない)状態で呼び出されることに注意!
//
#include "mcu.h"

#if	_ROMADRS

//
//	リセットベクター.
//
void SetSoftwareResetVector(int *vect)
{
	// dummy.
}
	// ---------------------------------------------------
	// 0000_4000番地にビルドする場合は不要.
	// ---------------------------------------------------

#else

	//
	// ---------------------------------------------------
	// 0000_0000番地にビルドする場合は以下のように,
	// RAMベクターへ飛ぶ余分なテーブルが追加される.
	// ---------------------------------------------------
	//


#include "monitor/gpio.h"



//
// cdctool/cdcbootからユーザープログラムをBOOTする機能を実装.
//
#define	REBOOT_FUNCTION	(1)	
//
// 電源投入時、Port P39 が GND接地されていたらユーザープログラムをBOOTしたいなら定義.
//
#define	USER_BOOT_FUNCTION	Yes

#define	USER_BOOT_JUMPER	P39			// ジャンパーの指定.
#define	USER_BOOT_VECTOR	0x4000		// ユーザープログラムの指定.


#define	FM3_RAMADRS_LO		0x1fff0000	// RAM実装番地
#define	FM3_RAMADRS_HI		0x20010000
#define	FM3_ROMADRS_LO		0x00000000	// ROM実装番地
#define	FM3_ROMADRS_HI		0x00100000



	// ---------------------------------------------------
	// 0000_0000番地にビルドする場合は
	// Software Reset 直後かどうかを判定して、
	// 別のアプリケーションを起動するコードも追加される.
	// ---------------------------------------------------
	//
#define	MARK0	0x55aa0f00
#define	MARK1	0x55aa0f01
#define	MARK2	0x55aa0f02

static int software_Reset_State[3]={0,0,0};
static int *software_Reset_New_Vector=0;

//void 	*g_Vectors[64];

void	Reset_Handler(void) ;
#define	ROM_VECTOR	(uint*) 0x00000100
#define	RAM_VECTOR	(uint*) 0x1fff0000

void	check_user_boot()
{

#if	REBOOT_FUNCTION		// cdctool/cdcbootからユーザープログラムをBOOTする機能を実装.
	if(	(software_Reset_State[0]==MARK0 ) &&
		(software_Reset_State[1]==MARK1 ) &&
		(software_Reset_State[2]==MARK2 )) {

		// Software Reset 直後と判定.
		software_Reset_State[0]=0;

		//
		// ユーザーベクターアドレスをVTORにセット.
		//
		SCB->VTOR = software_Reset_New_Vector;

		void (*func)(void);
		func = (void (*)()) software_Reset_New_Vector[1];	// SP , PC ,...
		// 飛ぶ.
		func();
	}
#endif

#ifdef	USER_BOOT_FUNCTION 	// Port P39 が GND接地されていたらユーザープログラムをBOOT.

	// BootJumperをPULLUPする.
	pinModePullup(USER_BOOT_JUMPER,1);
	
	//
	// 4000番地のユーザーベクターを見る.
	//
	int *user_vector_p = (int*) USER_BOOT_VECTOR;
	int user_sp = user_vector_p[0];
	int user_pc = user_vector_p[1];

	//
	// 4000番地のユーザーベクターが正しいかどうか確認する.
	//
	if(( user_sp >= FM3_RAMADRS_LO )
	 &&( user_sp <  FM3_RAMADRS_HI )
	 &&( user_pc >= USER_BOOT_VECTOR)
	 &&( user_pc <  FM3_ROMADRS_HI)  ){
		if( digitalRead(USER_BOOT_JUMPER)==0 ) {
			void (*func)(void);
			//
			// 4000番地のユーザーベクターをVTORにセット.
			//
			SCB->VTOR = USER_BOOT_VECTOR;
			//
			// 4000番地のユーザーベクターへ飛ぶ.
			//
			func = (void (*)()) (USER_BOOT_VECTOR+4);	// Initial PC.
			func();
		}
	}
#endif

}

//
//	ソフトウェアリセットを実行する直前にアプリケーションのベクターテーブル
//	を設定する.
//
void SetSoftwareResetVector(int *vect)
{
	software_Reset_State[0]=MARK0;
	software_Reset_State[1]=MARK1;
	software_Reset_State[2]=MARK2;

	software_Reset_New_Vector = vect;
}


#endif	// _ROMADRS
