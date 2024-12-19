#include "includes.h"




// declaration of ACC registers
// acc[0] = TICKET COUNTER
// acc[1]..acc[5]   acc1 to acc5
// acc[6] check sum used to indicate that an update is required for flash


long ACC[_ACC_REGS]; // this is the working array used for managing ACCumalators

//the following variabels are processor flash page memeory specific
// _ACC_REGS is the number of ACC register the ticket macro system support so ACC1 to ACC 5 where ACC0 is the ticket counter
// _ACC_CIRCULAR is the number of entries for the circular buffer used to reduce flash wear and tare
// total flash memory needed to store ACC values is _ACC_COUNT * _ACC_CIRCULAR * 4    (long = 4 bytes)

//long ACC_CIRCULAR[_ACC_REGS][ _ACC_CIRCULAR];     // circular buffer globle table used for accumalators in the ticket system being stored to flash memory 
 long ACC_CIRCULAR[_ACC_CIRCULAR][ _ACC_REGS ];     // circular buffer globle table used for accumalators in the ticket system being stored to flash memory 


int TICKET_COUNTER; // ticket counter register




void ticket_control(int data)
{
  
char temp[TEMPBUFF];  // temperary buffer for printing   
int i;
int time; 
bool flag;

temp[0]=0;       // null terminate
data=data&0x7ff; // strip control codes

 switch(data)
 {  
   
   case 1: {    // print gross weight
            print( __GROSS);    
            break;
           }
      
           
           
   case 2: {    // print tare weight
            print( __TARE); 
            break;
           }           
 
          
           
   case 3: {    // print net weight
            print( __NET);
            break;   
           }      
           
               
           
   case 4: {    // print displayed weight 
              print( __DISPLAYED);
            break;
           }           
            
       
           
   case 5: {    // print weight stored in ACCx                  
            TICKET.ACC_INDEX = get_index();  // get the acummalator index we want to print      
            print( __ACC );  // print weight from accumalator 
            break;
           }            
           
           
   case 6: {    // print numeric value stored in ACCx
            TICKET.ACC_INDEX=get_index();  // get the acummalator index we want to print      
            sprintf(temp,"%ld",ACC[TICKET.ACC_INDEX]);  //format the string and prepare to print
            break;
           }            
           
           
   case 7: {    // print time
            format_time(temp);
            break;
           }            
                
             
   case 8: {    // print date dd/mm/yy            
             format_date(temp);
            break;
           }            

        
   case 9: {    // no header
            TICKET.NO_HEADER = true;
            break;
            }          
              
           
           
   case 10: {    // no units
            TICKET.NO_UNITS = true;
            break;
            }
         
            
            
            
   case 12: {   // print ticket serial count
            sprintf(temp," %03i ",ACC[0]);
            ACC[0]++;
            
            if (ACC[0] >999)
            {
              ACC[0]=0;
            }  
          
            break;
            }
 
            
    case 13: {  //clear  ticket counter
               ACC[0]=0;
        
            break;
            }
            
            
    case 14: {  //print units
            xmit_units( SCALE.DIS_UNITS);
            break;
            }        
          
            
            
     case 15: {  // flag that sets no priniting of seconds for time
               SCALE.TIME_NOSECONDS=1;
               break;
              }                        
            
            
            
    case 20:  {  // Clear ACCx
                TICKET.ACC_INDEX=get_index();  // clear accumalator register   
                ACC[TICKET.ACC_INDEX]=0;
                break;
              }
                
              
    case 21:  {  // ACC1= gross weight   
                ACC[1]=format_deadzero( SCALE.DIS_GROSSWEIGHT);
                break;
              }         
           
           
    case 22:  {  // ACC1= tare weight              
               ACC[1]=format_deadzero( SCALE.DIS_TAREWEIGHT);
             
                break;
              }            
           
           
    case 23:  {  // ACC1= net weight
                   
                ACC[1]= format_deadzero( SCALE.DIS_NETWEIGHT); 
                break;
              }           
           
           
    case 24:  {  // ACC1= displayed weight
                   
                ACC[1]=SCALE.DIS_WEIGHT; 
                if (SCALE.NEGATIVE==1)
                 {
                   ACC[1]=ACC[1]*-1;
                 }
                break;
              }            
           
              
    case 25:  {  // ACCx= ACC1    copy ACC1 to ACCx
                TICKET.ACC_INDEX=get_index();  // SEARCH FOR INDEX
                ACC[TICKET.ACC_INDEX]=ACC[1] ; // COPY acc4 TO accX
                break;
              }       
            
                 
    case 26:  {  // ACC1= ACCx    copy ACCx to ACC1
                TICKET.ACC_INDEX=get_index();  // SEARCH FOR INDEX
                ACC[1]=ACC[TICKET.ACC_INDEX]; // COPY acc4 TO accX
                break;
              }                                    
              
              
              
    case 27:  {  // ACCx= ACCx + ACC1    accumalation to accx
                TICKET.ACC_INDEX=get_index();     // SEARCH FOR INDEX
                ACC[TICKET.ACC_INDEX]=ACC[1] + ACC[TICKET.ACC_INDEX] ;
                break;
              }         
               
    case 28: {  // ACC1=ACC2+ACC3 
                ACC[1]=ACC[2]+ACC[3];
                break;
             }
             
    case 29: {  // ACC1=ACC2-ACC3
                ACC[1]=ACC[2]-ACC[3];
                break;
             }      
             
    case 30: {  // ACC3=ACC4 * (ACC2/100)   Take a percentage of ACC4, results stored in ACC3
      
                //            
                ACC[3]=ACC[4]  * (ACC[2]/100) ;
                break;
             }           
             
             
    case 31: {  // ACC1=ACC2/ACC2
              
                if (ACC[3]==0) 
                {
                  break;
                } 
                
                ACC[1] = ACC[2] / ACC[3] ;
   
                  break;
             }  
   
          
            
    case 32: {  // ACC1=ACC2*ACC3
              
                
                
                ACC[1] = ACC[1] * ACC[2] ;
   
                  break;
             }  
                
             
             
             
    case  33:    {  //  ACCx=ACCx+1
   
                     TICKET.ACC_INDEX=get_index();     // SEARCH FOR INDEX
                     ACC[TICKET.ACC_INDEX]=ACC[TICKET.ACC_INDEX]+1;
                    
                   break;  
                   }  
                   
                   
                   
 case  34:    {  // send ACC to display as weight
   
                 for (i=0;i!=6;i++)  
                 {
                    DISPLAY.DIGITS[i]=' '; //blank displsy               
                 }
                 
                  TICKET.ACC_INDEX=get_index();  // SEARCH FOR INDEX
                 
                  displayvalue(ACC[TICKET.ACC_INDEX],SCALE.DIS_DEC,SCALE.DIS_DEADZERO,""); // format weight string
             
                  SYSTEM.DISPLAY_MODE=_DISPLAY;  // display message
                 
                 update_display();   
                
               break;
              }   
                    
                
                          
                  
 case  35:    {  // send ACC to display as numeric
   
                 for (i=0;i!=6;i++)  
                 {
                    DISPLAY.DIGITS[i]=' '; //blank displsy               
                 }
                 
                  TICKET.ACC_INDEX=get_index();  // SEARCH FOR INDEX
                 
                  displayvalue(ACC[TICKET.ACC_INDEX],0,0,""); // format string
                         
                  SYSTEM.DISPLAY_MODE= _MESSAGE;  // display message
              
                 update_display();   
                
               break;
              }   
               
 
 case 37:  {  // Clear all ACCs
               
                for (i=0;i!=_ACC_REGS;i++)
                {
                  ACC[i]=0;
                }
                break;
              }             
              
              
              
              
              
              
              
 case 36:     { // print lockout function
   
   
                   if (SCALE.PRINTLOCKOUT==true)
                   {
                      SCALE.PRINT_ABORT=true;  // set flag to abort printing if SCALE.PRINTLOCKOUT is true
                   }  
                   
                   
                     
               break;
              }
                   
 
              
 case  40:    {  // send text to display
   
                 for (i=0;i!=6;i++)  // get next 6 characters in buffer and display
                 {
                    DISPLAY.DIGITS[i]=' '; //blank displsy               
                 }
                 
                 
                  for (i=0;i!=6;i++)  // get next 6 characters in buffer and display
                  {//3
                    
                    TICKET.ADDRESS++;  // point to next data in buffer
                    data=ticketbuffer(0,TICKET.ADDRESS,READ); // read next dataword from buffer  
                    
                    data=toupper(data); // shift characters to uppercase
                    
                 
                    
                    flag=false; // this  flag is used to evaluate the data as displayable
                    
                    if ((data>64) && (data < 91))  // check if we have letters between A..Z
                      {//1
                        flag=true;  
                      }//1 
                    
                  
                    
                     if ((data>47) && (data < 58))  // check if we have letters between 0..9
                      {//2
                        flag=true;  
                      }//2
                                              
                     
                        
                      if ( data==32)  // check for a space
                       {//3
                         flag=true;  
                       }//3
                    
                     
                    if (flag==false)  // current data is not part of the display message
                    {
                      TICKET.ADDRESS--; // the ticket pointer was incremented, we need to decrament it one
                      break;
                    }
                    else
                    {
                     DISPLAY.DIGITS[i]=data;
                    } 
                    
                    
                  }//3

                 
                  
                 SYSTEM.DISPLAY_MODE=_MESSAGE;  // display message
                 update_display();   
                
               break;
              }   
                    
                
              
 
              
 
 case  41:     {  // weight keyboard entry to ACC4 
                     
                 i=get_index();     // get number of digits that we want to format
      
                 if (i>6)  // cannot be more than 6 digits
                  {
                   i=6;  // set count to 6
                  }
                 
                   ACC[4]=parameter_entry(ACC[4],i,1,"      ",0);                     
       
                break;      
               }  
   

 case  42:     {  // numeric entry to ACC4, number digits 
                     
                  i=get_index();     // get number of digits
                  if (i>6)  // cannot be more than 6 digits
                  {
                   i=6;  // set count to 6
                  }
                    
                  
   
                  ACC[4]=parameter_entry(ACC[4],i,1,"      ",1);
                  
                
     
                break;     
               }              
                            
         


     
  
 case 43:      {   // delay in 0.25 sec increments
   
                       i=0;
                       
                      time=get_index();     // get number of digits
                      
                      if (time > 40)  // check for error condition
                      {
                        break;  // exit delay
                      } 
                       
                      timer(MESSAGE,START);  // start timer

                  
                      
                   while (1) // loop and wait for delay
                   {


                    if (timer(MESSAGE,TIMEOUT))
                    {
                     timer(MESSAGE,START);
                     i++;
                      if (i==time) break;  // break out of while loop
                    }
   
                   } // end of while 
                    
                   SYSTEM.DISPLAY_MODE=_DISPLAY;  // reset display mode back to weight
                   
                   
                break;
               }
               
      
 case 45:   { // declare menu name for ticket
             
     for (i=0;i!=6;i++)  // get next 6 characters in buffer and display
                  {//3
                    
                    TICKET.ADDRESS++;  // point to next data in buffer
                    data=ticketbuffer(0,TICKET.ADDRESS,READ); // read next dataword from buffer  
                    
                    data=toupper(data); // shift characters to uppercase
                    
                 
                    
                    flag=false; // this  flag is used to evaluate the data as displayable
                    
                    if ((data>64) && (data < 91))  // check if we have letters between A..Z
                      {//1
                        flag=true;  
                      }//1 
                    
                  
                    
                     if ((data>47) && (data < 58))  // check if we have letters between 0..9
                      {//2
                        flag=true;  
                      }//2
                                              
                     
                        
                      if ( data==32)  // check for a space
                       {//3
                         flag=true;  
                       }//3
                    
                     
                    if (flag==false)  // current data is not part of the display message
                    {
                      TICKET.ADDRESS--; // the ticket pointer was incremented, we need to decrament it one
                      break;
                    }
   
                    
                  }//3  
                    
                    
                    
             break;  
             }
               
               
     
 case 50:   {  // direct com output to COM1
               TICKET.COM_DIRECT = _COM1;
               break;
            }
               
               
 case 51:   {  // dirext com output to COM2
               TICKET.COM_DIRECT = _COM2;
               break;
            }               
 
                      
            
      
   
} // end of switch
  
  
  
  
//print any strings that were built in the case staement above


if (temp[0]!=0) // check if there is something to print
{//1
  for (i=0;i!=TEMPBUFF;i++) // loop through string and transmit contents
  {
  
   
   if (temp[i]==0) // check for null character
   {
     break;  // break out of loop, finished tranmitting string
   }
   
    xmitchar(temp[i]);
   
  }
  
}//1






  
  
} // end of function

 




