#ifndef	packet_h_
#define	packet_h_

int		hidReadPoll(uchar *buf, int Length, int id);
int		hidWriteBuffer(uchar *buf, int len);
int		hidWriteBuffer2(uchar *buf, int len);
int		hidReadBuffer(uchar *buf, int len);

int		hidCommand(int cmd,int arg1,int arg2,int arg3);
int		hidPokeMem(int addr,int data0,int mask);
int		hidPeekMem(int addr);
int		hidasp_init(int type, const char *serial);
void	hidasp_close();
int		hidasp_cmd(const unsigned char cmd[4], unsigned char res[4]);
void	memdump(char *msg, char *buf, int len);
int		hidasp_list(char * string);

#define	PACKET_SIZE		60	//64

#endif

