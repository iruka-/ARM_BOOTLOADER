/*********************************************************************
 *	gcc�ɖ����֐���⊮.
 *********************************************************************
 */
#include "includes.h"



#define	uint unsigned int
/*********************************************************************
 * ==== B and D sections initialization ====
 *********************************************************************
 */
#define	EXTERN	extern uint

EXTERN	_bss_start__	;/* start address for the .bss section.   */
EXTERN	_bss_end__		;/* end address for the .bss section.     */

int _INITSCT(void)					
{
	uint *t;

#if	0	// ROM��PG�p�FRAM��œ��삷��ꍇ�͗v��Ȃ�.
	uint *s;
	s = &_sidata;
	t = &_sdata;
//;
//;	�f�[�^�Z�N�V������RAM�ɃR�s�[����.
//;
	while( t < &_edata ) {
		*t++ = *s++;
	}
#endif
//;
//;	bss�Z�N�V�������[���E�N���A����.
//;
	t = &_bss_start__;
	while( t < &_bss_end__ ) {
		*t++ = 0;
	}
	return 0;
}

void set_imask( int imask )   
{
    int  srreg;

    asm("   stc sr,%0   ":"=r"(srreg):);   
    srreg = (srreg &  ~0xf0 ) | ((imask << 4) & 0xf0) ;   
    asm("   ldc %0,sr "::"r"(srreg));   
}
void set_vbr(void *vbr)	
{
    asm(" ldc %0,vbr "::"r"(vbr)); 
}
void set_cr(int sr)	
{
    asm(" ldc %0,sr "::"r"(sr)); 
}
void set_fpscr(int fpscr)
{
#ifdef	USE_FPU		// by Makefile.
    asm(" lds %0,fpscr "::"r"(fpscr)); 
#endif
}

int	bswap16(int i)
{
	return 	((i & 0x00ff) << 8 )
		|	((i & 0xff00) >> 8 )
	;
}
int	bswap32(int i)
{
	return 	((i & 0x000000ff) << 24)
		|	((i & 0x0000ff00) << 8 )
		|	((i & 0x00ff0000) >> 8 )
		|	((i & 0xff000000) >> 24)
	;
}
void cache_enable(void)
{
	/* �L���b�V�����䃌�W�X�^1 */
	CCNT.CCR1.BIT.ICF = 1;	/* ���߃L���b�V���t���b�V�� */
	CCNT.CCR1.BIT.OCF = 1;	/* �I�y�����h�L���b�V���t���b�V�� */

	CCNT.CCR1.BIT.ICE = 1;	/* ���߃L���b�V���L�� */
	CCNT.CCR1.BIT.OCE = 1;	/* �I�y�����h�L���b�V���L�� */
	CCNT.CCR1.BIT.WT = 1;	/* ���C�g�X���[�L�� */

	return;
}

void cache_disable(void)
{
	/* �L���b�V�����䃌�W�X�^1 */
	CCNT.CCR1.BIT.WT = 0;	/* ���C�g�X���[���� */
	CCNT.CCR1.BIT.ICE = 0;	/* ���߃L���b�V������ */
	CCNT.CCR1.BIT.OCE = 0;	/* �I�y�����h�L���b�V������ */

	CCNT.CCR1.BIT.ICF = 1;	/* ���߃L���b�V���t���b�V�� */
	CCNT.CCR1.BIT.OCF = 1;	/* �I�y�����h�L���b�V���t���b�V�� */

	return;
}

//	jmp (pc,vbr)
void Restart(void *jump, void *adr)
{
	void (*func)(void);
	
	set_vbr(adr);

	func = (void (*)()) jump;
	func();

}

/*********************************************************************
 *	�����u
 *********************************************************************
 */
void nop(void)
{
}

void sleep(void)
{
	while(1) { }
}
