■ cdclinkの概要

   「cdclink」は、OpenOCD （ARM用のオープンソース JTAG-ICE デバッグツール）
   から使用できる JTAGインターフェース（リンクケーブル）の１つです。

   下記のような構成を用いて、ARMマイコンのデバッグやFLASH書き込みに使用できます。

   +-----------------+
   |                 |
   | Windowsパソコン |   <=== GDB + OpenOCD + cdclink.dll（本ソフト）
   |                 |
   +-----------------+
          ||
          || USBケーブル
          ||
   +-----------------+
   | cdclink マイコン|   <=== JTAGインターフェース(USB機能つきの小さなマイコン)
   +-----------------+        （今回紹介するハードウェア）
          ||
          || JTAG ケーブル
          ||
          vv
     +-------------+
     | ARMマイコン |      <=== デバッグ対象の ターゲット・マイコンボード
     +-------------+


■ 特徴

   cdclinkはWindowsパソコンからは、シリアルポート(USB-CDCクラス)として
   認識されます。
   
   OpenOCDのドライバーとしての実装方法は外部に DLL を置く(cdclink.dll)
   ことで、分離実装を試みています。

   各種cdclinkマイコンのファームウェアソースが公開されていますので、
   OpenOCDと組み合わせる JTAGアダプターを独自に自作することも可能です。


■ 試し方

  openocd/openocd.exe を以下のようなオプションを付けて起動します。

  D:> openocd.exe -f cdc_if.cfg -f stm32.cfg -f batch.cfg

  オプション '-f' で指定するconfigファイルの詳細は、以下の通りです
  -----------------------------------------------------------------------------
  cdc_if.cfg は 「cdclink」インターフェース の構成を記述したものです。
  stm32.cfg  は ターゲットマイコン(stm32)の構成を記述したものです。
  batch.cfg  は openocd.exe 起動後の振る舞い（バッチ処理等）を記述したものです。
  -----------------------------------------------------------------------------



■ openocd.exeの動作概要

  openocd.exe はコマンドラインから起動すると、(shutdownするまでは) TCP/IP の
  3333番ポートと4444番ポートを受け持つデバッグサーバーとして働きます。

  この状態で、gdbから、localhost:3333 に接続することで、ターゲットマイコンと
  gdbの間を取り持つことができます。
  
  また、localhost:4444番は telnetプロトコルで接続することで、対話的な操作
  （例えばFLASH ROMの書き込みなど）を行なうことができます。




■ ディレクトリ構成

 openocd -+- ソース.
          |
          +--helper\   ヘッダーファイル.
          +--jtag\     ヘッダーファイル.
          +--jimtcl\   ヘッダーファイル.
          |
          +--openocd_patch\  openocd本体側作成用の改造点

 helper,jtag,jimtclディレクトリのヘッダーファイルは使用するopenocd.exeをビルド
 したソースと同じものです。


■ プログラムの再ビルド方法

   Windows上のMinGW-gccコンパイラを用いてmakeしてください。
   makeすると、hidblast.dll が作成されます。

   openocd.exe本体を再ビルドする方法は、以下のURLを参照してください。

-http://hp.vector.co.jp/authors/VA000177/html/2010-09.html
   
   今回の改造部分ソースはopenocd_patch/ ディレクトリに置いています。

   出来上がった openocd.exe 本体は、ドライバーとして、同一ディレクトリに存在する
   cdclink.dll を起動時に呼び出します。
   (cdclink.dllの読み込みに成功した場合は、dummyドライバーと置き換わります)
   (cdclink.dllが存在しなければ、dummyドライバーのままになります)

   openocd.exeビルド時のオプションは以下のように行ないました。
   libUSB-win32経由でのft2232やjlinkインターフェースなども使用可能です。
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



■ openocd-nogdb-hacks.exe について

   openocd.exe をビルドするさいに、以下の定義
   ----
   /* define for enabling armv7 gdb workarounds */
   #define ARMV7_GDB_HACKS
   ----

   をコメントアウトしたものをご用意いたしました。(openocd-nogdb-hacks.exe)

   もし、使用する gdb や Insight のバージョンによってopenocd との相性が
   悪い場合は、openocd.exe の代わりに、この実行ファイルをご使用ください。



■ ライセンス

   OpenOCDの配布ライセンスに準じます。


■ 展望

   cdclink.dll ファイルを(自力で)差し替えるだけで、自作デバイスがサポート可能になります。

   （たとえばATtiny2313を使用したJTAGアダプターなどをサポート出来る可能性があります）

   cdclink.dll のエントリーポイントは、
      DLL_int get_if_spec(struct jtag_command **q);
   だけです。引数のstruct jtag_command **qのqには、openocd本体のjtag_command_queueという
   グローバル変数のアドレスを渡します。
   戻り値は、(intになっていますが) ドライバー記述構造体のアドレスになります。


