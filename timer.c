#include "includes.h"

//static long lTIMERTICS = 0 ;
//static long dTIMERTICS = 0 ;
static long lasttimeA[35];  // holds the last time value for all the timers
static long lasttimeB[10];  // holds the last time value for all the timers

// timer values for different timers, 25 timer tics a second						
const long lTIMEVALUE[] = {
/*  
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
*/
                         6,   // MOTION_SAMPLE  timer, used for sample weights on intervals of time
			 6,   // DISPLAY_UPDATE      refresh timer
			 6,   // KEYSCAN        timer for debouncing keypad
			175,  // DISPLAY_PRM    parameter display time out
			 20,  // DISPLAY_C      display blinking C timer in CAL mode on display
                         12,  // PARAMEDIT_B    blink an individual digit in edit value mode
                         50,  // PARAMEDIT_D
                         50,  // SAMPLE_DELAY   used when deadloading or spanning, wait for scale to stablize
			 12,  // MESSAGE        used to display a message for timer duration
			 35,  // KEYPRESS       used for detecting that long keypresses on keypad
                         12,  // DEAD           flash DEAD on display
                         12,  // BAR            flash graph
			 75,  // DISPLAY_TGT    display target value
			 75,  // ENTER_CAL      used to time key presses for entering into calibration
			 12,  // SCROLL_U       used for scrolling up through cal prm
			 12,  // SCOLL_D        used for scrolling up through cal prm
                         25,  // 1 SEC          1 second timer
                         20,  // KEYPRESSC      keypress timer used in calibration
                         15,  // IZSM_START     startup delay to wait for things to settle before IZSM activates
                         25,  // IZSM_TIMEOUT   timeout periode for trying to zero the scale for IZSM startup
                         50,  // DISPLAY_ID     timeout for diplaying ID number 
                         150, // DISPLAY_ID_LONG   timeout for diplaying ID number long delay
                         6,   // STREAM_DELAY0     0.25 delay for tranmission delays for streaming to comport
                         25,  // TICREAD         timeout for tic read
                         12,  // KEYPRESSP       premature long keypress detection       
                         25,  // UNIT           used for toggle of UNITs on long keypresses
                         6,   // POLL           used to controll the speed of the '?' POLL character in remote display mode
                         150, //REMOTE_DISPLAY  no sctivity timeout for remote display
                         6,   //REMOTE_SWITCH1  used to debounce a re,mote switch attached to com1
                         6,   // STREAM_DELAY1      delay for tranmission delays for streaming to comport
                         6,   // REMOTE_SWITCH2 used to debouce remote switch com2
                         12,  //status                   
                         6,
                         6,  //fast
                         25, 
                         };


/******************************************************************************
** Function name:		Timer0Handler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**				executes each 10ms @ 60 MHz CPU Clock
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void Timer0Handler (void)
{
 
    T0IR = 1;			 /* clear interrupt flag */         
    lTIMERTICS++; 
   // DISPLAY.TIMERTIC = lTIMERTICS;
    __enable_interrupt();       /* handles nested interrupt */  
    //VICVectAddr = 0;		/* Acknowledge Interrupt */     
} // 

