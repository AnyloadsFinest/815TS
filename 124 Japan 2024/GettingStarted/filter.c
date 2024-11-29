
#include "includes.h"


#define HISTORY 5

//------------------------------------------------------------------------
// scan the ad converter for a new conversion update
//------------------------------------------------------------------------

void scan_AD(void)
{
signed long ad;

 ad = readAD(); // call the hardware related code to read the ad converter

if ( ad != 0  ) // the likely hood of the ad converter being at zero is very rare
 { 
  SCALE.AD = AD_average(ad);  // filter the AD value  
//  return; 
 }


 }  // end of function



//--AD_average()----------------------------------------------------------
// The AD converter is averaged over several samples
// Depending on the amount of vibration the length of the filter can be
// set between 1 and 255.
// Take the current sample and add it to the averaging register.
// Return a new AD value that is averaged over c number of samples
//------------------------------------------------------------------------

signed long AD_average(signed long newADvalue)
{

long sum=0;          // sum of all the samples in the average register
long avge=0;         // average value of all samples in the average register
int i;               // holds position in the average register array


float live_weight; // temperary weight calculations used for faststep
float history_weight;

static long avreg[255];  // averaging register upto 250 samples
static long history[30]; // shift register for looking at 20 samples ago
static int avsmp=0;      // length of averaging register
static int threshold;    // threshold counter





// check if we have to reinitialize the filter registers
// initialization occurs when avsmp=0
if (avsmp==0) // if the current filter setting in avlen does match the calibration setting
 {
    avsmp=CAL.FILTER; // set avsmp equal to calibration value

    for (i=0;i!=255;i++)
     {
       avreg[i]=newADvalue;  // preload the average registers with AD samples
     } // end of for loop



   for (i=0;i!=HISTORY;i++)   //5
   {
     history[i]=newADvalue;
   }

    threshold=0; // reset fastep threshold count to zero

 } // end of if




//************************************************************************************
//  Filtering is done through a simple moving average filter using a FIFO.
//  Load the average register with anew ADvalue.
//  The length of the average register is set in calibration as a filter parameter.
//  The length of the averaging register is held in the variable avsmp. It holds the number of samples to be averaged.
//  The length of the averaging register can be set in calibration from 1 to 250.
//*************************************************************************************




// load samples to the main sample register, averaging length is determined by avsmp
 for (i=(avsmp-1);i!=0;i--)  // shift in new data into averaging register
  {//1
   avreg[i]=avreg[i-1];  // left shift average table
   sum=sum+avreg[i];
  }//1

  avreg[0]=newADvalue;       // load AD new value into table




 // loop through and calculate the average of the averaging register
 sum=0;
 for (i=0;i!=avsmp;i++)
 {//2
 sum=avreg[i]+sum; // sum all the samples in the averaging register
 }//2


 avge=sum/avsmp;    // avge now holds the average AD value


for (i=HISTORY;i!=-1;i--) history[i]=history[i-1]; // shift the array one to the right
history[0]=newADvalue; // load a new ADvalue into the history array


history_weight =  ((history[HISTORY-1] - SCALE.ZERO) * CAL.SPAN);  // history weight with no filtering
live_weight  =  ((newADvalue - SCALE.ZERO) * CAL.SPAN);  // weight with no filtering

  if (fabs(live_weight-history_weight) > CAL.FILTERTHRESH) //weight step is greater than filter threshold in grads
   {
    threshold++; // increment the threshold counter
   }
      else
      {
	threshold=0;  //reset the threshold count
      }



if (CAL.FASTSTEP) // if fastep is disabled then do not preform fastep operation
{//3
  if (threshold > CAL.FILTERSENS)  // if the threshold exceeds the set value
  {//2
    avsmp=0; // set the average legnth to one, we are now in fastep mode
  }//2
} //3


  if (avsmp<CAL.FILTER)  // if the register lengths are not equal
  {
    avsmp++;  // increment the legnth of the primary averaging reg by one
  }




return(avge); // return the average filtered AD value


} // end of function




//--------------------------------------------------------------------------------------------------
// average  samples and return value
// used for deadload and span calibration where we need controlled stable result
//--------------------------------------------------------------------------------------------------

