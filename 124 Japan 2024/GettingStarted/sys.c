
#include "includes.h"
extern long lTIMERTICS;


//extern struct menu;
//---------------------------------------------------------------
// update the 7 segment display display
//---------------------------------------------------------------

void update_display(void)
{
//const static char string[] = "--EE--";
static char flag;
char i;
long d;
char neg=0;
float dd;
float fraction;

  switch (SYSTEM.DISPLAY_MODE)
  { // 1

   case  _WEIGHT:   { //----------------------  display mode is weight ------------------------  
                          
                       if (!SYSTEM.CAL) // if we are not in calibration mode
                        {//12         
                                             
                       if ( SCALE.DIS_WEIGHT > 999999)  // check for neagtive
                        { 
                        
                            display_message( "--E8--",0);  
                            update_status(1); // blank status display
                            output_display(0); // update seven segment display
                            break; // break out of case statement
                        }//12

                        if ( SCALE.IZSM_ERROR==1 ) // check for scale over condition
                       {//10                               
                            display_message( "--E4--",0);  
                            update_status(1); // blank status display
                            output_display(0); // update seven segment display
                            break; // break out of case statement
                        }//12
                                  
                       
                       
                      if (SCALE.OVER) // check for scale over condition
                       {//10                               
                            display_message( "--E0--",0);  
                            update_status(1); // blank status display
                            output_display(0); // update seven segment display
                            break; // break out of case statement
                        }//12
                       
                       
                       // motion blanking
                        if (CAL.MOTION_BLANK==1) // check if motion blanking is activated
                       {//20

                       if (SCALE.MOTION)  // if scale motion
                       {//22
                        
                           for (i=0;i!=6;i++)  // blank display
                           {//31
                            DISPLAY.DIGITS[i] =' ';
                            scale_status();   // update scale status anunciators
                            output_display(0); // update seven segment display
                           }//31
                            break; // break out of case statement
                       
                       }//22

                       }//20
                       
                       }//10       

               
                     if ( refresh_timer(TIMEOUT)) // has the display update rate timer timed out
                     {//23
                                            
                       refresh_timer(START); // restart the timer
                       displayvalue(SCALE.DIS_WEIGHT,SCALE.DIS_DEC,SCALE.DIS_DEADZERO,""); // format weight string
                  
                      if (SCALE.NEGATIVE)  // check if the scale is negative
                      {
                        DISPLAY.DIGITS[0] = '-';
                      } // 

                     }//23
                      
                      if (SYSTEM.CAL) // are we in calibration mode
                      { //1 insert blinking C in the display

                        if ( timer(DISPLAY_C,TIMEOUT) )  // blink timer for the blinking 'C' character
                        {//2
                          timer(DISPLAY_C,START); // restart the timer

                          if (flag==0) flag=1;
                           else
                             flag=0;
                        }//2

                        if (flag==1)
                        {//3
                          DISPLAY.DIGITS[0] ='C';  // insert blink C to left most character position
                     
                        }//3
                      }//1

                      scale_status();   // update scale status anunciators
                      output_display(SCALE.DIS_DEC); // update seven segment display
                   break;
                   }

   case  _PARAMETER:{ //------------------------ display mode is parameter entry --------------------------
                  // 
                   break;
                   }

   case  _CAL:    { // --------------- display mode is scrolling through calibraton parameter -------------

                   timer(DISPLAY_PRM, START);  // start the display timer for displaying the CAL parameter number       
                   if ( timer(DISPLAY_PRM, TIMEOUT) ) // timeout for displaying calibration number
                   {
                     SYSTEM.DISPLAY_MODE=_WEIGHT;
                   }
                   update_status(1); // blank status display
                   output_display(3); // update seven segment display inserting a decimal place

                 break;
                 }


  

  case  _RAWCOUNTS: {    
    
                     d=SCALE.AD;
    
    
                     if ( d < 0)  // check for neagtive
                      {
                        d=d*-1;
                        neg=1;
                      }
    
                      displayvalue(d,0,0,"");
                   
                      
                        if ( timer(DISPLAY_C,TIMEOUT) )  // blink timer for the blinking 'A' character
                        {//2
                          timer(DISPLAY_C,START); // restart the timer

                          if (flag==0) flag=1;
                           else
                             flag=0;
                        }//2

                       //
                        
                        if (neg)
                        {
                         DISPLAY.DIGITS[0] ='-';                         
                        }
                        
                        
                        if (flag==1)
                        {//3
                         DISPLAY.DIGITS[0] ='A';  // insert blink C to left most character position
                        }//3
                      
                       if ( d > 999999)  // check for neagtive
                        {
                         display_message( "--E9--",0);  // error outside of zero range       
                        }                         
                         
                      update_status(1); // blank status display
                      output_display(0); // update seven segment display
                     break;
                    }
        
          
     
         
 case  _MESSAGE: { // displaymode is message         
                  
                 update_status(1);
                 output_display(0); // update seven segment display 
    
          break;
         }
    
         
         
 case  _DISPLAY: { // displaymode is DISPLAY, display a value from ACC in print system                       
          
               output_display(SCALE.DIS_DEC); // update seven segment display               
            break;
         }
                     
         
   case  _PIECE: { // displaymode is DISPLAY, display a value from ACC in print system
                   
                   
            if(CAL.PIECE_AVG > 0)
              {               
           
             dd = (float) (SCALE.DIS_WEIGHT * CAL.PIECE_AVG);                
             d = (long) dd;
             fraction= dd-d; //get the remaining fractional value from float
             //fraction now holds the fractional part of the weight
             if (fraction> 0.5) d++;  //if the fractional value is > 0.5 then round up the integer val             
             SCALE.DIS_COUNT = d;                  
                          
             displayvalue( SCALE.DIS_COUNT,0,0,"");        
             if (SCALE.NEGATIVE)  // check if the scale is negative
                      {
                        DISPLAY.DIGITS[0] = '-';
                      }
             
             scale_status();   // update scale status anunciators
             output_display(0); // update seven segment display
              break; 
              }
              
         SYSTEM.DISPLAY_MODE=_WEIGHT;     
         break;
         }                      
              
          
    default:  {
               SYSTEM.DISPLAY_MODE=_WEIGHT;
              break;
              }

   } //1 end of switch

} // end of function

