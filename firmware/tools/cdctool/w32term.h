#ifndef	_include_monit_h_
#define	_include_monit_h_


#include "packet.h"
//typedef	unsigned char uchar;

typedef	struct {
	uchar bmRequestType;
	uchar cmd;
	short adr;

	uchar data0;
	uchar data1;
	short wlength;
} RxBuf;

//
//	area.
//
enum {
	AREA_RAM    = 0   ,
	AREA_EEPROM = 0x20,
	AREA_PGMEM  = 0x40,
//	reserved      0x60
	AREA_MASK	= 0x60,
};


typedef	struct {
	char *name;
	void (*func)();
} CmdList;


/*
 *	�I�v�V����������`�F�b�N
 */
char  *opt[128];	/* �I�v�V�����������w�肳��Ă����炻�̕�����
					   ������������i�[�A�w��Ȃ����NULL	*/

/*
 *	�I�v�V����������`�F�b�N
 *		optstring �Ɋ܂܂��I�v�V���������́A
 *				  �㑱�p�����[�^�K�{�Ƃ݂Ȃ��B
 */
#define Getopt(argc,argv,optstring)           		\
 {int i;int c;for(i=0;i<128;i++) opt[i]=NULL; 		\
   while( ( argc>1 )&&( *argv[1]=='-') ) {    		\
	 c = argv[1][1] & 0x7f;   						\
       opt[c] = &argv[1][2] ; 						\
       if(( *opt[c] ==0 )&&(strchr(optstring,c))) {	\
         opt[c] = argv[2] ;argc--;argv++;          	\
       }                        					\
     argc--;argv++;           						\
 } }

#if	0
#define Getopt(argc,argv)  \
 {int i;for(i=0;i<128;i++) opt[i]=NULL; \
   while( ( argc>1 )&&( *argv[1]=='-') ) \
    { opt[ argv[1][1] & 0x7f ] = &argv[1][2] ; argc--;argv++; } \
 }
#endif

#define IsOpt(c) ((opt[ c & 0x7f ])!=NULL)
#define   Opt(c)   opt[ c & 0x7f ]

#define Ropen(name) {ifp=fopen(name,"rb");if(ifp==NULL) \
{ printf("Fatal: can't open file:%s\n",name);exit(1);}  \
}

#define Wopen(name) {ofp=fopen(name,"wb");if(ofp==NULL) \
{ printf("Fatal: can't create file:%s\n",name);exit(1);}  \
}

#define Read(buf,siz)   fread (buf,1,siz,ifp)
#define Write(buf,siz)  fwrite(buf,1,siz,ofp)
#define Rclose()  fclose(ifp)
#define Wclose()  fclose(ofp)


#define	ZZ	printf("%s:%d: ZZ\n",__FILE__,__LINE__);

#endif	//_include_monit_h_

