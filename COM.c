#include "includes.h"

struct circularbuff             RX0; // comport circular buffer
struct circularbuffradio        RX1; // circular buffer for radio
struct circularbuffrssi         RX2; // circular buffer for rssi

extern volatile DWORD UART0Status, UART1Status, UART2Status, UART3Status;
extern volatile BYTE  UART0TxEmpty, UART1TxEmpty, UART2TxEmpty, UART3TxEmpty;
extern volatile BYTE U0Status, U1Status, U2Status, U3Status; 
//------------------------------------------------------------------------------
struct circularbuff // circular recieve buffer for COM0 
{
char BUFF[BUFFMAX + 1]; // holds 80 characters
char WRITE; // write pointer in buffer
char READ;  // read pointer in buffer	
};  

struct circularbuffradio // circular recieve buffer for radio
{
char BUFF[BUFFMAX + 1]; // holds 80 characters
char WRITE; // write pointer in buffer
char READ;  // read pointer in buffer
};  

struct circularbuffrssi // circular recieve buffer for rssi pin
{
char BUFF[BUFFMAX + 1]; // holds 80 characters
char WRITE; // write pointer in buffer
char READ;  // read pointer in buffer	
}; //    


//------------------------------------------------------------------------------
void com0buffer(char ch) 
{
 

 if (RX0.WRITE < RX0.READ) // check if the WRITE pointer has wrapped around the circular buffer
  {//1                     // but the READ pointer has not yet wrapped around.
    if ((RX0.WRITE++) != RX0.READ) // check if the write pointer is 1 pos behind the read pointer
     {                             // if this happens the the write pointer has wrapped around circular buffer
      return; // buffer overrun, return
	 }
  }//1

 if (RX0.WRITE==BUFFMAX)
   {
    RX0.BUFF[RX0.WRITE]=ch; // store character to buffer
    RX0.WRITE=0;
    return;
   }

  RX0.BUFF[RX0.WRITE]=ch; // store character to buffer
  RX0.WRITE++;
 
} // end of function

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
char readcom0buffer(void)
{
char ch;
// check if the read pointer has cought up to the write pointer
if (RX0.WRITE == RX0.READ) 
    {
    return(0); // no characters in the buffer
    }//

if (RX0.READ==BUFFMAX) // check if we at the end of the circular buffer and do a wrap around
 {
 ch = RX0.BUFF[RX0.READ]; // get current character
 RX0.READ=0; // set read point to 0 to wrap around
 return(ch);
 }     // 

 ch = RX0.BUFF[RX0.READ];
 RX0.READ++;
 return (ch); // return character
  
} // end of funtion
// 

//------------------------------------------------------------------------------
void com1buffer(char ch) 
{
 if (RX1.WRITE < RX1.READ) // check if the WRITE pointer has wrapped around the circular buffer
  {//1                     // but the READ pointer has not yet wrapped around.
    if ((RX1.WRITE++) != RX1.READ) // check if the write pointer is 1 pos behind the read pointer
     {                             // if this happens the the write pointer has wrapped around circular buffer
      return; // buffer overrun, return
	 }
  }//1
  else
  if (RX1.WRITE==BUFFMAX)
   {
    RX1.BUFF[RX1.WRITE]=ch; // store character to buffer
    RX1.WRITE=0;
    return;
   }
  else
  { // 
  RX1.BUFF[RX1.WRITE]=ch; // store character to buffer
  RX1.WRITE++; //increment pointer
  } //

} // end of function

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
char readcom1buffer(void)
{
char ch;
// check if the read pointer has cought up to the write pointer
if (RX1.WRITE == RX1.READ) return(0); // no characters in the buffer
//
if (RX1.READ==BUFFMAX) // check if we at the end of the circular buffer and do a wrap around
 {
 ch = RX1.BUFF[RX1.READ]; // get current character
 RX1.READ=0; // set read point to 0 to wrap around
 return(ch);
 }     // 

  return (RX1.BUFF[RX1.READ++]); // return character
 
} // end of funtion
// 

//------------------------------------------------------------------------------
void com2buffer(char ch) 
{
 if (RX2.WRITE < RX2.READ) // check if the WRITE pointer has wrapped around the circular buffer
  {//1                     // but the READ pointer has not yet wrapped around.
    if ((RX2.WRITE++) != RX2.READ) // check if the write pointer is 1 pos behind the read pointer
     {                             // if this happens the the write pointer has wrapped around circular buffer
      return; // buffer overrun, return
	 }
  }//1
  else
  if (RX2.WRITE==BUFFMAX)
   {
    RX2.BUFF[RX2.WRITE]=ch; // store character to buffer
    RX2.WRITE=0;
    return;
   }
  else
  { // 
  RX2.BUFF[RX2.WRITE]=ch; // store character to buffer
  RX2.WRITE++; //increment pointer
  } //

} // end of function

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
char readcom2buffer(void)
{
char ch;
// check if the read pointer has cought up to the write pointer
if (RX2.WRITE == RX2.READ) return(0); // no characters in the buffer
//
if (RX2.READ==BUFFMAX) // check if we at the end of the circular buffer and do a wrap around
 {
 ch = RX2.BUFF[RX2.READ]; // get current character
 RX2.READ=0; // set read point to 0 to wrap around
 return(ch);
 }     // 

  return (RX2.BUFF[RX2.READ++]); // return character
 
} // end of funtion
// 