// --------------------------------------------------------------------------------------------------------
// parameter entry
//
// enter numeric values usinf the up/down and left/right key
//
//  'val' is the initial value to be dsipalyed in the editor
//  'digits' is the number digits the editor will edit [1..6]
//  'start' is the position in display the editor starts from the left hand side [1..6]
//  'prompt' is the 6 character string to embed in the display with editied value
//  'nodec'  do not format with decimals or psecial tic editor mode
//  
//
// note: if nodec has a value of 99, parameter entry will enter into a special mode for ticket editing
// this was done to reuse the code of this function as much as possible
//---------------------------------------------------------------------------------------------------------

long parameter_entry(long val,char digits,char start,char string[],char nodec)
  {

char key;       // keypress value
char digit[6];  // holds value of each digit being editied 0-9
char digitpos;  // which digit position are we pointing to 0-5
char i;
char j;
char blink;         // flag used to for blinking a character
bool ticedit;       // flag used to indicate that we are in ticedit mode
bool ticcode=false; // flag to indicate that a tic code has been set
bool clearval;      // flag to indicate the digits are to be cleared
long value;
int mask;
char ch;
char st[5];

// digit max min control                                    
char digitmax[6]={9,9,9,9,9,9};  // used in special conditions to control the max/min of individual digit values 
char digitmin[6]={0,0,0,0,0,0};  // default is 0 for min number and 9 for max number

long div;
long result;
 
char offset;
char left_boundary;
char dec;


   
val=val&0x7ff;  // strip control bits, just data value is to be displayed     
   






// caclulate offset in positioning digits
offset=5-( (digits-1) + (start-1) );

// initialize the blinking cursor to right most digit
digitpos=5;

//initilaize left boundary for left cursor
left_boundary=6-digits;


// initialize parameter editor with starting value passed externally
// 6 digit long to string conversion
                   div=100000;  // initialize divsor for binary to 6 digit conversion
                   for (i=0;i!=6;i++) // loop through 6 digits
                     {//3
                      result=val/div; // divide by divisor
                      digit[i]=result; // this is the current result for the digit[i]
                      val=val-(result*div); // subtract result to get remainder for next digit
                      div=div/10; // ajust the divisor for next digit

    


      switch (key)
      {//3

       case _UP:
                { // incrememnt digit

                   
                  
                 if (ticcode==true)    // check for special ticedit condition 
                 {//1                                         
                    digit[digitpos]++;
                    if (digit[digitpos]==12)
                    {
                      digit[digitpos]=0;
                    }
                   
                 }//1
                 else  // normal digit entry to a max count of 9
                 {//2             
                    if (digit[digitpos]!=digitmax[digitpos]) // check we are no going past max
                    {
                      digit[digitpos]++;
                    }
                 }//2

                  timer(PARAMEDIT_B,START); // restart the blink timer
                  blink=0; // clear blinking while scrolling up/down

                break;
                }


       case _DOWN:
                 { // decrement digit

                   
                    if (ticcode==true)    // check for special ticedit condition 
                 {//1                                 // in digit position 2, we only allow a max count of 2        
                                        
                    if (digit[digitpos]==0)
                    {
                      digit[digitpos]=11;
                    }
                    else
                    {
                     digit[digitpos]--; 
                    }
                    
                 }//1
                 else  // normal digit entry to a min count of 0
                   
                 if (digit[digitpos]!=digitmin[digitpos]) // check we are not going past min
                 {
                   digit[digitpos]--;
                 }

                  timer(PARAMEDIT_B,START); // restart the blink timer, blink the digit
                  blink=0; // clear blinking while scrolling up/down

                 break;
                 }



        case _LEFT:
                 { // shift the digit position to the left
                  if (digitpos!=left_boundary)
                  {
                    digitpos--;
                  }

                  timer(PARAMEDIT_B,START); // restart the blink timer, blink the digit
                  blink=1; // blink character right away to show that you have moved left or right

                 break;
                 }



        case _RIGHT:
                 {  // shift the digit position to the right

                  if(digitpos!=5)
                  {
                    digitpos++;
                  }

                  timer(PARAMEDIT_B,START); // restart the blink timer, blink the digit
                  blink=1;  // blink character right away to show that you have moved left or right

                 break;
                 }


          case _CLEAR:
                 {  // abort editing and return -1 to indicate we have aborted
                    //
                    //  
                    //
                    return(-1);
                 }

 

      case _ENTER: {  // ENTER key is pressed, process calibration command that we are currrently pointing to in the menu

                  value=0;
                  mask=0;
            
                   if (ticedit==true)  // special tic editor mode, modify to support display of C and P codes (characters)
                    {//2
       
                     
                      clearval=false; // initialize flag
                      
                      for (i=5;i!=0;i--)   // loop through and clear any numbers after a C or P code (safety check)
                       {//4
                         
                            
                           if (clearval==true)  // check if the clearval flag has been set
                           {
                            digit[i]=0;  // clear digit after C or P codes 
                           }
                         
                         
                           if (digit[i]>9)           // check if C code is set
                           {
                             clearval=true;  // set flag
                           }                         
                           
                       }//4   
                      
                            
                      
               
                    }//2
                                                    
                  value=value + digit[5];
                  value=value + digit[4]*10;
                  value=value + digit[3]*100;
                  value=value + digit[2]*1000;
                  value=value + digit[1]*10000;
                  value=value + digit[0]*100000;
         
               
                  return(value);
                  //
                   }
                  
      } // end of switch
// build the display to display the value being edited
// copy the digit array over to the display array and format
// copy the the message prompt string over to the display
  for (i=0;i!=6;i++)
  {
   DISPLAY.DIGITS[i]=string[i];
  }

   // copy digits over to display
  j=5;
  for (i=0; i!=digits; i++)
  {
   DISPLAY.DIGITS[j-offset]=digit[j]+48;
   j--;
  }

  // blink the digit position we are pointing to in the editor
  if (timer(PARAMEDIT_B,TIMEOUT))
   {//1
    timer(PARAMEDIT_B,START);

     if (blink==1) blink=0;
      else
      {
       blink=1;

      }
    }//1

  
   if (ticedit==true)  // special tic editor mode, modify to support display of C and P codes (characters)
   {//2
    for (i=0;i!=6;i++)
    {
     if (digit[i]==10)           // if digit position 2 (from the left of the display) is a one, display a 'C'
     {
      DISPLAY.DIGITS[i]='C'; 
     }
     else
      if (digit[i]==11)           // if digit position 2 (from the left of the display) is a two, display a 'P'
      {
       DISPLAY.DIGITS[i]='P'; 
      } 
    } 
     
   }//2
   
//    
  
   if (blink)
   {//3
        DISPLAY.DIGITS[digitpos-offset]=' ';  // blank digit we are pointing to simulate blinking

   }//3

   dec=SCALE.DIS_DEC+offset; // calculate decimal position
  
   if (nodec)
   {
     dec=0;
   }
    
  
         
   output_display(dec); // update seven segment display

} //3 end of while

}// end of function parameter entry


