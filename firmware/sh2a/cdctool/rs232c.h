//#define	BAUD_RATE	230400 // 460800
//#define	BAUD_RATE	460800		// 115200 / 230400 / 460800
//#define	BAUD_RATE	500000		// 115200 / 230400 / 460800
#define	BAUD_RATE	1000000


int		RS_putdata(	unsigned char *buf , int cnt );
int		RS_putc(int c);
int		RS_keyInput(void);
int		RS_getdata(unsigned char *buf,int cnt);
int		RS_checkdata();
int		RS_init(int portno,int baudrate);
void	RS_exit(void);

void	RS_terminal(int port_no,int baudrate);
void 	RS_echohost(int port_no,int baudrate);
void	RS_usbhost( int port_no,int baudrate);
void 	RS_benchmark(int port_no,int baudrate);
int 	RS_printinfo(int pr);
