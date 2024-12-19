
#include "includes.h"

extern long lTIMERTICS;

void zero_tracking(void)
{

static char tracking; // zero track flag to indicate that we are performing zero tracking
long zeroposition;
static long lastAD; // holds the last ad value, used to compare against to detect slow moving weight

SCALE.ZEROTRK=set_trkval();

// how far off is the AD converter from the last zero position
zeroposition = labs(SCALE.AD-SCALE.ZERO); //get the zero position of the AD converter

if (CAL.ZEROTRACK) // if the zero tracking is enabled
{//2
 if (!SCALE.MOTION )  //if there is no weighing MOTION then zero track
 {//1

        // check if we are in the zero tracking range
	if ( zeroposition < SCALE.ZEROTRK ) // if rawcounts are less than zero track max range
	 { // 2
                        // the tracking flag is used to identify that a tracking operation is in progress
			if (tracking) // check if we are currently trying to track
			{ //3   if we are tracking, then we are waiting for a timer to timeout to make sure don't have a slow moving scale
				  if (track_timer(TIMEOUT)) // check if the track value is stable for 0.25 sec
				   { //4
 	                        	if ( labs(lastAD-SCALE.AD) < SCALE.TRACKSENS ) // if the last AD value is still withing tracking range
                         		zero_scale(); // zero scale quietly
                        		tracking=0; // reset tracking flag, finished tracking
				    } //4 end of if (tracktimer)
			} //3 end of if (tracking)

			else // this is the first time we detected a zero track condition

			{//5
				tracking=1; // set the tracking flag
				track_timer(START); // start the zero track timer for 1/4 second
				lastAD=SCALE.AD; // capture the current AD value
			}//5

	 } //2

	 else
         {  //6
           tracking=0;  // reset the tracking flag
         }  //6




 } //1  end of if (!scalemotion)


 else // scalemotion detected abort any zero tracking
 {
 tracking=0;
 }

}//2

// now check is we are at center of zero																		


	 if (zeroposition < SCALE.CENTEROFZERO) // check if we are <= of 25% of a graduation close to zero
	 SCALE.ATZERO=1; // set the at zero flag
	 else
	 {
	 SCALE.ATZERO=0;  // else not at zero, clear the at zero flag
	 }

} // end of function zero_tracking




long  calc_zerorange(void)
{
long zerorange;
long grad;

grad = CAL.GRAD;

if (CAL.DEADZERO>0)  // check if we have dead zeros i.e. 10,20,50,100
{  
  if (CAL.DEADZERO==1)
  {
   grad = grad * 10;  // multiply by deadzero
  }
  else
  {
      if (CAL.DEADZERO==2)
      {
       grad = grad * 100;  // multiply by deadzero
      }
  }
  
  
}


zerorange= (long) (( CAL.ZERORANGE * 0.01) * (CAL.SCALECAP / grad));
return(zerorange);

} // end of funtion calc_zerorange()


//--calc_centerofzero------------------------------------------------------
//
// By definition zero id defined to be +/- 25% of 1 graduation to the center of zero.
// The rawcounts value needs to be calculated at startup to indicate a center of zero.
//
//-----------------------------------------------------------------------------
long calc_centerofzero(void)
{

long zerocenter;
float gradcount;

gradcount= (1 / CAL.SPAN);
zerocenter = (long)(0.25*gradcount); //calculate 25% of 1 graduation in rawcounts

return (zerocenter);

} // end of funtion



//------------------------------------------------------------------------------------------
//  track_timer()
//  zero tracking timer
//
//
//------------------------------------------------------------------------------------------
/*
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
*/





long  set_trkval(void)
 {

 long ADcount;
 int track;

 track=CAL.ZEROTRACK;
 ADcount= (long) (0.01*(track)*(1/CAL.SPAN)); // zero track in raw counts
 SCALE.TRACKSENS=(long)(0.3*ADcount); // set the senstivity to 0.3 the zero track value
 return(ADcount);

 } // end of function






//--zero_scale---------------------------------------------------------------
//  This function is linked to the zero button on the indicator.
//  When the zero button is pressed, and the weight value is within the allowable
//  zero range, then zero the scale.
//
//---------------------------------------------------------------------------

char  zero_scale(void)
{
float absweight; // absolute weight with reference to deadload zero
//long absweight; // absolute weight with reference to deadload zero

/*
if(CAL.REGULA == 3)  // tare the scale depending on which regulatory setting is set
  {
    if (SCALE.NETMODE==1) // check if cleared
               {
                return(0); 
               }
  }
*/

	 if (!SCALE.MOTION) // if there is no motion
	 {//1

		 absweight= fabs( (SCALE.AD-CAL.DEADLOAD)*CAL.SPAN ); // calculate weight with ref to deadload
                            
		      if (absweight < SCALE.ZERORANGE) // abs weight must be below zerorange to zero
			{//2
			 SCALE.ZERO = SCALE.AD; // then zero scale
                         return(1);  // return that zero was successful
			}//2
                        else
                        {
                          return(2);  // return outside of zero range
                         }
		
	}//1
	         
  return(0); // not successful zeroing
         
} // end of funtion zero_scale();


