#include "includes.h"
long lasttimeMotion;      // holds the last time value
                          // 25 ticks a second

extern long lTIMERTICS;
//-motion()---------------------------------------------------------------
//Look at the current weight in grads and compare it to a past sampled weight.
//If the two weights exceeds the threshold specified in cal.motion, this activates
//the motion flag for a period of time.
//
//Setting the motion flag also sets the motion timer. The motion timer keeps the
//motion flag active for a periode of time after motion has ceased.
//
//------------------------------------------------------------------------

void motion(void)
{
long currentweight; // current value in grads

static long lastweight;


   if (CAL.MOTION==0) 
     
   {
      SCALE.MOTION=0;  // if the motion was set, and has timed out, clear motion flag
     return; // check if motion is disabled
   }

   currentweight=SCALE.GRADWEIGHT;  // get the latest gradweight value

    // evaluate below the step change from the last weight to the new weight
   if (labs(currentweight-lastweight) >= CAL.MOTION) // check if there has been a change in weight from 3 samples ago
  	     {//1 there is a change in weight, motion detected
		SCALE.MOTION=1; // set motion flag
		motion_timer(START); // start motion timer duration
    	     }//1
 	     else
  	     {//2 no motion detected

		if ( motion_timer(TIMEOUT) ) // check if motion on duration has timed out
		{//3
		  SCALE.MOTION=0;  // if the motion was set, and has timed out, clear motion flag
		}//3

	      }//2


            // We sample new weights on timer intervals
            // If the filter system is heavy, and we add a small amount of weight on the scale,
            // the weight would increase slowly, but motion may never be detected.
            // We sample weight on a timer interval to make motion more sensitive to slow changing weight.


            if (timer(MOTION_SAMPLE,TIMEOUT))
            {//4
	         timer(MOTION_SAMPLE,START);  // start the timer
		 lastweight=SCALE.GRADWEIGHT; // refresh lastweight ch1
            }//4


  
            
            

} // end of function motion






/*

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
 lasttimeMotion=lTIMERTICS;  // get the globle timer 25 tics/sec
 return(0);
 }

  ltime=lasttimeMotion; // get the last time
  diff = labs(lTIMERTICS-ltime); // calculate the difference in time

 mottime = CAL.MOTION_TIME*4; //  25 tics per second

 if ( diff > mottime )  // if the difference is greater than the motion timer
 {
 return(1);
 }
 else
   return(0);

} // end of function motion_timer()

*/