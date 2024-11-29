#include "includes.h" 



//------------------------------------------------------------------------------
// build a string for output on COM port
//------------------------------------------------------------------------------

void stringformat(int port,int format)
{
  
long weight;           // displayed weight
char negative;         // polarity
char wstring[20];      // weight string
char tstring[20];      // weight string for tare
char outstring[30];    // final string for transmission 
char addstring[5];
char units[3];
char unit=' ';   // used for condec string
char mode[3];
char mode2=' ';        
char status=' ';
char status2[3];
char polarity;
char i;

char toledoA=0x20;      // toledo status A                                   
char toledoB=0x20;      // toledo status B
char toledoC=0x20;      // toledo status C
long deadzero=0;          // used for deadzero on TOLEDO string

unsigned int chk;      // checksum for TOLEDO

long dweight;   // displayed weight scratchpad 
long tweight;   // tare weight scratchpad

char motion=' ';   // used for motion flag for SMA format
char sl;    // used for stoplight 
char cz;    // used for center of zero flag
char address;

static unsigned char TIMERCOUNT[4];  // keep tracks for delay counts for inserting delays inbetween streams
int stimer;

// check if the COM port is IDLE ready for a new string to transmit
if (COM[port].IDLE==FALSE)  // if the com port is still busy tranmitting a string, wait for it to be idle
{ 
 return;   
}

if (CAL.STREAMDELAY!=0)  // check if we are not using stream delays 
{//1

      // depending on which port we are using,we have to select between stream timers for delays
      if (port==_PORT0)
      {
        stimer=STREAM_DELAY0;      
        
      }
      else if (port==_PORT1)
      {
        
        stimer=STREAM_DELAY1;
        
      }
    else if (port==_PORT2)
      {
        //
        stimer=STREAM_DELAY2;
        // 
      }
  

      if (timer(stimer,TIMEOUT))      // call the timer to keep track of delay counts used for inserting delays in between string tranmissions
      {
        
        timer(stimer,START);  // restart timer (0.25sec)
        TIMERCOUNT[port]++;   //increment timercount  (counts the number of 0.25sec tics)
      }
      
      
      if (TIMERCOUNT[port]>4)  // safety check, cannot got past 4
      {
        TIMERCOUNT[port]=0;  // reset to zero
      }
      
      
      if (CAL.STREAMDELAY==TIMERCOUNT[port])  // check if we have reached delay duration
      {
       TIMERCOUNT[port]=0;     
      }      
      else
      {
      return;  // have not reached adequate delay in between streams, return
      }

}//1

    
   //
   weight=SCALE.DIS_WEIGHT;   // get the displayed weight 
   format_string(weight, wstring,SCALE.DIS_DEC);   // format the weight string and insert decimals if any
   negative=SCALE.NEGATIVE;  
   address = CAL.ADDRESS + 48;
   sprintf(addstring,"%c%c%c",0x01,address,0x03);   
//special deadzero for NTEP conformance of displaying deadzeroes when the scale is at zero on auxialry equipment
// deadzeroes must be transmitted in the string when scale is at zero
if (weight==0)  // if the scale is zero check if we have to insert deadzeros
{  //1
 
      if (SCALE.DIS_DEADZERO==1)  // check for single deadzero
      {
        wstring[1]='0';
        wstring[2]=0;  // null terminate
      }
      else
      if (SCALE.DIS_DEADZERO==2)  // check for double deadzero
      {
        wstring[1]='0';
        wstring[2]='0';
        wstring[3]=0; // null terminate
      } 
   
}  //1
   

   // format units string for output ----------------------------------
   
   switch (SCALE.DIS_UNITS)
   {
     
   case _KG: {
             strcpy(units,"KG");
             unit='K';  // used in Condec string
             toledoB=toledoB | 0x10;  //fortmat Toledo bit mask for toledo string
             break;     
            }
           
            
   case _LB: {
             
             strcpy(units,"LB");
             unit='L';  // used in Condec string
             break;
            }         
     
   
   case  _G: {
             strcpy(units," G"); 
             toledoC=toledoC | 0x01;  //fortmat Toledo bit mask for toledo string
             unit='G';  // used in Condec string
             break;
             }
            
             
   case  _OZ: {
             strcpy(units,"OZ");
             unit='O';  // used in Condec string
             break;
             }     
   
 case  _TON: {
             strcpy(units," T");
             unit='T';  // used in Condec string
             break;
             }  
             
 case  _STON: {
             strcpy(units,"TN");
             unit='T';  // used in Condec string
             break;
             }  
             
             case  _LTON: {
             strcpy(units,"LT");
             unit='T';  // used in Condec string
             break;
             }                               
   }  // end of switch
 
   
   // format GROSS/NET mode for output ------------------------------
   
   if (SCALE.NETMODE)
   {
    strcpy(mode,"NT");
    mode2='N';
    toledoB=toledoB | 0x01;  //fortmat Toledo bit mask for toledo string
   }
   else
   {
     strcpy(mode,"GR");
     mode2='G';
   }
   

   
  // format  status character ----------------------------------------------------------------
  status=' ';  // initialize status character 
  polarity=' '; // initialize polarity 

   if (negative)
  {
   toledoB=toledoB | 0x02;  //fortmat Toledo bit mask for toledo string
   status='-'; 
   polarity='-';
  }
  
    if (SCALE.MOTION)
  {
    status='M'; 
    toledoB=toledoB | 0x08;  //fortmat Toledo bit mask for toledo string
    motion='M';  // set motion flag for SMA string
  }
  
  
   if (SCALE.OVER)
  {
    status='O'; 
    strcpy(wstring,"EEEEEE");
    toledoB=toledoB | 0x04;  //fortmat Toledo bit mask for toledo string
  }
  
  
              sprintf(outstring,"%c%c%7.7s %s %s %c%c%c",0x02,polarity,wstring,units,mode,status,0x0d,0x0a);   
           
      
  //-------------------------------------------------------------------------------------------
  // transmit string to selected com port
  if (port==_PORT0)   // send to port 0
  {
    xmitport(outstring,_PORT0);  
  }  
  
  if (port==_PORT1)   // send to port 0  // or port 1
  {
    xmitport(outstring,_PORT1); 
  }
 
  
 if (port==_PORT2)   // send to port 0  // or port 1
 {
  
   xmitport(outstring,_PORT2); 
    
 }
  
} // end of function 

