	.text

	.extern		_main
#==============================
#      SH-2 Exception Vector
#==============================
	# パワーオンリセットベクタ	/*←リセットベクタ*/
	.org	0x00
	.long	__startup					
	.long	_stack_base		/*↓ラベル__startupにジャンプ。スタックは_stack_baseに設定される。*/
	# マニュアルリセットベクタ
	.long	__startup
	.long	_stack_base		/*↑*/
	.org	0x84			/*←この.textセクションの先頭から0x84バイトオフセットを確保*/
	# Trap33
	#.long	_trap33_exception	/*←TRAP 33用ベクタ。専用のハンドラへ分岐する。*/
	.org	0x88
	# Trap34
	#.long	_trap34_exception	/*←TRAP 34用ベクタ。専用のハンドラへ分岐する。*/
	.org	0x200			/*↓ほかのハンドラは用いない。*/
	# ERI_0
	.long	0x0
	# RXI_0
	.long 0x0
	# TXI_0
	.long	0x0
	# TEI_0
	.long	0x0			/*↑*/

	.org	0x400
	.global		__startup
	.type		__startup, @function
__startup:
	# スタックを設定する
	mov.l	_stack_base_addr, sp	/*←ユーザプログラム側でスタックを設定*/
.ifdef ROM_TARGET			/*↓(A) ROM化時のみ使用される。この部分はプロセッサの起動直後に実行されるべき初期化操作が記述される。*/
	# cs1の外部RAMを初期化。ない場合は以下をコメントアウトしてください。
#	mov.l	__init_cs1_addr, r1
#	jsr	@r1
#	nop
	
	#割り込み優先度を10に下げる
	stc	sr,r1
	mov	#0xF0, r2
	not	r2,r2
	and	r2,r1
	add	#0xA0,r1
	ldc	r1,sr

	/* dataセクションをROM->RAMコピー */	/*↓ROMに保存されている初期化済みのdataセクションをRAM領域にコピーする。*/
	mov.l	_sdata_loadaddr_addr, r1
	mov.l	_sdata_addr, r2
	mov.l	_edata_addr, r3
__copy_loop:	
	mov.b	@r1, r4
	mov.b	r4, @r2
	add	#1, r1
	add	#1, r2
	cmp/gt	r2, r3
	bt	__copy_loop
	nop					/*↑*/
.endif /* ROM_TARGET */				/*↑(A)*/
	

.ifndef ROM_TARGET				/*↓(B)ユーザプログラムデバッグ時のみ実行する．ROM化時は削除される．*/
	#PC,SR以外のレジスタをスタックに退避する
	sts.l	macl,@-r15			/*↓この時点でのスタックにレジスタの内容を積む*/
	sts.l	mach,@-r15
	stc.l	vbr,@-r15
	stc.l	gbr,@-r15
	sts.l	pr,@-r15
	mov.l	r14,@-r15
	mov.l	r13,@-r15
	mov.l	r12,@-r15
	mov.l	r11,@-r15
	mov.l	r10,@-r15
	mov.l	r9,@-r15
	mov.l	r8,@-r15
	mov.l	r7,@-r15
	mov.l	r6,@-r15
	mov.l	r5,@-r15
	mov.l	r4,@-r15
	mov.l	r3,@-r15
	mov.l	r2,@-r15
	mov.l	r1,@-r15
	mov.l	r0,@-r15			/*↑*/

	# ユーザ例外ベクタのベースアドレスを設定します。
	mov.l	_user_vbr_addr, r1		/*↓内蔵RAMの最上位の4バイトに、代替ベクタのアドレスを格納する*/
	mov.l	_user_vector_base_addr, r2
	mov.l	r2, @r1				/*↑*/
.endif /* !ROM_TARGET */

	mov.l	_start_addr_list,r1
	jsr	@r1				/*←main()関数にジャンプ*/
	nop

.ifndef ROM_TARGET				/*↓(C)ユーザプログラムデバッグ時のみ実行する．ROM化時は削除される．*/
	#PC,SR以外のレジスタをスタックから復帰する
	mov.l	@r15+,r0			/*↓退避したレジスタの内容を復帰*/
	mov.l	@r15+,r1
	mov.l	@r15+,r2
	mov.l	@r15+,r3
	mov.l	@r15+,r4
	mov.l	@r15+,r5
	mov.l	@r15+,r6
	mov.l	@r15+,r7
	mov.l	@r15+,r8
	mov.l	@r15+,r9
	mov.l	@r15+,r10
	mov.l	@r15+,r11
	mov.l	@r15+,r12
	mov.l	@r15+,r13
	mov.l	@r15+,r14
	lds.l	@r15+,pr
	ldc.l	@r15+,gbr
	ldc.l	@r15+,vbr
	lds.l	@r15+,mach
	lds.l	@r15+,macl		/*↑*/

	rts				/*←スタブでダウンロードプログラムが呼ばれたアドレスにリターンする*/
	nop
.endif /* !ROM_TARGET */		/*↑(C)*/

