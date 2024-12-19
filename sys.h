#ifndef __SYS_INCLUDED
#define __SYS_INCLUDED


#define _WEIGHT    0
#define _PARAMETER 1
#define _NUMERIC   2
#define _CAL       3
#define _RAWCOUNTS 4
#define _MESSAGE   5
#define _TARGET_D  6
#define _VIEWID    7
#define _MENU      8
#define _DISPLAY   9
#define _PIECE    10




void scan_system(void);
long parameter_entry(long val,char digits,char start,char string[],char nodec);
void update_display(void);
void displayvalue(long val, char decimal, char deadzero,char str[]);
void display_message(const char *message,char time);
void view_targets(void);
void format_display(long val,char digits,char start,char string[],char nodec);
char display_timer(char mode);
void toggle_units(void);



#endif

