/********************************************************************
 *	FM3 Flash自己書き換え
 ********************************************************************

int	FM3_FlashEraseWrite(int cmd,int adrs,short *buf,int size);
 cmd=1: erase
 cmd=2: write
 
注意:
	- HWの制約上、FlashROM上では実行できません.
	- この関数は必ずRAM上にコピーしたうえで、割り込み禁止状態で実行する
	  必要があります。
	- また、関数内からFlashROM上の関数やデータを読み出すことも出来ません。
	- eraseはセクター単位で行ないます。セクタサイズはデバイスごとに異なり、
	  同一デバイスでも異なるサイズのセクタを持ちます。
	- FM3の構造上、0-4000番地の消去や書き込みは不可です。
	　(vectorが消滅するので、この関数からリターンしたあとで暴走します)

引数:
	- adrs はFlashROMのアドレスを指します。
	- erase時は、引数 buf,sizeを使いません。sizeは暗黙に決まっています。
	- write時は、セクター全体でなくても書き込みできますが、sizeは4の倍数、
	  adrs も4の倍数にする必要があるようです。
 */
#include "common.h"	//FM3依存.

#include "hidcmd.h"
#include "monit.h"
#include "flash.h"


//	特殊アドレス.
#define FM3_ROM_CMD1 (0x00001550)
#define FM3_ROM_CMD2 (0x00000aa8)
#define FM3_ROM_MASK (0xffffc000)	//アドレスマスク.

//	書き込み中のFlashエリア読み出しデータ(シーケンスフラグ).
#define	DPOL	0x80
#define	TOGG	0x40
#define	TLOV	0x20
#define	SETI	0x08

//	Flashエリアを指すvolatileなポインタ.
unsigned short volatile *rom_cmd1;
unsigned short volatile *rom_cmd2;
unsigned short volatile *rom_cmdp;

//	ダミーリードで使用する格納場所.
int		 dummy_val;

//	Flashエリアへデータを書き込むマクロ.
#define	ROMCMD1(x)	*rom_cmd1 = (x)
#define	ROMCMD2(y)	*rom_cmd2 = (y)
#define	ROMCMDP(z)	*rom_cmdp = (z)

//	コマンドシーケンス：Flash消去.
#define	ERASE_CMD()	\
		ROMCMD1(0xaa);	\
		ROMCMD2(0x55);	\
		ROMCMD1(0x80);	\
		ROMCMD1(0xaa);	\
		ROMCMD2(0x55);

//	コマンドシーケンス：Flash書き込み.
#define	WRITE_CMD()	\
		ROMCMD1(0xaa);	\
		ROMCMD2(0x55);	\
		ROMCMD1(0xa0);



/********************************************************************
 *	FlashROMの消去、または書き込みを RAM上で実行する.
 ********************************************************************
    この関数はFlashROM上のコードを直接呼んではいけません。

    RAM上にコピーしたものを呼び出してください。

 */
int	FM3_FlashEraseWrite(int cmd,int adrs,short *buf,int size)
{
	int i,d0,d1,d2,d7,data;
	int err  = 0;
	int bank = adrs & FM3_ROM_MASK;
	FM3_FLASH_IF_TypeDef *flash_if=FM3_FLASH_IF;

	__disable_irq();
	__disable_fault_irq();

	rom_cmd1 = (ushort *) (FM3_ROM_CMD1);	// =0x1550
	rom_cmd2 = (ushort *) (FM3_ROM_CMD2);	// =0x0aa8
	rom_cmdp = (ushort *) (bank);

	flash_if->FASZR = 0x01;	// '01'	Flash ROM 16bitアクセス
	dummy_val = flash_if->FASZR;	// ダミーリード.
	(void) dummy_val;

	if(	cmd == FM3_ERASE_CMD) {
		{
			ERASE_CMD();		//消去シーケンスコマンド発行.
			ROMCMDP(0x30);		//消去アドレスに0x30を書き込む.
			d0 = *rom_cmdp;		//ダミーリード.
			(void) d0;
#if	1		// FlashROMの構成が32bit + 32bit構成なので、
			// 奇数ワードのエリア(xxxxxxx4など)	も一緒に消す.
			{
				rom_cmdp[2] = 0x30;
				d7 = *rom_cmdp & TOGG;	//１回目読み出し実行.
				d1 = *rom_cmdp;			//２回目読み出し実行.
				// d1のSETIビット監視(35uS超過チェック)はここでは省略
			}
#endif
			//消去完了?
			while(1) {
				d7 = *rom_cmdp & TOGG;	//１回目読み出し実行.
				d1 = *rom_cmdp;			//２回目読み出し実行.
				if((d1 & TOGG)==d7) break;	//反転データでなければ抜ける.

				if((d1 & TLOV)) {	//TLOV==1なら、タイミングリミット超過.
					d7 = *rom_cmdp & TOGG;	//１回目読み出し実行.
					d1 = *rom_cmdp;			//２回目読み出し実行.
					if((d1 & TOGG)==d7) break;		//反転データでなければ抜ける.
					//書き込みエラー発生.
					err=1;break;
				}
				//TLOV==0ならwhileループの頭へ.
			}
		}
	}
	if(	cmd == FM3_WRITE_CMD) {
		size = (size+1) >> 1;
		rom_cmdp = (ushort *) (adrs);
		for(i=0;i<size;i++) {
			data = *buf++;
			d7 = data & DPOL;	//書き込みデータのbit7.
			WRITE_CMD();		//書き込みシーケンスコマンド発行.
			ROMCMDP(data);		//ハーフワード書き込み.
			d0 = *rom_cmdp;		//ダミーリード.
			(void) d0;

			//書き込み完了?
			while(1) {
				d1 = *rom_cmdp;	//読み出し実行.
				if((d1 & DPOL)==d7) break;		//反転データでなければ抜ける.

				if((d1 & TLOV)) {	//TLOV==1なら、タイミングリミット超過.
					d2 = *rom_cmdp;	//さらに読み出し実行.
					if((d2 & DPOL)==d7) break;		//反転データでなければ抜ける.
					//書き込みエラー発生.
					err=1;break;
				}
				//TLOV==0ならwhileループの頭へ.
			}
			if(err) break;	//書き込みエラー発生.
			
			//次のアドレス.
			rom_cmdp++;
		}
	}

	flash_if->FASZR = 0x02;	// '10'	Flash ROM 32bitアクセス
	dummy_val = flash_if->FASZR;	// ダミーリード.
	(void) dummy_val;

	__enable_irq();
	__enable_fault_irq();

	return err;
}

/********************************************************************
 *
 ********************************************************************
 */