//-----------------------------------------------------------------------------------------------
// process any commands recieved on COM port
//-----------------------------------------------------------------------------------------------
void process_serial_poll_commands( int port)
{
char command=0; 
char stream=0;  
 
if (port==_PORT0)
{  
command = poll_com_port1();  



            

 
 
 if (stream==1)  // check if we are streaming a string to the comport
   {//1
      
       SCALE.COM_STREAM0=1;  // set flag to transmit stream to comport0 (if enabled)
       
   }//1

}

if (port==_PORT1)
{  
command = poll_com_port2(); 


 if (stream==1)  // check if we are streaming a string to the comport
   {//1
      
      // SCALE.COM_STREAM0=1;  // set flag to transmit stream to comport0 (if enabled)
       SCALE.COM_STREAM1=1;  // set flag to transmit stream to comport1 (if enabled)   
     //  SCALE.COM_STREAM2=1;  // set flag to transmit stream to comport1 (if enabled) 
   }//1
 
}

if (port==_PORT2)
{  
command = poll_com_port3();


 if (stream==1)  // check if we are streaming a string to the comport
   {//1
      
   //    SCALE.COM_STREAM0=1;  // set flag to transmit stream to comport0 (if enabled)
   //    SCALE.COM_STREAM1=1;  // set flag to transmit stream to comport1 (if enabled)  
         SCALE.COM_STREAM2=1;  // set flag to transmit stream to comport1 (if enabled) 
   }//1
 
} 
   
} // end of function
//--------------------------------------------------------------------
// see if there  are characters to transmit
// Transmits 1 character out of the comport every time it is called until there is nothing to xmit.
//--------------------------------------------------------------------
void process_COM_xmit(int port)
{
char c;

  if (isCOMready(port)==false)  // check if com port is ready
  {
    return;  // comport register not empty, abort try again next time    
  }
     
  if (COM[port].X==COM[port].W)  // check if com xmit pointer = write pointer
  {
    COM[port].IDLE=TRUE;  // there is nothing more to transmit, indicate idle state
    return;   // nothing more to do, return
  }
  
   COM[port].IDLE=FALSE;  // indicate that we are not idle and we are still transmitting data from the buffer   
// we are processing data to transmit 
  
 if (COM[port].X==40)  // check if we have a wrap around condition
        {
          COM[port].X=0;  // set xmit pointer to zero for wraparounf of circular buffer     
        } 
        
        c = COM[port].BUFFER[COM[port].X];  // get the next character, 
        COM[port].X++;   //incrememnt xmit pointer
           
        write_COM_TX(c,port); // transmit character
       
 //           
}  // end of function