//--------------------------------------------------------------------------------------------------
// this is the main system scan loop, all keyboard scanning and function execution is done here
// 
//--------------------------------------------------------------------------------------------------
void scan_system(void)
{

char key;

 if (SYSTEM.CAL)  // are we in calibration mode
   {//1
     
     key = scan_keypad(_EXTENDED);  // scan the keypad for keypresses

     // key = scan keypad
     
      switch (key)
      {//3

       case _UP: { // up key is pressed for scrolling up though calibration commands
                  
            if (SYSTEM.DISPLAY_MODE == _WEIGHT) // do not zero scale in raw counts mode
                    {
                    SCALE.ZEROSCALE=1;  // activate zero scale state machine
                    }
                   break;         
         
               //   prm_scroll(_UP);
                break;
                }


       case _UP_H: { // up key is pressed for scrolling up though calibration commands
                   
            break;
               //     prm_scroll(_UP_H);

                break;
                }

       case _DOWN:{ // down key is pressed for scolling down through calibration commands

              SYSTEM.DISPLAY_MODE=_WEIGHT; 
              break;
              //     prm_scroll(_DOWN);

                 break;
                 }

       case _DOWN_H:{ // down key is pressed for scrolling down through calibration commands

                 break;
                 //  prm_scroll(_DOWN_H);

                 break;
                 }

                 
      case _ENTER: {  // ENTER key is pressed, process calibration command that we are currrently pointing to in the menu

       
                    if (SYSTEM.DISPLAY_MODE == _CAL) // check if we are displaying calibration parameter number
                    {
                      calibration_command(SYSTEM.CAL_PARAMETER);  // execute calibration command, edit parameter we are pointing
                    }
                    
                    else
                      
                    if (SYSTEM.DISPLAY_MODE == _RAWCOUNTS) // check if we are displaying rawcounts
                    {
                      SYSTEM.DISPLAY_MODE = _WEIGHT;
                    }              
                    
                    break;
                   }


case _PRINT_H:       {   //PRINT  ENTER
                  
                       prm_scroll(_UP_H);

                break;
                 
                   }  
                     
                    
                   
                   
case _LEFT_H:          {
                           prm_scroll(_DOWN_H);

                 break;
                          }

       case _RIGHT: {  // ZERO key is pressed, zero the scale

                    prm_scroll(_UP);
                    break;
         
                 if (SYSTEM.DISPLAY_MODE!= _RAWCOUNTS) // do not zero scale in raw counts mode
                    {
                    SCALE.ZEROSCALE=1;  // activate zero scale state machine
                    }                                      

                  break;
                  }



       

      case _LEFT: {  // left key is pressed, zero the scale, exit out of P parameter

                prm_scroll(_DOWN);

                 break;
                  //SYSTEM.DISPLAY_MODE=_WEIGHT; 
                  //
                  break;
                  }           
                             
                  
                                 
                  
                  
       case _LEFTRIGHT_H: {  // TARE and UNITS are held for long period

                       if (SYSTEM.CAL)  // if in calibration
                       {
                         exit_cal();  // exit cal
                       }


                     break;
                    }
                    
                    
                    
      case _CLEAR: {
                  break;
                  // if the display is in RAWCOUNTS (calibration 9.0), then cancel back to PARAMETER;
                 if (SYSTEM.DISPLAY_MODE== _RAWCOUNTS)
                 {
              
                   prm_scroll(_UP);// exit rawcounts display, back to parameter mode
                     
                 }
              
                 
                    break;
                    }

///

                  }//3 end of switch


   }//1

 //-------------------------------------------------------------------------------------------------------------------
 
 else    // we are in scale indicator mode, process keypresses
    {//2



      key = scan_keypad(_NORMAL);  // scan the keypad for keypresses


      switch (key)
      {//3
        

 


 




       case _CLEAR: //  GROSS/NET KEY
                  { // this is actually a dual function key CLEAR/GROSS/NET

                  if (SCALE.TAREWEIGHT==0)   // there is no tare recorded, GROSS/NET has no function
                  {
                    SCALE.NETMODE=0;
                    break;  // exit
                  } 
                     
                  
                  if (SCALE.NETMODE==1)
                  {
                    SCALE.NETMODE=0;
                  }
                  else
                  {
                    SCALE.NETMODE=1;
                  }
                                      
                   // l      
                                  
                 break;
                 }

        case _CLEAR_H:
                 { // clear transaction memory
                     if (SCALE.TAREWEIGHT !=0) // check if at zero
                     {  
                       clear_tare();
                     }             
               
                 SCALE.IZSM_ERROR=0;  // set flag to indicate that we could not zero  
                 SCALE.PEAKCLEAR=1; // set flag to CLEAR peakdetect (if enabled)                   
                 break;
                 }



      case _TARE: {  //  tare scale
        
                     if (CAL.TARELOC ==0)  // check if tare key is not locked out
                      {
                          platter_tare();  // tare the scale
                      }
                
                    // 
                    break;
                   }
                   
        
      case _LEFT_H: {  //  tare scale

                              

                    break;
                   }           




       case _ZERO: {  // ZERO key is pressed, zero the scale
                
                    SCALE.ZEROSCALE=1;  // activate zero scale state machine
                    SCALE.IZSM_ERROR=0;  // set flag to indicate that we could not zero  
                  break;
                  }


       case _UNITS: {  // UNITs key is pressed, toggle between enabled units

               
                                               
          if(CAL.PIECE_EN == 1 )
          {
              if (SYSTEM.DISPLAY_MODE== _WEIGHT)
                 {
                   SCALE.DIS_UNITS = CAL.UNITS;// set units to what was selected 
                   SYSTEM.DISPLAY_MODE= _PIECE;
                     
                 }
               else
               {
                toggle_units(); // toggle the scale units
                SYSTEM.DISPLAY_MODE= _WEIGHT; 
               }
                 
           break;               
          }          
                     toggle_units(); // toggle the scale units
                     break;
                     }
  
                    
                    
      case _UNITS_H: {  // UNITs key is pressed, toggle between enabled units

        
           
                    break;                   
        
                    }
                                  
                
                
       case _PRINT: {  // PRINT/ENTER key is pressed

                    
                   SCALE.COM_STREAM0=1;  // set flag to transmit stream to comport (if enabled)
                   SCALE.COM_STREAM1=1;  // set flag to transmit stream to comport (if enabled)
                   SCALE.COM_STREAM2=1;  // set flag to transmit stream to comport (if enabled)
                   
                   
               
                  break;
                  }
      
                  
       case _PRINT_H: {  // PRINT are held for long period (left/right arrow)
 
                                
                          break;
                          }
                    
           case _TAREZERO_H: {  // TARE and UNITS are held for long period (left/right arrow)

                     //  
                        SYSTEM.DISPLAY_MODE=_WEIGHT; 
                        enter_cal(0);  //  enter ca
                       
                     break;
                    }



  case _TAREPRINT_H : {  // TARE and UNITS are held for long period (left/right arrow)

                        SYSTEM.DISPLAY_MODE=_WEIGHT;
                        display_audit();
                     // 
                     break;
                    }


                  }//3 end of switch


   }//2


}// end of function

