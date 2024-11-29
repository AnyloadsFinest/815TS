#define BUFFMAX 254       // character RX circular buffer length

#define COM0 0                          //RS-232
#define COM1 1                          //radio  
#define COM2 2                          //20mA
#define COM3 3                          //RS-485

#define NONE 0
#define EVEN 1
#define ODD  2                          //Parity select

void com0buffer(char ch);
char readcom0buffer(void);
void com1buffer(char ch);
char readcom1buffer(void);
void com2buffer(char ch);
char readcom2buffer(void);
int auto_learn_baud(void);
int baudratescan(void);
char baudcheck(int baud);
void init_comports(void);
char radio_cmd(char *cmd);
char xmit_string(char *cmd);
void scan_RX_buffer(void);
void scan_RX1_buffer(void);
char receive_timeout(char cmd);
