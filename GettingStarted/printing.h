#ifndef __PRINTING_INCLUDED
#define __PRINTING_INCLUDED

#define __DISPLAYED 1
#define __GROSS 2
#define __TARE  3
#define __NET   4
#define __ACC   5
#define __PIECE  6

#define _COM1 0
#define _COM2 2



void print_ticket(int ticketnumber);
void print( int displaymode);
void format_string(long value, char string[],char dec);
void xmit_units( int units);
char canprint(void);
void xmitchar(char ch);
void xmitstring (char string[]);
void process_autoprint(void);
void process_printlockout(void);
long format_deadzero(long value);
#endif