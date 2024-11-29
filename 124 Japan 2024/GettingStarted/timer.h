#ifndef __TIMER_INCLUDED 
#define __TIMER_INCLUDED

//#define TIME_INTERVAL	(Fpclk)/100 - 1
#define TIMER0_TICK_PER_SEC   25
#define TIMER0_IN_FREQ        25000000
#define TIMER1_TICK_PER_SEC   250
#define TIMER1_IN_FREQ        25000000

#define TIMER0 	0        // 	
#define TIMER1 	1        // 	

#define START 1
#define TIMEOUT 0

#define MOTION_SAMPLE     0
#define DISPLAY_UPDATE    1
#define KEYSCAN           2
#define DISPLAY_PRM       3
#define DISPLAY_C         4
#define PARAMEDIT_B       5
#define PARAMEDIT_D       6
#define SAMPLE_DELAY      7
#define MESSAGE           8
#define KEYPRESS          9
#define DEAD              10
#define BAR               11
#define DISPLAY_TGT       12
#define ENTER_CAL         13
#define SCROLL_U          14
#define SCROLL_D          15
#define SECOND            16
#define KEYPRESSC         17
#define IZSM_START        18
#define IZSM_TIMEOUT      19
#define DISPLAY_ID        20
#define DISPLAY_ID_LONG   21
#define STREAM_DELAY0     22
#define TICREAD           23
#define KEYPRESSP         24
#define UNIT              25
#define POLL              26
#define REMOTE_DISPLAY    27
#define REMOTE_SWITCH1    28
#define STREAM_DELAY1     29
#define REMOTE_SWITCH2    30
#define STATUS_RUN        31
#define STREAM_DELAY2     32
#define STATUS_FAST       33
#define STATUS_SEC        34
/* depending on the CCLK and PCLK setting, CCLK = 48Mhz, PCLK = 1/4 CCLK
10mSec = 240.000-1 counts */
//#define TIME_INTERVAL	227996   // PCLK=5.7MHZ	
//#define TIME_INTERVAL	455992   // PCLK=11.4MHZ	
//#define TIME_INTERVAL	479999   // PCLK=12.MHZ	
//#define TIME_INTERVAL1 23999   // PCLK=12.MHZ	4XTR0

DWORD init_timer0( DWORD timerInterval );
DWORD init_timer1( DWORD timerInterval );
void enable_timer( BYTE timer_num );
void disable_timer( BYTE timer_num );
void reset_timer( BYTE timer_num );
char timer(char timer,char mode);
char motion_timer(char mode);
char peak_timer(char mode);
char track_timer(char mode);
// timer name definitions to their index value
char timer(char timer,char mode);
char timer_one(char timer,char mode);
char com_timeout_timer(char mode);
char command_timeout_timer(char mode);
char Scroll_timer(char mode);
char message_timer(char mode);
char refresh_timer(char mode);
//char track_timer(char mode);
#endif /* end __TIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/