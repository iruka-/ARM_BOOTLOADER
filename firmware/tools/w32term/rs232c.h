#ifndef	rs232c_h
#define	rs232c_h

int		RS_putdata(	unsigned char *buf , int cnt );
int		RS_put_flush(void);

int		RS_putc(int c);
int		RS_getdata(unsigned char *buf,int cnt);
int		RS_checkdata(void);
int		RS_getc(void);
int		RS_init(int portno,int baudrate);
void	RS_exit(void);
int		RS_printCommInfo(int pr,int port_no);
int		RS_test_target(int portno,int baudrate);
int		RS_printinfo(int port_no,int baudrate,int pr);
int		RS_printinfoAll(int port_no,int baudrate,int pr,int all);

void	RS_error_exit(char *msg);

//----------------------------------------------------------
//#define	BAUD_RATE	230400
//#define	BAUD_RATE	460800
//#define	BAUD_RATE	500000
#define	BAUD_RATE	1000000
#endif