signed long CAL_ADaverage(void)
{

long ad;
signed long count;
long totalize;
long advalue;
char finished=0;
char string[6]= " CAL 5";
  

for (char i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string[i];
          }
          

count=0;
totalize=0;


timer (SAMPLE_DELAY,START); // start the timer

 while (1)  // wait for the scale to settle, before accepting samples
 {
  if (timer (SAMPLE_DELAY,TIMEOUT))
  {
    break;  // break out of loop
  }

 ad=readAD(); // dummy read ad converter
 //countdown(0,0);  // show countdown
 for (char i=0;i!=6;i++)
          {
          DISPLAY.DIGITS[i]=string[i];
          }
 update_status(1); // blank status display
 output_display(0);
 }

 countdown(1, 5);  // init start countdown

 while(1)  // endless loop
 {
 countdown(0, 5);  // init start countdown
   
 ad=readAD();
 
 if (ad!=0) // a non zero means we have a valid AD value
  {
    totalize = totalize + ad;  // sum samples
    count++;
  }

 if (count == 80)  // have we sampled 40 samples?
  {
  advalue=totalize / 80;  // get average of 80 samples
  finished=1; //finished averaging
  }

if (finished)  // check if countdown is finished
 {
  if (countdown(0,0))  // has count down timed out
  {
    break;  // break out of loop
  }
 }
 
 } // end of while

return(advalue);  // return sum

} // end of function


//-------------------------------------------------------------------------------------------------------
// filter preset initialize
// sets up filter parameters when called from calibration
//-------------------------------------------------------------------------------------------------------
void filter_preset(char preset)
{

  // 1=0.5hz
  // 2=1hz
  // 3=2hz
  // 4=3hz

  switch(preset)
  {

  case 1:  { // light
                 CAL.FILTER=10;           //number of samples used in the moving average register
                 CAL.FILTERTHRESH=2;      //weight change required to trip fastep
                 CAL.FILTERSENS=5;        //how many samples in a row there is a weight change to trigger fastep
                 CAL.FIR=4;               //FIR filter 3hz
             break;
           }

  case 2:  {//medium
                 CAL.FILTER=50;           //number of samples used in the moving average register
                 CAL.FILTERTHRESH=4;      //weight change required to trip fastep
                 CAL.FILTERSENS=5;        //how many samples in a row there is a weight change to trigger fastep
                 CAL.FIR=4;               //FIR filter 2hz

             break;
           }

  case 3:  {//heavy
                 CAL.FILTER=50;            //number of samples used in the moving average register
                 CAL.FILTERTHRESH=4;      //weight change required to trip fastep
                 CAL.FILTERSENS=5;        //how many samples in a row there is a weight change to trigger fastepnge to trigger fastep
                 CAL.FIR=3;               //FIR filter 1hz
             break;
           }


  case 4:  {//heavy 1
                 CAL.FILTER=100;           //number of samples used in the moving average register
                 CAL.FILTERTHRESH=8;      //weight change required to trip fastep
                 CAL.FILTERSENS=10;        //how many samples in a row there is a weight change to trigger fastep
                 CAL.FIR=2;               //FIR filter 1hz
             break;
           }

  case 5:  {//animal 2
                 CAL.FILTER=150;           //number of samples used in the moving average register
                 CAL.FILTERTHRESH=8;      //weight change required to trip fastep
                 CAL.FILTERSENS=10;        //how many samples in a row there is a weight change to trigger fastep
                 CAL.FIR=1;               //FIR filter 0.5hz
             break;
           }

  

  }// end of switch



}  // end of function





//-------------------------------------------------------------------------------------------
// Every time called counts down display by 1
//
// Used for deadload and span calibrating for user feedback that we are calibrating
// mode=1 set countdown   mode = 0 couny down in 1 sec increments
// return 1 if count down completed
//-------------------------------------------------------------------------------------------

char countdown(char mode, char value)
{

static int cdown;

  if (mode)
  {//1
   cdown=value;
   timer(SECOND,START);  // start 1 second timer
  }//1


    if (timer(SECOND,TIMEOUT))  // check 1 second timer
    {//3
      cdown--;
      timer(SECOND,START);  // restart 1 second timer
    }//3

      if (cdown<1) // are we finished counting down
      {//4
       return(1);
      }//4


displayvalue(cdown, 0, 0,"");
output_display(0); // update seven segment display
return(0);

} // end of function


