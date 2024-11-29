#include  "includes.h"
#include <ctype.h>
#include <string.h>

//---------------------------------------------------------------------------------
// The function runs as an endless loop only if it is enabled in calibration mode in COM1 mode.
// The Indicator will function as a remote display supporting DF1500 and DF2000 poll modes
// In slave mode, none of the scale functions are used. 

void  remote_display_slave(void)
{
char dec;
char j;
int State;
char c=0;
char i;
char pos;
char string[25];
char mode;
char key;
char command;
char keystate;
bool exit_remote=false;

if (CAL.COM1_MODE!=5)  // check if calibration is setup for remote display mode
{
 return;    // abort remote display mode 
}

State=0;
// start scanning com port for data
display_message( "DSPLAY",2);  // version number changed here

while(1) 
{  // loop through and read in ticket information
  
//------------------- frame strings and parse ----------------------------------------  
c=read_COM_RX(_PORT0);  // read from COM0 port

//c=read_COM_RX(_PORT2);  // read from COM0 port

if (c!=0)  // if there is a value. process state machine
 switch(State)
 {  
   
  case 0:  {  // wait for STX character
           
              if (c==0x02)
               { 
                 State = 1;
                 pos=0;
                 timer(REMOTE_DISPLAY,START);  // start the timeout timer
               }
              
            break;
            }
  
 case  1:  {  // wait for CR 
   
             string[pos++]=c;  // load in string
             
             if (pos>20) // safety check
             {
               State=0;  // reset 
             }
   
             if (c==13)  // we have finished loading in the string
             {
               State=2;
             }         
           break;
           }
 
 
  case 2: {  //  parse string to display
             dec=0;
             j=5;
             for (i=7;i!=0;i--)
             {
               if (string[i]=='.')
                 {
                  dec=7-i; 
                 }
                 else
                 {  
                   DISPLAY.DIGITS[j--]=string[i];
                 }  
             }
             
             if (string[0]=='-')
               {
                  DISPLAY.DIGITS[0]='-';
               }                
                        
                //1 = nt
                //2 = GR
                //4 = motion
                //8  = centre of zero
                //10 = g
                //20 = oz
                //40 = kg
                //80=  lb
          
                DISPLAY.STATUS2=0;
                DISPLAY.STATUS1=0;
                mode=0;
                              
                if (string[15] !='M') 
                {
                   DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0X10;  //  scale motion
                //  DISPLAY.STATUS=DISPLAY.STATUS | 4;  //  scale motion
                }                 
                                
                if (string[9]=='K')
                {
                   DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x04;  //  kg
                }                              
                
                if (string[9]=='L')
                {
                   DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x01;  //  lb
                }                         
                
                if (string[12]=='G')
                {
                   DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0x01; //  gr
                  mode='G'; 
                } 
                                               
                if (string[12]=='N') 
                {
                  DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0x02; //  nt        
                  mode='N';            
                }
            // i 
            State=0;
           // 
           break;
          } // 
 //       
 }  // end of switch
//------------------- check if keypad is pressed ----------------------------------------  
       
      key = scan_keypad(_NORMAL); // get latest keypress
     
      switch (keystate)   // this is used to prevent runaway keypresses for long key press holds
      {
        
      case 0: {   // check for a keypress
                if (key!=0)
                {
                  keystate=1;
                }
                else
                keystate=0;
                
               break; 
              } // 
            
      case 1: {  // keypress must release before going back to state 0
                if (key==0)
                {
                   keystate=0;                      
                }
                else
                {
                 keystate=1;  // wait for key release
                 key=0;    // ingnore keypress
                } 

               break;
              }         
              
      default: {
                 keystate=0;
                 break;
               }        
            
      } // end of switch
 
      switch (key)
      {  //1

       case _CLEAR: //  GROSS/NET KEY
                 { // this is actually a dual function key CLEAR/GROSS/NET
                  
                       if (mode=='G')
                      {
                         command='N'; 
                      }
                      else
                      {
                         command='G';
                      } // gross and net command  
                  break;
                 }

                 
      case _CLEAR_H: // CLEAR KEY
                 { // this is actually a dual function key CLEAR/GROSS/NET                                               
                      command='C';                 
                 break;
                 }


      case _TARE: {  //  TARE SCALE
                        command='T';
                   break;
                   }

       case _ZERO: {  // ZERO key is pressed, zero the scale

                        command='Z';
                    break;
                  } // 

       case _UNITS: {  // UNITs key is pressed, toggle between enabled units
                         
                     
                       if (string[9]=='K') // check what units to toggle
                       {
                          command='L';  //send a lb  command
                       }
                       else
                       {
                         command='K';  // send a kg  command
                         
                       } //                  
                     break;
                     } //                 
                   
       case _PRINT: {  // PRINT/ENTER key is pressed
                  command='P';             
                  break;
                  }
                          
      case _TAREZERO_H: {  // TARE and UNITS are held for long period (left/right arrow)
                     //   enter_cal(0);  //  enter cal
                        exit_remote=true; 
                      //  
                     break;
                    } // 
          }//3 end of switch
      // transmit poll characters ---------------------------------------------
      // transmit a poll character or a key press
           
      if( isCOMready(_PORT0)==true )   // can we transmit a character?
      {//1 
         if (command!=0) // do we have a keypress to transmit out the com port
         { //2
           write_COM_TX(command,_PORT0);
           command=0;  //reset command
          
           key=0;    // clear the key, to indicate sent
         }  // 
         else        // transmit a ? for poll mode
         {//3
              
             // transmit poll character 4 times a second
             if (timer( STREAM_DELAY0 , TIMEOUT))
             {//4
               write_COM_TX('?',_PORT0);
               timer( STREAM_DELAY0 , START);  
             }//4              
         }//3
      }//1

  if (timer(REMOTE_DISPLAY,TIMEOUT)) // check for no activity on comport
  {
    for (i=0;i!=6;i++)   // send all dashed to display
    {
     DISPLAY.DIGITS[i] ='-';
     update_status(1);
     output_display(0);
    }
  
  } // 
  else
  { //       
   update_status(0);
   output_display(dec);
  }
  
  if (exit_remote==true)
  {
    break; // break out of while loop 
  }  
  
  } // end of while
 
} // end of funtion

 


