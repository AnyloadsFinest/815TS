
#include "includes.h"

//version history for this project
char message[]= "v21-25";   // version number is entered here
char model[]= "815-ts";   // version number is entered here
char hello[]= " HELLO";   // version number is entered here
// spi fixed on Sep 29 202
//  version number below without decimals used to display version number in software

//  version number below without decimals used to display version number in software
void display_version(void)
{ 
  char i;
  char t;
  char time = 2;  // how long to display version number

  for (i=0;i!=6;i++)  // copy version number to display
  {
    DISPLAY.DIGITS[i]= model[i];
  }
 if( CAL.BUZZ_EN == 1)
                   {
                   FIO3CLR = 0x02000000; // disable output 
                   }
  t=0;
  timer(MESSAGE,START);
  
  while (1)  // loop and wait for timeout
  {
       
     output_display(0); // update seven segment display
 
    if (timer(MESSAGE,TIMEOUT))
    {
      FIO3SET = 0x02000000; // disable output
      timer(MESSAGE,START);
      t++;
      if (t == time) break;  // break out of while loop
    }

        // start loading FIR filter with sample to prep startup
       scan_AD();        // read AD converter value and filter
       get_weight();     // get AD values and calculate weights


  } // end of while

    for (i=0;i!=6;i++)  // copy version number to display
  {
    DISPLAY.DIGITS[i]= message[i];
  }
 if( CAL.BUZZ_EN == 1)
                   {
                   FIO3CLR = 0x02000000; // disable output 
                   }
  t=0;
  timer(MESSAGE,START);
  
  while (1)  // loop and wait for timeout
  {
       
     output_display(0); // update seven segment display
 
    if (timer(MESSAGE,TIMEOUT))
    {
      FIO3SET = 0x02000000; // disable output
      timer(MESSAGE,START);
      t++;
      if (t == time) break;  // break out of while loop
    }
        // start loading FIR filter with sample to prep startup
       scan_AD();        // read AD converter value and filter
       get_weight();     // get AD values and calculate weights


  } // end of while


  FIO3SET = 0x02000000; // buzer off
  FIO0SET = 0x00000030; // status leds off
  FIO1CLR = 0x00030000; // relays off  

}// end of function


  //-------------------------------------------------------------------------------------
void warmup_test(void)
{ 

  char i;
  char t;
  char time = 2;  // how long to display version number
  
  for (i=0;i!=6;i++)  // copy version number to display
  {
    DISPLAY.DIGITS[i]= hello[i];
  }

  t=0;
  timer(MESSAGE,START);
  
  while (1)  // loop and wait for timeout
  {
     update_status(1); 
     output_display(0); // update seven segment display
 
    if (timer(MESSAGE,TIMEOUT))
    {
      
      timer(MESSAGE,START);
      t++;
      if (t == time) break;  // break out of while loop
    }

              // start loading FIR filter with sample to prep startup
       scan_AD();        // read AD converter value and filter
       get_weight();     // get AD values and calculate weights


  } // end of while
  

  } 







