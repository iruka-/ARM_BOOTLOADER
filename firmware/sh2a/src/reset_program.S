	.text

	.extern		_main
#==============================
#      SH-2 Exception Vector
#==============================
	# �p���[�I�����Z�b�g�x�N�^	/*�����Z�b�g�x�N�^*/
	.org	0x00
	.long	__startup					
	.long	_stack_base		/*�����x��__startup�ɃW�����v�B�X�^�b�N��_stack_base�ɐݒ肳���B*/
	# �}�j���A�����Z�b�g�x�N�^
	.long	__startup
	.long	_stack_base		/*��*/
	.org	0x84			/*������.text�Z�N�V�����̐擪����0x84�o�C�g�I�t�Z�b�g���m��*/
	# Trap33
	#.long	_trap33_exception	/*��TRAP 33�p�x�N�^�B��p�̃n���h���֕��򂷂�B*/
	.org	0x88
	# Trap34
	#.long	_trap34_exception	/*��TRAP 34�p�x�N�^�B��p�̃n���h���֕��򂷂�B*/
	.org	0x200			/*���ق��̃n���h���͗p���Ȃ��B*/
	# ERI_0
	.long	0x0
	# RXI_0
	.long 0x0
	# TXI_0
	.long	0x0
	# TEI_0
	.long	0x0			/*��*/

	.org	0x400
	.global		__startup
	.type		__startup, @function
__startup:
	# �X�^�b�N��ݒ肷��
	mov.l	_stack_base_addr, sp	/*�����[�U�v���O�������ŃX�^�b�N��ݒ�*/
.ifdef ROM_TARGET			/*��(A) ROM�����̂ݎg�p�����B���̕����̓v���Z�b�T�̋N������Ɏ��s�����ׂ����������삪�L�q�����B*/
	# cs1�̊O��RAM���������B�Ȃ��ꍇ�͈ȉ����R�����g�A�E�g���Ă��������B
#	mov.l	__init_cs1_addr, r1
#	jsr	@r1
#	nop
	
	#���荞�ݗD��x��10�ɉ�����
	stc	sr,r1
	mov	#0xF0, r2
	not	r2,r2
	and	r2,r1
	add	#0xA0,r1
	ldc	r1,sr

	/* data�Z�N�V������ROM->RAM�R�s�[ */	/*��ROM�ɕۑ�����Ă��鏉�����ς݂�data�Z�N�V������RAM�̈�ɃR�s�[����B*/
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
	nop					/*��*/
.endif /* ROM_TARGET */				/*��(A)*/
	

.ifndef ROM_TARGET				/*��(B)���[�U�v���O�����f�o�b�O���̂ݎ��s����DROM�����͍폜�����D*/
	#PC,SR�ȊO�̃��W�X�^���X�^�b�N�ɑޔ�����
	sts.l	macl,@-r15			/*�����̎��_�ł̃X�^�b�N�Ƀ��W�X�^�̓��e��ς�*/
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
	mov.l	r0,@-r15			/*��*/

	# ���[�U��O�x�N�^�̃x�[�X�A�h���X��ݒ肵�܂��B
	mov.l	_user_vbr_addr, r1		/*������RAM�̍ŏ�ʂ�4�o�C�g�ɁA��փx�N�^�̃A�h���X���i�[����*/
	mov.l	_user_vector_base_addr, r2
	mov.l	r2, @r1				/*��*/
.endif /* !ROM_TARGET */

	mov.l	_start_addr_list,r1
	jsr	@r1				/*��main()�֐��ɃW�����v*/
	nop

.ifndef ROM_TARGET				/*��(C)���[�U�v���O�����f�o�b�O���̂ݎ��s����DROM�����͍폜�����D*/
	#PC,SR�ȊO�̃��W�X�^���X�^�b�N���畜�A����
	mov.l	@r15+,r0			/*���ޔ��������W�X�^�̓��e�𕜋A*/
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
	lds.l	@r15+,macl		/*��*/

	rts				/*���X�^�u�Ń_�E�����[�h�v���O�������Ă΂ꂽ�A�h���X�Ƀ��^�[������*/
	nop
.endif /* !ROM_TARGET */		/*��(C)*/

_loop_here:				/*������Ă����܂ŗ����Ƃ��ɖ������[�v�Ɋׂ点�邽�߂̃Z�C�t�e�B�l�b�g�B*/
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
.ifdef ROM_TARGET			/*��(D)ROM���̎��Ɋ܂܂��B*/
_sdata_loadaddr_addr:
	.long	_sdata_loadaddr
_sdata_addr:
	.long	_sdata
_edata_addr:
	.long	_edata
#__init_cs1_addr:
#	.long	__init_cs1
.endif /* ROM_TARGET */			/*��(D)*/
_stack_base_addr:
	.long	_stack_base
	
_trap33_exception:			/*��Trap 33�p��O�n���h��*/
	#PC,SR�ȊO�̃��W�X�^���X�^�b�N�ɑޔ�����
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

	mov.l	_trap33_exception_func_addr, r1		/*��C�̊֐��֕���*/
	jsr	@r1
	nop

	#PC,SR�ȊO�̃��W�X�^���X�^�b�N���畜�A����
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
	# ��O���烊�^�[��
	RTE
	nop

	.align	0x4
_trap33_exception_func_addr:
#	.long	_trap33_exception_func
	
_trap34_exception:
	#PC,SR�ȊO�̃��W�X�^���X�^�b�N�ɑޔ�����
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

	mov.l	_trap34_exception_func_addr, r1		/*��C�̊֐��֕���*/
	jsr	@r1
	nop

	#PC,SR�ȊO�̃��W�X�^���X�^�b�N���畜�A����
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
	# ��O���烊�^�[��
	RTE
	nop
	
#			.EXPORT		_Restart
#			.SECTION	P,CODE,ALIGN=4
	.global	_Restart
_Restart:
	mov.l		@r5+,r0			/*		; �W�����v��A�h���X�擾	*/
	ldc			r4,vbr			/*		; ���[�UVBR�ݒ�				*/
	jmp			@r0	/* ���[�U�v���O�����ɃW�����v	*/
	mov.l		@r5,r15			/*; �X�^�b�N�擾 */
#			.END


	.align	0x4
_trap34_exception_func_addr:
#	.long	_trap34_exception_func


	