_loop_here:				/*←誤ってここまで来たときに無限ループに陥らせるためのセイフティネット。*/
	bra	_loop_here_addr
	nop
	.align	0x4
_loop_here_addr:
	.long	_loop_here
	
	.align	0x4
_start_addr_list:
	.long	_main
_user_vbr_addr:
	.long	_user_vbr
_user_vector_base_addr:
	.long	_user_vector_base
.ifdef ROM_TARGET			/*↓(D)ROM化の時に含まれる。*/
_sdata_loadaddr_addr:
	.long	_sdata_loadaddr
_sdata_addr:
	.long	_sdata
_edata_addr:
	.long	_edata
#__init_cs1_addr:
#	.long	__init_cs1
.endif /* ROM_TARGET */			/*↑(D)*/
_stack_base_addr:
	.long	_stack_base
	
_trap33_exception:			/*←Trap 33用例外ハンドラ*/
	#PC,SR以外のレジスタをスタックに退避する
	sts.l	macl,@-r15
	sts.l	mach,@-r15
	stc.l	vbr,@-r15
	stc.l	gbr,@-r15
	sts.l	pr,@-r15
	mov.l	r14,@-r15
	mov.l	r13,@-r15
	mov.l	r12,@-r15
	mov.l	r11,@-r15
	mov.l	r10,@-r15
	mov.l	r9,@-r15
	mov.l	r8,@-r15
	mov.l	r7,@-r15
	mov.l	r6,@-r15
	mov.l	r5,@-r15
	mov.l	r4,@-r15
	mov.l	r3,@-r15
	mov.l	r2,@-r15
	mov.l	r1,@-r15
	mov.l	r0,@-r15

	mov.l	_trap33_exception_func_addr, r1		/*←Cの関数へ分岐*/
	jsr	@r1
	nop

	#PC,SR以外のレジスタをスタックから復帰する
	mov.l	@r15+,r0
	mov.l	@r15+,r1
	mov.l	@r15+,r2
	mov.l	@r15+,r3
	mov.l	@r15+,r4
	mov.l	@r15+,r5
	mov.l	@r15+,r6
	mov.l	@r15+,r7
	mov.l	@r15+,r8
	mov.l	@r15+,r9
	mov.l	@r15+,r10
	mov.l	@r15+,r11
	mov.l	@r15+,r12
	mov.l	@r15+,r13
	mov.l	@r15+,r14
	lds.l	@r15+,pr
	ldc.l	@r15+,gbr
	ldc.l	@r15+,vbr
	lds.l	@r15+,mach
	lds.l	@r15+,macl
	# 例外からリターン
	RTE
	nop

	.align	0x4
_trap33_exception_func_addr:
#	.long	_trap33_exception_func
	
_trap34_exception:
	#PC,SR以外のレジスタをスタックに退避する
	sts.l	macl,@-r15
	sts.l	mach,@-r15
	stc.l	vbr,@-r15
	stc.l	gbr,@-r15
	sts.l	pr,@-r15
	mov.l	r14,@-r15
	mov.l	r13,@-r15
	mov.l	r12,@-r15
	mov.l	r11,@-r15
	mov.l	r10,@-r15
	mov.l	r9,@-r15
	mov.l	r8,@-r15
	mov.l	r7,@-r15
	mov.l	r6,@-r15
	mov.l	r5,@-r15
	mov.l	r4,@-r15
	mov.l	r3,@-r15
	mov.l	r2,@-r15
	mov.l	r1,@-r15
	mov.l	r0,@-r15

	mov.l	_trap34_exception_func_addr, r1		/*←Cの関数へ分岐*/
	jsr	@r1
	nop

	#PC,SR以外のレジスタをスタックから復帰する
	mov.l	@r15+,r0
	mov.l	@r15+,r1
	mov.l	@r15+,r2
	mov.l	@r15+,r3
	mov.l	@r15+,r4
	mov.l	@r15+,r5
	mov.l	@r15+,r6
	mov.l	@r15+,r7
	mov.l	@r15+,r8
	mov.l	@r15+,r9
	mov.l	@r15+,r10
	mov.l	@r15+,r11
	mov.l	@r15+,r12
	mov.l	@r15+,r13
	mov.l	@r15+,r14
	lds.l	@r15+,pr
	ldc.l	@r15+,gbr
	ldc.l	@r15+,vbr
	lds.l	@r15+,mach
	lds.l	@r15+,macl
	# 例外からリターン
	RTE
	nop
	
#			.EXPORT		_Restart
#			.SECTION	P,CODE,ALIGN=4
	.global	_Restart
_Restart:
	mov.l		@r5+,r0			/*		; ジャンプ先アドレス取得	*/
	ldc			r4,vbr			/*		; ユーザVBR設定				*/
	jmp			@r0	/* ユーザプログラムにジャンプ	*/
	mov.l		@r5,r15			/*; スタック取得 */
#			.END


	.align	0x4
_trap34_exception_func_addr:
#	.long	_trap34_exception_func


	