//---------------------------------------------------------------
// extractvalue
//---------------------------------------------------------------

long extractvalue (char n)
{

char string[7];
char i;
long val;
int data;

for (i=0;i!=7;i++)  // initialize string to spacces
{
 string[i] =' ';
}

for (i=0;i!=n;i++)  // loop through ticketbuffer and gather digits
{
TICKET.ADDRESS++;  
data=ticketbuffer(0,TICKET.ADDRESS,READ); // read next dataword from buffer

if ((data >'9') || (data <'0') )  // check for valid character 0..9
{ 
  break;
}

string[6-i]  = data;
}

val = atol(string);

return(val);

}  // end of funtion
	 

  
  
 

  

//----------------------------------------------------------------
//
// used to extract an ACC index from the the ticket buffer
// certain C codes required the ACC index 
//----------------------------------------------------------------
int get_index(void)
{
int data;
 
TICKET.ADDRESS++;  // point to next data in buffer
data=ticketbuffer(0,TICKET.ADDRESS,READ); // read next dataword from buffer   

if (data > ACC_MAX) // safety test 
{
 return(0); 
}
else
{
return(data);
}     


} // end of function




//-------------------------------------------------------------------
// The accumalator registers used in the macro system needs to be stored to flash to preserve there values.
// A circular buffer is used to reduce wear and tear in the flash system, a flash erase only occures when the 
// circular table is filled, and loops around. 
//
//  This function copies current acc tables over to SRAM based circular table, and then copies the SRAM table to flash.
//  
//  The accumalators used in the ticket macro system are stored in an array long ACC[]
//  ACC[0] is reserved for the ticket counter
//  ACC[1]..ACC[x]  is used for the ACC register operations in the ticket macro system
//  mode = WRITE  write current acc registers to circular buffer and write to FLASH
//  mode = READ   read current circular buffer data from FLASH and refresh ACC[] registers
//--------------------------------------------------------------------
void refresh_ACC(void)
{

int j;
char status;

// Copy all the accumalator circular buffer data from flash to ram 
// We do this everytime  to stay in sync

         for (j=0;j!=_ACC_REGS;j++)  // copy current ACC register over to circular buffer array
         {//3
            NVMEM.ACC[j] = ACC[j];   // copy accumalators over to circular buffer      
         }//3
              
        status = write_circular_flash();        // write circular buffer to flash memory, this stores all non volatiles acc registers
      
    // if the checksum in writing to flash fails, then erase the flash and try again.
        if (status==0) // check for fail
        {//2
          
               // turn off all transistors that are driving the display
              //FIO0SET=0x007e3480; // clear outputs for digit select (pfets active low, so set to clear)
              //FIO0CLR=0x7e810000; // clear 8 segment outputs by clearing bits (nfets)
             
              
              __disable_interrupt();            // Global interrupt disable
            
              unlock_flash_sector(16);   // erase the entire flash sector 
              erase_flash_sector(16);
              status = write_circular_flash();  // attempt to write the block again
                   
              //T0TC=0; // reset the display update back to zero
              __enable_interrupt();            // Global interrupt enable
                   
        }//2
                               
        // 
        return; // acc circular has been written to flash, exit function  
 
}  // end of function