//-----------------------------------------------------------------------
//  flash message on display
//  *message is pointer to the string[]
//  time is in 1/4 second increments
//  blink is in 1/4 second increments (0= no blinking)
//-----------------------------------------------------------------------
void display_message(const char *message,char time)
{

  char i;
  char t;
 
  t=0;
  timer(MESSAGE,START);

  while (1)
  {

   for (i=0;i!=6;i++)
  {
    DISPLAY.DIGITS[i]=message[i];
  }

  output_display(0); // update seven segment display

  if (time==0)
  {
   break; 
  }
  

    if (timer(MESSAGE,TIMEOUT))
    {
      timer(MESSAGE,START);
      t++;
      if (t==time) break;  // break out of while loop
    }

  } // end of while

}

//-------------------------------------------------------------------------------------------------
// convert a long to a string and display it
//
//-------------------------------------------------------------------------------------------------
void displayvalue(long val, char decimal, char deadzero, char str[])
{

long div;
char lzero;
long result;
char string[8];
char i,j;
char zsup;

                   // 6 digit long to string conversion
                   div=100000;  // initialize divsor
                   // lzero set how many leading zeros we want
                   lzero=decimal+1; // make sure 1 leading zero is left in front of decimal place

                   if (deadzero)  // if there are deadzeros
                   {
                     lzero=SCALE.DIS_DEADZERO+1; // set up for leading zeros
                   }

                   lzero=6-lzero;   // calculate display position for leading zeros
                   zsup=1;           // set the zero supressio flag to supress leading zeros

                   for (i=0;i!=6;i++) // loop through 6 digits
                   {//3
                      result=val/div; // divide by divisor
                     // result is a digit that needs to be converted into ascii

                      if (result==0)  // do we have a leading zero to suppress?
                      {//1
                               if ( (i < lzero) && (zsup) ) // are we supressing leading zeros
                               {
                                 string[i] =' '; // insert blank to supress zero
                               }
                               else // insert leading zero
                               {
                                 string[i]='0'; // insert zero character
                               }
                      }//1
                      else
                      {//2
                       string[i]=result + 48; // convert result to ascii
                       zsup=0;   // non zero value, clear the zero supression flag
                      }//2

                      val=val-(result*div); // subtract result to get remainder
                      div=div/10; // ajust a divisor for next digit
                   }//3 end of loop

      
// numeric string is ready to be displayed, check if we are appending any alphanumeric characters to the string


                 // check for a string
                 for (i=0;i!=6;i++)  // determine if we have characters to appened to the numeric display
                  {//1
                    if (str[i]==0)
                    {
                      j=i;     // j holds the number of characters in the string
                      break;
                    }
                  }//1

                  // if we have to insert a character(s), then left shift
                  for (i=0;i!=6;i++)  // left shift string to make room for additional characters
                  {
                   DISPLAY.DIGITS[i]=string[i+j];  // shift off left most digits
                  }
                  //
                  for (i=0;i!=j;i++)  // append characters to numeric string
                  {

                   DISPLAY.DIGITS[5-i]=str[i];  // append to right most side of display

                  }
//output_display(decimal); // update seven segment display
} // end of function


