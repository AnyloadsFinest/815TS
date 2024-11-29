
#include "includes.h"

char KEYSTATE;
long KEY;

//-------------------------------------------------------------------------------------------------
// read what keys are pressed and return (debounced) key values
// mode = 0  return keypad press
// mode = 1  return alternate keypad value
//-------------------------------------------------------------------------------------------------
int scan_keypad(char mode)
{
long key;


 if (mode==_RESET)  // special mode that forces statemachine to reset 
 {
   KEYSTATE=4;  // force state machine to wait for no keypress, then reset   
 }

 key = get_key();  // read inputs from keypad

 switch(KEYSTATE)   // state machine is used for debouncing
  {//1

      case 0: {  // wait for keypress
               //   FIO3SET = 0x02000000; // disable output
                  
                   if (key!=0) // key has been pressed
                   {
                     KEYSTATE=1;  // go and wait for debounce timer
                     timer(KEYSCAN,START); // start debounce timer
                     KEY=key;  // capture key pressed
                    
                     if (SYSTEM.CAL==0)  // keypress blink feedback only if not in CAL mode
                     {
                      DISPLAY.BLINK = 100; // blink the display (100 scan duration) to indicate keypress
                     }
                     
                   if( CAL.BUZZ_EN == 1)
                   {
                   FIO3CLR = 0x02000000; // disable output 
                   }
                   }

               break;
              }



      case 1:  {  // debounce timer
               
                 if (timer(KEYSCAN,TIMEOUT))  // wait for timer to timeout
                 {
                   FIO3SET = 0x02000000; // disable output  
                   KEYSTATE=2;
                   timer(KEYPRESS,START);   // start timer to detect button held down
                   timer(KEYPRESSC,START);  // start timer to detect button held down (key press have different response in CALibration )
                   timer(KEYPRESSP,START);  // start timer to detect premature long keypress (a keypress < 3 sec and > 1 is ignored) 
                 }

               break;
               }


  
      case 2: { // key is pressed, wait for release of key
           
                // key sensitivity is different in calibration mode
                if (SYSTEM.CAL)  // long keypress detection in calibration is shorter
                {//1
                    if (timer(KEYPRESSC,TIMEOUT))  // check if key is held for long time
                    {
                    KEYSTATE=6;
                    }
                    else    
                     if (key!=KEY)  // is the key released, or changed from the last time?   
                     {//11
                     //
                       // all keys must be released (key==0)
                                                               if (key==0) // evaluate that the key release is a not 2 keys pressed together
                                                               {
                                                                 KEYSTATE=5;  // process keypress and reset state machine
                                                               }  
                                                               else
                                                               {
                                                                 KEYSTATE=0;  // reset statemachine, more than 1 key is pressed
                                                               } 
                       
                       
                       
                      // KEYSTATE=5;  // process key and reset statemachine and wait for next keypress
                     }//11
                  
                 
                }//1
                
                else  // we are not in calibration
                {//5

                                if (timer(KEYPRESS,TIMEOUT))  // check if key is held for long time
                                {//2
                                KEYSTATE=6; // process keypress for a long hold
                                }//2
                               
                                else
                                
                                        if (key!=KEY)  // is the key released, or changed from the last time? if (key!=KEY)
                                        {//4
                                                 
                                                  if (timer(KEYPRESSP,TIMEOUT)) // check for a premature long keypress timeout
                                                   {// 3 we ignore a key press that is held longer than a keypress, but not longer than a long keyhold
                                                     
                                                                                                     
                                                     KEYSTATE=4;  // ignore key release, reset statemachine
                                                   }//3
                                                     else  // we have a quick key release to evaluate
                                                     {//6 
                                                              // all keys must be released (key==0)
                                                               if (key==0) // evaluate that the key release is a not 2 keys pressed together
                                                               {
                                                                 KEYSTATE=5;  // process keypress and reset state machine
                                                               }  
                                                               else
                                                               {
                                                                 KEYSTATE=0;  // reset statemachine, more than 1 key is pressed
                                                               } 
                                                               
                                                             
                                                     }//6
                                            
                                        }//4

                }  //5


              break;
              }

              
       case 4:  {  // wait for key release, then reset statemachine
                  if (key==0)
                  {                  
                    KEYSTATE=0; 
                    FIO3SET = 0x02000000; // disable output
                  }
  
                 break;
                }
              
              
              
              

      case 5: { // detect key release, process key and  reset statemachine

              FIO3SET = 0x02000000; // disable output
              KEYSTATE=0;  // reset statemachine and wait for next keypress

              break;
              }


      case 6: { // key is pressed for long periode, wait for release

                if (key!=KEY)  // wait for key to be released
                {
                  FIO3SET = 0x02000000; // disable output 
                  KEYSTATE=0;  // reset statemachine and wait for next keypress
                }




              break;
              }



        default:  {
                    KEYSTATE=0;
                    break;
                  }



     }//1   // end of switch







   if (KEYSTATE == 5)  // we have detected that a key has been released
     
   {//5
     
         switch (KEY)
       {

                case _ZERO: {   //  ZERO

                    
                   if (mode==_EXTENDED)
                    {
                    return(_UP); //  RIGHT key
                    }
                    else
                    {
                    return(_ZERO);  // ZERO
                    }
                  
                   
                    
                   }

                 case _TARE: {   // TARE

                    if (mode==_EXTENDED)
                    {
                    return(_ENTER); //  LEFT key
                    }
                    else
                    {
                    return(_TARE);  // TARE
                    }
                   }


                 case _UNITS: {   // UNITS

                   if (mode==_EXTENDED)
                   {
                    return(_LEFT);
                   }
                   else
                   {
                     return(_UNITS);
                   }
                   }

                 case _PRINT: {   //PRINT  ENTER
                   if (mode==_EXTENDED)
                   {
                    return(_RIGHT);
                   }
                   else
                   {
                     return(_PRINT);
                   }
                   }

                   
                 case _CLEAR:
                   {   // CLEAR/DOWN key
                    if (mode==_EXTENDED)
                   {
                    return(_DOWN);
                   }
                   else
                   {
                     return(_CLEAR);
                   }
                   

                   }


                


  } // end of switch


   }//5

   if (KEYSTATE == 6)  // we have detected that a key has been held for a long periode of time
     
   {//6
     
     
         switch (KEY)
       {






case _TAREZERO:  {//1
                        if (mode==_EXTENDED)
                          {
                            return(_LEFTRIGHT_H);
                          }
                          else
                          {
                          return(_TAREZERO_H);  
                          }

                  }//1
                     
                  
case _TAREPRINT:
                     {
                     return(_TAREPRINT_H);   

                     }

                     
                     
                     
case _CLEAR:
                    {
                   
                                                          
                      
                    if (mode==_EXTENDED)
                    {
                        return(_DOWN_H); //  UP key
                    }
                    else
                    {
                       return(_CLEAR_H); //  CLEAR 
                    }
                       
                   
                       
                   
                     }
                     
                     
case _PRINT:       {   //PRINT  ENTER
                  
                         return(_PRINT_H);
                 
                   }  
                     
                    
                   
                   
case _UNITS:          {
                          if (mode==_EXTENDED)
                           {
                             return(_LEFT_H); //  UP key
                           }
                          else
                            return(_UNITS_H);
         
                      }
                     
                     

 case _ZERO:   {
         
                  return(_UP_H);
         
         
               }
               
           
               
 case _TARE:   {      
                     
                 return(_ENTER_H);    
                      
               }
   
   
   
  } // end of switch


   }//6


return(0);

} // end of function