//------------------------------------------------------------------------------
// AutoLearn BaudRate
//------------------------------------------------------------------------------
int auto_learn_baud(void)
{
int baud;

CAL.BaudRate = 9600; // set BaudRate to 9600   
CAL.ComPort = 0;
CAL.BaudSet = 0;      /// default set to auto baud detection

baud = baudratescan();

if (baud)
{
CAL.BaudRate = baud; // update the cal structure with new baudrate
CAL.ComPort  = BOARD.COMPORT;   // save current com port
}
return(baud);
} // end of function


//------------------------------------------------------------------------------
// scan through baudrates and detect match
//------------------------------------------------------------------------------
int baudratescan(void)
{   // 
// baudrates below 9600 take a while to hunt for, so display the word "learn" on the display
//stringtodisplay("BAUD ",0); // send the word learn to scorebaord
if ( baudcheck(115200)) return (115200);  
  
if ( baudcheck(57600)) return (57600);  
  
if ( baudcheck(38400))  return (38400);  

if ( baudcheck(19200)) return (19200);

if ( baudcheck(9600))  return (9600); 
   
// baudrates below 9600 take a while to hunt for, so display the word "learn" on the display
string_todisplay("LEARN ",0); // send the word learn to scorebaord
 
if ( baudcheck(4800) )  return (4800);

if ( baudcheck(2400) )  return (2400);

if ( baudcheck(1200) )  return (1200);

if ( baudcheck(600) )    return (600); 

if ( baudcheck(300) )    return (300);

return(0); // return 0, no valid baudrates could be found
} // end of function


//--------------------------------------------------------------------------------------------------------------
// analyze comport to check if the the buadrate matches
//--------------------------------------------------------------------------------------------------------------
char baudcheck(int baud)
{
char ch=0;
int  frame=0;
char count=0;
char rxcount=0;
char digitcount=0;
char decimalcount=0;
char spacecount=0;
//char status;
//
UARTInit( COM0, baud );
UARTInit( COM2, baud ); 
UARTInit( COM3, baud );

timer(BAUDTIMEOUT,START); // start the timeout timer

while(1) // endless loop
{
WDTFeed( );          // watch dog timer  
status_light(STATUSLEARN);  // flash the status light fast to indicate autolearn
display_timer(START);
scan_system();             // breaks the loop to config 

ch = readcom0buffer();   // read the serial RX0 circular buffer

if (ch) // check for recieve flag
 {//1       
      rxcount++;
      ch=ch&0x7f; // strip 8th bit to ignore odd or even parity
    // a valid baudrate is detected with multiple spaces and digits in a row
     if (ch==32)
               {
               count++; // check for space
               spacecount++;
               }
    else if (isdigit(ch))// check for number
              { //              
              count++; 
              digitcount++;              
              }
      else if (isxdigit(ch))// check for number
              { //              
              count++; 
              digitcount++;              
              }     
    else if (ch=='.') // search for decimal point
	      {
              decimalcount++; 
	      digitcount++;
              }  //    
    else count = 0;
    // 
            
 }//1


switch(BOARD.COMPORT)
      { //UART0Status |= 0x100; /* Bit 9 as the CTI error */    
      case 0:{if (U0Status ==1) {frame++; U0Status =0;} break;}  
      case 2:{if (U2Status ==1) {frame++; U2Status =0;} break;}  
      case 3:{if (U3Status ==1) {frame++; U3Status =0;} break;}   
      default: break; // framing error
      }
   
if (rxcount < 20) // wait for 20 characters to allow system to settle
 {
  frame = 0;
  count = 0;  
  digitcount = 0;
  decimalcount = 0;
  spacecount = 0; 
 } //   

// a matched baudrate has multiple space characters in a row and at least 1 digit and no more than 1 decimal
  if  ((count > 4) && (digitcount > 0) && (decimalcount < 2)) 
  {
    return(1); // baudrate match  
  }

 if (frame > 2) 
  {  
    return(0);  // to many framing errors, abort  
  } //

  if (rxcount > 60) 
  {
    return(0); // timeout after 65 characters
  } // 

  if (timer(BAUDTIMEOUT,TIMEOUT))
   {
     return(0); // no comport activity, return 0
   } // 
} // end of while
  // 
}// end of function
// 
//---------------------------------------------------------------------------------------------------------
// setup the comports for the scoreboard
// there are two comports used in the scoreboard, depending if the radio is installed
//---------------------------------------------------------------------------------------------------------
void init_comports(void)
{
// char ch=0; 
// COM 0 is RS232 hard wired, baudrate is set depending on string  
// COM 1 is the MAXSTREAM radio connected at 9600N81
// COM 2 is current loop hard wired, baudrate is set depending on string
// COM 3 is RS485 hard wired, baudrate is set depending on string
// determine if we are using the radio or not, and setup correct RX interupt
  UARTInit( COM0, CAL.BaudRate );
  UARTInit( COM2, CAL.BaudRate );
  UARTInit( COM3, CAL.BaudRate );

 }  // end of function

