*libmapleを使いこなせ !! 第３弾


**概要
-----------------------------------------------------------------------------------
これは、libmaple 
		（leaflabsが開発し、mapleで使われているSTM32用のArduino風ライブラリ）
をベースにしたＣ言語専用のフレームワークです。（Ｃ＋＋言語サポートを削除しています）


CQ-STARM , STM8S-DISCOVERY , STBee , STBeeMini の基板に対応しています。


現状の動作は、仮想COMデバイスになって、単純な文字単位のエコーバックを行っています。


**用途
-----------------------------------------------------------------------------------
ずばり、仮想COMブートローダー作成専用です。

その他の用途では、（コード削減のための） Ｃ言語限定にする必要性はほとんどありません。







**QuickStart 
-----------------------------------------------------------------------------------
CQ-STARM基板:
-cdcboot.zipを解凍します。
-cdcboot/ 直下のMakefileがありますので、
--Linux コマンドライン
--もしくはWindowsのコマンドプロンプトから
 D:\cdcboot> make
             ~~~~
--を実行してください。
-makeに成功したら、cdcboot/build/CQSTARM.hex が生成されています(0x0800_3000番地スタート)
-armbootを使用する場合は、
 D:\cdcboot> w.bat
-もしくは、dfuw.exeを使用してCQ-STARMに転送し、実行してください。

-シリアルポートのinfファイルはwin32drivers/serial/ にあります。










**改造方法、ビルド方法
-----------------------------------------------------------------------------------

（１）cdcboot/Makefile に基板選択の定義があります。
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

**ソース説明
-----------------------------------------------------------------------------------
-wirish/ やlibraries/ ディレクトリを削除しています。
-monitor/ 以下に簡易モニターのソースがあります。
--その中の関数の一部のみ機能しています。

**ビルドのフレームワークについて
-----------------------------------------------------------------------------------
はじめは、maple-bootloaderのフレームワークを使用していましたが、libmaple/ 以下の
ソースとの親和性に難点がある（割り込み関数の名称が異なる）ことが分かったので、
libmapleのフレームワークに戻して、startupのＣ言語モジュールのビルドオプションのみ
-Os に変更して再ビルドしました。

wirish/以下のusb関数は、代替品として monitor/usbio.c に移してあります。
wirish/以下のdigitalWrite()関数についても、代替品としてmonitor/util.c を置きました。


いろいろ試行錯誤を行ったため、両方のフレームワークの残骸が残ったままになっています。


**謝辞
-----------------------------------------------------------------------------------

-STBee , STBeeMini , STM32VLD への対応にあたり、[[xshigeさん:http://psp.dip.jp/web/cgi-bin/note/index.cgi?%b7%c7%bc%a8%c8%c4]]の[[Maple_0011_Adaptation_20110611.zip:http://cid-bbd4ba3beeca403e.office.live.com/self.aspx/pub/Maple%5E_0011%5E_Adaptation%5E_20110611.zip]]
の成果を思いっきり使わせていただきましたので、ここに謝辞を申し上げます。

