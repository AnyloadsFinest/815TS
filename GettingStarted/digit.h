
#ifndef __DIGIT_INCLUDED
#define __DIGIT_INCLUDED

void transmit_digits();
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