//----------------------------------------------------------------------------------------------------------------
// COM output stream depending on how it is setup in calibration
//----------------------------------------------------------------------------------------------------------------
void process_outputstream2(void)
{
    
  switch(CAL.COM3_MODE)
  {
 
  case 0:   // continuous stream output -----------------------------
          {
                 stringformat(_PORT2,CAL.STREAM3);  //  format output string and transmit 
                
                 break;
          }
  
  case 1:   //output stream on PRINT  -------------------------------
          {
                if (SCALE.COM_STREAM2==1) // check if lag is set for 1 time tranmit of stream
                 {
                //   SCALE.COM_STREAM0=0; // clear flag
                //   COM[_PORT0].IDLE=TRUE;  // there is nothing more to transmit, indicate idle state
                   stringformat(_PORT2,CAL.STREAM3);     //  format output string and transmit
                 }
                break;
          } 
          
          
 case 3:   //output stream on poll  -------------------------------
          {
                if (SCALE.COM_STREAM2==1) // check if lag is set for 1 time transmit of stream
                 {
               //    SCALE.COM_STREAM0=0; // clear flag
               //    COM[_PORT0].IDLE=TRUE;  // there is nothing more to transmit, indicate idle state
                   stringformat(_PORT2,CAL.STREAM3);     //  format output string and transmit
                 }
                break;
          }          
          
                
          
          
  case 4: //output stream on NO MOTION ----------------------------
           {
            if (SCALE.MOTION==0)
             {
               stringformat(_PORT2,CAL.STREAM3);     //  format output string and transmit
             }
           break;
          } 
          
          
     default: {
                SCALE.COM_STREAM2=0;  // set flag to transmit stream to comport0 (if enabled)
       //SCALE.COM_STREAM1=1;  // set flag to transmit stream to comport1 (if enabled)    
             break;
            }
     
 
    
  } // end of switch
  
  
  SCALE.COM_STREAM2=0;  // set flag to transmit stream to comport0 (if enabled) 
  
 }  


//----------------------------------------------------------------------------------------------------------------
// COM output stream depending on how it is setup in calibration
//----------------------------------------------------------------------------------------------------------------
void process_outputstream0(void)
{  
  switch(CAL.COM1_MODE)
  {
  
  case 0:   // continuous stream output -----------------------------
          {
                 stringformat(_PORT0,CAL.STREAM1);  //  format output string and transmit 
                
                 break;
          }
  
  case 1:   //output stream on PRINT  -------------------------------
          {
                if (SCALE.COM_STREAM0==1) // check if lag is set for 1 time tranmit of stream
                 {
                // SCALE.COM_STREAM0=0; // clear flag
                //   COM[_PORT0].IDLE=TRUE;  // there is nothing more to transmit, indicate idle state
                   stringformat(_PORT0,CAL.STREAM1);     //  format output string and transmit
                 }
                break;
          } 
          
          
 case 3:   //output stream on poll  -------------------------------
          {
                if (SCALE.COM_STREAM0==1) // check if lag is set for 1 time transmit of stream
                 {
               //    SCALE.COM_STREAM0=0; // clear flag
               //    COM[_PORT0].IDLE=TRUE;  // there is nothing more to transmit, indicate idle state
                   stringformat(_PORT0,CAL.STREAM1);     //  format output string and transmit
                 }
                break;
          }          
      
               
  case 4: //output stream on NO MOTION ----------------------------
           {
            if (SCALE.MOTION==0)
             {
               stringformat(_PORT0,CAL.STREAM1);     //  format output string and transmit
             }
           break;
          } 
          
          
     default: {
                SCALE.COM_STREAM0=0;  // set flag to transmit stream to comport0 (if enabled)
       //SCALE.COM_STREAM1=1;  // set flag to transmit stream to comport1 (if enabled)    
             break;
            }
    
  } // end of switch
  
  SCALE.COM_STREAM0=0; // clear flag 
} // end of function