// --------------------------------------------------------------------------------------------------------
// format display
//
// enter numeric values usinf the up/down and left/right key
//
//  'val' is the initial value to be dsipalyed in the editor
//  'digits' is the number digits the editor will edit [1..6]
//  'start' is the position in display the editor starts from the left hand side [1..6]
//  'prompt' is the 6 character string to embed in the display with editied value
//  'nodec'  no decimals
//---------------------------------------------------------------------------------------------------------

void format_display(long val,char digits,char start,char string[],char nodec)
{

char digit[6];  // holds value of each digit being editied 0-9

char i;
char j;

long div;
long result;

char offset;

char dec;

// caclulate offset in positioning digits
offset=5-( (digits-1) + (start-1) );

// initialize parameter editor with starting value passed externally
// 6 digit long to string conversion
                   div=100000;  // initialize divsor for binary to 6 digit conversion
                   for (i=0;i!=6;i++) // loop through 6 digits
                     {//3
                      result=val/div; // divide by divisor
                      digit[i]=result; // this is the current result for the digit[i]
                      val=val-(result*div); // subtract result to get remainder for next digit
                      div=div/10; // ajust the divisor for next digit

                     }//3 end of loop

// 
// build the display to display the value being edited
// copy the digit array over to the display array and format
//  
// 
//
  // copy the the message prompt string over to the display
  for (i=0;i!=6;i++)
  {
   DISPLAY.DIGITS[i]=string[i];
  }
   // copy digits over to display
  j=5;
  for (i=0; i!=digits; i++)
  {
   DISPLAY.DIGITS[j-offset]=digit[j]+48;
   j--;
  }


   dec=CAL.DEC+offset; // calculate decimal position

   if (nodec)
   {
    dec=0;
   }


   output_display(dec); // update seven segment display

}// end of function parameter entry



//------------------------------------------------------------------------------------------
//  display_timer()
//  Display timer is a user programmable timer used to time display updates
//
//
//------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
// toggle displayed units
void toggle_units(void)
{
 
  char i;
  
   if (timer(UNIT,TIMEOUT))  // the unit toggle timer is used to toggle the units when the unit keys is held down               
                    { //1                   
                      
                      timer(UNIT,START); // restart units toggle timer
                      
                      for(i=0;i!=4;i++)  // loop and select next available units
                     {
                      SYSTEM.UNITS++;
                      
                      if (SYSTEM.UNITS > 2) SYSTEM.UNITS=0; // keep track of which units we are using

                       if (CAL.D_UNITS[SYSTEM.UNITS]!=0)
                       {
                        break;
                       }          

                     }

                      SCALE.DIS_UNITS = CAL.D_UNITS[SYSTEM.UNITS]; // set units to what was selected
                
                     } //1
  
  
  
} // end of function
