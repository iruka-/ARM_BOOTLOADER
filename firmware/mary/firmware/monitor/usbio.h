#ifndef	_usbio_h_
#define	_usbio_h_

#include "fifo.h"

/****************************************************************
 *	
 ****************************************************************
 */
void	USBIOinit(void);
void 	USBenqueue(uchar *buf,int size);
int		USBavailable(void);
int		USBread(uchar *buf,int len);
int		USBwrite(uchar *data,int size);
void	USBputch(int ch);
int		USBinkey(void);
int		USBgetch(void);


#endif
