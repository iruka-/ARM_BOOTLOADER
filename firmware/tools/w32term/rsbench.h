int		RS_puts(uchar *buf,int len);
int		RS_keyInput(void);
int		RS_keyInputUP(void);
void	RS_echohost(int port_no,int baudrate);
void	bench_tx();
int		bench_rx(int record);
void	RS_benchmark(int port_no,int baudrate);
void	RS_terminal(int port_no,int baudrate);
