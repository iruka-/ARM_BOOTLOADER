/*********************************************************************
 *	�t�r�a�f�o�C�X�̎g�p�J�n�A�I���A�A�N�Z�X���̃��[�e�B���e�B.
 *********************************************************************
 *
 *�`�o�h:
int		UsbInit(int verbose,int enable_bulk);	������.
int		UsbExit(void);	�I��.
void 	UsbBench(int cnt,int psize);
void	UsbDump(int adr,int arena,int size);
void 	UsbPoke(int adr,int arena,int data0,int data1);
int 	UsbPeek(int adr,int arena);
int 	UsbRead(int adr,int arena,uchar *buf,int size);
 *
 *�����֐�:
usb_dev_handle	*open_dev(void);
void	dumpmem(usb_dev_handle *dev,int adr,int cnt);
void	pokemem(usb_dev_handle *dev,int adr,int data0,int data1);
void	memdump(void *ptr,int len,int off);
 */

#include <stdio.h>
#include <time.h>

#include "w32term.h"
#include "util.h"

/****************************************************************************
 *	�������[���e���_���v.
 ****************************************************************************
 */
void memdump(void *ptr,int len,int off)
{
	unsigned char *p = (unsigned char *)ptr;
	int i,j,c;

	for(i=0;i<len;i++) {
		if( (i & 15) == 0 ) printf("%06x",(int)p - (int)ptr + off);
		printf(" %02x",*p);p++;
		if( (i & 15) == 15 ) 
		{
#if	1	// ASCII DUMP
			printf("  ");
			for(j=0;j<16;j++) {
				c=p[j-16];
				if(c<' ') c='.';
				if(c>=0x7f) c='.';
				printf("%c",c);
			}
#endif
			printf("\n");
		}
	}
	printf("\n");
}




int 	UsbIsMary(void)
{
	return 0;
}
