#ifndef __GPIO_INCLUDED
#define __GPIO_INCLUDED


#define NIGHTIME 1
#define DAYTIME  0

#define OFF          0
#define GREEN        1
#define RED          2
#define G_BLINK      3
#define R_BLINK      4
#define GREEN_RED    5
#define G_R_BLINK    6


#define _PASSIVE     0
#define _ACTIVE      1

#define DLY_100US    100

void gpio_init();
void latchbankA(char ctrl);
void latchbankB(char ctrl);
void latchbankC(char ctrl);
void brightness(char mode, char powersave);
void scoreboard_init(void);
void status_light(char mode);
void scan_light_sensor(char mode);
void brightlevel(char brightctrl);
char init_radio(void);
void stoplight(char mode);
char scan_learn(void);
void isr_learn(void);
char scan_switch(void);
char scan_fen(void);
long scan_id(void);
char scan_test(void);
void reset_driver(char ctrl);
void reset_digit(char ctrl);
void recevie_comport(char data, char port);
void set_active(char ctrl);
void latchbankC(char ctrl);
void EINT1Handler( void );
void DelayResolution100us(Int32U Dly);
void currentloop_relay(char mode);
char scan_switch2(void);
void relay_control(char mode);
void Digit_Select(char digit);
void Output_Enable (char ctrl);
char read_legal(void);
void Select_Digit(long digit);
void relays_function(long set1, long set2);
void init_relays(void);
#endif
