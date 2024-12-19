#ifndef __TIMEDATE_INCLUDED
#define __TIMEDATE_INCLUDED



#define _24HOUR   0
#define _12HOUR   1
#define _INTDATE  2 
#define _USDATE   3 


void init_clock(void);
void format_time(char string[]);
void change_clock(void);
void change_date(void);
void change_time(void);
void format_date(char string[]);
int time_date_edit(char string2[], char mode);
long extract_digits(char *p);

#endif