//----------------------------------------------------------------------------------------------------------------
// COM output stream depending on how it is setup in calibration
//----------------------------------------------------------------------------------------------------------------
void process_outputstream1(void)
{  
  
  switch(CAL.COM2_MODE)
  {
  
  case 0:   // continuous stream output -----------------------------
          {
                 stringformat(_PORT1,CAL.STREAM2);  //  format output string and transmit 
                 break;
          }
  
  case 1:   //output stream on PRINT  -------------------------------
          {
                if (SCALE.COM_STREAM1==1)  // check if flag is set for 1 time tranmit of stream
                 {
                //  SCALE.COM_STREAM1=0; // clear flag
                //  COM[_PORT1].IDLE=TRUE;  // there is nothing more to transmit, indicate idle state
                  stringformat(_PORT1,CAL.STREAM2);     //  format output string and transmit
                 }
                break;
          } 
          
          
 case 3:   //output stream on poll  -------------------------------
          {
                if (SCALE.COM_STREAM1==1)  // check if flag is set for 1 time tranmit of stream
                 {
                //    SCALE.COM_STREAM1=0; // clear flag
                //   COM[_PORT1].IDLE=TRUE;  // there is nothing more to transmit, indicate idle state
                    stringformat(_PORT1,CAL.STREAM2);     //  format output string and transmit
                 }
                break;
          }
             
          
          
  case 4: //output stream on NO MOTION ----------------------------
           {
            if (SCALE.MOTION==0)
             {
               stringformat(_PORT1,CAL.STREAM2);     //  format output string and transmit
             }
           break;
          } 
          
          
     default: {
                SCALE.COM_STREAM1=0;  // set flag to transmit stream to comport0 (if enabled)
       //SCALE.COM_STREAM1=1;  // set flag to transmit stream to comport1 (if enabled)    
             break;
            }
     
 
    
  } // end of switch
  
 // 
 SCALE.COM_STREAM1=0; // clear flag
 
} // end of function

//
// 
//--------------------------------------------------------------------------------
void xmitport(char string[],int port)
{
  int i;
  
  
  if (COM[port].IDLE==FALSE)  // if the com port is still busy tranmitting a string, wait for it to be idle
{
 return;   
}
   
    COM[port].X=0;  // reset buffer pointers before spooling
    COM[port].W=0;
    
   if (port==_PORT0)
      {
    SCALE.COM_STREAM0=0; // clear flag 
      }
   
   if (port==_PORT1)
      {
      SCALE.COM_STREAM1=0; // clear flag   
      } 
   
   if (port==_PORT2)
      {
      SCALE.COM_STREAM2=0; // clear flag   
      }   
  
   i=0;
  while(string[i]!=0)
  {
  COM[port].BUFFER[COM[port].W++]=string[i++];  // add string to buffer to spooled to comport
      
  }   
  
  
} // end of function

//------------------------------------------------------------------------------------
//  read poll commands com 1
//  poll commands can either come from the comport as characters, or as external switch contacts
// wired to COM port as I/O, this is determined in the mode menu
//--- -----------------------------------------------------------------------------------
char poll_com_port1(void)
{
char commands[]="ZPCT"; // holds the com port poll command character
char string[]="      ";
char ch;
char mode; 
static char state; // static statemachine for external switch debouncing
static long debounce=0;  // static used for debouncing
char extswitch;

 mode = CAL.COM1_MODE;
 

return(0);

  
} // end of function 

//----------------------------------------------------------------------------------------

char poll_com_port3(void)
{
// 
char commands[]="ZPCT"; // holds the com port poll command character
char string[]="      ";
char ch;
char mode; 
char extswitch; 
char i;
static char state; // static statemachine for external switch debouncing
static char pos;
static long debounce=0;  // static used for debouncing
static char address;
static char idflag;

mode = CAL.COM3_MODE;


 
    
return(0);
// S
}

//------------------------------------------------------------------------------------
//  read poll commands com2
//  poll commands can either come from the comport as characters, or as external switch contacts
// wired to COM port as I/O, this is determined in the mode menu
//--- -----------------------------------------------------------------------------------
char poll_com_port2(void)
{
char commands[]="ZPCT"; // holds the com port poll command character
char string[]="      ";
char ch;
char mode; 
static char state; // static statemachine for external switch debouncing
static long debounce=0;  // static used for debouncing
char extswitch;

mode = CAL.COM2_MODE;



   return(0);
   
} // end of function 

//----------------------------------------------------------------------------------------
// Command mode
// Load in string until CR is received.  
// Parse Numeric, or parse AlpaNumeric
//-----------------------------------------------------------------------------------------------------------

void command_string(void)
{
  

                         
               
} // end of function

//##################################################################################################################
//##################################################################################################################
char read_COM_level(int port) // read the external push button switch status
{  


return(0);
}