//--------------------------------------------------------------------------------------------------
// power up zero task  IZSM

//--------------------------------------------------------------------------------------------------

void power_up_izsm(void)
{
  
 static char IZSM=0;  // statemachine that keep track of izsm 
  
 // check if we have to zero the scale after 100 samples on power up
  
  if (SYSTEM.CAL)  // we are not in calibration mode
      {//3
       SCALE.PWRZERO=0; 
       SCALE.IZSM_ERROR=0;  // set flag to indicate that we could not zero 
      }//3
  
 
 
      if (SCALE.PWRZERO==0)  // check if power zero flag is not set
      {
       return;  // no power up zero to be performed 
      }
  
     
        switch(IZSM)
         {//4
          
         case 0:  {// wait for 100 AD samples after power up
                   if ( SCALE.AD_COUNT > 100 )  // check if we sampled enough ad samples for a stable condition to zero the scale
                   {                           // PWRZERO is incremented on every AD coverter read in ads1232.c
                    IZSM=1;  // go to next state
                    timer(IZSM_START,START);  // start the timeout timer,START);  // start the timeout timer
                    timer(IZSM_TIMEOUT,START);  // start the timeout timer,START);  // start the timeout timer
                   }
                   break;
                  }
           
          case 1: { // wait for no motion, try and zero scale
                    if ( SCALE.MOTION==0)  // if no scale motion
                     {       
                     
                      IZSM=2;  // go to next state                    
                     }
                    
                   if(timer( IZSM_START, TIMEOUT))
                   {
                   IZSM=2;  // go to next state                     
                   }                                          
                  break;
                  }
                  
                  
         case 2:  {  // check if we zeroed succesfully
                   if ( zero_scale() == 1 )  // check if zero           
                    { 
                     display_message( "------",1);     
                    SCALE.IZSM_ERROR=0;  // clear flag  
                    IZSM=4;
                    }
                  
                  if(timer( IZSM_TIMEOUT , TIMEOUT))
                   {
                     SCALE.IZSM_ERROR=1;  // set flag to indicate that we could not zero 
                     IZSM=3;  // go to next state
                    }                  
           
                  break;
                  }     
                  
                  
            case 3:  {  // stay here until scale is zeroed manually
           
                     if (SCALE.DIS_WEIGHT==0)  // check if zero           
                     {   
                      SCALE.IZSM_ERROR=0;  // clear flag
                     
                     }
                 
                  IZSM=4;
                  break;
                  }            
                
                  
         case 4:   { // clear IZSM condition
                 //  SCALE.IZSM_ERROR=0;  // clear flag 
                   SCALE.PWRZERO=0;  // clear power up zero flag
                   break; 
                   
                   }
                  
          
          default:   { // clear IZSM condition
                 IZSM=0;
                   SCALE.PWRZERO=0;  // clear power up zero flag
                   break;  
                   
                   }           
                   
                   
         }//4
      
    
        
        
        
  
} // end of function power up zero    




//-------------------------------------------------------------------------------------
// pushbutton zero task
// on a sensitivr base, pressing the zero can cause a motion to occur, prevesnting
// this state machine waits for the scale to zero before zeroing
//-------------------------------------------------------------------------------------
void pushbutton_zero(void)
{
  
static char ZSM=0;

  switch (ZSM)
  {
    
  case 0:  {   // idle waiting for the zero button press
            
  
            if (SCALE.ZEROSCALE==1) // check if flag is set
           // if ((SCALE.ZEROSCALE==1) && ( SCALE.MOTION==0 )) // check if flag is set
            {
              SCALE.AD_COUNT=0;  // zero the free running AD counter
              ZSM=1;  // goto to the next state
            }  
            
            break;
            }
        
    
  case 1:   {  // try to zero the scale
    
             if(CAL.REGULA == 3)  // tare the scale depending on which regulatory setting is set
              {
                if (SCALE.NETMODE) // check if cleared
                           {                             
                            ZSM=2;
                            break;
                           }
              }
  
             if (!SCALE.MOTION) // check if flag is set
            {         
              
              if (zero_scale()==1)  // was zero successfull?
              {
               storezero(); // save the zero to non volatile memory 
               ZSM=2; 
              }
              else
               if (zero_scale()==2)  // out of push button zero range
               {               

                while(1){
                display_message( " ERROR",0);  // error outside of zero range   
                };
                // ZSM=2; 
               }              
            }
                              
           //   if (SCALE.AD_COUNT>25)  // timeout after 25 AD samples
             if (SCALE.AD_COUNT> 100)  // timeout after 25 AD samples
              {
               ZSM=2;               
              }
  
  
              break;
            }
            
            
            
  case 2:   {
              
              SCALE.ZEROSCALE=0; // clear flag;
              ZSM=0;  // reset state machine
              break;
            }
    
    default:   {
              
              SCALE.ZEROSCALE=0; // clear flag;
              ZSM=0;  // reset state machine
              break;
            }         
            
  } // end of switch


//   
  
  
  
  
} // end of funtion


//-----------------------------------------------


