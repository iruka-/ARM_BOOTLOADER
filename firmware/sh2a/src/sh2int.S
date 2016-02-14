#
#	----------------------------------------------------------------
#	アセンブラ記述の割り込みハンドラー	: FPU 非使用版.
#	----------------------------------------------------------------
#	割り込みハンドラーをアセンブラで書く場合、Makefile の ASRCARM に
#	 sh2int.S などを加え、common/intprg.c のINTASM を有効にする.
#
#
#

	.text


	.extern	_int_cmt_cmi0
	.extern	_usb_interrupt

#	----------------------------------------------------------------
#		USB割り込み
#	----------------------------------------------------------------
#
	.align	0x2
	.globl	_INT_USB_USBI
_INT_USB_USBI:
	mov.l	_1usb_interrupt, r1
	jsr/n	@r1

	resbank
	rte
	nop

	.align	0x2
_1usb_interrupt:
	.long	_usb_interrupt


#	----------------------------------------------------------------
#		タイマー割り込み
#	----------------------------------------------------------------
#
	.align	0x2
	.globl	_INT_CMT0_CMI0
_INT_CMT0_CMI0:
	mov.l	_1int_cmt_cmi0, r1
	jsr/n	@r1

	resbank
	rte
	nop

	.align	0x2
_1int_cmt_cmi0:
	.long	_int_cmt_cmi0




#define	SYS_exit	1
#define	SYS_fork	2
#define	SYS_read	3
#define	SYS_write	4
#define	SYS_open	5
#define	SYS_close	6
#define	SYS_wait4	7
#define	SYS_creat	8
#define	SYS_link	9
#define	SYS_unlink	10
#define	SYS_execv	11
#define	SYS_chdir	12
#define	SYS_mknod	14
#define	SYS_chmod	15
#define	SYS_chown	16
#define	SYS_lseek	19
#define	SYS_getpid	20
#define SYS_isatty  21
#define SYS_fstat   22
#define SYS_time 	23

#	----------------------------------------------------------------
#		__trap34 の置き換え.
#	----------------------------------------------------------------
#
	.align	0x2
	.globl	___trap34
___trap34:
	mov		#23,r1
	cmp/hi	r1,r4
	bt.s	_L2
	mova	_L8,r0
	add		r4,r4
	add		r4,r4
	mov.l	@(r0,r4),r1
	mov		r5,r4
	mov		r6,r5
	mov		r7,r6
	mov		r8,r7
	jmp		@r1
	nop
_L2:
	rts


	.extern	_trap_f00	
	.extern	_trap_exit	
	.extern	_trap_fork	
	.extern	_trap_read	
	.extern	_trap_write	
	.extern	_trap_open	
	.extern	_trap_close	
	.extern	_trap_wait4	
	.extern	_trap_creat	
	.extern	_trap_link	
	.extern	_trap_unlink
	.extern	_trap_execv	
	.extern	_trap_chdir	
	.extern	_trap_mknod	
	.extern	_trap_chmod	
	.extern	_trap_chown	
	.extern	_trap_lseek	
	.extern	_trap_getpid
	.extern	_trap_isatty
	.extern	_trap_fstat 
	.extern	_trap_time 	

	.align	0x2
_L8:
	.long	_trap_f00	
	.long	_trap_exit	
	.long	_trap_fork	
	.long	_trap_read	
	.long	_trap_write	
	.long	_trap_open	
	.long	_trap_close	
	.long	_trap_wait4	
	.long	_trap_creat	
	.long	_trap_link	
	.long	_trap_unlink
	.long	_trap_execv	
	.long	_trap_chdir	
	.long	_trap_f00	
	.long	_trap_mknod	
	.long	_trap_chmod	
	.long	_trap_chown	
	.long	_trap_f00	
	.long	_trap_f00	
	.long	_trap_lseek	
	.long	_trap_getpid
	.long	_trap_isatty
	.long	_trap_fstat 
	.long	_trap_time 	


#
