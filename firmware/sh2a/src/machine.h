#ifndef	machine_h_
#define	machine_h_


void	set_imask( int imask )   ;
void	set_vbr(void *vbr)	;
void	set_cr(int sr)	;
void	set_fpscr(int fpscr);
int		bswap16(int i);
int		bswap32(int i);
void	cache_enable(void);
void	cache_disable(void);
void	Restart(void *jump, void *adr);
void	nop(void);
void	sleep(void);


#endif
