/*********************************************************************
 *	gccに無い関数を補完.
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

#if	0	// ROM化PG用：RAM上で動作する場合は要らない.
	uint *s;
	s = &_sidata;
	t = &_sdata;
//;
//;	データセクションをRAMにコピーする.
//;
	while( t < &_edata ) {
		*t++ = *s++;
	}
#endif
//;
//;	bssセクションをゼロ・クリアする.
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
	/* キャッシュ制御レジスタ1 */
	CCNT.CCR1.BIT.ICF = 1;	/* 命令キャッシュフラッシュ */
	CCNT.CCR1.BIT.OCF = 1;	/* オペランドキャッシュフラッシュ */

	CCNT.CCR1.BIT.ICE = 1;	/* 命令キャッシュ有効 */
	CCNT.CCR1.BIT.OCE = 1;	/* オペランドキャッシュ有効 */
	CCNT.CCR1.BIT.WT = 1;	/* ライトスルー有効 */

	return;
}

void cache_disable(void)
{
	/* キャッシュ制御レジスタ1 */
	CCNT.CCR1.BIT.WT = 0;	/* ライトスルー無効 */
	CCNT.CCR1.BIT.ICE = 0;	/* 命令キャッシュ無効 */
	CCNT.CCR1.BIT.OCE = 0;	/* オペランドキャッシュ無効 */

	CCNT.CCR1.BIT.ICF = 1;	/* 命令キャッシュフラッシュ */
	CCNT.CCR1.BIT.OCF = 1;	/* オペランドキャッシュフラッシュ */

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
 *	やや放置
 *********************************************************************
 */
void nop(void)
{
}

void sleep(void)
{
	while(1) { }
}
