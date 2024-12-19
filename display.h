#ifndef __DISPLAY_INCLUDED
#define __DISPLAY_INCLUDED

extern long   RAW_DISPLAY[12]; // holds raw binary pattern for scanning LED display

void update_bar_graph(char flash,char blank);
void update_status(char blank);
void scale_status(void);
void output_display(char decimal);
void display_test(void);

void transmit_digits(char decimal);
void send_digit(char digit);
void scoreboard_startup(void);
void string_todisplay(char string[6], char blink);
void error_message(char error);
void clear_display(void);
void clear_annuciators(void);
void transmit_status(void);
void demo_display(void);
void text_message(char message[6],char delay);
void displayvalue(long val, char decimal, char deadzero, char str[]);
void scoreboard_id(void);
#endif

