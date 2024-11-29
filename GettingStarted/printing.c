#include "includes.h"

//-----------------------------------------------------------------------------------
// print ticket
//-----------------------------------------------------------------------------------

void print_ticket(int ticketnumber)
{
  
  if (!canprint())
  {
    display_message( "--E3--",2);  // display error 1
    return;    
  }
  
  
int data;
  
  TICKET.ADDRESS = searchticket( ticketnumber);  // search for the ticket in the ticket buffer
  
  if (TICKET.ADDRESS==-1) // if -1, then ticket could not be found 
  {
    return; 
  }
  
   
	TICKET.ADDRESS++; // we now point to the first available dataword for the ticket

	data=ticketbuffer(0,TICKET.ADDRESS,READ); // get data from ticket buffer


	while(data!=TICEND) // loop until ticket end
	{//1
                  if (SCALE.PRINT_ABORT==true)
                  {
                    break;  // break out of while loop, we are aborting print job 
                  }

	             if (data&TICCTRL) // check if character is a control character
	              {//2
	                 ticket_control(data);// parse and print the control function
	              }//2  

	              else   

	              if (data&PRNCTRL) // check if character is a printer control character
	              {//4
	                 printer_control(data);// parse and print the control function
                      }//4 end of PRNCTRL


                      else // we are printing a single assci character
	               {//5
                        xmitchar(data);

                      }//5 end of print ASCII


	 TICKET.ADDRESS++;
	 data=ticketbuffer(0,TICKET.ADDRESS,READ); // read next dataword from buffer

	}//1 end of while loop

   // 
   // reset special flags for print formating    
   TICKET.NO_UNITS=false;    // clear flags 
   TICKET.NO_HEADER=false;   
   TICKET.COM_DIRECT=0; // clear COM redirect
   refresh_ACC();  // refresh accumalators
   SCALE.PRINTLOCKOUT = true;  // set the print lock flag used in conjuction with C36
   SCALE.PRINT_ABORT = false; // reset flag
   SYSTEM.DISPLAY_MODE=_WEIGHT; // return display back to weight mode (safety)    
 
} // end of Print_ticket(char *msg)

 
//----------------------------------------------------------------------------------------
// Print a weight string to printer
// display mode is _DISPLAYED _GROSS, _TARE or _NET or _ACC
//----------------------------------------------------------------------------------------
void print( int displaymode)
{
 
char string[10];  
long weight;  

string[0]=0;
// 
//TICKET.NO_UNITS=FALSE;   // clear special no unit flags
//TICKET.NO_HEADER=FALSE;
// we use an existing function displayvalue to convert the weight to a string

  switch (displaymode)
  { //1
    
    case  __DISPLAYED:
               { // print current weight
                 weight=SCALE.DIS_WEIGHT; 
                 
                 if (SCALE.NEGATIVE==1) 
                   {
                     weight=weight*-1; 
                   }
         
                   break;
               }
               
               
     case  __GROSS:
               { // print GROSS weight
               weight=SCALE.DIS_GROSSWEIGHT; 
               weight=format_deadzero(weight);                                             
               break;
               }
    
    
     case  __TARE:
               { // print TARE weight
               weight=SCALE.DIS_TAREWEIGHT;
               weight=format_deadzero(weight);          
               break;
               }
    
     case  __NET: 
              { // print NET weight
               weight=SCALE.DIS_NETWEIGHT;
               weight=format_deadzero(weight);        
               break;
              } 
                
              
     case   __ACC:
             { // print weight in ACCx          
               
              weight=ACC[TICKET.ACC_INDEX];
               
              break;
             } 
               
  } //1 end of switch  
   
   // print the header before the weight 
   // 
  
  switch (displaymode) // print the words Gross, Tare or Net
    {
    
    
    case __NET:
    {
       strcpy(string,"Net   ");    
       break;
    }
    
    
    case __GROSS:
    {    
       strcpy(string,"Gross ");     
      break; 
    }

    
    case __TARE:
    {
        strcpy(string,"Tare  ");    
       break;
    }
  
    
  } // end of switch
    
      // print the header before the weight
      if ( (TICKET.NO_HEADER==FALSE) && (displaymode!=__ACC) ) // check if headers have been disabled from the ticket, or that we are printing an ACC register
      {
        xmitstring(string);  // send the string gross,tare,net to the comport
      }  
  
       // print the weight value
       format_string(weight, string,SCALE.DIS_DEC);  // format the final weight output
  
      // check if we have to insert dead zeros into string
    
  if ( (string[1]==0) && (string[0]=='0') )  // detect theat the string is showing a zero value
  {//1
        if (SCALE.DIS_DEADZERO==1)  // check for single deadzero
            {
              
              string[1]='0';
              string[2]=0;  // null terminate
              
            }
            else
            if (SCALE.DIS_DEADZERO==2)  // check for double deadzero
            {
              string[1]='0';
              string[2]='0';
              string[3]=0; // null terminae              
              
            } 
  }//1  
  
  
       xmitstring(string); // transmit the weight to the comport
           
       // print the units
       if (TICKET.NO_UNITS==FALSE) // check if units have been disabled from the ticket
       {
        xmit_units(SCALE.DIS_UNITS);  // send the scale units
       } 
   
       
} // end of function

 
//----------------------------------------------------------------------------------------------
void format_string(long value, char string[],char dec)
{
  
  char i;
  char j;
  char neg=0;    
  
  if (value < 0) // check for negative polarity
  {
    neg=1;
    value=value*-1;
  }
    
sprintf (string,"%7ld",value);  // build weight string
// we need now to insert a decimal, and leading zeros before the decimal
 if (dec>0)  // are there any decimals in the weight?
  {

   for (i=6;i!=(5-dec);i--)  // loop through and insert any leading zeros before or after decimal
   {
    if (string[i]==' ')
    {
     string[i]='0';
    }
    
   } //

   for (i=0;i!=(6-dec);i++) // loop through and shift string to create a space for the decimal
   {   
    string[i]=string[i+1];     
   }
    
  string[6-dec]='.';  // inset decimal
  }  
  
  if (neg==1)  // insert minus sign
  {//1
   for (i=7;i!=0;i--) // loop through and shift string to create a space for the sign
   {
     if (string[i]==' ')
     {
       string[i]='-';
       break;
     }
         
   }
  }//1

 // left allign the string
  for (i=0;i!=6;i++) //  left shift string 
  {//2
    if (string[0]==' ')  // if there is a space, left shift
    {
      for (j=0;j!=7;j++)  // left shift operation
       string[j]=string[j+1];
    }
    else
      break;  // no space, break out of loop
        
  }//2

} // end of function


