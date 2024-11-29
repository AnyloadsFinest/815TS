
#include "includes.h"

extern long lTIMERTICS;
//long lasttime;      // holds the last time value
//extern long lasttimeB[];  // holds the last time value for all the timers
//--zero_tracking()--------------------------------------------------------
// this is the main zero tracking rutine that is called ever weight cycle
//
// Zero tracking first checks is there is any motion. Zero tracking can
// only be performed on a stable scale.
//
// Depending if we are tracking whole grads or fractional grads we then
// compare the weight/AD value to see if it should be tracked off or not.
// If we are doing fraction zero tracking then the scale must be at zero
// we  zero track is evaluated.
//
// Values for zero tracking are taken from the global structure "scale" and
// "cal".
//
//
// This function modiifies the "scale.zero" value used for calculating weight.
//
//
//  cal[ch].zerotrack holds the zero track info and can have the following values:
//
//    0-99 which is a % of a scale graduation that is to be tracked
//    or the values 100,200,300 which means tracking full grads, 1,2,3.
//
//
//



//--calc_zerorange-----------------------------------------------------------
// Calculate the actual zero range you are allowed to zero the scale.
// Must be called on startup, and on changes to the zerorange and scale capacity
// parameters.
// Uses the value from cal.scalecap to calculate the zero range.
// Legal for trade scale do not allow you to zero more than 2% of scale capacity.
//---------------------------------------------------------------------------

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


//--set_trkval---------------------------------------------------------------
// based on the tracking value passed from the zero tracking menu we
// calculate the values used for zero tracking.
//
// There are two parameters used for zero tracking:
// "scale.ADzerotrk" holds fractional/grad values to track
// "cal.zerotrack"   holds zero track selection value
//
// "cal.zerotrack" is permantly stored in calibaration
// "scale.ADzerotrack" is dynamic and calculated at runtime
//
// if the value of track (passed from the menu) is between 1 and 10
// then we are dealing with fraction grad sizes and we must calulate
// the rawcounts to track against. How many rawcounts per graduation is
// determined from the span value. When we know the rawcount per grad then
// we take a fraction of that, so for zerotrack =0.2DD and 1 grad=100 counts
// then we will have 20 rawcounts as our zerotrack value.
//
//
// This function must be called on startup, or when zero tracking or span values changes.
//
//


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
                display_message( "--E1--",2);  // error outside of zero range               
                ZSM=2; 
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


