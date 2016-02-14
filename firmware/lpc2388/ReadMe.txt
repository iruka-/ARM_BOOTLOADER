■ 概要

  これは、ＣＱ出版社雑誌 Interfaceマガジン2009-5月号の CQ-FRK-NXP 付録基板用のHIDデバイス用の
  HID bootloader 兼 簡易monitorです。


■ 現在のステータス

   メモリーダンプ、逆アセンブル等を実行できます。
   2000番地以降のアドレスに任意の application (hexファイル) を書き込んで実行することが出来ます。

   将来的にはAVR/PICライター機能などを実装することを考えています。


■ 試し方

  (1)ELM-ChaNさんのサイトからlpcsp書き込みツールをダウンロードしてWindowsに入れます。
  (2)JP2(BOOT)ジャンパーをCloseした状態でCP2102(USB-シリアル変換チップ)経由でPCに接続します。
  (3)NXPチップ内蔵USB側のUSBコネクタもあらかじめPCに接続しておきます。
  (4)w.bat にて firmware/main-0000.hex をFlashに書き込みます。
  (5)JP2(BOOT)ジャンパーをCloseした状態でリセットします。
  (6)a.bat にて host/armon.exe を起動します。ここでメモリーダンプ等を実行します。

HIDブートローダーのテスト方法:
  上記main-0000.hex が動作している状態で、
  (7)firmwareディレクトリの b.bat を実行します
     すると、main-2000.hex(application) をFlashの2000番地～に 書き込んで起動します。

  ・bootloaderとapplicationは、大部分のソースが共用されています。
  ・applicationにはサイズ制限がないので、いろいろな拡張を行うことが出来ます。


■ ディレクトリ構成



 arm7mon --+-- host/ ----------------   ホストPCツール
           |
 		   +-- firmware/ --+--- hwlib/  ライブラリ部
                           |
                           +--- usb/    USBフレームワーク部

■ バッチファイル
   w.bat :
   		出来たmain-0000.hex をシリアル経由でLPC2388に書き込みます。(要lpcsp.exe,lpcsp.ini)
   b.bat :
   		出来たmain-2000.hex をUSB経由(HIDデバイス動作中)でLPC2388に書き込んで実行します。
   mkall.bat:
   		main-0000.hex（0番地:ブートローダー） とmain-2000.hex (2000番地:アプリ)
   		の両方をまとめて作成します。


■ プログラムの再ビルド方法

   ここでは LPCXpresso が用意した開発環境を使わずに、普通のARM用gccとMakeを
　 使ってビルドします。

   （１）CoreSourcery G++ Lite(ARM用:無償)をインストールします。

   （２）WinARMもしくはWinAVR をインストールして、C:/WinARM/utils/bin 
         もしくは、C:/WinAVR/utils/bin のみに実行PATHを通しておきます。
         （これは、make.exe や rm.exe などのunix系コマンドを使うためです）

   （３）src/ ディレクトリにて普通にmakeしてください。


■ Linuxでの動作方法

   （１）Linux ( ここでは ubuntu10.4を想定します ) で使用することが出来ます。
   （２）ビルドの準備(libusbの取得)
        # apt-get install libusb-dev
   （３）host/ ディレクトリにて、armon / armboot をビルドします。
        $ cd host
        $ make
   （４）root権限で実行してください。
        $ sudo ./armon

    USBデバイスの使用権限をＯＳ側で適切に設定すれば、ユーザー権限で動作することも出来ます。


■ 現状の問題点

   （１） ブートローダーとしての機能は実装しました。
		  ただし、現在ブートジャンパーの判定処理はありませんので、
		  	C:> armboot -r
		  もしくは
		  	C:> armon
		  	ARM> boot 2000
		  のいずれかの方法で、2000番地開始のファームウェアを起動するようにします。

   （２） FLASH ROMの0x8000～0x78000番地の領域はFLASHのSECTORサイズが4kBではなく32kBに
		　なっています。ですので、その領域の(32kBブロックの)途中からHEXを書き込みたい場合は、
          あらかじめ
		  C:> armboot -E
		  のように全消去しておいてから書き込みするようにしてください。
         (0x2000番地から連続で書き込む場合、32kBブロックは正しく消去されます)

		  この制限はFLASHのセクタ長が固定サイズの他のARMデバイスと共通処理になっている
		　ために生じています。

	（３）bb.bat は使用できません。(USBからブートローダーの書き換えは使用できません。)

		　この理由は、FLASH ROMの0x00～0xffのような番地に例外ベクターが存在していて、これを
		　消去してしまうと(USBなどの割り込みが途絶えて)暴走してしまうからです。

		  他のARMデバイスではNVIC を使用して割り込みの例外ベクターを０番地以外の場所に
          設定することが出来るので問題ありません。




■ ライセンス

  BSDライセンス

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



■ その他

メモリーダンプ,逆アセンブラ以外のコマンドはあまり実装されていません。