//
//------------------------------------------------------------------------------------
// send character to COMPORT hardware buffer buffer
// depending on the COM MODE setting determing if the port is xmitting the character
// also the ticket system can overide the comport and switch to another port
//------------------------------------------------------------------------------------
void xmitchar(char ch)
{ 
// determine if COM1 or COM2 (or both) is tranmitting characters from the ticket to comport
// COM port mode determines this
   
 if ((CAL.COM1_MODE==2) && (TICKET.COM_DIRECT==0))     // check if COM1 is enabled for printing
  {
    write_COM_TX(ch,_PORT0);  // send the character to the COM1 port
     while (isCOMready(_PORT0)==false);  // loop until xmit buffer has transmitted
     return;
  }
 
 else
     
     if ((CAL.COM2_MODE==2) && (TICKET.COM_DIRECT==0))   // check if COM2 is enabled for printing
      {
        write_COM_TX(ch,_PORT1);  // transmit character to COM2
          while (isCOMready(_PORT1)==false);  // loop until xmit buffer has transmitted  
          return;
      }
     
      else  
    
            if (TICKET.COM_DIRECT==1)     // com port overide for com1,  force tranmission to com1
            {
              // because we are overriding a string output, we could be interupting right in the middle of a string transmission
              while(COM[_PORT0].IDLE==false)  // flush COM1 tranmission of any continuous strings and empty buffer before sending a weight string
              {
                process_COM_xmit(_PORT0);  // check if there are any characters to transmit COM 0
              }
              // we have finished flushing transmit buffer            
              
              write_COM_TX(ch,_PORT0);  // send the character to the COM1 port from the ticket
               while (isCOMready(_PORT0)==false);  // loop until xmit buffer has transmitted
               return;
            } 
           
            else
    
    
                  if (TICKET.COM_DIRECT==2)     // com port overide for com2,  force transmission to com2
                  {
                    // because we are overriding a string output, we could be interupting right in the middle of a string transmission
                    while(COM[_PORT1].IDLE==false)  // flush COM1 tranmission of any continuous strings and empty buffer before sending a weight string
                    {
                      process_COM_xmit(_PORT1);  // check if there are any characters to transmit COM 0
                    }
                    // we have finished flushing transmit buffer
                    
                    
                    write_COM_TX(ch,_PORT1);  // send the character to the COM1 port from the ticket
                     while (isCOMready(_PORT1)==false);  // loop until xmit buffer has transmitted
                     return;
                  } 
                 
 
}// end of fuction



