#ifndef __STRING_INCLUDED
#define __STRING_INCLUDED

void process_COM_xmit(int port);
void stringformat(int port,int format);
void process_outputstream0(void);
void process_outputstream1(void);
void process_serial_poll_commands(int port);
void xmitport(char string[],int port);
char poll_com_port1(void);
char poll_com_port2(void);
char poll_com_port3(void);
void command_string(void);
void process_outputstream2(void);
char read_COM_level(int port); // read the external push button switch status
#endif