/******************************************************************************
** Function name:		Timer0Handler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**				executes each 10ms @ 60 MHz CPU Clock
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void Timer1Handler (void)
{
   //static char status;
    T1IR = 1;			 /* clear interrupt flag */
       
    dTIMERTICS++; 
   // transmit_digits();      // update the display on the board       
    
    /*
    if (status==1) // toggle LED
       { //                 
        reset_digit(0); // ON timer   
        status=0;      
       }
     else
       { // 
        reset_digit(1);  // Off timer
        status=1;       
       }        
      */
    
    __enable_interrupt();       /* handles nested interrupt */
    
   // VICVectAddr = 0;		/* Acknowledge Interrupt */
    
} // 
/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void enable_timer( BYTE timer_num )
{
    if ( timer_num == 0 )
    {
		T0TCR = 1;
    }
    else
    {
		T1TCR = 1;
    }
    return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void disable_timer( BYTE timer_num )
{
    if ( timer_num == 0 )
    {
		T0TCR = 0;
    }
    else
    {
		T1TCR = 0;
    }
    return;
} // 

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void reset_timer( BYTE timer_num )
{
    DWORD regVal;
    
    if ( timer_num == 0 )
    {
		regVal = T0TCR;
		regVal |= 0x02;
		T0TCR = regVal;
    }
    else //
    {
		regVal = T1TCR;
		regVal |= 0x02;
		T1TCR = regVal;
    }
    return;
}

DWORD init_timer0 ( DWORD TimerInterval ) 
{
  lTIMERTICS =0;
   
     // Enable TIM0 clocks
  PCONP_bit.PCTIM0 = 1; // enable clock
  // Init Time0
  T0TCR_bit.CE = 0;     // counting  disable
  T0TCR_bit.CR = 1;     // set reset
  T0TCR_bit.CR = 0;     // release reset
  T0CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  T0MCR_bit.MR0I = 1;   // Enable Interrupt on MR0
  T0MCR_bit.MR0R = 1;   // Enable reset on MR0
  T0MCR_bit.MR0S = 0;   // Disable stop on MR0
  // set timer 0 period
  T0PR = 18-1;
  T0MR0 = (TIMER0_IN_FREQ)/(18 * TIMER0_TICK_PER_SEC);
  // init timer 0 interrupt
  T0IR_bit.MR0INT = 1;  // clear pending interrupt
  T0TCR_bit.CE = 1;     // counting Enable
  /*Enable NVIC TMR0 Interrupt*/
  NVIC_IntEnable(NVIC_TIMER0);
   
  return (TRUE); 
  }
// 

DWORD init_timer1 ( DWORD TimerInterval ) 
{
    
  dTIMERTICS =0;
    
       // Enable TIM0 clocks
  PCONP_bit.PCTIM1 = 1; // enable clock

  // Init Time0
  T1TCR_bit.CE = 0;     // counting  disable
  T1TCR_bit.CR = 1;     // set reset
  T1TCR_bit.CR = 0;     // release reset
  T1CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  T1MCR_bit.MR0I = 1;   // Enable Interrupt on MR0
  T1MCR_bit.MR0R = 1;   // Enable reset on MR0
  T1MCR_bit.MR0S = 0;   // Disable stop on MR0
  // set timer 0 period
  T1PR = 18-1;
  T1MR0 = (TIMER1_IN_FREQ)/(18 * TIMER1_TICK_PER_SEC);
  // init timer 0 interrupt
  T1IR_bit.MR0INT = 1;  // clear pending interrupt
  T1TCR_bit.CE = 1;     // counting Enable
  /*Enable NVIC TMR0 Interrupt*/
  NVIC_IntEnable(NVIC_TIMER1);
    
   return (TRUE); 
   }
// 
/******************************************************************************
**                            End Of File
******************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////
//  This is the timer functions
//  Maintains all the timers used in the program
//  The timer works off the globle lTIMERTICS counting at 25Hz
//  The timer variable selects which timer to access as defined in the timer.h
//  the mode variable selects the operation, whether to test for timeout, or restart the timer
//////////////////////////////////////////////////////////////////////////////////////
char timer(char timer,char mode)
{
long ltime;
long diff;
  // 
  if (mode==1) // start the timer
  {
  lasttimeA[timer]=lTIMERTICS;
  return(0);
  }
  
  ltime=lasttimeA[timer];
  diff = labs(lTIMERTICS-ltime); // calculate the difference in time
  
  if ( diff > lTIMEVALUE[timer]) return(1);
  else return(0);
} // end of function timer()





//////////////////////////////////////////////////////////////////////////////////////
//  This is the comport time out delay timer function
//  This timer is variable and can be set between 1 to 20 seconds in calibration
//  This timer is used for delay before entering to dash if the commmunication is lost
//////////////////////////////////////////////////////////////////////////////////////
char com_timeout_timer(char mode)
{
long ltime;
long diff;
//

  if (mode==1) // start the timer 
   {
   lasttimeB[2]=lTIMERTICS;
   return(0); // 
   } // 
   
 ltime = lasttimeB[2]; // get the last time
 diff  = labs(lTIMERTICS-ltime); // calculate the difference in time
 
 
 
 if ( diff > 1 * 250 )  // if the difference is greater than the timer
   {
 return(1);
 } // 
 
 return(0);
} // end of function display_timer()

// 
//////////////////////////////////////////////////////////////////////////////////////
//  This is the comport time out delay timer function
//  This timer is variable and can be set between 1 to 20 seconds in calibration
//  This timer is used for delay before entering to dash if the commmunication is lost
//////////////////////////////////////////////////////////////////////////////////////
char command_timeout_timer(char mode)
{
long ltime;
long diff;
//
 if (mode==1) // start the timer 
 {
 lasttimeB[3] = lTIMERTICS;
 return(0); // 
 } // 
 ltime = lasttimeB[3]; // get the last time
 diff = labs(lTIMERTICS-ltime); // calculate the difference in time
 

 if ( diff > 1 * 250)  // if the difference is greater than the timer
  {
 return(1);
 }
 
 
 return(0);
 // 
 
} // end of function display_timer()

char refresh_timer(char mode)
{
                       // 25 ticks a second
long ltime;
long diff;
char reftime;

 if (mode==1) // start the timer 
 {
 lasttimeB[4]=lTIMERTICS;
 return(0); // 
 } // 
  ltime=lasttimeB[4]; // get the last time
  diff = labs(lTIMERTICS-ltime); // calculate the difference in time

  reftime =  CAL.DISPLAY_RATE * 6; //  25 tics per second
  
 if ( diff > reftime)  // if the difference is greater than the timer
 {
 return(1);
 }
 

return(0);
} // end of function display_timer()

//------------------------------------------------------------------------------------------
//  motion_timer()
//  Motion timer is a user programmable timer used to time motion delays
//  
//
//------------------------------------------------------------------------------------------
char motion_timer(char mode)
{
//static long lasttime;      // holds the last time value
                          // 25 ticks a second

char mottime;

long ltime;
long diff;


 if (mode==1) // start the timer
 {
 lasttimeB[6]=lTIMERTICS;  // get the globle timer 25 tics/sec
 return(0);
 }

  ltime=lasttimeB[6]; // get the last time
  diff = labs(lTIMERTICS-ltime); // calculate the difference in time

 mottime = CAL.MOTION_TIME * 6; //  25 tics per second

 if ( diff > mottime )  // if the difference is greater than the motion timer
 {
 return(1);
 }
 else
   return(0);

} // end of function motion_timer()


char message_timer(char mode)
{

long ltime;
long diff;
//
 if (mode==1) // start the timer 
 {
  lasttimeB[5]=lTIMERTICS;
 return(0); // 
 } // 
  ltime= lasttimeB[5]; // get the last time
  diff = labs(lTIMERTICS-ltime); // calculate the difference in time
  
if ( diff > 1 * 250 )  // 1500 if the difference is greater than the timer
 {
 return(1);
 }
 else return(0);
} // end of function display_timer()


char peak_timer(char mode)
{
//static long lasttime;      // holds the last time value
                          // 25 ticks a second

char peaktime;

long ltime;
long diff;


 if (mode==1) // start the timer
 {
 lasttimeB[8]=lTIMERTICS;  // get the globle timer 25 tics/sec
 return(0);
 }

  ltime=lasttimeB[8]; // get the last time
  diff = labs(lTIMERTICS-ltime); // calculate the difference in time

 peaktime= CAL.PEAKDELAY * 6; //  25 tics per second

 if ( diff > peaktime )  // if the difference is greater than the motion timer
 {
 return(1);
 }
 else
   return(0);

} // end of function peak_timer()


//------------------------------------------------------------------------------------------
//  track_timer()
//  zero tracking timer
//
//
//------------------------------------------------------------------------------------------
char track_timer(char mode)
{
//static long lasttime;      // holds the last time value
                          // 25 ticks a second

long ltime;
long diff;


 if (mode==1) // start the timer
 {
 lasttimeB[7]=lTIMERTICS;
 return(0);
 }

  ltime=lasttimeB[7]; // get the last time
  diff = labs(lTIMERTICS-ltime); // calculate the difference in time

 if ( diff > 6)  // if the difference is greater that 0.25sec (25 ticks a second)
 {
 return(1);
 }
 else
   return(0);

} // end of function motion_timer()

// 
//
/*************************************************************************
 * Function Name: TMR0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void TMR0_IRQHandler (void)
{
  /* Toggle LED1 */
  lTIMERTICS++; 
  /*clear interrupt */
  T0IR_bit.MR0INT = 1;
  /**/    
  NVIC_ClrPend(NVIC_TIMER0);
   __enable_interrupt(); 
} // 

//  
/*************************************************************************
 * Function Name: TMR0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void TMR1_IRQHandler (void)
{
  /* Toggle LED1 */
  dTIMERTICS++; 
  /*clear interrupt */
  T1IR_bit.MR0INT = 1;
  /**/    
  NVIC_ClrPend(NVIC_TIMER1);
   __enable_interrupt(); 
} //  
/******************************************************************************
************************** End Of File ****************************************
******************************************************************************/