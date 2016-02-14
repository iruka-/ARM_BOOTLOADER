*libmapleを使いこなせ!! 第二弾


**概要
-----------------------------------------------------------------------------------
libmaple （leaflabsが開発し、mapleで使われているSTM32用のArduino風ライブラリ）を
ベースにして作成したARM用の簡易モニタープログラムです。

CQ-STARM , STM8S-DISCOVERY , STBee , STBeeMini の基板に対応しています。


**QuickStart 
-----------------------------------------------------------------------------------
CQ-STARM基板:
-cdctool.zipを解凍します。
-cdctool/ 直下のMakefileがありますので、
--Linux コマンドライン
--もしくはWindowsのコマンドプロンプトから
 D:\cdctool> make
             ~~~~
--を実行してください。
-makeに成功したら、cdctool/build/CQSTARM.hex が生成されています(0x0800_3000番地スタート)
-armbootを使用する場合は、
 D:\cdctool> w.bat
-もしくは、dfuw.exeを使用してCQ-STARMに転送し、実行してください。

-シリアルポートのinfファイルはwin32drivers/serial/ にあります。
--(STM32のサンプル用infを使用する場合はVID:PIDを書き換えるかファーム側のVID:PIDをSTM32のものに合わせるかして、デバイス認識するようにください）

-デバイス認識したら、host/cdctool.exe を起動してメモリーダンプや逆アセンブルを試してみて
ください。
-scriptを指定して起動することで、Lチカなどを実行することも可能です。



**改造方法、ビルド方法
-----------------------------------------------------------------------------------

（１）cdctool/Makefile に基板選択の定義があります。
 # Valid BOARDs: maple, maple_native, ...
 #BOARD ?= maple
 #BOARD ?= STBee
 #BOARD ?= STBeeMini
 #BOARD ?= STM32VLD
 BOARD ?= CQSTARM
 #BOARD ?= DISCOVERY
-どれか１つを有効にしてください。

~
（２）CQSTARM , DISCOVERYは 0x0800_0300 から起動するelf/hexを生成します。
-開始番地を変更したい場合はlibmaple/support/ld/基板名/flash.ld と、libmaple/libmaple/libmaple.h 内の番地定義の両方を書き換える必要があります。

（３）HEXファイルはelfと同時に生成されます。
-libmaple/*.bat というバッチファイルにて、armboot経由でhexを書き込んで即実行します。

（４）ビルド環境(CodeSourceryG++ Lite)はCodeSourceryのサイトから入手しても良いですし、
-maple-0.0.11 のwindows版を入手してそれを展開して得たコンパイラのbinディレクトリに
そのまま実行パスを通してもＯＫです。

（５）CQSTARMとDISCOVERY(STM8SのSTLINK)のポート名称はSTBeeやMiniの定義順に準じています。
--というよりはSTBee.cppをそのまま流用させていただいております。

~

**解説
-----------------------------------------------------------------------------------
-このモニタープログラムは、仮想COMデバイス経由でCQ-STARMなどの基板に接続します。
-プロトコルはarmon/armbootのものを流用しています。
-簡単に説明すると

-- 0xaa,0x55 を受信することで、モニターモードに移行します。
-- 0x01,[ armonのコマンド列(31byte) ] というパケット(32byte固定長)を受け取ると、
   それをコマンドとして解釈して、返答が必要なら32byte固定長の返答パケットをCDCで
   返します。

-- COMデバイスの探索は、Win32レジストリのCOMデバイスを列挙して、最後に列挙された
   comポートを決めうちでオープンしています。(とりあえず。)

-使い方はarmon/armbootとほぼ同じです。
-AVR/PIC/PIC24F ライターとしての動作確認はまだです。


**問題点
-----------------------------------------------------------------------------------

- wirish の SerialUSB.read(buf,count)やSerialUSB.read() の下位層である、
  usbReceiveBytes(uint8* recvBuf, uint32 len); の実装にはやや難があるようです。

  （COMポートに書き込まれた受信データをFIFOとして扱っておらず、PCから送られた
  　パケット単位で保持しているため、ＰＣから連続で送られたものを、受信側で
  　小バイト数づつ小分けにしてread()すると読んだデータを再度読まされる状況が
  　発生します。）

- 現状、対策は困難なのでモニター側の実装は32byte固定長レコードで送受信するように
  してバグを回避しています。




**謝辞
-----------------------------------------------------------------------------------

-STBee , STBeeMini , STM32VLD への対応にあたり、[[xshigeさん:http://psp.dip.jp/web/cgi-bin/note/index.cgi?%b7%c7%bc%a8%c8%c4]]の[[Maple_0011_Adaptation_20110611.zip:http://cid-bbd4ba3beeca403e.office.live.com/self.aspx/pub/Maple%5E_0011%5E_Adaptation%5E_20110611.zip]]
の成果を思いっきり使わせていただきましたので、ここに謝辞を申し上げます。

