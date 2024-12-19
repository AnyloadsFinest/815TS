#include    "includes.h"

extern volatile DWORD timer_counter;
//long lTIMERTICS;  // main system timer

/*
#define STATUSRUN     0
#define STARTCYCLE    1
#define BAUDTIMEOUT   2
#define COMTIMEOUT    3
#define SCAN_LIGHT    4
#define STATUSLEARN   5
#define RADIODETECT   6
#define RELAY         7
#define REFRESH       8
#define SCANTIMEOUT   9
#define AXEL          10
#define AXELDISPLAY   11
#define TMOTION       12
#define THREEBLINK    13
#define MIRROR        14
#define COM           15
*/

// timer values for different timers, 25 timer tics a second						  
const long lTIMEVALUE[] = {   
                         25,   // Status LED blink timer (slow)
					         10,   // delay for startup cycling of numbers
					         75,   // timer for  BAUDTIMEOUT
						 250,  // timer for COM timeout
						 25,   // scan light sensor timeout 
                         2,    // Status LED blink timer (fast)
                         50,   // RADIO detect timeout
						 12,   // RELAY toggle delay 50 
						 2 ,   // REFRESH timer to update display 6
                         250,  // timer for string detect timeout 
						 325,  // no activity timeout for axel weight
						 60,   // display axel timer   
						 50,   // motion timer 
						 130,  // blink scoreboard 3 times timer
						 125,   // mirror alternate 5 sec timer
                                                 1   
                         };


//////////////////////////////////////////////////////////////////////////////////////
//  This is the timer functions
//  Maintains all the timers used in the program
//  The timer works off the globle lTIMERTICS counting at 25Hz
//  The timer variable selects which timer to access as defined in the timer.h
//  the mode variable selects the operation, whether to test for timeout, or restart the timer
//////////////////////////////////////////////////////////////////////////////////////
char timer(char timer,char mode)
{
static long lasttime[15];  // holds the last time value for all the timers
                          // timeout values for the different timers, counting at 25 ticks a second
long ltime;
long diff;

long lTIMERTICS=timer_counter;
//1TIMERTICS = timer_counter;
  if (mode==1) // start the timer
  {
  lasttime[timer]=lTIMERTICS;
  return(0);
  }

  ltime=lasttime[timer];
  diff = labs(lTIMERTICS-ltime); // calculate the difference in time
  if ( diff > lTIMEVALUE[timer]) return(1);
  else return(0);
} // end of function timer()