// 
//---------------------------------------------------------------------------------------------------------
// send command to RADIO
//---------------------------------------------------------------------------------------------------------
char radio_cmd(char *cmd)
{
char i;
char cmdl;
char ch;

cmdl=strlen(cmd);

for (i=0;i!=cmdl;i++)
 {//1
  ch=*cmd++; // send character to COM1
  U1THR=ch;
  while (!UART1TxEmpty);
  UART1TxEmpty=0;
 }//1
// 
return(cmdl);
//  
}


//---------------------------------------------------------------------------------------------------------
// transmit string
//---------------------------------------------------------------------------------------------------------
char xmit_string(char *cmd)
{
char i;
char cmdl;
char ch;

cmdl=strlen(cmd);

for (i=0;i!=cmdl;i++)
 {//1
  ch=*cmd++; // send character to COM0
  U0THR=ch;
  while (!UART0TxEmpty);
  UART0TxEmpty=0;
 }//1
//  
return(cmdl);
}// 


//----------------------------------------------------------------------------------------------
//  check if there is a charater in the com port buffer
//----------------------------------------------------------------------------------------------
void scan_RX_buffer(void)
{
char ch;
  // 
   ch = readcom0buffer();   // read the serial RX0 circular buffer
      
   if(ch) // is there a character to read?
     {//2  
     scan_character(ch);             // scan the character into the scoreboard
     parse_traffic(ch);              // parse traffic light commands     
     com_timeout_timer(START);       // timeout regular    
     }//2
   else  
   { // timeout
     if (com_timeout_timer(TIMEOUT))  
         {//4
         
          if(BOARD.NODASHES == 1)
         {//2
         //error_message(0); // send error message that no baudrate could be detected
         string_todisplay("      ",0);    
         }
         else
         {       
        // error_message(3); // send error message that no baudrate could be detected
         string_todisplay("------",0);    
         }
         
	 }//4    
   } // time out
  
} // 
 


//----------------------------------------------------------------------------------------------
//  check if there is a charater in the com port buffer
//----------------------------------------------------------------------------------------------
void scan_RX1_buffer(void)
{
//char ch;
  // 
 // ch = readcom1buffer();   // read the serial RX0 circular buffer
  
  if(BOARD.RADIO ==0)
   {
   BOARD.RSSIFLAG = 0;   
   return;
   }
   
   
   if(BOARD.STOPCONTROL ==0)  
    {
    BOARD.RSSIFLAG = 0;    
    return;
    } // 
  
   
   
  if (BOARD.RSSIFLAG)
    {
      
   displayvalue(BOARD.RSSICAPTURE, 0, 0, "");
        
   if(BOARD.RSSICAPTURE > 25) // 30 for 50 calibration
         {  
         BOARD.DIGIT[0] = 'H';
         BOARD.DIGIT[1] = 'I';
         }
         else
         { //  
         BOARD.DIGIT[0] = 'L';
         BOARD.DIGIT[1] = 'O';  
         }
   
   BOARD.RSSIFLAG = 0;
   
    } // l 
   
} //  
 
//---------------------------------------------------------------------------------------------------------
// send binary command to RADIO via com1
//---------------------------------------------------------------------------------------------------------
char receive_timeout(char cmd)
{
char error=0;


if(cmd)
{// command mode
  
  if (command_timeout_timer(TIMEOUT))  
         {//4           
          error=1;
        }// 4 
         
  
  
}// command mode
else
{// regular mode
 
    if (com_timeout_timer(TIMEOUT))  
         {//4           
          error=1;       
        }// 4 
       
 // 
  
}// regular mode
  


 if (error)  
         {//4
               
         if(BOARD.NODASHES == 1)
         {//2
         string_todisplay("      ",0); // disable dashes           
         } //2 with dashes
         else 
         {//1
         string_todisplay("------",0);                  
         }//1
        
         // reset 
        BOARD.BLANK = 0;
        BOARD.OVER = 0; 
        BOARD.MOTION = 0;
        BOARD.BLINK = 0; 
        
        // reset annunciators
        BOARD.LB=0; // clear anunciators in alpha mode
	BOARD.KG=0;
	BOARD.GROSS=0;
	BOARD.NET=0;
       
        // reset 
        BOARD.VALUE = 0;
        BOARD.POWERSAVE=0;
                 
        BOARD.TIMESOUT = 1;        
        return(1);
        }// 4 
  
        BOARD.TIMESOUT = 0;
        return(0);
} // end of function







