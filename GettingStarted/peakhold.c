
#include "includes.h"

extern long lTIMERTICS;
//long lasttime;      // holds the last time value
//extern long lasttimeB[];  // holds the last time value for all the timers


static  float LAST_PEAK_WEIGHT=0;  // holds the last known peak weight
bool  PEAK_THRESH;         // true if peak treshold has been met


//--------------------------------------------------------------------------
// weight is  AD*Span value, which is returned.
// if a peak hold is detected, the return value if frozen.
// this function is called from weight.c module
//--------------------------------------------------------------------------
float process_peakhold(float weight)
{
  
static int state=0;  // state machine for peak detect
float threshold = CAL.PEAKTHRESH/CAL.GRAD;


 // is peak detect enabled ?
  if (CAL.PEAK_EN == 0)    // check if peak is disabled
   {//1
     return(weight);   // peakhold not enabled, return live weight
   }//1 

// in calibration mode we disable peak detect 

  if (SYSTEM.CAL==1)  //are we cal mode
  {
    return(weight);   // no peakhold in calibration, return live weight
  }   
  
  if (CAL.PEAK_EN == 2)  // peak hold with motion
  {//4
             if (SCALE.MOTION==1)  // is there motion
             {//1
               
                       if (state==2)  // return peak weight if we are already locked on a weight
                       {//2
                          return(LAST_PEAK_WEIGHT);
                         
                       }//2
                       else // return free running weight
                       {//3
                         return(weight);  // there is motion, return live weight
                       }//3
             }//1
    
  }//4
  
 //   
 // state machine that runs to detect peak detect
switch (state)
{

        case 0: {   // wait for the weight to exceed peak detect threshold, return free running weight
               
                   if (weight > threshold)  // evaluate peak threshold 
                   {
                    LAST_PEAK_WEIGHT=0; // initialize
                    peak_timer(START) ; // start the peak detect delay timer
                    state=1;            // goto to state 1, wait for timer to timeout      
                   } 
                   
                 // 
                 SCALE.PEAKCLEAR=0; // clear flag set by CLEAR key
                 SCALE.PEAK = 0;    // clear peak detect flag
                 return(weight);    // return free runing weight
                }          
                
                                
       case 1: {  // wait for peakhold delay timer to timeout
                  // if a peak delay timer has been enabled, wait for the timer to time out before processing peak detect
         
                  if (peak_timer(TIMEOUT))  // weight for the timer to timeout
                  {
                    LAST_PEAK_WEIGHT=weight; // get new peak weight
                    state=2;                 // goto state 2 and lock peak weight
                  }
                  
                 SCALE.PEAK = 0;     // clear peak detect flag
                 SCALE.PEAKCLEAR=0; // clear flag set by CLEAR key
                 return(weight);    // return free runing weight
               }
                
               
      case 2:  { //  lock weight , and monitor for new peaks
  
                 if (weight > LAST_PEAK_WEIGHT)  // check if have a new peak value
                 {//4
                   LAST_PEAK_WEIGHT = weight;  // update latest peak weight
                   
                   SCALE.PEAK = 1; // set display status
                 }//4
  
                 
                 if (CAL.PEAKRESET==1) // if auto peak reset has been enabled
                 {//5    check if we have to reset the peak detect
                      if (weight < threshold)  // evaluate weight <peak threshold 
                        {//3
                           state=0;   // when weight has been removed from the scale below the thresh, reset statemachine, clear peak detect
                        }//3
       
                  }//5
                 
                 
                 if (SCALE.PEAKCLEAR==1)  // check if somebody cleared the peak detect with the clear key
                 {
                   
                  state =3 ;  // CLEAR pressed, goto state 3 
                 }
                 
               return(LAST_PEAK_WEIGHT);  // we are returning the locked weight to the indicator
               }
     
        
               
      case 3:  {  // CLEAR key is pressed, return to weight mode, and wait to rearm peak hold
  
                  SCALE.PEAKCLEAR=0; // clear flag set by CLEAR key
                  SCALE.PEAK = 0; // clear display status
                  
                  // 
                  if (weight < threshold)  // evaluate weight <peak threshold 
                    {//3
                       state=0;   
                    }//3
                //                 
                return(weight);  // we are now returning free runing weight
               // 
               }                    
                 
       default:  {
                state=0;   // safety
                break;
                 }
               
   } //end of switch               

return(weight);  // should never reach
  
} // end of function
//
//------------------------------------------------------------------------------------
//
void clear_peak_hold(void)
{  
 LAST_PEAK_WEIGHT=0;  // clear the last peak weight
 PEAK_THRESH=false;   // clear peak thresh flag             
       
               
} // end of function

//------------------------------------------------------------------------------------------
//  peak detect timer
//  Motion timer is a user programmable timer used to time motion delays
//  
//
//------------------------------------------------------------------------------------------
/*

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

 peaktime=CAL.PEAKDELAY*4; //  25 tics per second

 if ( diff > peaktime )  // if the difference is greater than the motion timer
 {
 return(1);
 }
 else
   return(0);

} // end of function peak_timer()


*/