//-------------------------------------------------------------
// check for valid printing conditions
//-------------------------------------------------------------
char canprint(void)
{

   if (!SCALE.MOTION)   // if there is no motion
     {
          if (!SCALE.OVER)  // is scale over
          {
            return(1);  //good to print
          }
     }


     
        return(0);  //  cannot print



} // end of function


//-------------------------------------------------------------------------------------


void xmitstring (char string[])
 {
  char c;
  int i=0;
   
  while(1) // end less loop
  {  //1


      
        
       c=string[i++];
       
       if (c==0)  // check for end of string
       {
        return;    // NULL end of string 
       }
    
       if (i>80)  // saftey check for run away condition
       {
        return; 
       }
       
       
       xmitchar(c);
       
        
      
           
    
  } //1
  
  
  
  
 } // end of function





//------------------------------------------------------------------------------------------------
void xmit_units( int units)
{
  
     switch(units)

    {

    case _KG:  {
                xmitstring (" kg");
              
              break;
              }


    case _LB:  {
                xmitstring (" lb") ;
              break;
              }

              
    case _G:  {
               xmitstring (" g") ;
               break;
              }

    case _OZ:  {
                xmitstring (" oz") ;
              break;
              }

  case _TON:  {
               xmitstring ("  t") ;
               break;
              }
              
 case _STON:  {
               xmitstring (" tn") ;
               break;
              }
              
              case _LTON:  {
               xmitstring (" lt") ;
               break;
              }
    } // end of switch

  
  
  
  
  
  
  
} // end of function






//------------------------------------------------------------------------------
// auto print process
// automatically prints if activated
//------------------------------------------------------------------------------

void process_autoprint(void)
{

if (CAL.AUTO_PRINT==0)   // check if autoprint is enabled
{
 return;  // not enabled, return
}

if (SYSTEM.CAL)   // check if in calibration
{
 return;  // calibration return
}



static char state=0;


  switch(state)
  {
  case 0: { // wait for scale to be at zero


              if (canprint())
              {//1
                 if (SCALE.DIS_WEIGHT < CAL.AUTO_PRINT) // scale must be below auto print thresh
               {//2
                 state=1;
               }//2
              }//1

          break;
          }



  case 1: {



                 if ( (!SCALE.NEGATIVE) && (SCALE.DIS_WEIGHT > CAL.AUTO_PRINT) ) // if scale not negative  and > 5
                   {//3
                      if (canprint()) // on no motion, or scale over
                      {//4
                        
                        
                        if ((CAL.COM1_MODE==2) || (CAL.COM2_MODE==2))  // is printing enabled
                    {
                     print_ticket(1); 
                    }
                        
                        
                        state=0; // reset statemachine
                        break;
                      }  //4
                   }//3

          break;
          }






  default: {
             state=0;
             break;
           }

  }  // end of switch




}  // end of function






//-----------------------------------------------------------------------------------------
// process print lockput task
// a flag is set when printing, and cleared when the weight goes below a threshold
// used to prevent false multiple accumalations if the print key is pressed numerous of times.
//-----------------------------------------------------------------------------------------
void  process_printlockout(void)
{
  
    if (SCALE.DIS_WEIGHT < CAL.PRINTLOCKOUT)  // if weight below threshold 
    {
      SCALE.PRINTLOCKOUT=false;  // clear flag
    }  
  // 
} // end of function

//-----------------------------------------------------------------------------------------
// insert deadzeros where required
// ----------------------------------------------------------------------------------------
long format_deadzero(long value)
{
  
if (SCALE.DIS_DEADZERO==1)  // check for single deadzero
{
  value=value*10;
}
else
if (SCALE.DIS_DEADZERO==2)  // check for double deadzero
{
  value=value*100;
}


  
  
return (value);  
  
} // end of funtion







